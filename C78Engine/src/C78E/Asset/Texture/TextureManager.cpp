#include "C78EPCH.h"
#include "TextureManager.h"

//TODO: check
#include <C78E/Core/Buffer.h>
#include <C78E/Renderer/API/Texture.h>

#include <C78E/Utils/Image/ImageLoader.h>

namespace C78E {

	void TextureImporter::TextureMeta::serialize(EditorAssetManager::AssetDescriptor assetDescriptor, YAML::Emitter& appendTo) {
		AssetHandle& handle = assetDescriptor.first;
		Ref<TextureMeta> meta = std::static_pointer_cast<TextureMeta>(assetDescriptor.second);

		appendTo << YAML::Key << "TextureType" << YAML::Value << Texture::Type::textureTypeToString(meta->textureType);

		appendTo << YAML::Key << "Specification" << YAML::BeginMap;
		appendTo << YAML::Key << "Format" << YAML::Key << Image::ImageFormat::imageFormatToString(meta->specification->format);
		switch ((uint8_t)meta->textureType) {
		case Texture::Type::None: break;
		case Texture::Type::Flat:
			appendTo << YAML::Key << "Width" << YAML::Key << std::static_pointer_cast<Texture2D::Specification>(meta->specification)->width;
			appendTo << YAML::Key << "Height" << YAML::Key << std::static_pointer_cast<Texture2D::Specification>(meta->specification)->height;
			break;
		case Texture::Type::Sphere:
			appendTo << YAML::Key << "Size" << YAML::Key << std::static_pointer_cast<CubeMap::Specification>(meta->specification)->size;
			break;
		default: break;
		}
		appendTo << YAML::EndMap;

		appendTo << YAML::Key << "Option" << YAML::BeginMap;
		appendTo << YAML::Key << "MinFilter" << YAML::Key << Texture::Filter::filterToString(meta->option->minFilter);
		appendTo << YAML::Key << "MagFilter" << YAML::Key << Texture::Filter::filterToString(meta->option->magFilter);
		appendTo << YAML::Key << "BlendU" << YAML::Key << meta->option->blenU;
		appendTo << YAML::Key << "BlendV" << YAML::Key << meta->option->blenV;
		appendTo << YAML::Key << "Brighness" << YAML::Key << meta->option->brightness;
		appendTo << YAML::Key << "Contrast" << YAML::Key << meta->option->contrast;
		switch ((uint8_t)meta->textureType) {
		case Texture::Type::None: break;
		case Texture::Type::Flat:
			appendTo << YAML::Key << "WrapU" << YAML::Key << Texture::Wrap::wrapToString(std::static_pointer_cast<Texture2D::Option>(meta->option)->wrapU);
			appendTo << YAML::Key << "WrapV" << YAML::Key << Texture::Wrap::wrapToString(std::static_pointer_cast<Texture2D::Option>(meta->option)->wrapV);
			appendTo << YAML::Key << "OriginOffset" << YAML::Key << std::static_pointer_cast<Texture2D::Option>(meta->option)->originOffset;
			appendTo << YAML::Key << "Scale" << YAML::Key << std::static_pointer_cast<Texture2D::Option>(meta->option)->scale;
			appendTo << YAML::Key << "Turbulence" << YAML::Key << std::static_pointer_cast<Texture2D::Option>(meta->option)->turbulence;
			break;
		case Texture::Type::Sphere:
			appendTo << YAML::Key << "WrapU" << YAML::Key << Texture::Wrap::wrapToString(std::static_pointer_cast<CubeMap::Option>(meta->option)->wrapU);
			appendTo << YAML::Key << "WrapV" << YAML::Key << Texture::Wrap::wrapToString(std::static_pointer_cast<CubeMap::Option>(meta->option)->wrapV);
			appendTo << YAML::Key << "WrapW" << YAML::Key << Texture::Wrap::wrapToString(std::static_pointer_cast<CubeMap::Option>(meta->option)->wrapW);
			appendTo << YAML::Key << "Turbulence" << YAML::Key << std::static_pointer_cast<CubeMap::Option>(meta->option)->turbulence;
			break;
		default: break;
		}
		appendTo << YAML::EndMap;
		return;
	}

