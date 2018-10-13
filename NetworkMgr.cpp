#include "NetworkMgr.h"
#include <iostream>
#include <thread>

using namespace std;


NetworkMgr::NetworkMgr():
    socket(ip::tcp::socket(service)),
    isServer(false)
{
    service.run();
}

NetworkMgr::~NetworkMgr()
{
}

void NetworkMgr::CreateServer()
{
    cout << "SERVER" << endl;
    isServer = true;
    ip::tcp::endpoint ep(ip::tcp::v4(), 13);

    ip::tcp::acceptor acc(service, ep);

    boost::system::error_code err;

    //while (true)
    {
        ip::tcp::socket tmpSocket(service);
        acc.accept(tmpSocket, err);
        if (err)
        {
            cout << err << endl;
            return;
        }
        cout << "Connected to client" << endl;
    //    break;
    }
    Write(string("Hello from Server"));
    //socket.write_some(buffer("Hello from Server\n"));
    //thread serverRead(Readd);
    //service.run();
    //if(!err)
    //    cout << err << endl;
}


// TODO make error handling
// Make normal class
// If need make separate classes for client and server
void NetworkMgr::CreateClient()
{
    cout << "CLIENT" << endl;
    isServer = false;

    ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 13);

    boost::system::error_code err;
    socket.connect(ep, err);
    if (err)
    {
        cout << err << endl;
        return;
    }
    Read();
    //char buf[512];
    //socket.read_some(buffer(buf));
    //cout << buf;

    //Read();
}


void NetworkMgr::Read()
{
    cout << "\t\t" << __FUNCTION__ << endl;

    char message[512];
    while (true)
    {
        boost::system::error_code err;
        socket.read_some(buffer(message), err);
        if (!err)
        {
            cout << "Error: " << err << endl;
            break;
        }
        cout << message << endl;
    }
}

void NetworkMgr::Write(string message)
{
    cout << "\t\t" << __FUNCTION__ << endl;

    while (true)
    {
        message = string("Hell!!!");
        boost::system::error_code err;
        write(socket, buffer(message.c_str(), message.length()));
        if (!err)
        {
            cout << "Error: " << err << endl;
            break;
        }
    }
}
