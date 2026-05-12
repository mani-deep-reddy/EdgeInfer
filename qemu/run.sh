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

echo "=== Running EdgeInfer in QEMU ==="
qemu-system-arm \
    -M mps2-an385 \
    -kernel "$BINARY" \
    -nographic \
    2>&1
