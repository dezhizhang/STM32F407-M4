# ---------------------------------------------------------------
# 正点原子探索者 STM32F407ZGT6 工程构建
#
#   cmake -B build -DCMAKE_TOOLCHAIN_FILE=cmake/stm32-toolchain.cmake
#   cmake --build build
# ---------------------------------------------------------------
cmake_minimum_required(VERSION 3.20)

project(STM32F407_M4 C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)   # 给 clangd / VSCode 用

set(TARGET ${PROJECT_NAME})

# ---------------- 源码 ----------------
set(HAL_DIR     ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver)
set(CMSIS_DIR   ${CMAKE_SOURCE_DIR}/Drivers/CMSIS)

file(GLOB HAL_SOURCES CONFIGURE_DEPENDS ${HAL_DIR}/Src/*.c)
# *_template.c 是 ST 提供的可选参考实现（timebase/bsp 模板），不参与编译
list(FILTER HAL_SOURCES EXCLUDE REGEX "_template\\.c$")

add_executable(${TARGET}.elf
  Core/Src/main.c
  Core/Src/stm32f4xx_it.c
  Core/Src/stm32f4xx_hal_msp.c
  Core/Src/system_stm32f4xx.c
  Core/Src/syscalls.c
  App/Src/led.c
  App/Src/key.c
  App/Src/usart.c
  ${CMSIS_DIR}/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f407xx.s
  ${HAL_SOURCES}
)

# ---------------- 头文件 ----------------
target_include_directories(${TARGET}.elf PRIVATE
  Core/Inc
  App/Inc
  ${HAL_DIR}/Inc
  ${HAL_DIR}/Inc/Legacy
  ${CMSIS_DIR}/Include
  ${CMSIS_DIR}/Device/ST/STM32F4xx/Include
)

# ---------------- 宏定义 ----------------
target_compile_definitions(${TARGET}.elf PRIVATE
  STM32F407xx          # 器件型号
  USE_HAL_DRIVER       # 使用 HAL 而非 LL
)

# ---------------- 编译/链接选项 ----------------
target_compile_options(${TARGET}.elf PRIVATE
  -Wall -Wextra
  -ffunction-sections -fdata-sections
  -fno-common
  -fstack-usage
  $<$<CONFIG:Debug>:-Og -g3>
  $<$<CONFIG:Release>:-O2>
  $<$<CONFIG:MinSizeRel>:-Os>
)

target_link_options(${TARGET}.elf PRIVATE
  -T${CMAKE_SOURCE_DIR}/stm32f407zgtx.ld
  -nostartfiles            # 不链接 newlib 的 crt0，入口由启动文件 Reset_Handler 接管
  -Wl,--gc-sections
  -Wl,-Map=${CMAKE_BINARY_DIR}/${TARGET}.map
  -Wl,--print-memory-usage
  -Wl,--start-group -lc -lm -Wl,--end-group
)

set_target_properties(${TARGET}.elf PROPERTIES
  OUTPUT_NAME "${TARGET}"      # target 名已含 .elf，组合出 STM32F407_M4.elf
  SUFFIX ".elf"
  LINK_DEPENDS ${CMAKE_SOURCE_DIR}/stm32f407zgtx.ld
)

# ---------------- 构建后：生成 hex/bin 并打印体积 ----------------
add_custom_command(TARGET ${TARGET}.elf POST_BUILD
  COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${TARGET}.elf>
  COMMAND ${CMAKE_OBJCOPY} -O ihex   $<TARGET_FILE:${TARGET}.elf> ${CMAKE_BINARY_DIR}/${TARGET}.hex
  COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${TARGET}.elf> ${CMAKE_BINARY_DIR}/${TARGET}.bin
  COMMENT "Generating HEX/BIN"
)
