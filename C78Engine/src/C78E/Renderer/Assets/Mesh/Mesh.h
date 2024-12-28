#pragma once
#include <C78E/Renderer/Assets/Asset/Asset.h>

namespace C78E {

	class Mesh : public Asset {
	public:
		using Index = size_t;
		using Position = glm::vec4;
		using Normal = glm::vec4;
		using TextureCoordinate = glm::vec2;
		using Color = glm::vec4;


		using VertexIndex = Index;
		struct VertexHandle;
		using HalfedgeIndex = Index;
		struct HalfedgeHandle;
		using FaceIndex = Index;
		struct FaceHandle;


		using PositionIndex = VertexIndex;
		using NormalIndex = Index;
		using ColorIndex = Index;
		using TextureCoordinateIndex = Index;

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

			T get() { C78E_CORE_ASSERT(m_Current != -1, "Mesh::MeshIterator: operator*() cant be called on end!"); return m_At(m_Mesh, m_Indecies[m_Current]); }
			const T get() const { C78E_CORE_ASSERT(m_Current != -1, "Mesh::MeshIterator: operator*() cant be called on end!"); return m_At(m_Mesh, m_Indecies.at(m_Current)); }
			T operator*() { return get(); }
			const T operator*() const { return get(); }

			void reset() { m_Current = (bool)m_Indecies.size() - (int64_t)1; }
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

			HalfedgeHandle prev();
			HalfedgeHandle next();
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
			T& propValue(VertexIndex vertexIndex);
		};

		template<typename T>
		struct HPropertyHandle : public BasePropertyHandle<T> {
		protected:
			HPropertyHandle() = delete;
			HPropertyHandle(Mesh& mesh, PropertyIndex propertyIndex) : BasePropertyHandle<T>(mesh, propertyIndex) { }
			~HPropertyHandle() = default;
		public:
			T& propValue(HalfedgeIndex halfedgeIndex);
		};

		template<typename T>
		struct FPropertyHandle : public BasePropertyHandle<T> {
		protected:
			FPropertyHandle() = delete;
			FPropertyHandle(Mesh& mesh, PropertyIndex propertyIndex) : BasePropertyHandle<T>(mesh, propertyIndex) { }
			~FPropertyHandle() = default;
		public:
			T& propValue(FaceIndex faceIndex);
		};

	public:
		Mesh();
		Mesh(const Mesh&) = delete;
		~Mesh() = default;

		void clear();

		// Meshdata
		bool hasNormals() const;
		bool hasColors() const;
		bool hasTextureCoordinates() const;
		bool hasMappedNormals() const;
		bool hasMappedColors() const;
		bool hasMappedTextureCoordinates() const;

		bool validVertexIndex(VertexIndex vertexIndex) const;
		bool validHalfedgeIndex(HalfedgeIndex halfedgeIndex) const;
		bool validFaceIndex(FaceIndex faceIndex) const;

		size_t vertexCount() const;
		size_t halfedgeCount() const;
		size_t faceHalfedgeCount() const;
		size_t faceCount() const;

		bool positionExists(Position position) const;
		bool normalExists(Normal normal) const;
		bool colorExists(Color color) const;
		bool textureCoordinateExists(TextureCoordinate textureCoordinate) const;
		
		PositionIndex addPosition(const Position& position);
		Position& position(VertexIndex vertexIndex);
		PositionIterator positions();

		NormalIndex addNormal(const Normal& normal);
		NormalIndex& normalIndex(HalfedgeIndex halfedgeIndex);
		Normal& normal(HalfedgeIndex halfedgeIndex);
		Normal& normal(VertexIndex vertexIndex, FaceIndex faceIndex);
		NormalIterator normals();

		ColorIndex addColor(const Color& color);
		ColorIndex& colorIndex(HalfedgeIndex halfedgeIndex);
		Color& color(HalfedgeIndex halfedgeIndex);
		Color& color(VertexIndex vertexIndex, FaceIndex faceIndex);
		ColorIterator colors();

