cmake_minimum_required(VERSION 3.20) # 最低版本 随便

cmake_policy(SET CMP0123 NEW) 

set(CMAKE_SYSTEM_NAME Generic) #单片机上裸机，不带系统 ps：搞了一个晚上CMAKE_SYSTEM_NAME SYSTEMP 多了一个P 我真服了

set(CPU_CORE cortex-m4) #自定义变量名 CPU核
set(CMAKE_SYSTEM_PROCESSOR ${CPU_CORE})

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/toolchain.cmake) # 工具链

project(output C ASM) #工程名，支持的语法 C ASM
set(CMAKE_C_STANDARD 99) # C语言标准
# set(CMAKE_CXX_STANDARD 17)

#---------------------------上述是固定模版---------------------------


#  编译选项 debug release
string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE_UPPER) # 将CMAKE_BUILD_TYPE的输入变量给到BUILD_TYPE_UPPER中

#
if(${BUILD_TYPE_UPPER} STREQUAL "RELEASE") #根据选项选择编译等级
    add_compile_options(-Os)
else()
    add_compile_options(-Og -g) # -g 带调试信息
endif()

# 全局宏定义
ADD_DEFINITIONS(
	-DUSE_STDPERIPH_DRIVER
    -DGD32F407
    -D__TARGET_FPU_VFP
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
)

# 源文件
file(GLOB SOURCE_FILE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/CMSIS/GD/GD32F4xx/Source/*.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/GD32F4xx_standard_peripheral/Source/*.c
    ${CMAKE_CURRENT_LIST_DIR}/app/*.c
    ${CMAKE_CURRENT_LIST_DIR}/module/soft_timer/soft_timer.c
    ${CMAKE_CURRENT_LIST_DIR}/user/bsp/bsp_uart.c
    ${CMAKE_CURRENT_LIST_DIR}/user/bsp/bsp_led.c
)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/output) # 输出文件路径
set(HEX_FILE ${EXECUTABLE_OUTPUT_PATH}/output.hex) #
set(BIN_FILE ${EXECUTABLE_OUTPUT_PATH}/output.bin)
set(MAP_FILE ${EXECUTABLE_OUTPUT_PATH}/output.map)

# 链接文件
if(USER_ARMGCC) #  armgcc
    set(STARTUP_FILE ${CMAKE_CURRENT_LIST_DIR}/startup/GCC/startup_gd32f4xx.s) # 启动文件
    set(LINK_FILE ${CMAKE_CURRENT_LIST_DIR}/startup/linker_scripts/link.ld) # 链接文件

    # 编译选项
    add_compile_options(
        -mcpu=${CPU_CORE}
        -mthumb
        -mthumb-interwork
        -ffunction-sections
        -fdata-sections
        -fno-common
        -fmessage-length=0
    )

    # 链接选项
    add_link_options(
        -Wl,-Map=${MAP_FILE}
        -Wl,--print-memory-usage
        -Wl,--gc-sections
        -T ${LINK_FILE}
        # --library_type=microlib
    )

else() #  armclang
    set(STARTUP_FILE ${CMAKE_CURRENT_LIST_DIR}/startup/ARM/startup_gd32f407_427.s) # 启动文件
    set(LINK_FILE ${CMAKE_CURRENT_LIST_DIR}/startup/linker_scripts/link.sct) # 链接文件

    add_compile_options(
        --target=arm-arm-none-eabi
        -mcpu=${CPU_CORE}
        -masm=armasm
        -Wa,armasm,--diag_suppress=A1950W
        -gdwarf-4
    )

    add_link_options(
        --strict
        --scatter ${LINK_FILE}
        --info=sizes,totals,unused,veneers
        --map
        --list=${MAP_FILE}
        # --library_type=microlib
    )
    
endif()



if(USER_ARMGCC) 
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
else()
    add_executable(${PROJECT_NAME} ${SOURCE_FILE} ${STARTUP_FILE}) # 创建可执行文件
    #  设置可执行文件输出路径
    set_target_properties(${PROJECT_NAME} PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
    )
    
    add_custom_command(
        TARGET ${PROJECT_NAME} # 目标文件
        POST_BUILD #  编译完成之后执行
        COMMAND ${CMAKE_OBJCOPY} -i32 --output=${HEX_FILE} $<TARGET_FILE:${PROJECT_NAME}>
        COMMAND ${CMAKE_OBJCOPY} -bin --output=${BIN_FILE} $<TARGET_FILE:${PROJECT_NAME}>
        COMMAND ${CMAKE_SIZE} --info=sizes $<TARGET_FILE:${PROJECT_NAME}>
    )   
endif()