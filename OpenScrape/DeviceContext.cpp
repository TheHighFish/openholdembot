#include "stdafx.h"
#include "DeviceContext.h"
#include <unordered_map>

bool CheckIfDCReturnsBlackScreen(const HWND hWnd) {
	// If this window is not visible, then return
	if (!::IsWindowVisible(hWnd)) { return false; }

	// Static unordered_map that stores the hwnd of windows that have been checked,
	// and if they returned true or not, so we don't have to check every pixel each time.
	static std::unordered_map<HWND, bool> checkedWindows;
	// window returns an iterator to the element if the hwnd is found,
	// or returns the same value as checkedWindows.end() if it isn't found.
	auto window = checkedWindows.find(hWnd);
	if (window != checkedWindows.end()) return checkedWindows[hWnd];

	HDC hdc = ::GetDC(hWnd);

	RECT client_rect;
	::GetClientRect(hWnd, &client_rect);
	int width = client_rect.right - client_rect.left;
	int height = client_rect.bottom - client_rect.top;

	// Allocate heap space for BITMAPINFO
	BITMAPINFO	*bmi = NULL;
	int			info_len = sizeof(BITMAPINFOHEADER) + 1024;
	bmi = (BITMAPINFO *) ::HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, info_len);

	// Load entire foreground pixel array into memory
	// Populate BITMAPINFOHEADER
	HBITMAP hbm = (HBITMAP)GetCurrentObject(hdc, OBJ_BITMAP);
	bmi->bmiHeader.biSize = sizeof(bmi->bmiHeader);
	bmi->bmiHeader.biBitCount = 0;
	::GetDIBits(hdc, hbm, 0, 0, NULL, bmi, DIB_RGB_COLORS);

	// Get the actual argb bit information
	bmi->bmiHeader.biHeight = -bmi->bmiHeader.biHeight;
	LPBYTE pBits = new BYTE[bmi->bmiHeader.biSizeImage];
	::GetDIBits(hdc, hbm, 0, height, pBits, bmi, DIB_RGB_COLORS);

	// Scanning for non-zero bytes
	LPCOLORREF pPixel = reinterpret_cast<LPCOLORREF>(pBits);
	LPCOLORREF pEndOfImage = pPixel + (width * height);

	for (; pPixel < pEndOfImage; pPixel++)
		if ((*pPixel) != 0) break;

	// Clean up
	HeapFree(GetProcessHeap(), NULL, bmi);
	delete[]pBits;
	DeleteObject(hbm);
	::ReleaseDC(hWnd, hdc);

	// Remove windows that no longer exist from the list
	for (auto it = checkedWindows.begin(); it != checkedWindows.end(); ++it) {
		if (!IsWindow(it->first))
			it = checkedWindows.erase(it);
	}

	// Add hwnd to checkedWindows	 
	checkedWindows[hWnd] = (pPixel == pEndOfImage);

	// True if we've checked all pixels and they are all black	
	return pPixel == pEndOfImage;
}

void BitBltFromClientOrDesktopDC(HDC hdc, int width, int height, HDC hdcSrc, int x, int y, const HWND hWnd) {
	if (CheckIfDCReturnsBlackScreen(hWnd)) {
		POINT client_to_screen_upper_left_points;
		client_to_screen_upper_left_points.x = x;
		client_to_screen_upper_left_points.y = y;
		::ClientToScreen(hWnd, &client_to_screen_upper_left_points);

		BitBlt(hdc, 0, 0, width, height, hdcSrc,
			client_to_screen_upper_left_points.x,
			client_to_screen_upper_left_points.y,
			SRCCOPY | CAPTUREBLT);
	}
	else {
		BitBlt(hdc, 0, 0, width, height, hdcSrc, x, y, SRCCOPY);
	}
}