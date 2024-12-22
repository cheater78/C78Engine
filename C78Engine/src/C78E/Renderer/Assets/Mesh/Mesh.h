#pragma once
#include <C78E/Renderer/Assets/Asset/Asset.h>

namespace C78E {

	class Mesh : public Asset {
	public:
		using Index = size_t;
		using Position = glm::vec3;
		using Normal = glm::vec3;
		using TextureCoordinate = glm::vec2;
		using Color = glm::vec4;


		using VertexIndex = Index;
		struct VertexHandle;
		using HalfedgeIndex = Index;
		struct HalfedgeHandle;
		using FaceIndex = Index;
		struct FaceHandle;

	public:

		template<typename T>
		class HandleIterator {
		protected:
			HandleIterator(Mesh* mesh, const std::vector<Index>& elements, std::function<T(Mesh&, Index)> at) : m_Mesh{mesh}, m_Indecies{elements}, m_At{at} { }
			HandleIterator(const HandleIterator<T>& other) : m_Mesh{ other.m_Mesh }, m_Indecies{ other.m_Indecies }, m_Current{other.m_Current} { }
			~HandleIterator() {}
		public:
			HandleIterator<T>& operator=(const HandleIterator<T>& rawIterator) = default;

			operator bool() const { return m_Current == (size_t) - 1; }

			bool operator==(const HandleIterator<T>& rawIterator) const { return m_Mesh == rawIterator.m_Mesh && m_Indecies == rawIterator.m_Indecies && m_Current == rawIterator.m_Current; }
			bool operator!=(const HandleIterator<T>& rawIterator) const { return !(*this == rawIterator); }

			HandleIterator<T>& operator+=(const size_t& movement) { m_Current = (m_Current + 1 < m_Indecies.size()) m_Current + 1 : -1; return *this; }
			HandleIterator<T>& operator-=(const size_t& movement) { m_Current = (m_Current > 0) m_Current - 1 : -1; return *this; }
			HandleIterator<T>& operator++() { *this += 1; return *this; }
			HandleIterator<T>& operator--() { *this -= 1; return *this; }
			HandleIterator<T>  operator++(int) { auto temp(*this); *this += 1; return temp; }
			HandleIterator<T>  operator--(int) { auto temp(*this); *this -= 1; return temp; }
			HandleIterator<T>  operator+(const size_t& movement) { auto temp(*this); temp += movement; return temp; }
			HandleIterator<T>  operator-(const size_t& movement) { auto temp(*this); temp -= movement; return temp; }

			size_t operator-(const HandleIterator<T>& other) { return (m_Current != -1 && other.m_Current != -1 && m_Current >= other.m_Current) ? m_Current - other.m_Current : 0; }

			T operator*() { C78E_CORE_ASSERT(m_Current != -1, "Mesh::HandleIterator: operator*() cant be called on end!"); return m_At(m_Mesh, m_Indecies.at(m_Current)); }
			const T operator*() const { C78E_CORE_ASSERT(m_Current != -1, "Mesh::HandleIterator: const operator*() cant be called on end!"); return m_At(m_Mesh, m_Indecies.at(m_Current)); }
			T operator->() { return *this; }	
			
		protected:
			Mesh& m_Mesh;
			std::vector<Index> m_Indecies;
			Index m_Current = -1;
			std::function<T(Mesh&, Index)> m_At;
		};

		class VertexIterator : public HandleIterator<VertexHandle> {
		public:
			static VertexHandle access(Mesh& mesh, VertexIndex index) { return VertexHandle(mesh, index); }
		public:
			VertexIterator(Mesh* mesh, const std::vector<Index>& elements) : HandleIterator<VertexHandle>(mesh, elements, access) {}
		};

		class HalfedgeIterator : public HandleIterator<HalfedgeHandle> {
		public:
			static HalfedgeHandle access(Mesh& mesh, HalfedgeIndex index) { return HalfedgeHandle(mesh, index); }
		public:
			HalfedgeIterator(Mesh* mesh, const std::vector<Index>& elements) : HandleIterator<HalfedgeHandle>(mesh, elements, access) {}
		};

		class FaceIterator : public HandleIterator<FaceHandle> {
		public:
			static FaceHandle access(Mesh& mesh, FaceIndex index) { return FaceHandle(mesh, index); }
		public:
			FaceIterator(Mesh* mesh, const std::vector<Index>& elements) : HandleIterator<FaceHandle>(mesh, elements, access) {}
		};

		

