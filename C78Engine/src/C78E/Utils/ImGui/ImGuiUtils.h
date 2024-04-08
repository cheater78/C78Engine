#pragma once

#include <imgui.h>
#include <random>

namespace C78E {

	namespace Gui {
		/*
		* Primitives
		*/
		static void SameLine() { ImGui::SameLine(); }

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
		private:
			int m_Handle;
		};

		/*
		*/
		class TextButton : public Element {
		public:
			TextButton(std::string label, std::function<void(void)> onClick);
			TextButton(const TextButton& other) = delete;
			~TextButton();

			virtual void show() override;

		private:
			std::string m_Label = "";
			std::function<void(void)> m_OnClick = nullptr;
		};

		/*
		*/
		class CyclingTextButton : public Element {
		public:
			CyclingTextButton(std::vector<std::string> labels, std::vector<std::function<void()>> onClicks, uint32_t initState = 0);
			CyclingTextButton(const CyclingTextButton& other) = delete;
			~CyclingTextButton();

			virtual void show() override;

		private:
			uint32_t m_State = 0;
			std::vector<std::string> m_Label = {};
			std::vector<std::function<void()>> m_OnClicks = {};
		};

		/*
		*/
		class TextInput : public Element {
		public:
			TextInput(std::string label = "", std::string defaultValue = "", size_t maxLength = 1024, Ref<Buffer> buffer = nullptr);
			TextInput(const TextInput& other) = delete;
			~TextInput();

			void setContent(const std::string& content);
			std::string getContent(); // ascii chars and symbols only!, 0x20(' ') <= char <= 0x7E('~')

			virtual void show() override;

		private:
			std::string m_Label = "";
			Ref<Buffer> m_Buffer = nullptr;
		};

	};

}
