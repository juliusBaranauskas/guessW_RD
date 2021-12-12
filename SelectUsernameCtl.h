#include "commonlibs.h"

#include "resource.h"
#include "GameState.h"

#include <iostream>


INT_PTR CALLBACK SelectUsernameLoop(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        {
            GameState::m_childHWND = hDlg;
            return (INT_PTR)TRUE;
        }
    case WM_CLOSE:
        {
            std::cout << "CLOSE windwos msg received" << std::endl;
            return 0;
        }
    case WM_DESTROY:
        {
            // Post the WM_QUIT message to
            // quit the application terminate.

            std::cout << "DESTROY window msg received" << std::endl;
            // PostQuitMessage(0);
            EndDialog(hDlg, 0);

            return 0;
        }
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            auto inputHWND = GetDlgItem(hDlg, IDC_INPUT);
            auto txtLen = GetWindowTextLength(inputHWND) + 1;

            // TODO: add check for strings consisting of whitespaces
            if (1 < txtLen) {
                char txtBuf[txtLen];
                GetWindowText(inputHWND, txtBuf, txtLen);

                GameState::setSelectedUsername(txtBuf);
                SendMessage(GameState::m_mainHWND, WM_COMMAND, CMD_OPEN_GAME_SELECTOR, 0);
            }

            return (INT_PTR)TRUE;
        }
        break;
//    case WM_DRAWITEM:
//    {
//        LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
//        if (pDIS->CtlID == IDC_STATIC_IMG)
//        {
//            HICON hIcon = (HICON)LoadImage(GetModuleHandle(0), (LPCSTR)L"C:\\Users\\juliu\\Pictures\\logo_picture.png" /*MAKEINTRESOURCE(ICO_LOGO)*/, IMAGE_ICON, 32, 32, LR_LOADFROMFILE /*LR_LOADTRANSPARENT*/);
//            DrawIconEx(pDIS->hDC, 0, 0, hIcon, 0, 0, 0, NULL, DI_NORMAL);
//            DestroyIcon(hIcon);
//            return TRUE;
//        }
//        break;
//    }
    case WM_PAINT:
        {
            // Start Gdiplus
            Gdiplus::GdiplusStartupInput gdiplusStartupInput;
            ULONG_PTR gdiplusToken;
            Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

            // Load the image
            Gdiplus::Image* image = Gdiplus::Image::FromFile( L"logo_picture.jpg" );

            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hDlg, &ps);
            Gdiplus::Graphics graphics(hdc);

            if (Gdiplus::Status::Ok != graphics.DrawImage(image, -5, 0))
                std::cout << "Failed to load game logo" << std::endl;

            EndPaint(hDlg, &ps);

            // delete the image when done
            delete image;
            // image = 0;

            // Shutdown Gdiplus
            // Gdiplus::GdiplusShutdown(gdiplusToken);
            return (INT_PTR)TRUE;
        }
    }
    return (INT_PTR)FALSE;
}
