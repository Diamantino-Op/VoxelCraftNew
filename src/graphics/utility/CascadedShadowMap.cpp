#include "CascadedShadowMap.h"
#include "../../world/chunk/ChunkManager.h"
#include "../WindowManager.h"
#include "../resources/shaders/Shared.h"
#include "../../utility/AssetManager.h"

#include <queue>
#include <array>

CascadedShadowMap::CascadedShadowMap(const std::vector<Cascade> &cascades) :
    cascades_(cascades),
    shader_((AssetManager::Instance().GetPath() + "shaders/shadow.vert").data(), (AssetManager::Instance().GetPath() + "shaders/shadow.frag").data())
{
    assert(cascades.size() <= MAX_CASCADES);

    for (const Cascade &c : cascades_)
        framebuffers_.emplace_back(c.resolution, c.resolution);
}

void CascadedShadowMap::Render(const glm::mat4 &cameraMatrix)
{
    shaderInfo_.clear();

    // Transforms from NDC space to world space
    const glm::mat4 inverseCameraMatrix = inverse(cameraMatrix);

    // Transforms from world space to light view space
    const glm::mat4 lightTransform = lookAt(glm::vec3(0.0f), -glm::vec3(LIGHT_DIR), glm::vec3(0.0f, 1.0f, 0.0f));

    constexpr std::array<glm::vec2, 4> corners = {
        glm::vec2(-1.0f, -1.0f),
        glm::vec2(-1.0f,  1.0f),
        glm::vec2( 1.0f, -1.0f),
        glm::vec2( 1.0f,  1.0f)
    };

    // Corners of camera frustum in light space
    std::array<glm::vec3, 4> nearCorners{};
    std::array<glm::vec3, 4> farCorners{};
    for (size_t i = 0; i < corners.size(); i++)
    {
        glm::vec4 world = inverseCameraMatrix * glm::vec4(corners[i], -1.0f, 1.0f);
        nearCorners[i] = lightTransform * (world / world.w);
        world = inverseCameraMatrix * glm::vec4(corners[i], 1.0f, 1.0f);
        farCorners[i] = lightTransform * (world / world.w);
    }

    // Circular buffer that stores all for current subFrastum
    std::array<glm::vec3, 8> subFrastumCorners{};
    std::copy(nearCorners.begin(), nearCorners.end(), subFrastumCorners.begin());
    size_t index = nearCorners.size();

    // Calculate the transformations for each cascade and render
    for (size_t i = 0; i < cascades_.size(); i++)
    {
        // Add the next plane's corners, overwriting previous's
        for (size_t j = 0; j < corners.size(); j++)
        {
            subFrastumCorners[index++] = glm::mix(nearCorners[j], farCorners[j], cascades_[i].depth);
        }
        index %= subFrastumCorners.size(); // Circular buffer

        // Calculate min and max of all 8 subFrastum points
        auto min = glm::vec3(std::numeric_limits<float>::infinity());
        auto max = glm::vec3(-std::numeric_limits<float>::infinity());
        for (const glm::vec3 &v : subFrastumCorners)
        {
            min = glm::min(min, v);
            max = glm::max(max, v);
        }

        // Create transformation for cascade (offset zNear so it includes objects outside view frustum)
        shaderInfo_.push_back({glm::ortho(min.x, max.x, min.y, max.y, -max.z - 200.0f, -min.z) * lightTransform, cascades_[i].depth, &framebuffers_[i].GetTexture()});
        
        // Render chunks using transformation
        framebuffers_[i].BindFramebuffer();
        ChunkManager::Instance().DrawChunks(shaderInfo_.back().transform, shader_);
    }
    // Set default framebuffer
    WindowManager::Instance().SetFramebuffer();
}

const std::vector<CascadeShaderInfo> &CascadedShadowMap::GetShaderInfo()
{
    return shaderInfo_;
}

