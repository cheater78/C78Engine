#include "C78EPCH.h"
#include "WavefrontImporter.h"

#define TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_USE_MAPBOX_EARCUT
#include "tiny_obj_loader.h"

namespace C78E {

    static bool readObjFile(tinyobj::ObjReader& reader, FilePath sourceFile, bool triangulate = false, FilePath relMaterialPath = "./");
    static bool readMtlFile(tinyobj::ObjReader& reader, FilePath absoluteSourceFile);

    static std::vector<FilePath> extractAllMtlLibFiles(const FilePath& absoluteObjFilePath);

    // Texture::Type
    static Texture::Type getMaterialTextureType(const tinyobj::texture_option_t& srcTextureOption);

    using SrcTextureInfo = std::pair<std::string, tinyobj::texture_option_t>;
    // Custom comparator for SrcTextureInfo, so we can compress with std::set
    struct SrcTextureInfoComparator {
        bool operator()(const std::pair<std::string, tinyobj::texture_option_t>& a,
            const std::pair<std::string, tinyobj::texture_option_t>& b) const;
    };
    using MaterialTextureNameMap = std::map<std::string, AssetHandle>;

    static Ref<EditorAssetManager::ImportedAssetGroup> extractMeshes(tinyobj::ObjReader& reader, const FilePath& sourceFile, Ref<EditorAssetManager::AssetDescriptorMap> importDescriptorMap);
    static Ref<EditorAssetManager::ImportedAssetGroup> extractMaterials(tinyobj::ObjReader& reader, const FilePath& sourceFile, Ref<EditorAssetManager::AssetDescriptorMap> importDescriptorMap, const FilePath& assetDirectory, const FilePath& relMaterialDirectory = "");
    /*
    * WavefrontMeshImporter
    */

    void WavefrontMeshImporter::WavefrontMeshMeta::serialize(EditorAssetManager::AssetDescriptor assetDescriptor, YAML::Emitter& appendTo) {
        AssetHandle& handle = assetDescriptor.first;
        Ref<WavefrontMeshMeta> meta = std::static_pointer_cast<WavefrontMeshMeta>(assetDescriptor.second);

        appendTo << YAML::Key << "WavefrontMesh" << YAML::BeginMap;
        
        appendTo << YAML::Key << "ShapeID" << YAML::Value << meta->wavefrontShapeID;
        appendTo << YAML::Key << "MaterialID" << YAML::Value << meta->wavefrontMaterialID;
        appendTo << YAML::Key << "GroupName" << YAML::Value << meta->wavefrontGroup;

        appendTo << YAML::EndMap;
    }

    void WavefrontMeshImporter::WavefrontMeshMeta::deserialize(const YAML::Node& serializedMeta, Ref<EditorAssetManager::AssetDescriptor>& appendTo) {
        Ref<WavefrontMeshMeta> meta = std::static_pointer_cast<WavefrontMeshMeta>(appendTo->second);
        
        YAML::Node wavefrontMeshNode = serializedMeta["WavefrontMesh"];
        C78E_CORE_VALIDATE( 
            wavefrontMeshNode && wavefrontMeshNode.IsMap(),
            return,
            "WavefrontMeshImporter::WavefrontMeshMeta::deserialize: Corrupted WavefrontMeshMeta!\n    WavefrontMesh:  <corrupted>"
        );

        YAML::Node shapeNameNode = wavefrontMeshNode["ShapeID"];
        YAML::Node materialNameNode = wavefrontMeshNode["MaterialID"];
        YAML::Node groupNameNode = wavefrontMeshNode["GroupName"];

        C78E_CORE_VALIDATE(
            shapeNameNode && materialNameNode && groupNameNode,
            return,
            "WavefrontMeshImporter::WavefrontMeshMeta::deserialize: Corrupted WavefrontMeshMeta!\n    ShapeName: {}\n    MaterialName: {}\n    GroupName: {}",
            (shapeNameNode) ? shapeNameNode.as<std::string>() : "<corrupted>",
            (materialNameNode) ? materialNameNode.as<std::string>() : "<corrupted>",
            (groupNameNode) ? groupNameNode.as<std::string>() : "<corrupted>"
        );

        meta->wavefrontShapeID = shapeNameNode.as<int>();
        meta->wavefrontMaterialID = materialNameNode.as<int>();
        meta->wavefrontGroup = groupNameNode.as<std::string>();
        
        appendTo->second = meta;
        return;
    }

    bool WavefrontMeshImporter::WavefrontMeshMeta::operator==(const WavefrontMeshMeta& meta) {
        return wavefrontShapeID == meta.wavefrontShapeID && wavefrontMaterialID == meta.wavefrontMaterialID && wavefrontGroup == meta.wavefrontGroup;
    }

    WavefrontMeshImporter::WavefrontMeshImporter(const FilePath& assetDirectory)
        : MeshImporter::MeshImporter(assetDirectory) {
    }

    WavefrontMeshImporter::~WavefrontMeshImporter() {
    }

    Ref<EditorAssetManager::ImportedAssetGroup> WavefrontMeshImporter::import(Ref<EditorAssetManager::AssetDescriptorMap> importDescriptorMap) {
        C78E_CORE_VALIDATE(importDescriptorMap, return nullptr, "WavefrontMeshImporter::import: importDescriptorMap was null!");
        FilePath sourceFile = verifyMapSourceFile(importDescriptorMap);
        C78E_CORE_VALIDATE(!sourceFile.empty(), return nullptr, "MeshImporter::import: verifyMapSourceFile failed!");
        const FilePath absoluteSourceFilePath = m_AssetDirectory / sourceFile;
        
        tinyobj::ObjReader reader;
        // Read in the sourceFile
        C78E_CORE_VALIDATE(
            readObjFile(reader, absoluteSourceFilePath),
            return nullptr,
            "WavefrontMeshImporter::import: Failed to read Wavefront Objects!\n    File: {}",
            absoluteSourceFilePath
        );

        // Import Meshes
        Ref<EditorAssetManager::ImportedAssetGroup> importedAssets = extractMeshes(reader, sourceFile, importDescriptorMap);
        // Import Materials
        for (FilePath relativeMtlFile : extractAllMtlLibFiles(absoluteSourceFilePath))
            importedAssets->merge(*extractMaterials(reader, m_AssetDirectory / ((sourceFile.has_parent_path()) ? sourceFile.parent_path() / relativeMtlFile : relativeMtlFile), importDescriptorMap, m_AssetDirectory));
        
        return importedAssets;
    }

