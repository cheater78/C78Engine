#include "C78EPCH.h"
#include "Mesh.h"

namespace C78E {

    bool Mesh::VertexHandle::valid() const { return BaseHandle::valid() && m_Index < m_Mesh.positions().count(); }
    Mesh::Position& Mesh::VertexHandle::position() { return m_Mesh.position(m_Index); }
    Mesh::HalfedgeHandle Mesh::VertexHandle::halfedge() { return *m_Mesh.m_Topology.halfedges(); }

    /**
    * Provides an Iterator over the Vertex' one-ring
    */
	Mesh::VertexIterator Mesh::VertexHandle::vertecies() {
        return m_Mesh.vertecies(m_Index);
    }

	Mesh::HalfedgeIterator Mesh::VertexHandle::halfedges() { return m_Mesh.m_Topology.halfedges(m_Index);  }
	Mesh::FaceIterator Mesh::VertexHandle::faces() { return m_Mesh.m_Topology.faces(m_Index); }



    bool Mesh::HalfedgeHandle::valid() const { return BaseHandle::valid() && m_Index < m_Mesh.halfedges().count(); }

    Mesh::VertexHandle Mesh::HalfedgeHandle::from() {
        C78E_CORE_VALIDATE(m_Index < m_Mesh.m_Topology.halfedges().count(), return VertexHandle(m_Mesh, -1), "Mesh::HalfedgeHandle::from: Index out of Bounds!");
        return VertexHandle(m_Mesh, m_Mesh.m_Topology.halfedge(m_Index).first);
    }

    Mesh::VertexHandle Mesh::HalfedgeHandle::to() {
        C78E_CORE_VALIDATE(m_Index < m_Mesh.m_Topology.halfedges().count(), return VertexHandle(m_Mesh, -1), "Mesh::HalfedgeHandle::to: Index out of Bounds!");
        return VertexHandle(m_Mesh, m_Mesh.m_Topology.halfedge(m_Index).second);
    }

    Mesh::HalfedgeHandle Mesh::HalfedgeHandle::next() {
        if (m_Mesh.m_Topology.halfedgeHasFace(m_Index)) {
            FaceIndex f = m_Mesh.m_Topology.halfedgeFaceIndex(m_Index);
            switch (m_Index % 3) {
            case 0: return HalfedgeHandle(m_Mesh, m_Index + 1);
            case 1: return HalfedgeHandle(m_Mesh, m_Index + 1);
            case 2: return HalfedgeHandle(m_Mesh, m_Index - 2);
            default: break;
            }
        }

        for (auto it = m_Mesh.m_Topology.halfedges(); it; it++)
            if ((*it).from() == to())
                return *it;
        return HalfedgeHandle(m_Mesh, -1);
    }

    Mesh::HalfedgeHandle Mesh::HalfedgeHandle::prev() {
        if (m_Mesh.m_Topology.halfedgeHasFace(m_Index)) {
            FaceIndex f = m_Mesh.m_Topology.halfedgeFaceIndex(m_Index);
            switch (m_Index % 3) {
            case 0: return HalfedgeHandle(m_Mesh, m_Index + 2);
            case 1: return HalfedgeHandle(m_Mesh, m_Index - 1);
            case 2: return HalfedgeHandle(m_Mesh, m_Index - 1);
            default: break;
            }
        }

        for (auto it = m_Mesh.m_Topology.halfedges(); it; it++)
            if ((*it).to() == from())
                return *it;
        return HalfedgeHandle(m_Mesh, -1);
    }

    Mesh::HalfedgeHandle Mesh::HalfedgeHandle::opp() { return HalfedgeHandle(m_Mesh, m_Mesh.halfedgeOpposite(m_Index)); }

    Mesh::FaceHandle Mesh::HalfedgeHandle::face() {
        if (m_Mesh.m_Topology.halfedgeHasFace(m_Index))
            return FaceHandle(m_Mesh, m_Mesh.m_Topology.halfedgeFaceIndex(m_Index));
        else
            return FaceHandle(m_Mesh, -1);
    }



