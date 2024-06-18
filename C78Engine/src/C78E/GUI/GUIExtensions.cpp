#include "C78ePCH.h"
#include "GUIExtensions.h"

namespace C78E::GUI {

	/*
	* abstract Table
	*/
	Table::TableEntry::TableEntry() {
	}

	Table::TableEntry::~TableEntry() {
	}

	void Table::TableEntry::show() {
	}



	Table::Table() {

	}

	Table::~Table() {
	}

	void Table::show() {
		Element::begin();
		m_Valid = ImGui::BeginTable(m_ImGuiStrID.c_str(), 2, m_Style.tabBarFlags);

		if (!m_Valid) return;

		ImGui::TableSetupColumn("Attribute", m_Style.attributeFlags);
		ImGui::TableSetupColumn("Value", m_Style.valueFlags);

		ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, ImGui::GetColorU32(ImVec4(m_Style.rowBgColor0.r, m_Style.rowBgColor0.g, m_Style.rowBgColor0.b, m_Style.rowBgColor0.a)));//0.05f, 0.10f, 0.13f, 0.6f)));
		ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg1, ImGui::GetColorU32(ImVec4(m_Style.rowBgColor1.r, m_Style.rowBgColor1.g, m_Style.rowBgColor1.b, m_Style.rowBgColor1.a)));//0.05f, 0.10f, 0.13f, 1.0f)));
		ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImVec4(m_Style.cellBgColor.r, m_Style.cellBgColor.g, m_Style.cellBgColor.b, m_Style.cellBgColor.a)));//0.15f, 0.10f, 0.13f, 1.0f)));
		
		content();

		ImGui::EndTable();
		Element::end();
	}

	inline bool Table::isValid() const {
		return m_Valid;
	}






	/*
	* KeyValueTable
	*/
	KeyValueTable::KeyValueTable(Style style, const std::string& indentStep, const std::string& strID)
		: Table() {
	}

	KeyValueTable::~KeyValueTable() {
	}

	void KeyValueTable::insLabeledElement(const std::string& label, Element& element, ImGuiTableRowFlags rowFlags) {
		if (!Table::isValid()) return;
		if (!DynamicElementSystem::shouldShow(element)) return;

		ImGui::TableNextRow(rowFlags);
		{
			ImGui::TableNextColumn();
			// Identifier
			Text(getIndentationString(label)).show();
			//--------------------------------------------
			ImGui::TableNextColumn();
			// Value
			DynamicElementSystem::show(element);
			//--------------------------------------------
		}
	}

	Ref<DynamicElementSystem::ToggleElementGroupButton> KeyValueTable::insToggleElementGroup(const std::string& label, const std::string& value, Ref<ToggleElementGroupButton> element, std::vector<ElementIdentifier> elementGroup, ImGuiTableRowFlags rowFlags) {
		Ref<ToggleElementGroupButton> button = element;
		if (!button)
			button = createToggleElementGroupButton(label, elementGroup);
		insLabeledElement(label, *std::static_pointer_cast<Element>(button), rowFlags);
		return button;
	}

	

	

}