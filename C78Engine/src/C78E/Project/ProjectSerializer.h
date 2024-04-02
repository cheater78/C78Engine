#pragma once
#include "Project.h"

namespace C78E {

	class ProjectSerializer {
	public:
		ProjectSerializer(Ref<Project> project);

		bool serializeProject(const FilePath& filepath);
		bool deserializeProject(const FilePath& filepath);
	private:
		Ref<Project> m_Project;
	};

}