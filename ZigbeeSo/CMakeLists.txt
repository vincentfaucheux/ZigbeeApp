# mosqitto
find_library(MOSQUITTO_LIB mosquitto REQUIRED)
find_package(yaml-cpp REQUIRED)

# Les headers des plugins sont accessibles à tous
add_library(zigbee_interface INTERFACE)

target_include_directories(zigbee_interface
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

# Options communes aux plugins
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(ZigbeeSo SHARED
    ZigbeeSo_interface.cpp
    ZigbeeSo_interface.h
    ZigbeeSo.cpp
    ZigbeeSo.h
)

target_link_libraries(ZigbeeSo PRIVATE
    ${MOSQUITTO_LIB}
)

target_link_libraries(ZigbeeSo
    PRIVATE ${yaml-cpp}
)

target_include_directories(ZigbeeSo
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..
)

set_target_properties(ZigbeeSo PROPERTIES
    OUTPUT_NAME "ZigbeeSo"
)
