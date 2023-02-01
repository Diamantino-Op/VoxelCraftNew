#include "input/Camera.h"
#include "graphics/Skybox.h"
#include "graphics/gui/Crosshair.h"

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "world/chunk/ChunkManager.h"
#include "entity/Player.h"
#include "input/InputManager.h"
#include "graphics/WindowManager.h"
#include "graphics/utility/CascadedShadowMap.h"
#include "network/NetworkManager.h"
#include "utility/AssetManager.h"

int main(int argc, char *argv[])
{
	char* serverIp;
	int screenX = 1920;
	int screenY = 1080;
	std::string assetPath;
	
	if (argc > 1)
	{
		for (int i = 0; i < argc; ++i)
		{
			if (argv[i] == "-server")
				serverIp = argv[i + 1];

			if (argv[i] == "-screenX")
				screenX = std::stoi(argv[i + 1]);

			if (argv[i] == "-screenY")
				screenY = std::stoi(argv[i + 1]);

			if (argv[i] == "-assetsPath")
				assetPath = argv[i + 1];
		}
	}

	// Create game systems
	WindowManager &windowManager = WindowManager::Instance();
    windowManager.Init(screenX, screenY);
	windowManager.Maximize();
	ChunkManager &chunkManager = ChunkManager::Instance();
	InputManager &inputManager = InputManager::Instance();
	NetworkManager &networkManager = NetworkManager::Instance();
	AssetManager &assetManager = AssetManager::Instance();

	networkManager.Start(serverIp);
	assetManager.SetPath(assetPath);
	
	Player player;
	CascadedShadowMap shadows({ { 2048, 0.025f }, { 2048, 0.125f }, { 2048, 1.0f } }); // Cascade resolution, z-depths
	Skybox skybox;
	Crosshair crosshair;

	// Render loop
	while (!glfwWindowShouldClose(windowManager.GetWindow()))
	{
		// Delta time
		static float lastFrame = 0.0f;
		const auto currentFrame = static_cast<float>(glfwGetTime());
		static float deltaTime;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Update
		windowManager.Update(deltaTime);
		player.Update(deltaTime);
		networkManager.Update(player);
		chunkManager.UpdateChunks(player.GetCamera().GetPosition(), deltaTime);
		inputManager.Update();

		// Draw
		const Camera &cam = player.GetCamera();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// Clear
		shadows.Render(cam.GetMatrix());								// Render all the chunks to cascaded shadow maps
		skybox.Render(cam.GetViewMatrix(), cam.GetProjectionMatrix());	// Render the skybox
		chunkManager.DrawChunksLit(cam, shadows.GetShaderInfo());		// Render all the chunks to the screen
		networkManager.Render(chunkManager.GetShader());				// Render other players
		crosshair.Render(glm::vec2(1.f, cam.GetAspect()) / 400.f);		// Render the crosshair
		glfwSwapBuffers(windowManager.GetWindow());						// Present frame to screen
	}

	return 0;
}

// Release has no console which makes it use WinMain
#ifdef NDEBUG
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	return main(__argc, __argv);
}
#endif