cmake_minimum_required(VERSION 3.11)
project(GraphicsTest)

file(GLOB SOURCES "*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE Graphic_Engine)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /Zc:preprocessor)
endif()
	