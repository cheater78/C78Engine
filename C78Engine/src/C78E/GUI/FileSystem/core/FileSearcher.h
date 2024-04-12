#pragma once

#include <C78E/Core/FileSystem.h>

namespace C78E {

	/*
	* structure of a search directive: * arbituary string
	*		*filename*.*ext*
	*		e.g. *.*ce -> will return all C78E files
	*	note: "**" = "*"
	*/

	class FileSearcher {
	public:
		using Result = std::vector<FilePath>;
	public:
		static Result search(FilePath baseDirectory, const std::string& searchDirective, bool recursive = true, bool matchCase = false);
	public:
		FileSearcher() = delete;
		FileSearcher(const FileSearcher& other) = delete;
		~FileSearcher() = delete;
	private:
		using ResolvedSearchDirective = std::vector<std::string>;
		using InternalSearchDirective = const std::vector<std::string>&;
	private:
		static Result recurseSearch(FilePath baseDirectory, InternalSearchDirective searchDirective, bool matchCase = false);
		static bool match(FilePath file, InternalSearchDirective searchDirective, bool matchCase);

		static bool validateSearchDirective(const std::string& searchDirective);
	
	};

}