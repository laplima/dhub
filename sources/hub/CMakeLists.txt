# SERVER
cmake_minimum_required(VERSION 3.20)

project(hub)   # name of the executable

set(sources
	${SKELETONS}
	${IDL_IMPL}
	hub.cpp)

add_executable(${PROJECT_NAME})
add_dependencies(${PROJECT_NAME} ${CMAKE_PROJECT_NAME}_stubs)
target_sources(${PROJECT_NAME} PRIVATE ${sources})
target_compile_options(${PROJECT_NAME} PUBLIC)
target_link_libraries(${PROJECT_NAME}
	${mylibs} spdlog::spdlog_header_only colibry::colibry)
