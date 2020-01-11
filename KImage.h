/*
 *  KImage.h
 *  Provides Gdiplus Bitmap main implemention
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
#include "gdip.h"
#define USEISTREAM

#include "tp_stub.h"
#include "ncbind.hpp"
#include "KCLCommons.h"
#include "LayerEx.h"
#include "propmacros.h"
#define CHECK_OPENED if (!self->gi)TVPThrowExceptionMessage(L"not opened")
#define CHECK_OPENED_N if (!gi)TVPThrowExceptionMessage(L"not opened")

class KImage
{
	typedef KImage THISTYPE;
	Gdiplus::Bitmap* gi=NULL;
	iTJSDispatch2* objthis;
	HICON hr=NULL;
	KImage(iTJSDispatch2 * objthis):objthis(objthis) {}
public:
	DEF_FUNCTION  Factory(FACTORY_ARGS);
	void close();
	DEF_FUNCTION open(INTF_ARGS);
	DEF_FUNCTION openImage(INTF_ARGS);
	DEF_FUNCTION openFile(INTF_ARGS);
	DEF_FUNCTION openFileEx(INTF_ARGS);
	DEF_FUNCTION openLayer(INTF_ARGS);
	DEF_FUNCTION openMainSurface(INTF_ARGS);
	DEF_FUNCTION openResource(INTF_ARGS);
	DEF_FUNCTION allocate(INTF_ARGS);
	DEF_FUNCTION resize(INTF_ARGS);
	DEF_FUNCTION cut(INTF_ARGS);
	DEF_FUNCTION getIconEx(INTF_ARGS);
	DEF_FUNCTION getIcon(INTF_ARGS);
	DEF_FUNCTION setPixel(INTF_ARGS);
	DEF_FUNCTION getPixel(INTF_ARGS);
	DEF_FUNCTION setResolution(INTF_ARGS);
	DEF_FUNCTION drawGraph(INTF_ARGS);
	DEF_FUNCTION doBlur(INTF_ARGS);
	DEF_FUNCTION doSharpen(INTF_ARGS);
	DEF_FUNCTION doTint(INTF_ARGS);
	DEF_FUNCTION doRedEyeCorrection(INTF_ARGS);
	DEF_FUNCTION doMatrix(INTF_ARGS);
	DEF_FUNCTION doColorLUT(INTF_ARGS);
	DEF_FUNCTION doHueSaturationLightness(INTF_ARGS);
	DEF_FUNCTION doColorBalance(INTF_ARGS);
	DEF_FUNCTION doLevels(INTF_ARGS);
	DEF_FUNCTION doColorCurve(INTF_ARGS);
	int getopened();
	int getwidth();
	int getheight();
	double gethdpi();
	double getvdpi();
	tTVInteger getBitmap();
	tTVInteger getImage();
};
CLASS(KImage) {
	CONSTRUCTOR;
	FUNCTION(open);
	METHOD(close);
	FUNCTION(openImage);
	FUNCTION(openFile);
	FUNCTION(openFileEx);
	FUNCTION(openLayer);
	FUNCTION(openMainSurface);
	FUNCTION(openResource);
	FUNCTION(allocate);
	FUNCTION(resize);
	FUNCTION(cut);
	FUNCTION(getIcon);
	FUNCTION(getIconEx);
	FUNCTION(setPixel);
	FUNCTION(getPixel);
	FUNCTION(setResolution);
	FUNCTION(drawGraph);
	RawCallback(L"drawTo", &Class::drawGraph, 0);
	FUNCTION(doBlur);
	FUNCTION(doSharpen);
	FUNCTION(doTint);
	FUNCTION(doRedEyeCorrection);
	FUNCTION(doMatrix);
	FUNCTION(doColorLUT);
	FUNCTION(doHueSaturationLightness);
	FUNCTION(doColorBalance);
	FUNCTION(doLevels);
	FUNCTION(doColorCurve);
	PROPERTY_RO(opened);
	PROPERTY_RO(width);
	PROPERTY_RO(height);
	PROPERTY_RO(hdpi);
	PROPERTY_RO(vdpi);
	PROPERTY_RO(Bitmap);
	PROPERTY_RO(Image);
}