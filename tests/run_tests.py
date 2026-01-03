#!/usr/bin/env python3
import subprocess
import sys
from pathlib import Path

BIN = Path(__file__).resolve().parents[1] / "abstract_vm"
TESTS_DIR = Path(__file__).resolve().parent

STDIN_DIR = TESTS_DIR / "stdin"


def ensure_stdin_terminator(src: str) -> str:
    s = src.replace("\r\n", "\n").replace("\r", "\n")
    lines = s.split("\n")
    if any(line.strip() == ";;" for line in lines):
        return s if s.endswith("\n") else s + "\n"
    if not s.endswith("\n"):
        s += "\n"
    return s + ";;\n"


def run_process(args, stdin_text=None):
    return subprocess.run(
        args,
        input=stdin_text,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )


def check_expected(avm_path: Path, stdout: str, stderr: str, code: int):
    out_path = avm_path.with_suffix(".out")
    err_path = avm_path.with_suffix(".err")

    # OK case: must have .out
    if out_path.exists():
        expected = out_path.read_text()
        if code != 0:
            return False, f"Expected exit code 0, got {code}\nStderr:\n{stderr}"
        if stdout != expected:
            return False, (
                "STDOUT mismatch\n"
                f"--- expected ({out_path.name}) ---\n{expected}\n"
                f"--- got ---\n{stdout}\n"
            )
        if err_path.exists():
            needle = err_path.read_text().strip()
            if needle and needle not in stderr:
                return False, f"Expected stderr to contain: {needle}\nGot:\n{stderr}"
        return True, "OK"

    # ERROR case: must have .err
    if err_path.exists():
        needle = err_path.read_text().strip()
        if code == 0:
            return False, f"Expected non-zero exit code, got 0\nStdout:\n{stdout}"
        if needle and needle not in stderr:
            return False, f"Expected stderr to contain: {needle}\nGot:\n{stderr}"
        return True, "OK"

    return False, "No expected .out or .err file found for this test"


def run_one_file_mode(avm_path: Path):
    proc = run_process([str(BIN), str(avm_path)])
    return check_expected(avm_path, proc.stdout, proc.stderr, proc.returncode)


def run_one_stdin_mode(avm_path: Path):
    src = avm_path.read_text()
    stdin_payload = ensure_stdin_terminator(src)
    proc = run_process([str(BIN)], stdin_text=stdin_payload)
    return check_expected(avm_path, proc.stdout, proc.stderr, proc.returncode)


def collect_file_tests():
    avms = sorted(TESTS_DIR.rglob("*.avm"))
    return [p for p in avms if p.is_file() and STDIN_DIR not in p.parents]


def collect_stdin_tests():
    if not STDIN_DIR.exists():
        return []
    return sorted([p for p in STDIN_DIR.rglob("*.avm") if p.is_file()])


def main():
    if not BIN.exists():
        print(f"Binary not found: {BIN}", file=sys.stderr)
        return 2

    file_tests = collect_file_tests()
    stdin_tests = collect_stdin_tests()

    if not file_tests and not stdin_tests:
        print("No tests found.", file=sys.stderr)
        return 2

    failed = 0
    total = 0

    for t in file_tests:
        total += 1
        ok, msg = run_one_file_mode(t)
        rel = t.relative_to(TESTS_DIR)
        if ok:
            print(f"\033[1;32m[PASS] {rel}\033[0m")
        else:
            failed += 1
            print(f"\033[1;31m[FAIL] {rel}\n{msg}\033[0m")

    for t in stdin_tests:
        total += 1
        ok, msg = run_one_stdin_mode(t)
        rel = t.relative_to(TESTS_DIR)
        label = f"{rel} (stdin)"
        if ok:
            print(f"\033[1;32m[PASS] {label}\033[0m")
        else:
            failed += 1
            print(f"\033[1;31m[FAIL] {label}\n{msg}\033[0m")

    print(f"\nSummary: {total - failed}/{total} passed")
    return 0 if failed == 0 else 1

if __name__ == "__main__":
    raise SystemExit(main())
