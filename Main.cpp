#define UNICODE
#include "Windows.h"
#include <string>

const wchar_t Calculate[] = L"kalkulator";

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

HWND hEditResult; 

double firstNumber = 0;      
double secondNumber = 0;      
wchar_t currentOperation = 0; 

void SetResultText(HWND hwnd, double value)
{
    wchar_t buffer[256];
    swprintf_s(buffer, 256, L"%.2f", value); 
    SetWindowText(hEditResult, buffer);
}

void AppendText(HWND hwnd, int number)
{
    wchar_t buffer[256];
    GetWindowText(hwnd, buffer, 256);

  
    if (wcscmp(buffer, L"0") == 0)
    {
        swprintf_s(buffer, 256, L"%d", number);
    }
    else
    {
        size_t len = wcslen(buffer);
        swprintf_s(buffer + len, 256 - len, L"%d", number); 
    }
    SetWindowText(hwnd, buffer);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX winClass = { 0 };
    winClass.cbSize = sizeof(WNDCLASSEX);
    winClass.style = CS_HREDRAW | CS_VREDRAW;
    winClass.lpfnWndProc = WndProc;
    winClass.hInstance = hInstance;
    winClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    winClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    winClass.lpszClassName = Calculate;
    winClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&winClass))
    {
        MessageBox(NULL, L"B³¹d rejestracji tego okna", L"B³¹d", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }

    HWND hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, Calculate, L"Kalkulator", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 400, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, L"B³¹d otwarcia okna", L"B³¹d", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG Msg;
    while (GetMessage(&Msg, NULL, 0, 0))
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
    
        hEditResult = CreateWindowEx(0, L"EDIT", L"0",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
            20, 20, 250, 40, hwnd, NULL, GetModuleHandle(NULL), NULL);

      
        const wchar_t* buttonLabels[] = { L"7", L"8", L"9", L"+", L"4", L"5", L"6", L"-", L"1", L"2", L"3", L"*", L"0", L"C", L"=", L"/" };
        int buttonIds[] = { 1007, 1008, 1009, 1010, 1004, 1005, 1006, 1011, 1001, 1002, 1003, 1012, 1000, 1013, 1014, 1015 };

        int xPos = 20, yPos = 80;

    
        for (int i = 0; i < 16; i++)
        {
            CreateWindowEx(0, L"BUTTON", buttonLabels[i],
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                xPos, yPos, 50, 50, hwnd, (HMENU)buttonIds[i], GetModuleHandle(NULL), NULL);

            xPos += 60;
            if ((i + 1) % 4 == 0) 
            {
                xPos = 20;
                yPos += 60;
            }
        }
        break;
    }

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        wchar_t buffer[256];
        GetWindowText(hEditResult, buffer, 256);

        if (wmId >= 1000 && wmId <= 1009)  
        {
            AppendText(hEditResult, wmId - 1000);
        }
        else if (wmId == 1010 || wmId == 1011 || wmId == 1012 || wmId == 1015)  
        {
            firstNumber = _wtof(buffer); 
            currentOperation = wmId;      
            SetWindowText(hEditResult, L"0"); 
        }
        else if (wmId == 1014)  
        {
            secondNumber = _wtof(buffer); 
            double result = 0.0;

            switch (currentOperation)
            {
            case 1010:  
                result = firstNumber + secondNumber;
                break;
            case 1011:  
                result = firstNumber - secondNumber;
                break;
            case 1012:  
                result = firstNumber * secondNumber;
                break;
            case 1015:  
                if (secondNumber != 0)
                {
                    result = firstNumber / secondNumber;
                }
                else
                {
                    MessageBox(hwnd, L"Nie mo¿na dzieliæ przez zero!", L"B³¹d", MB_OK | MB_ICONERROR);
                    result = 0;
                }
                break;
            }

            SetResultText(hEditResult, result);  
        }
        else if (wmId == 1013)  
        {
            SetWindowText(hEditResult, L"0");  
            firstNumber = 0;
            secondNumber = 0;
            currentOperation = 0; 
        }
        break;
    }

    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
