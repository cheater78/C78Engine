#include "C78EPCH.h"
#include "Mesh.h"

namespace C78E {

    Ref<Mesh> Mesh::createMesh() { return Ref<Mesh>(); }

    // Mesh VertexHandle
    bool Mesh::VertexHandle::valid() const { return BaseHandle::valid() && m_Mesh.validVertexIndex(m_Index); }
    Mesh::Position& Mesh::VertexHandle::position() { return m_Mesh.position(m_Index); }
    Mesh::HalfedgeHandle Mesh::VertexHandle::halfedge() { return *m_Mesh.halfedges(); }
	Mesh::VertexIterator Mesh::VertexHandle::vertecies() { return m_Mesh.vertexOneRing(m_Index); }
	Mesh::HalfedgeIterator Mesh::VertexHandle::halfedges() { return m_Mesh.vertexHalfedges(m_Index);  }
	Mesh::FaceIterator Mesh::VertexHandle::faces() { return m_Mesh.vertexFaces(m_Index); }
    template<typename T>
    T& Mesh::VPropertyHandle<T>::propValue(VertexIndex vertexIndex) { return this->m_Mesh.property(*this, vertexIndex); }

    // Mesh HalfedgeHandle
    bool Mesh::HalfedgeHandle::valid() const { return BaseHandle::valid() && m_Mesh.validHalfedgeIndex(m_Index); }
    Mesh::VertexHandle Mesh::HalfedgeHandle::from() { return m_Mesh.halfedgeFrom(m_Index); }
    Mesh::VertexHandle Mesh::HalfedgeHandle::to() { return m_Mesh.halfedgeTo(m_Index); }
    Mesh::HalfedgeHandle Mesh::HalfedgeHandle::prev() {
        if (m_Mesh.halfedgeHasFace(m_Index))
            return HalfedgeHandle(m_Mesh, m_Mesh.facePrevHalfedgeIndex(m_Index));
        else return HalfedgeHandle(m_Mesh, m_Mesh.halfedgePrevHalfedgeIndex(m_Index));
    }
    Mesh::HalfedgeHandle Mesh::HalfedgeHandle::next() {
        if (m_Mesh.halfedgeHasFace(m_Index))
            return HalfedgeHandle(m_Mesh, m_Mesh.faceNextHalfedgeIndex(m_Index));
        else return HalfedgeHandle(m_Mesh, m_Mesh.halfedgeNextHalfedgeIndex(m_Index));
    }
    Mesh::HalfedgeHandle Mesh::HalfedgeHandle::opp() { return HalfedgeHandle(m_Mesh, m_Mesh.halfedgeOppositeIndex(m_Index)); }
    Mesh::FaceHandle Mesh::HalfedgeHandle::face() {
        if (m_Mesh.halfedgeHasFace(m_Index))
            return FaceHandle(m_Mesh, m_Mesh.halfedgeFaceIndex(m_Index));
        else return FaceHandle(m_Mesh, -1);
    }
    template<typename T>
    T& Mesh::HPropertyHandle<T>::propValue(HalfedgeIndex index) { return this->m_Mesh.property(*this, index); }

    // Mesh FaceHandle
    bool Mesh::FaceHandle::valid() const { return BaseHandle::valid() && m_Mesh.validFaceIndex(m_Index); }
    Mesh::VertexHandle Mesh::FaceHandle::vertex() { return *vertecies(); }
    Mesh::HalfedgeHandle Mesh::FaceHandle::halfedge() { return *halfedges(); }
    Mesh::VertexIterator Mesh::FaceHandle::vertecies() { return m_Mesh.faceVertecies(m_Index); }
	Mesh::HalfedgeIterator Mesh::FaceHandle::halfedges() { return m_Mesh.faceHalfedges(m_Index); }
    Mesh::FaceIterator Mesh::FaceHandle::faces() { return m_Mesh.faceNeighbors(m_Index); }
    template<typename T>
    T& Mesh::FPropertyHandle<T>::propValue(FaceIndex faceIndex) { return this->m_Mesh.property(*this, faceIndex); }