    bool Mesh::FaceHandle::valid() const { return BaseHandle::valid() && m_Index < m_Mesh.faces().count(); }
    Mesh::VertexHandle Mesh::FaceHandle::vertex() { return VertexHandle(m_Mesh, m_Index * 3); }

    Mesh::HalfedgeHandle Mesh::FaceHandle::halfedge() {
        C78E_CORE_VALIDATE(m_Index  < m_Mesh.m_Topology.faces().count(), return HalfedgeHandle(m_Mesh, -1), "Mesh::FaceHandle::halfedge: Index out of Bounds!");
        return HalfedgeHandle(m_Mesh, m_Index * 3);
    }

    Mesh::VertexIterator Mesh::FaceHandle::vertecies() {
        std::vector<VertexIndex> indecies = std::vector<VertexIndex>();
        for (HalfedgeIterator it = m_Mesh.halfedges(m_Index); it; it++)
            indecies.emplace_back((*it).from().idx());
        return VertexIterator(m_Mesh, indecies);
    }

	Mesh::HalfedgeIterator Mesh::FaceHandle::halfedges() {
        return m_Mesh.m_Topology.faceHalfedges(m_Index);
    }

	Mesh::FaceIterator Mesh::FaceHandle::faces() {
        std::vector<FaceIndex> indecies = std::vector<FaceIndex>(halfedges().count());
        for (auto it = halfedges(); it; it++)
            indecies.emplace_back((*it).opp().face());
        return FaceIterator(m_Mesh, indecies);
    }

    
    
    template<typename T>
    T& Mesh::VPropertyHandle<T>::propValue(VertexIndex index) {
        return this->m_Mesh.property(*this, index);
    }

    template<typename T>
    T& Mesh::HPropertyHandle<T>::propValue(HalfedgeIndex index) {
        return this->m_Mesh.property(*this, index);
    }

    template<typename T>
    T& Mesh::FPropertyHandle<T>::propValue(FaceIndex index) {
        return this->m_Mesh.property(*this, index);
    }



    Mesh::Mesh() : m_Topology(*this) { }

    Mesh::VertexHandle Mesh::addVertex(Position position, Normal normal, Color color, TextureCoordinate textureCoordinate) {
        VertexIndex vIndex = m_Positions.size();
        m_Positions.emplace_back(position);

        if(normal != Normal{} || hasNormals()) {
            auto it = std::find(m_Normals.begin(), m_Normals.end(), normal);
            NormalIndex nIndex = it - m_Normals.begin();
            if(it == m_Normals.end())
                m_Normals.emplace_back(normal);
            m_NormalMap[vIndex] = nIndex;
        }

        if(color != Color{} || hasColor()) {
            auto it = std::find(m_Colors.begin(), m_Colors.end(), color);
            NormalIndex cIndex = it - m_Colors.begin();
            if(it == m_Colors.end())
                m_Colors.emplace_back(color);
            m_ColorMap[vIndex] = cIndex;
        }

        if(textureCoordinate != TextureCoordinate{} || hasTextureCoordinates()) {
            auto it = std::find(m_TextureCoordinates.begin(), m_TextureCoordinates.end(), textureCoordinate);
            NormalIndex tIndex = it - m_TextureCoordinates.begin();
            if(it == m_TextureCoordinates.end())
                m_TextureCoordinates.emplace_back(textureCoordinate);
            m_TextureCoordinateMap[vIndex] = tIndex;
        }

        return VertexHandle{ *this, vIndex};
    }

	Mesh::HalfedgeHandle Mesh::addHalfedge(VertexHandle from, VertexHandle to) {
        HalfedgeIndex index = m_Topology.addHalfedge(from, to);
        return HalfedgeHandle{*this, index};
    }

