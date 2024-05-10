#pragma once

#include <string>

namespace Engine3D {

	class FileDialogs{
	public:
		// @note filter is to specify when saving files to specific extension types.
		// @param returns string - which is the filepath.
		static std::string openFile(const char* filter);
		static std::string saveFile(const char* filter);
	};

}
