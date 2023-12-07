#!/usr/bin/env sh

restore_dir=$(pwd)
cd $(get rev-parse --show-toplevel)

set -e
meson setup _builds
meson compile -C _builds app

cd "$restore_dir"
