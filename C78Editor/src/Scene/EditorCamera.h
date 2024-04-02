#pragma once
#include <C78E.h>
#include <C78Elibs.h>


namespace C78Editor {
	class EditorCamera : C78E::SceneCamera {
	public:
		enum ControllerType {
			FPS,
			BLENDER
		};

	public:
		static C78E::Entity create(C78E::Ref<C78E::Scene> scene, std::string name);
		static void update(C78E::Entity camera, C78E::Timestep delta);


	private:
		
	};
}