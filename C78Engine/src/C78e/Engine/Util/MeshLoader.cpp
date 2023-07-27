#include "MeshLoader.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <filesystem>

namespace C78e {
    namespace MeshLoader {
        Mesh fromFile(const std::string& filePath) {
            const std::filesystem::path cwd = std::filesystem::current_path();
            const std::filesystem::path root = cwd.parent_path();

            Mesh mesh;

            //TinyObjLoader Setup
            tinyobj::attrib_t attrib;

            std::vector<tinyobj::shape_t> shapes;
            std::vector<tinyobj::material_t> materials;
            std::string warn, err;

            if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath.c_str())) {
                throw std::runtime_error(warn + " | " + err);
            }

            mesh.vertecies.clear();
            mesh.indecies.clear();

            for (const auto& shape : shapes) {
                for (const auto& index : shape.mesh.indices) {
                    Mesh::Vertex vert{};

                    if (index.vertex_index >= 0) {
                        vert.position = {
                            attrib.vertices[3 * index.vertex_index + 0],
                            attrib.vertices[3 * index.vertex_index + 1],
                            attrib.vertices[3 * index.vertex_index + 2]
                        };

                        vert.color = {
                            attrib.colors[3 * index.vertex_index + 0],
                            attrib.colors[3 * index.vertex_index + 1],
                            attrib.colors[3 * index.vertex_index + 2]
                        };
                    }

                    if (index.normal_index >= 0) {
                        vert.normal = {
                            attrib.normals[3 * index.normal_index + 0],
                            attrib.normals[3 * index.normal_index + 1],
                            attrib.normals[3 * index.normal_index + 2]
                        };
                    }

                    if (index.texcoord_index >= 0) {
                        vert.uv = {
                            attrib.texcoords[2 * index.texcoord_index + 0],
                            attrib.texcoords[2 * index.texcoord_index + 1]
                        };
                    }

                    bool hasVert = false;
                    uint32_t index = 0;
                    for (; index < mesh.vertecies.size(); index++) {
                        if (mesh.vertecies[index] == vert) {
                            hasVert = true;
                            break;
                        }

                    }
                    if (!hasVert) {
                        mesh.vertecies.push_back(vert);
                        index = static_cast<uint32_t>(mesh.vertecies.size()) - 1;
                    }
                    mesh.indecies.push_back(index);

                }
            }
            return mesh;
        }
    }
}