		template<typename T>
		class StorageIterator : private std::iterator<std::bidirectional_iterator_tag, T> {
		public:

		private:
			std::vector<T>& m_Data;

		};

	private: // Mesh Topology
		struct BaseHandle {
		protected:
			BaseHandle() = delete;
			BaseHandle(Mesh& mesh, Index index) : m_Mesh(mesh), m_Index(index) { }
			~BaseHandle() = default;
		public:
			virtual Mesh& mesh() final { return m_Mesh; }
			virtual bool valid() const { return m_Index == (size_t)-1; }
			operator bool() const { return valid(); }
		protected:
			Index m_Index = (Index)-1;
			Mesh& m_Mesh;
		};
	public:
		struct VertexHandle : public BaseHandle {
		public:
			VertexHandle() = delete;
			VertexHandle(Mesh& mesh, VertexIndex index) : BaseHandle(mesh, index) { }
			~VertexHandle() = default;

			VertexIndex idx() const { return m_Index; }
			virtual bool valid() const override { return BaseHandle::valid() && m_Index < m_Mesh.m_Positions.size(); }

			Position& position();
			HalfedgeHandle halfedge();

			VertexIterator vertecies();
			HalfedgeIterator halfedges();
			FaceIterator faces();
		};

		struct HalfedgeHandle : public BaseHandle {
		public:
			HalfedgeHandle() = delete;
			HalfedgeHandle(Mesh& mesh, HalfedgeIndex index) : BaseHandle(mesh, index) { }
			~HalfedgeHandle() = default;

			HalfedgeIndex idx() const { return m_Index; }
			virtual bool valid() const override { return BaseHandle::valid() && m_Index < m_Mesh.m_Halfedges.size(); }

			VertexHandle from();
			VertexHandle to();

			HalfedgeHandle next();
			HalfedgeHandle prev();
			HalfedgeHandle opp();

			FaceHandle face();
		};

		struct FaceHandle : public BaseHandle {
		public:
			FaceHandle() = delete;
			FaceHandle(Mesh& mesh, FaceIndex index) : BaseHandle(mesh, index) { }
			~FaceHandle() = default;

			FaceIndex idx() const { return m_Index; }
			virtual bool valid() const override { return BaseHandle::valid() && m_Index < m_Mesh.m_FaceCount; }

			HalfedgeHandle halfedge();

			VertexIterator vertecies();
			HalfedgeIterator halfedges();
			FaceIterator faces();
		};
		
	public: // Custom Meshdata
		using PropertyIndex = size_t;
	private:
		template<typename T>
		struct BasePropertyHandle {
		protected:
			BasePropertyHandle() = delete;
			BasePropertyHandle(Mesh& mesh, PropertyIndex propertyIndex) : m_Mesh(mesh), m_PropertyIndex(propertyIndex) { }
			~BasePropertyHandle() = default;
		protected:
			PropertyIndex m_PropertyIndex = (PropertyIndex)-1;
			Mesh& m_Mesh;
		};
	public:
		template<typename T>
		struct VPropertyHandle : public BasePropertyHandle<T> {
		protected:
			VPropertyHandle() = delete;
			VPropertyHandle(Mesh& mesh, PropertyIndex propertyIndex) : BasePropertyHandle<T>(mesh, propertyIndex) { }
			~VPropertyHandle() = default;
		public:
			T& propValue(VertexIndex index);
		};

		template<typename T>
		struct HPropertyHandle : public BasePropertyHandle<T> {
		protected:
			HPropertyHandle() = delete;
			HPropertyHandle(Mesh& mesh, PropertyIndex propertyIndex) : BasePropertyHandle<T>(mesh, propertyIndex) { }
			~HPropertyHandle() = default;
		public:
			T& propValue(HalfedgeIndex index);
		};

		template<typename T>
		struct FPropertyHandle : public BasePropertyHandle<T> {
		protected:
			FPropertyHandle() = delete;
			FPropertyHandle(Mesh& mesh, PropertyIndex propertyIndex) : BasePropertyHandle<T>(mesh, propertyIndex) { }
			~FPropertyHandle() = default;
		public:
			T& propValue(FaceIndex index);
		};

	public:
		Mesh();
		Mesh(const Mesh&) = delete;
		~Mesh() = default;

