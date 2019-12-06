#include "KImage.h"

tjs_error TJS_INTF_METHOD KImage::Factory(FACTORY_ARGS) {
	if (n < 1)
		return TJS_E_BADPARAMCOUNT;
	if (inst) {
		auto self = *inst = new KImage(objthis);
	}
	return TJS_S_OK;
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

tjs_error TJS_INTF_METHOD KImage::open(INTF_ARGS) {
	if (!self) return TJS_E_NATIVECLASSCRASH;
	if (n < 1) return TJS_E_BADPARAMCOUNT;
	auto oct = p[0]->AsOctetNoAddRef();
	auto cld = AllocCLD(oct->GetLength());
	memcpy(cld->addr, oct->GetData(), cld->size);
	int uicm = 0;
	if (n > 1)
		uicm = p[1]->AsInteger();
	self->close();
	self->gi = Gdiplus::Bitmap::FromStream(packStream(cld), uicm);
	return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD KImage::openImage(INTF_ARGS) {
	if (!self) return TJS_E_NATIVECLASSCRASH;
	if (n < 1) return TJS_E_BADPARAMCOUNT;
	auto obj = p[0]->AsObjectNoAddRef();
	self->close();
	self->gi = ((Gdiplus::Bitmap*)(int)getMember(obj, L"Bitmap"))->Clone(0, 0, getMember(obj, L"width"), getMember(obj, L"height"), PixelFormat32bppARGB);
	return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD KImage::openFile(INTF_ARGS) {
	if (!self) return TJS_E_NATIVECLASSCRASH;
	if (n < 1) return TJS_E_BADPARAMCOUNT;
	ttstr fn(p[0]);
	int uicm = 0;
	if (n > 1)
		uicm = p[1]->AsInteger();
	fn = TVPNormalizeStorageName(fn);
	TVPGetLocalName(fn);
	self->close();
	self->gi = Gdiplus::Bitmap::FromFile(fn.c_str(), uicm);
	return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD KImage::openFileEx(INTF_ARGS) {
	if (!self) return TJS_E_NATIVECLASSCRASH;
	if (n < 1) return TJS_E_BADPARAMCOUNT;
	ttstr fn(p[0]);
	int uicm = 0;
	if (n > 1)
		uicm = p[1]->AsInteger();
	self->close();
	self->gi = Gdiplus::Bitmap::FromStream(TVPCreateIStream(fn, TJS_BS_READ), uicm);
	return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD KImage::openLayer(INTF_ARGS) {
	if (!self) return TJS_E_NATIVECLASSCRASH;
	if (n < 1) return TJS_E_BADPARAMCOUNT;
	auto layer = p[0]->AsObjectNoAddRef();
	if (!layer->IsInstanceOf(NULL, NULL, NULL, L"Layer", layer))
		return TJS_E_INVALIDPARAM;
	LayerInfo li;
	getLayerInfo(&li, layer);
	self->close();
	auto agi = new Gdiplus::Bitmap(li.width, li.height, li.pitch, (Gdiplus::PixelFormat)PixelFormat32bppARGB, (BYTE*)li.scan);
	self->gi = agi->Clone(0, 0, li.width, li.height, PixelFormat32bppARGB);
	delete agi;
	return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD KImage::openMainSurface(INTF_ARGS) {
	if (!self) return TJS_E_NATIVECLASSCRASH;
	self->gi = Gdiplus::Bitmap::FromDirectDrawSurface7((IDirectDrawSurface7*)TVPGetDDPrimarySurfaceNoAddRef());
	return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD KImage::openResource(INTF_ARGS) {
	if (!self) return TJS_E_NATIVECLASSCRASH;
	if (n < 2) return TJS_E_BADPARAMCOUNT;

	HINSTANCE dll = LoadLibraryW(ttstr(p[0]).c_str());
	if (!dll)
		return TJS_E_INVALIDPARAM;
	self->close();
	self->gi = Gdiplus::Bitmap::FromResource(dll, ttstr(p[1]).c_str());
	FreeLibrary(dll);
	return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD KImage::allocate(INTF_ARGS) {
	if (!self) return TJS_E_NATIVECLASSCRASH;
	if (n < 2) return TJS_E_BADPARAMCOUNT;
	self->close();
	self->gi = new Gdiplus::Bitmap(p[0]->AsInteger(), p[1]->AsInteger(), PixelFormat32bppARGB);
	return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD KImage::resize(INTF_ARGS) {
	if (!self) return TJS_E_NATIVECLASSCRASH;
	if (n < 2) return TJS_E_BADPARAMCOUNT;
	if (!self->gi)TVPThrowExceptionMessage(L"not opened");
	auto ogi = self->gi;
	self->gi = new Gdiplus::Bitmap(p[0]->AsInteger(), p[1]->AsInteger());
	Gdiplus::Graphics gp(self->gi);
	gp.DrawImage(ogi, 0, 0, p[0]->AsInteger(), p[1]->AsInteger());
	delete ogi;
	return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD KImage::cut(INTF_ARGS) {
	if (!self) return TJS_E_NATIVECLASSCRASH;
	if (n < 4) return TJS_E_BADPARAMCOUNT;
	if (!self->gi)TVPThrowExceptionMessage(L"not opened");
	auto ogi = self->gi;
	//self->gi = new Gdiplus::Bitmap(p[0]->AsInteger(), p[1]->AsInteger());
	self->gi = ogi->Clone((int)p[0]->AsInteger(), p[1]->AsInteger(), p[2]->AsInteger(), p[3]->AsInteger(), PixelFormat32bppARGB);
	delete ogi;
	return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD KImage::getIconEx(INTF_ARGS) {
	if (!self) return TJS_E_NATIVECLASSCRASH;
	if (!self->gi)TVPThrowExceptionMessage(L"not opened");
	if (self->hr) {
		DWORD dw;
		if (GetHandleInformation(self->hr, &dw)){
			*r = self->hr;
			return TJS_S_OK;
		}
	}
	if (!self->gi->GetHICON(&self->hr)) {
		*r = self->hr;
		return TJS_S_OK;
	}
	else
		return TJS_E_FAIL;
}

tjs_error TJS_INTF_METHOD KImage::getIcon(INTF_ARGS) {
	if (!self) return TJS_E_NATIVECLASSCRASH;
	if (!self->gi)TVPThrowExceptionMessage(L"not opened");
	if (self->hr) {
		DWORD dw;
		if (GetHandleInformation(self->hr, &dw))
			DestroyIcon(self->hr);
	}
	if (!self->gi->GetHICON(&self->hr)) {
		*r = self->hr;
		return TJS_S_OK;
	}
	else
		return TJS_E_FAIL;
}

tjs_error TJS_INTF_METHOD KImage::setPixel(INTF_ARGS) {
	if (!self) return TJS_E_NATIVECLASSCRASH;
	if (n < 3) return TJS_E_BADPARAMCOUNT;
	if (!self->gi)TVPThrowExceptionMessage(L"not opened");
	self->gi->SetPixel(p[0]->AsInteger(), p[1]->AsInteger(), p[2]->AsInteger());
	return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD KImage::getPixel(INTF_ARGS) {
	if (!self) return TJS_E_NATIVECLASSCRASH;
	if (n < 2) return TJS_E_BADPARAMCOUNT;
	if (!self->gi)TVPThrowExceptionMessage(L"not opened");
	Gdiplus::Color clr;
	self->gi->GetPixel(p[0]->AsInteger(), p[1]->AsInteger(), &clr);
	*r = (int)clr.GetValue();
	return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD KImage::setResolution(INTF_ARGS) {
	if (!self) return TJS_E_NATIVECLASSCRASH;
	if (n < 2) return TJS_E_BADPARAMCOUNT;
	if (!self->gi)TVPThrowExceptionMessage(L"not opened");
	self->gi->SetResolution(p[0]->AsReal(), p[1]->AsReal());
	return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD KImage::drawTo(INTF_ARGS) {
	if (!self) return TJS_E_NATIVECLASSCRASH;
	if (n < 1) return TJS_E_BADPARAMCOUNT;
	if (!self->gi)TVPThrowExceptionMessage(L"not opened");
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
	return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD KImage::doBlur(INTF_ARGS) {
	if (!self) return TJS_E_NATIVECLASSCRASH;
	if (n < 5) return TJS_E_BADPARAMCOUNT;
	if (!self->gi)TVPThrowExceptionMessage(L"not opened");
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
	return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD KImage::doSharpen(INTF_ARGS) {
	if (!self) return TJS_E_NATIVECLASSCRASH;
	if (n < 6) return TJS_E_BADPARAMCOUNT;
	if (!self->gi)TVPThrowExceptionMessage(L"not opened");
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
	return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD KImage::doTint(INTF_ARGS) {
	if (!self) return TJS_E_NATIVECLASSCRASH;
	if (n < 6) return TJS_E_BADPARAMCOUNT;
	if (!self->gi)TVPThrowExceptionMessage(L"not opened");
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
	return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD KImage::doRedEyeCorrection(INTF_ARGS) {
	if (!self) return TJS_E_NATIVECLASSCRASH;
	if (n < 5) return TJS_E_BADPARAMCOUNT;
	if (!self->gi)TVPThrowExceptionMessage(L"not opened");
	RECT rct = { 0 };
	Gdiplus::RedEyeCorrection eff;
	Gdiplus::RedEyeCorrectionParams bp = { 0 };
	rct.left = p[0]->AsInteger();
	rct.top = p[1]->AsInteger();
	rct.right = rct.left + p[2]->AsInteger();
	rct.bottom = rct.top + p[3]->AsInteger();
	auto arr = p[4]->AsObjectNoAddRef();
	bp.numberOfAreas = TJSGetArrayElementCount(arr);
	if (bp.numberOfAreas>0) {
		bp.areas = new RECT[bp.numberOfAreas];
		for (int i = 0; i < bp.numberOfAreas; i++) {
			auto va = getMember(arr, i);
			if (va.Type() != tvtObject) { delete[](bp.areas + i); break; }
			auto ob = va.AsObjectNoAddRef();
			RECT* rcti = new RECT;
			rcti->left = (int)getMember(ob, L"x");
			rcti->top = (int)getMember(ob, L"y");
			rcti->right = rcti->left + (int)getMember(ob, L"w");;
			rcti->bottom = rcti->top + (int)getMember(ob, L"h");;
		}
	}

	eff.SetParameters(&bp);
	self->gi->ApplyEffect(&eff, &rct);
	delete[] bp.areas;
	return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD KImage::doMatrix(INTF_ARGS) {
	if (!self) return TJS_E_NATIVECLASSCRASH;
	if (n < 5) return TJS_E_BADPARAMCOUNT;
	if (!self->gi)TVPThrowExceptionMessage(L"not opened");
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
	return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD KImage::doColorLUT(INTF_ARGS) {
	if (!self) return TJS_E_NATIVECLASSCRASH;
	if (n < 8) return TJS_E_BADPARAMCOUNT;
	if (!self->gi)TVPThrowExceptionMessage(L"not opened");
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
	return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD KImage::doHueSaturationLightness(INTF_ARGS) {
	if (!self) return TJS_E_NATIVECLASSCRASH;
	if (n < 7) return TJS_E_BADPARAMCOUNT;
	if (!self->gi)TVPThrowExceptionMessage(L"not opened");
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
	return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD KImage::doColorBalance(INTF_ARGS) {
	if (!self) return TJS_E_NATIVECLASSCRASH;
	if (n < 7) return TJS_E_BADPARAMCOUNT;
	if (!self->gi)TVPThrowExceptionMessage(L"not opened");
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
	return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD KImage::doLevels(INTF_ARGS) {
	if (!self) return TJS_E_NATIVECLASSCRASH;
	if (n < 7) return TJS_E_BADPARAMCOUNT;
	if (!self->gi)TVPThrowExceptionMessage(L"not opened");
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
	return TJS_S_OK;
}

tjs_error TJS_INTF_METHOD KImage::doColorCurve(INTF_ARGS) {
	if (!self) return TJS_E_NATIVECLASSCRASH;
	if (n < 7) return TJS_E_BADPARAMCOUNT;
	if (!self->gi)TVPThrowExceptionMessage(L"not opened");
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
	return TJS_S_OK;
}

int KImage::opened() {
	return (bool)gi;
}

int KImage::width() {
	return gi->GetWidth();
}

int KImage::height() {
	return gi->GetHeight();
}

double KImage::hdpi() {
	return gi->GetHorizontalResolution();
}

double KImage::vdpi() {
	return gi->GetVerticalResolution();
}