	Mesh::FaceHandle Mesh::addFace(VertexHandle vh0, VertexHandle vh1, VertexHandle vh2) {
        FaceIndex index = m_Topology.addFace(vh0, vh1, vh2);
        return FaceHandle{ *this, index };
    }



    Mesh::VertexHandle Mesh::vertex(VertexIndex vIndex) {
        C78E_CORE_VALIDATE(vIndex < m_Positions.size(), return VertexHandle(*this, (VertexIndex)-1); "Mesh::vertex: vIndex out of Bounds!");
        return VertexHandle(*this, vIndex);
    }

	Mesh::HalfedgeHandle Mesh::halfedge(HalfedgeIndex hIndex) {
        C78E_CORE_VALIDATE(hIndex < m_Topology.halfedges().count(), return HalfedgeHandle(*this, (HalfedgeIndex)-1); "Mesh::halfedge: hIndex out of Bounds!");
        return HalfedgeHandle(*this, hIndex);
    }

	Mesh::FaceHandle Mesh::face(FaceIndex fIndex) {
        C78E_CORE_VALIDATE(fIndex < m_Topology.faces().count(), return FaceHandle(*this, (FaceIndex)-1); "Mesh::face: fIndex out of Bounds!");
        return FaceHandle(*this, fIndex);
    }



    Mesh::Position& Mesh::position(VertexIndex vIndex) {
        C78E_CORE_ASSERT(vIndex < m_Positions.size(), "Mesh::position: VertexIndex out of Bounds!");
        return m_Positions.at(vIndex);
    }

    Mesh::PositionIterator Mesh::positions() {
        std::vector<VertexIndex> indecies = std::vector<VertexIndex>(m_Positions.size());
        for (VertexIndex i = 0; i < m_Positions.size(); i++)
            indecies[i] = i;
        return PositionIterator(*this, indecies);
    }

    bool Mesh::hasNormals() const { return m_Normals.size() && m_NormalMap.size(); }

    Mesh::Normal& Mesh::normal(VertexIndex vIndex) {
        C78E_CORE_ASSERT(hasNormals(), "Mesh::normal: Mesh has no Normals!");
        auto nIt = m_NormalMap.find(vIndex);
        C78E_CORE_ASSERT(nIt != m_NormalMap.end(), "Mesh::normal: VertexIndex does not map to a Normal!");
        C78E_CORE_ASSERT(nIt->second < m_Normals.size(), "Mesh::normal: Mapped normal index is out of Bounds!");
        return m_Normals.at(nIt->second);
    }

    Mesh::NormalIterator Mesh::normals() {
        std::vector<NormalIndex> indecies = std::vector<NormalIndex>(m_Positions.size());
        for (NormalIndex i = 0; i < m_Normals.size(); i++)
            indecies[i] = i;
        return NormalIterator(*this, indecies);
    }

    bool Mesh::hasColor() const { return m_Colors.size() && m_ColorMap.size(); }

    Mesh::Color& Mesh::color(VertexIndex vIndex) {
        C78E_CORE_ASSERT(hasColor(), "Mesh::color: Mesh has no Colors!");
        auto cIt = m_ColorMap.find(vIndex);
        C78E_CORE_ASSERT(cIt != m_ColorMap.end(),, "Mesh::color: VertexIndex does not map to a Color!");
        C78E_CORE_ASSERT(cIt->second < m_Colors.size(), "Mesh::color: Mapped color index is out of Bounds!");
        return m_Colors.at(cIt->second);
    }

    Mesh::ColorIterator Mesh::colors() {
        std::vector<ColorIndex> indecies = std::vector<ColorIndex>(m_Positions.size());
        for (ColorIndex i = 0; i < m_Colors.size(); i++)
            indecies[i] = i;
        return ColorIterator(*this, indecies);
    }

