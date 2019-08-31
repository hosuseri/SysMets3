#ifndef __MY_WINDOW_H__
#define __MY_WINDOW_H__

#include <windows.h>

class MyWindow {
public:
    MyWindow(HWND hwnd);
    LRESULT onCreate(WPARAM wParam, LPARAM lParam);
    LRESULT onDestroy(WPARAM wParam, LPARAM lParam);
    LRESULT onPaint(WPARAM wParam, LPARAM lParam);
    LRESULT onSize(WPARAM wParam, LPARAM lParam);
    LRESULT onVScroll(WPARAM wParam, LPARAM lParam);
    LRESULT onHScroll(WPARAM wParam, LPARAM lParam);
    LRESULT onMouseWheel(WPARAM wParam, LPARAM lParam);

private:
    MyWindow() {}

protected:
    HWND m_handle;
    int  m_xChar;
    int m_xCaps;
    int m_yChar;
    int m_xClient;
    int m_yClient;
    int m_maxWidth;
};

#endif  //  __MY_WINDOW_H__
