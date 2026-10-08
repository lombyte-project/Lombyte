#!/usr/bin/env python3
"""Regression tests for the repository-owned build and map scripts.

Run with:  python3 scripts/test_public_tools.py -v
"""

import contextlib
import hashlib
import importlib.util
import io
import json
import os
import shutil
import struct
import subprocess
import sys
import tempfile
import textwrap
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest import mock

ROOT = Path(__file__).resolve().parents[1]


def load_module(name: str, path: Path):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    # Register before exec: a module that declares a dataclass looks itself up
    # in sys.modules while the class body is evaluated, and raises
    # "NoneType object has no attribute '__dict__'" if it is not there yet.
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


class TreemapClassificationTests(unittest.TestCase):
    """generate_treemap.py must classify owners exactly as documented."""

    @classmethod
    def setUpClass(cls):
        cls.treemap = load_module(
            "rnc_generate_treemap", ROOT / "scripts" / "generate_treemap.py"
        )

    def _workspace(self, tmp: Path):
        (tmp / "src" / "textbin").mkdir(parents=True)
        (tmp / "src" / "textbin" / "promoted.c").write_text("/* promoted */\n")
        (tmp / "src" / "assembly" / "textbin").mkdir(parents=True)
        (tmp / "src" / "assembly" / "textbin" / "pending.c").write_text(
            "/* oracle */\n"
        )
        config = tmp / "config.yaml"
        config.write_text(
            textwrap.dedent(
                """
            segments:
              - [0, databin, elf_header]
              - name: main
                type: code
                start: 0x1000
                vram: 0x100080
                subsegments:
                  - [0x1000, textbin, vutext]
                  - [0x13300, c, textbin/promoted]
                  - [0x13400, c, assembly/textbin/pending]
                  - [0x13500, c, assembly/ee/legacy_exact]
                  - [0x13600, textbin, text_gap_end]
            """
            )
        )
        categories = tmp / "categories.json"
        categories.write_text(
            json.dumps(
                {
                    "exact_under_assembly": ["assembly/ee/legacy_exact"],
                    "intentional_asm": ["assembly/asm/leaf"],
                }
            )
        )
        return config, categories

    def test_measure_scores_persists_the_index(self):
        with tempfile.TemporaryDirectory() as name:
            tmp = Path(name)
            calls = []

            def fake_run(command, **kwargs):
                calls.append(command)
                return SimpleNamespace(
                    returncode=0,
                    stdout=json.dumps([{"unit": "assembly/x/y", "score": 42.0}]),
                    stderr="",
                )

            with mock.patch.object(self.treemap.subprocess, "run", fake_run):
                result = self.treemap.measure_scores(tmp, tmp / "idx.json")
        self.assertEqual(result, ({"assembly/x/y": 42.0}, tmp / "idx.json"))
        self.assertIn("--out", calls[0])
        self.assertIn(str(tmp / "idx.json"), calls[0])

    def test_categories_drive_tile_colors(self):
        with tempfile.TemporaryDirectory() as name:
            tmp = Path(name)
            config, categories = self._workspace(tmp)
            units = {
                u["owner"]: u["category"]
                for u in self.treemap.build_units(tmp, config, categories)
            }
        self.assertEqual(units["textbin/promoted"], "exact")
        self.assertEqual(units["assembly/ee/legacy_exact"], "exact")
        self.assertEqual(units["assembly/textbin/pending"], "pending")

    def test_missing_categories_file_is_not_fatal(self):
        with tempfile.TemporaryDirectory() as name:
            tmp = Path(name)
            config, _ = self._workspace(tmp)
            units = self.treemap.build_units(tmp, config, tmp / "missing.json")
        self.assertTrue(any(u["owner"] == "textbin/promoted" for u in units))

    def test_fuzzy_progress_weighting_and_exclusions(self):
        units = [
            {"owner": "exact/a", "size": 100, "category": "exact"},
            {"owner": "pending/b", "size": 300, "category": "pending"},
            {"owner": "pending/c", "size": 400, "category": "pending"},
            {"owner": "asm/d", "size": 10_000, "category": "asm"},
        ]
        # (100x100 + 300x50 + 400x0) / 800 bytes -> 31.25%.
        self.assertAlmostEqual(
            self.treemap.fuzzy_progress(units, {"pending/b": 50.0}), 31.25
        )
        # Matching units count 100% whatever a score file says, intentional asm
        # is outside the denominator, and scores are clamped to 0..100.
        self.assertAlmostEqual(
            self.treemap.fuzzy_progress(
                units, {"exact/a": 10.0, "pending/b": 150.0}
            ),
            (100 * 100 + 300 * 100) / 800,
        )
        self.assertEqual(self.treemap.fuzzy_progress([], {}), 0.0)

    def test_workspace_reports_c_fuzzy(self):
        with tempfile.TemporaryDirectory() as name:
            tmp = Path(name)
            config, categories = self._workspace(tmp)
            output = tmp / "map.svg"
            payload = json.dumps(
                [{"unit": "assembly/textbin/pending", "score": 50.0}]
            )
            completed = subprocess.CompletedProcess([], 0, stdout=payload, stderr="")
            with mock.patch.object(
                self.treemap.subprocess, "run", return_value=completed
            ), contextlib.redirect_stdout(io.StringIO()) as stdout:
                code = self.treemap.main(
                    [
                        "--repo",
                        str(tmp),
                        "--config",
                        str(config),
                        "--categories",
                        str(categories),
                        "--output",
                        str(output),
                        "--workspace",
                        str(tmp / "workspace"),
                    ]
                )
            self.assertEqual(code, 0)
            # 512 exact bytes / 768 recoverable -> 66.7% C_EXACT,
            # (512x100 + 256x50) / 768 -> 83.3% C_FUZZY.
            self.assertIn("C_FUZZY 83.3%", output.read_text())
            self.assertIn("C_FUZZY", stdout.getvalue())


