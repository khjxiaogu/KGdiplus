#pragma once
#include "gdip.h"
#define USEISTREAM

//#include "../tp_stub.h"
#include "../ncbind.hpp"
#include "KCLCommons.h"
#include "LayerEx.h"
#include "propmacros.h"
#define DEF_METHOD static tjs_error TJS_INTF_METHOD
#define INTF_ARGS tTJSVariant* r, tjs_int n, tTJSVariant** p, KImage* self
#define FACTORY_ARGS KImage** inst, tjs_int n, tTJSVariant** p, iTJSDispatch2* objthis
class KImage
{
	Gdiplus::Bitmap* gi=NULL;
	iTJSDispatch2* objthis;
	HICON hr=NULL;
	KImage(iTJSDispatch2 * objthis):objthis(objthis) {}
public:
	DEF_METHOD Factory(FACTORY_ARGS);
	void close();
	DEF_METHOD open(INTF_ARGS);
	DEF_METHOD openImage(INTF_ARGS);
	DEF_METHOD openFile(INTF_ARGS);
	DEF_METHOD openFileEx(INTF_ARGS);
	DEF_METHOD openLayer(INTF_ARGS);
	DEF_METHOD openMainSurface(INTF_ARGS);
	DEF_METHOD openResource(INTF_ARGS);
	DEF_METHOD allocate(INTF_ARGS);
	DEF_METHOD resize(INTF_ARGS);
	DEF_METHOD cut(INTF_ARGS);
	DEF_METHOD getIconEx(INTF_ARGS);
	DEF_METHOD getIcon(INTF_ARGS);
	DEF_METHOD setPixel(INTF_ARGS);
	DEF_METHOD getPixel(INTF_ARGS);
	DEF_METHOD setResolution(INTF_ARGS);
	DEF_METHOD drawTo(INTF_ARGS);
	DEF_METHOD doBlur(INTF_ARGS);
	DEF_METHOD doSharpen(INTF_ARGS);
	DEF_METHOD doTint(INTF_ARGS);
	DEF_METHOD doRedEyeCorrection(INTF_ARGS);
	DEF_METHOD doMatrix(INTF_ARGS);
	DEF_METHOD doColorLUT(INTF_ARGS);
	DEF_METHOD doHueSaturationLightness(INTF_ARGS);
	DEF_METHOD doColorBalance(INTF_ARGS);
	DEF_METHOD doLevels(INTF_ARGS);
	DEF_METHOD doColorCurve(INTF_ARGS);
	int opened();
	int width();
	int height();
	double hdpi();
	double vdpi();
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
	FUNCTION(drawTo);
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
}