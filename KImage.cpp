/*
 *  KImage.cpp
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
#include "KImage.h"

IMPL_FUNCTION KImage::Factory(FACTORY_ARGS) {
	if (inst) {
		auto self = *inst = new KImage(objthis);
	}
	RET_OK;
}

void KImage::close() {
	if (gi) {
		delete gi;
		gi = NULL;
	}
	if (hr) {
		DWORD dw;
		if (GetHandleInformation(hr, &dw))
			DestroyIcon(hr);
	}
}

IMPL_FUNCTION KImage::open(INTF_ARGS) {
	NOT_STATIC;
	CHECK_PARAM_COUNT(1);
	auto oct = p[0]->AsOctetNoAddRef();
	auto cld = AllocCLD(oct->GetLength());
	memcpy(cld->addr, oct->GetData(), cld->size);
	int uicm = 0;
	if (n > 1)
		uicm = p[1]->AsInteger();
	self->close();
	self->gi = new Gdiplus::Bitmap(packStream(cld), uicm);
	RET_OK;
}

IMPL_FUNCTION KImage::openImage(INTF_ARGS) {
	NOT_STATIC;
	CHECK_PARAM_COUNT(1);
	auto obj = p[0]->AsObjectNoAddRef();
	self->close();
	self->gi = ((Gdiplus::Bitmap*)(tTVInteger)getMember(obj, L"Bitmap"))->Clone(0, 0, getMember(obj, L"width"), getMember(obj, L"height"), PixelFormat32bppARGB);
	RET_OK;
}

IMPL_FUNCTION KImage::openFile(INTF_ARGS) {
	NOT_STATIC;
	CHECK_PARAM_COUNT(1);
	ttstr fn(p[0]);
	int uicm = 0;
	if (n > 1)
		uicm = p[1]->AsInteger();
	fn = TVPNormalizeStorageName(fn);
	TVPGetLocalName(fn);
	self->close();
	self->gi = new Gdiplus::Bitmap(fn.c_str(), uicm);
	RET_OK;
}

IMPL_FUNCTION KImage::openFileEx(INTF_ARGS) {
	NOT_STATIC;
	CHECK_PARAM_COUNT(1);
	ttstr fn(p[0]);
	int uicm = 0;
	if (n > 1)
		uicm = p[1]->AsInteger();
	self->close();
	self->gi = new Gdiplus::Bitmap(TVPCreateIStream(fn, TJS_BS_READ), uicm);
	RET_OK;
}

IMPL_FUNCTION KImage::openLayer(INTF_ARGS) {
	NOT_STATIC;
	CHECK_PARAM_COUNT(1);
	auto layer = p[0]->AsObjectNoAddRef();
	if (!layer->IsInstanceOf(NULL, NULL, NULL, L"Layer", layer))
		return TJS_E_INVALIDPARAM;
	LayerInfo li;
	getLayerInfo(&li, layer);
	self->close();
	auto agi = new Gdiplus::Bitmap(li.width, li.height, li.pitch, (Gdiplus::PixelFormat)PixelFormat32bppARGB, (BYTE*)li.scan);
	self->gi = agi->Clone(0, 0, li.width, li.height, PixelFormat32bppARGB);
	delete agi;
	RET_OK;
}

IMPL_FUNCTION KImage::openMainSurface(INTF_ARGS) {
	NOT_STATIC;
	self->gi = Gdiplus::Bitmap::FromDirectDrawSurface7((IDirectDrawSurface7*)TVPGetDDPrimarySurfaceNoAddRef());
	RET_OK;
}

IMPL_FUNCTION KImage::openResource(INTF_ARGS) {
	NOT_STATIC;
	CHECK_PARAM_COUNT(1);

	HINSTANCE dll = LoadLibraryW(ttstr(p[0]).c_str());
	if (!dll)
		return TJS_E_INVALIDPARAM;
	self->close();
	self->gi = Gdiplus::Bitmap::FromResource(dll, ttstr(p[1]).c_str());
	FreeLibrary(dll);
	RET_OK;
}

IMPL_FUNCTION KImage::allocate(INTF_ARGS) {
	NOT_STATIC;
	CHECK_PARAM_COUNT(2);
	self->close();
	self->gi = new Gdiplus::Bitmap(p[0]->AsInteger(), p[1]->AsInteger(), PixelFormat32bppARGB);
	RET_OK;
}

IMPL_FUNCTION KImage::resize(INTF_ARGS) {
	NOT_STATIC;
	CHECK_PARAM_COUNT(2);
	CHECK_OPENED;
	auto ogi = self->gi;
	self->gi = new Gdiplus::Bitmap(p[0]->AsInteger(), p[1]->AsInteger());
	Gdiplus::Graphics gp(self->gi);
	gp.DrawImage(ogi, 0, 0, p[0]->AsInteger(), p[1]->AsInteger());
	delete ogi;
	RET_OK;
}

IMPL_FUNCTION KImage::cut(INTF_ARGS) {
	NOT_STATIC;
	CHECK_PARAM_COUNT(2);
	CHECK_OPENED;
	auto ogi = self->gi;
	//self->gi = new Gdiplus::Bitmap(p[0]->AsInteger(), p[1]->AsInteger());
	self->gi = ogi->Clone((int)p[0]->AsInteger(), p[1]->AsInteger(), p[2]->AsInteger(), p[3]->AsInteger(), PixelFormat32bppARGB);
	delete ogi;
	RET_OK;
}

IMPL_FUNCTION KImage::getIconEx(INTF_ARGS) {
	NOT_STATIC;
	CHECK_OPENED;
	if (self->hr) {
		DWORD dw;
		if (GetHandleInformation(self->hr, &dw)){
			*r = self->hr;
			RET_OK;
		}
	}
	if (!self->gi->GetHICON(&self->hr)) {
		*r = self->hr;
		RET_OK;
	}
	else
		return TJS_E_FAIL;
}

IMPL_FUNCTION KImage::getIcon(INTF_ARGS) {
	NOT_STATIC;
	CHECK_OPENED;
	if (self->hr) {
		DWORD dw;
		if (GetHandleInformation(self->hr, &dw))
			DestroyIcon(self->hr);
	}
	if (!self->gi->GetHICON(&self->hr)) {
		*r = self->hr;
		RET_OK;
	}
	else
		return TJS_E_FAIL;
}

IMPL_FUNCTION KImage::setPixel(INTF_ARGS) {
	NOT_STATIC;
	CHECK_PARAM_COUNT(3);
	CHECK_OPENED;
	self->gi->SetPixel(p[0]->AsInteger(), p[1]->AsInteger(), p[2]->AsInteger());
	RET_OK;
}

IMPL_FUNCTION KImage::getPixel(INTF_ARGS) {
	NOT_STATIC;
	CHECK_PARAM_COUNT(2);
	CHECK_OPENED;
	Gdiplus::Color clr;
	self->gi->GetPixel(p[0]->AsInteger(), p[1]->AsInteger(), &clr);
	*r = (int)clr.GetValue();
	RET_OK;
}

IMPL_FUNCTION KImage::setResolution(INTF_ARGS) {
	NOT_STATIC;
	CHECK_PARAM_COUNT(2);
	CHECK_OPENED;
	self->gi->SetResolution(p[0]->AsReal(), p[1]->AsReal());
	RET_OK;
}

IMPL_FUNCTION KImage::drawGraph(INTF_ARGS) {
	NOT_STATIC;
	CHECK_PARAM_COUNT(1);
	CHECK_OPENED;
	auto layer = p[0]->AsObjectNoAddRef();
	if (!layer->IsInstanceOf(NULL, NULL, NULL, L"Layer", layer))
		return TJS_E_INVALIDPARAM;
	LayerInfo li;
	getLayerInfo(&li, layer);
	Gdiplus::Bitmap bm(li.width, li.height, li.pitch, PixelFormat32bppARGB, (BYTE*)li.scan);
	Gdiplus::Graphics la(&bm);
	if (n == 1)
		la.DrawImage(self->gi, 0, 0, self->gi->GetWidth(), self->gi->GetHeight());
	else if (n == 3)
		la.DrawImage(self->gi, (int)p[1]->AsInteger(), p[2]->AsInteger(), self->gi->GetWidth(), self->gi->GetHeight());
	else if (n == 5)
		la.DrawImage(self->gi, (int)p[1]->AsInteger(), p[2]->AsInteger(), p[3]->AsInteger(), p[4]->AsInteger());
	else
		return TJS_E_BADPARAMCOUNT;
	layer->FuncCall(TJS_MEMBERENSURE, L"update", NULL, NULL, 0, NULL, layer);
	RET_OK;
}

IMPL_FUNCTION KImage::doBlur(INTF_ARGS) {
	NOT_STATIC;
	CHECK_PARAM_COUNT(5);
	CHECK_OPENED;
	RECT rct = { 0 };
	Gdiplus::Blur eff;
	Gdiplus::BlurParams bp = { 0 };

	rct.left = p[0]->AsInteger();
	rct.top = p[1]->AsInteger();
	rct.right = rct.left + p[3]->AsInteger();
	rct.bottom = rct.top + p[4]->AsInteger();
	bp.radius = p[4]->AsInteger();
	if (n > 5)
		bp.expandEdge = p[5]->AsInteger();
	eff.SetParameters(&bp);
	self->gi->ApplyEffect(&eff, &rct);
	RET_OK;
}

IMPL_FUNCTION KImage::doSharpen(INTF_ARGS) {
	NOT_STATIC;
	CHECK_PARAM_COUNT(6);
	CHECK_OPENED;
	RECT rct = { 0 };
	Gdiplus::Sharpen eff;
	Gdiplus::SharpenParams bp = { 0 };
	rct.left = p[0]->AsInteger();
	rct.top = p[1]->AsInteger();
	rct.right = rct.left + p[2]->AsInteger();
	rct.bottom = rct.top + p[3]->AsInteger();
	bp.radius = p[4]->AsInteger();
	bp.amount = p[5]->AsInteger();
	eff.SetParameters(&bp);
	self->gi->ApplyEffect(&eff, &rct);
	RET_OK;
}

IMPL_FUNCTION KImage::doTint(INTF_ARGS) {
	NOT_STATIC;
	CHECK_PARAM_COUNT(6);
	CHECK_OPENED;
	RECT rct = { 0 };
	Gdiplus::Tint eff;
	Gdiplus::TintParams bp = { 0 };
	rct.left = p[0]->AsInteger();
	rct.top = p[1]->AsInteger();
	rct.right = rct.left + p[2]->AsInteger();
	rct.bottom = rct.top + p[3]->AsInteger();
	bp.hue = p[4]->AsInteger();
	bp.amount = p[5]->AsInteger();
	eff.SetParameters(&bp);
	self->gi->ApplyEffect(&eff, &rct);
	RET_OK;
}
inline auto ArrayCount(iTJSDispatch2* dsp) {
	return TJSGetArrayElementCount(dsp);
}
IMPL_FUNCTION KImage::doRedEyeCorrection(INTF_ARGS) {
	NOT_STATIC;
	CHECK_PARAM_COUNT(5);
	CHECK_OPENED;
	RECT rct = { 0 };
	Gdiplus::RedEyeCorrection eff;
	Gdiplus::RedEyeCorrectionParams bp = { 0 };
	rct.left = p[0]->AsInteger();
	rct.top = p[1]->AsInteger();
	rct.right = rct.left + p[2]->AsInteger();
	rct.bottom = rct.top + p[3]->AsInteger();
	auto arr = p[4]->AsObjectNoAddRef();
	bp.numberOfAreas = ArrayCount(arr);
	if (bp.numberOfAreas>0) {
		bp.areas = new RECT[bp.numberOfAreas];
		for (int i = 0; i < bp.numberOfAreas; i++) {
			auto va = getMember(arr, i);
			if (va.Type() != tvtObject) { delete[](bp.areas + i); break; }
			auto ob = va.AsObjectNoAddRef();
			bp.areas[i].left = (int)getMember(ob, L"x");
			bp.areas[i].top = (int)getMember(ob, L"y");
			bp.areas[i].right = bp.areas[i].left + (int)getMember(ob, L"w");;
			bp.areas[i].bottom = bp.areas[i].top + (int)getMember(ob, L"h");;
		}
	}
	eff.SetParameters(&bp);
	self->gi->ApplyEffect(&eff, &rct);
	delete[] bp.areas;
	RET_OK;
}

IMPL_FUNCTION KImage::doMatrix(INTF_ARGS) {
	NOT_STATIC;
	CHECK_PARAM_COUNT(5);
	CHECK_OPENED;
	RECT rct = { 0 };
	Gdiplus::ColorMatrixEffect eff;
	Gdiplus::ColorMatrix bp = { 0 };
	rct.left = p[0]->AsInteger();
	rct.top = p[1]->AsInteger();
	rct.right = rct.left + p[2]->AsInteger();
	rct.bottom = rct.top + p[3]->AsInteger();
	auto arr = p[4]->AsObjectNoAddRef();
	for (int i = 0; i < 25; i++) {
		((Gdiplus::REAL*)bp.m)[i] = (double)getMember(arr, i);
	}
	eff.SetParameters(&bp);
	self->gi->ApplyEffect(&eff, &rct);
	RET_OK;
}

IMPL_FUNCTION KImage::doColorLUT(INTF_ARGS) {
	NOT_STATIC;
	CHECK_PARAM_COUNT(8);
	CHECK_OPENED;
	RECT rct = { 0 };
	Gdiplus::ColorLUT eff;
	Gdiplus::ColorLUTParams bp = { 0 };
	rct.left = p[0]->AsInteger();
	rct.top = p[1]->AsInteger();
	rct.right = rct.left + p[2]->AsInteger();
	rct.bottom = rct.top + p[3]->AsInteger();
	auto arr = p[4]->AsObjectNoAddRef();
	auto arr2 = p[5]->AsObjectNoAddRef();
	auto arr3 = p[6]->AsObjectNoAddRef();
	auto arr4 = p[7]->AsObjectNoAddRef();
	for (int i = 0; i < 255; i++) {
		bp.lutA[i] = (double)getMember(arr, i);
		bp.lutR[i] = (double)getMember(arr2, i);
		bp.lutG[i] = (double)getMember(arr3, i);
		bp.lutB[i] = (double)getMember(arr4, i);
	}
	eff.SetParameters(&bp);
	self->gi->ApplyEffect(&eff, &rct);
	RET_OK;
}

IMPL_FUNCTION KImage::doHueSaturationLightness(INTF_ARGS) {
	NOT_STATIC;
	CHECK_PARAM_COUNT(7);
	CHECK_OPENED;
	RECT rct = { 0 };
	Gdiplus::HueSaturationLightness eff;
	Gdiplus::HueSaturationLightnessParams bp = { 0 };
	rct.left = p[0]->AsInteger();
	rct.top = p[1]->AsInteger();
	rct.right = rct.left + p[2]->AsInteger();
	rct.bottom = rct.top + p[3]->AsInteger();
	bp.hueLevel = p[4]->AsInteger();
	bp.saturationLevel = p[5]->AsInteger();
	bp.lightnessLevel = p[6]->AsInteger();
	eff.SetParameters(&bp);
	self->gi->ApplyEffect(&eff, &rct);
	RET_OK;
}

IMPL_FUNCTION KImage::doColorBalance(INTF_ARGS) {
	NOT_STATIC;
	CHECK_PARAM_COUNT(7);
	CHECK_OPENED;
	RECT rct = { 0 };
	Gdiplus::ColorBalance eff;
	Gdiplus::ColorBalanceParams bp = { 0 };
	rct.left = p[0]->AsInteger();
	rct.top = p[1]->AsInteger();
	rct.right = rct.left + p[2]->AsInteger();
	rct.bottom = rct.top + p[3]->AsInteger();
	bp.cyanRed = p[4]->AsInteger();
	bp.magentaGreen = p[5]->AsInteger();
	bp.yellowBlue = p[6]->AsInteger();
	eff.SetParameters(&bp);
	self->gi->ApplyEffect(&eff, &rct);
	RET_OK;
}

IMPL_FUNCTION KImage::doLevels(INTF_ARGS) {
	NOT_STATIC;
	CHECK_PARAM_COUNT(7);
	CHECK_OPENED;
	RECT rct = { 0 };
	Gdiplus::Levels eff;
	Gdiplus::LevelsParams bp = { 0 };
	rct.left = p[0]->AsInteger();
	rct.top = p[1]->AsInteger();
	rct.right = rct.left + p[2]->AsInteger();
	rct.bottom = rct.top + p[3]->AsInteger();
	bp.highlight = p[4]->AsInteger();
	bp.midtone = p[5]->AsInteger();
	bp.shadow = p[6]->AsInteger();
	eff.SetParameters(&bp);
	self->gi->ApplyEffect(&eff, &rct);
	RET_OK;
}

IMPL_FUNCTION KImage::doColorCurve(INTF_ARGS) {
	NOT_STATIC;
	CHECK_PARAM_COUNT(7);
	CHECK_OPENED;
	RECT rct = { 0 };
	Gdiplus::ColorCurve eff;
	Gdiplus::ColorCurveParams bp;
	rct.left = p[0]->AsInteger();
	rct.top = p[1]->AsInteger();
	rct.right = rct.left + p[2]->AsInteger();
	rct.bottom = rct.top + p[3]->AsInteger();
	bp.adjustment = (Gdiplus::CurveAdjustments)p[4]->AsInteger();
	bp.channel = (Gdiplus::CurveChannel) p[5]->AsInteger();
	bp.adjustValue = p[6]->AsInteger();
	eff.SetParameters(&bp);
	self->gi->ApplyEffect(&eff, &rct);
	RET_OK;
}

int KImage::getopened() {
	return (bool)gi;
}

int KImage::getwidth() {
	CHECK_OPENED_N;
	return gi->GetWidth();
}

int KImage::getheight() {
	CHECK_OPENED_N;
	return gi->GetHeight();
}

double KImage::gethdpi() {
	CHECK_OPENED_N;
	return gi->GetHorizontalResolution();
}

double KImage::getvdpi() {
	CHECK_OPENED_N;
	return gi->GetVerticalResolution();
}

tTVInteger KImage::getBitmap() {
	CHECK_OPENED_N;
	return (tTVInteger)gi;
}

tTVInteger KImage::getImage()
{
	CHECK_OPENED_N;
	return (tTVInteger)(Gdiplus::Image*)gi;
}
