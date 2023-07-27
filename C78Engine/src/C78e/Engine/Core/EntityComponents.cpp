#include "EntityComponents.h"

#include "UUID.h"

#include "Engine/Util/types.h"
#include "Engine/Util/MeshLoader.h"

#include <unordered_map>
#include <stdexcept>
#include <memory>
#include <iostream>
#include <filesystem>

namespace C78e {

    IDComponent::IDComponent() {}

    IDComponent::IDComponent(UUID uuid) : id(uuid) { }

    TagComponent::TagComponent() { tag = "<unknown>"; }

    TagComponent::TagComponent(const std::string atag) : tag{ atag } { if (tag == "") tag = "<unknown>"; }

    glm::mat4 TransformComponent::mat4() {
        const float c3 = glm::cos(rotation.z);
        const float s3 = glm::sin(rotation.z);
        const float c2 = glm::cos(rotation.x);
        const float s2 = glm::sin(rotation.x);
        const float c1 = glm::cos(rotation.y);
        const float s1 = glm::sin(rotation.y);
        return glm::mat4{
            {
                scale.x* (c1* c3 + s1 * s2 * s3),
                scale.x* (c2* s3),
                scale.x* (c1* s2* s3 - c3 * s1),
                0.0f,
            },
            {
                scale.y * (c3 * s1 * s2 - c1 * s3),
                scale.y * (c2 * c3),
                scale.y * (c1 * c3 * s2 + s1 * s3),
                0.0f,
            },
            {
                scale.z * (c2 * s1),
                scale.z * (-s2),
                scale.z * (c1 * c2),
                0.0f,
            },
            { translation.x, translation.y, translation.z, 1.0f } };
    }


    glm::mat3 TransformComponent::normalMat() {

        const float c3 = glm::cos(rotation.z);
        const float s3 = glm::sin(rotation.z);
        const float c2 = glm::cos(rotation.x);
        const float s2 = glm::sin(rotation.x);
        const float c1 = glm::cos(rotation.y);
        const float s1 = glm::sin(rotation.y);

        const glm::vec3 invScale = 1.f / scale;

        return glm::mat3{
            {
                invScale.x* (c1* c3 + s1 * s2 * s3),
                    invScale.x* (c2* s3),
                    invScale.x* (c1* s2* s3 - c3 * s1)
            },
            {
                invScale.y * (c3 * s1 * s2 - c1 * s3),
                invScale.y * (c2 * c3),
                invScale.y * (c1 * c3 * s2 + s1 * s3)
            },
            {
                invScale.z * (c2 * s1),
                invScale.z * (-s2),
                invScale.z * (c1 * c2)
            }
        };
    }

    MeshComponent::MeshComponent(std::string file) {
        Mesh mesh = MeshLoader::fromFile(file);
        vertecies = mesh.vertecies;
        indecies = mesh.indecies;
    }

    MeshComponent::MeshComponent(std::vector<Mesh::Vertex> vertecies, std::vector<uint32_t> indecies) {
        this->vertecies = vertecies;
        this->indecies = indecies;
    }

    MaterialComponent::MaterialComponent(const RawImage& image) : file{ image.file }, width{ image.width }, height{ image.height }, channels{ image.channels } {
        data.resize(width * height * channels);
        memcpy(data.data(), image.data, width * height * channels * sizeof(char));
    }

    PointLightComponent::PointLightComponent(glm::vec3 color, float intensity, float radius)
        : color(color), intensity(intensity), radius(radius) {
    }
}