    bool Mesh::hasTextureCoordinates() const { return m_TextureCoordinates.size() && m_TextureCoordinateMap.size(); }

    Mesh::TextureCoordinate& Mesh::textureCoordinate(VertexIndex vIndex) {
        C78E_CORE_ASSERT(hasTextureCoordinates(), "Mesh::textureCoordinate: Mesh has no TextureCoordinates!");
        auto tIt = m_TextureCoordinateMap.find(vIndex);
        C78E_CORE_ASSERT(tIt != m_TextureCoordinateMap.end(), "Mesh::textureCoordinate: VertexIndex does not map to a TextureCoordinate!");
        C78E_CORE_ASSERT(tIt->second < m_TextureCoordinates.size(), "Mesh::textureCoordinate: Mapped TextureCoordinate index is out of Bounds!");
        return m_TextureCoordinates.at(tIt->second);
    }

    Mesh::TextureCoordinateIterator Mesh::textureCoordinates() {
        std::vector<TextureCoordinateIndex> indecies = std::vector<TextureCoordinateIndex>(m_Positions.size());
        for (TextureCoordinateIndex i = 0; i < m_TextureCoordinates.size(); i++)
            indecies[i] = i;
        return TextureCoordinateIterator(*this, indecies);
    }



    bool Mesh::positionExists(Position position) const {
        auto it = std::find(m_Positions.begin(), m_Positions.end(), position);
        return it != m_Positions.end();
    }

    bool Mesh::normalExists(Normal normal) const {
        auto it = std::find(m_Normals.begin(), m_Normals.end(), normal);
        return it != m_Normals.end();
    }

    bool Mesh::colorExists(Color color) const {
        auto it = std::find(m_Colors.begin(), m_Colors.end(), color);
        return it != m_Colors.end();
    }

    bool Mesh::textureCoordinateExists(TextureCoordinate textureCoordinate) const {
        auto it = std::find(m_TextureCoordinates.begin(), m_TextureCoordinates.end(), textureCoordinate);
        return it != m_TextureCoordinates.end();
    }



    bool Mesh::halfedgeVerteciesExist(VertexHandle from, VertexHandle to) const {
        for (auto it = m_Topology.halfedges(); it; it++)
            if ((*it).from().idx() == from && (*it).to().idx() == to)
                return true;
        return false;
    }

    bool Mesh::faceVerteciesExist(VertexHandle vh0, VertexHandle vh1, VertexHandle vh2) const {
        return m_Topology.faces(vh0).intersectionIter(m_Topology.faces(vh1)).intersectionIter(m_Topology.faces(vh2)).count();
    }

    bool Mesh::faceHalfedgesExist(HalfedgeHandle he0, HalfedgeHandle he1, HalfedgeHandle he2) const {
        return m_Topology.halfedgeHasFace(he0.idx()) && 
            m_Topology.halfedgeHasFace(he1.idx()) && 
            m_Topology.halfedgeHasFace(he2.idx()) && 
            m_Topology.halfedgeFaceIndex(he0.idx()) == m_Topology.halfedgeFaceIndex(he1.idx()) &&
            m_Topology.halfedgeFaceIndex(he2.idx()) == m_Topology.halfedgeFaceIndex(he1.idx());
    }

    bool Mesh::vertexHasHalfedge(VertexIndex v) const { return m_Topology.vertexHasHalfedge(v); }
    bool Mesh::vertexHasFace(VertexIndex v) const { return m_Topology.vertexHasFace(v); }

    Mesh::VertexIterator Mesh::vertecies() {
        std::vector<VertexIndex> indecies = std::vector<VertexIndex>(m_Positions.size());
        for (VertexIndex i = 0; i < m_Positions.size(); i++)
            indecies[i] = i;
        return VertexIterator(*this, indecies);
    }

