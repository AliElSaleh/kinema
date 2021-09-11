cmake_minimum_required(VERSION 3.21)

project(kinema)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/binaries)

file(GLOB_RECURSE KINEMA_FILES source/*.cpp source/*.h source/*.c)

source_group(TREE ${CMAKE_SOURCE_DIR}/source FILES ${KINEMA_FILES})

add_executable(${PROJECT_NAME} ${KINEMA_FILES})

#target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/thirdparty/binaries)
#target_link_libraries(${PROJECT_NAME} SDL2 SDL2main
#    debug debug/PhysX_64 debug/PhysXCommon_64 debug/PhysXFoundation_64 debug/PhysXCooking_64 debug/PhysXExtensions_static_64
#    optimized PhysX_64 PhysXCommon_64 PhysXFoundation_64 PhysXCooking_64 PhysXExtensions_static_64)

find_package(SDL2 REQUIRED)
find_package(fmt REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

# PhysX Libraries
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/thirdparty/vcpkg/packages/physx_x64-windows)
target_link_libraries(${PROJECT_NAME} PRIVATE
    debug debug/lib/PhysX_64
    debug debug/lib/PhysXCommon_64
    debug debug/lib/PhysXFoundation_64
    debug debug/lib/PhysXCooking_64
    debug debug/lib/PhysXExtensions_static_64
    optimized lib/PhysX_64
    optimized lib/PhysXCommon_64
    optimized lib/PhysXFoundation_64
    optimized lib/PhysXCooking_64
    optimized lib/PhysXExtensions_static_64)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/thirdparty/vcpkg/packages/physx_x64-windows/include)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/thirdparty)

target_compile_definitions(${PROJECT_NAME} PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/data
    VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${CMAKE_SOURCE_DIR}/thirdparty/binaries"
    )

set_directory_properties(PROPERTIES
    VS_STARTUP_PROJECT ${PROJECT_NAME}
    )