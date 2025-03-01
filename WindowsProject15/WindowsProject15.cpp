#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <string>

using namespace std;

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
void CreateButton(HWND hWnd, int x, int y, int width, int height, int id, LPCWSTR text);
void UpdateButtonLabel(HWND hWnd, int id);

bool HasWinner();
void RestartGame(HWND hWnd);

bool isCrossTurn = true;
int turnCounter = 0;

wstring cellState[9] = { L"-", L"-", L"-", L"-", L"-", L"-", L"-", L"-", L"-" };

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) 
    {
    case WM_INITDIALOG:

        SetWindowPos(hWnd, HWND_TOP, 800, 250, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

        CreateButton(hWnd, 38, 38, 75, 75, 1, L" ");

        CreateButton(hWnd, 38, 128, 75, 75, 2, L" ");

        CreateButton(hWnd, 38, 218, 75, 75, 3, L" ");

        CreateButton(hWnd, 128, 38, 75, 75, 4, L" ");

        CreateButton(hWnd, 128, 128, 75, 75, 5, L" ");

        CreateButton(hWnd, 128, 218, 75, 75, 6, L" ");

        CreateButton(hWnd, 218, 38, 75, 75, 7, L" ");

        CreateButton(hWnd, 218, 128, 75, 75, 8, L" ");

        CreateButton(hWnd, 218, 218, 75, 75, 9, L" ");

        SendMessage(GetDlgItem(hWnd, IDC_CHECK1), BM_SETCHECK, isCrossTurn ? BST_CHECKED : BST_UNCHECKED, 0);

        return TRUE;

    case WM_COMMAND:
    {
        int id = LOWORD(wParam);

        if (id >= 1 && id <= 9 && cellState[id - 1] == L"-")
        {
            UpdateButtonLabel(hWnd, id - 1);
            turnCounter++;

            if (HasWinner()) 
            {
                MessageBox(hWnd, isCrossTurn ? L"Победа крестиков" : L"Победа ноликов", L"Конец", MB_OK);
                
                RestartGame(hWnd);
            }

            if (turnCounter == 9)
            {
                MessageBox(hWnd, L"Ничья", L"Конец", MB_OK);
                
                RestartGame(hWnd);
            }
        }

        if (id == IDC_BUTTON1) RestartGame(hWnd);

        if (turnCounter > 0 && id == IDC_CHECK1) return TRUE;

        if (id == IDC_CHECK1)
        {
            isCrossTurn = SendMessage(GetDlgItem(hWnd, IDC_CHECK1), BM_GETCHECK, 0, 0) == BST_CHECKED;
        }

        return TRUE;
    }

    case WM_CLOSE:

        EndDialog(hWnd, 0);
        return TRUE;
    }
    return FALSE;
}

void CreateButton(HWND hWnd, int x, int y, int width, int height, int id, LPCWSTR text)
{
    CreateWindow(L"BUTTON", text, WS_CHILD | WS_VISIBLE | BS_CENTER | WS_BORDER, x, y, width, height, hWnd, (HMENU)id, GetModuleHandle(NULL), NULL);
}

void UpdateButtonLabel(HWND hWnd, int id)
{
    if (cellState[id] != L"-") return;

    cellState[id] = isCrossTurn ? L"○" : L"✕";

    isCrossTurn = !isCrossTurn;

    HWND hButton = GetDlgItem(hWnd, id + 1);
    SetWindowText(hButton, cellState[id].c_str());
}

int winPatterns[8][3] =
{
    { 0, 1, 2 }, { 3, 4, 5 }, { 6, 7, 8 }, { 0, 4, 8 }, { 2, 4, 6 }, { 0, 3, 6 }, { 1, 4, 7 }, { 2, 5, 8 },
};

bool HasWinner()
{
    for (int i = 0; i < 8; i++)
    {
        int* combo = winPatterns[i];

        if (cellState[combo[0]] != L"-" &&

            cellState[combo[0]] == cellState[combo[1]] &&
            cellState[combo[0]] == cellState[combo[2]])
        {
            return true;
        }
    }
    return false;
}

void RestartGame(HWND hWnd)
{
    for (int i = 0; i < 9; i++)
    {
        cellState[i] = L"-";

        HWND hButton = GetDlgItem(hWnd, i + 1);

        SetWindowText(hButton, L"-");
    }

    isCrossTurn = SendMessage(GetDlgItem(hWnd, IDC_CHECK1), BM_GETCHECK, 0, 0) == BST_CHECKED;
    turnCounter = 0;
}