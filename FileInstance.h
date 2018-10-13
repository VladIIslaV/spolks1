#pragma once

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class FileInstance : public fstream
{
    string name;
    size_t size;
    size_t chunkSize;
    size_t lastPosition;
public:
    FileInstance();    
    FileInstance(string fileName, ios_base::openmode mode);
    ~FileInstance();

    void Open(string fileName, ios_base::openmode mode);

    size_t GetSize();
    static size_t GetSize(string fileName);

    void ShowInfo();

    //void SetChunkSize(size_t newChunkSize);
    //size_t GetChunkSize();
    //size_t SetNextChunk(char* buffer, size_t chunkSize);
    size_t GetNextChunk(char* buffer, size_t chunkSize);
};


inline size_t FileInstance::GetSize()
{
    return size;
}

inline size_t FileInstance::GetSize(string fileName)
{
    ifstream file(fileName, ios::ate);
    size_t fileSize = file.tellg();
    file.close();
    return fileSize;
}

inline void FileInstance::ShowInfo()
{
    cout << "FILENAME_" << name << endl;
    cout << "FILESIZE_" << size << endl;
}

//inline void FileInstance::SetChunkSize(size_t newChunkSize)
//{
//    chunkSize = newChunkSize;
//}
//
//inline size_t FileInstance::GetChunkSize()
//{
//    return chunkSize;
//}

inline size_t FileInstance::GetNextChunk(char* buffer, size_t chunkSize)
{
    size_t leftToTransfer = size - tellg();
    size_t g = tellg(); //////////////////////////////////////////
    if (leftToTransfer == (uint32_t)-1)
    {
        return 0;
    }
    
    size_t tmpChunkSize = (leftToTransfer < chunkSize) ? leftToTransfer : chunkSize;
    leftToTransfer -= tmpChunkSize;
    //cout << "POS_" << tellg() << endl;
    read(buffer, tmpChunkSize);
    return tmpChunkSize;
}
