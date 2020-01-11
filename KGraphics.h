#pragma once
#include "gdip.h"
#include "../ncbind.hpp"
#include "KCLCommons.h"
#include "LayerEx.h"
#include "propmacros.h"
#define CHECK_OPENED if (!self->gp)TVPThrowExceptionMessage(L"not opened")
#define CHECK_OPENED_N if (!gp)TVPThrowExceptionMessage(L"not opened")
inline Gdiplus::Image* getImage(tTJSVariant * var) {
	return (Gdiplus::Image*)getMember(var->AsObjectNoAddRef(), L"Image").AsInteger();
}
inline Gdiplus::Pen* getPen(tTJSVariant * var) {
	return (Gdiplus::Pen*)getMember(var->AsObjectNoAddRef(), L"Pen").AsInteger();
}
inline Gdiplus::Brush* getBrush(tTJSVariant* var) {
	return (Gdiplus::Brush*)getMember(var->AsObjectNoAddRef(), L"Brush").AsInteger();
}
inline Gdiplus::Font* getFont(tTJSVariant* var) {
	return (Gdiplus::Font*)getMember(var->AsObjectNoAddRef(), L"Font").AsInteger();
}
inline Gdiplus::Rect getRect(tTJSVariant** p, tjs_int i) {
	return Gdiplus::Rect(p[i]->AsInteger(), p[i+1]->AsInteger(), p[i+2]->AsInteger(), p[i+3]->AsInteger());
}
inline Gdiplus::Point getPoint(tTJSVariant** p, tjs_int i) {
	return Gdiplus::Point(p[i]->AsInteger(), p[i + 1]->AsInteger());
}
inline Gdiplus::Size getSize(tTJSVariant** p, tjs_int i) {
	return Gdiplus::Size(p[i]->AsInteger(), p[i + 1]->AsInteger());
}
inline Gdiplus::RectF getRectF(tTJSVariant** p, tjs_int i) {
	return Gdiplus::RectF(p[i]->AsInteger(), p[i + 1]->AsInteger(), p[i + 2]->AsInteger(), p[i + 3]->AsInteger());
}
inline Gdiplus::PointF getPointF(tTJSVariant** p, tjs_int i) {
	return Gdiplus::PointF(p[i]->AsInteger(), p[i + 1]->AsInteger());
}
inline Gdiplus::PointF* getPointFArr(tTJSVariant* p,int* c) {
	auto arr = p->AsObjectNoAddRef();
	*c = TJSGetArrayElementCount(arr);
	if (*c > 0) {
		auto ps = new Gdiplus::PointF[*c];
		for (int i = 0; i < *c; i++) {
			auto va = getMember(arr, i);
			if (va.Type() != tvtObject) { delete[](ps + i); break; }
			auto ob = va.AsObjectNoAddRef();
			ps[i] =Gdiplus::PointF(getMember(ob, L"x").AsReal(), getMember(ob, L"y").AsReal());
		}
		return ps;
	}
	return NULL;
}
inline Gdiplus::SizeF getSizeF(tTJSVariant** p, tjs_int i) {
	return Gdiplus::SizeF(p[i]->AsInteger(), p[i + 1]->AsInteger());
}
class KGraphics
{
	typedef KGraphics THISTYPE;
	Gdiplus::Graphics* gp = NULL;
	iTJSDispatch2* objthis;
	KGraphics(iTJSDispatch2* objthis) :objthis(objthis) {}
public:
	DEF_FUNCTION Factory(FACTORY_ARGS) {
		if (inst) {
			auto self = *inst = new THISTYPE(objthis);
		}
		return TJS_S_OK;
	}
	DEF_FUNCTION openImage(INTF_ARGS) {
		NOT_STATIC;
		CHECK_PARAM_COUNT(1);
		self->gp=Gdiplus::Graphics::FromImage(getImage(p[0]));
		RET_OK;
	}
	DEF_FUNCTION drawArc(INTF_ARGS) {
		NOT_STATIC;
		CHECK_PARAM_COUNT(7);
		CHECK_OPENED;
		self->gp->DrawArc(getPen(p[0]), getRectF(p, 1), p[5]->AsReal(), p[6]->AsReal());
		RET_OK;
	}
	DEF_FUNCTION drawBezier(INTF_ARGS) {
		NOT_STATIC;
		CHECK_PARAM_COUNT(9);
		CHECK_OPENED;
		self->gp->DrawBezier(getPen(p[0]),getPointF(p,1), getPointF(p, 3), getPointF(p, 5), getPointF(p, 7));
		RET_OK;
	}
	DEF_FUNCTION drawClosedCurve(INTF_ARGS) {
		NOT_STATIC;
		CHECK_PARAM_COUNT(2);
		CHECK_OPENED;
		int c;
		auto pfa = getPointFArr(p[1], &c);
		self->gp->DrawClosedCurve(getPen(p[0]),pfa,c);
		delete[] pfa;
		RET_OK;
	}
	DEF_FUNCTION drawCurve(INTF_ARGS) {
		NOT_STATIC;
		CHECK_PARAM_COUNT(2);
		CHECK_OPENED;
		int c;
		auto pfa = getPointFArr(p[1], &c);
		self->gp->DrawCurve(getPen(p[0]), pfa, c);
		delete[] pfa;
		RET_OK;
	}
	DEF_FUNCTION drawEllipse(INTF_ARGS) {
		NOT_STATIC;
		CHECK_PARAM_COUNT(5);
		CHECK_OPENED;
		self->gp->DrawEllipse(getPen(p[0]),getRectF(p,1));
		RET_OK;
	}
	DEF_FUNCTION drawImage(INTF_ARGS) {
		NOT_STATIC;
		CHECK_PARAM_COUNT(5);
		CHECK_OPENED;
		self->gp->DrawImage(getImage(p[0]),getRect(p,1));
		RET_OK;
	}
	DEF_FUNCTION drawLine(INTF_ARGS) {
		NOT_STATIC;
		CHECK_PARAM_COUNT(5);
		CHECK_OPENED;
		self->gp->DrawLine(getPen(p[0]), getPointF(p, 1),getPointF(p,3));
		RET_OK;
	}
	DEF_FUNCTION drawPie(INTF_ARGS) {
		NOT_STATIC;
		CHECK_PARAM_COUNT(7);
		CHECK_OPENED;
		self->gp->DrawPie(getPen(p[0]), getRectF(p, 1),p[5]->AsReal(),p[6]->AsReal());
		RET_OK;
	}
	DEF_FUNCTION drawPolygon(INTF_ARGS) {
		NOT_STATIC;
		CHECK_PARAM_COUNT(2);
		CHECK_OPENED;
		int c;
		auto pfa = getPointFArr(p[1], &c);
		self->gp->DrawPolygon(getPen(p[0]), pfa, c);
		delete[] pfa;
		RET_OK;
	}
	DEF_FUNCTION drawRect(INTF_ARGS) {
		NOT_STATIC;
		CHECK_PARAM_COUNT(5);
		CHECK_OPENED;
		self->gp->DrawRectangle(getPen(p[0]), getRectF(p, 1));
		RET_OK;
	}
	DEF_FUNCTION drawText(INTF_ARGS) {
		NOT_STATIC;
		CHECK_PARAM_COUNT(5);
		CHECK_OPENED;
		self->gp->DrawString(ttstr(p[2]).c_str(),p[2]->AsStringNoAddRef()->GetLength(),getFont(p[3]), getPointF(p, 0),getBrush(p[4]));
		RET_OK;
	}
	//fills
	DEF_FUNCTION fillClosedCurve(INTF_ARGS) {
		NOT_STATIC;
		CHECK_PARAM_COUNT(2);
		CHECK_OPENED;
		int c;
		auto pfa = getPointFArr(p[1], &c);
		self->gp->FillClosedCurve(getBrush(p[0]), pfa, c);
		delete[] pfa;
		RET_OK;
	}

