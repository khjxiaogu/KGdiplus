#pragma once
#pragma comment(linker, "\"/manifestdependency:type='Win32' name='Microsoft.Windows.GdiPlus' version='1.1.0.0' processorArchitecture='X86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#define GDIPVER     0x0110
#include <windows.h>
#include <windowsx.h>
#include <gdiplus.h>
#include <gdipluseffects.h>
#pragma comment(lib,"gdiplus.lib")//very important
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"Msimg32.lib")
#include<OleCtl.h>
#pragma comment(lib,"OleAut32.lib")
#define ARGB(a,r,g,b) ((int)(((BYTE)a)<<24)|(((BYTE)r)<<16)|(((BYTE)g)<<8)|((BYTE)b))