cmake_minimum_required(VERSION 3.16)

project(MX210Test
    VERSION 1.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MX210API_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

if(WIN32)

    add_executable(MX210Test_SERIAL src/testSerial_Windows.c)
    add_executable(MX210Test_ETHERNET src/testEthernet_Windows.c)

elseif(UNIX) 

    add_executable(MX210Test_SERIAL src/testSerial_Linux.c)
    add_executable(MX210Test_ETHERNET src/testEthernet_Linux.c)

endif()

target_include_directories(MX210Test_SERIAL
    PRIVATE
        "${MX210API_DIR}/include"
)

target_include_directories(MX210Test_ETHERNET
    PRIVATE
        "${MX210API_DIR}/include"
)

target_link_libraries(MX210Test_SERIAL
    PRIVATE
        "${MX210API_DIR}/lib/libMX210.so"
)

target_link_libraries(MX210Test_ETHERNET
    PRIVATE
        "${MX210API_DIR}/lib/libMX210.so"
)

foreach(TARGET_NAME MX210Test_SERIAL MX210Test_ETHERNET)
    add_custom_command(
        TARGET ${TARGET_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${MX210API_DIR}/lib/libMX210.so"
            "$<TARGET_FILE_DIR:${TARGET_NAME}>"
    )
endforeach()