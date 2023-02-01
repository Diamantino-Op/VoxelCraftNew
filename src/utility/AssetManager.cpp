#include "AssetManager.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <fstream>
#include <iostream>
#include <ranges>
#include <stb_image.h>
#include <stb_image_write.h>
#include <utility>

AssetManager::AssetManager()
{
    assetPath = "resources/";
}

void AssetManager::SetPath(std::string path)
{
    assetPath = std::move(path);
    assetPath.append("/");
}

std::string AssetManager::GetPath()
{
    return assetPath;
}

// Load a texture file into memory
std::vector<unsigned char> AssetManager::LoadTexture(const std::string& filename, int& width, int& height, int& channels)
{
    // Load the image using stb_image
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "Failed to load image " << filename << ": " << stbi_failure_reason() << std::endl;
        return {};
    }

    // Convert the loaded image to a vector
    std::vector imageData(data, data + width * height * channels);

    // Free the loaded image
    stbi_image_free(data);

    return imageData;
}

// Save an atlas image and a text file with the mapping from texture names to filenames
void AssetManager::PackTextures(const std::map<std::string, std::string>& textures, const std::string& atlasFilename, const std::string& mappingFilename)
{
    // Determine the size of the atlas
    int atlasWidth = 0;
    int atlasHeight = 0;
    for (const auto& filename : textures | std::views::values) {
        int width, height, channels;
        auto data = LoadTexture(filename, width, height, channels);
        if (data.empty()) {
            return;
        }

        atlasWidth = std::max(atlasWidth, width);
        atlasHeight += height;
    }

    // Allocate memory for the atlas image
    std::vector<unsigned char> atlasImage(atlasWidth * atlasHeight * 4);

    // Write the mapping from texture names to filenames to a text file
    std::ofstream mappingFile(mappingFilename);
    if (!mappingFile) {
        std::cerr << "Failed to open mapping file " << mappingFilename << " for writing" << std::endl;
        return;
    }

    // Copy the textures into the atlas
    int y = 0;
    int i = 0;
    for (const auto &[name, filename] : textures) {
        int width, height, channels;
        auto data = LoadTexture(filename, width, height, channels);
        if (data.empty()) {
            return;
        }

        // Copy the texture data into the atlas image
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                atlasImage[((y + i) * atlasWidth + j) * 4 + 0] = data[(i * width + j) * channels + 0];
                atlasImage[((y + i) * atlasWidth + j) * 4 + 1] = data[(i * width + j) * channels + 1];
                atlasImage[((y + i) * atlasWidth + j) * 4 + 2] = data[(i * width + j) * channels + 2];
                atlasImage[((y + i) * atlasWidth + j) * 4 + 3] = channels == 3 ? 255 : data[(i * width + j) * channels + 3];
            }
        }

        mappingFile << name << ":" << i << std::endl;

        i++;
        y += height;
    }

    // Save the atlas image to a file using stb_image_write
	if (!stbi_write_png(atlasFilename.c_str(), atlasWidth, atlasHeight, 4, atlasImage.data(), 0)) {
		std::cout << "Failed to write atlas image to " << atlasFilename << ": " << stbi_failure_reason() << std::endl;
	}
}

/*
    // Get all .png files in a folder
    std::map<std::string, std::string> textures;
    std::filesystem::path folder("C:/images");
    for (const auto &entry : std::filesystem::directory_iterator(folder)) {
        if (entry.path().extension() == ".png") {
            auto name = entry.path().stem().string();
            auto filename = entry.path().string();
            textures[name] = filename;
        }
    }
    
    packTextures(textures, "atlas.png", "mapping.txt");
*/
