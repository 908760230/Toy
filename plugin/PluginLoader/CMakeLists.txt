set(TARGET_NAME "PluginLoader")
file(GLOB PLUGIN_LOADER_H *.h)
file(GLOB PLUGIN_LOADER_CPP *.cpp)

add_library(${TARGET_NAME}  ${PLUGIN_LOADER_H}  ${PLUGIN_LOADER_CPP})

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${TARGET_NAME} Core LogPlugin SystemWinPlugin)

set_target_properties( ${TARGET_NAME} PROPERTIES OUTPUT_NAME_DEBUG "${TARGET_NAME}")
set_target_properties( ${TARGET_NAME} PROPERTIES PREFIX "")
set_target_properties( ${TARGET_NAME} PROPERTIES FOLDER "Plugins")

set_target_properties( ${TARGET_NAME} PROPERTIES  
        ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR})