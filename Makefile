# ---------------------------------------------------------------
# 正点原子探索者 STM32F407ZGT6 — Makefile（包装 CMake + Ninja + OpenOCD）
#
#   make            编译（首次自动 configure）
#   make flash      编译 + 烧录 + 复位运行
#   make erase      整片擦除 Flash
#   make size       打印 Flash/RAM 占用
#   make monitor    串口终端（自动探测 /dev/tty.usbserial-*）
#   make debug      启动 OpenOCD GDB server（:3333）
#   make gdb        arm-none-eabi-gdb 连接（另开终端，先 make debug）
#   make clean      删除 build/
#   make rebuild    全量重编
#
#   可覆盖变量：make BUILD_TYPE=Release flash
#              make monitor SERIAL=/dev/tty.usbserial-XXXX
# ---------------------------------------------------------------

BUILD_DIR   := build
BUILD_TYPE  ?= Debug
TOOLCHAIN   := cmake/stm32-toolchain.cmake
OPENOCD_CFG := openocd.cfg
ELF         := $(BUILD_DIR)/STM32F407_M4.elf

# 板载 CH340G：Apple 驱动 / WCH 驱动两种设备名都探测
SERIAL      ?= $(firstword $(wildcard /dev/tty.usbserial-* /dev/tty.wchusbserial*))
BAUD        ?= 115200

.PHONY: all configure flash erase size monitor debug gdb clean rebuild

# build.ninja 不存在或构建脚本变了就重新 configure（cmake -B 幂等）
$(BUILD_DIR)/build.ninja: CMakeLists.txt $(TOOLCHAIN) stm32f407zgtx.ld
	cmake -B $(BUILD_DIR) -G Ninja \
	  -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

all: $(BUILD_DIR)/build.ninja
	cmake --build $(BUILD_DIR)

# 强制重新 configure（切换 BUILD_TYPE 用）
configure:
	cmake -B $(BUILD_DIR) -G Ninja \
	  -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

flash: all
	openocd -f $(OPENOCD_CFG) -c "program $(ELF) verify reset exit"

erase:
	openocd -f $(OPENOCD_CFG) -c "init; reset halt; stm32f4x mass_erase 0; exit"

size: all
	arm-none-eabi-size $(ELF)

monitor:
ifeq ($(strip $(SERIAL)),)
	@echo "错误：未找到 /dev/tty.usbserial-* 或 /dev/tty.wchusbserial*"
	@echo "用法：make monitor SERIAL=/dev/tty.usbserial-XXXX"
	@exit 1
else
	@echo "打开 $(SERIAL) @ $(BAUD)，退出 screen：Ctrl-A 然后 K"
	screen $(SERIAL) $(BAUD)
endif

debug:
	openocd -f $(OPENOCD_CFG)

gdb: all
	arm-none-eabi-gdb $(ELF) \
	  -ex 'target remote :3333' -ex 'monitor reset halt'

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean all