    /**
    * Mesh
    */
    Mesh::Mesh() { }
    
    void Mesh::clear() {
        m_Positions.clear();
        m_Normals.clear();
        m_Colors.clear();
        m_TextureCoordinates.clear();
        m_Properties.clear();
        m_Halfedges.clear(); 
        m_Faces.clear();
        m_MappedNormals.clear();
        m_MappedColors.clear();
        m_MappedTextureCoordinates.clear();
    }

    //Mesh Data
    bool Mesh::hasNormals() const { return !m_Normals.empty(); }
    bool Mesh::hasColors() const { return !m_Colors.empty(); }
    bool Mesh::hasTextureCoordinates() const { return !m_TextureCoordinates.empty(); }

    bool Mesh::hasMappedNormals() const { return hasNormals() && !m_MappedNormals.empty(); };
    bool Mesh::hasMappedColors() const { return hasColors() && !m_MappedColors.empty(); };
    bool Mesh::hasMappedTextureCoordinates() const { return hasTextureCoordinates() && !m_MappedTextureCoordinates.empty(); }

    bool Mesh::validVertexIndex(VertexIndex vertexIndex) const { return vertexIndex < m_Positions.size(); }
    bool Mesh::validHalfedgeIndex(HalfedgeIndex halfedgeIndex) const { return halfedgeIndex < m_Halfedges.size(); }
    bool Mesh::validFaceIndex(FaceIndex faceIndex) const { return faceIndex < m_Faces.size(); }

    size_t Mesh::vertexCount() const { return m_Positions.size(); }
    size_t Mesh::halfedgeCount() const { return m_Halfedges.size(); }
    size_t Mesh::faceHalfedgeCount() const { return m_Faces.size(); }
    size_t Mesh::faceCount() const {
        std::set<FaceIndex> faces;
        for (const auto& [heIndex, fIndex] : m_Faces)
            faces.insert(fIndex);
        return faces.size();
    }

    bool Mesh::positionExists(Position position) const {
        return std::find(m_Positions.begin(), m_Positions.end(), position) != m_Positions.end();
    }
    bool Mesh::normalExists(Normal normal) const {
        return std::find(m_Normals.begin(), m_Normals.end(), normal) != m_Normals.end();
    }
    bool Mesh::colorExists(Color color) const {
        return std::find(m_Colors.begin(), m_Colors.end(), color) != m_Colors.end();
    }
    bool Mesh::textureCoordinateExists(TextureCoordinate textureCoordinate) const { 
        return std::find(m_TextureCoordinates.begin(), m_TextureCoordinates.end(), textureCoordinate) != m_TextureCoordinates.end();
    }

    Mesh::PositionIndex Mesh::addPosition(const Position& position) {
        VertexIndex vertexIndex = m_Positions.size();
        m_Positions.emplace_back(position);
        return vertexIndex;
    }

    //Mesh Data Position
    Mesh::Position& Mesh::position(VertexIndex vertexIndex) {
        C78E_CORE_ASSERT(vertexIndex < m_Positions.size(), "Mesh::position: VertexIndex out of Bounds!");
        return m_Positions.at(vertexIndex);
    }
    Mesh::PositionIterator Mesh::positions() {
        std::vector<VertexIndex> indecies = std::vector<VertexIndex>(m_Positions.size());
        for (VertexIndex i = 0; i < m_Positions.size(); i++)
            indecies[i] = i;
        return PositionIterator(*this, indecies);
    }

    Mesh::NormalIndex Mesh::addNormal(const Normal& normal) {
        NormalIndex normalIndex = m_Normals.size();
        m_Normals.emplace_back(normal);
        return normalIndex;
    }

    Mesh::NormalIndex& Mesh::normalIndex(HalfedgeIndex halfedgeIndex) { return m_MappedNormals[halfedgeIndex]; }