		// Mesh Topology
		VertexHandle addVertex(Position position, Normal normal = {}, Color color = {}, TextureCoordinate textureCoordinate = {});
		HalfedgeHandle addHalfedge(VertexHandle from, VertexHandle to);
		FaceHandle addFace(VertexHandle vh0, VertexHandle vh1, VertexHandle vh2);

		VertexHandle vertex(VertexIndex vIndex);
		HalfedgeHandle halfedge(HalfedgeIndex hIndex);
		FaceHandle face(FaceIndex fIndex);

		// Meshdata
		Position& position(VertexIndex vIndex);
		VertexIterator positions();

		bool hasNormals();
		Normal& normal(VertexIndex vIndex);
		VertexIterator normals();

		bool hasColor();
		Color& color(VertexIndex vIndex);
		VertexIterator colors();

		bool hasTextureCoordinates();
		TextureCoordinate& textureCoordinate(VertexIndex vIndex);
		VertexIterator textureCoordinates();

		bool positionExists(Position position);
		bool normalExists(Normal normal);
		bool colorExists(Color color);
		bool textureCoordinateExists(TextureCoordinate textureCoordinate);
		bool halfedgeVerteciesExist(VertexHandle from, VertexHandle to);
		bool FaceVerteciesExist(VertexHandle vh0, VertexHandle vh1, VertexHandle vh2);
		bool FaceHalfedgesExist(HalfedgeHandle he0, HalfedgeHandle he1, HalfedgeHandle he2);

		// Custom Meshdata
		template<typename T>
		VPropertyHandle<T> createVertexProperty(const T& initialValue = T());

		template<typename T>
		HPropertyHandle<T> createHalfedgeProperty(const T& initialValue = T());

		template<typename T>
		FPropertyHandle<T> createFaceProperty(const T& initialValue = T());

		template<typename T>
		T& property(BasePropertyHandle<T> propertyHandle, Index index);

	public:
		virtual Type getType() const override { return Asset::Type::Mesh; };
		static Type getClassType() { return Type::Mesh; };
	private:
		// Meshdata
		std::vector<Position> m_Positions;

		using NormalIndex = size_t;
		std::map<VertexIndex, NormalIndex> m_NormalMap;
		std::vector<Normal> m_Normals;

		using ColorIndex = size_t;
		std::map<VertexIndex, ColorIndex> m_ColorMap;
		std::vector<Color> m_Colors;

		using TextureCoordinateIndex = size_t;
		std::map<VertexIndex, TextureCoordinateIndex> m_TextureCoordinateMap;
		std::vector<TextureCoordinate> m_TextureCoordinates;

		// Custom Meshdata
		class PropertyContainer {
		public:
			PropertyContainer() = delete;
			PropertyContainer(size_t elementSize, size_t elementCount = 0);
			~PropertyContainer();

			void clear(void* clearValue = nullptr);
			void resize(size_t count);
			void* at(size_t index);

			size_t size() const { return m_ElementCount; }
			size_t sizeOfElement() const { return m_ElementSize; }

		private:
			void alloc();
			void free();
		private:
			const size_t m_ElementSize; // ByteSize of an Element
			void* m_ElementData; // Base ptr to first element
			size_t m_ElementCount; // Count of elements
		};
		std::vector<PropertyContainer> m_Properties;

		// Mesh Topology
		class Topology {
			/*
			Implicit Topology

			he0 \
			he1  | Triangle/Face 0
			he2 /

			he3
			he4
			he5

			...
			heN-1 <- (m_FaceCount * 3 - 1)

			heN <- faceless edge
			heN+1
			...

			*/
		public:
			Topology() = delete;
			Topology(Mesh* mesh);
			Topology(const Topology& other) = delete;
			~Topology() = default;

			HalfedgeIndex addHalfedge(VertexIndex v0, VertexIndex v1);
			FaceIndex addFace(HalfedgeIndex he0, HalfedgeIndex he1, HalfedgeIndex he2);

			HalfedgeIterator halfedges();
			FaceIterator faces();

		private:
			size_t vertexCount() const { return m_Mesh->m_Positions.size(); }
		private:
			Mesh* m_Mesh;
			std::vector<std::pair<VertexIndex, VertexIndex>> m_Halfedges;
			size_t m_FaceCount = 0;
		};

		Topology m_Topology;
	};
}