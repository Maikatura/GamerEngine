#include "GraphicsEngine.pch.h"
#include "Project.h"

#include "ProjectSerializer.h"
#include "Utilites/Pointers.h"
#include "Utilites/VisualProfiler.h"

namespace GamerEngine {

    Ref<Project> Project::New()
    {
        PROFILE_SCOPE("Project::New");

        s_ActiveProject = MakeRef<Project>();
        return s_ActiveProject;
    }

    Ref<Project> Project::Load(const std::filesystem::path& path)
    {
        Ref<Project> project = MakeRef<Project>();

        ProjectSerializer serializer(project);
        if (serializer.Deserialize(path))
        {
            project->m_ProjectDirectory = path.parent_path();
            s_ActiveProject = project;
            return s_ActiveProject;
        }

        return nullptr;
    }

    bool Project::SaveActive(const std::filesystem::path& path)
    {
        ProjectSerializer serializer(s_ActiveProject);
        if (serializer.Serialize(path))
        {
            s_ActiveProject->m_ProjectDirectory = path.parent_path();
            return true;
        }

        return false;
    }

}
