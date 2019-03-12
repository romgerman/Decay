#include "Application.h"

Application::Application() : m_handle(NULL),
							 m_backgroundColor(Gdiplus::Color(48, 50, 80)) { }

Application::~Application()
{
	for (UIElement* el : m_items)
	{
		if (el)	delete el;
	}

	Gdiplus::GdiplusShutdown(m_gdiplusToken);
}

// TODO: OnContextMenu to open context menu
// TODO: fix crash on exit

LRESULT CALLBACK Application::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
		{
			SetFocus(hWnd);

			LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
			Application* app = (Application*)pcs->lpCreateParams;

			SetWindowLongPtrW(hWnd,	GWLP_USERDATA, PtrToUlong(app));

			return 1;
		}
		case WM_MOUSEMOVE:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		{
			Application* app = reinterpret_cast<Application*>(static_cast<LONG_PTR>(GetWindowLongPtrW(hWnd, GWLP_USERDATA)));

			if (app)
			{
				for (UIElement* el : app->m_items)
					el->HandleMouse(LOWORD(lParam), HIWORD(lParam), wParam);
			}

			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			// TODO:
		}
		break;
		case WM_KEYDOWN:
		break;
		case WM_KEYUP:
		break;
		case WM_DISPLAYCHANGE:
		{
			InvalidateRect(hWnd, NULL, FALSE);
			return 0;
		}
		case WM_PAINT:
		{
			Application* app = reinterpret_cast<Application*>(static_cast<LONG_PTR>(GetWindowLongPtrW(hWnd, GWLP_USERDATA)));

			if (app) // This is kinda double buffering brought from somewhere
			{
				// Get the area we drawing
				RECT clientRect;
				GetClientRect(hWnd, &clientRect);

				int width = clientRect.right - clientRect.left;
				int height = clientRect.bottom - clientRect.top;
				
				PAINTSTRUCT ps;
				HDC memHdc;
				HDC hdc;
				HBITMAP memBitmap;

				hdc = BeginPaint(hWnd, &ps);
				memHdc = CreateCompatibleDC(hdc);
				memBitmap = CreateCompatibleBitmap(hdc, width, height);
				
				SelectObject(memHdc, memBitmap);

				Gdiplus::Graphics gtx(memHdc);
				gtx.Clear(app->m_backgroundColor);
				gtx.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
				app->Render(&gtx);

				BitBlt(hdc, 0, 0, width, height, memHdc, 0, 0, SRCCOPY);

				DeleteObject(memBitmap);
				DeleteObject(memHdc);
				DeleteObject(hdc);

				EndPaint(hWnd, &ps);
				ValidateRect(hWnd, NULL);
			}

			return 0;
		}
		/*
			case WM_SETFOCUS:
			return 0;
			case WM_KILLFOCUS:
			return 0;*/
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

void Application::Init(HWND parent, int width, int height, TCPPFruityPlug* plugin)
{
	m_parent = parent;
	m_plugin = plugin;
	
	HINSTANCE hInstance = (HINSTANCE)GetWindowLong(parent, GWL_HINSTANCE);

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	WNDCLASS wndClass;

	ZeroMemory(&wndClass, sizeof(WNDCLASS));

	wndClass.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = Application::WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = 0;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = 0;
	wndClass.lpszMenuName = 0;
	wndClass.lpszClassName = "VSTPluginWClass";

	RegisterClass(&wndClass);

	m_handle = CreateWindow("VSTPluginWClass", "Window", WS_CHILDWINDOW | WS_CLIPCHILDREN,
		0, 0, width, height, parent, NULL, hInstance, this);

	if (!m_handle)
		MessageBox(0, "WINDOW CREATION FAILED", "", 0);

	SetNeedRedraw();
}

HWND Application::GetWindowHandle()
{
	return m_handle;
}

void Application::SetNeedRedraw()
{
	InvalidateRect(m_handle, NULL, FALSE);
}

void Application::Show(int cmdShow)
{
	ShowWindow(m_handle, cmdShow);
}

void Application::SetParentWindow(HWND parent)
{
	SetParent(m_handle, parent);
}

void Application::AddElement(UIElement* item)
{
	item->m_app = this;
	m_items.push_back(item);
}

void Application::SetBackgroundColor(Gdiplus::Color& color)
{
	m_backgroundColor = color;
}

TCPPFruityPlug* Application::GetPlugin() const
{
	return m_plugin;
}

void Application::Render(Gdiplus::Graphics* context)
{
	for (UIElement* el : m_items)
		el->Draw(context);
}