    //Mesh Data Normal
    Mesh::Normal& Mesh::normal(HalfedgeIndex halfedgeIndex) {
        C78E_CORE_ASSERT(hasMappedNormals(), "Mesh::normal: Mesh has no (mapped) Normals!");
        auto normalIterator = m_MappedNormals.find(halfedgeIndex);
        C78E_CORE_ASSERT(normalIterator != m_MappedNormals.end(), "Mesh::normal: VertexIndex does not map to a Normal!");
        C78E_CORE_ASSERT(normalIterator->second < m_Normals.size(), "Mesh::normal: Mapped normal index is out of Bounds!");
        return m_Normals.at(normalIterator->second);
    }
    Mesh::Normal& Mesh::normal(VertexIndex vertexIndex, FaceIndex faceIndex) {
        return normal(halfedgeIndex(vertexIndex, faceIndex));
    }
    Mesh::NormalIterator Mesh::normals() {
        std::vector<NormalIndex> indecies = std::vector<NormalIndex>(m_Positions.size());
        for (NormalIndex i = 0; i < m_Normals.size(); i++)
            indecies[i] = i;
        return NormalIterator(*this, indecies);
    }

    Mesh::ColorIndex Mesh::addColor(const Color& color) {
        ColorIndex colorIndex = m_Colors.size();
        m_Colors.emplace_back(color);
        return colorIndex;
    }

    Mesh::ColorIndex& Mesh::colorIndex(HalfedgeIndex halfedgeIndex) { return m_MappedColors[halfedgeIndex]; }

    //Mesh Data Color
    Mesh::Color& Mesh::color(HalfedgeIndex halfedgeIndex) {
        C78E_CORE_ASSERT(hasMappedColors(), "Mesh::color: Mesh has no Colors!");
        auto colorIterator = m_MappedColors.find(halfedgeIndex);
        C78E_CORE_ASSERT(colorIterator != m_MappedColors.end(), , "Mesh::color: VertexIndex does not map to a Color!");
        C78E_CORE_ASSERT(colorIterator->second < m_Colors.size(), "Mesh::color: Mapped color index is out of Bounds!");
        return m_Colors.at(colorIterator->second);
    }
    Mesh::Color& Mesh::color(VertexIndex vertexIndex, FaceIndex faceIndex) {
        return color(halfedgeIndex(vertexIndex, faceIndex));
    }
    Mesh::ColorIterator Mesh::colors() {
        std::vector<ColorIndex> indecies = std::vector<ColorIndex>(m_Positions.size());
        for (ColorIndex i = 0; i < m_Colors.size(); i++)
            indecies[i] = i;
        return ColorIterator(*this, indecies);
    }

    Mesh::TextureCoordinateIndex Mesh::addTextureCoordinate(const TextureCoordinate& textureCoordinate) {
        TextureCoordinateIndex textureCoordinateIndex = m_TextureCoordinates.size();
        m_TextureCoordinates.emplace_back(textureCoordinate);
        return textureCoordinateIndex;
    }

    Mesh::TextureCoordinateIndex& Mesh::textureCoordinateIndex(HalfedgeIndex halfedgeIndex) { return m_MappedTextureCoordinates[halfedgeIndex]; }

    //Mesh Data TextureCoordinate
    Mesh::TextureCoordinate& Mesh::textureCoordinate(HalfedgeIndex halfedgeIndex) {
        C78E_CORE_ASSERT(hasTextureCoordinates(), "Mesh::textureCoordinate: Mesh has no TextureCoordinates!");
        auto textureCoordinateIterator = m_MappedTextureCoordinates.find(halfedgeIndex);
        C78E_CORE_ASSERT(textureCoordinateIterator != m_MappedTextureCoordinates.end(), "Mesh::textureCoordinate: VertexIndex does not map to a TextureCoordinate!");
        C78E_CORE_ASSERT(textureCoordinateIterator->second < m_TextureCoordinates.size(), "Mesh::textureCoordinate: Mapped TextureCoordinate index is out of Bounds!");
        return m_TextureCoordinates.at(textureCoordinateIterator->second);
    }
    Mesh::TextureCoordinate& Mesh::textureCoordinate(VertexIndex vertexIndex, FaceIndex faceIndex) {
        return textureCoordinate(halfedgeIndex(vertexIndex, faceIndex));
    }
    Mesh::TextureCoordinateIterator Mesh::textureCoordinates() {
        std::vector<TextureCoordinateIndex> indecies = std::vector<TextureCoordinateIndex>(m_Positions.size());
        for (TextureCoordinateIndex i = 0; i < m_TextureCoordinates.size(); i++)
            indecies[i] = i;
        return TextureCoordinateIterator(*this, indecies);
    }


