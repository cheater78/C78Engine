#include "C78EPCH.h"
#include "FileSearcher.h"

namespace C78E {

	
	FileSearcher::Result FileSearcher::search(FilePath baseDirectory, const std::string& searchDirective, bool recursive, bool matchCase) {
		Result result = {};

		if (!validateSearchDirective(searchDirective)) return {};

		ResolvedSearchDirective directive = std::split(searchDirective, '*');

		for (auto& entry : std::filesystem::directory_iterator(baseDirectory)) {
			if (match(entry, directive, matchCase))
				result.push_back(entry);

			if (recursive && std::filesystem::is_directory(entry)) {
				Result subDirResult = recurseSearch(entry, directive, matchCase);
				result.insert(result.end(), subDirResult.begin(), subDirResult.end());
			}
		}
		return result;
	}

	FileSearcher::Result FileSearcher::recurseSearch(FilePath baseDirectory, InternalSearchDirective searchDirective, bool matchCase) {
		Result result = {};
		for (auto& entry : std::filesystem::directory_iterator(baseDirectory)) {
			if (match(entry, searchDirective, matchCase))
				result.push_back(entry);

			if (std::filesystem::is_directory(entry)) {
				Result subDirResult = recurseSearch(entry, searchDirective, matchCase);
				result.insert(result.end(), subDirResult.begin(), subDirResult.end());
			}
		}
		return result;
	}

	bool FileSearcher::match(FilePath file, InternalSearchDirective searchDirective, bool matchCase) {
		// BE AWARE: InternalSearchDirective searchDirective is a tuple of patterns,
		// which the filename must contain in the same order with arbituary strings in between them

		if (searchDirective.size() == 0) return true; // no SearchPattern -> matches all files

		const std::string filename = file.filename().string();

		// searchDirective starts with literal -> can't match if filename doesn't start with it
		if (!searchDirective.at(0).empty() && filename.find(searchDirective.at(0)) != 0) return false;
		
		// searchDirective starts with wildcard -> pos = 0; searchDirective starts with literal -> pos = literal.size();
		// in both cases we're at the next pos that needs checking
		size_t pos = searchDirective.at(0).size();

		for (size_t patternIndex = 1; patternIndex < searchDirective.size(); patternIndex++) {
			// get the current pattern that must be matched
			const std::string_view pattern = searchDirective.at(patternIndex);

			// directtive can contain "**", which is semantically nonsense, but no reason to deny the match, just skipping
			if (pattern.empty()) continue;

			// End of filename reached/ overstepped by 1(pos points to the first unchecked charachter in filename),
			// but there is still atleast 1 pattern that must be contained -> doesn't match
			if (pos >= filename.size()) return false;
			// throw away the alr checked part of the filename, since the (next)current pattern must be behind it
			const std::string_view filenameBack = filename.substr(pos, std::string::npos);
			
			// retrieve the next position of the current pattern inside of our leftover filename fragment
			size_t foundPatternIndex = std::findInString(filenameBack, pattern, matchCase);

			// the current pattern wasn't found inside of our leftover filename fragment
			if (foundPatternIndex == std::string::npos) return false;

			// pattern was found inside of our leftover filename fragment -> iterate
			// the successfully found pattern starts at foundPatternIndex,
			// which is relative to the beginning of our leftover filename fragment (NOT the full filename)
			// the next filename fragment starts AFTER our found pattern
			// pos contains the absolute offset into filename,
			// therefor it gets incremented by the wildcard length(foundPatternIndex) and the found pattern size
			// pos points to the first unchecked charachter in filename
			pos += foundPatternIndex + pattern.size();
		}

		return true;
	}

	bool FileSearcher::validateSearchDirective(const std::string& searchDirective) {
		static const char illegal[] = {'/', '\\', '%', '\"'};

		for (char c : illegal)
			if (searchDirective.find(c) != std::string::npos)
				return false;

		return true;
	}

}
