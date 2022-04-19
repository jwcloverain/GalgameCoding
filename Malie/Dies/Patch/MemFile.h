// MemFile.h

#pragma once

class CMemFile
{
public:
    CMemFile(const void* buffer, int size);
    ~CMemFile();
    int Read(void* buffer, int size);
    int Tell();
    int Seek(int offset, int origin);

private:
    unsigned char* _buffer;
    int _size;
    int _position;
};
