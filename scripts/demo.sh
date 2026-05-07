#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

echo "=== EdgeInfer Demo ==="
echo "Building..."
bash "$SCRIPT_DIR/build.sh"

echo ""
echo "Running in QEMU..."
bash "$PROJECT_ROOT/qemu/run.sh"