class ProgressReportTests(unittest.TestCase):
    """gen_progress_report.py must count only C_EXACT units as matched."""

    @classmethod
    def setUpClass(cls):
        cls.report = load_module(
            "rnc_gen_progress_report", ROOT / "scripts" / "gen_progress_report.py"
        )

    def _repo(self, tmp: Path) -> Path:
        (tmp / "config" / "us").mkdir(parents=True)
        (tmp / "config" / "us" / "rnc1.us.yaml").write_text(
            textwrap.dedent(
                """
            segments:
              - name: main
                subsegments:
                  - [0x13300, c, textbin/promoted]
                  - [0x13400, c, assembly/textbin/pending]
                  - [0x13500, c, assembly/sdk/vu0_asm]
                  - [0x13600, c, sdk/library/exact_sdk]
                  - [0x13680, textbin, text_gap_end]
            """
            )
        )
        (tmp / "config" / "us" / "unit_categories.json").write_text(
            json.dumps({"intentional_asm": ["assembly/sdk/vu0_asm"]})
        )
        (tmp / "src" / "textbin").mkdir(parents=True)
        (tmp / "src" / "textbin" / "promoted.c").write_text("void Promoted(void) {\n}\n")
        (tmp / "src" / "sdk" / "library").mkdir(parents=True)
        (tmp / "src" / "sdk" / "library" / "exact_sdk.c").write_text("int sceExact(void) {\n}\n")
        (tmp / "src" / "assembly" / "textbin").mkdir(parents=True)
        (tmp / "src" / "assembly" / "textbin" / "pending.c").write_text(
            '#ifndef NON_MATCHING\nINCLUDE_ASM("x/FUN_00112480.s", FUN_00112480);\n#endif\n'
        )
        return tmp

    def _build(self, repo: Path, scores: dict) -> dict:
        with mock.patch.object(self.report, "REPO", repo):
            return self.report.build_report(scores)

    def test_only_exact_units_are_matched(self):
        with tempfile.TemporaryDirectory() as tmp:
            report = self._build(self._repo(Path(tmp)), {"textbin/pending": 100.0})
        units = {unit["name"]: unit for unit in report["units"]}
        self.assertEqual(set(units), {"game/unclassified", "sdk/library"})
        self.assertEqual(report["measures"]["total_code"], str(0x100 + 0x100 + 0x80))
        self.assertEqual(report["measures"]["matched_code"], str(0x100 + 0x80))
        game_group = units["game/unclassified"]
        pending = next(function for function in game_group["functions"]
                       if function["name"] == "FUN_00112480")
        self.assertEqual(len(game_group["functions"]), 2)
        self.assertEqual(game_group["measures"]["matched_code"], str(0x100))
        self.assertLess(pending["fuzzy_match_percent"], 100.0)
        self.assertEqual(units["sdk/library"]["metadata"], {
            "complete": True,
            "progress_categories": ["boot", "sdk"],
        })
        game = next(c for c in report["categories"] if c["id"] == "game")
        self.assertEqual(game["measures"]["complete_units"], 0)
        boot = next(c for c in report["categories"] if c["id"] == "boot")
        self.assertEqual(boot["measures"]["total_code"], report["measures"]["total_code"])

    def test_overlay_functions_join_the_report(self):
        """A shared/level function counts once; C in src/overlays is matched,
        an INCLUDE_ASM stub is pending; exe rows belong to the executable."""
        with tempfile.TemporaryDirectory() as tmp:
            repo = self._repo(Path(tmp))
            (repo / "config" / "overlays" / "us").mkdir(parents=True)
            (repo / "config" / "overlays" / "us" / "functions.tsv").write_text(
                "# name\tkind\tsize\tfingerprint\tlevels\tplaces\texe unit\n"
                "FUN_00112400\texe\t256\tf0\t19\t00:001EAA08\ttextbin/promoted\n"
                "FUN_L00_00200000\tshared\t64\tf1\t19\t00:00200000,01:00210000\t\n"
                "FUN_L03_00300000\tlevel\t32\tf2\t1\t03:00300000\t\n"
            )
            (repo / "config" / "overlays" / "us" / "levels.json").write_text(json.dumps({
                "schema": "rnc-overlay-levels-v1",
                "levels": [{"index": 3, "id": "level-03", "planet": "Kerwan", "description": "Metropolis"}],
            }))
            (repo / "src" / "overlays" / "shared").mkdir(parents=True)
            (repo / "src" / "overlays" / "shared" / "unit_00200000.c").write_text(
                "int FUN_L00_00200000(int a) {\n    return a + 1;\n}\n")
            (repo / "src" / "overlays" / "l03").mkdir(parents=True)
            (repo / "src" / "overlays" / "l03" / "unit_00300000.c").write_text(
                '#include "asm.h"\nINCLUDE_ASM("config/us/overlays/asm/FUN_L03_00300000.s", FUN_L03_00300000);\n')
            report = self._build(repo, {})
        units = {unit["name"]: unit for unit in report["units"]}
        self.assertIn("shared/unit_00200000", units)
        self.assertIn("level_03/unit_00300000", units)
        self.assertEqual(report["measures"]["total_code"], str(0x100 + 0x100 + 0x80 + 64 + 32))
        self.assertEqual(report["measures"]["matched_code"], str(0x100 + 0x80 + 64))
        shared = units["shared/unit_00200000"]
        self.assertEqual(shared["metadata"]["progress_categories"], ["overlays", "shared"])
        self.assertTrue(shared["metadata"]["complete"])
        level = units["level_03/unit_00300000"]
        self.assertEqual(level["metadata"]["progress_categories"], ["overlays", "levels", "level_03"])
        self.assertEqual(level["functions"][0]["fuzzy_match_percent"], 0.0)
        categories = {c["id"]: c["measures"] for c in report["categories"]}
        self.assertEqual(categories["overlays"]["total_code"], "96")
        self.assertEqual(categories["overlays"]["matched_code"], "64")
        self.assertEqual(categories["level_03"]["total_code"], "32")
        self.assertNotIn("level_00", categories)

    def test_overlay_one_line_c_definition_counts(self):
        """A definition whose body opens on the same line (the shape a tiny
        promoted function takes) is matched, not pending; the classifier used
        to require the parameter list to close the line."""
        with tempfile.TemporaryDirectory() as tmp:
            repo = self._repo(Path(tmp))
            (repo / "config" / "overlays" / "us").mkdir(parents=True)
            (repo / "config" / "overlays" / "us" / "functions.tsv").write_text(
                "# name\tkind\tsize\tfingerprint\tlevels\tplaces\texe unit\n"
                "FUN_L00_00200000\tshared\t40\tf1\t19\t00:00200000\t\n"
            )
            (repo / "config" / "overlays" / "us" / "levels.json").write_text(json.dumps({
                "schema": "rnc-overlay-levels-v1",
                "levels": [],
            }))
            (repo / "src" / "overlays" / "shared").mkdir(parents=True)
            (repo / "src" / "overlays" / "shared" / "unit_00200000.c").write_text(
                "int FUN_L00_00200000(float a) { return a <= 1.0f; }\n")
            report = self._build(repo, {})
        units = {unit["name"]: unit for unit in report["units"]}
        self.assertIn("shared/unit_00200000", units)
        self.assertTrue(units["shared/unit_00200000"]["metadata"]["complete"])
        shared = {c["id"]: c["measures"] for c in report["categories"]}["shared"]
        self.assertEqual(shared["total_code"], "40")
        self.assertEqual(shared["matched_code"], "40")

    def test_promotion_shows_in_the_regenerated_report_and_comment(self):
        """CI regenerates the report from the tree; the PR comment lists a
        promoted function and C_FUZZY survives through fuzzy_scores.json."""
        comment = load_module("rnc_progress_comment", ROOT / "scripts" / "progress_comment.py")
        with tempfile.TemporaryDirectory() as tmp:
            repo = self._repo(Path(tmp))
            base, head = repo / "base.json", repo / "head.json"
            scores = repo / "progress" / "fuzzy_scores.json"
            scores.parent.mkdir(parents=True, exist_ok=True)
            scores.write_text(json.dumps({"schema": "rnc-fuzzy-scores-v1",
                                          "scores": {"0x00013400": 42.5}}))
            with mock.patch.object(self.report, "REPO", repo), \
                    contextlib.redirect_stdout(io.StringIO()):
                self.assertEqual(self.report.main(["--report", str(base), "--scores", str(scores)]), 0)
                (repo / "src" / "assembly" / "textbin" / "pending.c").unlink()
                (repo / "src" / "textbin" / "pending.c").write_text("void P(void) {\n}\n")
                (repo / "config" / "us" / "rnc1.us.yaml").write_text(
                    (repo / "config" / "us" / "rnc1.us.yaml").read_text().replace(
                        "assembly/textbin/pending", "textbin/pending"))
                self.assertEqual(self.report.main(["--report", str(head), "--scores", str(scores)]), 0)
            old, new = json.loads(base.read_text()), json.loads(head.read_text())
            fuzzy = [f["fuzzy_match_percent"] for u in old["units"] for f in u["functions"]
                     if f["metadata"]["virtual_address"] == str(0x13400)]
            self.assertEqual(fuzzy, [42.5])
            self.assertLess(int(old["measures"]["matched_code"]), int(new["measures"]["matched_code"]))
            text = comment.render(old, new, "main")
            self.assertTrue(text.startswith(comment.MARKER))
            self.assertIn("1 newly matched function", text)
            self.assertIn("`P`", text)
            self.assertIn("No report for main yet", comment.render({}, new, "main"))


class OverlaySourceStateTests(unittest.TestCase):
    """overlay_units.source_state must count a C-named function bound to its label."""

    def test_named_definition_counts_as_its_label(self):
        units = load_module("rnc_overlay_units", ROOT / "scripts" / "overlay_units.py")
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "src" / "overlays" / "l14").mkdir(parents=True)
            (root / "src" / "overlays" / "l14" / "a.c").write_text(textwrap.dedent("""
                extern int hero_set_state(int, int) __asm__("FUN_L14_0022ff58");
                int hero_set_state(int a, int b) {
                    return a + b;
                }
                void FUN_L14_00230000(void) {
                }
                INCLUDE_ASM("asm/FUN_L14_00230100.s", FUN_L14_00230100);
                extern void other(void) __asm__("FUN_L14_00230200");
            """))
            stubs, c = units.source_state(root)
        self.assertIn("FUN_L14_0022ff58", c)
        self.assertIn("FUN_L14_00230000", c)
        self.assertNotIn("FUN_L14_00230200", c)
        self.assertEqual(list(stubs), ["FUN_L14_00230100"])


class BaselineGuardTests(unittest.TestCase):
    """verify-baseline.sh must never delete a non-baseline directory."""

    def test_refuses_foreign_directory(self):
        with tempfile.TemporaryDirectory() as name:
            foreign = Path(name) / "user-data"
            foreign.mkdir()
            keep = foreign / "important.txt"
            keep.write_text("do not delete\n")
            env = dict(os.environ)
            env["BASELINE_ROOT"] = str(foreign)
            proc = subprocess.run(
                ["bash", str(ROOT / "verify-baseline.sh")],
                capture_output=True,
                text=True,
                env=env,
                cwd=ROOT,
            )
            self.assertNotEqual(proc.returncode, 0)
            self.assertIn("refusing to remove", proc.stderr)
            self.assertTrue(keep.is_file())
            self.assertEqual(keep.read_text(), "do not delete\n")

    def test_accepts_marked_baseline_directory(self):
        with tempfile.TemporaryDirectory() as name:
            marked = Path(name) / "baseline"
            marked.mkdir()
            (marked / ".rnc-baseline-root").write_text("")
            env = dict(os.environ)
            env["BASELINE_ROOT"] = str(marked)
            # Stop after the guard: an intentionally invalid venv makes the
            # script die at the next check instead of building.
            env["VENV"] = str(Path(name) / "missing-venv")
            proc = subprocess.run(
                ["bash", str(ROOT / "verify-baseline.sh")],
                capture_output=True,
                text=True,
                env=env,
                cwd=ROOT,
            )
            self.assertNotIn("refusing to remove", proc.stderr)
            self.assertIn("missing virtual environment", proc.stderr)

    def test_refuses_the_checkout_and_its_parents(self):
        with tempfile.TemporaryDirectory() as name:
            tmp = Path(name)
            checkout = tmp / "checkout"
            checkout.mkdir()
            script = checkout / "verify-baseline.sh"
            script.write_text((ROOT / "verify-baseline.sh").read_text())
            for baseline in (checkout, tmp):
                env = dict(os.environ)
                env["BASELINE_ROOT"] = str(baseline)
                proc = subprocess.run(
                    ["bash", str(script)],
                    capture_output=True,
                    text=True,
                    env=env,
                    cwd=checkout,
                )
                self.assertNotEqual(proc.returncode, 0)
                self.assertIn("must not be the checkout or a parent", proc.stderr)
            self.assertTrue(checkout.is_dir())


