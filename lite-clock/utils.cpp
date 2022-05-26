#include "utils.h"

/// Opens a file and returns a string of the raw data
void readFileRaw(const std::string& fullPath, std::string& output)
{
	std::ifstream fs(fullPath.c_str(), std::ios::in | std::ios::binary);

	if (!fs.is_open())
	{
		//	std::cout << "readFileRaw: " << fullPath << " -- " << "WARNING: Could not open file." << std::endl;
		return;
	}
	else
	{
		//		std::cout << "Opened! " << fullPath << std::endl;
	}

	fs.seekg(0, std::ios::end);
	const size_t LEN = fs.tellg();
	fs.seekg(0, std::ios::beg);

	output.resize(LEN);
	fs.read(&output[0], LEN);
	fs.close();
}

/// Opens a file and stores it in a stb_memory for automatic management
void readFileRaw_toMemory(const std::string& fullPath, sttfont_memory& mem)
{
	std::ifstream fs(fullPath.c_str(), std::ios::in | std::ios::binary);

	if (!fs.is_open())
	{
		//	std::cout << "readFileRaw: " << fullPath << " -- " << "WARNING: Could not open file." << std::endl;
		return;
	}
	else
	{
		// std::cout << "Opened! " << fullPath << std::endl;
	}

	fs.seekg(0, std::ios::end);
	const size_t LEN = fs.tellg();
	fs.seekg(0, std::ios::beg);

	mem.alloc(LEN);
	fs.read(mem.data, LEN);
	fs.close();
}
