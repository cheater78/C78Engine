#include "C78EPCH.h"
#include "HEMesh.h"

namespace C78E {

    Ref<HEMesh> HEMesh::createMesh() { return createRef<HEMesh>(); }

    // HEMesh VertexHandle
    bool HEMesh::VertexHandle::valid() const { return BaseHandle::valid() && m_Mesh.validVertexIndex(m_Index); }
    HEMesh::Position& HEMesh::VertexHandle::position() { return m_Mesh.position(m_Index); }
    HEMesh::HalfedgeHandle HEMesh::VertexHandle::halfedge() { return *m_Mesh.halfedges(); }
	HEMesh::VertexIterator HEMesh::VertexHandle::vertecies() { return m_Mesh.vertexOneRing(m_Index); }
	HEMesh::HalfedgeIterator HEMesh::VertexHandle::halfedges() { return m_Mesh.vertexHalfedges(m_Index);  }
	HEMesh::FaceIterator HEMesh::VertexHandle::faces() { return m_Mesh.vertexFaces(m_Index); }
    template<typename T>
    T& HEMesh::VPropertyHandle<T>::propValue(VertexIndex vertexIndex) { return this->m_Mesh.property(*this, vertexIndex); }

    // HEMesh HalfedgeHandle
    bool HEMesh::HalfedgeHandle::valid() const { return BaseHandle::valid() && m_Mesh.validHalfedgeIndex(m_Index); }
    HEMesh::VertexHandle HEMesh::HalfedgeHandle::from() { return m_Mesh.halfedgeFrom(m_Index); }
    HEMesh::VertexHandle HEMesh::HalfedgeHandle::to() { return m_Mesh.halfedgeTo(m_Index); }
    HEMesh::HalfedgeHandle HEMesh::HalfedgeHandle::prev() {
        if (m_Mesh.halfedgeHasFace(m_Index))
            return HalfedgeHandle(m_Mesh, m_Mesh.facePrevHalfedgeIndex(m_Index));
        else return HalfedgeHandle(m_Mesh, m_Mesh.halfedgePrevHalfedgeIndex(m_Index));
    }
    HEMesh::HalfedgeHandle HEMesh::HalfedgeHandle::next() {
        if (m_Mesh.halfedgeHasFace(m_Index))
            return HalfedgeHandle(m_Mesh, m_Mesh.faceNextHalfedgeIndex(m_Index));
        else return HalfedgeHandle(m_Mesh, m_Mesh.halfedgeNextHalfedgeIndex(m_Index));
    }
    HEMesh::HalfedgeHandle HEMesh::HalfedgeHandle::opp() { return HalfedgeHandle(m_Mesh, m_Mesh.halfedgeOppositeIndex(m_Index)); }
    HEMesh::FaceHandle HEMesh::HalfedgeHandle::face() {
        if (m_Mesh.halfedgeHasFace(m_Index))
            return FaceHandle(m_Mesh, m_Mesh.halfedgeFaceIndex(m_Index));
        else return FaceHandle(m_Mesh, -1);
    }
    template<typename T>
    T& HEMesh::HPropertyHandle<T>::propValue(HalfedgeIndex index) { return this->m_Mesh.property(*this, index); }

    // HEMesh FaceHandle
    bool HEMesh::FaceHandle::valid() const { return BaseHandle::valid() && m_Mesh.validFaceIndex(m_Index); }
    HEMesh::VertexHandle HEMesh::FaceHandle::vertex() { return *vertecies(); }
    HEMesh::HalfedgeHandle HEMesh::FaceHandle::halfedge() { return *halfedges(); }
    HEMesh::VertexIterator HEMesh::FaceHandle::vertecies() { return m_Mesh.faceVertecies(m_Index); }
	HEMesh::HalfedgeIterator HEMesh::FaceHandle::halfedges() { return m_Mesh.faceHalfedges(m_Index); }
    HEMesh::FaceIterator HEMesh::FaceHandle::faces() { return m_Mesh.faceNeighbors(m_Index); }
    template<typename T>
    T& HEMesh::FPropertyHandle<T>::propValue(FaceIndex faceIndex) { return this->m_Mesh.property(*this, faceIndex); }

