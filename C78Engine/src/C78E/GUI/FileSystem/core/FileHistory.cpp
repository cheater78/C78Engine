#include "C78ePCH.h"
#include "FileHistory.h"

namespace C78E {
	FileHistory::FileHistory(FilePath baseDirectory) {
		m_CurrentIndex = 0;
		m_History = { baseDirectory };
	}

	FilePath FileHistory::getCWD() const {
		return m_History.at(m_CurrentIndex);
	}

	FilePath FileHistory::getBasePath() const {
		return m_History.at(0);
	}

	void FileHistory::cd(FilePath filePath) {
		//changing Directory exactly like cdForward
		if (wouldForward(filePath)) {
			cdForward();
			return;
		}
		//changing Directory exactly like cdBackward
		if (wouldBackward(filePath)) {
			cdBackward();
			return;
		}

		// if u are somewhere in ur History path and take a new turn,
		// bc it wasn't forward (see ^^)
		// delete the old "forward"-Path since u just entered a new one
		// if u couldn't go forward u alr r at the (back-)end of your path...
		if (canCDForward())
			m_History.erase(std::next(m_History.begin(), m_CurrentIndex + 1), m_History.end());
		//...so just add the new point to ur path
		m_History.push_back(filePath);
	}

	void FileHistory::cdParent() {
		C78_CORE_ASSERT(canCDParent(), "FileHistory::cdParent: no Parent Path to cd to!");
		cd(m_History.at(m_CurrentIndex).parent_path());
	}

	void FileHistory::cdForward() {
		C78_CORE_ASSERT(canCDForward(), "FileHistory::cdForward: no History to move forward to!");
		m_CurrentIndex++;
	}

	void FileHistory::cdBackward() {
		C78_CORE_ASSERT(canCDBackward(), "FileHistory::cdBackward: no History to move backward to!");
		m_CurrentIndex--;
	}

	bool FileHistory::canCDParent() const {
		return m_History.at(m_CurrentIndex).has_parent_path();
	}

	bool FileHistory::canCDForward() const {
		return m_CurrentIndex + 1 < m_History.size();
	}

	bool FileHistory::canCDBackward() const {
		return m_CurrentIndex > 0;
	}

	bool FileHistory::wouldForward(FilePath filePath) const {
		return canCDForward() && m_History.at(m_CurrentIndex + 1) == filePath;
	}

	bool FileHistory::wouldBackward(FilePath filePath) const {
		return canCDBackward() && m_History.at(m_CurrentIndex - 1) == filePath;
	}

}
