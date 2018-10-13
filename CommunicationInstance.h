#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include "boost/asio.hpp"
#include <boost/thread.hpp>
#include <chrono>
#include "FileInstance.h"

using namespace boost::asio;
using namespace std::chrono;
using namespace std;

const size_t Kb = 1024;
const size_t Mb = Kb * Kb;

const string border = to_string(0xFFFF);

const string cNoDataForTransfer = string("There is no_data for transfer");
const string cReadyToSendFile = string("I want to send file to you");
const string cNoFile = string("There is no such file");
const string cRemoveThisFile = string("Remove this file if it is exists");
const string cReadyToGetFile = string("I am ready to get file");
const string cGotSize = string("I got the file size");
const string cFinishDownloading = string("Finish downloading");

static const vector< pair<string, uint16_t> > networkCommandsList =
{ // Command             Command code
    {cNoDataForTransfer, 0 },
    {cReadyToSendFile,   1 },
    {cNoFile,            2 },
    {cRemoveThisFile,    3 },
    {cReadyToGetFile,    4 },
    {cFinishDownloading, 5 }
};

typedef ip::tcp::socket Socket_t;
typedef boost::system::error_code Error_t;
typedef ip::tcp::endpoint Endpoint_t;

enum class Role {
    Default = 0,
    Client,
    Server
};

class CommunicationInstance
{
protected:
    static io_service service;
    static Socket_t socket;
    static Role role;
    static size_t chunkSize;
    static Error_t err;

public:
    static Socket_t* GetSocket();
    static Role GetRole();
    static void SetRole(Role newRole);
    static void SetChunkSize(size_t newChunkSize);

    static void Write(string message);
    static void ReadAndShow();

    static size_t ReadSome(char *message, size_t size);
    static bool WriteSome(const char *message, size_t size);

    static size_t ReadNumber();
    static void WriteNumber(size_t number);

    static size_t ReadCommand(char *message, size_t size);

    static size_t GetFileSize(const char* fileName);
    static size_t GetFileSize(string fileName);

    static void ShowSpeed(uint32_t speedB);
    static void DrowSpeed(uint32_t speedB);

    static void DownloadFile(string fileName);
    static void UploadFile(string fileName);

    static void RepairConnection();

    static short GetOpCode(string message);
};

template<typename AnyNumber_t> char* PackNumber(AnyNumber_t number)
{
    return (char*)(&number);
}

template<typename AnyNumber_t> AnyNumber_t UnpackNumber(const char* message)
{
    return *(AnyNumber_t*)message;
}



inline Socket_t* CommunicationInstance::GetSocket()
{
    return &socket;
}

inline Role CommunicationInstance::GetRole()
{
    return role;
}

inline void CommunicationInstance::SetRole(Role newRole)
{
    role = newRole;
}

inline void CommunicationInstance::SetChunkSize(size_t newChunkSize)
{
    chunkSize = newChunkSize;
}

inline void CommunicationInstance::Write(string message)
{
#ifdef DEBUG
    cout << "\t\t" << __FUNCDNAME__ << endl;
#endif
    size_t msgSize = message.length();
    const char *tmpBuffer = message.c_str();

    while (msgSize)
    {
        size_t tmpChunkSize = (msgSize >= chunkSize) ? chunkSize : msgSize;
        WriteSome(tmpBuffer, tmpChunkSize);
        tmpBuffer += tmpChunkSize;
        msgSize -= tmpChunkSize;
    }
}

inline void CommunicationInstance::ReadAndShow()
{
#ifdef DEBUG
    cout << "\t\t" << __FUNCDNAME__ << endl;
#endif
    char *message = new char[chunkSize];

    ReadSome(message, chunkSize);
    cout << message << endl;

    delete[] message;
    message = nullptr;
}

inline size_t CommunicationInstance::ReadSome(char *message, size_t size)
{
#ifdef DEBUG
    cout << "\t\t" << __FUNCDNAME__ << endl;
#endif

    size_t msgSize = socket.read_some(buffer(message, size - 1), err);   // Leave 1 sign for '\0'
    if (err)    // 10054 - no connection with client
    {
        cout << err << endl;
        msgSize = 0;
    }
    else
    {
        // Speed calculating
        static high_resolution_clock::time_point prevT = high_resolution_clock::now();
        high_resolution_clock::time_point curT = high_resolution_clock::now();
        duration<double> dT = duration_cast<duration<double>>(curT - prevT);
        ShowSpeed(msgSize / (dT.count()));
        prevT = curT;
    }
    message[msgSize] = '\0';

#ifdef DEBUG
    cout << "\t\t" << __FUNCDNAME__ << "EXIT" << endl;
#endif

    return msgSize;
}

