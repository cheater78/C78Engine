#include "C78EPCH.h"
#include "Material.h"

namespace C78E {

	Ref<Material> Material::create(Material::Type materialType) {
		switch (materialType) {
		case C78E::Material::Type::Material:
			return Ref<Material>();
		case C78E::Material::Type::BasicMaterial:
			return Ref<BasicMaterial>();
		case C78E::Material::Type::PBRMaterial:
			return Ref<PBRMaterial>();
		default:
			C78E_CORE_VALIDATE(false, return nullptr, "Material::create: illegal materialType, was {}", std::to_string((uint8_t)materialType));
		}
	}

}

