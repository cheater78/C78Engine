#pragma once
#include "GUIElements.h"

#include <C78E/Core/Timestep.h>

namespace C78E::GUI {

	/*
	* TEMPLATE
	* Never use! - its just a template for which methods could be implemented for Systems providing GUI Components
	*/
	class AbstractGUISystem {
	public:
		AbstractGUISystem() = default;
		AbstractGUISystem(const AbstractGUISystem& other) = delete;
		~AbstractGUISystem() = default;

		virtual void onUpdate(C78E::Timestep dt) = 0;

		virtual void onImGuiMainMenuBar() = 0;
		virtual void onImGuiMainMenuSection() = 0;
		virtual void onImGuiRender() = 0;

	private:
		// Manager
		// GUIComponents
	};

	class DynamicElementSystem {
	public:
		class ToggleElementGroupButton : public TextButton {
		public:
			ToggleElementGroupButton(DynamicElementSystem* parent, const std::string& label, const std::vector<ElementIdentifier>& elementGroup);

		private:
			std::string m_Label;
			std::vector<ElementIdentifier> m_ElementGroup;
		};
	public: //TODO ... weird
		Ref<ToggleElementGroupButton> createToggleElementGroupButton(const std::string& label, const std::vector<ElementIdentifier>& elementGroup);
	public:
		DynamicElementSystem() = default;
		DynamicElementSystem(const DynamicElementSystem& other) = delete;
		~DynamicElementSystem() = default;

		virtual void show(Element& element) final;

		virtual bool& shouldShow(ElementIdentifier element) final;

		virtual void setShow(ElementIdentifier element) final;
		virtual void setHide(ElementIdentifier element) final;
		virtual void toggleShow(ElementIdentifier element) final;

	private:
		std::unordered_map<ElementIdentifier, bool> m_ShowElement;
	};

	class StringIndentationSystem {
	public:
		StringIndentationSystem(uint32_t inititialIndent = 0, const std::string& front = "", const std::string& back = "", const std::string& mid = " ", const std::string& single = "");
		StringIndentationSystem(const StringIndentationSystem& other) = delete;
		~StringIndentationSystem();

		virtual void pushIndent() final;
		virtual void popIndent() final;
	protected:
		virtual void setIndent(uint32_t indent) final;
		virtual uint32_t getIndent() const final;

		virtual std::string getIndentationString() const final;
		virtual std::string getIndentationString(const std::string& suffix, const std::string& spacer = " ") const final;
	private:
		uint32_t m_Indent;
		const std::string m_Front;
		const std::string m_Back;
		const std::string m_Mid;
		const std::string m_Single;
	};
}
