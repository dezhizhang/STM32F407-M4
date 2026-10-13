# STM32F407-M4 — 正点原子探索者开发板工程

基于 **STM32CubeF4 HAL 库 + CMake + arm-none-eabi-gcc** 的裸机工程模板，
适用于正点原子（ALIENTEK）探索者 STM32F407ZGT6 开发板。

## 板卡信息

| 项目 | 说明 |
|---|---|
| MCU | STM32F407ZGT6（Cortex-M4F @ 168MHz, 1MB Flash, 192KB RAM） |
| 晶振 | HSE 8MHz（PLL 倍频至 168MHz）、LSE 32.768kHz |
| LED | LED0=PF9、LED1=PF10（低电平点亮） |
| 按键 | KEY0=PE4、KEY1=PE3、KEY2=PE2（低有效）、WK_UP=PA0（高有效） |
| 串口 | USART1: PA9-TX / PA10-RX，115200-8-N-1（板载 CH340G） |

## 目录结构

```
├── App/                 用户驱动层（LED / KEY / USART，仿正点原子 HARDWARE 风格）
├── Core/
│   ├── Inc/             main.h、stm32f4xx_hal_conf.h（HAL 裁剪配置）
│   └── Src/             main.c、中断、MSP、newlib syscalls（printf 重定向）
├── Drivers/             ST 官方库（vendor，勿手改）
│   ├── CMSIS/           内核头文件 + 器件支持包（含启动文件）
│   └── STM32F4xx_HAL_Driver/
├── cmake/               交叉编译工具链文件
├── stm32f407zgtx.ld     链接脚本（1MB Flash / 128K RAM / 64K CCMRAM）
├── openocd.cfg          OpenOCD 烧录调试配置（ST-Link / DAP 可切换）
└── tools/flash.sh       一键编译烧录脚本
```

## 构建

```bash
# 首次配置（工具链文件必须绝对/相对路径传入）
cmake -B build -DCMAKE_TOOLCHAIN_FILE=cmake/stm32-toolchain.cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug

# 编译（输出 build/STM32F407_M4.elf/.hex/.bin + .map）
cmake --build build
```

> Debug 用 `-Og -g3` 便于调试，Release 用 `-O2`。

## 烧录 / 调试

调试器默认 **ST-Link**；用 DAP-Link（ATK-DAP）的话改 `openocd.cfg` 里 interface 那两行。

```bash
# 一键编译烧录
./tools/flash.sh

# 或手动烧录
openocd -f openocd.cfg -c "program build/STM32F407_M4.elf verify reset exit"

# 命令行 GDB 调试（另开终端）
openocd -f openocd.cfg
arm-none-eabi-gdb build/STM32F407_M4.elf -ex 'target remote :3333' -ex 'monitor reset halt'
```

VSCode 用户推荐 **Cortex-Debug** 插件（`servertype: openocd`，configFiles 指向本目录 `openocd.cfg`）。

## 串口

USB 线接板上 **USB_TTL（CH340G）** 口，`ls /dev/tty.usbserial-*` 找到设备后：

```bash
screen /dev/tty.usbserial-XXXX 115200   # 或 minicom / picocom
```

上电后每 500ms LED0（PF9）翻转；按 KEY0 翻转 LED1，KEY1/KEY2/WK_UP 打印消息。

## 加新外设的姿势

1. `Core/Inc/stm32f4xx_hal_conf.h` 打开对应 `HAL_XXX_MODULE_ENABLED`；
2. `App/` 下新建模块（参考 led.c）并加进 `CMakeLists.txt` 的源码列表；
3. 引脚/时钟初始化放 `Core/Src/stm32f4xx_hal_msp.c` 对应 `HAL_xxx_MspInit`。

## printf 说明

本机 newlib（4.5.0，源码编译）已启用 `%f`/`%lf` 浮点与 C99 格式支持，printf 可直接打印浮点。
系统调用重定向在 `Core/Src/syscalls.c`（`_write` → USART1），`-nostartfiles` 模式下由该文件提供 `_init`/`_fini` 空桩。

## 工具链备注（本机环境）

- arm-none-eabi-gcc 16.2.0（Homebrew）+ newlib 4.5.0（源码编译，安装于 `/opt/homebrew/opt/arm-none-eabi-gcc`）
- Homebrew core 已移除 `arm-none-eabi-newlib` formula，**brew 升级 GCC 后 newlib 会丢失**，
  需重新源码编译安装到新 keg（configure `--target=arm-none-eabi --prefix=$(brew --prefix arm-none-eabi-gcc)`），否则报 `stdint.h` 找不到
- vanilla newlib 无 `libc_nano.a`/`nano.specs`，故链接不用 `-specs`，改 `-nostartfiles` + 自带 syscalls

## 版本

- STM32CubeF4 HAL Driver / CMSIS Device F4：master（2026-10 vendor）
