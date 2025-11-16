cmake_minimum_required(VERSION 3.16)
project(Graphic_Engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES
    Shapes/*.cpp
    GraphicManager/*.cpp
    Windows/*.cpp
    Events/*.cpp
)


add_library(Graphic_Engine STATIC ${SOURCES})


# Link depending on config
target_link_libraries(Graphic_Engine PRIVATE
    $<$<CONFIG:Debug>:${ENGINE_LIBS_DEBUG}>
    $<$<CONFIG:Release>:${ENGINE_LIBS_RELEASE}>
)

target_include_directories(Graphic_Engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

if(MSVC)
    target_compile_options(Graphic_Engine PRIVATE /Zc:preprocessor)
endif()

find_package(glm CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC glm::glm)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

find_package(glad CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC glad::glad)

find_package(assimp CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC assimp::assimp)


