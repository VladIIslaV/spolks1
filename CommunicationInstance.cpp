#include "CommunicationInstance.h"
#include "FileInstance.h"
#include <queue>
#include <string>

using namespace std;

io_service CommunicationInstance::service;
Socket_t CommunicationInstance::socket(service);
Role CommunicationInstance::role = Role::Default;
size_t CommunicationInstance::chunkSize = 1 * Kb;
Error_t CommunicationInstance::err;

void CommunicationInstance::DownloadFile(string fileName)
{
#ifdef DEBUG
    cout << "\t\t" << __FUNCDNAME__ << endl;
#endif

    bool fileReceiving = true;
    FileInstance file;
    size_t fileSize = 1;
    char *message = new char[chunkSize + 1];

    while (fileReceiving)
    {   
        size_t msgSize = ReadSome(message, chunkSize);
        if (msgSize == 0)
        {
            return;
        }

        uint32_t cmdId = GetOpCode(message);

        cout << "CMDID_" << cmdId << endl;
        cout << "FS_" << fileSize << endl;

        switch (cmdId)
        {
        case 1: // cReadyToSendFile
            if (!file.is_open())
            {
                cout << "Open file" << endl;
                file.Open(fileName, ios::out | ios::ate | ios::app | ios::binary);
                if (!file)
                {
                    // ��������� ��� ���������
                    fileReceiving = false;
                }
                else
                {
                    size_t fS = file.GetSize();
                    cout << "FS_" << fS << endl;
                    Write(string((char*)(&fS), sizeof(fS)));
                    
                    char *strNum = new char[sizeof(size_t) + 1];
                    ReadSome(strNum, sizeof(size_t) + 1);
                    fileSize = *(size_t*)strNum;
                    
                    if (fS > fileSize)
                    {
                        fS = 0;
                        cout << "ERASE file" << endl;
                        file.close();
                        file.Open(fileName.c_str(), ios::out | ios::binary | ios::trunc);
                        Write(cGotSize);
                    }
                    else if(fS == fileSize)
                    {
                        fileReceiving = false;
                        Write(cFinishDownloading);
                        file.close();
                    }
                    else
                    {
                        fileSize -= fS;
                        cout << "PRESIZE_" << fileSize << endl;

                        Write(cGotSize);
                    }
                }
            }
            break;

        case 2: // cNoFile
            cout << cNoFile << endl;
            Write(cNoFile);
            break;

        default:    // Write data to the file
            file.write(message, msgSize);
            if (!file.good())
            {
                cout << "MSG_" << message << endl;
                cout << "xERROR" << endl;
                fileReceiving = false;
            }
            fileSize -= msgSize;
            if (fileSize == 0)
            {
                file.close();
                fileReceiving = false;
                Write(cFinishDownloading);
                cout << "File receiving = false" << endl;
            }
            break;
        }
    }
    delete[] message;
    message = nullptr;

    cout << "FILESIZE_SIZE_" << GetFileSize(fileName) << endl;
#ifdef DEBUG
    cout << "\t\t" << __FUNCDNAME__ << "Exit" << endl;
#endif
}


void CommunicationInstance::UploadFile(string fileName) //finish with read
{
#ifdef DEBUG
    cout << "\t\t" << __FUNCDNAME__ << endl;
#endif
    char *buffer = new char[chunkSize];
    
    FileInstance file(fileName, ios::in | ios::binary);
    if (!file)
    {
        Write(cNoFile);
        ReadSome(buffer, chunkSize);
        cout << cNoFile << endl;

        return;
    }
    
    file.ShowInfo();
    Write(cReadyToSendFile);        ///

    size_t filePos = ReadNumber();
    cout << "File_pos_" << filePos << endl;

    size_t size = file.GetSize();

    if (filePos == size)
    {
        WriteNumber(size);
        ReadSome(buffer, chunkSize);
        
        file.close();
        return;
    }
    
    if (filePos > size) // If file from the outside is broken
    {
        filePos = 0;
    }
    file.seekg(filePos, ios::beg);
    WriteNumber(size);
    
    ReadSome(buffer, chunkSize);

    
    size_t nextChunkSize = 0;
    do
    {
        nextChunkSize = file.GetNextChunk(buffer, chunkSize);   // read file
        bool isWriteSucceed = WriteSome(buffer, nextChunkSize); // write to the socket
        if (!isWriteSucceed)
        {
            cout << "Error: write was not successful" << endl;
            break;
        }
    } while (nextChunkSize);

    if (!nextChunkSize)  // file sent is successful
    {
        ReadSome(buffer, chunkSize);
    }


    delete[] buffer;
    buffer = nullptr;
    file.close();
}