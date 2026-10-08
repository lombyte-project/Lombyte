#!/usr/bin/env python3
"""Build the optional patched EE-GCC 2.9-ee-991111-01 profile.

Clones the public snapshot at the pinned revision, applies
``patches/ee-gcc-2.9-991111-01/patched-ee-gcc.patch``, and builds ``cc1``,
``cpp`` and ``xgcc`` into ``tools/ee-gcc2.9-991111-01-patched/``. Needs a
32-bit host build environment (``gcc -m32``, e.g. ``gcc-multilib``); builds
bison 1.28 automatically when no 1.2x bison is on PATH. See
docs/patched-toolchain.md. Offline: pass ``--source-dir`` and ``--bison``.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import os
import shutil
import subprocess
import sys
import tarfile
import urllib.request
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

SOURCE_URL = "https://github.com/SSXModding/ps2-ee-toolchain.git"
SOURCE_REVISION = "b595ded"
PATCH_PATH = (
    ROOT / "patches" / "ee-gcc-2.9-991111-01" / "patched-ee-gcc.patch"
)
PATCH_SHA256 = "0faa74c638ad4ce2fa3ebf7e9b1a99afc295bb2b61958d177bf883d4fa3eef4e"
BISON_URL = "https://ftp.gnu.org/gnu/bison/bison-1.28.tar.gz"
BISON_SHA256 = "c5d3e4858e17cb440cee9de7837f07277bcfb03507e9d2f0c506cab5efe36c3a"
HOST_CFLAGS = "-O2 -fcommon -std=gnu89 -D_GNU_SOURCE"
# The reference build compiled these objects without optimization; keep the
# same host flags so the produced compiler matches the maintained recipe.
HOST_OBJECTS_CFLAGS_MK = (
    "version.o real.o getpwd.o mbchar.o dyn-string.o splay-tree.o "
    "obstack.o gcc.o mkstemp.o: override CFLAGS = -g\n"
)
# Reference hashes of a Linux cloud host build. Rebuilds embed their own
# build paths and differ; they are reported, not enforced unless
# --verify-hashes is passed.
REFERENCE_HASHES = {
    "cc1": "c2aaf9dcbd5d72eafcc63b4ae98472d3f8b9f7202bab3a10dfe7b535f10b9213",
    "cpp": "1509d06d85068c86c6e2529108597c0e4d3d9ecafcd3e43b6d14607c9bc3b1b7",
    "xgcc": "28872669c68b760b3220077ac313f908c479f3b1282e29308988318e2c76b252",
}


def parse_args(argv=None):
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument(
        "--source-dir",
        type=Path,
        help="existing ps2-ee-toolchain checkout to clone from (offline); "
        f"must be at revision {SOURCE_REVISION}",
    )
    parser.add_argument(
        "--work",
        type=Path,
        default=ROOT / "build" / "toolchain-build",
        help="build directory (default: build/toolchain-build)",
    )
    parser.add_argument(
        "--output",
        type=Path,
        default=ROOT / "tools" / "ee-gcc2.9-991111-01-patched",
        help="install directory for cc1/cpp/xgcc (default: tools/ee-gcc2.9-991111-01-patched)",
    )
    parser.add_argument(
        "--bison",
        type=Path,
        help="path to a bison 1.2x binary (default: use PATH, else download and build 1.28)",
    )
    parser.add_argument(
        "--jobs",
        type=int,
        default=os.cpu_count() or 1,
        help="parallel build jobs (default: number of CPUs)",
    )
    parser.add_argument(
        "--check",
        action="store_true",
        help="verify the source revision and patch without building",
    )
    parser.add_argument(
        "--verify-hashes",
        action="store_true",
        help="fail when the built binaries differ from the reference hashes",
    )
    return parser.parse_args(argv)


class BuildError(SystemExit):
    def __init__(self, message: str):
        super().__init__(f"build-patched-toolchain: error: {message}")


def run(command: list[str], *, cwd: Path | None = None, env: dict | None = None):
    printable = " ".join(str(part) for part in command)
    print(f"+ {printable}", flush=True)
    result = subprocess.run(command, cwd=str(cwd) if cwd else None, env=env)
    if result.returncode != 0:
        raise BuildError(f"command failed with status {result.returncode}: {printable}")


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1 << 20), b""):
            digest.update(chunk)
    return digest.hexdigest()


def check_patch() -> None:
    if not PATCH_PATH.is_file():
        raise BuildError(f"missing patch file: {PATCH_PATH}")
    digest = sha256(PATCH_PATH)
    if digest != PATCH_SHA256:
        raise BuildError(
            f"{PATCH_PATH} has sha256 {digest}, expected {PATCH_SHA256}"
        )


def clone_source(work: Path, source_dir: Path | None) -> Path:
    source = work / "source"
    if (source / ".git").is_dir():
        # Keep repeated runs deterministic: the patch is applied below.
        run(["git", "-C", str(source), "checkout", "--quiet", "--", "."])
        run(["git", "-C", str(source), "clean", "-fdq"])
        return source
    if source.exists():
        shutil.rmtree(source)
    if source_dir is not None:
        checkout = source_dir.expanduser().resolve()
        if not (checkout / ".git").is_dir():
            raise BuildError(f"not a git checkout: {checkout}")
        run(["git", "clone", "--quiet", "--shared", str(checkout), str(source)])
    else:
        run(["git", "clone", "--quiet", SOURCE_URL, str(source)])
    revision = subprocess.run(
        ["git", "-C", str(source), "rev-parse", "HEAD"],
        capture_output=True,
        text=True,
        check=True,
    ).stdout.strip()
    if not revision.startswith(SOURCE_REVISION):
        raise BuildError(
            f"source is at {revision}, expected {SOURCE_REVISION}; "
            "delete the work directory and retry"
        )
    return source


def apply_patch(source: Path) -> None:
    check = subprocess.run(
        ["git", "-C", str(source), "apply", "--check", str(PATCH_PATH)],
        capture_output=True,
        text=True,
    )
    if check.returncode != 0:
        raise BuildError(f"patch does not apply: {check.stderr.strip()}")
    run(["git", "-C", str(source), "apply", str(PATCH_PATH)])


def local_bison() -> Path | None:
    candidate = shutil.which("bison")
    if candidate is None:
        return None
    try:
        version = subprocess.run(
            [candidate, "--version"], capture_output=True, text=True, check=True
        ).stdout.splitlines()[0]
    except (OSError, subprocess.CalledProcessError, IndexError):
        return None
    # The 991111 grammar needs the bison 1.x skeleton (YYLEX); 2.x and 3.x
    # generate an incompatible parser.
    if "1.2" in version:
        return Path(candidate)
    return None


def build_bison(work: Path, jobs: int) -> tuple[Path, dict]:
    """Download and build bison 1.28; return (binary, environment)."""
    install = work / "bison"
    binary = install / "bin" / "bison"
    if binary.is_file():
        return binary, bison_environment(binary)
    archive = work / "bison-1.28.tar.gz"
    if not archive.is_file():
        print(f"downloading {BISON_URL}")
        urllib.request.urlretrieve(BISON_URL, archive)
    if sha256(archive) != BISON_SHA256:
        raise BuildError(f"{archive} failed its sha256 check; delete it and retry")
    source = work / "bison-1.28"
    if not source.is_dir():
        with tarfile.open(archive) as tar:
            try:
                tar.extractall(work, filter="data")
            except TypeError:  # Python < 3.12
                tar.extractall(work)
    env = dict(os.environ, CC="gcc -std=gnu89", CFLAGS="-O2 -fcommon")
    run(
        [str(source / "configure"), f"--prefix={install}"],
        cwd=source,
        env=env,
    )
    run(["make", f"-j{jobs}"], cwd=source, env=env)
    run(["make", "install"], cwd=source, env=env)
    return binary, bison_environment(binary)


def bison_environment(bison: Path) -> dict:
    """Locate the bison 1.x skeletons next to a bison binary, if present."""
    env = {}
    for directory in (bison.parent, bison.parent.parent / "share", bison.parent.parent / "lib"):
        simple = directory / "bison.simple"
        hairy = directory / "bison.hairy"
        if simple.is_file():
            env["BISON_SIMPLE"] = str(simple)
        if hairy.is_file():
            env["BISON_HAIRY"] = str(hairy)
        if "BISON_SIMPLE" in env:
            break
    return env


def build(
    source: Path,
    work: Path,
    output: Path,
    bison: Path,
    bison_env: dict,
    jobs: int,
) -> dict:
    build_dir = work / "build"
    build_dir.mkdir(parents=True, exist_ok=True)
    # GCC's generated Makefiles do not track tm.h (mips.h) dependencies, so a
    # reused build directory silently keeps stale option tables after a recipe
    # change (e.g. a changed mips.h).  Stamp the objects with the recipe
    # they were built from and start over when it moves.
    recipe_stamp = build_dir / ".recipe-sha256"
    recipe_now = sha256(PATCH_PATH)
    if (
        (build_dir / "gcc").is_dir()
        and recipe_stamp.is_file()
        and recipe_stamp.read_text().strip() != recipe_now
    ):
        print("recipe changed: discarding the previous toolchain build")
        shutil.rmtree(build_dir)
        build_dir.mkdir(parents=True, exist_ok=True)
    prefix = work / "install"
    host_flags = build_dir / "host-flags.mk"
    host_flags.write_text(HOST_OBJECTS_CFLAGS_MK)

    env = dict(os.environ, CC="gcc -m32", CXX="g++ -m32", CFLAGS=HOST_CFLAGS)
    if not (build_dir / "Makefile").is_file():
        run(
            [
                "bash",
                str(source / "ee" / "configure"),
                "--target=ee",
                "--host=i686-linux-gnu",
                "--build=i686-linux-gnu",
                "--disable-nls",
                "--enable-languages=c",
                "--without-headers",
                "--disable-sim",
                "--without-sim",
                f"--prefix={prefix}",
            ],
            cwd=build_dir,
            env=env,
        )
    jobs = f"-j{jobs}"
    run(["make", jobs, "all-libiberty"], cwd=build_dir, env=env)
    # Reusing a build directory must not reuse parser files generated by a
    # different bison version; regenerate them with the selected bison.
    for generated in ("c-parse.c", "c-parse.h", "c-parse.y", "cexp.c"):
        (build_dir / "gcc" / generated).unlink(missing_ok=True)
    run(
        [
            "make",
            "-C",
            "gcc",
            "-f",
            "Makefile",
            "-f",
            str(host_flags),
            jobs,
            "LANGUAGES=c",
            "CC=gcc -m32",
            f"CFLAGS={HOST_CFLAGS}",
            f"BISON={bison}",
            "cc1",
            "cpp",
            "xgcc",
        ],
        cwd=build_dir,
        env=dict(env, **bison_env),
    )

    output.mkdir(parents=True, exist_ok=True)
    provenance = {
        "schema": "rnc-patched-ee-gcc-v1",
        "source": f"{SOURCE_URL} @ {SOURCE_REVISION}",
        "revision": SOURCE_REVISION,
        "bison": str(bison),
        "patch": str(PATCH_PATH.relative_to(ROOT)),
        "patch_sha256": PATCH_SHA256,
        "hashes": {},
        "reference_hashes": REFERENCE_HASHES,
    }
    for name in ("cc1", "cpp", "xgcc"):
        built = build_dir / "gcc" / name
        if not built.is_file():
            raise BuildError(f"build did not produce {built}")
        shutil.copy2(built, output / name)
        (output / name).chmod(0o755)
        provenance["hashes"][name] = sha256(output / name)
    (output / "provenance.json").write_text(
        json.dumps(provenance, indent=2) + "\n"
    )
    recipe_stamp.write_text(recipe_now + "\n")
    return provenance


def smoke_test(output: Path) -> None:
    banner = subprocess.run(
        [str(output / "xgcc"), "--version"], capture_output=True, text=True
    )
    if banner.returncode != 0:
        raise BuildError(f"{output / 'xgcc'} does not run")
    probe = output / "smoke.c"
    probe.write_text("int smoke(void) { return 42; }\n")
    try:
        # The game build passes -B<root>/ so the driver finds cc1 next to it;
        # use the same invocation here.
        result = subprocess.run(
            [
                str(output / "xgcc"),
                "-B" + str(output) + os.sep,
                "-S",
                str(probe),
                "-o",
                str(output / "smoke.s"),
            ],
            capture_output=True,
            text=True,
        )
        if result.returncode != 0:
            raise BuildError(
                "the built xgcc cannot compile a test file: "
                f"{result.stderr.strip()[-400:]}"
            )
    finally:
        for path in (probe, output / "smoke.s"):
            path.unlink(missing_ok=True)


def main(argv=None) -> int:
    args = parse_args(argv)
    check_patch()

    if args.check:
        print(f"patch {PATCH_PATH.name} sha256 OK")
        work = args.work.expanduser().resolve()
        source = clone_source(work, args.source_dir)
        apply_patch(source)
        print(f"patch applies to {SOURCE_URL} @ {SOURCE_REVISION}")
        return 0

    work = args.work.expanduser().resolve()
    output = args.output.expanduser().resolve()
    work.mkdir(parents=True, exist_ok=True)

    source = clone_source(work, args.source_dir)
    apply_patch(source)

    if args.bison is not None:
        bison = args.bison.expanduser().resolve()
        if not bison.is_file():
            raise BuildError(f"bison not found: {bison}")
        bison_env = bison_environment(bison)
    else:
        bison = local_bison()
        if bison is None:
            bison, bison_env = build_bison(work, args.jobs)
        else:
            bison_env = bison_environment(bison)
    print(f"using bison: {bison}")

    provenance = build(source, work, output, bison, bison_env, args.jobs)
    smoke_test(output)

    print("\nbuilt:")
    for name, digest in provenance["hashes"].items():
        marker = (
            "" if digest == REFERENCE_HASHES[name] else "  (differs from reference)"
        )
        print(f"  {name:<5} sha256 {digest}{marker}")
    if args.verify_hashes:
        for name, digest in provenance["hashes"].items():
            if digest != REFERENCE_HASHES[name]:
                raise BuildError(
                    f"{name} differs from the reference hash "
                    f"{REFERENCE_HASHES[name]}"
                )
    print(
        "\nThe published reference hashes identify the maintainer's build; "
        "rebuilds embed their own build paths and normally differ.\n"
        "Use it with:\n"
        f"  export EE_GCC_PATCHED_ROOT={output}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