class PatchedProfileTests(unittest.TestCase):
    """check-patched-profile.py must flag profiles built from other patches."""

    @classmethod
    def setUpClass(cls):
        cls.checker = load_module(
            "check_patched_profile", ROOT / "scripts" / "check-patched-profile.py"
        )

    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        self.patch = self.root / "patched-ee-gcc.patch"
        self.patch.write_text("diff --git a/x b/x\n")
        self.profile = self.root / "profile"
        self.profile.mkdir()
        (self.profile / "xgcc").write_text("")

    def write_provenance(self, digest):
        (self.profile / "provenance.json").write_text(
            json.dumps({"schema": "rnc-patched-ee-gcc-v1", "patch_sha256": digest})
        )

    def run_check(self):
        with contextlib.redirect_stdout(io.StringIO()) as captured:
            code = self.checker.check(self.profile, self.patch)
        return code, captured.getvalue()

    def test_matching_patch_is_ok(self):
        self.write_provenance(self.checker.sha256(self.patch))
        code, output = self.run_check()
        self.assertEqual(code, 0)
        self.assertIn("ok", output)

    def test_other_patch_is_stale(self):
        self.write_provenance("d" * 64)
        code, output = self.run_check()
        self.assertEqual(code, 1)
        self.assertIn("warning", output)
        self.assertIn("build-patched-toolchain.py", output)

    def test_missing_provenance_is_unverifiable(self):
        code, output = self.run_check()
        self.assertEqual(code, 2)
        self.assertIn("no provenance.json", output)


class RebuildIsoExtentTests(unittest.TestCase):
    """rebuild-iso.py must locate, bounds-check and patch the boot extent."""

    SECTOR = 2048
    ROOT_LBA = 20
    SCUS_LBA = 30
    SCUS_SIZE = 4096

    @classmethod
    def setUpClass(cls):
        cls.rebuild = load_module("rnc_rebuild_iso", ROOT / "scripts/rebuild-iso.py")

    @classmethod
    def _record(cls, name: str, lba: int, size: int) -> bytes:
        encoded = name.encode("latin1")
        pad = b"\x00" if len(encoded) % 2 == 0 else b""
        record = bytearray(33 + len(encoded) + len(pad))
        record[0] = len(record)
        struct.pack_into("<I", record, 2, lba)
        struct.pack_into("<I", record, 10, size)
        record[32] = len(encoded)
        record[33 : 33 + len(encoded)] = encoded
        record[33 + len(encoded) :] = pad
        return bytes(record)

    @classmethod
    def _mini_iso(
        cls, scus_lba: int = SCUS_LBA, scus_size: int = SCUS_SIZE, prefix: bytes = b""
    ) -> bytearray:
        buf = bytearray(cls.SECTOR * 40)  # 80 KiB; SCUS at LBA 30 ends at 64 KiB
        buf[0x8000 : 0x8000 + 6] = b"\x01CD001"
        struct.pack_into("<I", buf, 0x8000 + 156 + 2, cls.ROOT_LBA)
        struct.pack_into("<I", buf, 0x8000 + 156 + 10, 512)
        off = cls.ROOT_LBA * cls.SECTOR
        records = (
            prefix
            + cls._record("SYSTEM.CNF;1", 19, 59)
            + b"\x00"
            + cls._record("SCUS_971.99;1", scus_lba, scus_size)
        )
        buf[off : off + len(records)] = records
        return buf

    def test_finds_boot_extent(self):
        iso = self._mini_iso()
        self.assertEqual(
            self.rebuild.find_boot_extent(bytes(iso)), (self.SCUS_LBA, self.SCUS_SIZE)
        )

    def test_walks_even_length_names_and_padding(self):
        iso = self._mini_iso(prefix=self._record("AB;1", 10, 1))
        self.assertEqual(
            self.rebuild.find_boot_extent(bytes(iso)), (self.SCUS_LBA, self.SCUS_SIZE)
        )

    def test_rejects_missing_name(self):
        with self.assertRaises(SystemExit):
            self.rebuild.find_boot_extent(
                bytes(self._mini_iso()), want_name="NOPE.BIN;1"
            )

    def test_rejects_non_iso_image(self):
        with self.assertRaises(SystemExit):
            self.rebuild.find_boot_extent(bytes(self.SECTOR * 17))

    def test_refuses_extent_beyond_eof(self):
        iso = self._mini_iso(scus_lba=1000)  # LBA 1000 > 40-sector image
        with tempfile.TemporaryDirectory() as name:
            iso_path = Path(name) / "bad.iso"
            elf_path = Path(name) / "built.elf"
            iso_path.write_bytes(bytes(iso))
            elf_path.write_bytes(b"\x00" * self.SCUS_SIZE)
            with self.assertRaises(SystemExit) as raised:
                with mock.patch.object(
                    sys,
                    "argv",
                    [
                        "rebuild-iso.py",
                        "--iso",
                        str(iso_path),
                        "--elf",
                        str(elf_path),
                        "--out",
                        str(Path(name) / "out.iso"),
                    ],
                ):
                    with contextlib.redirect_stdout(io.StringIO()):
                        self.rebuild.main()
            self.assertIn("outside the ISO image", str(raised.exception))

    def test_patches_identical_extent_end_to_end(self):
        iso = self._mini_iso()
        extent = bytes(
            iso[
                self.SCUS_LBA * self.SECTOR : self.SCUS_LBA * self.SECTOR
                + self.SCUS_SIZE
            ]
        )
        with tempfile.TemporaryDirectory() as name:
            iso_path = Path(name) / "game.iso"
            elf_path = Path(name) / "built.elf"
            out_path = Path(name) / "rebuilt.iso"
            iso_path.write_bytes(bytes(iso))
            elf_path.write_bytes(extent)
            argv = [
                "rebuild-iso.py",
                "--iso",
                str(iso_path),
                "--elf",
                str(elf_path),
                "--out",
                str(out_path),
            ]
            stdout = io.StringIO()
            with (
                mock.patch.object(sys, "argv", argv),
                mock.patch.object(
                    self.rebuild, "RETAIL_SHA", hashlib.sha256(extent).hexdigest()
                ),
            ):
                with contextlib.redirect_stdout(stdout):
                    code = self.rebuild.main()
            self.assertEqual(code, 0)
            self.assertIn("byte-identical", stdout.getvalue())
            self.assertEqual(out_path.read_bytes(), bytes(iso))