    Mesh::VertexIterator Mesh::vertecies(VertexIndex v) {
        std::vector<VertexIndex> indecies = std::vector<VertexIndex>();
        for (auto it = m_Topology.halfedges(v); it; it++)
            indecies.emplace_back((*it).to().idx());
        return VertexIterator(*this, indecies);
    }

    bool Mesh::halfedgeHasFace(HalfedgeIndex index) const { return m_Topology.halfedgeHasFace(index); }
    Mesh::FaceHandle Mesh::halfedgeFace(HalfedgeIndex he) { return FaceHandle(*this, m_Topology.halfedgeFaceIndex(he)); }
    Mesh::HalfedgeHandle Mesh::halfedgeOpposite(HalfedgeIndex he) { return HalfedgeHandle(*this, m_Topology.halfedgeOpposite(he)); }
    Mesh::HalfedgeIterator Mesh::halfedges() { return m_Topology.halfedges(); }
    const Mesh::HalfedgeIterator Mesh::halfedges() const { return m_Topology.halfedges(); }
    Mesh::HalfedgeIterator Mesh::halfedges(VertexIndex v, bool outgoing) { return m_Topology.halfedges(v, outgoing); }
    Mesh::HalfedgeIterator Mesh::faceHalfedges(FaceIndex f, bool cw) { return m_Topology.faceHalfedges(f, cw); }
    Mesh::FaceIterator Mesh::faces() { return m_Topology.faces(); }
    const Mesh::FaceIterator Mesh::faces() const { return m_Topology.faces(); }
    Mesh::FaceIterator Mesh::faces(VertexIndex v) { return m_Topology.faces(v); }
    const Mesh::FaceIterator Mesh::faces(VertexIndex v) const { return m_Topology.faces(v); }



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



    Mesh::PropertyContainer::PropertyContainer(size_t elementSize, size_t elementCount)
        : m_ElementSize(elementSize), m_ElementCount(elementCount), m_ElementData(nullptr) {
        if(elementCount)
            alloc();
    }

    Mesh::PropertyContainer::~PropertyContainer() {
        free();
    }
    
    void Mesh::PropertyContainer::clear(void* clearValue) {
        for(size_t off = 0; off < m_ElementCount * m_ElementSize; off++) {
            unsigned char* elem = static_cast<unsigned char*>(m_ElementData) + off;
            if(clearValue)
                *elem = *(static_cast<unsigned char*>(clearValue) + (off % m_ElementSize));
            else
                *elem = (char)0;
        }
    }

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

    void Mesh::PropertyContainer::alloc(){
        m_ElementData = std::malloc(m_ElementSize * m_ElementCount);
    }

    void Mesh::PropertyContainer::free(){
        std::free(m_ElementData);
    }




    Mesh::Topology::Topology(Mesh& mesh) : m_Mesh(mesh) { }

    /**
    * Adds an Halfedge give its two VertexIndecies
    * returns HalfedgeIndex of then new Halfedge or the alr existing one
    */
    Mesh::HalfedgeIndex Mesh::Topology::addHalfedge(VertexIndex from, VertexIndex to) {
        HalfedgeIndex index = m_Halfedges.size();
        const Halfedge he = { from, to };
        const auto it = std::find(m_Halfedges.begin(), m_Halfedges.end(), he);
        C78E_CORE_VALIDATE(it == m_Halfedges.end(), return HalfedgeIndex(std::distance(it, m_Halfedges.begin()) / sizeof(Halfedge)), "Mesh::Topology::addHalfedge: Halfedge arleady exists!");
        m_Halfedges.emplace_back(from, to);
        return index;
    }

