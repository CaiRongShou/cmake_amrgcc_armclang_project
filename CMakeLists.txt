cmake_minimum_required(VERSION 3.20) # 最低版本 随便

cmake_policy(SET CMP0123 NEW) 

set(CMAKE_SYSTEM_NAME Generic) #单片机上裸机，不带系统

set(CPU_CORE cortex-m4) #自定义变量名 CPU核
set(CMAKE_SYSTEM_PROCESSOR ${CPU_CORE})

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/toolchain.cmake) # 工具链

project(output C CXX ASM) #工程名，支持的语法 C ASM
set(CMAKE_C_STANDARD 99) # C语言标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#---------------------------上述是固定模版---------------------------


#  编译选项 debug release
string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE_UPPER) # 将CMAKE_BUILD_TYPE的输入变量给到BUILD_TYPE_UPPER中

#
if(${BUILD_TYPE_UPPER} STREQUAL "RELEASE") #根据选项选择编译等级
    add_compile_options(-O3)
else()
    add_compile_options(-Og -g3) # -g 带调试信息
endif()

# 全局宏定义
ADD_DEFINITIONS(
	-DUSE_STDPERIPH_DRIVER
    -DGD32F407
    # -D__TARGET_FPU_VFP
)

# 头文件
include_directories(
    ${CMAKE_CURRENT_LIST_DIR}/libraries/GD32F4xx_standard_peripheral/Include
    ${CMAKE_CURRENT_LIST_DIR}/libraries/CMSIS
    ${CMAKE_CURRENT_LIST_DIR}/libraries/CMSIS/GD/GD32F4xx/Include
    ${CMAKE_CURRENT_LIST_DIR}/app/inc
    ${CMAKE_CURRENT_LIST_DIR}/user/bsp/inc
    ${CMAKE_CURRENT_LIST_DIR}/user/sys/inc
    ${CMAKE_CURRENT_LIST_DIR}/module/soft_timer
    ${CMAKE_CURRENT_LIST_DIR}/libraries/xprintf/src
    ${CMAKE_CURRENT_LIST_DIR}/libraries/lwip-2.1.3/arch
    ${CMAKE_CURRENT_LIST_DIR}/libraries/lwip-2.1.3/src/include
    ${CMAKE_CURRENT_LIST_DIR}/libraries/lwip-2.1.3

    ${CMAKE_CURRENT_LIST_DIR}/libraries/CherryUSB-1.5.0
    ${CMAKE_CURRENT_LIST_DIR}/libraries/CherryUSB-1.5.0/class/cdc
    ${CMAKE_CURRENT_LIST_DIR}/libraries/CherryUSB-1.5.0/common
    ${CMAKE_CURRENT_LIST_DIR}/libraries/CherryUSB-1.5.0/core
    ${CMAKE_CURRENT_LIST_DIR}/libraries/CherryUSB-1.5.0/port/dwc2
)

