#pragma once

// Configurable world variables
namespace World
{
#ifndef NDEBUG
	constexpr float renderDistance = 64.0f; // block render radius
	constexpr unsigned renderSpeed = 1; // chunks generated per frame
#else
	const float renderDistance = 400.0f; // block render radius
	const unsigned renderSpeed = 2; // chunks generated per frame
#endif

	// Entity gravity force
	constexpr float gravity = 20.0f;

	// Chunk dimensions
	constexpr unsigned chunkHeight = 256;
	constexpr unsigned chunkSize = 16;
	constexpr unsigned chunkArea = chunkSize * chunkSize;
	constexpr unsigned chunkVolume = chunkArea * chunkHeight;

	// Chunk load in animation
	constexpr float chunkFloatDistance = chunkHeight / 2.f;
	constexpr float chunkFloatInSpeed = 1.0f;
	constexpr float chunkFloatOutSpeed = 0.25f;

	// Configurable world generation variables
	namespace Generation
	{
		// Lowest block elevation possible
		constexpr unsigned minHeight = 1;

		// Biome noise
		constexpr float landScale = 2048.0f;
		constexpr float landMinMult = 0.1f;
		constexpr float landTransitionSharpness = 2.0f;
		constexpr float landMountainBias = 0.2f; // -1 (flat) to 1 (mountains)

		// Mountain noise
		constexpr float heightScale = 256.0f;
		constexpr float heightWeight = 0.8f;
		constexpr unsigned heightMaxHeight = 250;

		// Hill noise
		constexpr float detailScale = 32.0f;
		constexpr float detailWeight = 1.0f - heightWeight;
		constexpr unsigned detailMaxHeight = 100;

		// Trees
		constexpr float treeDensity = 0.03f;

		// Interpolation grid size
		constexpr float terrainInterpGrid = 4.0f;
	}
}