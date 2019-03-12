#include "BaseKnob.h"
#include "Application.h"
#include "../Utils/String.h"
#include <string>

BaseKnob::BaseKnob(int x, int y, int radius, Parameter* link) : m_active(false),
																m_x(x), m_y(y),
																m_radius(radius),
																m_linkedParam(link)
{
	m_collision = CircleCollision(float(x + radius / 2), float(y + radius / 2), float(radius / 2));
}

void BaseKnob::Draw(Gdiplus::Graphics* context)
{	
	// Set up the arc.
	Gdiplus::Pen arcPen(Gdiplus::Color(188, 255, 49), 3);
	arcPen.SetEndCap(Gdiplus::LineCap::LineCapRound);
	arcPen.SetStartCap(Gdiplus::LineCap::LineCapRound);
	Gdiplus::Pen backArcPen(Gdiplus::Color(39, 43, 46), 3);
	Gdiplus::SolidBrush knobBrush(Gdiplus::Color(75, 82, 88));
	Gdiplus::Rect ellipseRect(m_x, m_y, m_radius, m_radius);

	if (m_active)
	{
		arcPen.SetColor(Gdiplus::Color(172, 255, 0));
		knobBrush.SetColor(Gdiplus::Color(96, 102, 107));
	}

	
	// Draw the arc.
	context->DrawEllipse(&backArcPen, m_x, m_y, m_radius, m_radius);
	context->DrawArc(&arcPen, ellipseRect, -180.0f, m_linkedParam->GetNormalizedValue() * 180.0f);
	context->FillEllipse(&knobBrush, m_x + 4, m_y + 4, m_radius - 8, m_radius - 8);


	Gdiplus::SolidBrush fontBrush(Gdiplus::Color(200, 200, 200));
	Gdiplus::Font font(L"Arial", (float)(log10(m_radius) * 5));
	Gdiplus::RectF layoutRect(m_x, m_y + m_radius + 10, m_radius, m_radius);

	Gdiplus::StringFormat format;
	format.SetAlignment(Gdiplus::StringAlignmentCenter);

	context->DrawString(CharToWchar(m_linkedParam->GetName(), m_linkedParam->GetName().length),
						m_linkedParam->GetName().length,
						&font, layoutRect, &format, &fontBrush);

	auto wval = CharToWchar(m_linkedParam->GetUnits(), m_linkedParam->GetUnits().length);
	auto val = std::to_wstring(m_linkedParam->GetValue()) + wval;
	Gdiplus::RectF layoutRect2(m_x, m_y + m_radius + 30, m_radius, m_radius);
	
	context->DrawString(val.c_str(), val.length(), &font, layoutRect2, &format, &fontBrush);

	// debug
	//context->DrawRectangle(&arcPen, ellipseRect);
}

void BaseKnob::HandleMouse(int x, int y, int buttons)
{	
	if ((buttons & MK_LBUTTON))
	{		
		if (!m_active)
		{
			if (m_collision.PointInside(x, y))
			{
				m_startMouseX = x;
				m_startMouseY = y;
				m_active = true;
				LockCursor(x, y);
				m_app->SetNeedRedraw();
			}
		}
		else
		{
			auto newval = m_linkedParam->GetNormalizedValue() + (m_startMouseY - y) / 100.f;

			m_linkedParam->SetNormalizedValue(newval);
			m_app->GetPlugin()->PlugHost->OnParamChanged(m_app->GetPlugin()->HostTag, m_linkedParam->GetIndex(), m_linkedParam->GetInternalValue());
			//m_app->GetPlugin()->ProcessParam(m_linkedParam->GetIndex(), 0, REC_PlugReserved);
			

			if (LockCursorInPlace(x, y))
			{
				m_startMouseX = m_cursorLockPos.x;
				m_startMouseY = m_cursorLockPos.y;
			}
			else
			{
				m_startMouseX = x;
				m_startMouseY = y;
			}

			m_app->SetNeedRedraw();
		}
	}
	else
	{
		if (m_active)
		{
			m_active = false;
			UnlockCursor();
			m_app->SetNeedRedraw();
		}
	}

	if (m_collision.PointInside(x, y))
		m_app->GetPlugin()->PlugHost->OnHint(m_app->GetPlugin()->HostTag, m_linkedParam->GetName());
}

void BaseKnob::SetParamValue(float val)
{
	m_linkedParam->SetNormalizedValue(val);
	m_app->SetNeedRedraw();
}

void BaseKnob::LockCursor(int x, int y)
{
	m_cursorLockPos.x = x;
	m_cursorLockPos.y = y;

	ShowCursor(FALSE);
	
	GetClipCursor(&m_cursorLock);

	RECT rc;
	GetWindowRect(m_app->GetWindowHandle(), &rc);

	ClipCursor(&rc);
}

void BaseKnob::UnlockCursor()
{
	ClipCursor(&m_cursorLock);

	RECT rc;
	GetWindowRect(m_app->GetWindowHandle(), &rc);

	SetCursorPos(rc.left + m_cursorLockPos.x, rc.top + m_cursorLockPos.y);
	ShowCursor(TRUE);
}

bool BaseKnob::LockCursorInPlace(int x, int y)
{	
	RECT rc;
	GetClientRect(m_app->GetWindowHandle(), &rc);
	
	if (x >= (rc.right - 5) || x <= 0 || // Without -5 it doesn't work with right and bottom sides (maybe because of the border)
	    y >= (rc.bottom - 5) || y <= 0)
	{
		GetWindowRect(m_app->GetWindowHandle(), &rc);
		SetCursorPos(rc.left + m_cursorLockPos.x, rc.top + m_cursorLockPos.y);
		return true;
	}

	return false;
}