    /**
    //Mesh Topology Vertex
    */

    /**
    * Adds a Vertex
    */
    Mesh::VertexIndex Mesh::addVertex(const Position& position) { return addPosition(position); }

    bool Mesh::vertexHasHalfedge(VertexIndex v) const {
        for (HalfedgeIndex i = 0; i < m_Halfedges.size(); i++)
            if (m_Halfedges[i].first == v || m_Halfedges[i].second == v)
                return true;
        return false;
    }

    bool Mesh::vertexHasFace(VertexIndex v) const {
        for (HalfedgeIndex i = 0; i < m_Halfedges.size(); i++)
            if (m_Halfedges[i].first == v || m_Halfedges[i].second == v)
                if (m_Faces.contains(i)) return true;
        return false;
    }

    Mesh::VertexIndex Mesh::vertexIndex(const Mesh::Position& position) const {
        auto it = std::find(m_Positions.begin(), m_Positions.end(), position);
        if (it != m_Positions.end()) return VertexIndex(it - m_Positions.begin());
        else return VertexIndex(-1);
    }

    Mesh::VertexHandle Mesh::vertex(VertexIndex vIndex) {
        C78E_CORE_VALIDATE(vIndex < m_Positions.size(), return VertexHandle(*this, (VertexIndex)-1); "Mesh::vertex: vIndex out of Bounds!");
        return VertexHandle(*this, vIndex);
    }

    Mesh::VertexIterator Mesh::vertexOneRing(VertexIndex vertexIndex) {
        std::vector<VertexIndex> indecies = std::vector<VertexIndex>();
        for (auto it = vertexHalfedges(vertexIndex); it; it++)
            indecies.emplace_back((*it).to().idx());
        return VertexIterator(*this, indecies);
    }

    Mesh::HalfedgeIterator Mesh::vertexHalfedges(VertexIndex v, bool outgoing) {
        std::vector<HalfedgeIndex> indecies = std::vector<HalfedgeIndex>();
        for (HalfedgeIndex i = 0; i < m_Halfedges.size(); i++)
            if (m_Halfedges[i].first == v && outgoing || m_Halfedges[i].second == v && !outgoing)
                indecies[i] = i;
        return HalfedgeIterator(*this, indecies);
    }

    Mesh::VertexIterator Mesh::vertecies() {
        std::vector<VertexIndex> indecies = std::vector<VertexIndex>(vertexCount());
        for (VertexIndex i = 0; validVertexIndex(i); i++)
            indecies[i] = i;
        return VertexIterator(*this, indecies);
    }

    /**
    * iterator over all faces that share the provided Vertex
    */
    Mesh::FaceIterator Mesh::vertexFaces(VertexIndex vertexIndex) {
        std::vector<FaceIndex> indecies = std::vector<FaceIndex>();
        for (HalfedgeIndex i = 0; i < faceHalfedgeCount(); i++)
            if (m_Halfedges[i].first == vertexIndex)
                indecies[i] = i;
        return FaceIterator(*this, indecies);
    }
    
    /**
    //Mesh Topology Halfedge
    */

