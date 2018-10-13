#pragma once

#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include "TaskMgr.h"

using namespace std;

static const vector<string> commandList =
{// OPERATION				OPCODE
#if 0
    "AddEmptyTask",			// 0
	"AddTask",				// 1
	"DeleteTask",			// 2
	"FreeTaskPool",			// 3
	"ShowTaskList",			// 4
    "AddGetPositionTask",	// 5
#elif 1
    "__placeholder",        // 0
    "__placeholder",        // 1
    "__placeholder",        // 2
    "__placeholder",        // 3
    "__placeholder",        // 4
    "__placeholder",        // 5
#endif
	"Help",					// 6
    "Exit",					// 7
	"Time",					// 8
	"GlobalTime",			// 9
    "Echo",                 // 10
    "DownloadFile",         // 11
    "UploadFile"
};

class CommandMgr
{
    static string rawCommand;
    static string command;
    static string arguments;
    static int opCode;
    static string result;
public:
    //static unsigned int chunkSize;

    static string GetAndParseCommand();
	static string GetAndParseCommand(string externalCmd);
    static string GetArgs();
    static int GetOpCode();
    static string GetOpResult();
    static string GetSubnetCommand(string);

	static void ExecuteCommand();
	static void ShowCmdList();

    // Compare user command and commands from list
    static string DetermineCommand();
    static int DetermineOpCode();
};

