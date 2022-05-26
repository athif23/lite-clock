#pragma once
#include <string>
#include <fstream>
#include <cstdint>
#include <SDL.h>

#include "sdlStbFont.h"

/// Opens a file and returns a string of the raw data
void readFileRaw(const std::string& fullPath, std::string& output);
/// Opens a file and stores it in a stb_memory for automatic management
void readFileRaw_toMemory(const std::string& fullPath, sttfont_memory& mem);
