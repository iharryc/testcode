#---------------------------------------------------------------------
# 
# Modified by HaeRyong Kim (harry) on 04/16/2022
#
#---------------------------------------------------------------------


set(myProjectName "TEST-SW")
# 프로젝트네임     ^^^^^^^
set(myMCU "at90can128")
# 사용MCU  ^^^^^^^^^^

add_definitions(  # 전처리기에 define 정의. 컴파일러 옵션중 -D에 해당.
  # 내장함수 _delay_us(), _delay_ms() 이용시 선언필요
  -DF_CPU=16000000UL  
# 동작주파수 ^^^^^^^^ 
)
set(myCOMPILER_ROOT "C:/DevFiles/Compilers/avr-gcc")
# 컴타일러 위치      ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^               


# set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)


#---------------------------------------------------------------------
# 컴파일러(툴체인) 경로설정
#---------------------------------------------------------------------

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)


set(CMAKE_C_COMPILER   "${myCOMPILER_ROOT}/bin/avr-gcc.exe"     CACHE FILEPATH "C Compiler path")
set(CMAKE_ASM_COMPILER "${myCOMPILER_ROOT}/bin/avr-gcc.exe"     CACHE FILEPATH "ASM Compiler path")
set(CMAKE_CXX_COMPILER "${myCOMPILER_ROOT}/bin/avr-g++.exe"     CACHE FILEPATH "C++ Compiler path")
set(CMAKE_OBJCOPY      "${myCOMPILER_ROOT}/bin/avr-objcopy.exe" CACHE FILEPATH "objcopy tool")
set(CMAKE_OBJDUMP      "${myCOMPILER_ROOT}/bin/avr-objdump.exe" CACHE FILEPATH "objdump tool")
set(CMAKE_SIZE_UTIL    "${myCOMPILER_ROOT}/bin/avr-size.exe"    CACHE FILEPATH "size tool")


# Disable compiler checks.
set(CMAKE_C_COMPILER_FORCED   TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)

set(CMAKE_FIND_ROOT_PATH ${myCOMPILER_ROOT})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
# not added automatically, since CMAKE_SYSTEM_NAME is "generic"
set(CMAKE_SYSTEM_INCLUDE_PATH "${CMAKE_FIND_ROOT_PATH}/include")
set(CMAKE_SYSTEM_LIBRARY_PATH "${CMAKE_FIND_ROOT_PATH}/lib")

# end of toolchain
#---------------------------------------------------------------------



cmake_minimum_required(VERSION 3.0.0)
project( ${myProjectName} LANGUAGES ASM C CXX )

include_directories (
  src 
  src/include             #
)


link_directories (
  src
  src/lib
)



set(EXECUTABLE ${PROJECT_NAME}.elf)
# 해당 폴더의 파일만 찾는다.
file(GLOB SRC_FILES CONFIGURE_DEPENDS *.c *.cpp )
# 해당 폴더를 포함해서 하위 폴더까지의 파일도 찾는다.
file(GLOB_RECURSE SRC_FILES_RECURSE CONFIGURE_DEPENDS src/*.c src/*.cpp )
add_executable(${EXECUTABLE}  ${SRC_FILES} ${SRC_FILES_RECURSE} )



# Compiler options
target_compile_options (${EXECUTABLE} PRIVATE
  -mmcu=${myMCU} -Wall -std=gnu99
  # -O2  # Optimization level[0, 1, 2, 3, s]. 0=turn off, s=size.
  # -g2  # generate debugging information level[0, 1, 2, 3]. 0=turn off
  # -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
  # -fdata-sections -ffunction-sections -flto -fno-fat-lto-objects -MMD  
)

# Linker options
target_link_options (${EXECUTABLE} PRIVATE
  -mmcu=${myMCU} -Wl,-Map=${PROJECT_NAME}.map
  # -flto -fuse-linker-plugin -Wl,--gc-sections -Xlinker -print-memory-usage -Xlinker
)

# POSE_BUILD 옵션은 빌드가 완료된후 실행된다. 따라서 빌드가 실패하면 실행되지 않는다.
add_custom_command (TARGET ${EXECUTABLE} POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} ARGS -O ihex -R .eeprom -R .fuse -R .lock -R .signature ${EXECUTABLE} ${PROJECT_NAME}.hex
  COMMENT "Invoking: Make Hex"
)  

# add_custom_command (TARGET ${EXECUTABLE} POST_BUILD
#   COMMAND ${CMAKE_OBJCOPY} ARGS -O binary -R .eeprom ${EXECUTABLE} ${PROJECT_NAME}.bin
#   COMMENT "Invoking: Make Bin"
# )  


add_custom_command (TARGET ${EXECUTABLE} POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} ARGS -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load 
  --no-change-warnings --change-section-lma .eeprom=0 ${EXECUTABLE} ${PROJECT_NAME}.eep
  COMMENT "Invoking: Make EEPROM"
)   

add_custom_command (TARGET ${EXECUTABLE} POST_BUILD
  COMMAND ${CMAKE_OBJDUMP} ARGS -h -S ${EXECUTABLE} > ${PROJECT_NAME}.lst
  COMMENT "Invoking: Make List file"
)   

