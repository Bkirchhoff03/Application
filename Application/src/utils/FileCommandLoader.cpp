/*
 * FileCommandLoader.cpp
 *
 *  Created on: Jul 13, 2023
 *      Author: nebk2
 */
#include "FileCommandLoader.h"
#include <fstream>
#include <iostream>
#include "Color.h"
#include "Vec2D.h"
void FileCommandLoader::addCommand(const Command &command) {
	mCommands.push_back(command);
}
bool FileCommandLoader::loadFile(const std::string &filePath) {
	std::ifstream inFile;
	//std::cout << "Starting to load file:" << filePath << std::endl;
	inFile.open(filePath);

	std::string line = "";

	if (!inFile.is_open()) {
		std::cout << "Could not open the file: " << filePath << std::endl;
		return false;
	}
	while (!inFile.eof()) {
		//std::cout << "getting line, not at end of file" << std::endl;
		std::getline(inFile, line);
		std::cout << line << std::endl;
		size_t commandPos = std::string::npos;

		if ((commandPos = line.find(":")) != std::string::npos) {
			size_t dilimitPos = line.find_first_of(" ", commandPos);

			if (dilimitPos == std::string::npos) {
				dilimitPos = line.length();
			} else {
				dilimitPos -= 1;
			}

			std::string commandStr = line.substr(commandPos + 1, dilimitPos);
			dilimitPos += 1;

			for (size_t commandIndex = 0; commandIndex < mCommands.size(); ++commandIndex) {
				//std::cout << "commandIndex:" << commandIndex << std::endl;
				if (commandStr == mCommands[commandIndex].command) {
					if (mCommands[commandIndex].commandType == COMMAND_ONE_LINE) {
						ParseFuncParams params;
						params.dilimitPos = dilimitPos;
						params.lineNum = 0;
						params.line = line;
						mCommands[commandIndex].parseFunc(params);
					} else {
						std::string numLines = line.substr(dilimitPos + 1);
						int totalLines = std::stoi(numLines);
						int lineNum = 0;
						//std::cout << " Total: " << totalLines << std::endl;
						while (lineNum < totalLines) {
							//std::cout << "lineNum: " << lineNum << " Total: " << totalLines << std::endl;
							std::getline(inFile, line);

							if (line.empty()) {
								continue;
							} else {
								ParseFuncParams params;
								params.dilimitPos = 0;
								params.lineNum = lineNum;
								params.line = line;
								mCommands[commandIndex].parseFunc(params);
								++lineNum;
							}
						}
					}
				}
			}

		}
	}
	//std::cout << "Reached end of file" << std::endl;
	return true;
}

Color FileCommandLoader::readColor(const ParseFuncParams &params) {
	size_t nextSpacePos = params.line.find_first_of(" ", params.dilimitPos + 1);
	int r = std::stoi(params.line.substr(params.dilimitPos, (nextSpacePos - params.dilimitPos)));
	size_t lastSpacePos = nextSpacePos;
	nextSpacePos = params.line.find_first_of(" ", lastSpacePos + 1);
	int g = std::stoi(params.line.substr(lastSpacePos + 1, nextSpacePos - lastSpacePos));
	lastSpacePos = nextSpacePos;
	nextSpacePos = params.line.find_first_of(" ", lastSpacePos + 1);
	int b = std::stoi(params.line.substr(lastSpacePos + 1, nextSpacePos - lastSpacePos));
	int a = std::stoi(params.line.substr(nextSpacePos + 1));

	return Color(r, g, b, a);

}
Vec2D FileCommandLoader::readSize(const ParseFuncParams &params) {
	size_t nextSpacePos = params.line.find_first_of(" ", params.dilimitPos + 1);
	int width = std::stoi(params.line.substr(params.dilimitPos, nextSpacePos - params.dilimitPos));
	size_t lastSpacePos = nextSpacePos;
	nextSpacePos = params.line.find_first_of(" ", lastSpacePos + 1);
	int height = std::stoi(params.line.substr(lastSpacePos + 1, nextSpacePos - lastSpacePos));
	return Vec2D(width, height);

}
int FileCommandLoader::readInt(const ParseFuncParams &params) {
	std::string intStr = params.line.substr(params.dilimitPos);
	return std::stoi(intStr);
}
std::string FileCommandLoader::readString(const ParseFuncParams &params) {
	return params.line.substr(params.dilimitPos + 1);
}
char FileCommandLoader::readChar(const ParseFuncParams &params) {
	return params.line.substr(params.dilimitPos + 1)[0];
}