class UnitListHelpersTests(unittest.TestCase):
    """rnc_units.py must parse the config and guard conventions exactly."""

    @classmethod
    def setUpClass(cls):
        cls.units = load_module("rnc_units", ROOT / "scripts" / "rnc_units.py")
        cls.treemap = load_module(
            "rnc_generate_treemap", ROOT / "scripts" / "generate_treemap.py"
        )

    def test_parse_config_rows(self):
        with tempfile.TemporaryDirectory() as name:
            config = Path(name) / "config.yaml"
            config.write_text(
                textwrap.dedent(
                    """
                segments:
                  - [0, databin, elf_header]
                  - name: main
                    type: code
                    subsegments:
                      - [0x1000, textbin, vutext]
                      - [0x13300, c, textbin/exact_one]
                      - [0x13400, c, assembly/textbin/pending_one]
                      - [0x13500, textbin, text_gap_end]
                """
                )
            )
            rows = self.units.parse_config_rows(config)
            self.assertEqual(
                rows,
                [
                    (0x1000, "textbin", "vutext"),
                    (0x13300, "c", "textbin/exact_one"),
                    (0x13400, "c", "assembly/textbin/pending_one"),
                    (0x13500, "textbin", "text_gap_end"),
                ],
            )
            units = self.units.configured_units(config)
        self.assertEqual(
            units,
            [
                {"owner": "textbin/exact_one", "address": 0x13300, "size": 0x100},
                {
                    "owner": "assembly/textbin/pending_one",
                    "address": 0x13400,
                    "size": 0x100,
                },
            ],
        )

    def test_classify_units(self):
        with tempfile.TemporaryDirectory() as name:
            repo = Path(name)
            (repo / "config" / "us").mkdir(parents=True)
            (repo / "src" / "textbin").mkdir(parents=True)
            (repo / "src" / "assembly" / "textbin").mkdir(parents=True)
            (repo / "src" / "textbin" / "exact_one.c").write_text("/* promoted */\n")
            (repo / "src" / "assembly" / "textbin" / "pending_one.c").write_text(
                "/* oracle */\n"
            )
            (repo / "config" / "us" / "rnc1.us.yaml").write_text(
                textwrap.dedent(
                    """
                subsegments:
                  - [0x1000, c, textbin/exact_one]
                  - [0x1100, c, assembly/textbin/pending_one]
                  - [0x1200, c, assembly/textbin/intentional_asm]
                  - [0x1300, textbin, text_gap_end]
                """
                )
            )
            (repo / "config" / "us" / "unit_categories.json").write_text(
                json.dumps({"intentional_asm": ["assembly/textbin/intentional_asm"]})
            )
            units = {u["owner"]: u["category"] for u in self.units.classify_units(repo)}
        self.assertEqual(units["textbin/exact_one"], "exact")
        self.assertEqual(units["assembly/textbin/pending_one"], "pending")
        self.assertEqual(units["assembly/textbin/intentional_asm"], "asm")

    def test_non_label_asm(self):
        cases = {
            'void f(void) __asm__("FUN_00202d10");\n': False,
            'extern int g __asm__("D_001604F0"); /* asm("x" : : ) */\n': False,
            'void f(void) { __asm__ __volatile__("" ::: "memory"); }\n': True,
            '__asm__(".extern D_0015FD64, 4");\n': True,
            'void f(void) { register int x asm("v1") = 0; }\n': True,
            'void f(void) { register int x __asm__("$5"); }\n': True,
            'void f(int p) { __asm__ volatile("" : "+r"(p)); }\n': True,
            'int f(void) { int r; __asm__("mfc0 %0, $12" : "=r"(r)); return r; }\n': True,
        }
        with tempfile.TemporaryDirectory() as name:
            for index, (text, expected) in enumerate(cases.items()):
                source = Path(name) / f"u{index}.c"
                source.write_text(text)
                with self.subTest(text=text):
                    self.assertEqual(self.units.non_label_asm(source), expected)
                    self.assertEqual(self.treemap.non_label_asm(source), expected)

    def test_classify_units_asm_body_is_not_exact(self):
        with tempfile.TemporaryDirectory() as name:
            repo = Path(name)
            (repo / "config" / "us").mkdir(parents=True)
            (repo / "src" / "textbin").mkdir(parents=True)
            (repo / "src" / "textbin" / "barrier.c").write_text(
                'void f(void) { __asm__ __volatile__("" ::: "memory"); }\n')
            (repo / "config" / "us" / "rnc1.us.yaml").write_text(
                "  - [0x1000, c, textbin/barrier]\n  - [0x1100, textbin, text_gap_end]\n")
            units = {u["owner"]: u["category"] for u in self.units.classify_units(repo)}
        self.assertEqual(units["textbin/barrier"], "pending")

    def test_split_oracle_guard_plain_source(self):
        self.assertEqual(self.units.split_oracle_guard("int f(void) { return 1; }\n"), (False, None))

    def test_split_oracle_guard_body(self):
        text = textwrap.dedent(
            """\
            #include "types.h"
            #ifndef NON_MATCHING
            INCLUDE_ASM("oracle.s", FUN_00123456);
            #else
            int FUN_00123456(void) {
                return 1;
            }
            #endif /* NON_MATCHING */
            """
        )
        has_guard, body = self.units.split_oracle_guard(text)
        self.assertTrue(has_guard)
        self.assertIn("return 1;", body)
        self.assertNotIn("INCLUDE_ASM", body)

    def test_split_oracle_guard_without_body(self):
        text = "#ifndef NON_MATCHING\nINCLUDE_ASM(\"oracle.s\", FUN_00123456);\n#endif\n"
        self.assertEqual(self.units.split_oracle_guard(text), (True, None))

    def test_split_oracle_guard_nested_directives(self):
        text = textwrap.dedent(
            """\
            #ifndef NON_MATCHING
            INCLUDE_ASM("oracle.s", FUN_00123456);
            #else
            #ifdef DEBUG
            #endif
            int FUN_00123456(void) {
                return 1;
            }
            #endif /* NON_MATCHING */
            """
        )
        has_guard, body = self.units.split_oracle_guard(text)
        self.assertTrue(has_guard)
        self.assertIn("return 1;", body)
        self.assertIn("#ifdef DEBUG", body)

    def test_default_workspace_is_project_local(self):
        repo = Path("/tmp/example-checkout")
        with mock.patch.dict(os.environ, {}, clear=False):
            os.environ.pop("BASELINE_ROOT", None)
            self.assertEqual(
                self.units.default_workspace(repo), repo / "build" / "baseline"
            )
        with mock.patch.dict(os.environ, {"BASELINE_ROOT": "/tmp/custom-ws"}):
            self.assertEqual(
                self.units.default_workspace(repo), Path("/tmp/custom-ws")
            )

    def test_workspace_problem_refuses_checkout_and_parents(self):
        with tempfile.TemporaryDirectory() as name:
            repo = Path(name) / "checkout"
            (repo / "build" / "baseline").mkdir(parents=True)
            self.assertIn(
                "checkout or one of its parents",
                self.units.workspace_problem(repo, repo),
            )
            self.assertIn(
                "checkout or one of its parents",
                self.units.workspace_problem(repo.parent, repo),
            )
            self.assertIn(
                "is not a baseline workspace",
                self.units.workspace_problem(repo / "build" / "baseline", repo),
            )

    def test_workspace_problem_accepts_a_marked_workspace(self):
        with tempfile.TemporaryDirectory() as name:
            repo = Path(name) / "checkout"
            workspace = repo / "build" / "baseline"
            (workspace / "config" / "us").mkdir(parents=True)
            (workspace / ".rnc-baseline-root").write_text("")
            (workspace / "config" / "us" / "build.ninja").write_text("")
            (workspace / "tools" / "objdiff").mkdir(parents=True)
            (workspace / "tools" / "objdiff" / "objdiff-cli").write_text("")
            self.assertIsNone(self.units.workspace_problem(workspace, repo))

    def test_path_inside(self):
        with tempfile.TemporaryDirectory() as name:
            root = Path(name)
            self.assertTrue(self.units.path_inside(root / "src" / "x.c", root))
            self.assertFalse(self.units.path_inside(root.parent / "x.c", root))

    def test_unsafe_unit_name(self):
        self.assertFalse(self.units.unsafe_unit_name("assembly/textbin/fun_00112380"))
        self.assertFalse(self.units.unsafe_unit_name("textbin/fun_002172c0"))
        self.assertFalse(self.units.unsafe_unit_name("_dtoa_r"))
        self.assertTrue(self.units.unsafe_unit_name("textbin/../../../../escape"))
        self.assertTrue(self.units.unsafe_unit_name("/etc/passwd"))
        self.assertTrue(self.units.unsafe_unit_name("textbin/foo; rm -rf /"))
        self.assertTrue(self.units.unsafe_unit_name("textbin/foo bar"))
        self.assertTrue(self.units.unsafe_unit_name(""))

    def test_oracle_fallback_units(self):
        with tempfile.TemporaryDirectory() as name:
            workspace = Path(name) / "ws"
            (workspace / "config" / "us").mkdir(parents=True)
            self.assertEqual(self.units.oracle_fallback_units(workspace), set())
            (workspace / "config" / "us" / "oracle-fallback-units.json").write_text(
                json.dumps(
                    {
                        "schema": "rnc-oracle-fallback-v1",
                        "units": ["sdk/time/bcd_to_time", "textbin/fun_002133d0"],
                    }
                )
            )
            self.assertEqual(
                self.units.oracle_fallback_units(workspace),
                {"sdk/time/bcd_to_time", "textbin/fun_002133d0"},
            )


