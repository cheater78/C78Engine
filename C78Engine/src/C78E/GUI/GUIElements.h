#pragma once
#include "ImGui/TextureHandle.h"

#include <imgui.h>
#include <random>
#include <C78E/Utils/PlatformUtils.h>

namespace C78E::GUI {

	using ElementIdentifier = int; // directly used in ImGui::PushID(int); -> must be int or int projectable

	enum LabelPostition : uint8_t {
		NONE = 0,
		RIGHT = 1,
		LEFT = 2,
		BELOW = 3,
		ABOVE = 4
	};

	/*
	* Visible
	* Base for all toggleable UI Objects
	*/
	class Visible {
	public:
		Visible(bool visible = true);
		~Visible() = default;

		virtual void setVisible(bool visible = true) final;
		virtual bool isVisible() const final;

	protected:
		bool m_Visible = true;
	};

	/*
	* UIContext
	* Base for all UI Objects that need System/Platform info
	*/
	class UIContext {
	public:
		struct Specs {
			//TODO: fetch what monitor this UI Object is drawn on (just primary rn)
			C78E::System::Monitor monitor;
			
		};
	public:
		UIContext();
		~UIContext() = default;

		virtual const Specs getSpecs() final;
		virtual glm::vec2 toAbsolute(glm::vec2 relative) const final;
		virtual glm::vec2 toRelative(glm::vec2 absolute) const final;

		virtual glm::vec2 getScreenSize() const final;
		virtual float getShorterScreenSideLength() const final;
		virtual float getLongerScreenSideLength() const final;

		virtual glm::vec2 resizeToScale(glm::vec2 size) const final;

	protected:
		float m_Scale = 1.0f;
	private:
		Specs m_Specs{};
	};

	/*
	* abstract Element
	*/
	class Element : public Visible, public UIContext {
	public:
		struct Style { //TODO: extend
		public:
			float alpha = 1.0f;
			float disabledAlpha = 0.f;

			float parentRelativeItemWidth = 0.f;
			glm::vec2 itemSpacing = {0.f, 0.f};

			glm::vec2 framePadding = { 0.f, 0.f };

		protected:
			virtual void pushStyle();
			virtual void popStyle();

			friend class C78E::GUI::Element;
		};

	public:
		Element();
		Element(const Element& other) = delete;
		~Element();

		virtual void show() = 0;
		Element::Style& style();

		inline operator ElementIdentifier() const { return m_Handle; }

	protected:
		virtual void begin() final;
		virtual void end() final;

	protected:
		ElementIdentifier m_Handle;
		Style m_Style;
	private:
		static std::random_device s_RandomDevice;
		static std::mt19937_64 s_Engine;
		static std::uniform_int_distribution<int> s_UniformDistribution;
	};

	/*
	* abstract Window
	*/
	class Window : public Element {
	public:
		struct Style : public Element::Style { //TODO: extend
		public:
			glm::vec2 windowPadding = { 0.f, 0.f };
			float windowRounding = 0.f;
			float windowBorderSize = 0.f;
			glm::vec2 windowMinSize = { 0.f, 0.f };
			glm::vec2 windowTitleAlign = { 0.f, 0.5f };


			ImGuiWindowFlags windowFlags = 0;

		private:
			virtual void pushStyle() override;
			virtual void popStyle() override;
		};
	public:
		Window(const std::string& title = "", const Style& style = {});
		Window(const Window& other) = delete;
		~Window() = default;

		virtual void show() final;

		virtual void content() = 0;

		Window::Style& style();

		virtual void setTitle(const std::string& title);
		virtual std::string getTitle(const std::string& title);

	protected:
		std::string m_Title = "Unnamed ImGui Window";
		Style m_Style;
	};

	/*
	* Text
	*/
	class Text : public Element {
	public:
		Text(const std::string& label);
		Text(const Text& other) = delete;
		~Text();

		virtual void show() override;

	protected:
		std::string m_Label = "";
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
		ImageButton(std::string label, LabelPostition labelPos, std::string toolTip, TextureHandle texture, std::function<void(void)> onClick);
		ImageButton(const ImageButton& other) = delete;
		~ImageButton();

		virtual void show() override;
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
		std::string getContent() const; // ascii chars and symbols only!, 0x20(' ') <= char <= 0x7E('~')

		virtual void show() override;

	protected:
		std::string m_Label = "";
		Ref<Buffer> m_Buffer = nullptr;
	};

	/*
	* DragFloat
	*/
	class DragFloat : public Element {
	public:
		DragFloat(float* value = nullptr, const std::string& label = "", float step = 1.f, float min = 0.f, float max = 0.f, const std::string& format = ".3f");
		~DragFloat();

		virtual void show() override;

		float& getValue();

		virtual void setStep(float step) final;
		virtual void setMin(float min) final;
		virtual void setMax(float max) final;

		inline operator float& () { return *m_Value; }
	protected:
		float m_Step;
		float m_Min;
		float m_Max;
		std::string m_Label;
		std::string m_Format;
		float* m_Value;
	};

	/*
	* DragFloat2
	*/
	class DragFloat2 : public DragFloat {
	public:
		DragFloat2(float* value = nullptr, const std::string& label = "", float step = 1.f, float min = 0.f, float max = 0.f, const std::string& format = ".3f");
		DragFloat2(glm::vec2* values = nullptr, const std::string& label = "", float step = 1.f, float min = 0.f, float max = 0.f, const std::string& format = ".3f");
		~DragFloat2();

		virtual void show() override;

		glm::vec2& getValue();

		inline operator glm::vec2& () { return *(glm::vec2*)m_Value; }
	};

	/*
	* DragFloat3
	*/
	class DragFloat3 : public DragFloat {
	public:
		DragFloat3(float* values = nullptr, const std::string& label = "", float step = 1.f, float min = 0.f, float max = 0.f, const std::string& format = ".3f");
		DragFloat3(glm::vec3* values = nullptr, const std::string& label = "", float step = 1.f, float min = 0.f, float max = 0.f, const std::string& format = ".3f");
		~DragFloat3();

		virtual void show() override;

		glm::vec3& getValue();

		inline operator glm::vec3& () { return *(glm::vec3*)m_Value; }
	};

	/*
	* DragFloat4
	*/
	class DragFloat4 :public DragFloat {
		DragFloat4(float* values = nullptr, const std::string& label = "", float step = 1.f, float min = 0.f, float max = 0.f, const std::string& format = ".3f");
		DragFloat4(glm::vec4* values = nullptr, const std::string& label = "", float step = 1.f, float min = 0.f, float max = 0.f, const std::string& format = ".3f");
		~DragFloat4();

		virtual void show() override;

		glm::vec4& getValue();

		inline operator glm::vec4& () { return *(glm::vec4*)m_Value; }
	};

}
