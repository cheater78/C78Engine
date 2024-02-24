#include "C78ePCH.h"
#include "Model.h"

#include <C78E/Assets/AssetManager.h>
#include <C78E/Utils/TinyObjectLoader/TinyObjectLoader.h>
#include <C78E/Utils/StdUtils.h>



namespace C78E {

    Model::Model(const Asset<Mesh>& mesh,const Asset<Material>& material)
        : m_Mesh(mesh), m_Material(material)
    { }

    Model::~Model()
	{ }

    Ref<Model> Model::create(std::string file)
    {
        return nullptr;
    }

    std::vector<Ref<Model>> Model::createAll(std::string file) {
        return TinyObjectLoader::loadAllModels(file);
    }
}