	void TextureImporter::TextureMeta::deserialize(const YAML::Node& serializedMeta, Ref<EditorAssetManager::AssetDescriptor>& appendTo) {
		Ref<TextureMeta> meta = std::static_pointer_cast<TextureMeta>(appendTo->second);

		YAML::Node textureTypeNode = serializedMeta["TextureType"];
		C78E_CORE_VALIDATE(
			textureTypeNode && Texture::Type::textureTypeFromString(textureTypeNode.as<std::string>()) != Texture::Type::None,
			return,
			"TextureImporter::TextureMeta::deserialize: Corrupted TextureMeta!\n    TextureType: {}",
			(textureTypeNode) ? textureTypeNode.as<std::string>() : "<corrupted>"
		);
		meta->textureType = Texture::Type::textureTypeFromString(textureTypeNode.as<std::string>());

		YAML::Node textureSpecNode = serializedMeta["Specification"];
		C78E_CORE_VALIDATE(
			textureSpecNode && textureSpecNode.IsMap(),
			return,
			"TextureImporter::TextureMeta::deserialize: Corrupted Specification!\n    Specification: {}",
			(textureSpecNode) ? textureSpecNode.as<std::string>() : "<corrupted>"
		);
		YAML::Node textureSpecFormatNode = textureSpecNode["Format"];
		switch ((uint8_t)meta->textureType) {
		case Texture::Type::None: break;
		case Texture::Type::Flat:
		{
			YAML::Node textureSpecWidthNode = textureSpecNode["Width"];
			YAML::Node textureSpecHeightNode = textureSpecNode["Height"];
			C78E_CORE_VALIDATE(
				textureSpecFormatNode && textureSpecWidthNode && textureSpecHeightNode,
				return,
				"TextureImporter::TextureMeta::deserialize: Corrupted Specification!\n    Format: {}\n    Width: {}\n    Height: {}",
				(textureSpecNode) ? textureSpecNode.as<std::string>() : "<corrupted>",
				(textureSpecWidthNode) ? textureSpecWidthNode.as<std::string>() : "<corrupted>",
				(textureSpecHeightNode) ? textureSpecHeightNode.as<std::string>() : "<corrupted>"
			);
			std::static_pointer_cast<Texture2D::Specification>(meta->specification)->format = Image::ImageFormat::imageFormatFromString(textureSpecNode.as<std::string>());
			std::static_pointer_cast<Texture2D::Specification>(meta->specification)->width = textureSpecWidthNode.as<uint32_t>();
			std::static_pointer_cast<Texture2D::Specification>(meta->specification)->height = textureSpecHeightNode.as<uint32_t>();
			break;
		}
		case Texture::Type::Sphere:
		{
			YAML::Node textureSpecSizeNode = textureSpecNode["Size"];
			C78E_CORE_VALIDATE(
				textureSpecFormatNode && textureSpecSizeNode,
				return,
				"TextureImporter::TextureMeta::deserialize: Corrupted Specification!\n    Format: {}\n    Size: {}",
				(textureSpecFormatNode) ? textureSpecFormatNode.as<std::string>() : "<corrupted>",
				(textureSpecSizeNode) ? textureSpecSizeNode.as<std::string>() : "<corrupted>"
			);
			std::static_pointer_cast<CubeMap::Specification>(meta->specification)->format = Image::ImageFormat::imageFormatFromString(textureSpecNode.as<std::string>());
			std::static_pointer_cast<CubeMap::Specification>(meta->specification)->size = textureSpecSizeNode.as<uint32_t>();
			break;
		}
		default: break;
		}
		
		YAML::Node textureOptNode = serializedMeta["Option"];
		C78E_CORE_VALIDATE(
			textureOptNode && textureOptNode.IsMap(),
			return,
			"TextureImporter::TextureMeta::deserialize: Corrupted Option!\n    Option: {}",
			(textureOptNode) ? textureOptNode.as<std::string>() : "<corrupted>"
		);
		YAML::Node textureOptMinFilterNode = textureOptNode["MinFilter"];
		YAML::Node textureOptMagFilterNode = textureOptNode["MagFilter"];
		YAML::Node textureOptBlendUNode = textureOptNode["BlendU"];
		YAML::Node textureOptBlendVNode = textureOptNode["BlendV"];
		YAML::Node textureOptBrighnessNode = textureOptNode["Brighness"];
		YAML::Node textureOptContrastNode = textureOptNode["Contrast"];
		switch ((uint8_t)meta->textureType) {
		case Texture::Type::None: break;
		case Texture::Type::Flat:
		{
			YAML::Node textureOptWrapUNode = textureOptNode["WrapU"];
			YAML::Node textureOptWrapVNode = textureOptNode["WrapV"];
			YAML::Node textureOptOriginOffsetNode = textureOptNode["OriginOffset"];
			YAML::Node textureOptScaleNode = textureOptNode["Scale"];
			YAML::Node textureOptTurbulenceNode = textureOptNode["Turbulence"];
			C78E_CORE_VALIDATE(
				textureOptMinFilterNode &&
				textureOptMagFilterNode &&
				textureOptBlendUNode &&
				textureOptBlendVNode &&
				textureOptBrighnessNode &&
				textureOptContrastNode &&
				textureOptWrapUNode &&
				textureOptWrapVNode &&
				textureOptOriginOffsetNode &&
				textureOptScaleNode &&
				textureOptTurbulenceNode,
				return,
				"TextureImporter::TextureMeta::deserialize: Corrupted Specification!\n    MinFilter: {}\n    MagFilter: {}\n    BlendU: {}\n    BlendV: {}\n    Brighness: {}\n    Contrast: {}\n    WrapU: {}\n    WrapV: {}\n    OriginOffset: {}\n    Scale: {}\n    Turbulence: {}",
				(textureSpecNode) ? textureSpecNode.as<std::string>() : "<corrupted>",
				(textureOptMinFilterNode) ? textureOptMinFilterNode.as<std::string>() : "<corrupted>",
				(textureOptMagFilterNode) ? textureOptMagFilterNode.as<std::string>() : "<corrupted>",
				(textureOptBlendUNode) ? textureOptBlendUNode.as<std::string>() : "<corrupted>",
				(textureOptBlendVNode) ? textureOptBlendVNode.as<std::string>() : "<corrupted>",
				(textureOptBrighnessNode) ? textureOptBrighnessNode.as<std::string>() : "<corrupted>",
				(textureOptContrastNode) ? textureOptContrastNode.as<std::string>() : "<corrupted>",
				(textureOptWrapUNode) ? textureOptWrapUNode.as<std::string>() : "<corrupted>",
				(textureOptWrapVNode) ? textureOptWrapVNode.as<std::string>() : "<corrupted>",
				(textureOptOriginOffsetNode) ? textureOptOriginOffsetNode.as<std::string>() : "<corrupted>",
				(textureOptScaleNode) ? textureOptScaleNode.as<std::string>() : "<corrupted>",
				(textureOptTurbulenceNode) ? textureOptTurbulenceNode.as<std::string>() : "<corrupted>"
			);
			std::static_pointer_cast<Texture2D::Option>(meta->option)->minFilter = Texture::Filter::filterFromString(textureOptMinFilterNode.as<std::string>());
			std::static_pointer_cast<Texture2D::Option>(meta->option)->magFilter = Texture::Filter::filterFromString(textureOptMagFilterNode.as<std::string>());
			std::static_pointer_cast<Texture2D::Option>(meta->option)->blenU = textureOptBlendUNode.as<bool>();
			std::static_pointer_cast<Texture2D::Option>(meta->option)->blenV = textureOptBlendVNode.as<bool>();
			std::static_pointer_cast<Texture2D::Option>(meta->option)->brightness = textureOptBrighnessNode.as<float>();
			std::static_pointer_cast<Texture2D::Option>(meta->option)->contrast = textureOptContrastNode.as<float>();
			std::static_pointer_cast<Texture2D::Option>(meta->option)->wrapU = Texture::Wrap::wrapFromString(textureOptWrapUNode.as<std::string>());
			std::static_pointer_cast<Texture2D::Option>(meta->option)->wrapV = Texture::Wrap::wrapFromString(textureOptWrapVNode.as<std::string>());
			std::static_pointer_cast<Texture2D::Option>(meta->option)->originOffset = textureOptOriginOffsetNode.as<glm::vec2>();
			std::static_pointer_cast<Texture2D::Option>(meta->option)->scale = textureOptScaleNode.as<glm::vec2>();
			std::static_pointer_cast<Texture2D::Option>(meta->option)->turbulence = textureOptTurbulenceNode.as<glm::vec2>();
			break;
		}
		case Texture::Type::Sphere:
		{
			YAML::Node textureOptWrapUNode = textureOptNode["WrapU"];
			YAML::Node textureOptWrapVNode = textureOptNode["WrapV"];
			YAML::Node textureOptWrapWNode = textureOptNode["wrapW"];
			YAML::Node textureOptTurbulenceNode = textureOptNode["Turbulence"];
			C78E_CORE_VALIDATE(
				textureOptMinFilterNode &&
				textureOptMagFilterNode &&
				textureOptBlendUNode &&
				textureOptBlendVNode &&
				textureOptBrighnessNode &&
				textureOptContrastNode &&
				textureOptWrapUNode &&
				textureOptWrapVNode &&
				textureOptWrapWNode &&
				textureOptTurbulenceNode,
				return,
				"TextureImporter::TextureMeta::deserialize: Corrupted Specification!\n    MinFilter: {}\n    MagFilter: {}\n    BlendU: {}\n    BlendV: {}\n    Brighness: {}\n    Contrast: {}\n    WrapU: {}\n    WrapV: {}\n    WrapW: {}\n    Turbulence: {}",
				(textureSpecNode) ? textureSpecNode.as<std::string>() : "<corrupted>",
				(textureOptMinFilterNode) ? textureOptMinFilterNode.as<std::string>() : "<corrupted>",
				(textureOptMagFilterNode) ? textureOptMagFilterNode.as<std::string>() : "<corrupted>",
				(textureOptBlendUNode) ? textureOptBlendUNode.as<std::string>() : "<corrupted>",
				(textureOptBlendVNode) ? textureOptBlendVNode.as<std::string>() : "<corrupted>",
				(textureOptBrighnessNode) ? textureOptBrighnessNode.as<std::string>() : "<corrupted>",
				(textureOptContrastNode) ? textureOptContrastNode.as<std::string>() : "<corrupted>",
				(textureOptWrapUNode) ? textureOptWrapUNode.as<std::string>() : "<corrupted>",
				(textureOptWrapVNode) ? textureOptWrapVNode.as<std::string>() : "<corrupted>",
				(textureOptWrapWNode) ? textureOptWrapWNode.as<std::string>() : "<corrupted>",
				(textureOptTurbulenceNode) ? textureOptTurbulenceNode.as<std::string>() : "<corrupted>"
			);
			std::static_pointer_cast<CubeMap::Option>(meta->option)->minFilter = Texture::Filter::filterFromString(textureOptMinFilterNode.as<std::string>());
			std::static_pointer_cast<CubeMap::Option>(meta->option)->magFilter = Texture::Filter::filterFromString(textureOptMagFilterNode.as<std::string>());
			std::static_pointer_cast<CubeMap::Option>(meta->option)->blenU = textureOptBlendUNode.as<bool>();
			std::static_pointer_cast<CubeMap::Option>(meta->option)->blenV = textureOptBlendVNode.as<bool>();
			std::static_pointer_cast<CubeMap::Option>(meta->option)->brightness = textureOptBrighnessNode.as<float>();
			std::static_pointer_cast<CubeMap::Option>(meta->option)->contrast = textureOptContrastNode.as<float>();
			std::static_pointer_cast<CubeMap::Option>(meta->option)->wrapU = Texture::Wrap::wrapFromString(textureOptWrapUNode.as<std::string>());
			std::static_pointer_cast<CubeMap::Option>(meta->option)->wrapV = Texture::Wrap::wrapFromString(textureOptWrapVNode.as<std::string>());
			std::static_pointer_cast<CubeMap::Option>(meta->option)->wrapW = Texture::Wrap::wrapFromString(textureOptWrapWNode.as<std::string>());
			std::static_pointer_cast<CubeMap::Option>(meta->option)->turbulence = textureOptTurbulenceNode.as<glm::vec3>();
			break;
		}
		default: break;
		}

		appendTo->second = meta;
		return;
	}

