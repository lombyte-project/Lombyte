#!/usr/bin/env python3
"""Build the game compiler (Sony/Cygnus EE-GCC 2.9-ee-991111b) from source.

Downloads the public source archive pinned in ``patches/sce-991111b/README.md``
(or reuses a local copy), applies the production patch stack from
``patches/sce-991111b/`` in order, and builds ``cc1``, ``cpp``, ``xgcc`` and
the patched GNU ``as`` into ``tools/compilers/game-compiler/`` together with
the ``ee-gcc`` driver wrapper and the three ``ginclude`` headers the driver
needs. Nothing proprietary is involved: the archive is GPL source code, the
patches live in this repository, and the output is checked by ``make elf``
(the built bytes depend on the host compiler, so hashes are reported, not
enforced).

Needs a 32-bit host build environment (``gcc -m32``, e.g. ``gcc-multilib``),
``make``, ``git`` (for ``git apply``) and ``flex``; builds bison 1.28 itself
when no 1.2x bison is on PATH. Offline: pass ``--archive`` and ``--bison``.
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
import urllib.error
import urllib.request
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
PATCH_DIR = ROOT / "patches" / "sce-991111b"

ARCHIVE_NAME = "gnu-ee-binutils-gcc-1.1.tar.gz"
ARCHIVE_URLS = (
    "https://web.archive.org/web/20060518234647id_/"
    "http://ps2dev.sourceforge.net:80/downloads/ee/gnu-ee-binutils-gcc-1.1.tar.gz",
)
ARCHIVE_SHA256 = "1f518043e252d6eda726386971d52eda26541ab936ea73a9783d73712b595f92"
ARCHIVE_TOP = "gnu-ee-binutils-gcc"

# Production stack, in application order (patches/sce-991111b/README.md).
PATCH_ORDER = (
    "0000", "0001", "0015", "0016", "0019", "0020", "0021", "0022", "0025",
    "0026", "0027", "0028", "0029", "0030", "0031", "0032", "0033", "0034",
    "0037", "0036", "0044", "0045", "0046", "0047", "0048", "0049", "0050",
    "0051", "0052", "0053", "0054", "0055", "0056",
)

BISON_URLS = (
    "https://ftp.gnu.org/gnu/bison/bison-1.28.tar.gz",
    "https://mirrors.kernel.org/gnu/bison/bison-1.28.tar.gz",
    "https://ftpmirror.gnu.org/gnu/bison/bison-1.28.tar.gz",
)
BISON_SHA256 = "c5d3e4858e17cb440cee9de7837f07277bcfb03507e9d2f0c506cab5efe36c3a"

# real.c type-puns through EMUSHORT pointers; strict aliasing on a modern host
# turns REAL_VALUE_NEGATE into a no-op (see patches/sce-991111b/README.md).
HOST_CFLAGS = "-O2 -fno-strict-aliasing -fcommon -std=gnu89 -D_GNU_SOURCE"
HOST_OBJECTS_CFLAGS_MK = "obstack.o gcc.o mkstemp.o: override CFLAGS = -g\n"

# Hashes of the maintainer's build (full stack through 0056, Ubuntu 24.04
# x86-64 host). Rebuilds on other hosts embed their own build paths and may
# differ; the full-ELF gate (`make elf`) is the check that matters.
REFERENCE_HASHES = {
    "cc1": "fc69951c0ec883e19179d289fd690b3c7f94f15abb71b1ddafad14d7794cdf15",
    "cpp": "c1ab66820a740deb5c2ec95c07bac49d06e2baa46f8e00641c7a9ae54e8e3759",
    "xgcc": "54a8bb9dfe0f51f4b6138569564dbc822bb2d67736e627a9c93c42934300e854",
    "as": "af95ed125045dcc0dd3549b25e2d73f44ed03b283d4129d09979e879eab34fff",
}

# The driver has no builtin include directory; these three headers from the
# archive's gcc/ginclude are installed next to it (docs/building.md).
GINCLUDE_HASHES = {
    "stdarg.h": "6cf354c23924f3389dd16f7f1f35b4189383dbedac26d821c2db2536836de3e0",
    "stddef.h": "23ed59b27cda4de82a312f98e85a258f29d61711beb873a42a1f8404558894ae",
    "va-mips.h": "2610e1591656a950ccf3e2bd0a751f5819c7d852ddca0705158e0700589a14e0",
}

EE_GCC_WRAPPER = """#!/bin/sh
DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
exec "$DIR/xgcc" -B"$DIR/" "$@"
"""


class BuildError(SystemExit):
    def __init__(self, message: str):
        super().__init__(f"build-game-compiler: error: {message}")


def parse_args(argv=None):
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument(
        "--archive",
        type=Path,
        help=f"local copy of {ARCHIVE_NAME} (default: download and cache it)",
    )
    parser.add_argument(
        "--work",
        type=Path,
        default=ROOT / "build" / "game-compiler-build",
        help="build directory (default: build/game-compiler-build)",
    )
    parser.add_argument(
        "--output",
        type=Path,
        default=ROOT / "tools" / "compilers" / "game-compiler",
        help="install directory (default: tools/compilers/game-compiler)",
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
        help="verify the archive and that the patch stack applies, without building",
    )
    return parser.parse_args(argv)


def run(command: list[str], *, cwd: Path | None = None, env: dict | None = None,
        log: Path | None = None) -> None:
    printable = " ".join(str(part) for part in command)
    print(f"+ {printable}", flush=True)
    if log is None:
        result = subprocess.run(command, cwd=str(cwd) if cwd else None, env=env)
    else:
        with log.open("wb") as stream:
            result = subprocess.run(
                command, cwd=str(cwd) if cwd else None, env=env,
                stdout=stream, stderr=subprocess.STDOUT,
            )
    if result.returncode != 0:
        where = f" (log: {log})" if log else ""
        raise BuildError(f"command failed with status {result.returncode}: {printable}{where}")


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1 << 20), b""):
            digest.update(chunk)
    return digest.hexdigest()


def download(urls: tuple[str, ...], target: Path, expected: str) -> None:
    if target.is_file() and sha256(target) == expected:
        print(f"using cached {target.name}")
        return
    target.parent.mkdir(parents=True, exist_ok=True)
    partial = target.with_suffix(target.suffix + ".part")
    errors = []
    for url in urls:
        print(f"downloading {url}", flush=True)
        try:
            with urllib.request.urlopen(url, timeout=120) as response, partial.open("wb") as out:
                shutil.copyfileobj(response, out)
        except (urllib.error.URLError, OSError) as error:
            errors.append(f"{url}: {error}")
            partial.unlink(missing_ok=True)
            continue
        digest = sha256(partial)
        if digest == expected:
            partial.replace(target)
            return
        errors.append(f"{url}: sha256 {digest}, expected {expected}")
        partial.unlink(missing_ok=True)
    raise BuildError(
        f"could not fetch {target.name}:\n  " + "\n  ".join(errors)
        + f"\nPlace a verified copy at {target} (or pass --archive) and retry."
    )


def patch_files() -> list[Path]:
    files = []
    for prefix in PATCH_ORDER:
        matches = sorted(PATCH_DIR.glob(f"{prefix}-*.patch"))
        if len(matches) != 1:
            raise BuildError(f"expected exactly one {prefix}-*.patch in {PATCH_DIR}")
        files.append(matches[0])
    return files


def extract_source(archive: Path, work: Path) -> Path:
    source = work / "source"
    if source.exists():
        shutil.rmtree(source)
    stage = work / "source.extract"
    if stage.exists():
        shutil.rmtree(stage)
    stage.mkdir(parents=True)
    print(f"extracting {archive.name}", flush=True)
    with tarfile.open(archive) as tar:
        try:
            tar.extractall(stage, filter="data")
        except TypeError:  # Python < 3.12
            tar.extractall(stage)
    top = stage / ARCHIVE_TOP
    if not (top / "gcc" / "version.c").is_file():
        raise BuildError(f"unexpected archive layout: no {ARCHIVE_TOP}/gcc/version.c")
    top.rename(source)
    shutil.rmtree(stage)
    return source


def apply_patches(source: Path) -> None:
    # Apply from the repository root with --directory: run from inside the
    # checkout, git resolves the patch paths against the repository root and
    # skips every path that is not there with exit status 0, so no patch lands
    # and the first hunk that no longer matches (0046) is blamed instead.
    try:
        directory = ["--directory", str(source.resolve().relative_to(Path.cwd().resolve()))]
    except ValueError:
        directory = []
    for patch in patch_files():
        check = subprocess.run(
            ["git", "apply", "--check", *directory, str(patch)],
            capture_output=True, text=True,
        )
        if check.returncode != 0:
            raise BuildError(f"{patch.name} does not apply: {check.stderr.strip()}")
        run(["git", "apply", *directory, str(patch)])


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
    # The 991111 grammar needs the bison 1.x skeleton; 2.x/3.x parsers differ.
    if "1.2" in version:
        return Path(candidate)
    return None


def bison_environment(bison: Path) -> dict:
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


def build_bison(work: Path, jobs: int) -> tuple[Path, dict]:
    install = work / "bison"
    binary = install / "bin" / "bison"
    if binary.is_file():
        return binary, bison_environment(binary)
    archive = work / "bison-1.28.tar.gz"
    download(BISON_URLS, archive, BISON_SHA256)
    source = work / "bison-1.28"
    if not source.is_dir():
        with tarfile.open(archive) as tar:
            try:
                tar.extractall(work, filter="data")
            except TypeError:
                tar.extractall(work)
    env = dict(os.environ, CC="gcc -std=gnu89", CFLAGS="-O2 -fcommon")
    log = work / "bison-build.log"
    run([str(source / "configure"), f"--prefix={install}"], cwd=source, env=env, log=log)
    run(["make", f"-j{jobs}"], cwd=source, env=env, log=log)
    run(["make", "install"], cwd=source, env=env, log=log)
    return binary, bison_environment(binary)


def build(source: Path, work: Path, bison: Path, bison_env: dict, jobs: int) -> Path:
    build_dir = work / "build"
    if build_dir.exists():
        shutil.rmtree(build_dir)
    build_dir.mkdir(parents=True)
    host_flags = build_dir / "host-flags.mk"
    host_flags.write_text(HOST_OBJECTS_CFLAGS_MK)
    env = dict(os.environ, CC="gcc -m32", CXX="g++ -m32", CFLAGS=HOST_CFLAGS)
    log = work / "build.log"
    run(
        [
            "bash", str(source / "configure"),
            "--target=mips64r5900-sf-elf",
            "--host=i686-linux-gnu",
            "--build=i686-linux-gnu",
            "--disable-nls",
            "--enable-languages=c",
            "--without-headers",
            f"--prefix={work / 'install'}",
        ],
        cwd=build_dir, env=env, log=log,
    )
    parallel = f"-j{jobs}"
    run(["make", parallel, "all-libiberty"], cwd=build_dir, env=env, log=log)
    run(
        [
            "make", "-C", "gcc", "-f", "Makefile", "-f", str(host_flags), parallel,
            "LANGUAGES=c", "CC=gcc -m32", f"CFLAGS={HOST_CFLAGS}", f"BISON={bison}",
            "cc1", "cpp", "xgcc",
        ],
        cwd=build_dir, env=dict(env, **bison_env), log=log,
    )
    run(["make", parallel, "all-gas"], cwd=build_dir, env=env, log=log)
    return build_dir


def install(source: Path, build_dir: Path, output: Path, archive: Path, bison: Path) -> dict:
    stage = output.parent / f".{output.name}.stage"
    if stage.exists():
        shutil.rmtree(stage)
    stage.mkdir(parents=True)
    built = {
        "cc1": build_dir / "gcc" / "cc1",
        "cpp": build_dir / "gcc" / "cpp",
        "xgcc": build_dir / "gcc" / "xgcc",
        "as": build_dir / "gas" / "as-new",
    }
    hashes = {}
    for name, path in built.items():
        if not path.is_file():
            raise BuildError(f"build did not produce {path}")
        shutil.copy2(path, stage / name)
        (stage / name).chmod(0o755)
        hashes[name] = sha256(stage / name)
    (stage / "ee-gcc").write_text(EE_GCC_WRAPPER)
    (stage / "ee-gcc").chmod(0o755)
    (stage / "include").mkdir()
    for header, expected in GINCLUDE_HASHES.items():
        origin = source / "gcc" / "ginclude" / header
        if sha256(origin) != expected:
            raise BuildError(f"{origin} does not match the pinned hash")
        shutil.copy2(origin, stage / "include" / header)
        (stage / "include" / header).chmod(0o644)
    provenance = {
        "schema": "rnc-game-compiler-v1",
        "source": f"{ARCHIVE_NAME} sha256 {ARCHIVE_SHA256}",
        "patches": [path.name for path in patch_files()],
        "patch_sha256": {path.name: sha256(path) for path in patch_files()},
        "bison": str(bison),
        "hashes": hashes,
        "reference_hashes": REFERENCE_HASHES,
    }
    (stage / "provenance.json").write_text(json.dumps(provenance, indent=2) + "\n")
    if output.exists():
        backup = output.with_name(output.name + ".previous")
        if backup.exists():
            shutil.rmtree(backup)
        output.rename(backup)
    stage.rename(output)
    return provenance


def smoke_test(output: Path) -> None:
    probe = output / "smoke.c"
    probe.write_text("#include <stdarg.h>\nint smoke(int a, ...) { return a * 42; }\n")
    try:
        result = subprocess.run(
            [str(output / "ee-gcc"), "-O2", "-S", str(probe), "-o", str(output / "smoke.s")],
            capture_output=True, text=True,
        )
        if result.returncode != 0:
            raise BuildError(
                "the built compiler cannot compile a test file: "
                f"{result.stderr.strip()[-400:]}"
            )
        result = subprocess.run(
            [str(output / "as"), str(output / "smoke.s"), "-o", str(output / "smoke.o")],
            capture_output=True, text=True,
        )
        if result.returncode != 0:
            raise BuildError(
                "the built assembler cannot assemble the test output: "
                f"{result.stderr.strip()[-400:]}"
            )
    finally:
        for name in ("smoke.c", "smoke.s", "smoke.o"):
            (output / name).unlink(missing_ok=True)


def main(argv=None) -> int:
    args = parse_args(argv)
    if sys.platform != "linux":
        raise BuildError("the game compiler is a 32-bit Linux build; run this on Linux or WSL")
    for tool in ("gcc", "make", "git", "flex"):
        if shutil.which(tool) is None:
            raise BuildError(f"{tool} is not installed")
    work = args.work.expanduser().resolve()
    output = args.output.expanduser().resolve()
    work.mkdir(parents=True, exist_ok=True)

    if args.archive is not None:
        archive = args.archive.expanduser().resolve()
        if not archive.is_file():
            raise BuildError(f"archive not found: {archive}")
        if sha256(archive) != ARCHIVE_SHA256:
            raise BuildError(f"{archive} does not match the pinned sha256 {ARCHIVE_SHA256}")
    else:
        archive = work / ARCHIVE_NAME
        download(ARCHIVE_URLS, archive, ARCHIVE_SHA256)
    print(f"archive {archive.name} sha256 OK")

    source = extract_source(archive, work)
    apply_patches(source)
    print(f"{len(PATCH_ORDER)} patches applied")
    if args.check:
        return 0

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

    build_dir = build(source, work, bison, bison_env, args.jobs)
    provenance = install(source, build_dir, output, archive, bison)
    smoke_test(output)

    print("\ninstalled to", output)
    for name, digest in provenance["hashes"].items():
        marker = "" if digest == REFERENCE_HASHES[name] else "  (differs from reference)"
        print(f"  {name:<5} sha256 {digest}{marker}")
    print(
        "\nReference hashes identify the maintainer's build; other hosts may"
        " differ. `make elf` is the check."
    )
    return 0


if __name__ == "__main__":
    sys.exit(main())
