find_package(Qt5 REQUIRED COMPONENTS Widgets)

add_executable(ZigbeeLaunch
    main.cpp
    mainwindow.cpp
    mainwindow.h
)

target_include_directories(ZigbeeLaunch
    PRIVATE ${CMAKE_SOURCE_DIR}/ZigbeeSo
)

target_link_libraries(ZigbeeLaunch
    Qt5::Widgets
    dl
)

set_target_properties(ZigbeeLaunch PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
