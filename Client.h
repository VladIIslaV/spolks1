#pragma once

#include "CommandMgr.h"
#include "CommunicationInstance.h"
#include "boost/asio.hpp"
#include <fstream>
#include <iostream>

using namespace std;
using namespace boost::asio;

class Client : public CommunicationInstance
{
public:
    static void Connect(const char* ipAddress = "192.168.100.3", const uint16_t port = 50000);
    static void Execute();
};

inline void Client::Connect(const char* ipAddress, const uint16_t port)
{
    cout << "Client::Connect" << endl;
#ifdef DEBUG
    cout << "\t\t" << __FUNCDNAME__ << endl;
#endif
    Endpoint_t endpoint(ip::address::from_string(ipAddress), port);

    // Loop while server is not connected
    do
    {
        socket.connect(endpoint, err);
        if (err.value() == 10061)   // Server is not connected yet
        {
            cout << "\t<NAK>";
        }
        else
        {
            //cout << err.value() << endl;
            cout << err.message() << endl;
        }
    } while (err);/*
    cout << "My endpoint port = " << endpoint.port() << endl;*/
}

inline void Client::Execute()
{
#ifdef DEBUG
    cout << "\t\t" << __FUNCDNAME__ << endl;
#endif

    while (true)
    {
        CommandMgr::GetAndParseCommand();
        CommandMgr::ExecuteCommand();
    }
}
