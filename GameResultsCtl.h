#include "commonlibs.h"
#include <iostream>

#include "resource.h"
#include "GameState.h"


INT_PTR CALLBACK GameResultsLoop(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        {
            GameState::m_childHWND = hDlg;

            auto hWndText = GetDlgItem(hDlg, IDC_TEXT);
            char selGameText[6];
            GameState::getScoreString(selGameText);

            char scoreText[13];
            sprintf(scoreText, "Score: %s", selGameText);
            SendMessage(hWndText, WM_SETTEXT, 0, (LPARAM)scoreText);

            return (INT_PTR)TRUE;
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
        {

            if (LOWORD(wParam) == IDOK)
            {
                GameState::ResetGameState();
                SendMessage(GameState::m_mainHWND, WM_COMMAND, CMD_OPEN_GAME_SELECTOR, 0);
                return (INT_PTR)TRUE;

                return (INT_PTR)TRUE;
            }
            else if (LOWORD(wParam) == IDCANCEL)
                SendMessage(GameState::m_mainHWND, WM_COMMAND, CMD_QUIT_GAME, 0);

            break;
        }
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
