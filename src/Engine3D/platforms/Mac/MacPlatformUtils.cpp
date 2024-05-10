#include <Engine3D/Engine3DPrecompiledHeader.h>
#include <Engine3D/platforms/PlatformUtils.h>
/* #include <commdlg.h> */
/* #include <GLFW/glfw3.h> */
/* #define GLFW_EXPOSE_NATIVE_WIN32 */
/* #include <GLFW/glfw3native.h> */
// Mac Version
#include <Engine3D/platforms/Mac/nfd.h>
#include <stdio.h>
#include <stdlib.h>

namespace Engine3D{
	// Windows way of doing the following operations below
	/* std::string FileDialogs::openFile(const char* filter){ */
	/* 	OPENFILENAMEA ofn; */
	/* 	CHAR szFo;e[260]; */
	/* 	ZeroMemory(&ofn, sizeof(OPENFILENAME)); */
	/* 	ofn.lStrucSize = sizeof(OPENFILENAME); */
	/* 	ofn.hwnOwer = glfwGerWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow()); */
	/* 	ofn.nMaxFile = sizeof(szFile); */
	/* 	ofn.lpppstrFilter = filter; */
	/* 	ofn.nFilterIndex = 1; */
	/* 	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR; */

	/* 	if(GetOpenFileNameA(&ofn) == TRUE){ */
	/* 		return ofn.lpstrFile; */
	/* 	} */

	/* 	return std::string(); */
	/* } */
	
	std::string FileDialogs::openFile(const char *filter){
		NFD_Init();

		char* outputPath; // @note is going to be the output filepath we will be returning
		nfdfilteritem_t filterItem[1] = { filter }; // Filtering for files with *.engine, and other sort of extensions that we can consider valid.
		nfdresult_t result = NFD_OpenDialog(&outputPath, filterItem, 0, NULL);


		if(result == NFD_ERROR){
			coreLogWarn("Could not open file\n");
			return std::string();
		}

		NFD_Quit();

		return outputPath;
	}

	std::string FileDialogs::saveFile(const char* filter){
		NFD_Init();

		char* savedFilepath;
		const char* savedFilepathName = "Untitled.engine"; // This is going to be the default name of the file we are saving data too.

		nfdfilteritem_t filterItem[1] = { filter };

		nfdresult_t result = NFD_SaveDialog(&savedFilepath, filterItem, 0, nullptr, savedFilepathName);

		if(result == NFD_ERROR){
			coreLogWarn("Could not save that file!");
			return std::string();
		}

		return savedFilepath;
	}

};