class OracleMaterializerGuardTests(unittest.TestCase):
    """materialize-textbin-oracles.py must reject names that escape the tree."""

    @classmethod
    def setUpClass(cls):
        cls.materializer = load_module(
            "rnc_materialize_oracles",
            ROOT / "scripts" / "materialize-textbin-oracles.py",
        )

    def _fixture(self, tmp: Path, unit: str):
        config = tmp / "config.yaml"
        config.write_text(
            "segments:\n"
            "  - name: main\n"
            "    type: code\n"
            "    start: 0x1000\n"
            "    vram: 0x100080\n"
            "    subsegments:\n"
            f"          - [0x13300, c, {unit}]\n"
            "          - [0x13328, textbin, gap_after]\n"
        )
        function_map = tmp / "map.csv"
        function_map.write_text(
            "Name,Start,End,Size\nFUN_00112380,0x00112380,0x001123A8,40\n"
        )
        return config, function_map

    def test_rejects_unsafe_unit_names(self):
        with tempfile.TemporaryDirectory() as name:
            tmp = Path(name)
            config, function_map = self._fixture(tmp, "textbin/../../../../escape")
            with self.assertRaises(ValueError):
                self.materializer.configured_textbin_functions(config, function_map)

    def test_rejects_unsafe_symbol_names(self):
        with tempfile.TemporaryDirectory() as name:
            tmp = Path(name)
            config, _ = self._fixture(tmp, "assembly/textbin/fun_00112380")
            function_map = tmp / "map.csv"
            function_map.write_text(
                "Name,Start,End,Size\n../evil,0x00112380,0x001123A8,40\n"
            )
            with self.assertRaises(ValueError):
                self.materializer.configured_textbin_functions(config, function_map)

    def test_accepts_valid_names(self):
        with tempfile.TemporaryDirectory() as name:
            tmp = Path(name)
            config, function_map = self._fixture(tmp, "assembly/textbin/fun_00112380")
            rows = self.materializer.configured_textbin_functions(config, function_map)
        self.assertEqual(len(rows), 1)
        self.assertEqual(rows[0]["symbol"], "FUN_00112380")


class ListFunctionsTests(unittest.TestCase):
    """list-functions.py must filter, sort and print the pending work list."""

    @classmethod
    def setUpClass(cls):
        cls.lister = load_module(
            "rnc_list_functions", ROOT / "scripts" / "list-functions.py"
        )

    def _repo(self, tmp: Path):
        (tmp / "config" / "us").mkdir(parents=True)
        (tmp / "src" / "textbin").mkdir(parents=True)
        (tmp / "src" / "assembly" / "textbin").mkdir(parents=True)
        (tmp / "src" / "textbin" / "exact_one.c").write_text("/* promoted */\n")
        (tmp / "src" / "assembly" / "textbin" / "with_c.c").write_text(
            textwrap.dedent(
                """\
                #ifndef NON_MATCHING
                INCLUDE_ASM("oracle.s", FUN_00123456);
                #else
                int FUN_00123456(void) {
                    return 1;
                }
                #endif /* NON_MATCHING */
                """
            )
        )
        (tmp / "src" / "assembly" / "textbin" / "no_c.c").write_text(
            "#ifndef NON_MATCHING\nINCLUDE_ASM(\"oracle.s\", FUN_00123457);\n#endif\n"
        )
        (tmp / "src" / "assembly" / "textbin" / "almost.c").write_text(
            textwrap.dedent(
                """\
                #ifndef NON_MATCHING
                INCLUDE_ASM("oracle.s", FUN_00123458);
                #else
                int FUN_00123458(void) {
                    return 2;
                }
                #endif /* NON_MATCHING */
                """
            )
        )
        (tmp / "config" / "us" / "rnc1.us.yaml").write_text(
            textwrap.dedent(
                """
            segments:
              - [0, databin, elf_header]
              - name: main
                type: code
                subsegments:
                  - [0x1000, c, textbin/exact_one]
                  - [0x1100, c, assembly/textbin/with_c]
                  - [0x1200, c, assembly/textbin/almost]
                  - [0x1300, c, assembly/textbin/no_c]
                  - [0x1400, textbin, text_gap_end]
            """
            )
        )
        (tmp / "config" / "us" / "unit_categories.json").write_text(json.dumps({}))
        (tmp / "config" / "us" / "recovered_names.json").write_text(json.dumps({}))

    def test_default_lists_only_units_with_a_c_body(self):
        with tempfile.TemporaryDirectory() as name:
            tmp = Path(name)
            self._repo(tmp)
            stdout = io.StringIO()
            with (
                mock.patch.object(self.lister, "ROOT", tmp),
                contextlib.redirect_stdout(stdout),
            ):
                code = self.lister.main([])
        self.assertEqual(code, 0)
        output = stdout.getvalue()
        self.assertIn("assembly/textbin/with_c", output)
        self.assertNotIn("assembly/textbin/no_c", output)
        self.assertNotIn("textbin/exact_one", output)

    def test_all_and_json_include_units_without_a_c_body(self):
        with tempfile.TemporaryDirectory() as name:
            tmp = Path(name)
            self._repo(tmp)
            stdout = io.StringIO()
            with (
                mock.patch.object(self.lister, "ROOT", tmp),
                contextlib.redirect_stdout(stdout),
            ):
                code = self.lister.main(["--all", "--json"])
        self.assertEqual(code, 0)
        listed = json.loads(stdout.getvalue())
        self.assertEqual(
            [entry["unit"] for entry in listed],
            [
                "assembly/textbin/almost",
                "assembly/textbin/no_c",
                "assembly/textbin/with_c",
            ],
        )
        self.assertFalse(listed[1]["has_c_body"])
        self.assertTrue(listed[2]["has_c_body"])

    def _workspace(self, tmp: Path) -> Path:
        workspace = tmp / "ws"
        (workspace / "config" / "us").mkdir(parents=True)
        (workspace / ".rnc-baseline-root").write_text("")
        (workspace / "config" / "us" / "build.ninja").write_text("")
        (workspace / "tools" / "objdiff").mkdir(parents=True)
        (workspace / "tools" / "objdiff" / "objdiff-cli").write_text("")
        return workspace

    def test_score_sorts_by_match_percentage(self):
        with tempfile.TemporaryDirectory() as name:
            tmp = Path(name)
            self._repo(tmp)
            workspace = self._workspace(tmp)
            scores = {
                "assembly/textbin/with_c": 42.0,
                "assembly/textbin/almost": 99.5,
            }

            def fake_run(command, **kwargs):
                owner = command[2]
                return SimpleNamespace(
                    returncode=0,
                    stdout=json.dumps({"text_match_percent": scores[owner]}),
                    stderr="",
                )

            stdout = io.StringIO()
            with (
                mock.patch.object(self.lister, "ROOT", tmp),
                mock.patch.object(self.lister.subprocess, "run", fake_run),
                contextlib.redirect_stdout(stdout),
                contextlib.redirect_stderr(io.StringIO()),
            ):
                code = self.lister.main(
                    ["--score", "--limit", "0", "--workspace", str(workspace)]
                )
        self.assertEqual(code, 0)
        output = stdout.getvalue()
        self.assertIn("99.5%", output)
        self.assertIn("42.0%", output)
        self.assertLess(output.index("almost"), output.index("with_c"))

    def test_score_does_not_round_near_exact_units_to_100(self):
        with tempfile.TemporaryDirectory() as name:
            tmp = Path(name)
            self._repo(tmp)
            workspace = self._workspace(tmp)
            scores = {
                "assembly/textbin/with_c": 100.0,
                "assembly/textbin/almost": 99.96416,
            }

            def fake_run(command, **kwargs):
                return SimpleNamespace(
                    returncode=0,
                    stdout=json.dumps(
                        {"text_match_percent": scores[command[2]]}
                    ),
                    stderr="",
                )

            stdout = io.StringIO()
            with (
                mock.patch.object(self.lister, "ROOT", tmp),
                mock.patch.object(self.lister.subprocess, "run", fake_run),
                contextlib.redirect_stdout(stdout),
                contextlib.redirect_stderr(io.StringIO()),
            ):
                code = self.lister.main(
                    ["--score", "--limit", "0", "--workspace", str(workspace)]
                )
        self.assertEqual(code, 0)
        output = stdout.getvalue()
        almost_line = next(
            line for line in output.splitlines() if "assembly/textbin/almost" in line
        )
        exact_line = next(
            line for line in output.splitlines() if "assembly/textbin/with_c" in line
        )
        self.assertIn("99.964%", almost_line)
        self.assertNotIn("100.0%", almost_line)
        self.assertIn("100.0%", exact_line)

    def test_score_requires_a_baseline_workspace(self):
        with tempfile.TemporaryDirectory() as name:
            tmp = Path(name)
            self._repo(tmp)
            stderr = io.StringIO()
            with (
                mock.patch.object(self.lister, "ROOT", tmp),
                contextlib.redirect_stderr(stderr),
            ):
                code = self.lister.main(
                    ["--score", "--workspace", str(tmp / "not-a-workspace")]
                )
        self.assertEqual(code, 2)
        self.assertIn("baseline workspace", stderr.getvalue())

    def test_score_out_writes_the_similarity_index(self):
        with tempfile.TemporaryDirectory() as name:
            tmp = Path(name)
            self._repo(tmp)
            workspace = self._workspace(tmp)
            audit = tmp / "audit.json"
            audit.write_text("{}")
            out = tmp / "index.json"

            def fake_run(command, **kwargs):
                owner = command[2]
                return SimpleNamespace(
                    returncode=0,
                    stdout=json.dumps(
                        {
                            "unit": owner,
                            "text_match_percent": 10.0,
                            "strict_match_percent": None,
                            "measurable": True,
                            "unmeasurable_reason": None,
                            "non_text_ok": True,
                        }
                    ),
                    stderr="",
                )

            stdout = io.StringIO()
            with (
                mock.patch.object(self.lister, "ROOT", tmp),
                mock.patch.object(self.lister.subprocess, "run", fake_run),
                contextlib.redirect_stdout(stdout),
                contextlib.redirect_stderr(io.StringIO()),
            ):
                code = self.lister.main(
                    ["--score", "--out", str(out), "--audit", str(audit),
                     "--workspace", str(workspace), "--json"]
                )
            self.assertEqual(code, 0)
            payload = json.loads(out.read_text())
        self.assertEqual(payload["schema"], "rnc-pending-similarity-v1")
        self.assertEqual(payload["count"], 3)
        self.assertEqual(
            [unit["unit"] for unit in payload["units"]],
            sorted(unit["unit"] for unit in payload["units"]),
        )
        by_unit = {unit["unit"]: unit for unit in payload["units"]}
        self.assertIsNone(by_unit["assembly/textbin/no_c"]["score"])
        self.assertFalse(by_unit["assembly/textbin/no_c"]["has_c_body"])
        self.assertEqual(
            by_unit["assembly/textbin/no_c"]["unmeasurable_reason"], "no-c-body"
        )
        self.assertTrue(by_unit["assembly/textbin/with_c"]["measurable"])
        self.assertEqual(by_unit["assembly/textbin/with_c"]["score"], 10.0)
        self.assertTrue(payload["audit_sha256"])

    def test_score_out_is_atomic(self):
        with tempfile.TemporaryDirectory() as name:
            tmp = Path(name)
            self._repo(tmp)
            workspace = self._workspace(tmp)

            def fake_run(command, **kwargs):
                return SimpleNamespace(
                    returncode=0,
                    stdout=json.dumps(
                        {
                            "unit": command[2],
                            "text_match_percent": 1.0,
                            "measurable": True,
                            "unmeasurable_reason": None,
                        }
                    ),
                    stderr="",
                )

            out = tmp / "index.json"
            with (
                mock.patch.object(self.lister, "ROOT", tmp),
                mock.patch.object(self.lister.subprocess, "run", fake_run),
                contextlib.redirect_stdout(io.StringIO()),
                contextlib.redirect_stderr(io.StringIO()),
            ):
                code = self.lister.main(
                    ["--score", "--out", str(out), "--workspace", str(workspace)]
                )
            leftovers = [p.name for p in tmp.iterdir() if ".tmp" in p.name]
        self.assertEqual(code, 0)
        self.assertEqual(leftovers, [])

    def test_score_defaults_to_the_project_local_workspace(self):
        environment = {
            key: value
            for key, value in os.environ.items()
            if key != "BASELINE_ROOT"
        }
        with tempfile.TemporaryDirectory() as name:
            tmp = Path(name)
            self._repo(tmp)
            stderr = io.StringIO()
            with (
                mock.patch.object(self.lister, "ROOT", tmp),
                mock.patch.dict(os.environ, environment, clear=True),
                contextlib.redirect_stderr(stderr),
            ):
                code = self.lister.main(["--score"])
        self.assertEqual(code, 2)
        self.assertIn(str(tmp / "build" / "baseline"), stderr.getvalue())