    /**
    * HEMesh
    */
    HEMesh::HEMesh() { }
    
    void HEMesh::clear() {
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

    //HEMesh Data
    bool HEMesh::hasNormals() const { return !m_Normals.empty(); }
    bool HEMesh::hasColors() const { return !m_Colors.empty(); }
    bool HEMesh::hasTextureCoordinates() const { return !m_TextureCoordinates.empty(); }

    bool HEMesh::hasMappedNormals() const { return hasNormals() && !m_MappedNormals.empty(); };
    bool HEMesh::hasMappedColors() const { return hasColors() && !m_MappedColors.empty(); };
    bool HEMesh::hasMappedTextureCoordinates() const { return hasTextureCoordinates() && !m_MappedTextureCoordinates.empty(); }

    bool HEMesh::validVertexIndex(VertexIndex vertexIndex) const { return vertexIndex < m_Positions.size(); }
    bool HEMesh::validHalfedgeIndex(HalfedgeIndex halfedgeIndex) const { return halfedgeIndex < m_Halfedges.size(); }
    bool HEMesh::validFaceIndex(FaceIndex faceIndex) const { return faceIndex < m_Faces.size(); }

    size_t HEMesh::vertexCount() const { return m_Positions.size(); }
    size_t HEMesh::halfedgeCount() const { return m_Halfedges.size(); }
    size_t HEMesh::faceHalfedgeCount() const { return m_Faces.size(); }
    size_t HEMesh::faceCount() const {
        std::set<FaceIndex> faces;
        for (const auto& [heIndex, fIndex] : m_Faces)
            faces.insert(fIndex);
        return faces.size();
    }

    bool HEMesh::positionExists(Position position) const {
        return std::find(m_Positions.begin(), m_Positions.end(), position) != m_Positions.end();
    }
    bool HEMesh::normalExists(Normal normal) const {
        return std::find(m_Normals.begin(), m_Normals.end(), normal) != m_Normals.end();
    }
    bool HEMesh::colorExists(Color color) const {
        return std::find(m_Colors.begin(), m_Colors.end(), color) != m_Colors.end();
    }
    bool HEMesh::textureCoordinateExists(TextureCoordinate textureCoordinate) const { 
        return std::find(m_TextureCoordinates.begin(), m_TextureCoordinates.end(), textureCoordinate) != m_TextureCoordinates.end();
    }

    HEMesh::PositionIndex HEMesh::addPosition(const Position& position) {
        VertexIndex vertexIndex = m_Positions.size();
        m_Positions.emplace_back(position);
        return vertexIndex;
    }

    //HEMesh Data Position
    HEMesh::Position& HEMesh::position(VertexIndex vertexIndex) {
        C78E_CORE_ASSERT(vertexIndex < m_Positions.size(), "HEMesh::position: VertexIndex out of Bounds!");
        return m_Positions.at(vertexIndex);
    }
    HEMesh::PositionIterator HEMesh::positions() {
        std::vector<VertexIndex> indecies = std::vector<VertexIndex>(m_Positions.size());
        for (VertexIndex i = 0; i < m_Positions.size(); i++)
            indecies[i] = i;
        return PositionIterator(*this, indecies);
    }

    HEMesh::NormalIndex HEMesh::addNormal(const Normal& normal) {
        NormalIndex normalIndex = m_Normals.size();
        m_Normals.emplace_back(normal);
        return normalIndex;
    }

    HEMesh::NormalIndex& HEMesh::normalIndex(HalfedgeIndex halfedgeIndex) { return m_MappedNormals[halfedgeIndex]; }

    //HEMesh Data Normal
    HEMesh::Normal& HEMesh::normal(HalfedgeIndex halfedgeIndex) {
        C78E_CORE_ASSERT(hasMappedNormals(), "HEMesh::normal: HEMesh has no (mapped) Normals!");
        auto normalIterator = m_MappedNormals.find(halfedgeIndex);
        C78E_CORE_ASSERT(normalIterator != m_MappedNormals.end(), "HEMesh::normal: VertexIndex does not map to a Normal!");
        C78E_CORE_ASSERT(normalIterator->second < m_Normals.size(), "HEMesh::normal: Mapped normal index is out of Bounds!");
        return m_Normals.at(normalIterator->second);
    }
    HEMesh::Normal& HEMesh::normal(VertexIndex vertexIndex, FaceIndex faceIndex) {
        return normal(halfedgeIndex(vertexIndex, faceIndex));
    }
    HEMesh::NormalIterator HEMesh::normals() {
        std::vector<NormalIndex> indecies = std::vector<NormalIndex>(m_Positions.size());
        for (NormalIndex i = 0; i < m_Normals.size(); i++)
            indecies[i] = i;
        return NormalIterator(*this, indecies);
    }

    HEMesh::ColorIndex HEMesh::addColor(const Color& color) {
        ColorIndex colorIndex = m_Colors.size();
        m_Colors.emplace_back(color);
        return colorIndex;
    }

    HEMesh::ColorIndex& HEMesh::colorIndex(HalfedgeIndex halfedgeIndex) { return m_MappedColors[halfedgeIndex]; }

    //HEMesh Data Color
    HEMesh::Color& HEMesh::color(HalfedgeIndex halfedgeIndex) {
        C78E_CORE_ASSERT(hasMappedColors(), "HEMesh::color: HEMesh has no Colors!");
        auto colorIterator = m_MappedColors.find(halfedgeIndex);
        C78E_CORE_ASSERT(colorIterator != m_MappedColors.end(), , "HEMesh::color: VertexIndex does not map to a Color!");
        C78E_CORE_ASSERT(colorIterator->second < m_Colors.size(), "HEMesh::color: Mapped color index is out of Bounds!");
        return m_Colors.at(colorIterator->second);
    }
    HEMesh::Color& HEMesh::color(VertexIndex vertexIndex, FaceIndex faceIndex) {
        return color(halfedgeIndex(vertexIndex, faceIndex));
    }
    HEMesh::ColorIterator HEMesh::colors() {
        std::vector<ColorIndex> indecies = std::vector<ColorIndex>(m_Positions.size());
        for (ColorIndex i = 0; i < m_Colors.size(); i++)
            indecies[i] = i;
        return ColorIterator(*this, indecies);
    }

    HEMesh::TextureCoordinateIndex HEMesh::addTextureCoordinate(const TextureCoordinate& textureCoordinate) {
        TextureCoordinateIndex textureCoordinateIndex = m_TextureCoordinates.size();
        m_TextureCoordinates.emplace_back(textureCoordinate);
        return textureCoordinateIndex;
    }

    HEMesh::TextureCoordinateIndex& HEMesh::textureCoordinateIndex(HalfedgeIndex halfedgeIndex) { return m_MappedTextureCoordinates[halfedgeIndex]; }

    //HEMesh Data TextureCoordinate
    HEMesh::TextureCoordinate& HEMesh::textureCoordinate(HalfedgeIndex halfedgeIndex) {
        C78E_CORE_ASSERT(hasTextureCoordinates(), "HEMesh::textureCoordinate: HEMesh has no TextureCoordinates!");
        auto textureCoordinateIterator = m_MappedTextureCoordinates.find(halfedgeIndex);
        C78E_CORE_ASSERT(textureCoordinateIterator != m_MappedTextureCoordinates.end(), "HEMesh::textureCoordinate: VertexIndex does not map to a TextureCoordinate!");
        C78E_CORE_ASSERT(textureCoordinateIterator->second < m_TextureCoordinates.size(), "HEMesh::textureCoordinate: Mapped TextureCoordinate index is out of Bounds!");
        return m_TextureCoordinates.at(textureCoordinateIterator->second);
    }
    HEMesh::TextureCoordinate& HEMesh::textureCoordinate(VertexIndex vertexIndex, FaceIndex faceIndex) {
        return textureCoordinate(halfedgeIndex(vertexIndex, faceIndex));
    }
    HEMesh::TextureCoordinateIterator HEMesh::textureCoordinates() {
        std::vector<TextureCoordinateIndex> indecies = std::vector<TextureCoordinateIndex>(m_Positions.size());
        for (TextureCoordinateIndex i = 0; i < m_TextureCoordinates.size(); i++)
            indecies[i] = i;
        return TextureCoordinateIterator(*this, indecies);
    }


    /**
    //HEMesh Topology Vertex
    */

    /**
    * Adds a Vertex
    */
    HEMesh::VertexIndex HEMesh::addVertex(const Position& position) { return addPosition(position); }

    bool HEMesh::vertexHasHalfedge(VertexIndex v) const {
        for (HalfedgeIndex i = 0; i < m_Halfedges.size(); i++)
            if (m_Halfedges[i].first == v || m_Halfedges[i].second == v)
                return true;
        return false;
    }

    bool HEMesh::vertexHasFace(VertexIndex v) const {
        for (HalfedgeIndex i = 0; i < m_Halfedges.size(); i++)
            if (m_Halfedges[i].first == v || m_Halfedges[i].second == v)
                if (m_Faces.contains(i)) return true;
        return false;
    }

    HEMesh::VertexIndex HEMesh::vertexIndex(const HEMesh::Position& position) const {
        auto it = std::find(m_Positions.begin(), m_Positions.end(), position);
        if (it != m_Positions.end()) return VertexIndex(it - m_Positions.begin());
        else return VertexIndex(-1);
    }

    HEMesh::VertexHandle HEMesh::vertex(VertexIndex vIndex) {
        C78E_CORE_VALIDATE(vIndex < m_Positions.size(), return VertexHandle(*this, (VertexIndex)-1); "HEMesh::vertex: vIndex out of Bounds!");
        return VertexHandle(*this, vIndex);
    }

    HEMesh::VertexIterator HEMesh::vertexOneRing(VertexIndex vertexIndex) {
        std::vector<VertexIndex> indecies = std::vector<VertexIndex>();
        for (auto it = vertexHalfedges(vertexIndex); it; it++)
            indecies.emplace_back((*it).to().idx());
        return VertexIterator(*this, indecies);
    }

    HEMesh::HalfedgeIterator HEMesh::vertexHalfedges(VertexIndex v, bool outgoing) {
        std::vector<HalfedgeIndex> indecies = std::vector<HalfedgeIndex>();
        for (HalfedgeIndex i = 0; i < m_Halfedges.size(); i++)
            if (m_Halfedges[i].first == v && outgoing || m_Halfedges[i].second == v && !outgoing)
                indecies[i] = i;
        return HalfedgeIterator(*this, indecies);
    }

    HEMesh::VertexIterator HEMesh::vertecies() {
        std::vector<VertexIndex> indecies = std::vector<VertexIndex>(vertexCount());
        for (VertexIndex i = 0; validVertexIndex(i); i++)
            indecies[i] = i;
        return VertexIterator(*this, indecies);
    }

    /**
    * iterator over all faces that share the provided Vertex
    */
    HEMesh::FaceIterator HEMesh::vertexFaces(VertexIndex vertexIndex) {
        std::vector<FaceIndex> indecies = std::vector<FaceIndex>();
        for (HalfedgeIndex i = 0; i < faceHalfedgeCount(); i++)
            if (m_Halfedges[i].first == vertexIndex)
                indecies[i] = i;
        return FaceIterator(*this, indecies);
    }
    
    /**
    //HEMesh Topology Halfedge
    */

    /**
    * Adds an Halfedge give its two VertexIndecies
    * returns HalfedgeIndex of then new Halfedge or the alr existing one
    */
    HEMesh::HalfedgeIndex HEMesh::addHalfedge(VertexIndex from, VertexIndex to) {
        HalfedgeIndex index = m_Halfedges.size();
        const Halfedge he = { from, to };
        const auto it = std::find(m_Halfedges.begin(), m_Halfedges.end(), he);
        C78E_CORE_VALIDATE(it == m_Halfedges.end(), return HalfedgeIndex(std::distance(it, m_Halfedges.begin()) / sizeof(Halfedge)), "HEMesh::addHalfedge: Halfedge arleady exists!");
        m_Halfedges.emplace_back(from, to);
        return index;
    }

    bool HEMesh::halfedgeHasFace(HalfedgeIndex index) const { return m_Faces.contains(index); }
    bool HEMesh::halfedgeIsBoundary(HalfedgeIndex index) const { return halfedgeHasFace(halfedgeOppositeIndex(index)) && !halfedgeHasFace(index); }

    HEMesh::HalfedgeIndex HEMesh::halfedgeIndex(VertexIndex vertexFrom, VertexIndex vertexTo) const {
        auto it = std::find(m_Halfedges.begin(), m_Halfedges.end(), Halfedge{ vertexFrom, vertexTo });
        if (it != m_Halfedges.end()) return HalfedgeIndex(it - m_Halfedges.begin());
        else return HalfedgeIndex(-1);
    }

    HEMesh::HalfedgeIndex HEMesh::halfedgeOppositeIndex(HalfedgeIndex index) const {
        Halfedge he = m_Halfedges[index];
        he = { he.second, he.first };
        auto it = std::find(m_Halfedges.begin(), m_Halfedges.end(), he);
        if (it != m_Halfedges.end())
            return (it - m_Halfedges.begin()) / sizeof(Halfedge);
        return -1;
    }

    HEMesh::FaceIndex HEMesh::halfedgeFaceIndex(HalfedgeIndex halfedgeIndex) const {
        if (m_Faces.contains(halfedgeIndex)) return m_Faces.at(halfedgeIndex);
        else return FaceIndex(-1);
    }

    HEMesh::HalfedgeHandle HEMesh::halfedge(HalfedgeIndex halfedgeIndex) {
        C78E_CORE_VALIDATE(halfedgeIndex < m_Halfedges.size(), return HalfedgeHandle(*this, (HalfedgeIndex)-1); "HEMesh::halfedge: hIndex out of Bounds!");
        return HalfedgeHandle(*this, halfedgeIndex);
    }

    HEMesh::VertexHandle HEMesh::halfedgeFrom(HalfedgeIndex halfedgeIndex) {
        C78E_CORE_VALIDATE(halfedgeIndex < m_Halfedges.size(), return VertexHandle(*this, (VertexIndex)-1); "HEMesh::halfedgeFrom: hIndex out of Bounds!");
        return VertexHandle(*this, m_Halfedges[halfedgeIndex].first);
    }

    HEMesh::VertexHandle HEMesh::halfedgeTo(HalfedgeIndex halfedgeIndex) {
        C78E_CORE_VALIDATE(halfedgeIndex < m_Halfedges.size(), return VertexHandle(*this, (VertexIndex)-1); "HEMesh::halfedgeTo: hIndex out of Bounds!");
        return VertexHandle(*this, m_Halfedges[halfedgeIndex].second);
    }

    HEMesh::HalfedgeIterator HEMesh::halfedges() {
        std::vector<HalfedgeIndex> indecies = std::vector<HalfedgeIndex>(m_Halfedges.size());
        for (HalfedgeIndex i = 0; i < m_Halfedges.size(); i++)
            indecies[i] = i;
        return HalfedgeIterator(*this, indecies);
    }

    HEMesh::HalfedgeIterator HEMesh::halfedgeIncoming(HalfedgeIndex halfedgeIndex, bool includeOpposite) {
        C78E_CORE_VALIDATE(validHalfedgeIndex(halfedgeIndex), return HalfedgeIterator(*this, { }), "HEMesh::halfedgeIncoming: halfedgeIndex invalid!");

        std::vector<HalfedgeIndex> indecies = std::vector<HalfedgeIndex>();
        for (auto it = halfedges(); it; it++) // search for incomming Halfedges
            if ((*it).to() == m_Halfedges[halfedgeIndex].first && ((*it).from() != m_Halfedges[halfedgeIndex].second || includeOpposite)) // not allowing for the halfedges opposite
                indecies.emplace_back((*it).idx());
        return HalfedgeIterator(*this, indecies);
    }

    HEMesh::HalfedgeIterator HEMesh::halfedgeOutgoing(HalfedgeIndex halfedgeIndex, bool includeOpposite) {
        C78E_CORE_VALIDATE(validHalfedgeIndex(halfedgeIndex), return HalfedgeIterator(*this, { }), "HEMesh::halfedgeOutgoing: halfedgeIndex invalid!");
        std::vector<HalfedgeIndex> indecies = std::vector<HalfedgeIndex>();
        for (auto it = halfedges(); it; it++) // search for outgoing Halfedges
            if ((*it).from() == m_Halfedges[halfedgeIndex].second && ((*it).to() != m_Halfedges[halfedgeIndex].first || includeOpposite)) // not/allowing for the halfedges opposite
                indecies.emplace_back((*it).idx());
        return HalfedgeIterator(*this, indecies);
    }

    HEMesh::HalfedgeIndex HEMesh::halfedgePrevHalfedgeIndex(HalfedgeIndex halfedgeIndex, bool includeOpposite) {
        HalfedgeIterator halfedgeIterator = halfedgeIncoming(halfedgeIndex, includeOpposite);
        if (halfedgeIterator) return (*halfedgeIterator).idx();
        else return HalfedgeIndex(-1);
    }

    HEMesh::HalfedgeIndex HEMesh::halfedgeNextHalfedgeIndex(HalfedgeIndex halfedgeIndex, bool includeOpposite) {
        HalfedgeIterator halfedgeIterator = halfedgeOutgoing(halfedgeIndex, includeOpposite);
        if (halfedgeIterator) return (*halfedgeIterator).idx();
        else return HalfedgeIndex(-1);
    }

    /**
    //HEMesh Topology Face
    */

    /**
    * Adds a Face give its three HalfedgeIndecies
    * returns FaceIndex of then new Face or the alr existing one
    */
    HEMesh::FaceIndex HEMesh::addFace(HalfedgeIndex he0, HalfedgeIndex he1, HalfedgeIndex he2) { return addFace({ he0, he1, he2 }); }
    HEMesh::FaceIndex HEMesh::addFace(const std::vector<HalfedgeIndex>& halfedges) {
        FaceIndex index = faceCount();
        for (HalfedgeIndex heIndex : halfedges) {
            C78E_CORE_VALIDATE(heIndex < m_Halfedges.size(), return FaceIndex(-1), "HEMesh::addFace: HalfedgeIndex({}) is out of Bounds({})!", heIndex, m_Halfedges.size());
            C78E_CORE_VALIDATE(!m_Faces.contains(heIndex), return FaceIndex(m_Faces.at(heIndex)), "HEMesh::addFace: HalfedgeIndex({}) is out of Bounds({})!", heIndex, m_Halfedges.size());
        }
        for (HalfedgeIndex heIndex : halfedges)
            m_Faces[heIndex] = index;
        return FaceIndex(index);
    }

    bool HEMesh::faceHalfedgesExist(HalfedgeHandle he0, HalfedgeHandle he1, HalfedgeHandle he2) const {
        return validFaceIndex(halfedgeFaceIndex(he0.idx())) && halfedgeFaceIndex(he0.idx()) == halfedgeFaceIndex(he1.idx()) && halfedgeFaceIndex(he1.idx()) == halfedgeFaceIndex(he2.idx());
    }

    HEMesh::HalfedgeIndex HEMesh::faceHalfedgeIndex(FaceIndex faceIndex, VertexIndex vertexIndex) const {
        for (auto& [halfedgeIndex, face] : m_Faces)
            if (face == faceIndex && m_Halfedges[halfedgeIndex].first == vertexIndex)
                return halfedgeIndex;
        C78E_CORE_ERROR("HEMesh::halfedgeIndex: Vertex and Face do not share an incident Halfedge!");
        return HalfedgeIndex(-1);
    }

    HEMesh::HalfedgeIndex HEMesh::faceNextHalfedgeIndex(HalfedgeIndex halfedgeIndex) {
        C78E_CORE_VALIDATE(halfedgeHasFace(halfedgeIndex), return HalfedgeIndex(-1), "HEMesh::faceNextHalfedgeIndex: halfedgeIndex is not associated with a Face!");
        for (HalfedgeIterator it = faceHalfedges(halfedgeFaceIndex(halfedgeIndex)); it; it++)
            if ((*it).idx() == halfedgeIndex)
                if (!++it) return (*++it).idx(); // next Iterator reached end, restart at front 
                else return (*it).idx(); // next Iterator is fine
        C78E_CORE_ERROR("HEMesh::faceNextHalfedgeIndex: Halfedge has Face but wasn't found in its Faces Halfedges!");
        return HalfedgeIndex();
    }

    HEMesh::HalfedgeIndex HEMesh::facePrevHalfedgeIndex(HalfedgeIndex halfedgeIndex) {
        C78E_CORE_VALIDATE(halfedgeHasFace(halfedgeIndex), return HalfedgeIndex(-1), "HEMesh::facePrevHalfedgeIndex: halfedgeIndex is not associated with a Face!");
        for (HalfedgeIterator it = faceHalfedges(halfedgeFaceIndex(halfedgeIndex)); it; it++)
            if ((*it).idx() == halfedgeIndex)
                if (!--it) return (*--it).idx(); // prev Iterator reached front, restart at end 
                else return (*it).idx(); // prev Iterator is fine
        C78E_CORE_ERROR("HEMesh::facePrevHalfedgeIndex: Halfedge has Face but wasn't found in its Faces Halfedges!");
        return HalfedgeIndex();
    }

    HEMesh::FaceHandle HEMesh::face(FaceIndex faceIndex) {
        C78E_CORE_VALIDATE(validFaceIndex(faceIndex), return FaceHandle(*this, (FaceIndex)-1); "HEMesh::face: fIndex out of Bounds!");
        return FaceHandle(*this, faceIndex);
    }

    HEMesh::VertexIterator HEMesh::faceVertecies(FaceIndex faceIndex) {
        std::vector<VertexIndex> indecies = std::vector<VertexIndex>();
        for (auto it = faceHalfedges(faceIndex); it; it++)
            indecies.emplace_back((*it).to().idx());
        return VertexIterator(*this, indecies);
    }

    HEMesh::HalfedgeIterator HEMesh::faceHalfedges(FaceIndex faceIndex, bool cw) {
        std::vector<HalfedgeIndex> indecies = std::vector<HalfedgeIndex>(m_Halfedges.size());
        for (HalfedgeIndex i = 0; i < m_Halfedges.size(); i++)
            if(m_Faces[i] = faceIndex)
                indecies[i] = i;
        return HalfedgeIterator(*this, indecies);
    }

    /**
    * iterator over all faces of the mesh
    */
    HEMesh::FaceIterator HEMesh::faces() {
        std::vector<FaceIndex> indecies = std::vector<FaceIndex>(faceCount());
        for (FaceIndex i = 0; i < faceCount(); i++)
            indecies[i] = i;
        return FaceIterator(*this, indecies);
    }
    
    HEMesh::FaceIterator HEMesh::faceNeighbors(FaceIndex faceIndex) {
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
    HEMesh::VPropertyHandle<T> HEMesh::createVertexProperty(const T& initialValue) {
        size_t propIndex = m_Properties.size();
        m_Properties.emplace_back(sizeof(T), vertecies().count());
        if(initialValue != T())
            m_Properties.back().clear(&initialValue);
        return VPropertyHandle<T>(*this, propIndex);
    }

    template<typename T>
    HEMesh::HPropertyHandle<T> HEMesh::createHalfedgeProperty(const T& initialValue) {
        size_t propIndex = m_Properties.size();
        m_Properties.emplace_back(sizeof(T), halfedges().count());
        if(initialValue != T())
            m_Properties.back().clear(&initialValue);
        return HPropertyHandle<T>(*this, propIndex);
    }

    template<typename T>
    HEMesh::FPropertyHandle<T> HEMesh::createFaceProperty(const T& initialValue) {
        size_t propIndex = m_Properties.size();
        m_Properties.emplace_back(sizeof(T), faces().count());
        if(initialValue != T())
            m_Properties.back().clear(&initialValue);
        return FPropertyHandle<T>(*this, propIndex);
    }

    template<typename T>
    T& HEMesh::property(BasePropertyHandle<T> propertyHandle, Index index) {
        C78E_CORE_VALIDATE(propertyHandle.m_PropertyIndex < m_Properties.size(), return {}, "HEMesh::property: Property Index out of Bounds!");
        PropertyContainer& prop = m_Properties.at(propertyHandle.m_PropertyIndex);
        C78E_CORE_VALIDATE(prop.sizeOfElement() == sizeof(T), return {}, "HEMesh::property: Size of T and Property element do not match!");
        C78E_CORE_VALIDATE(index < prop.size(), return {}, "HEMesh::property: Index into property out of Bounds!");
        T* elementData = std::static_pointer_cast<T>(prop.at(index));
        C78E_CORE_VALIDATE(elementData, return {}, "HEMesh::property: Retrieved element ptr is null!");
        return *elementData;
    }




    /**
    * PropertyContainer
    * creates a custom Property container, given the size of a single element and optionally the initial Element count
    */
    HEMesh::PropertyContainer::PropertyContainer(size_t elementSize, size_t elementCount)
        : m_ElementSize(elementSize), m_ElementCount(elementCount), m_ElementData(nullptr) {
        if(elementCount)
            alloc();
    }

    /**
    * ~PropertyContainer
    * frees all allocated storage for that Property
    */
    HEMesh::PropertyContainer::~PropertyContainer() { free(); }
    
    /**
    * Clears all Memory for this Property to 0, or a clear value of the PropertyContainers m_ElementSize
    */
    void HEMesh::PropertyContainer::clear(void* clearValue) {
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
    void HEMesh::PropertyContainer::resize(size_t count) {
        if(count != m_ElementCount) {
            void* oldData = m_ElementData;
            size_t oldSize = m_ElementSize * m_ElementCount;
            m_ElementCount = count;
            alloc();
            std::memcpy(m_ElementData, oldData, oldSize);
            std::free(oldData);
        }
    }

    void* HEMesh::PropertyContainer::at(size_t index) {
        return (void*)((size_t)m_ElementData + (index * m_ElementSize));
    }

    /**
    * Allocares the nessecary memory
    */
    void HEMesh::PropertyContainer::alloc(){ m_ElementData = std::malloc(m_ElementSize * m_ElementCount); }
    /**
    * Frees all of this PropertyContainers memory
    */
    void HEMesh::PropertyContainer::free(){ std::free(m_ElementData); }

}