    bool WavefrontMeshImporter::compatible(const FilePath& assetFile) {
        return assetFile.has_extension() && assetFile.extension() == C78E_FILE_EXT_WAFEFRONT_MODEL;
    }

    /*
    * WavefrontMaterialImporter
    */

    void WavefrontMaterialImporter::WavefrontMaterialMeta::serialize(EditorAssetManager::AssetDescriptor assetDescriptor, YAML::Emitter& appendTo) {
        AssetHandle& handle = assetDescriptor.first;
        Ref<WavefrontMaterialMeta> meta = std::static_pointer_cast<WavefrontMaterialMeta>(assetDescriptor.second);

        appendTo << YAML::Key << "WavefrontName" << YAML::Value << meta->wavefrontName;
        appendTo << YAML::Key << "Dependecies" << YAML::BeginSeq;
        for (const auto& dep : meta->dependecies)
            appendTo << YAML::Key << dep.first << YAML::Value << dep.second;
        appendTo << YAML::EndSeq;
    }

    void WavefrontMaterialImporter::WavefrontMaterialMeta::deserialize(const YAML::Node& serializedMeta, Ref<EditorAssetManager::AssetDescriptor>& appendTo) {
        Ref<WavefrontMaterialMeta> meta = std::static_pointer_cast<WavefrontMaterialMeta>(appendTo->second);

        YAML::Node wavefrontNameNode = serializedMeta["WavefrontName"];
        YAML::Node dependencyNode = serializedMeta["Dependecies"];

        C78E_CORE_VALIDATE(
            wavefrontNameNode && dependencyNode && dependencyNode.IsMap(),
            return,
            "WavefrontMeshImporter::WavefrontMeshMeta::deserialize: Corrupted WavefrontMaterialMeta!\nWavefrontName: {}\nDependecies: {}",
            (wavefrontNameNode) ? wavefrontNameNode.as<std::string>() : "<corrupted>",
            (dependencyNode) ? ((dependencyNode.IsMap()) ? std::to_string(std::size(dependencyNode)) + " elements" : "<corrupted(NotAMap)>") : "<corrupted>"
        );

        meta->wavefrontName = wavefrontNameNode.as<std::string>();
        for (const auto& node : dependencyNode) {
            YAML::Node keyNode = node.first;
            YAML::Node valueNode = node.second;

            C78E_CORE_VALIDATE(
                keyNode && valueNode,
                continue,
                "WavefrontMeshImporter::WavefrontMeshMeta::deserialize: Corrupted Dependency!\nAssetHandle: {}\nWavefrontName: {}",
                (keyNode) ? std::to_string(keyNode.as<AssetHandle>()) : "<corrupted>",
                (valueNode) ? valueNode.as<std::string>() : "<corrupted>"
            );

            meta->dependecies[keyNode.as<AssetHandle>()] = valueNode.as<std::string>();
        }
        appendTo->second = meta;
        return;
    }

    bool WavefrontMaterialImporter::WavefrontMaterialMeta::operator==(const WavefrontMaterialMeta& meta) {
        if (wavefrontName == meta.wavefrontName && meta.dependecies.size() == meta.dependecies.size()) {
            for (size_t off = 0; off < meta.dependecies.size(); off++) {
                const auto it = std::next(dependecies.begin(), off);
                const auto itOther = std::next(meta.dependecies.begin(), off);

                if (it->first != itOther->first || it->second != itOther->second)
                    return false;
            }
            return true;
        }
        else return false;
    }

    WavefrontMaterialImporter::WavefrontMaterialImporter(const FilePath& assetDirectory)
        : MaterialImporter::MaterialImporter(assetDirectory){
    }

    WavefrontMaterialImporter::~WavefrontMaterialImporter() {
    }

    Ref<EditorAssetManager::ImportedAssetGroup> WavefrontMaterialImporter::import(Ref<EditorAssetManager::AssetDescriptorMap> importDescriptorMap) {
        C78E_CORE_VALIDATE(importDescriptorMap, return nullptr, "WavefrontMeshImporter::import: importDescriptorMap was null!");
        FilePath sourceFile = verifyMapSourceFile(importDescriptorMap);
        C78E_CORE_VALIDATE(!sourceFile.empty(), return nullptr, "MeshImporter::import: verifyMapSourceFile failed!");
        const FilePath absoluteSourceFilePath = m_AssetDirectory / sourceFile;
        
        tinyobj::ObjReader reader;
        // Read in the sourceFile
        C78E_CORE_VALIDATE(
            readMtlFile(reader, absoluteSourceFilePath),
            return nullptr,
            "WavefrontMaterialImporter::import: Failed to read Wavefront Material!\n    File: {}",
            absoluteSourceFilePath
        );
        // Import Materials, Texture
        return extractMaterials(reader, sourceFile, importDescriptorMap, m_AssetDirectory);
    }

    bool WavefrontMaterialImporter::compatible(const FilePath& assetFile) {
        return assetFile.has_extension() && assetFile.extension() == C78E_FILE_EXT_WAFEFRONT_MATERIAL;
    }

    /*
    * Helpers Implementation
    */

    /**
     * @brief Determines whether a provided Wavefront Object File contains vertex color data
     * @param absoluteObjFilePath the Wavefront Object Files absolute Path
     * @return whether vertex color data was found
     */
    static bool fileHasColorData(const FilePath& absoluteObjFilePath) {
        std::ifstream file(absoluteObjFilePath);
        C78E_CORE_VALIDATE(file.is_open(), return false, "WavefrontImporter:fileHasColorData: failed to open File {}", absoluteObjFilePath);

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;

            if (prefix == "v") { // Vertex line
                float x, y, z, r, g, b;
                if (iss >> x >> y >> z >> r >> g >> b) {
                    return true; // Found a vertex with color data
                }
            }
        }

