#pragma once

#include "Winapi.h"
#include "UIElement.h"

#include <vector>

#include <fp_plugclass.h>
#include <fp_cplug.h>

class Application
{
public:
	Application();
	~Application();

	void Init(HWND parent, int width, int height, TCPPFruityPlug* plugin);

	HWND GetWindowHandle();

	void SetNeedRedraw();

	void Show(int cmdShow);
	void SetParentWindow(HWND parent);

	void AddElement(UIElement* item);

	void SetBackgroundColor(Gdiplus::Color& color);

	TCPPFruityPlug* GetPlugin() const;

private:
	void Render(Gdiplus::Graphics* context);

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	HWND m_parent;
	HWND m_handle;
	ULONG_PTR m_gdiplusToken;
	TCPPFruityPlug* m_plugin;

	std::vector<UIElement*> m_items;

	Gdiplus::Color m_backgroundColor;
};
