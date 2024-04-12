#pragma once
#include <C78E/ImGui/TextureHandle.h>
#include <imgui.h>
#include <random>

namespace C78E {

	namespace Gui {
		/*
		* Primitives
		*/
		static void SameLine() { ImGui::SameLine(); }
		static const glm::vec2 autoSize() { return {0.f, 0.f}; }
		static const glm::vec4 autoColor() { return { 0.f, 0.f, 0.f, 0.f }; }
		static const glm::vec4 markedColor() { return { .1f, .2f, .3f, 1.f }; }
		static const glm::vec4 noTintColor() { return { 1.f, 1.f, 1.f, 1.f }; }

		static glm::vec2 getContentRegionAvail() { ImVec2 size = ImGui::GetContentRegionAvail(); return glm::vec2(size.x, size.y); }

		/*
		* BaseClass Element
		* Should not be used!
		*/
		class Element {
		private:
			static std::random_device s_RandomDevice;
			static std::mt19937_64 s_Engine;
			static std::uniform_int_distribution<int> s_UniformDistribution;
		public:
			Element();
			Element(const Element& other) = delete;
			~Element();

			void begin();
			void end();
		public:
			virtual void show() = 0;
		protected:
			int m_Handle;
		};

		/*
		* TextButton
		*/
		class TextButton : public Element {
		public:
			TextButton(std::string label, std::function<void(void)> onClick);
			TextButton(const TextButton& other) = delete;
			~TextButton();

			virtual void show() override;
			virtual void show(glm::vec2 size);

		protected:
			std::string m_Label = "";
			std::function<void(void)> m_OnClick = nullptr;
		};

		/*
		* CyclingTextButton
		*/
		class CyclingTextButton : public Element {
		public:
			CyclingTextButton(std::vector<std::string> labels, std::vector<std::function<void()>> onClicks, uint32_t initState = 0);
			CyclingTextButton(const CyclingTextButton& other) = delete;
			~CyclingTextButton();

			virtual void show() override;
			virtual void show(glm::vec2 size);

		protected:
			uint32_t m_State = 0;
			std::vector<std::string> m_Label = {};
			std::vector<std::function<void()>> m_OnClicks = {};
		};

		/*
		* ImageButton
		*/
		class ImageButton : public Element {
		public:
			enum LabelPostition : uint8_t {
				NONE = 0,
				RIGHT = 1,
				LEFT = 2,
				BELOW = 3,
				ABOVE = 4
			};
		public:
			ImageButton(std::string label, LabelPostition labelPos, std::string toolTip, TextureHandle texture, std::function<void(void)> onClick);
			ImageButton(const ImageButton& other) = delete;
			~ImageButton();

			virtual void show() override;
			virtual void show(glm::vec2 size);
			virtual void show(glm::vec2 size, glm::vec4 backGroundColor, glm::vec4 tintColor);
		protected:
			std::string m_Label = "";
			LabelPostition m_LabelPostition = LabelPostition::RIGHT;
			std::string m_Tooltip = "";
			std::function<void(void)> m_OnClick = nullptr;
			TextureHandle m_Texture;
		};

		/*
		* TextInput
		*/
		class TextInput : public Element {
		public:
			TextInput(const std::string& label = "", const std::string& defaultValue = "", size_t maxLength = 1024, Ref<Buffer> buffer = nullptr);
			TextInput(const TextInput& other) = delete;
			~TextInput();

			void setContent(const std::string& content);
			std::string getContent(); // ascii chars and symbols only!, 0x20(' ') <= char <= 0x7E('~')

			virtual void show() override;
			virtual void show(uint32_t width);

		protected:
			std::string m_Label = "";
			Ref<Buffer> m_Buffer = nullptr;
		};

	};

}