        return false; // No color data found
    }

    /**
     * @brief Extracts all Wavefront Material FileNames from a Wavefront Object file, specified as mtllib
     * @param absoluteObjFilePath the Wavefront Object Files absolute Path
     * @return the vector of Wavefront Material FileNames
     */
    static std::vector<FilePath> extractAllMtlLibFiles(const FilePath& absoluteObjFilePath) {
        std::vector<FilePath> mtlFiles;
        std::ifstream objFile(absoluteObjFilePath);

        C78E_CORE_VALIDATE(objFile.is_open(), return mtlFiles, "WavefrontImporter::extractAllMtlLibFiles: Failed to read File: {}", absoluteObjFilePath);

        std::string line;
        while (std::getline(objFile, line)) {
            if (line.substr(0, 6) == "mtllib") {
                std::string mtlFile = line.substr(7);  // Extract filename
                mtlFiles.push_back(mtlFile);
            }
        }

        objFile.close();
        return mtlFiles;
    }

    /**
     * @brief Reads in a Wavefront Object File
     * @param reader the ObjReader storing the read in data
     * @param absoluteSourceFile the Wavefront Object File Path
     * @param triangulate whether to triangualate any read Mesh data
     * @param relMaterialPath relative Path to the Wavefront Material Files
     * @return whether the operation was successful
     */
    static bool readObjFile(tinyobj::ObjReader& reader, FilePath absoluteSourceFile, bool triangulate, FilePath relMaterialPath) {
        tinyobj::ObjReaderConfig reader_config;
        reader_config.mtl_search_path = relMaterialPath.string();
        reader_config.triangulate = triangulate;
        reader_config.vertex_color = fileHasColorData(absoluteSourceFile); // determine wheter there is actual color data, so tinyObj wont make up defaults (which for some reason can be white..)


        C78E_CORE_VALIDATE(reader.ParseFromFile(absoluteSourceFile.string(), reader_config),
            return false,
            "WavefrontImporter: Failed to Parse Wavefront Obj File: {}\n TinyOBJ Error: {}",
            absoluteSourceFile.string(),
            (!reader.Error().empty()) ? reader.Error() : "No error message!"
        );

        C78E_CORE_SOFT_VALIDATE(reader.Warning().empty(), "WavefrontImporter: TinyOBJ Warn: {}", reader.Warning());
        return true;
    }

    /**
     * @brief Reads in a Wavefront Material File
     * @param reader the ObjReader storing the read in data
     * @param absoluteSourceFile  the Wavefront Material File Path
     * @return whether the operation was successful
     */
    static bool readMtlFile(tinyobj::ObjReader& reader, FilePath absoluteSourceFile) {
        tinyobj::ObjReaderConfig reader_config;

        C78E_CORE_VALIDATE(reader.ParseFromFile(absoluteSourceFile.string(), reader_config),
            return false,
            "WavefrontImporter: Failed to Parse Wavefront Mtl File: {}\n TinyOBJ Error: {}",
            absoluteSourceFile.string(),
            (!reader.Error().empty()) ? reader.Error() : "No error message!"
        );

        C78E_CORE_SOFT_VALIDATE(reader.Warning().empty(), "WavefrontImporter: TinyOBJ Warn: {}", reader.Warning());
        return true;
    }

    /**
     * @brief Retrieves the native Texture::Type from a tinyobj::texture_option_t
     * @param srcTextureOption the tinyobj::texture_option_t to be read from
     * @return the native Texture::Type
     */
    static Texture::Type getMaterialTextureType(const tinyobj::texture_option_t& srcTextureOption) {
        switch (srcTextureOption.type) {
        case tinyobj::texture_type_t::TEXTURE_TYPE_NONE: return Texture::Type::Flat;
        case tinyobj::texture_type_t::TEXTURE_TYPE_SPHERE: return Texture::Type::Sphere;
            //case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_TOP: return Texture::Type::None;
            //case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_BOTTOM: return Texture::Type::None;
            //case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_FRONT: return Texture::Type::None;
            //case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_BACK: return Texture::Type::None;
            //case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_LEFT: return Texture::Type::None;
            //case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_RIGHT: return Texture::Type::None;
        default: return Texture::Type::None;
        }
    }

    /**
     * @brief Converts a tinyobj::texture_option_t to a CubeMap::Texture2D
     * @param srcTextureOption the tinyobj::texture_option_t to be read from
     * @return the native Texture2D::Option
     */
    static Texture2D::Option getMaterialTexture2DOption(const tinyobj::texture_option_t& srcTextureOption) {
        Texture2D::Option opt;
        C78E_CORE_VALIDATE(getMaterialTextureType(srcTextureOption) == Texture::Type::Flat, return opt, "WavefrontImporter::getMaterialTexture2DOption: TextureFormat is not Flat!");
        // Texture::Option
        opt.minFilter; //?
        opt.magFilter; //?
        opt.blenU = srcTextureOption.blendu;
        opt.blenV = srcTextureOption.blendv;
        opt.brightness = srcTextureOption.brightness;
        opt.contrast = srcTextureOption.contrast;

        // Texture2D::Option
        opt.wrapU = (srcTextureOption.clamp) ? Texture::Wrap::ClampToEdge : Texture::Wrap::Repeat;
        opt.wrapV = (srcTextureOption.clamp) ? Texture::Wrap::ClampToEdge : Texture::Wrap::Repeat;
        opt.originOffset = glm::vec2{ srcTextureOption.origin_offset[0], srcTextureOption.origin_offset[1] };
        opt.scale = glm::vec2{ srcTextureOption.scale[0], srcTextureOption.scale[1] };
        opt.turbulence = glm::vec2{ srcTextureOption.turbulence[0], srcTextureOption.turbulence[1] };

		return opt;
    }

    /**
     * @brief Converts a tinyobj::texture_option_t to a CubeMap::Option
     * @param srcTextureOption the tinyobj::texture_option_t to be read from
     * @return the native CubeMap::Option
     */
    static CubeMap::Option getMaterialCubeMapOption(const tinyobj::texture_option_t& srcTextureOption) {
        CubeMap::Option opt;
        C78E_CORE_VALIDATE(getMaterialTextureType(srcTextureOption) == Texture::Type::Sphere, return opt, "WavefrontImporter::getMaterialCubeMapOption: TextureFormat is not Sphere!");
        // Texture::Option
        opt.minFilter; //?
        opt.magFilter; //?
        opt.blenU = srcTextureOption.blendu;
        opt.blenV = srcTextureOption.blendv;
        opt.brightness = srcTextureOption.brightness;
        opt.contrast = srcTextureOption.contrast;

        // Texture2D::Option
        opt.wrapU = (srcTextureOption.clamp) ? Texture::Wrap::ClampToEdge : Texture::Wrap::Repeat;
        opt.wrapV = (srcTextureOption.clamp) ? Texture::Wrap::ClampToEdge : Texture::Wrap::Repeat;
        opt.wrapW = (srcTextureOption.clamp) ? Texture::Wrap::ClampToEdge : Texture::Wrap::Repeat;

        opt.turbulence = glm::vec3{ srcTextureOption.turbulence[0], srcTextureOption.turbulence[1], srcTextureOption.turbulence[2] };
        return opt;
    }

    /**
     * @brief Converts the int illum to IlluminationModel
     * @param srcIlluminationModel illum as in
     * @return the matching IlluminationModel
     */
    Material::IlluminationModel getMaterialIlluminationModel(int srcIlluminationModel) {
        return Material::IlluminationModel((uint8_t)srcIlluminationModel);
    }

    bool SrcTextureInfoComparator::operator()(const std::pair<std::string, tinyobj::texture_option_t>& a,
        const std::pair<std::string, tinyobj::texture_option_t>& b) const {
        // Compare by string first
        if (a.first != b.first) {
            return a.first < b.first;
        }

        // Now compare texture_option_t field by field
        const tinyobj::texture_option_t& left = a.second;
        const tinyobj::texture_option_t& right = b.second;

        return std::tie(left.type, left.sharpness, left.brightness, left.contrast,
            left.origin_offset[0], left.origin_offset[1], left.origin_offset[2],
            left.scale[0], left.scale[1], left.scale[2],
            left.turbulence[0], left.turbulence[1], left.turbulence[2],
            left.texture_resolution, left.clamp, left.imfchan, left.blendu, left.blendv,
            left.bump_multiplier, left.colorspace)
            < std::tie(right.type, right.sharpness, right.brightness, right.contrast,
                right.origin_offset[0], right.origin_offset[1], right.origin_offset[2],
                right.scale[0], right.scale[1], right.scale[2],
                right.turbulence[0], right.turbulence[1], right.turbulence[2],
                right.texture_resolution, right.clamp, right.imfchan, right.blendu, right.blendv,
                right.bump_multiplier, right.colorspace);
    }

    /**
     * @brief Collects the SrcTextureInfos of a BasicMaterial
     * @param srcMaterial the tinyobj::material_t to be read from
     * @return an array of SrcTextureInfos
     */
    static std::array<SrcTextureInfo, 8> getBasicMaterialTextureFileNames(const tinyobj::material_t& srcMaterial) {
        return {
            SrcTextureInfo(srcMaterial.ambient_texname,             srcMaterial.ambient_texopt              ),
            SrcTextureInfo(srcMaterial.diffuse_texname,             srcMaterial.diffuse_texopt              ),
            SrcTextureInfo(srcMaterial.specular_texname,            srcMaterial.specular_texopt             ),
            SrcTextureInfo(srcMaterial.specular_highlight_texname,  srcMaterial.specular_highlight_texopt   ),
            SrcTextureInfo(srcMaterial.bump_texname,                srcMaterial.bump_texopt                 ),
            SrcTextureInfo(srcMaterial.displacement_texname,        srcMaterial.displacement_texopt         ),
            SrcTextureInfo(srcMaterial.alpha_texname,               srcMaterial.alpha_texopt                ),
            SrcTextureInfo(srcMaterial.reflection_texname,          srcMaterial.reflection_texopt           )
        };
    }

    /**
     * @brief Collects the SrcTextureInfos of a PBRMaterial
     * @param srcMaterial the tinyobj::material_t to be read from
     * @return an array of SrcTextureInfos
     */
    static std::array<SrcTextureInfo, 5> getPBRMaterialTextureFileNames(const tinyobj::material_t& srcMaterial) {
        return {
            SrcTextureInfo(srcMaterial.roughness_texname,   srcMaterial.roughness_texopt),
            SrcTextureInfo(srcMaterial.metallic_texname,    srcMaterial.metallic_texopt),
            SrcTextureInfo(srcMaterial.sheen_texname,       srcMaterial.sheen_texopt),
            SrcTextureInfo(srcMaterial.emissive_texname,    srcMaterial.emissive_texopt),
            SrcTextureInfo(srcMaterial.normal_texname,      srcMaterial.normal_texopt)
        };
    }

    /**
     * @brief Collects the SrcTextureInfos of both the BasicMaterial and the PBRMaterial
     * @param srcMaterial the tinyobj::material_t to be read from
     * @return an array of SrcTextureInfos
     */
    static std::set<SrcTextureInfo, SrcTextureInfoComparator> getAllMaterialTextureFileNames(const tinyobj::material_t& srcMaterial) {
        std::set<SrcTextureInfo, SrcTextureInfoComparator> fileNames;
        const std::array<SrcTextureInfo, 8> basic = getBasicMaterialTextureFileNames(srcMaterial);
        const std::array<SrcTextureInfo, 5> pbr = getPBRMaterialTextureFileNames(srcMaterial);
        fileNames.insert(basic.begin(), basic.end());
        fileNames.insert(pbr.begin(), pbr.end());
        return fileNames;
    }

    /**
     * @brief Converts tinyobj::material_t properties to native BasicMaterial::Properties
     * @param srcMaterial the tinyobj::material_t to be read from
     * @return the native BasicMaterial::Properties
     */
    static BasicMaterial::Properties getBasicMaterialProperties(const tinyobj::material_t& srcMaterial) {
        return {
            {srcMaterial.ambient[0], srcMaterial.ambient[1], srcMaterial.ambient[2]},
            {srcMaterial.diffuse[0], srcMaterial.diffuse[1], srcMaterial.diffuse[2]},
            {srcMaterial.specular[0], srcMaterial.specular[1], srcMaterial.specular[2]},
            {srcMaterial.transmittance[0], srcMaterial.transmittance[1], srcMaterial.transmittance[2]},
            {srcMaterial.emission[0], srcMaterial.emission[1], srcMaterial.emission[2]},
            srcMaterial.shininess,
            srcMaterial.ior,
            srcMaterial.dissolve
        };
    }

    /**
     * @brief Converts tinyobj::material_t properties to native PBRMaterial::Properties
     * @param srcMaterial the tinyobj::material_t to be read from
     * @return the native PBRMaterial::Properties
     */
    static PBRMaterial::Properties getPBRMaterialProperties(const tinyobj::material_t& srcMaterial) {
        return {
            srcMaterial.roughness,
            srcMaterial.metallic,
            srcMaterial.sheen,
            srcMaterial.clearcoat_thickness,
            srcMaterial.clearcoat_roughness,
            srcMaterial.anisotropy,
            srcMaterial.anisotropy_rotation
        };
    }

    /**
     * @brief Converts the Texture file names of a BasicMaterial to AssetHandles
     * @param srcMaterial the tinyobj::material_t to be read from
     * @param materialTextureNameMap map of loaded Material Textures for conversion
     * @return
     */
    static BasicMaterial::Textures getBasicMaterialTextures(const tinyobj::material_t& srcMaterial, const MaterialTextureNameMap& materialTextureNameMap) {
        return {
            materialTextureNameMap.at(srcMaterial.ambient_texname),
            materialTextureNameMap.at(srcMaterial.diffuse_texname),
            materialTextureNameMap.at(srcMaterial.specular_texname),
            materialTextureNameMap.at(srcMaterial.specular_highlight_texname),
            materialTextureNameMap.at(srcMaterial.bump_texname),
            materialTextureNameMap.at(srcMaterial.displacement_texname),
            materialTextureNameMap.at(srcMaterial.alpha_texname),
            materialTextureNameMap.at(srcMaterial.reflection_texname)
        };
    }

    /**
     * @brief Converts the Texture file names of a PBRMaterial to AssetHandles
     * @param srcMaterial the tinyobj::material_t to be read from
     * @param materialTextureNameMap map of loaded Material Textures for conversion
     * @return 
     */
    static PBRMaterial::Textures getPBRMaterialTextures(const tinyobj::material_t& srcMaterial, const MaterialTextureNameMap& materialTextureNameMap) {
        return {
            materialTextureNameMap.at(srcMaterial.roughness_texname),
            materialTextureNameMap.at(srcMaterial.metallic_texname),
            materialTextureNameMap.at(srcMaterial.sheen_texname),
            materialTextureNameMap.at(srcMaterial.emissive_texname),
            materialTextureNameMap.at(srcMaterial.normal_texname)
        };
    }

    /**
     * @brief Converts a tinyobj::material_t to a BasicMaterial
     * @param material the Material to be written to
     * @param srcMaterial the tinyobj::material_t to be read from
     * @param materialTextureNameMap map of loaded Material Textures
     */
    static inline void parseBasicMaterial(Ref<BasicMaterial> material, const tinyobj::material_t& srcMaterial, const MaterialTextureNameMap& materialTextureNameMap) {
        material->m_MaterialProperties = getBasicMaterialProperties(srcMaterial);
        material->m_MaterialTextures = getBasicMaterialTextures(srcMaterial, materialTextureNameMap);
    }

    /**
     * @brief Converts a tinyobj::material_t to a PBRMaterial
     * @param material the Material to be written to
     * @param srcMaterial the tinyobj::material_t to be read from
     * @param materialTextureNameMap map of loaded Material Textures
     */
    static inline void parsePBRMaterial(Ref<PBRMaterial> material, const tinyobj::material_t& srcMaterial, const MaterialTextureNameMap& materialTextureNameMap) {
        material->m_MaterialProperties = getPBRMaterialProperties(srcMaterial);
        material->m_MaterialTextures = getPBRMaterialTextures(srcMaterial, materialTextureNameMap);
    }

    /**
     * @brief Converts a Wavefront Shape(by id) to (single Material) Meshes
     * @param reader conataining the Wavefront data
     * @param shapeID the Shape ID of the shape to convert
     * @return Meshes split by Materials
     */
    static EditorAssetManager::ImportedAssetGroup parseWavefrontShape(tinyobj::ObjReader& reader, int shapeID) {
        std::map<size_t, Ref<Mesh>> meshes; // Meshes with one Material each(or none)
        std::map<size_t, WavefrontMeshImporter::WavefrontMeshMeta> metas;

        const tinyobj::attrib_t& srcAttributes = reader.GetAttrib();
        const std::vector<tinyobj::shape_t>& srcShapes = reader.GetShapes();
        const std::vector<tinyobj::material_t>& srcMaterials = reader.GetMaterials();

        const tinyobj::shape_t& shape = srcShapes[shapeID];

        // the current Material and therefor also Mesh ID
        size_t currentMMID = -1; // -1(size_t max) -> no material

        // Loop over faces(polygon)
        size_t srcFaceAttrOffset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
            bool faceIsValid = true;
            /*
            * First, query the face material to determine whether its nessecary to change the(create a new / switch from the current) material
            * if srcFaceMaterialID < 0 there is no material -> which gets one mesh(materialless mesh)
            * if srcFaceMaterialID >= 0, it identifies a material
            */
            const int srcFaceMaterialID = shape.mesh.material_ids[f];
            if (srcFaceMaterialID < 0)
                currentMMID = -1;
            else if (srcFaceMaterialID != currentMMID)
                currentMMID = srcFaceMaterialID;

            metas[currentMMID].wavefrontShapeID = shapeID;
            metas[currentMMID].wavefrontMaterialID = srcFaceMaterialID;
            metas[currentMMID].wavefrontGroup = shape.name;

            Ref<Mesh>& mesh = meshes[currentMMID]; // provide the current mesh, materialless mesh at -1(size_t max)
            if (!mesh)
                mesh = Mesh::createMesh(); // init mesh, first time

            const size_t faceVertexCount = size_t(shape.mesh.num_face_vertices[f]);
            C78E_CORE_VALIDATE(faceVertexCount > 2, continue, "WavefrontImporter: Face {} has less than 3 Vertecies, skipping!", f);
            std::vector<Mesh::VertexIndex> faceVertexIndecies = std::vector<Mesh::VertexIndex>(faceVertexCount); // mandatory
            // for optional vertex Data !vector::empty() || vector::size() will determine whether the 
            std::vector<Mesh::NormalIndex> faceNormalIndecies; // optional
            std::vector<Mesh::TextureCoordinateIndex> faceTextureCoordinateIndecies; // optional
            std::vector<Mesh::ColorIndex> faceColorIndecies; // optional

            /*
            * Loop over all vertecies in this face, gather vertex data
            * a face has to be consistent in its atrttributes, that means e.g. if one faceVertex has a normal (v//n), all faceVerties have to have normals
            */
            for (size_t srcFaceVertID = 0; srcFaceVertID < faceVertexCount; srcFaceVertID++) {
                const tinyobj::index_t srcVertAttr = shape.mesh.indices[srcFaceAttrOffset + srcFaceVertID];

                const bool hasNormal = srcVertAttr.normal_index >= 0;
                const bool hasTextureCoordinate = srcVertAttr.texcoord_index >= 0;
                const bool hasColor = !srcAttributes.colors.empty(); // only works bc we check before parsing

                // VertexPosition
                /* mandatory */ {
                    const Mesh::Position position = {
                        srcAttributes.vertices[3 * size_t(srcVertAttr.vertex_index) + 0],
                        srcAttributes.vertices[3 * size_t(srcVertAttr.vertex_index) + 1],
                        srcAttributes.vertices[3 * size_t(srcVertAttr.vertex_index) + 2],
                        1.f
                    };
                    faceVertexIndecies[srcFaceVertID] = mesh->addVertex(position);
                }

                // VertexNormal(optional, )
                if (srcFaceVertID && (faceIsValid &= hasNormal == faceNormalIndecies.empty())) break; // if not first time and attr definition is ambiguous, discard face
                if (hasNormal) {
                    if (!srcFaceVertID) faceNormalIndecies = std::vector<Mesh::NormalIndex>(faceVertexCount); // first time alloc
                    const Mesh::Normal normal = {
                        srcAttributes.normals[3 * size_t(srcVertAttr.normal_index) + 0],
                        srcAttributes.normals[3 * size_t(srcVertAttr.normal_index) + 1],
                        srcAttributes.normals[3 * size_t(srcVertAttr.normal_index) + 2],
                        0.f
                    };
                    faceNormalIndecies[srcFaceVertID] = mesh->addNormal(normal);
                }

                // VertexTextureCoordinate(optional, )
                if (srcFaceVertID && (faceIsValid &= hasTextureCoordinate == faceTextureCoordinateIndecies.empty())) break; // if not first time and attr definition is ambiguous, discard face
                if (hasTextureCoordinate) {
                    if (!srcFaceVertID) faceTextureCoordinateIndecies = std::vector<Mesh::TextureCoordinateIndex>(faceVertexCount); // first time alloc
                    const Mesh::TextureCoordinate textureCoordinate = {
                        srcAttributes.texcoords[2 * size_t(srcVertAttr.texcoord_index) + 0],
                        srcAttributes.texcoords[2 * size_t(srcVertAttr.texcoord_index) + 1]
                    };
                    faceTextureCoordinateIndecies[srcFaceVertID] = mesh->addTextureCoordinate(textureCoordinate);
                }

                // VertexColor(optional, )
                if (srcFaceVertID && (faceIsValid &= hasColor == faceColorIndecies.empty())) break; // if not first time and attr definition is ambiguous, discard face
                if (hasColor) {
                    if (!srcFaceVertID) faceColorIndecies = std::vector<Mesh::ColorIndex>(faceVertexCount); // first time alloc
                    const Mesh::Color color = {
                        srcAttributes.colors[3 * size_t(srcVertAttr.vertex_index) + 0],
                        srcAttributes.colors[3 * size_t(srcVertAttr.vertex_index) + 1],
                        srcAttributes.colors[3 * size_t(srcVertAttr.vertex_index) + 2],
                        1.f
                    };
                    faceColorIndecies[srcFaceVertID] = mesh->addColor(color);
                }
            }

            if (!faceIsValid) continue; // discard face if not valid

            /*
            * construct all Halfedges based on the the face vertecies
            * add Halfedge from: last, to: first, at the end
            */
            std::vector<Mesh::HalfedgeIndex> halfedges = std::vector<Mesh::HalfedgeIndex>(faceVertexCount);
            for (auto vIt = faceVertexIndecies.begin(); vIt != --faceVertexIndecies.end();) {
                Mesh::VertexIndex from = *vIt;
                Mesh::VertexIndex to = *(++vIt);
                halfedges[vIt - faceVertexIndecies.begin()] = mesh->addHalfedge(from, to);
            }
            halfedges.back() = mesh->addHalfedge(faceVertexIndecies.back(), faceVertexIndecies.front());

            // finally add the face
            mesh->addFace(halfedges);

            srcFaceAttrOffset += faceVertexCount;
        } //faces

        EditorAssetManager::ImportedAssetGroup importedAssetGroup;
        for (const auto& [mmid, mesh] : meshes)
            importedAssetGroup[mesh] = createRef<WavefrontMeshImporter::WavefrontMeshMeta>(metas[mmid]);

        return importedAssetGroup;
    }

    /**
    * @brief Searches the ImportDescriptorMap for a WavefrontMeshMeta matching the provided one
    * @param meshMeta WavefrontMeshMeta that has to match
    * @param importDescriptorMap the ImportDescriptorMap to search in
    * @return the found AssetHandle or AssetHandle::invalid if none found
    */
    static AssetHandle getMeshHandle(Ref<WavefrontMeshImporter::WavefrontMeshMeta> meshMeta, Ref<EditorAssetManager::AssetDescriptorMap> importDescriptorMap) {
        for (const auto& [handle, meta] : *importDescriptorMap) {
            if (meta->type != Asset::Type::Mesh)
                continue;

            Ref<WavefrontMeshImporter::WavefrontMeshMeta> existingMeshMeta = std::static_pointer_cast<WavefrontMeshImporter::WavefrontMeshMeta>(meta);
            if ((*existingMeshMeta).operator==(*meshMeta))
                return handle;
        }
        return AssetHandle::invalid();
    }

    /**
    * @brief Extracts and Creates Mesh Assets and Metas from a tinyobj::ObjReader
    * @param reader the tinyobj::ObjReader to extract from
    * @param sourceFile the (assetDirectory relative) source file, for new Asset Metas
    * @param importDescriptorMap alr existing Meta Info
    * @return all imported Meshes
    */
    static Ref<EditorAssetManager::ImportedAssetGroup> extractMeshes(tinyobj::ObjReader& reader, const FilePath& sourceFile, Ref<EditorAssetManager::AssetDescriptorMap> importDescriptorMap) {
        auto& srcAttributes = reader.GetAttrib();
        auto& srcShapes = reader.GetShapes();
        auto& srcMaterials = reader.GetMaterials();

        Ref<EditorAssetManager::ImportedAssetGroup> importedAssets = createRef<EditorAssetManager::ImportedAssetGroup>();

        // Loop over shapes => Mesh
        for (size_t s = 0; s < srcShapes.size(); s++) {
            EditorAssetManager::ImportedAssetGroup shapeMeshes = parseWavefrontShape(reader, static_cast<int>(s)); // load Mesh and WavefrontMeshMeta
            for (auto& [shapeMesh, shapeMeta] : shapeMeshes) {
                Ref<Mesh> readMesh = std::static_pointer_cast<Mesh>(shapeMesh);
                Ref<WavefrontMeshImporter::WavefrontMeshMeta> readMeshMeta = std::static_pointer_cast<WavefrontMeshImporter::WavefrontMeshMeta>(shapeMeta);

                // Search existing Handle
                if (AssetHandle handle = getMeshHandle(readMeshMeta, importDescriptorMap)) {  // existing Handle found
                    // getting the existing MeshMeta, its the same as readMeshMeta(since thats how its matched), however this one also contains the Base Meta Info
                    Ref<WavefrontMeshImporter::WavefrontMeshMeta> existingMeshMeta = std::static_pointer_cast<WavefrontMeshImporter::WavefrontMeshMeta>(importDescriptorMap->at(handle));
                    readMesh->handle() = handle;

                    EditorAssetManager::AssetLinkedMeta importedAssetLinkedMeta{ readMesh, existingMeshMeta };
                    importedAssets->insert(importedAssetLinkedMeta);

                }
                else { // first time Mesh Import, setup required
                    AssetHandle newHandle{};

                    // WavefrontMeshMeta is already read in, now create a base Meta
                    readMeshMeta->type = Asset::Type::Mesh;
                    readMeshMeta->fileSource = sourceFile;
                    // primitive name creation for now, this needs something more user friendly
                    readMeshMeta->name = sourceFile.filename().string() + readMeshMeta->wavefrontGroup + std::to_string(readMeshMeta->wavefrontShapeID) + std::to_string(readMeshMeta->wavefrontMaterialID);

                    readMesh->handle() = newHandle;

                    EditorAssetManager::AssetLinkedMeta importedAssetLinkedMeta{ readMesh, readMeshMeta };
                    importedAssets->insert(importedAssetLinkedMeta);
                }
            }

        } //shapes

        return importedAssets;
    }

    /**
     * @brief Checks if a float is set(not a default), just for nicer readability below
     * @param real the float value to be checked
     * @param defaultValue the default value that specifies the unset state
     * @return whether the float is not the default
     */
    static inline bool isRealDefined(const tinyobj::real_t& real, const float defaultValue = 0.f) {
        return real != defaultValue;
    }

    /**
     * @brief Checks if a vec3 is set(not a default), just for nicer readability below
     * @param real float* to the first vec3 value
     * @param defaultValue the default value that specifies the unset state
     * @return whether the vec3 is not the default
     */
    static inline bool isReal3Defined(const tinyobj::real_t* real, const float defaultValue = 0.f) {
        return
            isRealDefined(real[0], defaultValue) &&
            isRealDefined(real[1], defaultValue) &&
            isRealDefined(real[2], defaultValue);
    }

    /**
     * @brief Checks if a String is empty, completely unnecessary, just for nicer readability below
     * @param str the string to be checked
     * @return whether the string contains something
     */
    static inline bool isStringDefined(const std::string& str) {
        return !str.empty();
    }

    /**
     * @brief Checks the Source material fields, if strictly fields of Basic OR PBR are set
     * @param material the Source material
     * @return the Material Type, is (base) Material if ambiguous or none set
     */
    static Material::Type getMaterialType(const tinyobj::material_t& material) {
        // while illum is only in the spec for basic, it can be used sometimes in pbr, so dont check it
        const bool basicMaterialParamDefined =
            isReal3Defined(material.ambient) ||
            isReal3Defined(material.diffuse) ||
            isReal3Defined(material.specular) ||
            isReal3Defined(material.transmittance) ||
            isReal3Defined(material.emission) ||
            isRealDefined(material.shininess) ||
            isRealDefined(material.ior) ||
            isRealDefined(material.dissolve);

        const bool pbrMaterialParamDefined =
            isRealDefined(material.roughness) ||
            isRealDefined(material.metallic) ||
            isRealDefined(material.sheen) ||
            isRealDefined(material.clearcoat_thickness) ||
            isRealDefined(material.clearcoat_roughness) ||
            isRealDefined(material.anisotropy) ||
            isRealDefined(material.anisotropy_rotation) ||

            isStringDefined(material.roughness_texname) ||
            isStringDefined(material.metallic_texname) ||
            isStringDefined(material.sheen_texname) ||
            isStringDefined(material.emissive_texname) ||
            isStringDefined(material.normal_texname);

        C78E_CORE_VALIDATE(basicMaterialParamDefined != pbrMaterialParamDefined, return Material::Type::Material, "");
        return (basicMaterialParamDefined) ? Material::Type::BasicMaterial : Material::Type::PBRMaterial;
    }

    /**
    * @brief Searches the ImportDescriptorMap for a WavefrontMaterialMeta matching the provided one
    * @param materialMeta WavefrontMaterialMeta that has to match
    * @param importDescriptorMap the ImportDescriptorMap to be searched
    * @return the found AssetHandle or AssetHandle::invalid if none found
    */
    static AssetHandle getMaterialHandle(const std::string wavefrontMaterialName, Ref<EditorAssetManager::AssetDescriptorMap> importDescriptorMap) {
        for (const auto& [handle, meta] : *importDescriptorMap) {
            if (meta->type != Asset::Type::Material)
                continue;

            Ref<WavefrontMaterialImporter::WavefrontMaterialMeta> existingMeshMeta = std::static_pointer_cast<WavefrontMaterialImporter::WavefrontMaterialMeta>(meta);
            if (existingMeshMeta->wavefrontName == wavefrontMaterialName)
                return handle;
        }
        return AssetHandle::invalid();
    }

    /**
    * @brief Searches a WavefrontMaterialMeta for a texture name matching the provided one
    * @param wavefrontMaterialTextureName texture name that needs to be matched
    * @param materialMeta the WavefrontMaterialMeta to be searched
    * @return the found AssetHandle or AssetHandle::invalid if none found
    */
    static AssetHandle getMaterialTextureHandle(const std::string wavefrontMaterialTextureName, const WavefrontMaterialImporter::WavefrontMaterialMeta& materialMeta) {
        for (const auto& [handle, name] : materialMeta.dependecies) {
            if (name == wavefrontMaterialTextureName)
                return handle;
        }
        return AssetHandle::invalid();
    }

    /**
    * @brief Extracts and Creates Material Assets and Metas from a tinyobj::ObjReader,
    * also imports and loads all required Textures for the Materials
    * @param reader the tinyobj::ObjReader to extract from
    * @param sourceFile the (assetDirectory relative) source file, for new Asset Metas
    * @param importDescriptorMap alr existing Meta Info
    * @param assetDirectory the FilePath of the AssetDirectory, required for Texture Importing
    * @param relMaterialDirectory the relative MaterialDirectory, if materials are somewhere else, so are the Textures
    * @return all imported Materials and Textures
    */
    static Ref<EditorAssetManager::ImportedAssetGroup> extractMaterials(tinyobj::ObjReader& reader, const FilePath& sourceFile, Ref<EditorAssetManager::AssetDescriptorMap> importDescriptorMap, const FilePath& assetDirectory, const FilePath& relMaterialDirectory) {
        auto& srcAttributes = reader.GetAttrib();
        auto& srcShapes = reader.GetShapes();
        auto& srcMaterials = reader.GetMaterials();

        Ref<EditorAssetManager::ImportedAssetGroup> importedAssets = createRef<EditorAssetManager::ImportedAssetGroup>();

        // Loop over Materials
        for (size_t materialID = 0; materialID < srcMaterials.size(); materialID++) {
            const tinyobj::material_t& srcMaterial = srcMaterials.at(materialID);
            const std::string srcMaterialName = srcMaterial.name;
            Material::Type type = getMaterialType(srcMaterial);
            Ref<Material> material = Material::create(type);
            Ref<WavefrontMaterialImporter::WavefrontMaterialMeta> materialMeta = createRef<WavefrontMaterialImporter::WavefrontMaterialMeta>();

            const auto& requiredTextures = getAllMaterialTextureFileNames(srcMaterial);
            Ref<EditorAssetManager::AssetDescriptorMap> materialTextureDescriptors = createRef<EditorAssetManager::AssetDescriptorMap>();
            MaterialTextureNameMap materialTextureNameMap = { { "", AssetHandle::invalid() } };

            // Search existing Handle
            if (AssetHandle existingHandle = getMaterialHandle(srcMaterialName, importDescriptorMap)) {
                material->handle() = existingHandle;
                materialMeta = std::static_pointer_cast<WavefrontMaterialImporter::WavefrontMaterialMeta>(importDescriptorMap->at(existingHandle));

            }
            else {
                material->handle() = AssetHandle();

                materialMeta->type = Asset::Type::Material;
                materialMeta->fileSource = sourceFile;
                // primitive name creation for now, this needs something more user friendly
                materialMeta->name = sourceFile.filename().string() + srcMaterialName;

                materialMeta->wavefrontName = srcMaterialName;
            }

            material->m_IlluminationModel = getMaterialIlluminationModel(srcMaterial.illum);

            // Material Textures
            for (const auto& [textureName, textureOption] : requiredTextures) {
                Ref<TextureImporter::TextureMeta> textureMeta = createRef<TextureImporter::TextureMeta>();
                AssetHandle handle = getMaterialTextureHandle(textureName, *materialMeta);
                if (!handle)
                    handle = AssetHandle();

                textureMeta->textureType = getMaterialTextureType(textureOption);
                switch ((uint8_t)textureMeta->textureType) {
                case Texture::Type::Flat:
                    textureMeta->option = createRef<Texture2D::Option>();
                    *textureMeta->option = getMaterialTexture2DOption(textureOption);
                    break;
                case Texture::Type::Sphere:
                    textureMeta->option = createRef<CubeMap::Option>();
                    *textureMeta->option = getMaterialCubeMapOption(textureOption);
                    break;
                default: break;
                }

                textureMeta->type = Asset::Type::Texture;
                textureMeta->fileSource = textureName;
                textureMeta->name = textureName;

                EditorAssetManager::AssetDescriptor textureDescriptor{ handle, textureMeta };
                materialTextureDescriptors->insert(textureDescriptor);
            }
            importedAssets->merge(*TextureImporter(assetDirectory).import(materialTextureDescriptors)); // add loaded Textures

            switch (type) {
            case C78E::Material::Type::Material:
                C78E_CORE_ASSERT(false, "WavefrontMaterialImporter::extractMaterials: importing of generic Materials is not supported!");
            case C78E::Material::Type::BasicMaterial:
                parseBasicMaterial(std::static_pointer_cast<BasicMaterial>(material), srcMaterial, materialTextureNameMap);
                break;
            case C78E::Material::Type::PBRMaterial:
                parsePBRMaterial(std::static_pointer_cast<PBRMaterial>(material), srcMaterial, materialTextureNameMap);
                break;
            default: break;
            }

            EditorAssetManager::AssetLinkedMeta importedAssetLinkedMeta{ material, materialMeta };
            importedAssets->insert(importedAssetLinkedMeta);
        } //Materials

        return importedAssets;
    }

}