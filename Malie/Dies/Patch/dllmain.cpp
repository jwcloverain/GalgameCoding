// dllmain.cpp

#include "util.h"
#include "MemFile.h"


//=============================================================================
// Hook Code
//=============================================================================


int MemFile_getc(void* fp)
{
    auto file = static_cast<CMemFile*>(fp);
    unsigned char buf[4];
    if (file->Read(buf, 1) == 0)
        return -1;
    return buf[0];
}


int MemFile_read(void* fp, void* buffer, int size)
{
    auto file = static_cast<CMemFile*>(fp);
    return file->Read(buffer, size);
}


int MemFile_tell(void* fp)
{
    auto file = static_cast<CMemFile*>(fp);
    return file->Tell();
}


int MemFile_seek(void* fp, int offset, int origin)
{
    auto file = static_cast<CMemFile*>(fp);
    return file->Seek(offset, origin);
}


#include "res.h"


void* MemFile_open(const wchar_t* name, const wchar_t* args)
{
    if (_wcsicmp(name, L".\\data\\system\\exec.dat") == 0)
    {
        return new CMemFile(data_system_exec_dat, sizeof(data_system_exec_dat));
    }
    if (_wcsicmp(name, L".\\data\\extra\\glos.csv") == 0)
    {
        return new CMemFile(data_extra_glos_csv, sizeof(data_extra_glos_csv));
    }

    return NULL;
}


void MemFile_close(void* fp)
{
    auto file = static_cast<CMemFile*>(fp);
    delete file;
}


struct io_provider_t
{
    int (*pfn_getc)(void* fp);
    int (*pfn_putc)(void* fp, int c);
    int (*pfn_read)(void* fp, void* buffer, int size);
    int (*pfn_write)(void* fp, const void* buffer, int size);
    int (*pfn_tell)(void* fp);
    int (*pfn_seek)(void* fp, int offset, int origin);
    void* (*pfn_open)(const wchar_t* name, const wchar_t* args);
    void (*pfn_close)(void* fp);

    wchar_t name[34];
};


typedef void (*IOREGISTERPROC)(io_provider_t*);
auto IoRegister = (IOREGISTERPROC)0x780F80;


void RegMemFileIo()
{
    io_provider_t p;

    p.pfn_getc = MemFile_getc;
    p.pfn_putc = NULL;
    p.pfn_read = MemFile_read;
    p.pfn_write = NULL;
    p.pfn_tell = MemFile_tell;
    p.pfn_seek = MemFile_seek;
    p.pfn_open = MemFile_open;
    p.pfn_close = MemFile_close;

    wcscpy_s(p.name, L"MEMFILE_I");

    IoRegister(&p);
}


//=============================================================================
// Hook Code
//=============================================================================


auto pfnGetPrivateProfileStringW = GetPrivateProfileStringW;
DWORD WINAPI MineGetPrivateProfileStringW(LPCWSTR lpAppName, LPCWSTR lpKeyName, LPCWSTR lpDefault, LPWSTR lpReturnedString, DWORD nSize, LPCWSTR lpFileName)
{
    if (_wcsicmp(lpFileName, L".\\malie.ini") == 0)
    {
        if (_wcsicmp(lpAppName, L"SYSTEM") == 0)
        {
            if (_wcsicmp(lpKeyName, L"TITLE") == 0)
            {
                wcscpy_s(lpReturnedString, nSize, L"Dies irae ～Interview with Kaziklu Bey～ 中文版");
                return wcslen(lpReturnedString);
            }
        }
    }

    return pfnGetPrivateProfileStringW(lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize, lpFileName);
}


typedef PVOID (*IOOPENFILEPROC)(LPCWSTR lpFileName, LPCWSTR lpProvider);
auto pfnIoOpenFile = (IOOPENFILEPROC)0x7810E0;
PVOID IoOpenFile(LPCWSTR lpFileName, LPCWSTR lpProvider)
{
    //LogWrite(L"IoOpenFile: FileName=\"%s\" Provider=\"%s\"\r\n", lpFileName, lpProvider);

    if (lpFileName)
    {
        if (_wcsicmp(lpFileName, L".\\data\\system\\exec.dat") == 0 && _wcsicmp(lpProvider, L"FILE") == 0)
        {
            lpProvider = L"MEMFILE_I";
            goto end;
        }
        if (_wcsicmp(lpFileName, L".\\data\\font\\anticcezannepro.otf") == 0)
        {
            lpFileName = L".\\data\\font\\notoserifsc-regular.otf";
            lpProvider = L"LFILE_I";
            goto end;
        }
        if (_wcsicmp(lpFileName, L".\\data\\extra\\glos.csv") == 0 && _wcsicmp(lpProvider, L"MBC_I") == 0)
        {
            lpProvider = L"UTF8_I:MEMFILE_I";
            goto end;
        }
    }

end:
    return pfnIoOpenFile(lpFileName, lpProvider);
}