    /**
    * Adds an Halfedge give its two VertexIndecies
    * returns HalfedgeIndex of then new Halfedge or the alr existing one
    */
    Mesh::HalfedgeIndex Mesh::addHalfedge(VertexIndex from, VertexIndex to) {
        HalfedgeIndex index = m_Halfedges.size();
        const Halfedge he = { from, to };
        const auto it = std::find(m_Halfedges.begin(), m_Halfedges.end(), he);
        C78E_CORE_VALIDATE(it == m_Halfedges.end(), return HalfedgeIndex(std::distance(it, m_Halfedges.begin()) / sizeof(Halfedge)), "Mesh::addHalfedge: Halfedge arleady exists!");
        m_Halfedges.emplace_back(from, to);
        return index;
    }

    bool Mesh::halfedgeHasFace(HalfedgeIndex index) const { return m_Faces.contains(index); }
    bool Mesh::halfedgeIsBoundary(HalfedgeIndex index) const { return halfedgeHasFace(halfedgeOppositeIndex(index)) && !halfedgeHasFace(index); }

    Mesh::HalfedgeIndex Mesh::halfedgeIndex(VertexIndex vertexFrom, VertexIndex vertexTo) const {
        auto it = std::find(m_Halfedges.begin(), m_Halfedges.end(), Halfedge{ vertexFrom, vertexTo });
        if (it != m_Halfedges.end()) return HalfedgeIndex(it - m_Halfedges.begin());
        else return HalfedgeIndex(-1);
    }

    Mesh::HalfedgeIndex Mesh::halfedgeOppositeIndex(HalfedgeIndex index) const {
        Halfedge he = m_Halfedges[index];
        he = { he.second, he.first };
        auto it = std::find(m_Halfedges.begin(), m_Halfedges.end(), he);
        if (it != m_Halfedges.end())
            return (it - m_Halfedges.begin()) / sizeof(Halfedge);
        return -1;
    }

    Mesh::FaceIndex Mesh::halfedgeFaceIndex(HalfedgeIndex halfedgeIndex) const {
        if (m_Faces.contains(halfedgeIndex)) return m_Faces.at(halfedgeIndex);
        else return FaceIndex(-1);
    }

    Mesh::HalfedgeHandle Mesh::halfedge(HalfedgeIndex halfedgeIndex) {
        C78E_CORE_VALIDATE(halfedgeIndex < m_Halfedges.size(), return HalfedgeHandle(*this, (HalfedgeIndex)-1); "Mesh::halfedge: hIndex out of Bounds!");
        return HalfedgeHandle(*this, halfedgeIndex);
    }

    Mesh::VertexHandle Mesh::halfedgeFrom(HalfedgeIndex halfedgeIndex) {
        C78E_CORE_VALIDATE(halfedgeIndex < m_Halfedges.size(), return VertexHandle(*this, (VertexIndex)-1); "Mesh::halfedgeFrom: hIndex out of Bounds!");
        return VertexHandle(*this, m_Halfedges[halfedgeIndex].first);
    }

    Mesh::VertexHandle Mesh::halfedgeTo(HalfedgeIndex halfedgeIndex) {
        C78E_CORE_VALIDATE(halfedgeIndex < m_Halfedges.size(), return VertexHandle(*this, (VertexIndex)-1); "Mesh::halfedgeTo: hIndex out of Bounds!");
        return VertexHandle(*this, m_Halfedges[halfedgeIndex].second);
    }

    Mesh::HalfedgeIterator Mesh::halfedges() {
        std::vector<HalfedgeIndex> indecies = std::vector<HalfedgeIndex>(m_Halfedges.size());
        for (HalfedgeIndex i = 0; i < m_Halfedges.size(); i++)
            indecies[i] = i;
        return HalfedgeIterator(*this, indecies);
    }

    Mesh::HalfedgeIterator Mesh::halfedgeIncoming(HalfedgeIndex halfedgeIndex, bool includeOpposite) {
        C78E_CORE_VALIDATE(validHalfedgeIndex(halfedgeIndex), return HalfedgeIterator(*this, { }), "Mesh::halfedgeIncoming: halfedgeIndex invalid!");

        std::vector<HalfedgeIndex> indecies = std::vector<HalfedgeIndex>();
        for (auto it = halfedges(); it; it++) // search for incomming Halfedges
            if ((*it).to() == m_Halfedges[halfedgeIndex].first && ((*it).from() != m_Halfedges[halfedgeIndex].second || includeOpposite)) // not allowing for the halfedges opposite
                indecies.emplace_back((*it).idx());
        return HalfedgeIterator(*this, indecies);
    }

