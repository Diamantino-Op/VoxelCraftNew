cmake_minimum_required(VERSION 3.24)
project(VoxelCraft)

set(CMAKE_CXX_STANDARD 23)

link_directories(lib)

include(include/glm/CMakeLists.txt)

include_directories(include)

add_executable(${PROJECT_NAME} lib/glad.c src/Main.cpp src/world/TerrainGenerator.h src/world/WorldConstants.h src/world/TerrainGenerator.cpp src/world/chunk/Chunk.cpp src/world/chunk/Chunk.h src/world/chunk/ChunkManager.cpp src/world/chunk/ChunkManager.h src/utility/AssetManager.cpp src/utility/AssetManager.h src/utility/Math.cpp src/utility/Math.h src/network/NetworkManager.cpp src/network/NetworkManager.h src/network/RemotePlayers.cpp src/network/RemotePlayers.h src/network/Socket.cpp src/network/Socket.h src/input/Camera.cpp src/input/Camera.h src/input/InputManager.cpp src/input/InputManager.h src/graphics/Skybox.cpp src/graphics/Skybox.h src/graphics/WindowManager.cpp src/graphics/WindowManager.h src/graphics/utility/CascadedShadowMap.cpp src/graphics/utility/CascadedShadowMap.h src/graphics/utility/Framebuffer.cpp src/graphics/utility/Framebuffer.h src/graphics/utility/Mesh.cpp src/graphics/utility/Mesh.h src/graphics/utility/Shader.cpp src/graphics/utility/Shader.h src/graphics/utility/Texture.cpp src/graphics/utility/Texture.h src/graphics/gui/Crosshair.cpp src/graphics/gui/Crosshair.h src/entity/Entity.cpp src/entity/Entity.h src/entity/Player.cpp src/entity/Player.h src/block/Block.h resources/shaders/Shared.h)

target_link_libraries(${PROJECT_NAME} glfw3)
target_link_libraries(${PROJECT_NAME} Ws2_32)

target_compile_options(${PROJECT_NAME} PRIVATE -Wno-volatile)