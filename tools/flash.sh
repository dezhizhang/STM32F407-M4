#!/usr/bin/env bash
# 一键编译 + 烧录（兼容入口，逻辑见 Makefile 的 flash 目标）
set -euo pipefail
cd "$(dirname "$0")/.."
exec make flash "$@"
