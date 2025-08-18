#include "C78EPCH.h"
#ifdef C78E_PLATFORM_WINDOWS
#include <C78E/Utils/PlatformUtils.h>
#include <C78E/Utils/stdutils.h> // Include the stdutils.h for std::join

#include <C78E/Core/Application/Application.h>

namespace C78E {
    static const char multiItemDelimiter = ';';

    std::wstring stringToWString(const std::string& str) {
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
        std::wstring wstrTo(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
        return wstrTo;
    }

    std::string winFileDialog(const FileDialogs::Filter& filter, const FilePath& baseDirectory, const FilePath& defaultFile, const std::string& dialogTitle, bool save, bool multiple) {
        std::string result;
        IFileDialog* pfd = nullptr;

        C78E_CORE_VALIDATE(SUCCEEDED(CoInitialize(nullptr)), return result, "winFileDialog: Failed to initialize COM library");
        C78E_CORE_VALIDATE(
            SUCCEEDED(CoCreateInstance(save ? CLSID_FileSaveDialog : CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd))),
            CoUninitialize(); return result,
            "winFileDialog: Failed to create file dialog instance"
        );

        DWORD options;
        pfd->GetOptions(&options);
        if (multiple && !save) {
            pfd->SetOptions(options | FOS_ALLOWMULTISELECT);
        }

        bool pickFolders = std::find(filter.begin(), filter.end(), FileSystem::EntryType::Directory) != filter.end();
        if (pickFolders) {
            pfd->SetOptions(options | FOS_PICKFOLDERS);
        }

        if (!baseDirectory.empty()) {
            IShellItem* psiFolder;
            std::wstring baseDirectoryW = stringToWString(baseDirectory.string());
            if (SUCCEEDED(SHCreateItemFromParsingName(baseDirectoryW.c_str(), nullptr, IID_PPV_ARGS(&psiFolder)))) {
                pfd->SetFolder(psiFolder);
                psiFolder->Release();
            }
        }

        if (!dialogTitle.empty()) {
            pfd->SetTitle(stringToWString(dialogTitle).c_str());
        }

        std::vector<COMDLG_FILTERSPEC> fileTypes;
        std::vector<std::wstring> filterStorage;
        if (!filter.empty() && !pickFolders) {
            filterStorage.reserve(filter.size() * 2);
            fileTypes.reserve(filter.size() * 2);
            for (const auto& entryType : filter) { // never Directory
                filterStorage.emplace_back(stringToWString(FileSystem::stringFromEntryType(entryType)));
                filterStorage.emplace_back(stringToWString(std::replace(std::join(FileSystem::extensionsFromEntryType(entryType), ";"), ".", "*.")));
                fileTypes.emplace_back(filterStorage[filterStorage.size() - 2].c_str(), filterStorage[filterStorage.size() - 1].c_str());
            }
            if (!fileTypes.empty()) {
                pfd->SetFileTypes(static_cast<UINT>(fileTypes.size()), fileTypes.data());
            }
        }

        if (SUCCEEDED(pfd->Show(nullptr))) {
            IShellItemArray* pItemArray = nullptr;
            if (IFileOpenDialog* pfdOpen = nullptr; SUCCEEDED(pfd->QueryInterface(IID_PPV_ARGS(&pfdOpen)))) {
                if (SUCCEEDED(pfdOpen->GetResults(&pItemArray))) {
                    DWORD itemCount;
                    pItemArray->GetCount(&itemCount);
                    for (DWORD i = 0; i < itemCount; ++i) {
                        IShellItem* pItem;
                        if (SUCCEEDED(pItemArray->GetItemAt(i, &pItem))) {
                            PWSTR pszFilePath;
                            if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath))) {
                                char filePath[MAX_PATH];
                                WideCharToMultiByte(CP_ACP, 0, pszFilePath, -1, filePath, MAX_PATH, nullptr, nullptr);
                                if (!result.empty()) {
                                    result += multiItemDelimiter;
                                }
                                result += filePath;
                                CoTaskMemFree(pszFilePath);
                            }
                            pItem->Release();
                        }
                    }
                    pItemArray->Release();
                }
                pfdOpen->Release();
            }
            else {
                IShellItem* pItem;
                if (SUCCEEDED(pfd->GetResult(&pItem))) {
                    PWSTR pszFilePath;
                    if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath))) {
                        char filePath[MAX_PATH];
                        WideCharToMultiByte(CP_ACP, 0, pszFilePath, -1, filePath, MAX_PATH, nullptr, nullptr);
                        result = filePath;
                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
        }
        pfd->Release();
        CoUninitialize();

        return result;
    }

    FilePath FileDialogs::openFile(const Filter& filter, C78E::FilePath baseDirectory, C78E::FilePath defaultFile, const std::string& dialogTitle) {
        return winFileDialog(filter, baseDirectory, defaultFile, dialogTitle, false, false);
    }

    std::vector<FilePath> FileDialogs::openFiles(const Filter& filter, C78E::FilePath baseDirectory, C78E::FilePath defaultFile, const std::string& dialogTitle) {
        std::string out = winFileDialog(filter, baseDirectory, defaultFile, dialogTitle, false, true);
        auto frags = std::split(out, std::string(1, multiItemDelimiter));
        std::vector<FilePath> paths;
        paths.reserve(frags.size());
        for (auto& frag : frags)
            paths.emplace_back(frag);
        return paths;
    }

    FilePath FileDialogs::saveFile(const Filter& filter, C78E::FilePath baseDirectory, C78E::FilePath defaultFile, const std::string& dialogTitle) {
        return winFileDialog(filter, baseDirectory, defaultFile, dialogTitle, true, false);
    }

    float Time::getTime() {
        return static_cast<float>(glfwGetTime());
    }

    System::Monitor System::getPrimaryMonitor() {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        C78E_CORE_ASSERT(monitor, "glfw Monitor is nullptr!");
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        return {
            static_cast<uint32_t>(mode->width),
            static_cast<uint32_t>(mode->height),
            static_cast<uint32_t>(mode->refreshRate),
            static_cast<uint32_t>(mode->redBits),
            static_cast<uint32_t>(mode->greenBits),
            static_cast<uint32_t>(mode->blueBits)
        };
    }

    FilePath System::getExecutionBinaryPath() {
        wchar_t path[1024] = { 0 };
        GetModuleFileNameW(NULL, path, 1024);
        return path;
    }

}

#endif // C78E_PLATFORM_WINDOWS

