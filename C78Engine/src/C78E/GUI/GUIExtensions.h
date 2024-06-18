#pragma once
#include "GUIElements.h"
#include "GUISystems.h"

namespace C78E::GUI {

	/*
	* abstract Table
	*/
	class Table : public Element, protected DynamicElementSystem, protected StringIndentationSystem {
	public:
		struct Style {
			ImGuiTabBarFlags tabBarFlags = 0;
			ImGuiTableColumnFlags attributeFlags = 0;
			ImGuiTableColumnFlags valueFlags = 0;

			glm::vec4 rowBgColor0 = glm::vec4(0.f);
			glm::vec4 rowBgColor1 = glm::vec4(0.f);
			glm::vec4 cellBgColor = glm::vec4(0.f);



		};

		class TableEntry {
		public:
			TableEntry();
			~TableEntry();

			virtual void show() final;

			virtual void content() = 0;

		private:

		};
	public:
		Table();
		~Table();
		virtual void show() final;

		virtual void content() = 0;
	protected:
		inline bool isValid() const;
	private:
		uint32_t m_Column = 0;
		std::string m_ImGuiStrID;
		Style m_Style;

		bool m_Valid = false;
	};

	class KeyValueTable : public Table {
	public:
		
	public:
		KeyValueTable(Style style, const std::string& indentStep = "", const std::string& strID = "");
		KeyValueTable(const KeyValueTable& other) = delete;
		~KeyValueTable();

		void insLabeledElement(const std::string& label, Element& element, ImGuiTableRowFlags rowFlags = 0);
		Ref<ToggleElementGroupButton> insToggleElementGroup(const std::string& label, const std::string& value, Ref<ToggleElementGroupButton> element, std::vector<ElementIdentifier> elementGroup, ImGuiTableRowFlags rowFlags = 0);

	private:
		
	};
	
}