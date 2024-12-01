#pragma once
#include "C78EPCH.h"

namespace C78E {

	struct MaterialProperties {
		glm::vec3 ambient = { 1.f, 1.f, 1.f };						// Ka: Ambient Color Reaction [0,1]³
		alignas(16) glm::vec3 diffuse = { 1.f, 1.f, 1.f };			// Kd: Diffuse Color Reaction [0,1]³
		alignas(16) glm::vec3 specular = { 1.f, 1.f, 1.f };			// Ks: Specular Color Reaction [0,1]³
		alignas(16) glm::vec3 transmittance = { 1.f, 1.f, 1.f };	// Tf: Transmission Filter [0,1]³
		alignas(16) glm::vec3 emission = { 0.f, 0.f, 0.f };			// Ke: Emissive Color [0,1]³
		alignas(16) float shininess = 100.0;						// Ns: Specular Highlight [0,1000]
		float ior = 1.f;											// Ni: Optical Density -> Index Of Refraction [0.001,10] (e.g. glass 1.5)
		float dissolve = 1.0f;										// d:  Dissolve Factor -> Opacity [0,1]
	};

}
