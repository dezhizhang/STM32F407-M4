#!/usr/bin/env bash
# 一键编译 + 烧录（ST-Link/DAP 经 openocd.cfg 选择）
set -euo pipefail
cd "$(dirname "$0")/.."

BUILD_DIR=build
ELF="$BUILD_DIR/STM32F407_M4.elf"

cmake --build "$BUILD_DIR"
openocd -f openocd.cfg -c "program $ELF verify reset exit"
