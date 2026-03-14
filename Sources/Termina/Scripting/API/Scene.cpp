#include "Scene.hpp"
#include <Termina/Renderer/UIUtils.hpp>
#include <ImGui/imgui.h>

namespace TerminaScript {
    bool Scene::Inspect(const char* label)
    {
        ImGui::TextUnformatted(label);
        ImGui::SameLine();
        return Termina::UIUtils::ScenePicker(Path);
    }
}