class CheckUnitTests(unittest.TestCase):
    """check-unit.py must stage, build and score one unit from a workspace."""

    @classmethod
    def setUpClass(cls):
        cls.check = load_module("rnc_check_unit", ROOT / "scripts" / "check-unit.py")
        cls.simple = SimpleNamespace
        cls.unit = "assembly/textbin/demo"

    def _repo(self, tmp: Path):
        (tmp / "src" / "assembly" / "textbin").mkdir(parents=True)
        (tmp / "src" / "assembly" / "textbin" / "demo.c").write_text(
            textwrap.dedent(
                """\
                #include "asm.h"

                #ifndef NON_MATCHING
                INCLUDE_ASM("oracle.s", FUN_00123456);
                #else
                int FUN_00123456(void) {
                    return 1;
                }
                #endif /* NON_MATCHING */
                """
            )
        )

    def _workspace(self, tmp: Path):
        ws = tmp / "ws"
        (ws / "config" / "us").mkdir(parents=True)
        (ws / ".rnc-baseline-root").write_text("")
        (ws / "config" / "us" / "build.ninja").write_text("")
        (ws / "tools" / "objdiff").mkdir(parents=True)
        (ws / "tools" / "objdiff" / "objdiff-cli").write_text("")
        (ws / "config" / "us" / "expected" / "asm" / self.unit).mkdir(parents=True)
        (ws / "config" / "us" / "expected" / "asm" / self.unit / "FUN_00123456.s").write_text("")
        (ws / "src" / "assembly" / "textbin").mkdir(parents=True)
        (ws / "src" / "assembly" / "textbin" / "demo.c").write_text("/* stale */\n")
        return ws

    def _payload(self, match: float):
        def side(instructions):
            return {
                "sections": [{"name": ".text", "size": "8", "match_percent": match}],
                "symbols": [
                    {
                        "name": "FUN_00123456",
                        "kind": "SYMBOL_FUNCTION",
                        "size": "8",
                        "match_percent": match,
                        "instructions": instructions,
                    }
                ],
            }

        target = [{"instruction": {"address": "0", "size": 4, "formatted": "jr ra"}}]
        current = list(target)
        if match != 100.0:
            target[0]["diff_kind"] = "DIFF_REPLACE"
            current[0] = {
                "diff_kind": "DIFF_REPLACE",
                "instruction": {"address": "0", "size": 4, "formatted": "nop"},
            }
        return {"left": side(target), "right": side(current)}

    def _run(self, argv, match=100.0):
        calls = []

        def fake_objdiff(objdiff, project, unit, config):
            calls.append(config)
            return self.simple(
                returncode=0, stdout=json.dumps(self._payload(match)), stderr=""
            )

        with (
            mock.patch.object(self.check, "find_ninja", return_value="/usr/bin/ninja"),
            mock.patch.object(self.check, "objdiff_report", fake_objdiff),
            mock.patch.object(
                self.check.subprocess,
                "run",
                lambda *a, **k: self.simple(returncode=0, stdout="", stderr=""),
            ),
            contextlib.redirect_stdout(io.StringIO()) as stdout,
        ):
            code = self.check.main(argv)
        return code, stdout.getvalue(), calls

    def test_rejects_source_without_c_body(self):
        with tempfile.TemporaryDirectory() as name:
            tmp = Path(name)
            (tmp / "src" / "assembly" / "textbin").mkdir(parents=True)
            (tmp / "src" / "assembly" / "textbin" / "demo.c").write_text(
                "#ifndef NON_MATCHING\nINCLUDE_ASM(\"oracle.s\", FUN_00123456);\n#endif\n"
            )
            with mock.patch.object(self.check, "ROOT", tmp):
                with contextlib.redirect_stderr(io.StringIO()) as stderr:
                    code = self.check.main([self.unit, "--workspace", str(tmp)])
        self.assertEqual(code, 2)
        self.assertIn("no readable C body", stderr.getvalue())

    def test_stages_body_and_reports_match(self):
        with tempfile.TemporaryDirectory() as name:
            tmp = Path(name)
            self._repo(tmp)
            ws = self._workspace(tmp)
            with mock.patch.object(self.check, "ROOT", tmp):
                code, output, _ = self._run(
                    [self.unit, "--workspace", str(ws)], match=100.0
                )
            staged = (ws / "src" / "assembly" / "textbin" / "demo.c").read_text()
        self.assertEqual(code, 0)
        self.assertIn("Object matches (promotable).", output)
        self.assertIn("return 1;", staged)
        self.assertNotIn("INCLUDE_ASM", staged)

    def test_refreshes_oracle_rule_after_staging_c(self):
        with tempfile.TemporaryDirectory() as name:
            tmp = Path(name)
            self._repo(tmp)
            ws = self._workspace(tmp)
            ninja_file = ws / "config/us/build.ninja"
            ninja_file.write_text(f"build build/src/{self.unit}.c.o: include-asm source.c\n")
            calls = []

            def run(command, **kwargs):
                calls.append(command)
                if command[0] == self.check.sys.executable:
                    self.assertEqual(kwargs["cwd"], str(ws))
                    self.assertNotIn("INCLUDE_ASM", (ws / f"src/{self.unit}.c").read_text())
                    ninja_file.write_text(f"build build/src/{self.unit}.c.o: game-compiler source.c\n")
                return self.simple(returncode=0, stdout="", stderr="")

            with (
                mock.patch.object(self.check, "ROOT", tmp),
                mock.patch.object(self.check, "find_ninja", return_value="ninja"),
                mock.patch.object(self.check.subprocess, "run", run),
                mock.patch.object(self.check, "objdiff_report", return_value=self.simple(
                    returncode=0, stdout=json.dumps(self._payload(100.0)), stderr="")),
                contextlib.redirect_stdout(io.StringIO()),
            ):
                self.assertEqual(self.check.main([self.unit, "--workspace", str(ws)]), 0)
                staged_source = ws / f"src/{self.unit}.c"
                os.utime(staged_source, ns=(1_000_000_000, 1_000_000_000))
                self.assertEqual(self.check.main([self.unit, "--workspace", str(ws)]), 0)
                self.assertEqual(staged_source.stat().st_mtime_ns, 1_000_000_000)
            self.assertEqual(calls[0], [self.check.sys.executable, str(ws / "configure.py"), "config/us/rnc1.us.yaml"])
            self.assertEqual(len(calls), 3)  # Configure once, then build each measurement.
            self.assertIn("INCLUDE_ASM", (tmp / f"src/{self.unit}.c").read_text())

    def test_reconfigure_uses_baseline_assembler_default_and_preserves_override(self):
        with mock.patch.dict(os.environ, {"SN_TOOLCHAIN_ROOT": ""}):
            self.assertEqual(self.check.build_environment()["SN_TOOLCHAIN_ROOT"],
                             str(self.check.ROOT / "tools/compilers/ee-gcc-2.95.2"))
        with mock.patch.dict(os.environ, {"SN_TOOLCHAIN_ROOT": "/custom/sn"}):
            self.assertEqual(self.check.build_environment()["SN_TOOLCHAIN_ROOT"], "/custom/sn")

    def test_keeps_other_compiler_routes(self):
        for rule in ("game-compiler", "sdk-compiler", "sdk-compiler-patched"):
            with self.subTest(rule=rule), tempfile.TemporaryDirectory() as name:
                tmp = Path(name)
                self._repo(tmp)
                ws = self._workspace(tmp)
                (ws / "config/us/build.ninja").write_text(
                    f"build build/src/{self.unit}.c.o: {rule} source.c\n")
                with (
                    mock.patch.object(self.check, "ROOT", tmp),
                    mock.patch.object(self.check, "find_ninja", return_value="ninja"),
                    mock.patch.object(self.check.subprocess, "run", return_value=self.simple(
                        returncode=0, stdout="", stderr="")) as run,
                    mock.patch.object(self.check, "objdiff_report", return_value=self.simple(
                        returncode=0, stdout=json.dumps(self._payload(100.0)), stderr="")),
                    contextlib.redirect_stdout(io.StringIO()),
                ):
                    self.assertEqual(self.check.main([self.unit, "--workspace", str(ws)]), 0)
                self.assertEqual(run.call_count, 1)
                self.assertEqual(run.call_args.args[0][0], "ninja")

    def test_configuration_failure_does_not_score_stale_object(self):
        with tempfile.TemporaryDirectory() as name:
            tmp = Path(name)
            self._repo(tmp)
            ws = self._workspace(tmp)
            (ws / "config/us/build.ninja").write_text(
                f"build build/src/{self.unit}.c.o: include-asm source.c\n")
            with (
                mock.patch.object(self.check, "ROOT", tmp),
                mock.patch.object(self.check, "find_ninja", return_value="ninja"),
                mock.patch.object(self.check.subprocess, "run", return_value=self.simple(
                    returncode=1, stdout="", stderr="configuration failed")) as run,
                mock.patch.object(self.check, "objdiff_report") as diff,
                contextlib.redirect_stdout(io.StringIO()) as stdout,
                contextlib.redirect_stderr(io.StringIO()),
            ):
                code = self.check.main([self.unit, "--workspace", str(ws), "--json"])
            self.assertEqual(code, 2)
            self.assertEqual(json.loads(stdout.getvalue())["unmeasurable_reason"], "configure-failed")
            self.assertEqual(run.call_count, 1)
            diff.assert_not_called()

    def test_reports_mismatch_and_scores(self):
        with tempfile.TemporaryDirectory() as name:
            tmp = Path(name)
            self._repo(tmp)
            ws = self._workspace(tmp)
            with mock.patch.object(self.check, "ROOT", tmp):
                code, output, calls = self._run(
                    [self.unit, "--workspace", str(ws)], match=50.0
                )
        self.assertEqual(code, 1)
        self.assertIn("Not matching yet", output)
        self.assertIn("first differences", output)
        self.assertIn("FUN_00123456", output)
        self.assertEqual(calls, ["functionRelocDiffs=none"])

    def test_requires_a_baseline_workspace(self):
        with tempfile.TemporaryDirectory() as name:
            tmp = Path(name)
            self._repo(tmp)
            (tmp / "not-a-workspace").mkdir()
            with mock.patch.object(self.check, "ROOT", tmp):
                with contextlib.redirect_stderr(io.StringIO()) as stderr:
                    code = self.check.main(
                        [self.unit, "--workspace", str(tmp / "not-a-workspace")]
                    )
        self.assertEqual(code, 2)
        self.assertIn("not a baseline workspace", stderr.getvalue())

    def test_rejects_intentional_asm_units(self):
        with tempfile.TemporaryDirectory() as name:
            tmp = Path(name)
            self._repo(tmp)
            (tmp / "config" / "us").mkdir(parents=True)
            (tmp / "config" / "us" / "unit_categories.json").write_text(
                json.dumps({"intentional_asm": [self.unit]})
            )
            with mock.patch.object(self.check, "ROOT", tmp):
                with contextlib.redirect_stderr(io.StringIO()) as stderr:
                    code = self.check.main([self.unit, "--workspace", str(tmp)])
        self.assertEqual(code, 2)
        self.assertIn("intentional low-level asm", stderr.getvalue())

    def test_verdict_text_match_data_mismatch_is_not_promotable(self):
        verdict = self.check.score_verdict(
            text_match=100.0,
            functions=[{"name": "FUN_00123456", "match_percent": 100.0}],
            section_rows=[{"name": ".rodata", "match_percent": 50.0}],
        )
        self.assertTrue(verdict["measurable"])
        self.assertTrue(verdict["ok"])
        self.assertFalse(verdict["non_text_ok"])
        self.assertFalse(verdict["promotable"])
        self.assertIsNone(verdict["unmeasurable_reason"])

    def test_verdict_custom_sections_are_unmeasurable(self):
        verdict = self.check.score_verdict(
            text_match=0.0,
            functions=[{"name": "FUN_00123456", "match_percent": None}],
            section_rows=[],
            custom_sections=[".text.seed_name"],
        )
        self.assertFalse(verdict["measurable"])
        self.assertEqual(verdict["unmeasurable_reason"], "custom-sections")
        self.assertFalse(verdict["ok"])

    def test_verdict_unpaired_symbol_is_no_pairing(self):
        verdict = self.check.score_verdict(
            text_match=None,
            functions=[{"name": "FUN_00123456", "match_percent": None}],
            section_rows=[],
        )
        self.assertFalse(verdict["measurable"])
        self.assertEqual(verdict["unmeasurable_reason"], "no-pairing")

    def test_verdict_zero_size_alias_does_not_block(self):
        verdict = self.check.score_verdict(
            text_match=100.0,
            functions=[
                {"name": "FUN_00204ef8", "size": 100, "match_percent": 100.0},
                {"name": "func_00204EF8", "size": 0, "match_percent": None},
            ],
            section_rows=[],
        )
        self.assertTrue(verdict["ok"])
        self.assertTrue(verdict["promotable"])

    def test_verdict_no_reloc_textbin_is_promotable(self):
        verdict = self.check.score_verdict(
            text_match=100.0,
            functions=[{"name": "FUN_00123456", "match_percent": 100.0}],
            section_rows=[],
        )
        self.assertTrue(verdict["measurable"])
        self.assertTrue(verdict["non_text_ok"])
        self.assertTrue(verdict["promotable"])
        self.assertTrue(verdict["ok"])

    def test_json_reports_promotable_fields(self):
        with tempfile.TemporaryDirectory() as name:
            tmp = Path(name)
            self._repo(tmp)
            ws = self._workspace(tmp)
            with mock.patch.object(self.check, "ROOT", tmp):
                code, output, _ = self._run(
                    [self.unit, "--workspace", str(ws), "--json"], match=100.0
                )
        self.assertEqual(code, 0)
        payload = json.loads(output)
        self.assertTrue(payload["measurable"])
        self.assertIsNone(payload["unmeasurable_reason"])
        self.assertTrue(payload["non_text_ok"])
        self.assertTrue(payload["promotable"])
        self.assertTrue(payload["ok"])

    def test_terminal_reports_data_mismatch(self):
        data_payload = self._payload(100.0)
        data_payload["left"]["sections"].append(
            {"name": ".rodata", "size": "8", "match_percent": 25.0}
        )
        with tempfile.TemporaryDirectory() as name:
            tmp = Path(name)
            self._repo(tmp)
            ws = self._workspace(tmp)
            with (
                mock.patch.object(self.check, "ROOT", tmp),
                mock.patch.object(self.check, "find_ninja", return_value="/usr/bin/ninja"),
                mock.patch.object(
                    self.check, "objdiff_report",
                    lambda *a, **k: self.simple(returncode=0, stdout=json.dumps(data_payload), stderr=""),
                ),
                mock.patch.object(
                    self.check.subprocess, "run",
                    lambda *a, **k: self.simple(returncode=0, stdout="", stderr=""),
                ),
                contextlib.redirect_stdout(io.StringIO()) as stdout,
            ):
                code = self.check.main([self.unit, "--workspace", str(ws)])
        self.assertEqual(code, 0)
        self.assertIn("Text matches; data/rodata differ", stdout.getvalue())

    def test_compile_failure_reports_reason_in_json(self):
        with tempfile.TemporaryDirectory() as name:
            tmp = Path(name)
            self._repo(tmp)
            ws = self._workspace(tmp)
            with (
                mock.patch.object(self.check, "ROOT", tmp),
                mock.patch.object(self.check, "find_ninja", return_value="/usr/bin/ninja"),
                mock.patch.object(
                    self.check.subprocess, "run",
                    lambda *a, **k: self.simple(returncode=1, stdout="boom", stderr=""),
                ),
                contextlib.redirect_stdout(io.StringIO()) as stdout,
                contextlib.redirect_stderr(io.StringIO()),
            ):
                code = self.check.main([self.unit, "--workspace", str(ws), "--json"])
        self.assertEqual(code, 1)
        payload = json.loads(stdout.getvalue())
        self.assertFalse(payload["measurable"])
        self.assertEqual(payload["unmeasurable_reason"], "compile-failed")

    def test_normalize_unit(self):
        self.assertEqual(self.check.normalize_unit("src/assembly/x/y.c"), "assembly/x/y")
        self.assertEqual(self.check.normalize_unit("./assembly/x/y"), "assembly/x/y")
        self.assertIsNone(self.check.normalize_unit("../../etc/passwd"))
        self.assertIsNone(self.check.normalize_unit("assembly/../../x"))
        self.assertIsNone(self.check.normalize_unit("/etc/passwd"))
        self.assertIsNone(self.check.normalize_unit(""))

    def test_rejects_path_traversal(self):
        with tempfile.TemporaryDirectory() as name:
            tmp = Path(name)
            self._repo(tmp)
            with mock.patch.object(self.check, "ROOT", tmp):
                with contextlib.redirect_stderr(io.StringIO()) as stderr:
                    code = self.check.main(["../outside/x"])
        self.assertEqual(code, 2)
        self.assertIn("invalid unit path", stderr.getvalue())

    def test_refuses_oracle_fallback_units(self):
        with tempfile.TemporaryDirectory() as name:
            tmp = Path(name)
            self._repo(tmp)
            ws = self._workspace(tmp)
            (ws / "config" / "us" / "oracle-fallback-units.json").write_text(
                json.dumps({"units": [self.unit]})
            )
            with mock.patch.object(self.check, "ROOT", tmp):
                with contextlib.redirect_stderr(io.StringIO()) as stderr:
                    code = self.check.main([self.unit, "--workspace", str(ws)])
        self.assertEqual(code, 2)
        self.assertIn("rebuilt from the retail oracle", stderr.getvalue())


