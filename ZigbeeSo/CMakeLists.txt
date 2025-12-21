# Les headers des plugins sont accessibles à tous
add_library(zigbee_interface INTERFACE)

target_include_directories(zigbee_interface
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

# Options communes aux plugins
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Ajouter les plugins ici
add_subdirectory(ZigbeeSoSub)
