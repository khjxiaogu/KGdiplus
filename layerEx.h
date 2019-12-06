#pragma once
struct LayerInfo {
	void* scan;
	int top;
	int left;
	int width;
	int height;
	int opacity;
	int pitch;
};
void getLayerInfo(LayerInfo* li, iTJSDispatch2* layer);

void getLayerInfoFW(LayerInfo* li, iTJSDispatch2* layer);