	DEF_FUNCTION fillEllipse(INTF_ARGS) {
		NOT_STATIC;
		CHECK_PARAM_COUNT(5);
		CHECK_OPENED;
		self->gp->FillEllipse(getBrush(p[0]), getRectF(p, 1));
		RET_OK;
	}
	DEF_FUNCTION fillPie(INTF_ARGS) {
		NOT_STATIC;
		CHECK_PARAM_COUNT(7);
		CHECK_OPENED;
		self->gp->FillPie(getBrush(p[0]), getRectF(p, 1), p[5]->AsReal(), p[6]->AsReal());
		RET_OK;
	}
	DEF_FUNCTION fillPolygon(INTF_ARGS) {
		NOT_STATIC;
		CHECK_PARAM_COUNT(2);
		CHECK_OPENED;
		int c;
		auto pfa = getPointFArr(p[1], &c);
		self->gp->FillPolygon(getBrush(p[0]), pfa, c);
		delete[] pfa;
		RET_OK;
	}
	DEF_FUNCTION fillRect(INTF_ARGS) {
		NOT_STATIC;
		CHECK_PARAM_COUNT(5);
		CHECK_OPENED;
		self->gp->FillRectangle(getBrush(p[0]), getRectF(p, 1));
		RET_OK;
	}
	int getcomposingMode() {
		CHECK_OPENED_N;
		return (int)gp->GetCompositingMode();
	}
	void setcomposingMode(int v) {
		CHECK_OPENED_N;
		gp->SetCompositingMode((Gdiplus::CompositingMode)v);
	}
	int getcompositingQuality() {
		CHECK_OPENED_N;
		return (int)gp->GetCompositingQuality();
	}
	void setcompositingQuality(int v) {
		CHECK_OPENED_N;
		gp->SetCompositingQuality((Gdiplus::CompositingQuality)v);
	}
	double getDPIX() {
		CHECK_OPENED_N;
		return gp->GetDpiX();
	}
	double getDPIY() {
		CHECK_OPENED_N;
		return gp->GetDpiY();
	}
	int getinterpolationMode() {
		CHECK_OPENED_N;
		return (int)gp->GetInterpolationMode();
	}
	void setinterpolationMode(int v) {
		CHECK_OPENED_N;
		gp->SetInterpolationMode((Gdiplus::InterpolationMode)v);
	}
	double getpageScale() {
		CHECK_OPENED_N;
		return gp->GetPageScale();
	}
	void setpageScale(double v) {
		CHECK_OPENED_N;
		gp->SetPageScale(v);
	}
	int getpixelOffsetMode() {
		CHECK_OPENED_N;
		return (int)gp->GetPixelOffsetMode();
	}
	void setpixelOffsetMode(int v) {
		CHECK_OPENED_N;
		gp->SetPixelOffsetMode((Gdiplus::PixelOffsetMode)v);
	}
	int getRenderingOriginX() {
		CHECK_OPENED_N;
		int x;
		int y;
		gp->GetRenderingOrigin(&x,&y);
		return x;
	}
	int getRenderingOriginY() {
		CHECK_OPENED_N;
		int x;
		int y;
		gp->GetRenderingOrigin(&x, &y);
		return y;
	}
	void setRenderingOrigin(int x,int y) {
		CHECK_OPENED_N;
		gp->SetRenderingOrigin(x,y);
	}
};

