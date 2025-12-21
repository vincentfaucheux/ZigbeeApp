add_library(ZigbeeSoSub SHARED
    ZigbeeSoSub.cpp
)

target_include_directories(ZigbeeSoSub
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..
)

set_target_properties(ZigbeeSoSub PROPERTIES
    OUTPUT_NAME "ZigbeeSo"
)