# 源文件
file(GLOB DIRVES_FILE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/CMSIS/GD/GD32F4xx/Source/system_gd32f4xx.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/GD32F4xx_standard_peripheral/Source/*.c
    ${CMAKE_CURRENT_LIST_DIR}/app/*.c
    ${CMAKE_CURRENT_LIST_DIR}/module/soft_timer/soft_timer.c
    ${CMAKE_CURRENT_LIST_DIR}/user/bsp/bsp_uart.c
    ${CMAKE_CURRENT_LIST_DIR}/user/bsp/bsp_led.c
    
    ${CMAKE_CURRENT_LIST_DIR}/user/bsp/bsp_enet.c

    ${CMAKE_CURRENT_LIST_DIR}/user/sys/sdcard.c

    ${CMAKE_CURRENT_LIST_DIR}/libraries/xprintf/src/xprintf.c

    ${CMAKE_CURRENT_LIST_DIR}/libraries/lwip-2.1.3/src/core/*.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/lwip-2.1.3/src/core/ipv4/*.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/lwip-2.1.3/src/api/*.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/lwip-2.1.3/src/netif/ethernet.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/lwip-2.1.3/arch/ethernetif.c

    ${CMAKE_CURRENT_LIST_DIR}/libraries/CherryUSB-1.5.0/class/cdc/usbd_cdc_acm.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/CherryUSB-1.5.0/core/usbd_core.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/CherryUSB-1.5.0/port/dwc2/usb_dc_dwc2.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/CherryUSB-1.5.0/port/dwc2/usb_glue_gd.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/CherryUSB-1.5.0/demo/cdc_acm_template.c    
)
# 源文件
if(USER_ARMGCC) #  armgcc
file(GLOB SYSTEM_FILE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/CMSIS/GD/GD32F4xx/Source/syscalls.c
)
    set(SOURCE_FILE ${DIRVES_FILE} ${SYSTEM_FILE})
else() #  armclang
    set(SOURCE_FILE ${DIRVES_FILE})

endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/output) # 输出文件路径
set(HEX_FILE ${EXECUTABLE_OUTPUT_PATH}/output.hex) #
set(BIN_FILE ${EXECUTABLE_OUTPUT_PATH}/output.bin)
set(MAP_FILE ${EXECUTABLE_OUTPUT_PATH}/output.map)

if(USER_ARMGCC) #  armgcc
    set(STARTUP_FILE ${CMAKE_CURRENT_LIST_DIR}/startup/GCC/startup_gd32f407_427.S) # 启动文件
    set(LINK_FILE ${CMAKE_CURRENT_LIST_DIR}/startup/linker_scripts/gd32f4xx_flash.ld) # 链接文件

    set(CPU_FLAGS "-mthumb -mcpu=${CPU_CORE} -mfloat-abi=hard -mfpu=fpv4-sp-d16")
    set(SPECS_FLAGS "--specs=nosys.specs --specs=nano.specs")
    set(COMMON_FLAGS "-ffunction-sections -fdata-sections -Wall -Wdouble-promotion -Wno-sign-compare -Wno-psabi -g3 -ggdb3")
    set(ASM_FLAGS "-x assembler-with-cpp")
    set(CXX_FLAGS "-fno-rtti -fno-exceptions -fno-threadsafe-statics -Wsuggest-override -Wno-register")

    set(CMAKE_C_FLAGS "${CPU_FLAGS} ${COMMON_FLAGS} ${SPECS_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CPU_FLAGS} ${COMMON_FLAGS} ${SPECS_FLAGS} ${CXX_FLAGS}")
    set(CMAKE_ASM_FLAGS "${CPU_FLAGS} ${SPECS_FLAGS} -x assembler-with-cpp")

    # 链接选项
    set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections,--no-warn-rwx-segments,--print-memory-usage")
    
    add_link_options(
        -Wl,-Map=${MAP_FILE}
        ${CMAKE_EXE_LINKER_FLAGS}
        -T ${LINK_FILE}
    )
    
    add_executable(${PROJECT_NAME}.elf ${SOURCE_FILE} ${STARTUP_FILE}) # 创建可执行文件
    
    #  设置可执行文件输出路径
    set_target_properties(${PROJECT_NAME}.elf PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
    )

    add_custom_command(
        TARGET ${PROJECT_NAME}.elf # 目标文件
        POST_BUILD #  编译完成之后执行
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${PROJECT_NAME}.elf>
    )   

else() #  armclang
    set(STARTUP_FILE ${CMAKE_CURRENT_LIST_DIR}/startup/ARM/startup_gd32f407_427.s) # 启动文件
    set(LINK_FILE ${CMAKE_CURRENT_LIST_DIR}/startup/linker_scripts/link.sct) # 链接文件

    if(NOT EXISTS ${LINK_FILE})
        message(FATAL_ERROR "Link script not found: ${LINK_FILE}")
    endif()

    set(C_CXX_COMPILE_OPTIONS
        # -xc -std=c11 
        -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -c
        -fno-rtti -funsigned-char -fshort-enums -fshort-wchar -fno-exceptions
        -O1 -ffunction-sections
        -Wno-packed -Wno-missing-variable-declarations -Wno-missing-prototypes
        -Wno-missing-noreturn -Wno-sign-conversion -Wno-nonportable-include-path
        -Wno-reserved-id-macro -Wno-unused-macros -Wno-documentation-unknown-command
        -Wno-documentation -Wno-license-management -Wno-parentheses-equality
        -Wno-reserved-identifier
        )

    set(ASM_COMPILE_OPTIONS
        --target=arm-arm-none-eabi -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -masm=auto -c
        -Wa,armasm,--diag_suppress=A1950W -c
        )
        
    set_source_files_properties(${CMAKE_CURRENT_LIST_DIR}/startup/startup_gd32f407_427.s PROPERTIES LANGUAGE ASM)

    add_executable(${PROJECT_NAME} ${SOURCE_FILE} ${STARTUP_FILE}) # 创建可执行文件

    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<COMPILE_LANGUAGE:C>:${C_CXX_COMPILE_OPTIONS}>
        $<$<COMPILE_LANGUAGE:CXX>:${C_CXX_COMPILE_OPTIONS}>
        $<$<COMPILE_LANGUAGE:ASM>:${ASM_COMPILE_OPTIONS}>
    )

    target_link_options(${PROJECT_NAME} PRIVATE
        --cpu=Cortex-M4.fp.sp
        --scatter=${LINK_FILE}
        --summary_stderr
        --map
        --info=totals
        --info=sizes
        --list=${CMAKE_BINARY_DIR}/${PROJECT_NAME}.map
    )


    #  设置可执行文件输出路径
    set_target_properties(${PROJECT_NAME} PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
    )

    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} --i32combined --output=${HEX_FILE} $<TARGET_FILE:${PROJECT_NAME}>
        COMMAND ${CMAKE_OBJCOPY} --bincombined --output=${BIN_FILE} $<TARGET_FILE:${PROJECT_NAME}>
        COMMAND ${CMAKE_SIZE} --text --info=sizes $<TARGET_FILE:${PROJECT_NAME}>
    )

endif()

