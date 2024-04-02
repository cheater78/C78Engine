#include <C78ePCH.h>
#include "TinyObjectLoader.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <C78E/Assets/Mesh/Mesh.h>

namespace C78E {

    std::vector<Ref<Model>> TinyObjectLoader::loadAllModels(std::string file, std::string name) {
        std::vector<Ref<Model>> models;


        std::vector<std::string> path = std::split(file, '/');
        tinyobj::ObjReaderConfig reader_config;
        //reader_config.mtl_search_path = ""; // Path to material files

        tinyobj::ObjReader reader;

        if (!reader.ParseFromFile(file, reader_config))
            C78_CORE_ASSERT(false, "Model: create -> failed to read ModelFile, TOL: " + reader.Error());

        if (!reader.Warning().empty())
            C78_CORE_WARN("Model: create -> Warn TOL: " + reader.Warning());

        auto& attrib = reader.GetAttrib();
        auto& shapes = reader.GetShapes();
        auto& materials = reader.GetMaterials();

        // Loop over shapes
        for (size_t s = 0; s < shapes.size(); s++) {
            std::string shapeName = shapes[s].name;

            if (name != "" && name != shapeName) continue;

            std::unordered_map<Vertex, uint32_t> uniqueVertecies{};
            std::vector<Vertex> vertecies{};
            std::vector<uint32_t> indecies{};

            // Loop over faces(polygon)
            uint32_t materialID = -1;
            std::string materialName = "";
            size_t index_offset = 0;
            for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
                size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

                // per-face material
                int material_id = shapes[s].mesh.material_ids[f];
                materialName = materials[material_id].name;

                if (material_id != materialID) {
                    // Material Changed -> Save Current Model(if not first run),
                    // and add new Material to Lib
                    if (materialID != -1) {
                        // Material Changed and not first run -> Save Current Model and start a new one
                        Ref<Mesh> mesh = createRef<Mesh>(vertecies, indecies);
                        AssetManager::addMesh(shapeName + "_" + materialName, mesh, file);
                        models.push_back(createRef<Model>(AssetManager::getMeshAsset(shapeName), AssetManager::getMaterialAsset(materialName)));

                        uniqueVertecies.clear();
                        vertecies.clear();
                        indecies.clear();
                    }
                    materialID = material_id;
                    AssetManager::addMaterial(materialName, toMaterial(materials[material_id]), file);
                }
                

                // Loop over vertices in the face.
                for (size_t v = 0; v < fv; v++) {
                    tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                    Vertex vertex;
                    uint32_t index = shapes[s].mesh.indices[index_offset + v].vertex_index;

                    vertex.Position = {
                        attrib.vertices[3 * size_t(index) + 0],
                        attrib.vertices[3 * size_t(index) + 1],
                        attrib.vertices[3 * size_t(index) + 2]
                    };

                    vertex.Color = glm::vec4{
                        attrib.colors[3 * size_t(idx.vertex_index) + 0],
                        attrib.colors[3 * size_t(idx.vertex_index) + 1],
                        attrib.colors[3 * size_t(idx.vertex_index) + 2],
                        1
                    };

                    // Check if `normal_index` is zero or positive. negative = no normal data
                    vertex.Normal = (idx.normal_index >= 0) ? glm::vec3{
                        attrib.normals[3 * size_t(idx.normal_index) + 0],
                        attrib.normals[3 * size_t(idx.normal_index) + 1],
                        attrib.normals[3 * size_t(idx.normal_index) + 2]
                    } : glm::vec3{ 0, 0, 0 };

                    // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                    vertex.TexCoord = (idx.texcoord_index >= 0) ? glm::vec2{
                        attrib.texcoords[2 * size_t(idx.texcoord_index) + 0],
                        attrib.texcoords[2 * size_t(idx.texcoord_index) + 1]
                    } : glm::vec2{ 0, 0 };

                    vertex.TexIndex = 0.f;

                    if (uniqueVertecies.count(vertex) <= 0) {
                        uniqueVertecies[vertex] = static_cast<uint32_t>(vertecies.size());
                        vertecies.push_back(vertex);
                    }
                    indecies.push_back(uniqueVertecies[vertex]);
                }
                index_offset += fv;
            }