inline bool CommunicationInstance::WriteSome(const char* buf, size_t length)
{
#ifdef DEBUG
    cout << "\t\t" << __FUNCDNAME__ << endl;
#endif
    bool isWriteSuccessful = true;

    socket.write_some(buffer(buf, length), err);
    if (err)
    {
        cout << err << endl;
        isWriteSuccessful = false;
    }
    else    // Only for speed transfer calculating
    {
        static high_resolution_clock::time_point prevT = high_resolution_clock::now();
        high_resolution_clock::time_point curT = high_resolution_clock::now();
        duration<double> dT = duration_cast<duration<double>>(curT - prevT);
        ShowSpeed(length / dT.count());
        prevT = curT;
    }

    return isWriteSuccessful;
}

inline size_t CommunicationInstance::ReadNumber()
{
    char *strNum = new char[sizeof(size_t) + 1];
    ReadSome(strNum, sizeof(size_t) + 1);
    size_t num = UnpackNumber<size_t>(strNum);

    return num;
}

inline void CommunicationInstance::WriteNumber(size_t number)
{
    char* strNum = (char*)&number;
    Write(string(strNum, sizeof(size_t)));
}

inline size_t CommunicationInstance::ReadCommand(char *message, size_t size)
{
#ifdef DEBUG
    cout << "\t\t" << __FUNCDNAME__ << endl;
#endif
    size_t msgSize = socket.read_some(buffer(message, size - 1), err);   // Leave 1 sign for '\0'
    if (err)    // 10054 - no connection with client
    {
        cout << err << endl;
        msgSize = 0;
    }
    else
    {
        // Speed calculating
        static high_resolution_clock::time_point prevT = high_resolution_clock::now();
        high_resolution_clock::time_point curT = high_resolution_clock::now();
        duration<double> dT = duration_cast<duration<double>>(curT - prevT);
        ShowSpeed(msgSize / (dT.count()));
        prevT = curT;
    }
    message[msgSize] = '\0';

#ifdef DEBUG
    cout << "\t\t" << __FUNCDNAME__ << "EXIT" << endl;
#endif

    return msgSize;
}

inline size_t CommunicationInstance::GetFileSize(const char * fileName)
{
#ifdef DEBUG
    cout << "\t\t" << __FUNCDNAME__ << endl;
#endif
    ifstream file(fileName, ios::ate);
    size_t fileSize = file.tellg();
    file.close();
    return fileSize;
}

inline size_t CommunicationInstance::GetFileSize(string fileName)
{
    return GetFileSize(fileName.c_str());
}

inline void CommunicationInstance::ShowSpeed(uint32_t speedB)
{
#ifdef DEBUG
    cout << "\t\t" << __FUNCDNAME__ << endl;
#endif
    if (speedB > 0)
        if (speedB > Kb)
            if (speedB > Mb)
                cout << "Speed: " << speedB / Mb << "\tMB/s";
            else
                cout << "Speed: " << speedB / Kb << "\tKB/s";
        else
            cout << "Speed: " << speedB << "\tB/s";
    DrowSpeed(speedB);
}

inline void CommunicationInstance::DrowSpeed(uint32_t speedB)
{
#ifdef DEBUG
    cout << "\t\t" << __FUNCDNAME__ << endl;
#endif
    if (speedB > 0)
    {
        if (speedB > Kb)
        {
            if (speedB > Mb)
            {
                for (uint16_t i = 0; i < speedB / Mb; i += 50)   cout << ">>>";    //Mbyte speed part
                speedB %= Mb;
            }
            for (uint16_t i = 0; i < speedB / Kb; i += 200)   cout << ">>";    //Kbyte speed part
            speedB %= Kb;
        }
        for (uint16_t i = 0; i < speedB; i += 400)   cout << ">";    // Byte speed part
    }
    cout << endl;
}

inline void CommunicationInstance::RepairConnection()
{
#ifdef DEBUG
    cout << "\t\t" << __FUNCDNAME__ << endl;
#endif
    // Connection has been broken     
    //socket.release();
    socket.close();
}

inline short CommunicationInstance::GetOpCode(string message)
{
    short opCode = -1;
    for (auto i : networkCommandsList)
    {
        if (i.first == message)
        {
            opCode = i.second;
        }
    }

    return opCode;
}