		TextureCoordinateIndex addTextureCoordinate(const TextureCoordinate& textureCoordinate);
		TextureCoordinateIndex& textureCoordinateIndex(HalfedgeIndex halfedgeIndex);
		TextureCoordinate& textureCoordinate(HalfedgeIndex halfedgeIndex);
		TextureCoordinate& textureCoordinate(VertexIndex vertexIndex, FaceIndex faceIndex);
		TextureCoordinateIterator textureCoordinates();

		
		// Mesh Topology Vertex
		VertexIndex addVertex(const Position& position);
		bool vertexHasHalfedge(VertexIndex vertexIndex) const;
		bool vertexHasFace(VertexIndex vertexIndex) const;
		VertexIndex vertexIndex(const Position& position) const;
		VertexHandle vertex(VertexIndex vertexIndex);
		VertexIterator vertexOneRing(VertexIndex vertexIndex);
		HalfedgeIterator vertexHalfedges(VertexIndex v, bool outgoing = true);
		VertexIterator vertecies();
		FaceIterator vertexFaces(VertexIndex vertexIndex);

		// Mesh Topology Halfedge
		HalfedgeIndex addHalfedge(VertexIndex from, VertexIndex to);
		bool halfedgeHasFace(HalfedgeIndex index) const;
		bool halfedgeIsBoundary(HalfedgeIndex index) const;
		HalfedgeIndex halfedgeIndex(VertexIndex vertexFrom, VertexIndex vertexTo) const;
		HalfedgeIndex halfedgeOppositeIndex(HalfedgeIndex index) const;
		FaceIndex halfedgeFaceIndex(HalfedgeIndex halfedgeIndex) const;
		HalfedgeHandle halfedge(HalfedgeIndex halfedgeIndex);
		HalfedgeIterator halfedges();
		HalfedgeIterator halfedgeIncoming(HalfedgeIndex halfedgeIndex, bool includeOpposite = false);
		HalfedgeIterator halfedgeOutgoing(HalfedgeIndex halfedgeIndex, bool includeOpposite = false);
		HalfedgeIndex halfedgePrevHalfedgeIndex(HalfedgeIndex halfedgeIndex, bool includeOpposite = false);
		HalfedgeIndex halfedgeNextHalfedgeIndex(HalfedgeIndex halfedgeIndex, bool includeOpposite = false);

		// Mesh Topology Face
		FaceIndex addFace(HalfedgeIndex he0, HalfedgeIndex he1, HalfedgeIndex he2);
		FaceIndex addFace(const std::vector<HalfedgeIndex>& halfedges);
		bool faceHalfedgesExist(HalfedgeHandle he0, HalfedgeHandle he1, HalfedgeHandle he2) const;
		HalfedgeIndex faceHalfedgeIndex(FaceIndex faceIndex, VertexIndex vertexIndex) const;
		HalfedgeIndex faceNextHalfedgeIndex(HalfedgeIndex halfedgeIndex);
		HalfedgeIndex facePrevHalfedgeIndex(HalfedgeIndex halfedgeIndex);
		FaceHandle face(FaceIndex faceIndex);
		VertexIterator faceVertecies(FaceIndex faceIndex);
		HalfedgeIterator faceHalfedges(FaceIndex faceIndex, bool cw = true);
		FaceIterator faces();
		FaceIterator faceNeighbors(FaceIndex faceIndex);

		
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
		std::vector<Normal> m_Normals;
		std::vector<Color> m_Colors;
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
		using Halfedge = std::pair<VertexIndex, VertexIndex>;
		using Face = std::vector<Halfedge>;

		std::vector<Halfedge> m_Halfedges;
		std::map<HalfedgeIndex, FaceIndex> m_Faces;
		std::map<HalfedgeIndex, NormalIndex> m_MappedNormals;
		std::map<HalfedgeIndex, ColorIndex> m_MappedColors;
		std::map<HalfedgeIndex, TextureCoordinateIndex> m_MappedTextureCoordinates;
	};
}