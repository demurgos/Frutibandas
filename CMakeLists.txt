cmake_minimum_required(VERSION 3.16)

project(frutibandas)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules ${CMAKE_MODULE_PATH})
set(CMAKE_RUNTIME_OUTPUT ${CMAKE_BINARY_DIR})
set(APP_ICON_RESOURCE_WINDOWS "src/resources.rc")
cmake_policy(SET CMP0072 NEW)

include_directories(include
	${SDL2_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR}
	${ASSIMP_INCLUDE_DIR}
	${BULLET_INCLUDE_DIR}
	${OPENAL_INCLUDE_DIR}
	${LIBSNDFILE_INCLUDE_DIR}
    ${FREETYPE_INCLUDE_DIR}
    ${ENET_INCLUDE_DIR}
	)

link_directories(${SDL2_LIB_DIR}
	${GLEW_LIB_DIR}
	${ASSIMP_LIB_DIR}
	${BULLET_LIB_DIR}
	${OPENAL_LIB_DIR}
	${LIBSNDFILE_LIB_DIR}
    ${FREETYPE_LIB_DIR}
    ${ENET_LIB_DIR}
	)

set(SRCS
	src/stb_image.cpp
	src/main.cpp
	src/window.cpp
	src/shader_light.cpp
	src/camera.cpp
	src/mesh.cpp
	src/object.cpp
	src/grid_axis.cpp
	src/skybox.cpp
	src/scene.cpp
	src/framebuffer.cpp
	src/game.cpp
	src/graphics.cpp
	src/animatedObject.cpp
	src/joint.cpp
	src/worldPhysics.cpp
	src/character.cpp
	src/IBL.cpp
	src/particle.cpp
	src/audio.cpp
	src/vehicle.cpp
    src/renderTexture.cpp
    src/user_interface.cpp
	src/network_client.cpp
	src/helpers.cpp
	src/mouse.cpp
	src/imgui.cpp
	src/imgui_draw.cpp
	src/imgui_tables.cpp
	src/imgui_widgets.cpp
	src/imgui_impl_sdl.cpp
	src/imgui_impl_opengl3.cpp
	src/tinyfiledialogs.cpp)

set(HEADERS
	include/stb_image.h
	include/color.hpp
	include/window.hpp
	include/shader_light.hpp
	include/camera.hpp
	include/mesh.hpp
	include/object.hpp
	include/grid_axis.hpp
	include/skybox.hpp
	include/scene.hpp
	include/framebuffer.hpp
	include/game.hpp
	include/graphics.hpp
	include/animatedObject.hpp
	include/joint.hpp
	include/worldPhysics.hpp
	include/character.hpp
	include/IBL.hpp
	include/particle.hpp
	include/audio.hpp
	include/vehicle.hpp
    include/renderTexture.hpp
    include/user_interface.hpp
    include/editorUI.hpp
	include/rapidxml.hpp
	include/lightning.hpp
	include/helpers.hpp
	include/allocation.hpp
	include/network_client.hpp
	include/mouse.hpp
	include/imgui.h
	include/imconfig.h
	include/imgui_internal.h
	include/imstb_textedit.h
	include/imstb_rectpack.h
	include/imstb_truetype.h
	include/imgui_impl_sdl.h
	include/imgui_impl_opengl3.h
	include/tinyfiledialogs.hpp)

add_executable(${PROJECT_NAME} ${SRCS} ${HEADERS} ${APP_ICON_RESOURCE_WINDOWS})

find_package(OPENMP REQUIRED)
if(OPENMP_FOUND)
	target_link_libraries(${PROJECT_NAME} ${OpenMP_LD_FLAGS})
else()
	message(FATAL_ERROR "OpenMP not found.")
endif()

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
	target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})
	target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARY})
else()
	message(FATAL_ERROR "OpenGL not found.")
endif()

target_link_libraries(${PROJECT_NAME}
	${SDL2_LIBS}
	${GLEW_LIBS}
	${ASSIMP_LIBS}
	${BULLET_LIBS}
	${OPENAL_LIBS}
	${LIBSNDFILE_LIBS}
    ${FREETYPE_LIBS}
    ${ENET_LIBS}
	)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
