#!/usr/bin/env python3
import subprocess
import sys
from pathlib import Path

BIN = Path(__file__).resolve().parents[1] / "abstract_vm"
TESTS_DIR = Path(__file__).resolve().parent

def run_one(avm_path: Path):
    # Decide expected files
    out_path = avm_path.with_suffix(".out")
    err_path = avm_path.with_suffix(".err")

    proc = subprocess.run(
        [str(BIN), str(avm_path)],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )

    stdout = proc.stdout
    stderr = proc.stderr
    code = proc.returncode

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
        # If .err exists too, check it appears (optional)
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

def collect_tests():
    # any .avm under ok/ and err/
    avms = sorted(TESTS_DIR.rglob("*.avm"))
    return [p for p in avms if p.is_file()]

def main():
    if not BIN.exists():
        print(f"Binary not found: {BIN}", file=sys.stderr)
        return 2

    tests = collect_tests()
    if not tests:
        print("No tests found.", file=sys.stderr)
        return 2

    failed = 0
    for t in tests:
        ok, msg = run_one(t)
        rel = t.relative_to(TESTS_DIR)
        if ok:
            print(f"\033[1;32m[PASS] {rel}\033[0m")
        else:
            failed += 1
            print(f"\033[1;31m[FAIL] {rel}\n{msg}\033[0m")

    print(f"\nSummary: {len(tests)-failed}/{len(tests)} passed")
    return 0 if failed == 0 else 1

if __name__ == "__main__":
    raise SystemExit(main())