    Mesh::HalfedgeIterator Mesh::halfedgeOutgoing(HalfedgeIndex halfedgeIndex, bool includeOpposite) {
        C78E_CORE_VALIDATE(validHalfedgeIndex(halfedgeIndex), return HalfedgeIterator(*this, { }), "Mesh::halfedgeOutgoing: halfedgeIndex invalid!");
        std::vector<HalfedgeIndex> indecies = std::vector<HalfedgeIndex>();
        for (auto it = halfedges(); it; it++) // search for outgoing Halfedges
            if ((*it).from() == m_Halfedges[halfedgeIndex].second && ((*it).to() != m_Halfedges[halfedgeIndex].first || includeOpposite)) // not/allowing for the halfedges opposite
                indecies.emplace_back((*it).idx());
        return HalfedgeIterator(*this, indecies);
    }

    Mesh::HalfedgeIndex Mesh::halfedgePrevHalfedgeIndex(HalfedgeIndex halfedgeIndex, bool includeOpposite) {
        HalfedgeIterator halfedgeIterator = halfedgeIncoming(halfedgeIndex, includeOpposite);
        if (halfedgeIterator) return (*halfedgeIterator).idx();
        else return HalfedgeIndex(-1);
    }

    Mesh::HalfedgeIndex Mesh::halfedgeNextHalfedgeIndex(HalfedgeIndex halfedgeIndex, bool includeOpposite) {
        HalfedgeIterator halfedgeIterator = halfedgeOutgoing(halfedgeIndex, includeOpposite);
        if (halfedgeIterator) return (*halfedgeIterator).idx();
        else return HalfedgeIndex(-1);
    }

    /**
    //Mesh Topology Face
    */

    /**
    * Adds a Face give its three HalfedgeIndecies
    * returns FaceIndex of then new Face or the alr existing one
    */
    Mesh::FaceIndex Mesh::addFace(HalfedgeIndex he0, HalfedgeIndex he1, HalfedgeIndex he2) { return addFace({ he0, he1, he2 }); }
    Mesh::FaceIndex Mesh::addFace(const std::vector<HalfedgeIndex>& halfedges) {
        FaceIndex index = faceCount();
        for (HalfedgeIndex heIndex : halfedges) {
            C78E_CORE_VALIDATE(heIndex < m_Halfedges.size(), return FaceIndex(-1), "Mesh::addFace: HalfedgeIndex({}) is out of Bounds({})!", heIndex, m_Halfedges.size());
            C78E_CORE_VALIDATE(!m_Faces.contains(heIndex), return FaceIndex(m_Faces.at(heIndex)), "Mesh::addFace: HalfedgeIndex({}) is out of Bounds({})!", heIndex, m_Halfedges.size());
        }
        for (HalfedgeIndex heIndex : halfedges)
            m_Faces[heIndex] = index;
        return FaceIndex(index);
    }

    bool Mesh::faceHalfedgesExist(HalfedgeHandle he0, HalfedgeHandle he1, HalfedgeHandle he2) const {
        return validFaceIndex(halfedgeFaceIndex(he0.idx())) && halfedgeFaceIndex(he0.idx()) == halfedgeFaceIndex(he1.idx()) && halfedgeFaceIndex(he1.idx()) == halfedgeFaceIndex(he2.idx());
    }

    Mesh::HalfedgeIndex Mesh::faceHalfedgeIndex(FaceIndex faceIndex, VertexIndex vertexIndex) const {
        for (auto& [halfedgeIndex, face] : m_Faces)
            if (face == faceIndex && m_Halfedges[halfedgeIndex].first == vertexIndex)
                return halfedgeIndex;
        C78E_CORE_ERROR("Mesh::halfedgeIndex: Vertex and Face do not share an incident Halfedge!");
        return HalfedgeIndex(-1);
    }

