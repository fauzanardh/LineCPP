cmake_minimum_required(VERSION 3.4)
project(LineCPP VERSION "0.0.1")
add_definitions("-std=c++14")
# add_compile_definitions("CMAKE_CXX_COMPILER=/usr/bin/clang++")
# add_compile_definitions("_GLIBCXX_USE_CXX11_ABI=0")

include(CTest)
enable_testing()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

# Conan stuff
include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
conan_basic_setup()

# Thrift stuff
## SecondaryQrCode
set(SecondaryQrCode_SOURCES
    src/generated/SecondaryQrCodeService/exceptions_types.cpp
    src/generated/SecondaryQrCodeService/SecondaryQrCodeLoginService.cpp
    src/generated/SecondaryQrCodeService/SecondaryQrCodePermitService.cpp
    src/generated/SecondaryQrCodeService/SecondaryQrCodeLoginPermitNoticeService.cpp
    src/generated/SecondaryQrCodeService/structs_types.cpp
)
add_library(secondaryqrcode_gen STATIC ${SecondaryQrCode_SOURCES})
target_link_libraries(secondaryqrcode_gen ${CONAN_LIBS})

add_executable(LineCPP
    src/main.cpp 
    src/line/transport/custom_http_client.cpp
)
target_link_libraries(LineCPP ${CONAN_LIBS} secondaryqrcode_gen)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
