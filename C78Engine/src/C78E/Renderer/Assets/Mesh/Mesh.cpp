#include "C78EPCH.h"
#include "Mesh.h"

namespace C78E {

    Mesh::Position& Mesh::VertexHandle::position() {
        return m_Mesh.position(m_Index);
    }

    Mesh::HalfedgeHandle Mesh::VertexHandle::halfedge() {
    
    }

	Mesh::VertexIterator Mesh::VertexHandle::vertecies() {

    }

	Mesh::HalfedgeIterator Mesh::VertexHandle::halfedges() {

    }
    
	Mesh::FaceIterator Mesh::VertexHandle::faces() {

    }



    Mesh::VertexHandle Mesh::HalfedgeHandle::from() {
        C78E_CORE_VALIDATE(m_Index < m_Mesh.m_Halfedges.size(), return VertexHandle(m_Mesh, 0), "Mesh::HalfedgeHandle::from: Index out of Bounds!");
        return VertexHandle(m_Mesh, m_Mesh.m_Halfedges.at(m_Index).from);
    }

    Mesh::VertexHandle Mesh::HalfedgeHandle::to() {
        C78E_CORE_VALIDATE(m_Index < m_Mesh.m_Halfedges.size(), return VertexHandle(m_Mesh, 0), "Mesh::HalfedgeHandle::to: Index out of Bounds!");
        return VertexHandle(m_Mesh, m_Mesh.m_Halfedges.at(m_Index).to);
    }

    Mesh::HalfedgeHandle Mesh::HalfedgeHandle::next() {
        
    }

    Mesh::HalfedgeHandle Mesh::HalfedgeHandle::prev() {
        
    }

    Mesh::HalfedgeHandle Mesh::HalfedgeHandle::opp() {
        
    }

    Mesh::FaceHandle Mesh::HalfedgeHandle::face() {

    }



    Mesh::HalfedgeHandle Mesh::FaceHandle::halfedge() {
        C78E_CORE_VALIDATE(m_Index * 3 < m_Mesh.m_Halfedges.size(), return HalfedgeHandle(m_Mesh, 0), "Mesh::FaceHandle::halfedge: Index out of Bounds!");
        return HalfedgeHandle(m_Mesh, m_Index * 3);
    }

    Mesh::VertexIterator Mesh::FaceHandle::vertecies() {

    }

	Mesh::HalfedgeIterator Mesh::FaceHandle::halfedges() {

    }

	Mesh::FaceIterator Mesh::FaceHandle::faces() {

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

        return VertexHandle{*this, vIndex};
    }

	Mesh::HalfedgeHandle Mesh::addHalfedge(VertexHandle from, VertexHandle to) {

        return HalfedgeHandle{*this, 0};
    }

	Mesh::FaceHandle Mesh::addFace(VertexHandle vh0, VertexHandle vh1, VertexHandle vh2) {

        return FaceHandle{*this, 0};
    }



    Mesh::VertexHandle Mesh::vertex(VertexIndex vIndex) {
        C78E_CORE_VALIDATE(vIndex < m_Positions.size(), return VertexHandle(*this, (VertexIndex)-1); "Mesh::vertex: vIndex out of Bounds!");
        return VertexHandle(*this, vIndex);
    }

	Mesh::HalfedgeHandle Mesh::halfedge(HalfedgeIndex hIndex) {
        C78E_CORE_VALIDATE(hIndex < m_Halfedges.size(), return HalfedgeHandle(*this, (HalfedgeIndex)-1); "Mesh::halfedge: hIndex out of Bounds!");
        return HalfedgeHandle(*this, hIndex);
    }

	Mesh::FaceHandle Mesh::face(FaceIndex fIndex) {
        C78E_CORE_VALIDATE(fIndex < m_FaceCount, return FaceHandle(*this, (FaceIndex)-1); "Mesh::face: fIndex out of Bounds!");
        return FaceHandle(*this, fIndex);
    }



    Mesh::Position& Mesh::position(VertexIndex vIndex) {
        C78E_CORE_ASSERT(vIndex < m_Positions.size(), "Mesh::position: VertexIndex out of Bounds!");
        return m_Positions.at(vIndex);
    }

    Mesh::VertexIterator Mesh::positions() {
        return {};
    }

    bool Mesh::hasNormals() {
        return m_Normals.size() && m_NormalMap.size();
    }

