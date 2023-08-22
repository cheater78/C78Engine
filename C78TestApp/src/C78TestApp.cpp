#include "C78TestApp.h"
#include <C78e.h>
#include <imgui/imgui.h>

class DebugLayer : public C78e::Layer {
public:
	DebugLayer() : Layer("DebugLayer") {

    }

	void onUpdate() override {
		//C78_INFO("LayerUpdate");
	}

	void onEvent(C78e::Event& e) override {
		//C78_TRACE("{0}", e);
	}

	virtual void onImGuiRender() override {
        if (!m_active) return;

        ImGui::Begin(c_Title.c_str(), &m_active);
        if (ImGui::SmallButton("[Debug] Add 5 entries"))
        {
            static int counter = 0;
            const char* categories[3] = { "info", "warn", "error" };
            const char* words[] = { "Bumfuzzled", "Cattywampus", "Snickersnee", "Abibliophobia", "Absquatulate", "Nincompoop", "Pauciloquent" };
            for (int n = 0; n < 5; n++)
            {
                const char* category = categories[counter % IM_ARRAYSIZE(categories)];
                const char* word = words[counter % IM_ARRAYSIZE(words)];
                m_Log.AddLog("[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n",
                    ImGui::GetFrameCount(), category, ImGui::GetTime(), word);
                counter++;
            }
        }
        ImGui::End();

        // Actually call in the regular Log helper (which will Begin() into the same window as we just did)
        m_Log.Draw(c_Title.c_str(), &m_active);
	}
private:
	bool m_active = true;
    const std::string c_Title = "Test";

    struct ExampleAppLog {
        ImGuiTextBuffer     Buf;
        ImGuiTextFilter     Filter;
        ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
        bool                AutoScroll;  // Keep scrolling if already at the bottom.

        ExampleAppLog()
        {
            AutoScroll = true;
            Clear();
        }

        void    Clear()
        {
            Buf.clear();
            LineOffsets.clear();
            LineOffsets.push_back(0);
        }

        void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
        {
            int old_size = Buf.size();
            va_list args;
            va_start(args, fmt);
            Buf.appendfv(fmt, args);
            va_end(args);
            for (int new_size = Buf.size(); old_size < new_size; old_size++)
                if (Buf[old_size] == '\n')
                    LineOffsets.push_back(old_size + 1);
        }

        void    Draw(const char* title, bool* p_open = NULL)
        {
            if (!ImGui::Begin(title, p_open))
            {
                ImGui::End();
                return;
            }

            // Options menu
            if (ImGui::BeginPopup("Options"))
            {
                ImGui::Checkbox("Auto-scroll", &AutoScroll);
                ImGui::EndPopup();
            }

            // Main window
            if (ImGui::Button("Options"))
                ImGui::OpenPopup("Options");
            ImGui::SameLine();
            bool clear = ImGui::Button("Clear");
            ImGui::SameLine();
            bool copy = ImGui::Button("Copy");
            ImGui::SameLine();
            Filter.Draw("Filter", -100.0f);

            ImGui::Separator();

            if (ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar))
            {
                if (clear)
                    Clear();
                if (copy)
                    ImGui::LogToClipboard();

                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
                const char* buf = Buf.begin();
                const char* buf_end = Buf.end();
                if (Filter.IsActive())
                {
                    // In this example we don't use the clipper when Filter is enabled.
                    // This is because we don't have random access to the result of our filter.
                    // A real application processing logs with ten of thousands of entries may want to store the result of
                    // search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
                    for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
                    {
                        const char* line_start = buf + LineOffsets[line_no];
                        const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                        if (Filter.PassFilter(line_start, line_end))
                            ImGui::TextUnformatted(line_start, line_end);
                    }
                }
                else
                {
                    // The simplest and easy way to display the entire buffer:
                    //   ImGui::TextUnformatted(buf_begin, buf_end);
                    // And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward
                    // to skip non-visible lines. Here we instead demonstrate using the clipper to only process lines that are
                    // within the visible area.
                    // If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them
                    // on your side is recommended. Using ImGuiListClipper requires
                    // - A) random access into your data
                    // - B) items all being the  same height,
                    // both of which we can handle since we have an array pointing to the beginning of each line of text.
                    // When using the filter (in the block of code above) we don't have random access into the data to display
                    // anymore, which is why we don't use the clipper. Storing or skimming through the search result would make
                    // it possible (and would be recommended if you want to search through tens of thousands of entries).
                    ImGuiListClipper clipper;
                    clipper.Begin(LineOffsets.Size);
                    while (clipper.Step())
                    {
                        for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
                        {
                            const char* line_start = buf + LineOffsets[line_no];
                            const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                            ImGui::TextUnformatted(line_start, line_end);
                        }
                    }
                    clipper.End();
                }
                ImGui::PopStyleVar();

                // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
                // Using a scrollbar or mouse-wheel will take away from the bottom edge.
                if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                    ImGui::SetScrollHereY(1.0f);
            }
            ImGui::EndChild();
            ImGui::End();
        }
    } m_Log{};
};


