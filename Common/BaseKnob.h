#pragma once
#include "UIElement.h"
#include "../Collisions/CircleCollision.h"
#include "Parameter.h"

class BaseKnob : public UIElement
{
public:
	BaseKnob(int x, int y, int radius, Parameter* link);

	void Draw(Gdiplus::Graphics* context) override;
	void HandleMouse(int x, int y, int buttons) override;

	// between 0 and 1
	void SetParamValue(float val);

private:
	void LockCursor(int x, int y);
	void UnlockCursor();

	// Call in HandleMouse
	bool LockCursorInPlace(int x, int y);

private:
	RECT m_cursorLock;
	POINT m_cursorLockPos;

private:
	CircleCollision m_collision;
	Parameter* m_linkedParam;

	bool m_active;

	int m_x, m_y, m_radius;
	int m_startMouseX, m_startMouseY;
};