    Mesh::Normal& Mesh::normal(VertexIndex vIndex) {
        C78E_CORE_ASSERT(hasNormals(), "Mesh::normal: Mesh has no Normals!");
        auto nIt = m_NormalMap.find(vIndex);
        C78E_CORE_ASSERT(nIt != m_NormalMap.end(), "Mesh::normal: VertexIndex does not map to a Normal!");
        C78E_CORE_ASSERT(nIt->second < m_Normals.size(), "Mesh::normal: Mapped normal index is out of Bounds!");
        return m_Normals.at(nIt->second);
    }

    Mesh::VertexIterator Mesh::normals() {
        return {};
    }

    bool Mesh::hasColor() {
        return m_Colors.size() && m_ColorMap.size();
    }

    Mesh::Color& Mesh::color(VertexIndex vIndex) {
        C78E_CORE_ASSERT(hasColor(), "Mesh::color: Mesh has no Colors!");
        auto cIt = m_ColorMap.find(vIndex);
        C78E_CORE_ASSERT(cIt != m_ColorMap.end(),, "Mesh::color: VertexIndex does not map to a Color!");
        C78E_CORE_ASSERT(cIt->second < m_Colors.size(), "Mesh::color: Mapped color index is out of Bounds!");
        return m_Colors.at(cIt->second);
    }

    Mesh::VertexIterator Mesh::colors() {
        return {};
    }

    bool Mesh::hasTextureCoordinates() {
        return m_TextureCoordinates.size() && m_TextureCoordinateMap.size();
    }

    Mesh::TextureCoordinate& Mesh::textureCoordinate(VertexIndex vIndex) {
        C78E_CORE_ASSERT(hasTextureCoordinates(), "Mesh::textureCoordinate: Mesh has no TextureCoordinates!");
        auto tIt = m_TextureCoordinateMap.find(vIndex);
        C78E_CORE_ASSERT(tIt != m_TextureCoordinateMap.end(), "Mesh::textureCoordinate: VertexIndex does not map to a TextureCoordinate!");
        C78E_CORE_ASSERT(tIt->second < m_TextureCoordinates.size(), "Mesh::textureCoordinate: Mapped TextureCoordinate index is out of Bounds!");
        return m_TextureCoordinates.at(tIt->second);
    }

    Mesh::VertexIterator Mesh::textureCoordinates() {
        return {};
    }



    bool Mesh::positionExists(Position position) {
        auto it = std::find(m_Positions.begin(), m_Positions.end(), position);
        return it != m_Positions.end();
    }

    bool Mesh::normalExists(Normal normal) {
        auto it = std::find(m_Normals.begin(), m_Normals.end(), normal);
        return it != m_Normals.end();
    }

    bool Mesh::colorExists(Color color) {
        auto it = std::find(m_Colors.begin(), m_Colors.end(), color);
        return it != m_Colors.end();
    }

    bool Mesh::textureCoordinateExists(TextureCoordinate textureCoordinate) {
        auto it = std::find(m_TextureCoordinates.begin(), m_TextureCoordinates.end(), textureCoordinate);
        return it != m_TextureCoordinates.end();
    }



    bool Mesh::halfedgeVerteciesExist(VertexHandle from, VertexHandle to) {
        auto it = std::find(m_Halfedges.begin(), m_Halfedges.end(), Halfedge{ from, to });
        return it != m_Halfedges.end();
    }

    bool Mesh::FaceVerteciesExist(VertexHandle vh0, VertexHandle vh1, VertexHandle vh2) {

        return false;
    }

    bool Mesh::FaceHalfedgesExist(HalfedgeHandle he0, HalfedgeHandle he1, HalfedgeHandle he2) {
        
        return false;
    }



    template<typename T>
    Mesh::VPropertyHandle<T> Mesh::createVertexProperty(const T& initialValue) {
        size_t propIndex = m_Properties.size();
        m_Properties.emplace_back(sizeof(T), m_Positions.size());
        if(initialValue != T())
            m_Properties.back().clear(&initialValue);
        return VPropertyHandle<T>(*this, propIndex);
    }

    template<typename T>
    Mesh::HPropertyHandle<T> Mesh::createHalfedgeProperty(const T& initialValue) {
        size_t propIndex = m_Properties.size();
        m_Properties.emplace_back(sizeof(T), m_Halfedges.size());
        if(initialValue != T())
            m_Properties.back().clear(&initialValue);
        return HPropertyHandle<T>(*this, propIndex);
    }

    template<typename T>
    Mesh::FPropertyHandle<T> Mesh::createFaceProperty(const T& initialValue) {
        size_t propIndex = m_Properties.size();
        m_Properties.emplace_back(sizeof(T), m_FaceCount);
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
                *elem = *static_cast<unsigned char*>(clearValue) + (off % m_ElementSize);
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
}