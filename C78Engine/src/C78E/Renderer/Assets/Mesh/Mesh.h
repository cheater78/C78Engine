#pragma once
#include <C78E/Core/Types.h> //TODO: resolve and remove
#include <C78E/Core/Primitives.h>
#include <C78E/Assets/Asset/Asset.h>

namespace C78E {

	class Mesh : public Asset {
	public:
		Mesh() = delete;


		//Deprecated
		Mesh(const std::vector<Vertex>& vertecies)
			: m_Indicies(), m_HasIndicies(false){
			
			for (auto& vertex : vertecies)
				m_Vertecies.push_back(Primitive::Vertex{ vertex.position, vertex.normal });
		}
		Mesh(const std::vector<Vertex>& vertecies, const std::vector<uint32_t>& indicies)
			: m_Indicies(indicies), m_HasIndicies(true) {
		
			for (auto& vertex : vertecies)
				m_Vertecies.push_back(Primitive::Vertex{ vertex.position, vertex.normal });
		}



		Mesh(const std::vector<Primitive::Vertex>&vertecies)
			: m_Vertecies(vertecies), m_Indicies(), m_HasIndicies(false)
		{ }
		Mesh(const std::vector<Primitive::Vertex>& vertecies, const std::vector<uint32_t>& indicies)
			: m_Vertecies(vertecies), m_Indicies(indicies), m_HasIndicies(true)
		{ }
		Mesh(const Mesh&) = delete;
		~Mesh() = default;

		std::vector<Primitive::Triangle> getTriangles() const {
			std::vector<Primitive::Triangle> triangles;

			if (m_HasIndicies) {
				C78_CORE_ASSERT(m_Indicies.size() % 3 == 0, "Mesh::getTriangles: m_Indicies doesn't have a multiple of 3 Elements!");

				triangles.reserve(m_Indicies.size() / 3);
				for (uint32_t i = 0; i < m_Indicies.size(); i += 3) {
					triangles.push_back(Primitive::Triangle{ m_Vertecies[m_Indicies[i]], m_Vertecies[m_Indicies[i+1]], m_Vertecies[m_Indicies[i+2]] });
				}
			}
			else {
				C78_CORE_ASSERT(m_Vertecies.size() % 3 == 0, "Mesh::getTriangles: m_Vertecies doesn't have a multiple of 3 Elements!");

				triangles.reserve(m_Vertecies.size() / 3);
				for (uint32_t i = 0; i < m_Vertecies.size(); i += 3) {
					triangles.push_back(Primitive::Triangle{ m_Vertecies[i], m_Vertecies[i + 1], m_Vertecies[i + 2] });
				}
			}
			return triangles;
		}

	public:
		virtual AssetType getType() { return Asset::AssetType::Mesh; };
	public:
		std::vector<uint32_t> m_Indicies;
		std::vector<Primitive::Vertex> m_Vertecies;
	private:
		bool m_HasIndicies = false; // TODO: unused...
	};
}