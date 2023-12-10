#!/usr/bin/env sh

restore_dir=$(pwd)
cd $(git rev-parse --show-toplevel)

set -e
meson setup _builds --prefix dist
meson install -C _builds

cd "$restore_dir"
