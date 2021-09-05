cmake_minimum_required(VERSION 3.21)

project(kinema)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/binaries)

file(GLOB_RECURSE KINEMA_FILES source/*.cpp source/*.h source/*.c)

source_group(TREE ${CMAKE_SOURCE_DIR}/source FILES ${KINEMA_FILES})

add_executable(${PROJECT_NAME} ${KINEMA_FILES})

target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/thirdparty/binaries)
target_link_libraries(${PROJECT_NAME} SDL2 SDL2main PhysX_64 PhysXCommon_64 PhysXFoundation_64 PhysXCooking_64 PhysXExtensions_static_64)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/thirdparty)

target_compile_definitions(${PROJECT_NAME} PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/data
    VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${CMAKE_SOURCE_DIR}/thirdparty/binaries"
    )

set_directory_properties(PROPERTIES
    VS_STARTUP_PROJECT ${PROJECT_NAME}
    )