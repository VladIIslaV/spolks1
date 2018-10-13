#pragma once

#include "boost/asio.hpp"
#include <iostream>

using namespace std;
using namespace boost::asio;

class StaticNetworkMgr
{
public:

    static io_service service1;
    static ip::tcp::socket socket1;
    static bool isServer1;

    static void ConnectToClient();

    static void ForwardMsg();

    static ip::tcp::socket* GetSocket()
    {

        return &socket1;
    }
};