    Mesh::HalfedgeIndex Mesh::faceNextHalfedgeIndex(HalfedgeIndex halfedgeIndex) {
        C78E_CORE_VALIDATE(halfedgeHasFace(halfedgeIndex), return HalfedgeIndex(-1), "Mesh::faceNextHalfedgeIndex: halfedgeIndex is not associated with a Face!");
        for (HalfedgeIterator it = faceHalfedges(halfedgeFaceIndex(halfedgeIndex)); it; it++)
            if ((*it).idx() == halfedgeIndex)
                if (!++it) return (*++it).idx(); // next Iterator reached end, restart at front 
                else return (*it).idx(); // next Iterator is fine
        C78E_CORE_ERROR("Mesh::faceNextHalfedgeIndex: Halfedge has Face but wasn't found in its Faces Halfedges!");
        return HalfedgeIndex();
    }

    Mesh::HalfedgeIndex Mesh::facePrevHalfedgeIndex(HalfedgeIndex halfedgeIndex) {
        C78E_CORE_VALIDATE(halfedgeHasFace(halfedgeIndex), return HalfedgeIndex(-1), "Mesh::facePrevHalfedgeIndex: halfedgeIndex is not associated with a Face!");
        for (HalfedgeIterator it = faceHalfedges(halfedgeFaceIndex(halfedgeIndex)); it; it++)
            if ((*it).idx() == halfedgeIndex)
                if (!--it) return (*--it).idx(); // prev Iterator reached front, restart at end 
                else return (*it).idx(); // prev Iterator is fine
        C78E_CORE_ERROR("Mesh::facePrevHalfedgeIndex: Halfedge has Face but wasn't found in its Faces Halfedges!");
        return HalfedgeIndex();
    }

    Mesh::FaceHandle Mesh::face(FaceIndex faceIndex) {
        C78E_CORE_VALIDATE(validFaceIndex(faceIndex), return FaceHandle(*this, (FaceIndex)-1); "Mesh::face: fIndex out of Bounds!");
        return FaceHandle(*this, faceIndex);
    }

    Mesh::VertexIterator Mesh::faceVertecies(FaceIndex faceIndex) {
        std::vector<VertexIndex> indecies = std::vector<VertexIndex>();
        for (auto it = faceHalfedges(faceIndex); it; it++)
            indecies.emplace_back((*it).to().idx());
        return VertexIterator(*this, indecies);
    }

    Mesh::HalfedgeIterator Mesh::faceHalfedges(FaceIndex faceIndex, bool cw) {
        std::vector<HalfedgeIndex> indecies = std::vector<HalfedgeIndex>(m_Halfedges.size());
        for (HalfedgeIndex i = 0; i < m_Halfedges.size(); i++)
            if(m_Faces[i] = faceIndex)
                indecies[i] = i;
        return HalfedgeIterator(*this, indecies);
    }

    /**
    * iterator over all faces of the mesh
    */
    Mesh::FaceIterator Mesh::faces() {
        std::vector<FaceIndex> indecies = std::vector<FaceIndex>(faceCount());
        for (FaceIndex i = 0; i < faceCount(); i++)
            indecies[i] = i;
        return FaceIterator(*this, indecies);
    }
    
    Mesh::FaceIterator Mesh::faceNeighbors(FaceIndex faceIndex) {
        std::vector<FaceIndex> indecies = std::vector<FaceIndex>();
        for (HalfedgeIterator halfedgeIterator = faceHalfedges(faceIndex); halfedgeIterator; halfedgeIterator++) {
            HalfedgeIndex neigborHalfedge = halfedgeOppositeIndex((*halfedgeIterator).idx());
            if (!halfedgeIsBoundary(neigborHalfedge))
                indecies.emplace_back(halfedgeFaceIndex(neigborHalfedge));
        }
        return FaceIterator(*this, indecies);
    }