class PatchedToolchainArtifactTests(unittest.TestCase):
    """The published toolchain patch and its build script must agree."""

    @classmethod
    def setUpClass(cls):
        cls.builder = load_module(
            "rnc_build_patched_toolchain",
            ROOT / "scripts" / "build-patched-toolchain.py",
        )

    def test_patch_file_matches_pinned_hash(self):
        patch = Path(self.builder.PATCH_PATH)
        self.assertTrue(patch.is_file(), patch)
        self.assertEqual(
            hashlib.sha256(patch.read_bytes()).hexdigest(),
            self.builder.PATCH_SHA256,
        )

    def test_source_and_bison_are_pinned(self):
        self.assertEqual(self.builder.SOURCE_REVISION, "b595ded")
        self.assertIn("ps2-ee-toolchain", self.builder.SOURCE_URL)
        self.assertEqual(len(self.builder.BISON_SHA256), 64)
        self.assertIn("bison-1.28", self.builder.BISON_URL)

    def test_reference_hashes_are_full_sha256(self):
        for name, digest in self.builder.REFERENCE_HASHES.items():
            self.assertEqual(len(digest), 64, name)


class RenameCatalogUnitTests(unittest.TestCase):
    """One rename proposal per unit, or the progress workflow cannot load.

    The rename tool rewrites the unit of every catalog entry that shares an
    address with a moved unit. A catalog that already carried two rows for one
    function - a proposal plus the unresolved row it replaced - therefore ends up
    with both rows naming the same unit, and progress_groups.py refuses to read
    it. That is how the 2026-09-26 music-track renames turned the progress
    workflow red.
    """

    def setUp(self):
        self.tool = load_module(
            "rename_tool", ROOT / "scripts/rename-function-proposals.py"
        )

    @staticmethod
    def _payload(paths):
        return {"rename_proposals": {"entries": [{"source_path": p} for p in paths]}}

    def test_one_row_per_unit_is_accepted(self):
        payload = self._payload(["src/textbin/a.c", "src/textbin/b.c"])
        self.assertEqual(self.tool.find_duplicate_catalog_units(payload), [])

    def test_two_rows_for_one_unit_are_reported(self):
        payload = self._payload(
            ["src/audio/music/music_start_track_10000.c"] * 2
        )
        self.assertEqual(
            self.tool.find_duplicate_catalog_units(payload),
            ["audio/music/music_start_track_10000"],
        )

    def test_two_functions_of_one_unit_are_accepted(self):
        payload = self._payload(["src/rendering/vu1_chain.c"] * 2)
        for entry, address in zip(
            payload["rename_proposals"]["entries"], ("0x002336a0", "0x002337b0")
        ):
            entry["address"] = address
        self.assertEqual(self.tool.find_duplicate_catalog_units(payload), [])

    def test_a_pending_and_a_promoted_path_are_the_same_unit(self):
        # assembly/<x>.c and <x>.c are one unit before and after promotion.
        payload = self._payload(
            ["src/assembly/textbin/fun_002158a0.c", "src/textbin/fun_002158a0.c"]
        )
        self.assertEqual(
            self.tool.find_duplicate_catalog_units(payload), ["textbin/fun_002158a0"]
        )

    def test_rows_without_a_source_path_are_ignored(self):
        payload = self._payload(["src/textbin/a.c"])
        payload["rename_proposals"]["entries"].append({"address": "0x1"})
        self.assertEqual(self.tool.find_duplicate_catalog_units(payload), [])

    def test_the_committed_catalog_has_no_duplicate(self):
        catalog = ROOT / "config/us/recovered_names.json"
        payload = json.loads(catalog.read_text(encoding="utf-8"))
        self.assertEqual(self.tool.find_duplicate_catalog_units(payload), [])


