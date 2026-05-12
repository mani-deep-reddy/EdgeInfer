#!/usr/bin/env bash
# verify_qemu.sh — Build, run in QEMU, and verify output against golden reference
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

BUILD_DIR="$PROJECT_ROOT/build"
BINARY="$BUILD_DIR/app.elf"
GOLDEN="$PROJECT_ROOT/examples/simple_mlp/expected_output.txt"
QEMU_OUTPUT=$(mktemp /tmp/edgeinfer_qemu_output.XXXXXX)
cleanup() { rm -f "$QEMU_OUTPUT"; }
trap cleanup EXIT

echo "=== Building ==="
make -C "$PROJECT_ROOT" -f "$SCRIPT_DIR/Makefile" clean 2>&1
make -C "$PROJECT_ROOT" -f "$SCRIPT_DIR/Makefile" 2>&1

if [ ! -f "$BINARY" ]; then
    echo "ERROR: Build failed — binary not found"
    exit 1
fi

echo ""
echo "=== Running in QEMU ==="
# Run QEMU with 5-second timeout, capture serial output
timeout 5s qemu-system-arm \
    -M mps2-an385 \
    -kernel "$BINARY" \
    -nographic \
    2>&1 > "$QEMU_OUTPUT" || true

echo "=== Raw QEMU output ==="
cat "$QEMU_OUTPUT"

echo ""
echo "=== Extracting float output ==="
# Strip carriage returns, then extract float-valued lines
FLOAT_PATTERN='^[-+]?[0-9]+\.?[0-9]*([eE][-+]?[0-9]+)?$'
QEMU_FLOATS=$(tr -d '\r' < "$QEMU_OUTPUT" | grep -E "$FLOAT_PATTERN" || true)

if [ -z "$QEMU_FLOATS" ]; then
    echo "ERROR: No float output found in QEMU serial output"
    exit 1
fi

# Strip comment lines from golden reference
GOLDEN_FLOATS=$(grep -v '^#' "$GOLDEN" || true)

echo "=== Expected ==="
echo "$GOLDEN_FLOATS"
echo "=== Got ==="
echo "$QEMU_FLOATS"

if [ "$QEMU_FLOATS" = "$GOLDEN_FLOATS" ]; then
    echo ""
    echo "=== PASS: Output matches expected ==="
    exit 0
else
    echo ""
    echo "=== FAIL: Output does not match expected ==="
    diff <(echo "$GOLDEN_FLOATS") <(echo "$QEMU_FLOATS") || true
    exit 1
fi