    /**
    * Adds a Face give its three HalfedgeIndecies
    * returns FaceIndex of then new Face or the alr existing one
    */
    Mesh::FaceIndex Mesh::Topology::addFace(HalfedgeIndex he0, HalfedgeIndex he1, HalfedgeIndex he2) {
        C78E_CORE_VALIDATE(he0 < m_Halfedges.size(), return FaceIndex(-1), "Mesh::Topology::addFace: HalfedgeIndex({}) is out of Bounds({})!", he0, m_Halfedges.size());
        C78E_CORE_VALIDATE(he1 < m_Halfedges.size(), return FaceIndex(-1), "Mesh::Topology::addFace: HalfedgeIndex({}) is out of Bounds({})!", he1, m_Halfedges.size());
        C78E_CORE_VALIDATE(he2 < m_Halfedges.size(), return FaceIndex(-1), "Mesh::Topology::addFace: HalfedgeIndex({}) is out of Bounds({})!", he2, m_Halfedges.size());
        FaceIndex he0Face = he0 / 3;
        FaceIndex he1Face = he1 / 3;
        FaceIndex he2Face = he2 / 3;
        C78E_CORE_VALIDATE(!(he0Face == he1Face && he1Face == he2Face && he0Face < m_FaceCount * 3), return FaceIndex(he0Face), "Mesh::Topology::addFace: Face already exists!");
        C78E_CORE_VALIDATE(he0Face >= m_FaceCount * 3, return FaceIndex(he0Face), "Mesh::Topology::addFace: Halfedge({}) is already part of Face({})!", he0, he0Face);
        C78E_CORE_VALIDATE(he1Face >= m_FaceCount * 3, return FaceIndex(he1Face), "Mesh::Topology::addFace: Halfedge({}) is already part of Face({})!", he1, he1Face);
        C78E_CORE_VALIDATE(he2Face >= m_FaceCount * 3, return FaceIndex(he2Face), "Mesh::Topology::addFace: Halfedge({}) is already part of Face({})!", he2, he2Face);
        
        FaceIndex index = m_FaceCount;

        Halfedge evacHE1 = m_Halfedges[index * 3 + 1];
        Halfedge evacHE2 = m_Halfedges[index * 3 + 2];
        Halfedge evacHE0 = m_Halfedges[index * 3 + 0];

        Halfedge faceHE0 = m_Halfedges[he0];
        Halfedge faceHE1 = m_Halfedges[he1];
        Halfedge faceHE2 = m_Halfedges[he2];

        m_Halfedges[index * 3 + 0] = faceHE0;
        m_Halfedges[index * 3 + 2] = faceHE1;
        m_Halfedges[index * 3 + 1] = faceHE2;

        m_Halfedges[he0] = evacHE1;
        m_Halfedges[he2] = evacHE2;
        m_Halfedges[he1] = evacHE0;

        m_FaceCount++;

        return FaceIndex(index);
    }

    bool Mesh::Topology::vertexHasHalfedge(VertexIndex v) const {
        for (HalfedgeIndex i = 0; i < m_Halfedges.size(); i++)
            if (m_Halfedges[i].first == v || m_Halfedges[i].second == v)
                return true;
        return false;
    }

    bool Mesh::Topology::vertexHasFace(VertexIndex v) const {
        for (HalfedgeIndex i = 0; i < m_Halfedges.size(); i++)
            if (m_Halfedges[i].first == v || m_Halfedges[i].second == v)
                return i < m_FaceCount * 3;
        return false;
    }

    

    const Mesh::Topology::Halfedge& Mesh::Topology::halfedge(HalfedgeIndex index) const {  return m_Halfedges[index]; }
    bool Mesh::Topology::halfedgeHasFace(HalfedgeIndex index) const { return index < m_FaceCount * 3; }

    bool Mesh::Topology::halfedgeIsBoundary(HalfedgeIndex index) const { return halfedgeHasFace(halfedgeOpposite(index)) && !halfedgeHasFace(index); }
    
