cmake_minimum_required(VERSION 3.31.6)
project(ZBinary2CArray)

set(CMAKE_CXX_STANDARD 20)

add_executable(zbtca-cli main.cpp)

if (MSVC)
    set_property(TARGET zbtca-cli PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    )
    
elseif (MINGW)
    target_link_options(zbtca-cli PRIVATE
        -static-libgcc
        -static-libstdc++
    )
endif()