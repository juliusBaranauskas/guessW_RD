#include "commonlibs.h"
#include <iostream>

#include "resource.h"
#include "GameLoader.h"
#include "GameState.h"
#include "SelectUsernameCtl.h"
#include "SelectGameCtl.h"
#include "GameCtl.h"
#include "GameResultsCtl.h"


//LRESULT CALLBACK EditInputProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
//{
//    if (uMsg == WM_CHAR && wParam == VK_RETURN)
//    {
//        std::cout << "ENTERINO CLICKO\n";
//        return 0;
//    }
//    LRESULT lRes = DefSubclassProc(hWnd, uMsg, wParam, lParam);
//    if (uMsg == WM_DESTROY)
//        RemoveWindowSubclass(hWnd, EditInputProc, 0);
//    return lRes;
//}


BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_INITDIALOG:
        {
            GameState::m_mainHWND = hwndDlg;
            auto hParent = hwndDlg;
            auto childH = GetDlgItem(hParent, IDC_CHILDSPACE);
            CreateDialog(GameState::m_hInstance, MAKEINTRESOURCE(IDD_USERNAME_DLG), childH, SelectUsernameLoop);
            if (nullptr == childH)
                std::cout << "Child not yet instantiated" << std::endl;

             ShowWindow(GameState::m_childHWND, SW_SHOW);
            //SetParent(GameState::m_childHWND, childH);
            return TRUE;
        }
        case WM_CLOSE:
        {
            EndDialog(hwndDlg, 0);
            return TRUE;
        }

        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case CMD_OPEN_GAME_SELECTOR:
                {
                    ShowWindow(GameState::m_childHWND, SW_HIDE);
                    DestroyWindow(GameState::m_childHWND);

                    auto hWndGroupbox = GetDlgItem(hwndDlg, IDC_CHILDSPACE);
                    CreateDialog(GameState::m_hInstance, MAKEINTRESOURCE(IDD_GAME_SEL_DLG), hWndGroupbox, GameSelectionLoop);
                    ShowWindow(GameState::m_childHWND, SW_SHOW);
                    break;
                }
                case CMD_OPEN_GAME:
                {
                    ShowWindow(GameState::m_childHWND, SW_HIDE);
                    DestroyWindow(GameState::m_childHWND);

                    auto hWndGroupbox = GetDlgItem(hwndDlg, IDC_CHILDSPACE);
                    CreateDialog(GameState::m_hInstance, MAKEINTRESOURCE(IDD_GAME_DLG), hWndGroupbox, GameLoop);
                    ShowWindow(GameState::m_childHWND, SW_SHOW);

                    break;
                }
                case CMD_OPEN_GAME_RESULTS:
                {
                    ShowWindow(GameState::m_childHWND, SW_HIDE);
                    DestroyWindow(GameState::m_childHWND);

                    auto hWndGroupbox = GetDlgItem(hwndDlg, IDC_CHILDSPACE);
                    CreateDialog(GameState::m_hInstance, MAKEINTRESOURCE(IDD_SCORE_DLG), hWndGroupbox, GameResultsLoop);
                    ShowWindow(GameState::m_childHWND, SW_SHOW);
                    break;
                }
                case CMD_QUIT_GAME:
                {
                    PostQuitMessage(0);
                    break;
                }
            }
            return TRUE;
        }
    }
    return FALSE;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    GameState::m_hInstance = hInstance;
    InitCommonControls();

    return DialogBox(GameState::m_hInstance, MAKEINTRESOURCE(IDD_CONTAINER), NULL, (DLGPROC)DlgMain);
}
