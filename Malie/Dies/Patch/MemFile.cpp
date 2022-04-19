// MemFile.cpp

#include "MemFile.h"
#include <string.h>

CMemFile::CMemFile(const void* buffer, int size)
{
    _buffer = (unsigned char*)buffer;
    _size = size;
    _position = 0;
}

CMemFile::~CMemFile()
{
}

int CMemFile::Read(void* buffer, int size)
{
    if (size < 1)
        return 0;
    if (_position + size > _size)
        return 0;

    memcpy(buffer, &_buffer[_position], size);
    _position += size;

    return size;
}

int CMemFile::Tell()
{
    return _position;
}

int CMemFile::Seek(int offset, int origin)
{
    int target;

    if (origin == 1)
        target = _position + offset;
    else if (origin == 2)
        target = _size + offset;
    else
        target = offset;

    if (target < 0 || target > _size)
        return 1;

    _position = target;

    return 0;
}
