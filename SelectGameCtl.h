#include "commonlibs.h"
#include <iostream>

#include "resource.h"
#include "GameState.h"


INT_PTR CALLBACK GameSelectionLoop(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        {
            GameState::m_childHWND = hDlg;

            auto dropdownHWND = GetDlgItem(hDlg, IDC_GAME_DROPWDOWN);
            auto games = GameState::getVerifiedGames();

            std::string game_names[games.size()];
            for (uint16_t i = 0; i < games.size(); i++)
                game_names[i] = games[i].m_name;

            for (uint16_t i = 0; i < games.size(); i++)
            {
                ComboBox_AddItemData(dropdownHWND, (LPARAM)game_names[i].c_str());
                GameState::addDropdownMap(i, games[i].m_gameId);
            }


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
            auto dropdownHWND = GetDlgItem(hDlg, IDC_GAME_DROPWDOWN);
            auto selIdx = ComboBox_GetCurSel(dropdownHWND);

            if (-1 == selIdx)
                return (INT_PTR)TRUE;

            GameState::setSelectedGameId(GameState::getMappedId(selIdx));
            SendMessage(GameState::m_mainHWND, WM_COMMAND, CMD_OPEN_GAME, 0);

            return (INT_PTR)TRUE;
        }
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
