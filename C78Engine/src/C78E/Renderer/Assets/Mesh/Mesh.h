#pragma once
#include <C78E/Renderer/Assets/Asset/Asset.h>

namespace C78E {

	/*
	* 
	*/
	class Mesh : public Asset {
	public:
		using Position = glm::vec3;
		using Normal = glm::vec3;
		using TextureCoordinate = glm::vec2;
		using Color = glm::vec4;
	
	public:
		using Index = size_t;
	private:
		struct BaseHandle {
		public:
			Index idx();
			Mesh& mesh();
		private:
			Index index = (Index)-1;
		};
	public:
		using VertexIndex = Index;
		struct VertexHandle : public BaseHandle {
			
		};

		using HalfedgeIndex = Index;
		struct HalfedgeHandle : public BaseHandle {
			
		};

		using FaceIndex = Index;
		struct FaceHandle : public BaseHandle {
			
		};


		/*
		* Custom Properties
		*/
	public:
		using PropertyIndex = size_t;
	private:
		template<typename T>
		struct BasePropertyHandle {
			PropertyIndex idx = (PropertyIndex)-1;
		};
	public:
		template<typename T>
		struct VPropertyHandle : public BasePropertyHandle<T> {
			
		};

		template<typename T>
		struct HPropertyHandle : public BasePropertyHandle<T> {
			
		};

		template<typename T>
		struct FPropertyHandle : public BasePropertyHandle<T> {
			
		};

	public:
		Mesh();
		Mesh(const Mesh&) = delete;
		~Mesh() = default;

		VertexHandle addVertex(Position position);

		Position& position(VertexHandle vhandle);

		bool hasNormals();
		Normal& normal(VertexHandle vhandle);

		bool hasTextureCoordinates();
		TextureCoordinate& textureCoordinate(VertexHandle vhandle);

		bool hasColor();
		Color& color(VertexHandle vhandle);


		

		template<typename T>
		VPropertyHandle<T> createVertexProperty();

		template<typename T>
		HPropertyHandle<T> createHalfedgeProperty();

		template<typename T>
		FPropertyHandle<T> createFaceProperty();

		template<typename T>
		T& property(BasePropertyHandle<T> propertyHandle, Index index);



	public:
		virtual Type getType() const override { return Asset::Type::Mesh; };
		static Type getClassType() { return Type::Mesh; };
	private:
		std::vector<Position> m_Positions;

		using NormalIndex = size_t;
		std::map<VertexIndex, NormalIndex> m_NormalMap;
		std::vector<Normal> m_Normals;

		using TextureCoordinateIndex = size_t;
		std::map<VertexIndex, TextureCoordinateIndex> m_TextureCoordinateMap;
		std::vector<TextureCoordinate> m_TextureCoordinates;

		using ColorIndex = size_t;
		std::map<VertexIndex, ColorIndex> m_ColorMap;
		std::vector<Color> m_Colors;


		struct PropertyContainer {
			void* data; //base ptr to first element
			size_t element; //byte size of the element
			size_t size; //count of elements
		};
		std::vector<PropertyContainer> m_Properties;
	};
}