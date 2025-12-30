find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(yaml-cpp REQUIRED)

add_executable(ZigbeeLaunch
    main.cpp
    mainwindow.cpp
    mainwindow.h
)

target_include_directories(ZigbeeLaunch
    PRIVATE ${CMAKE_SOURCE_DIR}/ZigbeeSo
)

target_link_libraries(ZigbeeLaunch
    Qt6::Widgets
    dl
)

target_link_libraries(ZigbeeLaunch
    yaml-cpp
)

set_target_properties(ZigbeeLaunch PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
