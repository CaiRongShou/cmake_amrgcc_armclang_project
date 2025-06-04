set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY) #amrgcc与armclang区别 amrgcc 裸机需要关闭掉一些文件 
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER) # 查找编译器时不使用根目录
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY) #  查找库文件时只使用根目录
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY) #  查找头文件时只使用根目录
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY) # 查找包时只使用根目录
option(USER_ARMGCC "user armgcc" OFF) # 定义类似一个宏，参数1 宏名称，参数2 宏定义内容，参数3 是否定义

if(USER_ARMGCC)
    set(COMPATIBLE_ROOT_PATH C:/SysGCC/arm-eabi/bin) # armgcc路径
    set(CMAKE_C_COMPILER ${COMPATIBLE_ROOT_PATH}/arm-none-eabi-gcc.exe) # armgcc c编译器路径
    set(CMAKE_ASM_COMPILER ${COMPATIBLE_ROOT_PATH}/arm-none-eabi-gcc.exe) # armgcc asm编译器路径
    set(CMAKE_OBJCOPY ${COMPATIBLE_ROOT_PATH}/arm-none-eabi-objcopy.exe)
    set(CMAKE_SIZE ${COMPATIBLE_ROOT_PATH}/arm-none-eabi-size.exe)
else() # armclang
    set(COMPATIBLE_ROOT_PATH C:/Keil_v5/ARM/ARMCLANG/bin) # armclang路径
    set(CMAKE_C_COMPILER ${COMPATIBLE_ROOT_PATH}/armclang.exe) # armclang c编译器路径
    set(CMAKE_ASM_COMPILER ${COMPATIBLE_ROOT_PATH}/armclang.exe) # armclang asm编译器路径
    set(CMAKE_OBJCOPY ${COMPATIBLE_ROOT_PATH}/fromelf.exe)
    set(CMAKE_SIZE ${COMPATIBLE_ROOT_PATH}/fromelf.exe)
endif()