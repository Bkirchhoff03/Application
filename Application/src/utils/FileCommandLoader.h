/*
 * FileCommandLoader.h
 *
 *  Created on: Jul 13, 2023
 *      Author: nebk2
 */

#ifndef UTILS_FILECOMMANDLOADER_H_
#define UTILS_FILECOMMANDLOADER_H_

#include <functional>
#include <vector>
#include <string>

class Color;
class Vec2D;

enum CommandType{
	COMMAND_ONE_LINE = 0,
	COMMAND_MULTI_LINE
};

struct ParseFuncParams{
	std::string line;
	size_t dilimitPos;
	uint32_t lineNum;
};

using ParseFunc = std::function<void(const ParseFuncParams& params)>;

struct Command{
	CommandType commandType = COMMAND_ONE_LINE;
	std::string command = "";
	ParseFunc parseFunc = nullptr;
};

class FileCommandLoader{
public:
	void addCommand(const Command& command);
	bool loadFile(const std::string& filePath);

	static Color readColor(const ParseFuncParams& params);
	static Vec2D readSize(const ParseFuncParams& params);
	static int readInt(const ParseFuncParams& params);
	static std::string readString(const ParseFuncParams& params);
	static char readChar(const ParseFuncParams& params);

private:
	std::vector<Command> mCommands;
};
#endif /* UTILS_FILECOMMANDLOADER_H_ */
