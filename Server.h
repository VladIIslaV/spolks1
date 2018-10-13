#pragma once

#include "CommunicationInstance.h"
#include "FileInstance.h"
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <iostream>

using namespace std;
using namespace boost::asio;

class Server : public CommunicationInstance
{
public:
    static bool Connect();
    static void Execute();
};

inline bool Server::Connect()
{
#ifdef DEBUG
    cout << "\t\t" << __FUNCDNAME__ << endl;
#endif
    Endpoint_t ep(ip::tcp::v4(), 50000);
    ip::tcp::acceptor acc(service, ep);

    acc.accept(socket, err);
    if (err)
    {
        RepairConnection();
        return false;
    }

    string newIP = socket.remote_endpoint().address().to_string();

    cout << "IP_" << socket.remote_endpoint().address() << endl
        << "Port_" << socket.remote_endpoint().port() << endl;

    return true;
}

// Should run command and return the result
inline void Server::Execute()
{
#ifdef DEBUG
    cout << "\t\t" << __FUNCDNAME__ << endl;
#endif
    char* command = new char[chunkSize];
    bool isReadSuccessful = false;

    while (true)
    {
        isReadSuccessful = ReadCommand(command, chunkSize);
        if (isReadSuccessful)
        {
            CommandMgr::GetAndParseCommand(command);
            CommandMgr::ExecuteCommand();
        }
        else
        {
            Connect();
        }
    }

    delete[] command;
    command = nullptr;
}
