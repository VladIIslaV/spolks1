#include <iostream>
#include <chrono>
#include <thread>
#include <stack>
#include <vector>
#include <boost/thread.hpp>

#include "TaskMgr.h"
#include "CommandMgr.h"
#include "time.h"
#include "Map.h"
#include "Client.h"
#include "Server.h"

using namespace std;

/*
    Show System and application configuration information
*/
void ConfigurationInfo(int argc, char** argv)
{
    // Target definition
#if _WIN32 | _WIN64
    cout << "Target:\tWINDOWS" << endl;
#elif unix
    cout << "Target:\tUNIX" << endl;
#endif  // unix

    // App parametrs parsing
    cout << "Role:\t";
    if (argc > 1)
    {
        switch (argv[1][1])
        {
        case 'c':
            CommunicationInstance::SetRole(Role::Client);
            cout << "CLIENT" << endl;
            break;
        case 's':
            CommunicationInstance::SetRole(Role::Server);
            cout << "SERVER" << endl;
            break;
        default:
            CommunicationInstance::SetRole(Role::Default);
            cout << "DEFAULT" << endl;
            break;
        }
    }
    else
    {
        cout << "DEFAULT" << endl;
    }
}

/*
    Command executor
*/
void Commander()
{

#ifdef DEBUG
    cout << "\t\t" << __FUNCDNAME__ << endl;
#endif

    while (1)
    {
        CommandMgr::GetAndParseCommand();
        CommandMgr::ExecuteCommand();
    }
}

/*
    Run any network role if needed
*/
void RunNetworkRole(int argc, char **argv)
{
    if (CommunicationInstance::GetRole() == Role::Client)
    {
        if(argc > 2)
            Client::Connect(argv[2]);
        else
            Client::Connect();

        Client::Execute();
    }
    else
    {
        if (CommunicationInstance::GetRole() == Role::Server)
        {
            Server::Connect();
            Server::Execute();
        }
        else
        {
            Commander();
            // DEFAULT ROLE
            // WOULD BE EXTENDED
        }
    }
}


/*******************************
        Main thread
********************************/
int main(int argc, char **argv) 
{
    CurrentTime();      // Initialize start time variable
    ConfigurationInfo(argc, argv);  // Show system and application configureation info
    
    RunNetworkRole(argc, argv);   // Run application role in acclordance with configuration

    return 0;
}
