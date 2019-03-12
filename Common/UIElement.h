#pragma once
#include <minmax.h>
#include <objidl.h>
#include <gdiplus.h>

class Application;

class UIElement
{
	friend class Application;

public:
	virtual void Draw(Gdiplus::Graphics* context) = 0;
	virtual void Update() { }
	virtual void HandleMouse(int x, int y, int buttons) { }
	virtual void HandleMouseWheel(int delta) { }

protected:
	Application* m_app;
};
