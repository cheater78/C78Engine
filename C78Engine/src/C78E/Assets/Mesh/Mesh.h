#pragma once
#include <C78E/Core/Types.h>

namespace C78E {

	class Mesh
	{
	public:
		static Ref<Mesh> create(std::string filename) { return createRef<Mesh>(); /* dummy for AssetMmg */ }

		Mesh() = default;
		Mesh(const std::vector<Vertex>&vertecies)
			: m_Vertecies(vertecies), m_Indicies(), m_HasIndicies(false)
		{ }
		Mesh(const std::vector<Vertex>& vertecies, const std::vector<uint32_t>& indicies)
			: m_Vertecies(vertecies), m_Indicies(indicies), m_HasIndicies(true)
		{ }
		Mesh(const Mesh&) = default;
		~Mesh() = default;

	public:
		std::vector<Vertex> m_Vertecies;
		std::vector<uint32_t> m_Indicies;
	private:

	private:
		bool m_HasIndicies = false;
	};
}