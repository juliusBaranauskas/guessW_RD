#include "commonlibs.h"
#include <iostream>

#include "resource.h"
#include "GameState.h"


INT_PTR CALLBACK GameLoop(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        {
            GameState::m_childHWND = hDlg;

            auto hWndText = GetDlgItem(hDlg, IDC_STATIC);
            auto selGameText = GameState::getSelectedGameText();

            SendMessage(hWndText, WM_SETTEXT, 0, (LPARAM)selGameText.c_str());

//            auto inputHWND = GetDlgItem(hDlg, IDC_INPUT);
//            SetWindowSubclass(inputHWND, EditInputProc, 0, 0);

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
        if (LOWORD(wParam) == IDOK)
        {
            auto inputHWND = GetDlgItem(hDlg, IDC_INPUT);
            auto txtLen = GetWindowTextLength(inputHWND) + 1;

            // TODO: add check for strings consisting of whitespaces
            if (1 < txtLen) {
                char txtBuf[txtLen];
                GetWindowText(inputHWND, txtBuf, txtLen);

                // Returns whether it was last guess
                if (GameState::guessClicked(txtBuf))
                {
                    SendMessage(GameState::m_mainHWND, WM_COMMAND, CMD_OPEN_GAME_RESULTS, 0);
                    return (INT_PTR)TRUE;
                }
                auto hWndText = GetDlgItem(hDlg, IDC_TEXT);
                char nthGuessText[15];
                sprintf(nthGuessText, "Guess word %d", GameState::GetCurrentGuess() + 1);
                SendMessage(hWndText, WM_SETTEXT, 0, (LPARAM)nthGuessText);

                // Clear textbox
                const char* emptyString = "";
                SetWindowText(inputHWND, emptyString);

                // Focus textbox so it doesn't need to be done by user
                SetFocus(inputHWND);
            }

            return (INT_PTR)TRUE;
        }
        break;

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
