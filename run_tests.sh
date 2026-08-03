#!/bin/sh
# Runs every built program. Each one ends with assert-based self tests, so a
# non-zero exit status means a test failed.

set -u

BIN_DIR="$(dirname "$0")/bin"
failed=0
total=0

if [ ! -d "$BIN_DIR" ]; then
    echo "No bin/ directory. Run 'make' first."
    exit 1
fi

for program in "$BIN_DIR"/*; do
    # -g skips the .dSYM debug bundles the compiler leaves next to binaries.
    [ -f "$program" ] && [ -x "$program" ] || continue
    total=$((total + 1))
    name=$(basename "$program")

    if output=$("$program" 2>&1); then
        printf 'PASS  %s\n' "$name"
    else
        printf 'FAIL  %s\n' "$name"
        printf '%s\n' "$output" | sed 's/^/      /'
        failed=$((failed + 1))
    fi
done

echo
if [ "$failed" -eq 0 ]; then
    echo "All $total programs passed."
    exit 0
fi

echo "$failed of $total programs failed."
exit 1
