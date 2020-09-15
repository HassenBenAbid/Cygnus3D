#pragma once

#include <iostream>
#include <fstream>
#include <string>

namespace Cygnus3D {

	class filesU {
	public:
		std::string static readFiles(char const *filePath) {
			std::ifstream file;
			file.open(filePath, std::ios::in);

			if (!file) {
				std::cout << "UNABLE TO OPEN FILE" << std::endl;
			}

			file.seekg(0, file.end);
			unsigned long int length = file.tellg();
			char* data = new char[length + 1];
			memset(data, 0, length + 1);
			file.seekg(0, file.beg);
			file.read(data, length + 1);
			file.close();

			std::string s(data);
			delete[] data;

			return s;

		}
	};


}