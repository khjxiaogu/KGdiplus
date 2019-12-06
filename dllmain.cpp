// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include <windows.h>
#include"../ncbind.hpp"
#include "gdip.h"
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
ULONG token;
void startup() {
	auto gi=new Gdiplus::GdiplusStartupInput();
	Gdiplus::GdiplusStartup(&token, gi, NULL);
	delete gi;
}
void shutdown() {
	Gdiplus::GdiplusShutdown(token);
}

NCB_PRE_REGIST_CALLBACK(startup);
NCB_POST_UNREGIST_CALLBACK(shutdown);