    // Custom Meshdata
    template<typename T>
    Mesh::VPropertyHandle<T> Mesh::createVertexProperty(const T& initialValue) {
        size_t propIndex = m_Properties.size();
        m_Properties.emplace_back(sizeof(T), vertecies().count());
        if(initialValue != T())
            m_Properties.back().clear(&initialValue);
        return VPropertyHandle<T>(*this, propIndex);
    }

    template<typename T>
    Mesh::HPropertyHandle<T> Mesh::createHalfedgeProperty(const T& initialValue) {
        size_t propIndex = m_Properties.size();
        m_Properties.emplace_back(sizeof(T), halfedges().count());
        if(initialValue != T())
            m_Properties.back().clear(&initialValue);
        return HPropertyHandle<T>(*this, propIndex);
    }

    template<typename T>
    Mesh::FPropertyHandle<T> Mesh::createFaceProperty(const T& initialValue) {
        size_t propIndex = m_Properties.size();
        m_Properties.emplace_back(sizeof(T), faces().count());
        if(initialValue != T())
            m_Properties.back().clear(&initialValue);
        return FPropertyHandle<T>(*this, propIndex);
    }

    template<typename T>
    T& Mesh::property(BasePropertyHandle<T> propertyHandle, Index index) {
        C78E_CORE_VALIDATE(propertyHandle.m_PropertyIndex < m_Properties.size(), return {}, "Mesh::property: Property Index out of Bounds!");
        PropertyContainer& prop = m_Properties.at(propertyHandle.m_PropertyIndex);
        C78E_CORE_VALIDATE(prop.sizeOfElement() == sizeof(T), return {}, "Mesh::property: Size of T and Property element do not match!");
        C78E_CORE_VALIDATE(index < prop.size(), return {}, "Mesh::property: Index into property out of Bounds!");
        T* elementData = std::static_pointer_cast<T>(prop.at(index));
        C78E_CORE_VALIDATE(elementData, return {}, "Mesh::property: Retrieved element ptr is null!");
        return *elementData;
    }




    /**
    * PropertyContainer
    * creates a custom Property container, given the size of a single element and optionally the initial Element count
    */
    Mesh::PropertyContainer::PropertyContainer(size_t elementSize, size_t elementCount)
        : m_ElementSize(elementSize), m_ElementCount(elementCount), m_ElementData(nullptr) {
        if(elementCount)
            alloc();
    }

    /**
    * ~PropertyContainer
    * frees all allocated storage for that Property
    */
    Mesh::PropertyContainer::~PropertyContainer() { free(); }
    
    /**
    * Clears all Memory for this Property to 0, or a clear value of the PropertyContainers m_ElementSize
    */
    void Mesh::PropertyContainer::clear(void* clearValue) {
        for(size_t off = 0; off < m_ElementCount * m_ElementSize; off++) {
            unsigned char* elem = static_cast<unsigned char*>(m_ElementData) + off;
            if(clearValue)
                *elem = *(static_cast<unsigned char*>(clearValue) + (off % m_ElementSize));
            else
                *elem = (char)0;
        }
    }

    /**
    * Resizes the MemoryBuffer 
    */
    void Mesh::PropertyContainer::resize(size_t count) {
        if(count != m_ElementCount) {
            void* oldData = m_ElementData;
            size_t oldSize = m_ElementSize * m_ElementCount;
            m_ElementCount = count;
            alloc();
            std::memcpy(m_ElementData, oldData, oldSize);
            std::free(oldData);
        }
    }

    void* Mesh::PropertyContainer::at(size_t index) {
        return (void*)((size_t)m_ElementData + (index * m_ElementSize));
    }

    /**
    * Allocares the nessecary memory
    */
    void Mesh::PropertyContainer::alloc(){ m_ElementData = std::malloc(m_ElementSize * m_ElementCount); }
    /**
    * Frees all of this PropertyContainers memory
    */
    void Mesh::PropertyContainer::free(){ std::free(m_ElementData); }

}