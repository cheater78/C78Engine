#include <C78EPCH.h>
#include "WavefrontLoader.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace C78E {

    Ref<WavefrontLoader::WavefrontModel> WavefrontLoader::loadModel(FilePath file) {
        C78E_CORE_TRACE("WavefrontLoader::loadModel: Loading Wavefront Model File: {}", file.string());
        Timer timer;
        
        C78E_CORE_ASSERT(file.extension() == ".obj", "WavefrontLoader::loadModel: cannot parse non .obj files!");

        Ref<WavefrontModel> wvfModel = createRef<WavefrontModel>();
        /*
        tinyobj::ObjReaderConfig reader_config;
        // Path to material files must be in the same directory as the .stl
        //reader_config.mtl_search_path = ""; 

        tinyobj::ObjReader reader;

        if (!reader.ParseFromFile(file.string(), reader_config)) {
            C78E_CORE_ERROR("WavefrontLoader::loadModel:   failed to read Wavefront Model File: \n   " + reader.Error());
            return nullptr;
        }

        if (!reader.Warning().empty()) {
            C78E_CORE_WARN("WavefrontLoader::loadModel:   Parsed File successfully, Status: Warning\n   " + reader.Warning());
        }
        else {
            C78E_CORE_TRACE("WavefrontLoader::loadModel:   Parsed File successfully, Status: Ok");
        }

        C78E_CORE_TRACE("WavefrontLoader::loadModel:   Reading Data:");
        auto& attrib = reader.GetAttrib();
        auto& shapes = reader.GetShapes();
        auto& materials = reader.GetMaterials();

        // copy all materials
        for (uint64_t mat_id = 0; mat_id < materials.size(); mat_id++) {
            wvfModel->materialNames.emplace(mat_id, materials.at(mat_id).name);
            wvfModel->materials.emplace(mat_id, WavefrontLoader::toMaterial(materials.at(mat_id)));
        }
        C78E_CORE_TRACE("WavefrontLoader::loadModel:     Materials: {}", materials.size());
        
        uint64_t meshID = 0;
        // Loop over shapes
        for (size_t s = 0; s < shapes.size(); s++) {
            tinyobj::shape_t shape = shapes[s];
            std::string shapeName = shape.name;

            std::unordered_map<Primitive::Vertex, uint32_t> uniqueVertecies{};
            std::vector<Primitive::Vertex> vertecies{};
            std::vector<Primitive::VertexColor> vertexColors{};
            std::vector<Primitive::VertexTexture> vertexTextures{};
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
                    // Material Changed -> Save Current Model(if not first run)
                    if (materialID != -1) {
                        // Material Changed and not first run -> Save Current Model and start a new one
                        Ref<Mesh> mesh = createRef<Mesh>(vertecies, indecies);
                        wvfModel->meshes.emplace(meshID, mesh);
                        wvfModel->meshNames.emplace(meshID, shapeName);

                        wvfModel->parts.emplace(meshID, materialID);

                        meshID++;

                        uniqueVertecies.clear();
                        vertecies.clear();
                        indecies.clear();
                    }
                    materialID = material_id;
                }
                

                // Loop over vertices in the face.
                for (size_t v = 0; v < fv; v++) {
                    tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                    Primitive::Vertex vertex;
                    vertex.position = {
                        attrib.vertices[3 * size_t(idx.vertex_index) + 0],
                        attrib.vertices[3 * size_t(idx.vertex_index) + 1],
                        attrib.vertices[3 * size_t(idx.vertex_index) + 2]
                    };

                    // Check if `normal_index` is zero or positive. negative = no normal data
                    vertex.normal = (idx.normal_index >= 0) ? glm::vec3{
                        attrib.normals[3 * size_t(idx.normal_index) + 0],
                        attrib.normals[3 * size_t(idx.normal_index) + 1],
                        attrib.normals[3 * size_t(idx.normal_index) + 2]
                    } : glm::vec3{ 0, 0, 0 };
                    
                    Primitive::VertexColor vertexColor;
                    vertexColor.color = glm::vec4{
                        attrib.colors[3 * size_t(idx.vertex_index) + 0],
                        attrib.colors[3 * size_t(idx.vertex_index) + 1],
                        attrib.colors[3 * size_t(idx.vertex_index) + 2],
                        1
                    };

                    Primitive::VertexTexture vertexTexture;
                    // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                    vertexTexture.textureCoordinate = (idx.texcoord_index >= 0) ? glm::vec2{
                        attrib.texcoords[2 * size_t(idx.texcoord_index) + 0],
                        attrib.texcoords[2 * size_t(idx.texcoord_index) + 1]
                    } : glm::vec2{ 0, 0 };
                    vertexTexture.textureIndex = 0;

                    //TODO: unique Vertecies by all elements!
                    if (uniqueVertecies.count(vertex) <= 0) {
                        uniqueVertecies[vertex] = static_cast<uint32_t>(vertecies.size());
                        vertecies.push_back(vertex);
                        vertexColors.push_back(vertexColor);
                        if(idx.texcoord_index >= 0)
                            vertexTextures.push_back(vertexTexture);
                    }
                    indecies.push_back(uniqueVertecies[vertex]);
                }
                index_offset += fv;
            }

            Ref<Mesh> mesh = createRef<Mesh>(vertecies, indecies);
            mesh->setVertexColorData(vertexColors);
            if(!vertexTextures.empty())
                mesh->setVertexTextureData(vertexTextures);
            wvfModel->meshes.emplace(meshID, mesh);
            wvfModel->meshNames.emplace(meshID, shapeName);

            wvfModel->parts.emplace(meshID, materialID);

            meshID++;
        }
        C78E_CORE_TRACE("WavefrontLoader::loadModel:     Models: {}", wvfModel->parts.size());
        C78E_CORE_TRACE("WavefrontLoader::loadModel:     Meshes: {}", wvfModel->meshes.size());
        */
        C78E_CORE_TRACE("FontLoader::loadFont: Loading Wavefront Model Successful, Took: {} ms", std::to_string(timer.elapsedMillis()));
        return wvfModel;
    }

    Ref<WavefrontLoader::WavefrontMaterial> WavefrontLoader::loadMaterial(FilePath file) {
        C78E_CORE_TRACE("WavefrontLoader::loadMaterial: loading File: {}", file.string());
        std::string matId;
        std::vector<tinyobj::material_t> materials;
        std::map<std::string, int> matMap;
        std::string warn;
        std::string err = "";

        tinyobj::MaterialFileReader reader(file.parent_path().string());
        reader(matId, &materials, &matMap, &warn, &err);

        if (!warn.empty()) C78E_CORE_WARN("WavefrontLoader::loadMaterial: tinyobj warn:\n   {}", warn);

        if (!err.empty()) {
            C78E_CORE_ERROR("WavefrontLoader::loadMaterial: Failed to load File: {} :\n   {}", file.string(), err);
            C78E_ASSERT(false);
            return createRef<WavefrontMaterial>();
        }

        Ref<WavefrontMaterial> material = createRef<WavefrontMaterial>();
        for (auto& kv : matMap) {
            material->materialNames.emplace(kv.second, kv.first);
            material->materials.emplace(kv.second, toMaterial(materials.at(kv.second)));
            C78E_CORE_TRACE("WavefrontLoader::loadMaterial:   - {}", kv.first);
        }

        return material;
    }


    Ref<Material> WavefrontLoader::toMaterial(const tinyobj::material_t& material) {
        return createRef<Material>(
            AssetHandle::invalid(), //TODO Shader handle
            material.illum,
            toMaterialProperties(material),
            toMaterialTextures(material),
            toMaterialPropertiesPBRext(material),
            toMaterialTexturesPBRext(material)
        );
    }

    Material::MaterialProperties WavefrontLoader::toMaterialProperties(const tinyobj::material_t& material) {
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

    Material::MaterialTextures WavefrontLoader::toMaterialTextures(const tinyobj::material_t& material) {
        return {
            AssetHandle::invalid(),AssetHandle::invalid(),AssetHandle::invalid(),AssetHandle::invalid(),AssetHandle::invalid(),AssetHandle::invalid(),AssetHandle::invalid(),AssetHandle::invalid()
        };
        /*
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
        */
    }

    Material::MaterialPropertiesPBRext WavefrontLoader::toMaterialPropertiesPBRext(const tinyobj::material_t& material) {
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

    Material::MaterialTexturesPBRext WavefrontLoader::toMaterialTexturesPBRext(const tinyobj::material_t& material) {
        return { AssetHandle::invalid(),AssetHandle::invalid(),AssetHandle::invalid(),AssetHandle::invalid(),AssetHandle::invalid() };

        /*
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
        */
    }

    Texture::TextureOption WavefrontLoader::toMaterialTextureOption(const tinyobj::texture_option_t& texOpt) {
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