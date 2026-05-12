#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BINARY="$PROJECT_ROOT/build/app.elf"

if [ ! -f "$BINARY" ]; then
    echo "Error: Binary not found at $BINARY"
    echo "Run 'make' first."
    exit 1
fi

echo "=== Starting EdgeInfer in QEMU (GDB server on port 1234) ==="
echo "Connect with: arm-none-eabi-gdb $BINARY"
echo "  (gdb) target remote :1234"

qemu-system-arm \
    -M mps2-an385 \
    -kernel "$BINARY" \
    -nographic \
    -s -S \
    2>&1
