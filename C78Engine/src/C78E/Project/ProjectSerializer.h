#pragma once
#include "Project.h"

namespace C78E {

	class ProjectSerializer {
	public:
		static Ref<Project> importProject(const FilePath& filepath);
		static bool exportProject(Ref<Project> project, const FilePath& filepath);

		//Project De-/Serialization
		static bool importEditorProject(Ref<Project> project, const std::string& serializedEditorProject);
		static bool importRuntimeProject(Ref<Project> project, Ref<ScopedBuffer> serializedRuntimeProject);

		static bool exportEditorProject(Ref<Project> project, std::string& serializedEditorProject);
		static bool exportRuntimeProject(Ref<Project> project, Ref<ScopedBuffer> serializedRuntimeProject);
		
	};

}