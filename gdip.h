/*
 *  gdip.h
 *  Gdiplus basic header
 *
 *  Copyright (C) 2019-2020 khjxiaogu
 *
 *  Author: khjxiaogu
 *  Web: https://www.khjxiaogu.com
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
*/
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