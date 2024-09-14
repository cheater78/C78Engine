#pragma once
#include <C78E/Core/Primitives.h>
#include <C78E/Core/Geometry.h>
#include <C78E/Renderer/Assets/Asset/Asset.h>

namespace C78E {

	class Mesh : public Asset {
	public:
		Mesh(const std::vector<Primitive::Vertex>& vertecies)
			: m_Vertecies(vertecies), m_Indicies(), m_HasIndicies(false)
		{ }
		Mesh(const std::vector<Primitive::Vertex>& vertecies, const std::vector<uint32_t>& indicies)
			: m_Vertecies(vertecies), m_Indicies(indicies), m_HasIndicies(true)
		{ }
		Mesh(const Mesh&) = delete;
		~Mesh() = default;

		std::vector<Geometry::Triangle> getTriangles() const {
			std::vector<Geometry::Triangle> triangles;

			if (m_HasIndicies) {
				C78_CORE_ASSERT(m_Indicies.size() % 3 == 0, "Mesh::getTriangles: m_Indicies doesn't have a multiple of 3 Elements!");

				triangles.reserve(m_Indicies.size() / 3);
				for (uint32_t i = 0; i < m_Indicies.size(); i += 3) {
					triangles.push_back(Geometry::Triangle{ m_Vertecies[m_Indicies[i]], m_Vertecies[m_Indicies[i+1]], m_Vertecies[m_Indicies[i+2]] });
				}
			}
			else {
				C78_CORE_ASSERT(m_Vertecies.size() % 3 == 0, "Mesh::getTriangles: m_Vertecies doesn't have a multiple of 3 Elements!");

				triangles.reserve(m_Vertecies.size() / 3);
				for (uint32_t i = 0; i < m_Vertecies.size(); i += 3) {
					triangles.push_back(Geometry::Triangle{ m_Vertecies[i], m_Vertecies[i + 1], m_Vertecies[i + 2] });
				}
			}
			return triangles;
		}

		uint32_t* getIndexData() { return m_Indicies.data(); }
		uint32_t getIndexCount() { return static_cast<uint32_t>(m_Indicies.size()); }

		Primitive::Vertex* getVertexData() { return m_Vertecies.data(); }
		uint32_t getVertexCount() { return static_cast<uint32_t>(m_Vertecies.size()); }
		uint32_t getVertexByteSize() { return static_cast<uint32_t>(getVertexCount() * sizeof(Primitive::Vertex)); }

		bool hasVertexColor() { return !m_VertexColors.empty(); }
		Primitive::VertexColor* getVertexColorData() { return m_VertexColors.data(); }
		void setVertexColorData(const std::vector<Primitive::VertexColor>& vertexColors) { m_VertexColors = vertexColors; }
		uint32_t getVertexColorByteSize() { return static_cast<uint32_t>(getVertexCount() * sizeof(Primitive::VertexColor)); }

		bool hasVertexTexture() { return !m_VertexTextures.empty(); }
		Primitive::VertexTexture* getVertexTextureData() { return m_VertexTextures.data(); }
		void setVertexTextureData(const std::vector<Primitive::VertexTexture>& vertexTextures) { m_VertexTextures = vertexTextures; }
		uint32_t getVertexTextureByteSize() { return static_cast<uint32_t>(getVertexCount() * sizeof(Primitive::VertexTexture)); }

	public:
		virtual AssetType getType() const override { return Asset::AssetType::Mesh; };
		static AssetType getClassType() { return AssetType::Mesh; };
	public:
		std::vector<uint32_t> m_Indicies;
		std::vector<Primitive::Vertex> m_Vertecies;
		std::vector<Primitive::VertexColor> m_VertexColors;
		std::vector<Primitive::VertexTexture> m_VertexTextures;
	private:
		bool m_HasIndicies = false;
	};
}