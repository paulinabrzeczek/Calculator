#define UNICODE
#include "Windows.h"
#include <string>

const wchar_t Calculate[] = L"kalkulator";

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

HWND hEditResult; // Pole tekstowe na wynik

double firstNumber = 0.0;       // Pierwsza liczba
double secondNumber = 0.0;      // Druga liczba
int currentOperation = 0;       // Obecny operator

void SetResultText(HWND hwnd, double value)
{
    wchar_t buffer[256];
    swprintf_s(buffer, 256, L"%.2f", value); // Konwersja double do stringa z dwoma miejscami po przecinku
    SetWindowText(hwnd, buffer);
}

void AppendText(HWND hwnd, const wchar_t* text)
{
    wchar_t buffer[256];
    GetWindowText(hwnd, buffer, 256);

    // Jeœli pole tekstowe pokazuje 0, zast¹p ca³kowicie pole tekstowe
    if (wcscmp(buffer, L"0") == 0)
    {
        SetWindowText(hwnd, text); // Zast¹p ca³kowicie pole tekstowe
    }
    else
    {
        // Dodaj tekst na koñcu
        wchar_t newText[256];
        wcscpy_s(newText, 256, buffer); // Skopiuj dotychczasowy tekst
        wcscat_s(newText, 256, text);    // Dodaj nowy tekst
        SetWindowText(hwnd, newText);    // Ustaw nowy tekst
    }
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
        // Utwórz pole tekstowe
        hEditResult = CreateWindowEx(0, L"EDIT", L"0",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
            20, 20, 250, 40, hwnd, NULL, GetModuleHandle(NULL), NULL);

        // Tworzenie przycisków (cyfry 0-9 i operacje)
        const wchar_t* buttonLabels[] = { L"7", L"8", L"9", L"+",
                                           L"4", L"5", L"6", L"-",
                                           L"1", L"2", L"3", L"*",
                                           L"0", L"C", L"=", L"/",
                                           L"," }; // Przycisk przecinka
        int buttonIds[] = { 1007, 1008, 1009, 1010,
                            1004, 1005, 1006, 1011,
                            1001, 1002, 1003, 1012,
                            1000, 1013, 1014, 1015,
                            1016 }; // ID dla przycisku przecinka

        int xPos = 20, yPos = 80;

        // Tworzenie przycisków dla cyfr i operatorów
        for (int i = 0; i < 17; i++)
        {
            CreateWindowEx(0, L"BUTTON", buttonLabels[i],
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                xPos, yPos, 50, 50, hwnd, (HMENU)buttonIds[i], GetModuleHandle(NULL), NULL);

            xPos += 60;
            if ((i + 1) % 4 == 0) // Przenoszenie do nastêpnego wiersza po 4 przyciskach
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

        if (wmId >= 1000 && wmId <= 1009)  // Obs³uga cyfr (0-9)
        {
            wchar_t text[2];
            swprintf_s(text, 2, L"%d", wmId - 1000);
            AppendText(hEditResult, text);
        }
        else if (wmId == 1016) // Obs³uga przecinka
        {
            if (wcschr(buffer, L',') == NULL) // Sprawdzenie, czy przecinek ju¿ istnieje
            {
                AppendText(hEditResult, L",");
            }
        }
        else if (wmId == 1010 || wmId == 1011 || wmId == 1012 || wmId == 1015)  // Operatorzy (+, -, *, /)
        {
            // Zamiana przecinka na kropkê w przypadku, gdy u¿ytkownik u¿ywa zmiennoprzecinkowych
            for (int i = 0; buffer[i] != '\0'; i++) {
                if (buffer[i] == L',') {
                    buffer[i] = L'.'; // Zmiana przecinka na kropkê
                }
            }

            firstNumber = _wtof(buffer);  // Zapisanie pierwszej liczby
            currentOperation = wmId;      // Zapisanie operatora
            SetWindowText(hEditResult, L"0"); // Resetowanie pola tekstowego, aby wprowadziæ drug¹ liczbê
        }
        else if (wmId == 1014)  // Obs³uga przycisku "="
        {
            // Zamiana przecinka na kropkê w przypadku, gdy u¿ytkownik u¿ywa zmiennoprzecinkowych
            for (int i = 0; buffer[i] != '\0'; i++) {
                if (buffer[i] == L',') {
                    buffer[i] = L'.'; // Zmiana przecinka na kropkê
                }
            }

            secondNumber = _wtof(buffer); // Pobranie drugiej liczby
            double result = 0.0;

            switch (currentOperation)
            {
            case 1010:  // Dodawanie
                result = firstNumber + secondNumber;

                break;
            case 1011:  // Odejmowanie
                result = firstNumber - secondNumber;
                break;
            case 1012:  // Mno¿enie
                result = firstNumber * secondNumber;
                break;
            case 1015:  // Dzielenie
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

            SetResultText(hEditResult, result);  // Wyœwietlenie wyniku
        }
        else if (wmId == 1013)  // Obs³uga przycisku "C" (Clear)
        {
            SetWindowText(hEditResult, L"0");  // Resetowanie wyniku
            firstNumber = 0.0;
            secondNumber = 0.0;
            currentOperation = 0; // Resetowanie operatora
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