    Mesh::HalfedgeIndex Mesh::Topology::halfedgeOpposite(HalfedgeIndex index) const {
        Topology::Halfedge he = m_Mesh.m_Topology.halfedge(index);
        he = { he.second, he.first };
        auto it = std::find(m_Halfedges.begin(), m_Halfedges.end(), he);
        if (it != m_Halfedges.end())
            return (it - m_Halfedges.begin()) / sizeof(Topology::Halfedge);
        return -1;
    }
    const Mesh::Topology::Face& Mesh::Topology::face(FaceIndex index) const { return *((Face *) &m_Halfedges[index * 3]); }

    Mesh::HalfedgeIterator Mesh::Topology::faceHalfedges(FaceIndex f, bool cw) {
        if(cw) return HalfedgeIterator(m_Mesh, { 3 * f + 0, 3 * f + 1, 3 * f + 2 });
        else return HalfedgeIterator(m_Mesh, { 3 * f + 2, 3 * f + 1, 3 * f + 0 });
    }

    Mesh::HalfedgeIterator Mesh::Topology::halfedges() {
        std::vector<HalfedgeIndex> indecies = std::vector<HalfedgeIndex>(m_Halfedges.size());
        for (HalfedgeIndex i = 0; i < m_Halfedges.size(); i++)
            indecies[i] = i;
        return HalfedgeIterator(m_Mesh, indecies);
    }

    const Mesh::HalfedgeIterator Mesh::Topology::halfedges() const {
        std::vector<HalfedgeIndex> indecies = std::vector<HalfedgeIndex>(m_Halfedges.size());
        for (HalfedgeIndex i = 0; i < m_Halfedges.size(); i++)
            indecies[i] = i;
        return HalfedgeIterator(m_Mesh, indecies);
    }

    Mesh::HalfedgeIterator Mesh::Topology::halfedges(VertexIndex v, bool outgoing) {
        std::vector<HalfedgeIndex> indecies = std::vector<HalfedgeIndex>();
        for (HalfedgeIndex i = 0; i < m_Halfedges.size(); i++)
            if (m_Halfedges[i].first == v && outgoing || m_Halfedges[i].second == v && !outgoing)
                indecies[i] = i;
        return HalfedgeIterator(m_Mesh, indecies);
    }


    Mesh::FaceIndex Mesh::Topology::halfedgeFaceIndex(HalfedgeIndex he) const {
        C78E_CORE_VALIDATE(he < m_FaceCount / 3, return FaceIndex(-1), " Mesh::Topology::face: HalfedgeIndex out of Face bounds!");
        return FaceIndex(he / 3);
    }

    /**
    * iterator over all faces of the mesh
    */
    Mesh::FaceIterator Mesh::Topology::faces() {
        std::vector<FaceIndex> indecies = std::vector<FaceIndex>(m_FaceCount);
        for (FaceIndex i = 0; i < m_FaceCount; i++)
            indecies[i] = i;
        return FaceIterator(m_Mesh, indecies);
    }

    const Mesh::FaceIterator Mesh::Topology::faces() const {
        std::vector<FaceIndex> indecies = std::vector<FaceIndex>(m_FaceCount);
        for (FaceIndex i = 0; i < m_FaceCount; i++)
            indecies[i] = i;
        return FaceIterator(m_Mesh, indecies);
    }

    /**
    * iterator over all faces that share the provided Vertex
    */
    Mesh::FaceIterator Mesh::Topology::faces(VertexIndex v) {
        std::vector<FaceIndex> indecies = std::vector<FaceIndex>();
        for (HalfedgeIndex i = 0; i < faceHalfedgeCount(); i++)
            if (m_Halfedges[i].first == v)
                indecies[i] = i;
        return FaceIterator(m_Mesh, indecies);
    }

    const Mesh::FaceIterator Mesh::Topology::faces(VertexIndex v) const {
        std::vector<FaceIndex> indecies = std::vector<FaceIndex>();
        for (HalfedgeIndex i = 0; i < faceHalfedgeCount(); i++)
            if (m_Halfedges[i].first == v)
                indecies[i] = i;
        return FaceIterator(m_Mesh, indecies);
    }
}