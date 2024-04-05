#pragma once
#include <C78E/Core/Types.h>
#include <C78E/Assets/Asset/Asset.h>

namespace C78E {

	class Mesh : public Asset {
	public:

		Mesh() = delete;
		Mesh(const std::vector<Vertex>&vertecies)
			: m_Vertecies(vertecies), m_Indicies(), m_HasIndicies(false)
		{ }
		Mesh(const std::vector<Vertex>& vertecies, const std::vector<uint32_t>& indicies)
			: m_Vertecies(vertecies), m_Indicies(indicies), m_HasIndicies(true)
		{ }
		Mesh(const Mesh&) = delete;
		~Mesh() = default;
	public:
		virtual AssetType getType() { return Asset::AssetType::Mesh; };
	public:
		std::vector<Vertex> m_Vertecies;
		std::vector<uint32_t> m_Indicies;
	private:

	private:
		bool m_HasIndicies = false; // TODO: unused...
	};
}