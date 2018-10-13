#pragma once

#include "boost/asio.hpp"
#include "boost/thread.hpp"
#include <iostream>

using namespace std;
using namespace boost::asio;

class NetworkMgr
{
    io_service service;
    ip::tcp::socket socket;
    bool isServer;

public:
    NetworkMgr();
    ~NetworkMgr();

    void CreateServer();
    void CreateClient();

    ip::tcp::socket* GetSocket()
    {

        return &socket;
    }

    void Read();
    void Write(string);
};