            Ref<Mesh> mesh = createRef<Mesh>(vertecies, indecies);
            AssetManager::addMesh(shapeName + "_" + materialName, mesh, file);
            models.push_back(createRef<Model>(AssetManager::getMeshAsset(shapeName + "_" + materialName), AssetManager::getMaterialAsset(materialName)));
        }
        return models;
    }


    Ref<Material> TinyObjectLoader::toMaterial(const tinyobj::material_t& material) {
        return createRef<Material>(
            toMaterialProperties(material),
            material.illum,
            toMaterialTextures(material),
            toMaterialPropertiesPBRext(material),
            toMaterialTexturesPBRext(material)
        );
    }

    Material::MaterialProperties TinyObjectLoader::toMaterialProperties(const tinyobj::material_t& material) {
        return {
            glm::vec3{ material.ambient[0], material.ambient[1], material.ambient[2] },
            glm::vec3{ material.diffuse[0], material.diffuse[1], material.diffuse[2] },
            glm::vec3{ material.specular[0], material.specular[1], material.specular[2] },
            glm::vec3{ material.transmittance[0], material.transmittance[1], material.transmittance[2] },
            glm::vec3{ material.emission[0], material.emission[1], material.emission[2] },
            material.shininess,
            material.ior,
            material.dissolve
        };
    }

    Material::MaterialTextures TinyObjectLoader::toMaterialTextures(const tinyobj::material_t& material) {
        return {
            material.ambient_texname,
            material.diffuse_texname,
            material.specular_texname,
            material.specular_highlight_texname,
            material.bump_texname,
            material.displacement_texname,
            material.alpha_texname,
            material.reflection_texname,
            toMaterialTextureOption(material.ambient_texopt),
            toMaterialTextureOption(material.diffuse_texopt),
            toMaterialTextureOption(material.specular_texopt),
            toMaterialTextureOption(material.specular_highlight_texopt),
            toMaterialTextureOption(material.bump_texopt),
            toMaterialTextureOption(material.displacement_texopt),
            toMaterialTextureOption(material.alpha_texopt),
            toMaterialTextureOption(material.reflection_texopt)
        };
    }

    Material::MaterialPropertiesPBRext TinyObjectLoader::toMaterialPropertiesPBRext(const tinyobj::material_t& material)
    {
        return {
            material.roughness,
            material.metallic,
            material.sheen,
            material.clearcoat_thickness,
            material.clearcoat_roughness,
            material.anisotropy,
            material.anisotropy_rotation,
        };
    }

    Material::MaterialTexturesPBRext TinyObjectLoader::toMaterialTexturesPBRext(const tinyobj::material_t& material)
    {
        return {
            material.ambient_texname,
            material.metallic_texname,
            material.sheen_texname,
            material.emissive_texname,
            material.normal_texname,
            toMaterialTextureOption(material.alpha_texopt),
            toMaterialTextureOption(material.metallic_texopt),
            toMaterialTextureOption(material.sheen_texopt),
            toMaterialTextureOption(material.emissive_texopt),
            toMaterialTextureOption(material.normal_texopt)
        };
    }

    Texture::TextureOption TinyObjectLoader::toMaterialTextureOption(const tinyobj::texture_option_t& texOpt)
    {
        return {
                ((texOpt.type >= 2) ? (Texture::TextureType)2 : (Texture::TextureType)texOpt.type), //TODO: Rework
                texOpt.sharpness,
                texOpt.brightness,
                texOpt.contrast,
                glm::vec3{texOpt.origin_offset[0], texOpt.origin_offset[1], texOpt.origin_offset[2]},
                glm::vec3{texOpt.scale[0], texOpt.scale[1], texOpt.scale[2]},
                glm::vec3{texOpt.turbulence[0], texOpt.turbulence[1], texOpt.turbulence[2]},
                static_cast<uint32_t>(texOpt.texture_resolution),
                texOpt.clamp,
                texOpt.imfchan,
                texOpt.blendu,
                texOpt.blendv,
                texOpt.bump_multiplier,
                texOpt.colorspace
        };
    }
}