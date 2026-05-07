#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

echo "=== EdgeInfer Build ==="
make -C "$PROJECT_ROOT" -f "$SCRIPT_DIR/Makefile" "$@"
echo "=== Build complete ==="
