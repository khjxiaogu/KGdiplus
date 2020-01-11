#pragma once
#include "gdip.h"

#include "tp_stub.h"
#include "ncbind.hpp"
#include "KCLCommons.h"
#include "LayerEx.h"
#include "propmacros.h"
#define GETTER(X) __int3264 get##X(){return (__int3264)X;}
LRESULT WINAPI GWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
class GdiplusWindow {
	typedef GdiplusWindow THISTYPE;
	iTJSDispatch2* objthis;
	iTJSDispatch2* layerList;
	TRACKMOUSEEVENT tme;
	int visible;
	Gdiplus::Graphics* graphics;
	Gdiplus::ImageAttributes imageAttr;
	Gdiplus::Brush* brush;
	int frameColor;
	int bgcolor;
	Gdiplus::Pen* pen;
	BITMAPINFO bitmapinfo;
	HWND hwnd;
	RECT wrct;
	HDC mdc;
	//HDC hdc;
	int frameVisible;
	SIZE size;
	int innerHeight;
	int innerWidth;
	bool RegisterClass() {

		WNDCLASSEX	wce;
		BOOL bInfo;
		HINSTANCE hthis = GetModuleHandle(NULL);
		bInfo = ::GetClassInfoEx(hthis,L"GdiplusWinForKRZ", &wce);
		if (!bInfo) {
			//如果没找到，就注册

			//注册窗口信息
			wce.cbSize = sizeof(WNDCLASSEX);	//结构体大小
			wce.style = 0;	//调整大小时重画背景
			wce.lpfnWndProc = GWndProc;				//消息处理函数
			wce.cbClsExtra = 0;
			wce.cbWndExtra = 0;
			wce.hInstance = hthis;							//所属进程
			wce.hIcon = (HICON)::LoadIcon(hthis, IDI_APPLICATION);	//图标
			wce.hIconSm = (HICON)::LoadIcon(hthis, IDI_APPLICATION);
			wce.hbrBackground = (HBRUSH)::GetStockObject(HOLLOW_BRUSH);//背景画刷
			wce.hCursor = (HCURSOR)::LoadCursor(NULL, IDC_ARROW);	 //鼠标光标
			wce.lpszMenuName = NULL;								 //菜单
			wce.lpszClassName = L"GdiplusWinForKRZ";						 //窗口类名

			return  ::RegisterClassEx(&wce);
		}
		return TRUE;

	}

public:
	DEF_FUNCTION bringAfter(INTF_ARGS) {
		if (n <= 0)
			return TJS_E_BADPARAMCOUNT;
		if (p[0]->Type() == tvtObject)
		{
			::SetWindowPos(self->hwnd, (HWND)getMember(p[0]->AsObjectNoAddRef(), L"HWND").AsInteger(), NULL, NULL, NULL, NULL, SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOMOVE);
		}
		else {
			::SetWindowPos(self->hwnd, (HWND)p[0]->AsInteger(), NULL, NULL, NULL, NULL, SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOMOVE);
		}

		return TJS_S_OK;
	}
	DEF_FUNCTION Factory(FACTORY_ARGS) {
		if (inst) {
			auto * self = *inst = new THISTYPE(objthis);
		}
		return TJS_S_OK;
	}
	GdiplusWindow(iTJSDispatch2* objthis):objthis(objthis) {
		BOOL ret = this->RegisterClass();
		if (!ret) {
			TVPThrowExceptionMessage(L"cannot regist class");
		}

		//WS_EX_LAYERED|WS_EX_TRANSPARENT,WS_VISIBLE|WS_POPUP

		hwnd = ::CreateWindowEx(WS_EX_LAYERED | WS_EX_TOPMOST/*| WS_EX_TOOLWINDOW| WS_EX_NOACTIVATE|WS_EX_TRANSPARENT*/, L"GdiplusWinForKRZ", L"test",  WS_POPUP|WS_VISIBLE, 0, 0,100,100,NULL, NULL, GetModuleHandle(NULL), NULL);
		layerList = TJSCreateArrayObject();
		if (!hwnd) {
			TVPThrowExceptionMessage(L"cannot create window");
		}
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.cbSize = sizeof(tme);
		tme.dwHoverTime = 50;
		tme.hwndTrack = hwnd;
		bgcolor = 0x77aaaaaa;
		frameColor = 0xffaaaaff;
		HDC hdc = ::GetDC(hwnd);
		mdc = CreateCompatibleDC(hdc);
		::ReleaseDC(hwnd, hdc);
		//ReleaseDC(hwnd, hdc);
		::ShowWindow(hwnd, SW_SHOW);
		::UpdateWindow(hwnd);
		::SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)(this));
		createBuffer();
		//::ShowWindow(m_hWnd, SW_SHOW);


	//	this->Init();
		UpdateWindow(hwnd);
	
	};
	virtual ~GdiplusWindow() {
		::DeleteDC(mdc);
		//::ReleaseDC(hwnd, hdc);
	}
	void redraw() {
		tTJSVariant r;
		//drawBackGround();
		//objthis->FuncCall(TJS_MEMBERENSURE, TJS_W("redraw"), NULL, &r, 0, NULL, objthis);//呼叫重绘操作
		update();//重绘窗口
	}
	void onPaint() {
		redraw();
	}
	LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{//窗口事件处理器
		tTJSVariant* arg;
		switch (uMsg) {
		case WM_CREATE:
			return 0;
			break;
		case WM_DESTROY://窗口已经销毁
			//PostQuitMessage(0);//退出消息循环，结束应用程序
			return 0;
			break;
		case WM_MOUSEMOVE:
		{
			TrackMouseEvent(&tme);
			arg = new tTJSVariant[3];
			if (wParam & MK_LBUTTON)
				arg[2] |= TVP_SS_LEFT;
			if (wParam & MK_MBUTTON)
				arg[2] |= TVP_SS_MIDDLE;
			if (wParam & MK_RBUTTON)
				arg[2] |= TVP_SS_RIGHT;
			if (wParam & MK_SHIFT)
				arg[2] |= TVP_SS_SHIFT;
			if (wParam & MK_ALT)
				arg[2] |= TVP_SS_ALT;
			if (wParam & MK_CONTROL)
				arg[2] |= TVP_SS_CTRL;
			arg[0] = GET_X_LPARAM(lParam);
			arg[1] = GET_Y_LPARAM(lParam);
			TVPPostEvent(objthis, objthis, ttstr("onMouseMove"), NULL, NULL, 3, arg);
			delete[] arg;
			return 0;
			break;
		}
		case WM_MOUSEWHEEL:
		{
			arg = new tTJSVariant[4];
			if (wParam & MK_LBUTTON)
				arg[0] |= TVP_SS_LEFT;
			if (wParam & MK_MBUTTON)
				arg[0] |= TVP_SS_MIDDLE;
			if (wParam & MK_RBUTTON)
				arg[0] |= TVP_SS_RIGHT;
			if (wParam & MK_SHIFT)
				arg[0] |= TVP_SS_SHIFT;
			if (wParam & MK_ALT)
				arg[0] |= TVP_SS_ALT;
			if (wParam & MK_CONTROL)
				arg[0] |= TVP_SS_CTRL;
			arg[1] = GET_WHEEL_DELTA_WPARAM(wParam) * WHEEL_DELTA;
			arg[2] = GET_X_LPARAM(lParam);
			arg[3] = GET_Y_LPARAM(lParam);
			TVPPostEvent(objthis, objthis, ttstr("onMouseWheel"), NULL, NULL, 4, arg);
			delete[] arg;
			return 0;
			break;
		}
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		{
			arg = new tTJSVariant[4];
			if (wParam & MK_LBUTTON)
				arg[2] = mbLeft;
			else if (wParam & MK_MBUTTON)
				arg[2] = mbMiddle;
			else if (wParam & MK_RBUTTON)
				arg[2] = mbRight;
			if (wParam & MK_SHIFT)
				arg[3] |= TVP_SS_SHIFT;
			if (wParam & MK_ALT)
				arg[3] |= TVP_SS_ALT;
			if (wParam & MK_CONTROL)
				arg[3] |= TVP_SS_CTRL;
			arg[0] = GET_X_LPARAM(lParam);
			arg[1] = GET_Y_LPARAM(lParam);
			TVPPostEvent(objthis, objthis, ttstr("onMouseDown"), NULL, NULL, 4, arg);
			delete[] arg;
			return 0;
			break;
		}
		case WM_MOUSEHOVER:
			TVPPostEvent(objthis, objthis, ttstr("onMouseEnter"), NULL, NULL, 0, NULL);
			return 0;
			break;
		case WM_LBUTTONDBLCLK:
		{
			arg = new tTJSVariant[2];
			arg[0] = GET_X_LPARAM(lParam);
			arg[1] = GET_Y_LPARAM(lParam);
			TVPPostEvent(objthis, objthis, ttstr("onDoubleClick"), NULL, NULL, 2, arg);
			delete[] arg;
			return 0;
			break;
		}
		case WM_MOUSELEAVE:
			TVPPostEvent(objthis, objthis, ttstr("onMouseLeave"), NULL, NULL, 0, NULL);
			return 0;
			break;
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		{
			arg = new tTJSVariant[4];
			if (wParam & MK_LBUTTON)
				arg[2] = mbLeft;
			else if (wParam & MK_MBUTTON)
				arg[2] = mbMiddle;
			else if (wParam & MK_RBUTTON)
				arg[2] = mbRight;
			if (wParam & MK_SHIFT)
				arg[3] |= TVP_SS_SHIFT;
			if (wParam & MK_ALT)
				arg[3] |= TVP_SS_ALT;
			if (wParam & MK_CONTROL)
				arg[3] |= TVP_SS_CTRL;
			arg[0] = GET_X_LPARAM(lParam);
			arg[1] = GET_Y_LPARAM(lParam);
			TVPPostEvent(objthis, objthis, ttstr("onMouseUp"), NULL, NULL, 4, arg);
			delete[] arg;
			return 0;
			break;
		}
		case WM_KEYDOWN:
		{
			arg = new tTJSVariant[2];
			arg[0] = (int)wParam;
			arg[1] = 0;
			TVPPostEvent(objthis, objthis, ttstr("onKeyDown"), NULL, NULL, 2, arg);
			delete[] arg;
			return 0;
			break;
		}
		case WM_KEYUP:
		{
			arg = new tTJSVariant[2];
			arg[0] = (int)wParam;
			arg[1] = 0;
			TVPPostEvent(objthis, objthis, ttstr("onKeyUp"), NULL, NULL, 2, arg);
			delete[] arg;
			return 0;
			break;
		}
		default:
			break;
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);//其他消息交给系统处理
	}
	HBITMAP b;
	void* bs;
	void createBuffer() {
	/*	GetWindowRect(hwnd, &wrct);
		long wWidth = getwidth();
		long wHeight = getheight();
		if (wWidth < bitmapinfo.bmiHeader.biHeight && wHeight < bitmapinfo.bmiHeader.biWidth)return;
		RtlZeroMemory(&bitmapinfo, sizeof(BITMAPINFO));
		bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitmapinfo.bmiHeader.biWidth = wWidth;
		bitmapinfo.bmiHeader.biHeight = wHeight;
		bitmapinfo.bmiHeader.biPlanes = 1;
		bitmapinfo.bmiHeader.biBitCount = 32;
		size.cx = wWidth;
		size.cy = wHeight;
		HBITMAP  old;
		//b = CreateDIBSection(0, &bitmapinfo, 0, &bs, 0, 0);
		//ZeroMemory(bs, wWidth * wHeight * 4);
		b = ::CreateCompatibleBitmap(mdc, wWidth, wHeight);
		old = (HBITMAP)SelectObject(mdc, b);
		if (old != NULL) {
			DeleteObject(old);
		}
		DeleteObject(b);
		if (graphics != NULL)
			delete graphics;
		graphics = new Gdiplus::Graphics(mdc);
		graphics->Clear(Gdiplus::Color(0, 0, 0, 0));*/
		//graphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	}
	POINT point = {0,0};
	COLORREF udcrKey;
	BLENDFUNCTION wblend;
	//DWORD udmode=ULW_ALPHA;
	void update() {
		/*BLENDFUNCTION _Blend;
		_Blend.BlendOp = 0;
		_Blend.BlendFlags = 0;
		_Blend.AlphaFormat = 1;
		_Blend.SourceConstantAlpha = 255;
		::GetWindowRect(hwnd, &wrct);
		POINT p1 = {wrct.left,wrct.top};
		//SIZE size = {getwidth(),getheight() };
		//HDC hdc = ::GetDC(hwnd);
		//POINT p1 = {0,0};
		//graphics.SetCompositingQuality(Gdiplus::CompositingQualityHighQuality);
		//graphics->Clear(0x00000000);
		HDC hdc = ::GetDC(hwnd);
		UpdateLayeredWindow(hwnd,hdc,&p1, &size, mdc, &point, 0, &wblend, ULW_ALPHA/*|ULW_EX_NORESIZE|ULW_COLORKEY*//*);//更新																	 
		//ShowWindow(hwnd, visible?SW_SHOW:SW_HIDE);//显示、更新窗口
		UpdateWindow(hwnd);
		ReleaseDC(hwnd, hdc);*/
		RECT rct;
		GetWindowRect(hwnd, &rct);
		Gdiplus::RectF rkt(rct.left, rct.top, rct.right - rct.left, rct.bottom - rct.top);
		HDC hdc = ::GetDC(hwnd);

		HDC memDC = ::CreateCompatibleDC(hdc);
		HBITMAP bmp;
		bmp = ::CreateCompatibleBitmap(hdc,rkt.Width,rkt.Height);
		::SelectObject(memDC, bmp);

		Gdiplus::Graphics g(memDC);
		Gdiplus::Color clrt(bgcolor);
		Gdiplus::SolidBrush br(clrt);


		g.FillRectangle(&br, rkt);
		if (frameVisible) {
			Gdiplus::Pen p(frameColor, 1);//创建画笔
			g.DrawRectangle(&p, 0, 0,(int)(rkt.Width-1),(int)(rkt.Height-1));
		}
		//Gdiplus::Bitmap bmps(&bitmapinfo,bs);
		int len = TJSGetArrayElementCount(layerList);
		for (int i = 0; i < len; i++) {
			iTJSDispatch2* lay = getMember(layerList, i).AsObjectNoAddRef();
			LayerInfo li;
			getLayerInfo(&li, lay);
			if (li.scan == NULL || li.width <= 0 || li.height <= 0)
				return;
			Gdiplus::Bitmap bmp(li.width, li.height, li.pitch, PixelFormat32bppARGB, (unsigned char*)li.scan);
			//Gdiplus::Graphics layg(&bmp);
			/*float alpha = (float)(li->opacity) / (float)255.0;
			Gdiplus::ColorMatrix cm = {
				1,0,0,0,0,
				0,1,0,0,0,
				0,0,1,0,0,
				0,0,0,alpha,0,
				0,0,0,0,1 };*/
				//imageAttr.SetColorMatrix(&cm);
			Gdiplus::RectF rctl(li.left, li.top, li.width, li.height);
			g.DrawImage(&bmp, rctl, 0, 0, rctl.Width, rctl.Height, Gdiplus::UnitPixel);
		}
		//g.DrawImage(&bmps, 0, 0);
		//g.DrawImage(m_png,0,0,APH_BKCX,APH_BKCY);
		//m_strInfo.DrawStr(&g, APH_BKCX, APH_BKCY);
		//g->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		//g.SetClip(&rgn);
		//
		//Gdiplus::LinearGradientBrush PlayBrush(Gdiplus::Point(rect.Width / 2, 6), Gdiplus::Point(rect.Width / 2, rect.Height), Gdiplus::Color(255, 93, 240, 252), Gdiplus::Color(255, 223, 252, 253));
		//rect.Width -= 200;
		//g.FillRectangle(&PlayBrush, rect);
		BLENDFUNCTION _Blend;
		_Blend.BlendOp = 0;
		_Blend.BlendFlags = 0;
		_Blend.AlphaFormat = 1;
		_Blend.SourceConstantAlpha = 255;

		POINT p1 = {rct.left,rct.top};
		POINT p2 = { 0,0 };
		//SIZE size = { APH_BKCX,APH_BKCY };
		//HDC hdc = ::GetDC(hwnd);
		::UpdateLayeredWindow(hwnd, hdc, &p1, &size, memDC, &p2, 0, &_Blend, ULW_ALPHA);

		::DeleteObject(bmp);
		::DeleteDC(memDC);
		::ReleaseDC(hwnd, hdc);
	}
	void drawBackGround() {
		/*createBuffer();
		Gdiplus::Rect winrct(0, 0, getinnerWidth(), getinnerHeight());
		//graphics->Clear(Gdiplus::Color(0, 0, 0, 0));
		brush = new Gdiplus::SolidBrush(Gdiplus::Color(bgcolor));
		graphics->FillRectangle(brush, winrct);
		delete brush;
		if (frameVisible) {
			pen = new Gdiplus::Pen(frameColor, 1);//创建画笔
			graphics->DrawRectangle(pen, 0, 0, winrct.Width - 1, winrct.Height - 1);
			delete pen;
		}*/
	}
	static tjs_error TJS_INTF_METHOD getLayerList(INTF_ARGS) {
		if (r)
			*r = tTJSVariant(self->layerList, self->layerList);
		return TJS_S_OK;
	}
	static tjs_error TJS_INTF_METHOD drawLayer(INTF_ARGS) {
		if (n < 1)
			return TJS_E_BADPARAMCOUNT;
		iTJSDispatch2* layer = p[0]->AsObjectNoAddRef();
		if (!layer->IsValid(TJS_MEMBERENSURE, NULL, NULL, layer))
			return TJS_E_INVALIDOBJECT;
		LayerInfo li;
		getLayerInfo(&li, layer);
		self->renderLayer(&li, (int)getMember(layer, L"width"), (int)getMember(layer, L"height"));
		return TJS_S_OK;
	}
	static tjs_error TJS_INTF_METHOD drawImage(INTF_ARGS) {
		if (n < 1)
			return TJS_E_BADPARAMCOUNT;
		iTJSDispatch2* kimg = p[0]->AsObjectNoAddRef();
		Gdiplus::Bitmap* img = (Gdiplus::Bitmap*)__int3264(getMember(kimg, TJS_W("Image")));
		int left;
		int top;
		int dwidth;
		int dheight;
		if (n > 2 && p[1]->Type() != tvtVoid) {
			left = *p[1];
			top = *p[2];
		}
		else {
			left = 0;
			top = 0;
		}
		if (n > 4 && p[3]->Type() != tvtVoid) {
			dwidth = *p[3];
			dheight = *p[4];
		}
		else {
			dwidth = getMember(kimg, L"width");
			dheight = getMember(kimg, L"height");
		}
		Gdiplus::RectF rctl(left, top, dwidth, dheight);
		if (n > 5) {
			float alpha = (float)((tTVReal)*p[5]) / (float)255.0;
			Gdiplus::ColorMatrix cm = {
			1,0,0,0,0,
			0,1,0,0,0,
			0,0,1,0,0,
			0,0,0,alpha,0,
			0,0,0,0,1 };
			self->imageAttr.SetColorMatrix(&cm);
			self->graphics->DrawImage(static_cast<Gdiplus::Image*>(img), rctl, 0, 0, rctl.Width, rctl.Height, Gdiplus::UnitPixel, &self->imageAttr);
		}
		else
			self->graphics->DrawImage(static_cast<Gdiplus::Image*>(img), rctl, 0, 0, rctl.Width, rctl.Height, Gdiplus::UnitPixel);
		return TJS_S_OK;
	}

	void renderLayer(LayerInfo* li, int dwidth, int dheight) {
		if (li->scan == NULL || li->width <= 0 || li->height <= 0)
			return;
		Gdiplus::Bitmap bmp(li->width, li->height, li->pitch, PixelFormat32bppARGB, (unsigned char*)li->scan);
		//Gdiplus::Graphics layg(&bmp);
		/*float alpha = (float)(li->opacity) / (float)255.0;
		Gdiplus::ColorMatrix cm = {
			1,0,0,0,0,
			0,1,0,0,0,
			0,0,1,0,0,
			0,0,0,alpha,0,
			0,0,0,0,1 };*/
		//imageAttr.SetColorMatrix(&cm);
		Gdiplus::RectF rctl(li->left, li->top, dwidth, dheight);
		graphics->DrawImage(&bmp, rctl, 0, 0, rctl.Width, rctl.Height,Gdiplus::UnitPixel);
	}
	static tjs_error TJS_INTF_METHOD EmptyMethod(INTF_ARGS) {
		return TJS_S_OK;
	}
	int getHWND() {//已经注册
		return (int)hwnd;
	}
	GETTER(visible);//已经注册
	GETTER(bgcolor);//已经注册
	GETTER(frameVisible);//已经注册
	GETTER(frameColor);//已经注册
	GETTER(hwnd);
	ttstr getcaption() {//已经注册
		wchar_t cptText[MAX_PATH];
		GetWindowText(hwnd, cptText, MAX_PATH);
		return ttstr(cptText);
		//return ttstr();
	}
	void setcaption(ttstr text) {//已经注册
		SetWindowText(hwnd,text.c_str());
	}
	void setvisible(int val) {//已经注册
		visible = val;
		ShowWindow(hwnd, visible ? SW_SHOW : SW_HIDE);
		update();
	}
	void setframeVisible(int val) {//已经注册
		frameVisible = val;
		redraw();
	}
	void setbgcolor(int val) {//已经注册
		bgcolor = val;
		redraw();
	}
	void setframeColor(int val) {//已经注册
		frameColor = val;
		redraw();
	}
	void setSize(int w, int h) {//已经注册
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, w, h, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
		createBuffer();
		setInnerSize(w, h);
	}
	void setPos(int l, int t) {//已经注册
		SetWindowPos(hwnd, HWND_TOPMOST, l, t, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER);
	}
	void setInnerSize(int w, int h) {//已经注册
		innerHeight = h;
		innerWidth = w;
		redraw();
	}
	void beginMove() {//已经注册
		PostMessage(hwnd, WM_SYSCOMMAND, 61458, 0);//让无边框窗口能够拖动(在窗口客户区拖动)
	}
	int getleft() {//已经注册
		GetWindowRect(hwnd, &wrct);
		return wrct.left;
	}
	void setleft(int l) {//已经注册
		GetWindowRect(hwnd, &wrct);
		setPos(l, wrct.top);
	}
	int gettop() {//已经注册
		GetWindowRect(hwnd, &wrct);
		return wrct.top;
	}
	void settop(int t) {//已经注册
		GetWindowRect(hwnd, &wrct);
		setPos(wrct.left, t);
	}
	void setwidth(int w) {//已经注册
		setSize(w, getheight());
	}
	int getwidth() {//已经注册
		return wrct.right - wrct.left;
	}
	void setheight(int h) {//已经注册
		setSize(getwidth(), h);
	}
	int getheight() {//已经注册
		return wrct.bottom - wrct.top;
	}
	void setinnerHeight(int h) {
		setInnerSize(innerWidth, h);
	}
	void setinnerWidth(int w) {
		setInnerSize(w, innerHeight);
	}
	int getinnerHeight() {
		return innerHeight;
	}
	int getinnerWidth() {
		return innerWidth;
	}
};
NCB_REGISTER_CLASS(GdiplusWindow) {
	CONSTRUCTOR;
	FUNCTION(bringAfter);
	FUNCTION(getLayerList);
	PROPERTY(visible);
	PROPERTY(frameVisible);
	PROPERTY(bgcolor);
	PROPERTY(height);
	PROPERTY(width);
	PROPERTY(left);
	PROPERTY(top);
	PROPERTY(caption);
	PROPERTY(frameColor);
	PROPERTY(innerWidth);
	PROPERTY(innerHeight);
	PROPERTY_RO(hwnd);
	//RawCallback("SetLayeredWindowAttributes", &Class::setWindowsBase, 0);
	METHOD(beginMove);
	METHOD(update);
	METHOD(onPaint);
	METHOD(drawBackGround);
	FUNCTION(drawLayer);
	FUNCTION(drawImage);
	EMPTYEVENT(redraw);
	METHOD(setSize);
	METHOD(setPos);
	METHOD(setInnerSize);
	EMPTYEVENT(onActivate);
	EMPTYEVENT(onClick);
	EMPTYEVENT(onCloseQuery);
	EMPTYEVENT(onDeactivate);
	EMPTYEVENT(onDoubleClick);
	EMPTYEVENT(onFileDrop);
	EMPTYEVENT(onKeyDown);
	EMPTYEVENT(onKeyPress);
	EMPTYEVENT(onKeyUp);
	EMPTYEVENT(onMouseDown);
	EMPTYEVENT(onMouseEnter);
	EMPTYEVENT(onMouseLeave);
	EMPTYEVENT(onMouseMove);
	EMPTYEVENT(onMouseUp);
	EMPTYEVENT(onMouseWheel);
	EMPTYEVENT(onPopupHide);
	EMPTYEVENT(onResize);
}

