#include "CommandMgr.h"
#include "Server.h"
#include "Client.h"
#include <boost/date_time.hpp>

using namespace std;

string CommandMgr::rawCommand;
string CommandMgr::command;
string CommandMgr::arguments;
int CommandMgr::opCode = 0;
string CommandMgr::result;


string CommandMgr::GetAndParseCommand()
{
	cout << ">>> ";

    // String to get command from the outside
    getline(cin, rawCommand);

    DetermineCommand();
    DetermineOpCode();

	return command;
}   

string CommandMgr::GetAndParseCommand(string rawExternalCmd)
{
    cout << ">>> ";

    // String to get command from the outside
    rawCommand = rawExternalCmd;

    DetermineCommand();
    DetermineOpCode();

    return command;
}

string CommandMgr::GetArgs()
{
    return arguments;
}

int CommandMgr::GetOpCode()
{
    return opCode;
}

string CommandMgr::GetOpResult()
{
    return result;
}

string CommandMgr::GetSubnetCommand(string message)
{
    if ((message.length() > 3) && (message.substr(0, 3) == string("net")))
    {
        return message.substr(3, message.length() - 3);
    }
    else
    {
        return message;
    }
}

int CommandMgr::DetermineOpCode()
{
	if (command == string())
	{
		opCode = -1;
	}
    for (int i = 0; i < commandList.size(); i++)
    {
        if (command == commandList[i]) 
        {
            opCode = i;
        }
	}
	return opCode;
}

void CommandMgr::ExecuteCommand()
{
    result = cNoDataForTransfer;

    if ((CommunicationInstance::GetRole() == Role::Client) && 
        (CommandMgr::rawCommand.substr(0, 3) == string("net")))
    {
        CommunicationInstance::Write(GetSubnetCommand(rawCommand));

        // There is spetial switch for network client with server communication
        switch (opCode)
        {
        case 7:	//Finish execution
            break;
        case 6:     //Help
        case 8:	    //Get current time
        case 9:     //Get global time
        case 10:    //Echo
            Client::ReadAndShow();
            break;
        case 11:    //Download file
            Client::DownloadFile(arguments);
            break;
        case 12:    //Upload file
        {
            char* tmp = new char[1024];
            Client::ReadSome(tmp, 1024);
            delete[] tmp;

            Client::UploadFile(arguments);
        }    
        break;
        default:
            cout << endl << "Warning: Command doesn't recognized" << endl;
            //Client::ReadAndShow();
            break;
        }
    }
    else
    {
        switch (opCode)
        {
        case 6:	//Help
            CommandMgr::ShowCmdList();
            if (CommunicationInstance::GetRole() == Role::Server)
            {
                CommunicationInstance::Write(string("Type just \"Help\", please.\n"));
            }
            break;
        case 7:	//Finish execution
            exit(0);
            break;
        case 8:	//Get current time
            if (CommunicationInstance::GetRole() == Role::Server)
            {
                result = to_string(CurrentTime()) + string("s");
                CommunicationInstance::Write(CommandMgr::GetOpResult());
            }
            else
            {
                cout << CurrentTime() << "s" << endl;
            }
            break;
        case 9: //Get global time
            if (CommunicationInstance::GetRole() == Role::Server)
            {
                boost::posix_time::ptime timeLocal =
                    boost::posix_time::second_clock::local_time();
                result = boost::posix_time::to_simple_string(timeLocal);
                CommunicationInstance::Write(CommandMgr::GetOpResult());
            }
            else
            {
                boost::posix_time::ptime timeLocal =
                    boost::posix_time::second_clock::local_time();
                cout << "Time " << timeLocal << endl;
            }
            break;
        case 10:    //Echo
            if (CommunicationInstance::GetRole() == Role::Server)
            {
                result = arguments;
                CommunicationInstance::Write(CommandMgr::GetOpResult());
            }
            else
            {
                cout << arguments << endl;
            }
            break;
        case 11:    //Download file
            if (CommunicationInstance::GetRole() == Role::Server)
            {
                CommunicationInstance::UploadFile(arguments);
            }
            break;
        case 12:    // Upload file
            if (CommunicationInstance::GetRole() == Role::Server)
            {
                CommunicationInstance::Write(cNoDataForTransfer);
                CommunicationInstance::DownloadFile(arguments);
            }
            break;
        default:
            cout << endl << "Warning: Command doesn't recognized" << endl;
        }
        
        // Made to show command result on the Server
        if (string(result) != cNoDataForTransfer)
        {
            cout << result << endl;
        }
    }
}

void CommandMgr::ShowCmdList()
{
	cout  << endl << "\tCOMMAND LIST:" << endl;
	for (string command : commandList)
	{
		cout << command << endl;
	}
}

string CommandMgr::DetermineCommand()
{
    // Create stream to parse command
    stringstream userStream(GetSubnetCommand(rawCommand));

    // Command word
    string userCmd;
    userStream >> userCmd;

    // Other words - arguments
    // Get args to enother variable
    if (!userStream.eof())
    {
        getline(userStream, arguments);
        // Remove first sign in the stream
        copy(arguments.begin() + 1, arguments.end(), arguments.begin());
        arguments.resize(arguments.size() - 1);
    }
    
    // Number of commands that similar to user's command
    int numOfEqCommands = 0;

    // Transform user userCmd to lower case
    transform(userCmd.begin(), userCmd.end(), userCmd.begin(), ::tolower);
    for (string cmdFromList : commandList)
    {
        string lowCmdFromList(cmdFromList);
        // Transform commands from list to the lower case
        transform(cmdFromList.cbegin(), cmdFromList.cend(), lowCmdFromList.begin(), ::tolower);
        lowCmdFromList.resize(userCmd.size());

        if (userCmd.compare(lowCmdFromList) == 0)
        {
            cout << cmdFromList << endl;
            command = cmdFromList;
            numOfEqCommands++;
        }
    }
    if (numOfEqCommands != 1)
        command = string();

    return command;
}

