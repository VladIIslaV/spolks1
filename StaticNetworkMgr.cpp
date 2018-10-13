#include "StaticNetworkMgr.h"

io_service StaticNetworkMgr::service1;
ip::tcp::socket StaticNetworkMgr::socket1(service1);
bool isServer1 = true;


void StaticNetworkMgr::ConnectToClient()
{
#if _DEBUG
    cout << "\t\t" << __FUNCDNAME__ << endl;
#endif

    ip::tcp::endpoint ep(ip::tcp::v4(), 13);
    ip::tcp::acceptor acc(service1, ep);
    boost::system::error_code err;

    acc.accept(socket1, err);
    if (err) 
    {
        cout << "ERROR: line " << __LINE__ << endl;
        return;
    }
}

// Should run command and return the result
void StaticNetworkMgr::ForwardMsg()
{
#if _DEBUG
    cout << "\t\t" << __FUNCDNAME__ << endl;
#endif

    char buf[512];
    boost::system::error_code err;

    while (true)
    {
        socket1.read_some(buffer(buf, 512), err);
        if (err)
        {
            cout << err << endl;
            return;
        }
        string reply = string("SERVER: ") + string(buf);
        socket1.write_some(buffer(reply.c_str(), reply.length()), err);
        if (err)
        {
            cout << err << endl;
            return;
        }
        cout << buf << endl;
    }
}
