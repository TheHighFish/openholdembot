// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: To handle possible issues that arise
//   when we try to scrape certain device contexts.
//
//******************************************************************************
//
// Created after receiving many complaints about black scraper output. 
// User "coli" pointed out a possible solution from stack overflow and gave a partial patch:
// http://www.maxinmontreal.com/forums/viewtopic.php?p=159348#p159348
//
// Stack Overflow post:
// https://stackoverflow.com/questions/53379482/bitblt-captured-pixels-are-all-zero-bgra-when-using-hdc-of-another-app
//
// "GetDC can be used to take screen shot of the client area in native Win32 
// applications only.That includes programs like Notepad.Or you can use GetWindowDC 
// to take screenshot of the whole window.
// But applications made with frameworks such as electron app, QT, WPF... will print 
// black screen in response to GetDC or GetWindowDC.The only way around it is to make 
// sure the target application is visible and take a screenshot of desktop at the 
// specific coordinates where the target application is at.
// Windows GDI functions generally ignore alpha channel.But if you retrieve 
// the screenshot in 32 - bit, then GetDIBits will set the all alpha values set 
// to 255 (at least in Windows 10)."
//
//******************************************************************************

#ifndef INC_DEVICE_CONTEXT_H
#define INC_DEVICE_CONTEXT_H

bool CheckIfDCReturnsBlackScreen(const HWND hWnd);
void BitBltFromClientOrDesktopDC(HDC hdc, int width, int height, HDC hdcSrc, int x, int y, const HWND hWnd);

#endif INC_DEVICE_CONTEXT_H
