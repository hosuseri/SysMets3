#include "MyWindow.h"
#include "SysMets.h"

MyWindow::MyWindow(HWND hwnd) : m_handle(hwnd)
{
}

LRESULT MyWindow::onCreate(WPARAM wParam, LPARAM lParam)
{
    HDC hdc = ::GetDC(m_handle);
    HANDLE h = ::GetStockObject(OEM_FIXED_FONT);
    ::SelectObject(hdc, h);
    TEXTMETRIC tm;
    ::GetTextMetrics(hdc, &tm);
    m_xChar = tm.tmAveCharWidth;
    m_xCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * m_xChar / 2;
    m_yChar = tm.tmHeight + tm.tmExternalLeading;
    ::ReleaseDC(m_handle, hdc);
    m_maxWidth = 40 * m_xChar + 22 * m_xCaps;
    return 0;
}

LRESULT MyWindow::onDestroy(WPARAM wParam, LPARAM lParam)
{
    ::PostQuitMessage(0);
    return 0;
}

LRESULT MyWindow::onPaint(WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc = ::BeginPaint(m_handle, &ps);
    HANDLE h = ::GetStockObject(OEM_FIXED_FONT);
    ::SelectObject(hdc, h);
    SCROLLINFO si;
    si.cbSize = sizeof(si);
    si.fMask  = SIF_POS;
    ::GetScrollInfo(m_handle, SB_VERT, &si);
    int iVertPos = si.nPos;
    ::GetScrollInfo(m_handle, SB_HORZ, &si);
    int iHorzPos = si.nPos;
    int iPaintBeg = iVertPos + ps.rcPaint.top / m_yChar;
    iPaintBeg = iPaintBeg >= 0 ? iPaintBeg : 0;
    int iPaintEnd = iVertPos + ps.rcPaint.bottom / m_yChar;
    iPaintEnd = iPaintEnd < NUMLINES ? iPaintEnd : NUMLINES - 1;
    for (int i = iPaintBeg; i <= iPaintEnd; i++) {
	int x = m_xChar * (1 - iHorzPos);
	int y = m_yChar * (i - iVertPos);
	int n = ::lstrlenA(sysmetrics[i].szLabel);
	::TextOutA(hdc, x, y, sysmetrics[i].szLabel, n);
	n = ::lstrlenA(sysmetrics[i].szDesc);
	::TextOutA(hdc, x + 22 * m_xCaps, y, sysmetrics[i].szDesc, n);
	::SetTextAlign(hdc, TA_RIGHT | TA_TOP);
	int v = ::GetSystemMetrics(sysmetrics[i].iIndex);
	char szBuffer[10];
	n = ::wsprintfA(szBuffer, "%5d", v);
	::TextOutA(hdc, x + 22 * m_xCaps + 40 * m_xChar, y, szBuffer, n);
	::SetTextAlign(hdc, TA_LEFT | TA_TOP);
    }
    ::EndPaint(m_handle, &ps);
    return 0;
}

LRESULT MyWindow::onSize(WPARAM wParam, LPARAM lParam)
{
    m_xClient = LOWORD(lParam);
    m_yClient = HIWORD(lParam);

    SCROLLINFO si;
    si.cbSize = sizeof(si);
    si.fMask = SIF_RANGE | SIF_PAGE;
    si.nMin = 0;
    si.nMax = NUMLINES - 1;
    si.nPage = m_yClient / m_yChar;
    ::SetScrollInfo(m_handle, SB_VERT, &si, TRUE);

    si.cbSize = sizeof(si);
    si.fMask = SIF_RANGE | SIF_PAGE;
    si.nMin = 0;
    si.nMax = 2 + m_maxWidth / m_xChar;
    si.nPage = m_xClient / m_xChar;
    ::SetScrollInfo(m_handle, SB_HORZ, &si, TRUE);
    return 0;
}

LRESULT MyWindow::onVScroll(WPARAM wParam, LPARAM lParam)
{
    SCROLLINFO si;
    si.cbSize = sizeof(si);
    si.fMask  = SIF_ALL;
    ::GetScrollInfo(m_handle, SB_VERT, &si);
    int iVertPos = si.nPos;

    switch(LOWORD(wParam)) {
    case SB_TOP:
	si.nPos = si.nMin;
	break;
               
    case SB_BOTTOM:
	si.nPos = si.nMax;
	break;
               
    case SB_LINEUP:
	si.nPos -= 1;
	break;
               
    case SB_LINEDOWN:
	si.nPos += 1;
	break;
               
    case SB_PAGEUP:
	si.nPos -= si.nPage;
	break;
               
    case SB_PAGEDOWN:
	si.nPos += si.nPage;
	break;
               
    case SB_THUMBTRACK:
	si.nPos = si.nTrackPos;
	break;
               
    default:
	break;
    }

    si.fMask = SIF_POS;
    ::SetScrollInfo(m_handle, SB_VERT, &si, TRUE);
    ::GetScrollInfo(m_handle, SB_VERT, &si);
    if (si.nPos == iVertPos)
	return 0;
    ::ScrollWindow(m_handle, 0, m_yChar * (iVertPos - si.nPos), NULL, NULL);
    ::UpdateWindow(m_handle);
    return 0;
}

LRESULT MyWindow::onHScroll(WPARAM wParam, LPARAM lParam)
{
    SCROLLINFO si;
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    ::GetScrollInfo(m_handle, SB_HORZ, &si);
    int iHorzPos = si.nPos;

    switch (LOWORD(wParam)) {
    case SB_LINELEFT:
	si.nPos -= 1;
	break;
               
    case SB_LINERIGHT:
	si.nPos += 1;
	break;
               
    case SB_PAGELEFT:
	si.nPos -= si.nPage;
	break;
               
    case SB_PAGERIGHT:
	si.nPos += si.nPage;
	break;
               
    case SB_THUMBPOSITION:
	si.nPos = si.nTrackPos;
	break;
               
    default :
	break;
    }

    si.fMask = SIF_POS;
    ::SetScrollInfo(m_handle, SB_HORZ, &si, TRUE);
    ::GetScrollInfo(m_handle, SB_HORZ, &si);
    if (si.nPos == iHorzPos)
	return 0;
    ::ScrollWindow(m_handle, m_xChar * (iHorzPos - si.nPos), 0, NULL, NULL);
    ::UpdateWindow(m_handle);
    return 0;
}

LRESULT MyWindow::onMouseWheel(WPARAM wParam, LPARAM lParam)
{
    SCROLLINFO si;
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    ::GetScrollInfo(m_handle, SB_VERT, &si);
    int iVertPos = si.nPos;
    int d = (short)HIWORD(wParam);
    d /= WHEEL_DELTA;
    si.nPos -= d;
    si.nPos = si.nPos >= 0 ? si.nPos : 0;
    si.nPos = si.nPos < NUMLINES ? si.nPos : NUMLINES -1;
    si.fMask = SIF_POS;
    ::SetScrollInfo(m_handle, SB_VERT, &si, TRUE);
    ::GetScrollInfo(m_handle, SB_VERT, &si);
    if (si.nPos == iVertPos)
	return 0;
    ::ScrollWindow(m_handle, 0, m_yChar * (iVertPos - si.nPos), NULL, NULL);
    ::UpdateWindow(m_handle);
    return 0;
}
