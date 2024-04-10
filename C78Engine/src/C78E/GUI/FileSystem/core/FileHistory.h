#pragma once


namespace C78E {

	class FileHistory {
	public:
		FileHistory(FilePath baseDirectory);
		FileHistory(const FileHistory& other) = delete;
		~FileHistory() = default;
		
		FilePath getCWD() const;
		FilePath getBasePath() const;

		void cd(FilePath filePath);
		void cdParent();
		void cdForward();
		void cdBackward();

		bool canCDParent() const;
		bool canCDForward() const;
		bool canCDBackward() const;
	private:
		bool wouldForward(FilePath filePath) const;
		bool wouldBackward(FilePath filePath) const;
	private:
		/*
		* m_History.at(0) -> DefaultDirectory: maximum Backward
		* m_History.at(m_CurrentIndex) -> CWD -> where the user's view currently is located
		* m_History.back() -> Farthest point: maximum Forward
		* 
		* m_History.size() > 0 is always true! -> there is at least the DefaultDirectory *.at(0)
		*/
		size_t m_CurrentIndex = 0;
		std::vector<FilePath> m_History;
	};

}
