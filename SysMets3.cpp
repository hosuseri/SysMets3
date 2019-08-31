#include "MyWindow.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI
WinMain
(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    static TCHAR szAppName[] = TEXT("SysMets3");
    HWND hwnd;
    MSG msg;
    WNDCLASS wndclass;
     
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = ::WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;
    if (!::RegisterClass(&wndclass)) {
	::MessageBox(NULL, TEXT("Program requires Windows NT!"), 
		     szAppName, MB_ICONERROR);
	return 0;
    }
    hwnd = ::CreateWindow(szAppName, TEXT("Get System Metrics No. 3"),
			  WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
			  CW_USEDEFAULT, CW_USEDEFAULT,
			  CW_USEDEFAULT, CW_USEDEFAULT,
			  NULL, NULL, hInstance, NULL);
    ::ShowWindow(hwnd, iCmdShow);
    ::UpdateWindow(hwnd);
     
    while (::GetMessage(&msg, NULL, 0, 0)) {
	::TranslateMessage(&msg);
	::DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    MyWindow *w;
    switch(message) {
    case WM_CREATE:
	w = new MyWindow(hwnd);
	::SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(w));
	return w->onCreate(wParam, lParam);
          
    case WM_SIZE:
	w = reinterpret_cast<MyWindow *>
	    (::GetWindowLongPtr(hwnd, GWLP_USERDATA));
	return w->onSize(wParam, lParam);
          
    case WM_VSCROLL:
	w = reinterpret_cast<MyWindow *>
	    (::GetWindowLongPtr(hwnd, GWLP_USERDATA));
	return w->onVScroll(wParam, lParam);
          
    case WM_MOUSEWHEEL:
	w = reinterpret_cast<MyWindow *>
	    (::GetWindowLongPtr(hwnd, GWLP_USERDATA));
	return w->onMouseWheel(wParam, lParam);

    case WM_HSCROLL:
	w = reinterpret_cast<MyWindow *>
	    (::GetWindowLongPtr(hwnd, GWLP_USERDATA));
	return w->onHScroll(wParam, lParam);

    case WM_PAINT:
	w = reinterpret_cast<MyWindow *>
	    (::GetWindowLongPtr(hwnd, GWLP_USERDATA));
	return w->onPaint(wParam, lParam);
          
    case WM_DESTROY:
	w = reinterpret_cast<MyWindow *>
	    (::GetWindowLongPtr(hwnd, GWLP_USERDATA));
	w->onDestroy(wParam, lParam);
	delete w;
	return 0;
    }
    return ::DefWindowProc(hwnd, message, wParam, lParam);
}
