#include "FileInstance.h"
#include "CommunicationInstance.h"

//string FileInstance::lastFileName;

FileInstance::FileInstance() : 
    fstream()
{
}

FileInstance::FileInstance(string fileName, ios_base::openmode mode) :
    fstream(fileName.c_str(), mode),
    name(fileName)
{
    size = GetSize(fileName);
}


FileInstance::~FileInstance()
{
}

void FileInstance::Open(string fileName, ios_base::openmode mode)
{
    name = fileName;
    size = GetSize(fileName);    
    open(fileName.c_str(), mode);
    if (!good())
    {
        cerr << "Error: " << strerror(errno);
    }
}

