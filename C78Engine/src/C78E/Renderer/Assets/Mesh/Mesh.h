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


		using NormalIndex = size_t;
		using ColorIndex = size_t;
		using TextureCoordinateIndex = size_t;

	public:

		template<typename T>
		class MeshIterator {
		protected:
			MeshIterator(Mesh& mesh, const std::vector<Index>& elements, std::function<T(Mesh&, Index)> at) : m_Mesh{ mesh }, m_Indecies{ elements }, m_At{ at } { reset(); }
			MeshIterator(const MeshIterator<T>& other) : m_Mesh{ other.m_Mesh }, m_Indecies{ other.m_Indecies }, m_Current{other.m_Current}, m_At{ other.m_At } { }
		public:
			~MeshIterator() {}
			MeshIterator<T>& operator=(const MeshIterator<T>& rawIterator) = default;

			operator bool() const { return m_Current == (size_t) - 1; }

			bool operator==(const MeshIterator<T>& rawIterator) const { return m_Mesh == rawIterator.m_Mesh && m_Indecies == rawIterator.m_Indecies && m_Current == rawIterator.m_Current; }
			bool operator!=(const MeshIterator<T>& rawIterator) const { return !(*this == rawIterator); }

			MeshIterator<T>& operator+=(const size_t& movement) { m_Current = (m_Current + 1 < m_Indecies.size()) ? (m_Current + 1) : -1; return *this; }
			MeshIterator<T>& operator-=(const size_t& movement) { m_Current = (m_Current > 0) ? (m_Current - 1) : -1; return *this; }
			MeshIterator<T>& operator++() { *this += 1; return *this; }
			MeshIterator<T>& operator--() { *this -= 1; return *this; }
			MeshIterator<T>  operator++(int) { auto temp(*this); *this += 1; return temp; }
			MeshIterator<T>  operator--(int) { auto temp(*this); *this -= 1; return temp; }
			MeshIterator<T>  operator+(const size_t& movement) { auto temp(*this); temp += movement; return temp; }
			MeshIterator<T>  operator-(const size_t& movement) { auto temp(*this); temp -= movement; return temp; }

			size_t operator-(const MeshIterator<T>& other) { return (m_Current != -1 && other.m_Current != -1 && m_Current >= other.m_Current) ? m_Current - other.m_Current : 0; }

			T operator*() { C78E_CORE_ASSERT(m_Current != -1, "Mesh::MeshIterator: operator*() cant be called on end!"); return m_At(m_Mesh, m_Indecies.at(m_Current)); }
			const T operator*() const { C78E_CORE_ASSERT(m_Current != -1, "Mesh::MeshIterator: const operator*() cant be called on end!"); return m_At(m_Mesh, m_Indecies.at(m_Current)); }
			T operator->() { return *this; }

			void reset() { m_Current = 0; }
			size_t count() const { return m_Indecies.size(); }

			MeshIterator find(const T& obj) const {
				MeshIterator it = *this;
				for(reset(); it; it++)
					if(*it = obj)
						return it;
				it->m_Current = -1;
				return it;
			}

			MeshIterator unionIter(const MeshIterator& other) const {
				std::vector<Index> indecies = m_Indecies;
				for(Index idx : other.m_Indecies)
					if(std::find(m_Indecies.begin(), m_Indecies.end(), idx) == m_Indecies.end())
						indecies.emplace_back(idx);
				return MeshIterator(m_Mesh, indecies, m_At);
			}

			MeshIterator intersectionIter(const MeshIterator& other) const {
				std::vector<Index> indecies = std::vector<Index>();
				for(Index idx : other.m_Indecies)
					if(std::find(m_Indecies.begin(), m_Indecies.end(), idx) != m_Indecies.end())
						indecies.emplace_back(idx);
				return MeshIterator(m_Mesh, indecies, m_At);
			}

			MeshIterator differenceIter(const MeshIterator& other) const {
				std::vector<Index> indecies = std::vector<Index>();
				for(Index idx : m_Indecies)
					if(std::find(other.m_Indecies.begin(), other.m_Indecies.end(), idx) == other.m_Indecies.end())
						indecies.emplace_back(idx);
				return MeshIterator(m_Mesh, indecies, m_At);
			}

		protected:
			Mesh& m_Mesh;
			std::vector<Index> m_Indecies;
			Index m_Current = -1;
			std::function<T(Mesh&, Index)> m_At;
		};

		class VertexIterator : public MeshIterator<VertexHandle> {
		protected:
			static VertexHandle access(Mesh& mesh, VertexIndex index) { return VertexHandle(mesh, index); }
		public:
			VertexIterator(Mesh& mesh, const std::vector<Index>& elements) : MeshIterator<VertexHandle>(mesh, elements, access) {}
		};

		class HalfedgeIterator : public MeshIterator<HalfedgeHandle> {
		protected:
			static HalfedgeHandle access(Mesh& mesh, HalfedgeIndex index) { return HalfedgeHandle(mesh, index); }
		public:
			HalfedgeIterator(Mesh& mesh, const std::vector<Index>& elements) : MeshIterator<HalfedgeHandle>(mesh, elements, access) {}
		};

		class FaceIterator : public MeshIterator<FaceHandle> {
		protected:
			static FaceHandle access(Mesh& mesh, FaceIndex index) { return FaceHandle(mesh, index); }
		public:
			FaceIterator(Mesh& mesh, const std::vector<Index>& elements) : MeshIterator<FaceHandle>(mesh, elements, access) {}
		};

		class PositionIterator : public MeshIterator<Position> {
		protected:
			static Position access(Mesh& mesh, VertexIndex index) { return mesh.m_Positions[index]; }
		public:
			PositionIterator(Mesh& mesh, const std::vector<Index>& elements) : MeshIterator<Position>(mesh, elements, access) {}
		};

		class NormalIterator : public MeshIterator<Normal> {
		protected:
			static Normal access(Mesh& mesh, NormalIndex index) { return mesh.m_Normals[index]; }
		public:
			NormalIterator(Mesh& mesh, const std::vector<Index>& elements) : MeshIterator<Normal>(mesh, elements, access) {}
		};

		class ColorIterator : public MeshIterator<Color> {
		protected:
			static Color access(Mesh& mesh, ColorIndex index) { return mesh.m_Colors[index]; }
		public:
			ColorIterator(Mesh& mesh, const std::vector<Index>& elements) : MeshIterator<Color>(mesh, elements, access) {}
		};

		class TextureCoordinateIterator : public MeshIterator<TextureCoordinate> {
		protected:
			static TextureCoordinate access(Mesh& mesh, TextureCoordinateIndex index) { return mesh.m_TextureCoordinates[index]; }
		public:
			TextureCoordinateIterator(Mesh& mesh, const std::vector<Index>& elements) : MeshIterator<TextureCoordinate>(mesh, elements, access) {}
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
			virtual bool valid() const override;

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
			virtual bool valid() const override;

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
			virtual bool valid() const override;

			VertexHandle vertex();
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

		// Meshdata
		Position& position(VertexIndex vIndex);
		PositionIterator positions();

		bool hasNormals() const;
		Normal& normal(VertexIndex vIndex);
		NormalIterator normals();

		bool hasColor() const;
		Color& color(VertexIndex vIndex);
		ColorIterator colors();

		bool hasTextureCoordinates() const;
		TextureCoordinate& textureCoordinate(VertexIndex vIndex);
		TextureCoordinateIterator textureCoordinates();

		bool positionExists(Position position) const;
		bool normalExists(Normal normal) const;
		bool colorExists(Color color) const;
		bool textureCoordinateExists(TextureCoordinate textureCoordinate) const;
		
		// Mesh Topology
		VertexHandle addVertex(Position position, Normal normal = {}, Color color = {}, TextureCoordinate textureCoordinate = {});
		HalfedgeHandle addHalfedge(VertexHandle from, VertexHandle to);
		FaceHandle addFace(VertexHandle vh0, VertexHandle vh1, VertexHandle vh2);

		VertexHandle vertex(VertexIndex vIndex);
		HalfedgeHandle halfedge(HalfedgeIndex hIndex);
		FaceHandle face(FaceIndex fIndex);
		
		bool vertexHasHalfedge(VertexIndex v) const;
		bool vertexHasFace(VertexIndex v) const;
		VertexIterator vertecies();
		VertexIterator vertecies(VertexIndex v);

		bool halfedgeVerteciesExist(VertexHandle from, VertexHandle to) const;
		bool halfedgeHasFace(HalfedgeIndex index) const;
		FaceHandle halfedgeFace(HalfedgeIndex he);
		HalfedgeHandle halfedgeOpposite(HalfedgeIndex he);
		HalfedgeIterator halfedges();
		const HalfedgeIterator halfedges() const;
		HalfedgeIterator halfedges(VertexIndex v, bool outgoing = true);

		bool faceVerteciesExist(VertexHandle vh0, VertexHandle vh1, VertexHandle vh2) const;
		bool faceHalfedgesExist(HalfedgeHandle he0, HalfedgeHandle he1, HalfedgeHandle he2) const;
		HalfedgeIterator faceHalfedges(FaceIndex f, bool cw = true);
		FaceIterator faces();
		const FaceIterator faces() const;
		FaceIterator faces(VertexIndex v);
		const FaceIterator faces(VertexIndex v) const;

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

		std::map<VertexIndex, NormalIndex> m_NormalMap;
		std::vector<Normal> m_Normals;

		std::map<VertexIndex, ColorIndex> m_ColorMap;
		std::vector<Color> m_Colors;

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
			using Halfedge = std::pair<VertexIndex, VertexIndex>;
			struct Face {
				Halfedge he0;
				Halfedge he1;
				Halfedge he2;
			};

			Topology() = delete;
			Topology(Mesh& mesh);
			Topology(const Topology& other) = delete;
			~Topology() = default;

			HalfedgeIndex addHalfedge(VertexIndex from, VertexIndex to);
			FaceIndex addFace(HalfedgeIndex he0, HalfedgeIndex he1, HalfedgeIndex he2);

			bool vertexHasHalfedge(VertexIndex v) const;
			bool vertexHasFace(VertexIndex v) const;

			const Halfedge& halfedge(HalfedgeIndex index) const;
			bool halfedgeHasFace(HalfedgeIndex index) const;
			bool halfedgeIsBoundary(HalfedgeIndex index) const;
			HalfedgeIndex halfedgeOpposite(HalfedgeIndex index) const;
			FaceIndex halfedgeFaceIndex(HalfedgeIndex he) const;
			HalfedgeIterator halfedges();
			const HalfedgeIterator halfedges() const;
			HalfedgeIterator halfedges(VertexIndex v, bool outgoing = true);

			const Face& face(FaceIndex index) const;
			HalfedgeIterator faceHalfedges(FaceIndex f, bool cw = true);
			FaceIterator faces();
			const FaceIterator faces() const;
			FaceIterator faces(VertexIndex v);
			const FaceIterator faces(VertexIndex v) const;

		private:
			size_t vertexCount() const { return m_Mesh.m_Positions.size(); }
			size_t faceHalfedgeCount() const { return m_FaceCount * 3; }
		private:
			Mesh& m_Mesh;
			std::vector<Halfedge> m_Halfedges;
			size_t m_FaceCount = 0; // last Faces' HalfEdge is at FC*3 - 1
		};

		Topology m_Topology;
	};
}