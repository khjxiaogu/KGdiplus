#include "propmacros.h"
#include "layerEx.h"

void getLayerInfo(LayerInfo* li, iTJSDispatch2* layer) {
	li->scan = (void*)(int)getMember(layer, L"mainImageBuffer");
	li->width = (int)getMember(layer, L"imageWidth");
	li->height = (int)getMember(layer, L"imageHeight");
	li->top = (int)getMember(layer, L"top");
	li->left = (int)getMember(layer, L"left");
	li->opacity = (int)getMember(layer, L"opacity");
	li->pitch = (int)getMember(layer, L"mainImageBufferPitch");
}
void getLayerInfoFW(LayerInfo* li, iTJSDispatch2* layer) {
	li->scan = (void*)(int)getMember(layer, L"mainImageBufferForWrite");
	li->width = (int)getMember(layer, L"imageWidth");
	li->height = (int)getMember(layer, L"imageHeight");
	li->top = (int)getMember(layer, L"top");
	li->left = (int)getMember(layer, L"left");
	li->opacity = (int)getMember(layer, L"opacity");
	li->pitch = (int)getMember(layer, L"mainImageBufferPitch");
}