class NoGameDataTests(unittest.TestCase):
    """Nothing cut from the disc (ELF, overlay records, their asm) is tracked."""

    def test_no_tracked_game_data(self):
        listed = subprocess.run(["git", "ls-files"], cwd=ROOT, capture_output=True,
                                text=True)
        if listed.returncode:
            self.skipTest("not a git checkout")
        bad = [p for p in listed.stdout.splitlines()
               if p.startswith(("config/us/overlays/", "config/us/expected/",
                                "config/us/asm/", "dumps/", "build/"))
               and not p.endswith("README.md")
               or p.endswith((".s", ".S", ".bin", ".iso", ".elf", ".o"))
               or p == "config/us/SCUS_971.99"]
        self.assertEqual(bad, [])


class OverlayStageTests(unittest.TestCase):
    def setUp(self):
        sys.path.insert(0, str(ROOT / "scripts"))
        self.unit = load_module("overlay_unit", ROOT / "scripts/overlay_unit.py")

    def test_guarded_body_is_staged_and_other_stubs_dropped(self):
        text = textwrap.dedent("""\
            #include "asm.h"
            INCLUDE_ASM("config/us/overlays/asm/FUN_L00_00000010.s", FUN_L00_00000010);
            #ifndef NON_MATCHING
            INCLUDE_ASM("config/us/overlays/asm/FUN_L00_00000020.s", FUN_L00_00000020);
            #else
            void FUN_L00_00000020(void) {}
            #endif /* NON_MATCHING */
            """)
        staged, how = self.unit.stage("FUN_L00_00000020", text)
        self.assertIn("void FUN_L00_00000020(void) {}", staged)
        self.assertNotIn("INCLUDE_ASM", staged)
        self.assertNotIn("NON_MATCHING", staged)
        self.assertIn("guard", how)

    def test_bare_stub_has_no_c(self):
        text = 'INCLUDE_ASM("config/us/overlays/asm/FUN_L00_00000010.s", FUN_L00_00000010);\n'
        self.assertIsNone(self.unit.stage("FUN_L00_00000010", text)[0])


if __name__ == "__main__":
    unittest.main()