	TextureImporter::TextureImporter(const FilePath& assetDirectory)
		: EditorAssetManager::Importer(assetDirectory) {
	}

	TextureImporter::~TextureImporter() {
	}

	Ref<EditorAssetManager::ImportedAssetGroup> TextureImporter::import(Ref<EditorAssetManager::AssetDescriptorMap> importDescriptorMap) {
		C78E_CORE_VALIDATE(importDescriptorMap, return nullptr, "TextureImporter::import: importDescriptorMap was null!");
		FilePath sourceFile = verifyMapSourceFile(importDescriptorMap);
		C78E_CORE_VALIDATE(!sourceFile.empty(), return nullptr, "TextureImporter::import: verifyMapSourceFile failed!");

		Ref<EditorAssetManager::ImportedAssetGroup> importedAssets = createRef<EditorAssetManager::ImportedAssetGroup>();

		for (auto [handle, baseMeta] : *importDescriptorMap) {
			Ref<TextureMeta> meta = std::static_pointer_cast<TextureMeta>(baseMeta);
			Texture::Type textureType = meta->textureType;
			Image::ImageFormat imageFormat = meta->specification->format;

			Ref<Image> image = ImageLoader::loadImage(m_AssetDirectory / meta->fileSource, true, static_cast<uint32_t>(Image::ImageFormat::imageFormatSize(imageFormat)));

			//TODO: validate image using meta, or override meta?

			switch ((uint8_t)textureType) {
			case Texture::Type::None:
			{
				Ref<Texture::Specification> spec = meta->specification;
				Ref<Texture::Option> opt = meta->option;
				C78E_CORE_ERROR("TextureImporter::import: Texture::Type::None cannot be imported!");
				break;
			}
			case Texture::Type::Flat:
			{
				Ref<Texture2D::Specification> spec = std::static_pointer_cast<Texture2D::Specification>(meta->specification);
				Ref<Texture2D::Option> opt = std::static_pointer_cast<Texture2D::Option>(meta->option);

				Ref<Texture2D> texture = Texture2D::create(image->getDataBuffer(), *spec, *opt);
				(*importedAssets)[std::static_pointer_cast<Asset>(texture)] = meta;
				break;
			}
			case Texture::Type::Sphere:
			{
				Ref<CubeMap::Specification> spec = std::static_pointer_cast<CubeMap::Specification>(meta->specification);
				Ref<CubeMap::Option> opt = std::static_pointer_cast<CubeMap::Option>(meta->option);

				Ref<CubeMap> texture = CubeMap::create(image->getDataBuffer(), *spec, *opt);
				(*importedAssets)[std::static_pointer_cast<Asset>(texture)] = meta;
				break;
			}
			default: break;
			}
			//TODO: LOG
		}

		return nullptr;
	}

	bool TextureImporter::compatible(const FilePath& assetFile) {
		return assetFile.has_extension() && (
			assetFile.extension() == C78E_FILE_EXT_IMAGE_PNG ||
			assetFile.extension() == C78E_FILE_EXT_IMAGE_JPG ||
			assetFile.extension() == C78E_FILE_EXT_IMAGE_JPEG
			);
	}
}