class C78TestApp : public C78e::Application
{
public:
	C78TestApp()
	{
		pushLayer(new DebugLayer());
	}
	~C78TestApp()
	{

	}
};

C78e::Application* C78e::createApplication() {
	return new C78TestApp();
}

/*
Scene& genTestScene(float ascpectRatio) {
	static C78e::Scene firstScene;

	C78e::Entity camEnt = firstScene.createEntity("Cam");
	C78e::Camera cam(camEnt.getEnttity(), &firstScene);
	firstScene.setActiveCam(cam);
	cam.setPerspectiveProj(
		70.f,
		ascpectRatio,
		.001f,
		1000.f
	);
	cam.setPosRot(glm::vec3(0.f, 0.f, -3.f), glm::vec3(0.f, 0.f, 0.f));

	C78e::Entity ent1 = firstScene.createEntity("Vase");
	C78e::TransformComponent& ent1trans = ent1.getComponent<C78e::TransformComponent>();
	ent1trans.translation = glm::vec3(0.0f, 0.f, 0.f);
	ent1trans.scale = glm::vec3(2.f, 1.f, 2.f);
	C78e::MeshComponent& ent1mesh = ent1.addComponent<C78e::MeshComponent>("/res/models/smooth_vase.obj");

	C78e::Entity ent2 = firstScene.createEntity("Floor");
	C78e::TransformComponent& ent2trans = ent2.getComponent<C78e::TransformComponent>();
	ent2trans.translation = glm::vec3(0.0f, 0.f, 0.f);
	ent2trans.scale = glm::vec3(1.f, 1.f, 1.f);
	C78e::MeshComponent& ent2mesh = ent2.addComponent<C78e::MeshComponent>("/res/models/quad.obj");

	std::vector<glm::vec3> lightColors{
		{1.f, .1f, .1f},
		{ .1f, .1f, 1.f },
		{ .1f, 1.f, .1f },
		{ 1.f, 1.f, .1f },
		{ .1f, 1.f, 1.f },
		{ 1.f, 1.f, 1.f }
	};

	for (int i = 0; i < lightColors.size(); i++) {
		C78e::Entity plight = firstScene.createEntity("PLight");
		C78e::PointLightComponent& plightc = plight.addComponent<C78e::PointLightComponent>(lightColors[i], 1.3f, 0.1f);
		C78e::TransformComponent& plightt = plight.getComponent<C78e::TransformComponent>();
		plightt.translation = glm::vec3(0.f, .0f, 2.f);
		plightt.scale = glm::vec3(.01f, .01f, .01f);

		auto rotateLight = glm::rotate(glm::mat4(1.f), (i * glm::two_pi<float>()) / lightColors.size(), { .0f, -1.f, .0f });
		plightt.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
		plightt.translation += glm::vec3(0.f, -1.f, 0.f);
	}
	return firstScene;
}
*/