typedef VOID(*IOSETUPPROC)();
auto pfnIoSetup = (IOSETUPPROC)0x78107C;
VOID IoSetup()
{
    pfnIoSetup();
    RegMemFileIo();
}


//=============================================================================
// Startup Code
//=============================================================================


void InstallPatches()
{
    PatchWrite((PVOID)0x5B04C7, (BYTE)0xEB);

    PatchWrite((PVOID)(0x53AD48 + 1), (DWORD)L"文件(&F)");
    PatchWrite((PVOID)(0x564ADE + 1), (DWORD)L"文件(&F)");
    PatchWrite((PVOID)(0x565089 + 1), (DWORD)L"文件(&F)");
    PatchWrite((PVOID)(0x565FE0 + 1), (DWORD)L"文件(&F)");
    PatchWrite((PVOID)(0x5B09F8 + 1), (DWORD)L"文件(&F)");
    PatchWrite((PVOID)(0x62E044 + 1), (DWORD)L"文件(&F)");
    PatchWrite((PVOID)(0x5B096A + 1), (DWORD)L"快速存档\tF9");
    PatchWrite((PVOID)(0x5B097C + 1), (DWORD)L"快速读档\tF10");
    PatchWrite((PVOID)(0x564B48 + 1), (DWORD)L"快速存档数据(&Q)");
    PatchWrite((PVOID)(0x565287 + 1), (DWORD)L"快速存档数据(&Q)");
    PatchWrite((PVOID)(0x5B0997 + 1), (DWORD)L"快速存档数据(&Q)");
    PatchWrite((PVOID)(0x5B09A9 + 1), (DWORD)L"返回标题界面(T)\tT");
    PatchWrite((PVOID)(0x5B09BF + 1), (DWORD)L"最小化(M)\tF1");
    PatchWrite((PVOID)(0x5B09CF + 1), (DWORD)L"切换全屏/窗口模式(W)\tAlt+Enter");
    PatchWrite((PVOID)(0x5B09E5 + 1), (DWORD)L"退出(X)\tAlt+F4");
    PatchWrite((PVOID)(0x5B0A94 + 1), (DWORD)L"设置(&C)");
    PatchWrite((PVOID)(0x5B0A3F + 1), (DWORD)L"自动模式");
    PatchWrite((PVOID)(0x5B0A0D + 1), (DWORD)L"开");
    PatchWrite((PVOID)(0x5B0A1F + 1), (DWORD)L"关");
    PatchWrite((PVOID)(0x5B0A2F + 1), (DWORD)L"播放语音时显示台词"); // "ボイス再生時のセリフ表示"
    PatchWrite((PVOID)(0x5B0A72 + 1), (DWORD)L"跳过");
    PatchWrite((PVOID)(0x5B0A50 + 1), (DWORD)L"跳过已读部分");
    PatchWrite((PVOID)(0x5B0A62 + 1), (DWORD)L"跳过全部");
    PatchWrite((PVOID)(0x5B0A84 + 1), (DWORD)L"清除快速存档数据");
    PatchWrite((PVOID)(0x5B0ACA + 1), (DWORD)L"帮助(&H)");
    PatchWrite((PVOID)(0x5B0AA8 + 1), (DWORD)L"版本信息...");
    PatchWrite((PVOID)(0x5B0ABA + 1), (DWORD)L"查看快捷键...");
}


void InstallHooks()
{
    InlineHook(pfnGetPrivateProfileStringW, MineGetPrivateProfileStringW);
    InlineHook(pfnIoOpenFile, IoOpenFile);
    InlineHook(pfnIoSetup, IoSetup);
}


//=============================================================================
// DLL Entry Point
//=============================================================================


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        {
            // See https://github.com/microsoft/Detours/wiki/DetourRestoreAfterWith
            DetourRestoreAfterWith();

            // LogInit(L"dev.log");

            InstallPatches();
            InstallHooks();

            break;
        }
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}


//=============================================================================
// Dummy Export Symbol
//=============================================================================


BOOL APIENTRY CreateObject()
{
    return TRUE;
}
