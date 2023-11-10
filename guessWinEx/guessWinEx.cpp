// guessWinEx.cpp : Définit le point d'entrée de l'application.
//

#include "framework.h"
#include "guessWinEx.h"

#define MAX_LOADSTRING 100
#define MAX_LOADPATH 2048

// Variables globales :
HINSTANCE hInst;                                // instance actuelle
WCHAR szTitle[MAX_LOADSTRING];                  // Texte de la barre de titre
WCHAR szBTitle[MAX_LOADSTRING];                 // Texte de la barre de fenetre Binders
WCHAR szCTitle[MAX_LOADSTRING];                 // Titre de la fenetre Cards
WCHAR szWindowClass[MAX_LOADSTRING];            // nom de la classe de fenêtre principale
WCHAR szBWndClass[MAX_LOADSTRING];              // nom de la classe de la fenêtre Binders

using namespace std;
namespace fs = std::filesystem;
//using namespace ios;

// Déclarations anticipées des fonctions incluses dans ce module de code :
ATOM                MyRegisterClass(HINSTANCE hInstance);
ATOM                RegisterMiniWindow(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);

//added functions:

//CRUD: Binders

INT                 createBinderW(WCHAR *wnameBinder, WCHAR* wpath);
void                alert(wchar_t*);
INT                 DeleteBinder(int fBinder);
INT                 UpdateBinder(int fBinder);
wchar_t here[] = L"here!";


INT_PTR CALLBACK    Outils(HWND, UINT, WPARAM, LPARAM);

INT_PTR CALLBACK    NewFileDialog(HWND, UINT, WPARAM, LPARAM);

//end added functions.

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    ATOM cm=0, cBframe=0;
    // TODO: Placez le code ici.




    // Initialise les chaînes globales
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GUESSWINEX, szWindowClass, MAX_LOADSTRING);

    LoadStringW(hInstance, IDS_BINDERWND_TITLE, szBTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDS_CARDWND_TITLE, szCTitle, MAX_LOADSTRING);

    cm = MyRegisterClass(hInstance);
    cBframe = RegisterMiniWindow(hInstance);

    // Effectue l'initialisation de l'application :
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GUESSWINEX));

    MSG msg;

    // Boucle de messages principale :
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FONCTION : MyRegisterClass()
//
//  OBJECTIF : Inscrit la classe de fenêtre.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW ;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GUESSWINEX));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GUESSWINEX);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}



ATOM RegisterMiniWindow(HINSTANCE hInstance) {
    WNDCLASSEX wB;

    wB.cbSize = sizeof(wB);
    wB.style = CS_HREDRAW | CS_VREDRAW;
    wB.lpfnWndProc = BinderFrameProc;
    wB.cbClsExtra = 0;
    wB.cbWndExtra = 0;
    wB.hInstance = hInstance;
    wB.lpszClassName = szBWndClass;

    return RegisterClassExW(&wB);
}

//
//   FONCTION : InitInstance(HINSTANCE, int)
//
//   OBJECTIF : enregistre le handle d'instance et crée une fenêtre principale
//
//   COMMENTAIRES :
//
//        Dans cette fonction, nous enregistrons le handle de l'instance dans une variable globale, puis
//        nous créons et affichons la fenêtre principale du programme.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Stocke le handle d'instance dans la variable globale

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   HWND bindersWnd = CreateWindowW(szWindowClass, szBTitle, WS_CHILD | WS_VSCROLL | WS_SYSMENU |WS_VISIBLE, 2, 2, 200, 500, nullptr, nullptr, hInstance, nullptr);
   //HWND cardsWnd = CreateWindow( 
   
   SetParent(bindersWnd, hWnd);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   ShowWindow(bindersWnd, nCmdShow);
   UpdateWindow(hWnd);
   UpdateWindow(bindersWnd);

   return TRUE;
}

//
//  FONCTION : WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  OBJECTIF : Traite les messages pour la fenêtre principale.
//
//  WM_COMMAND  - traite le menu de l'application
//  WM_PAINT    - Dessine la fenêtre principale
//  WM_DESTROY  - génère un message d'arrêt et retourne
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Analyse les sélections de menu :
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_NEWFILE:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_NEWFILEBOX), hWnd, NewFileDialog);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

            HBRUSH hbrFrame = CreateSolidBrush(0x00444444);
            RECT temp(0,0,0,0);

            if (GetWindowRect(hWnd, &temp)) {

                //to initialise frames (functions might be have to be made)
                RECT cpy = temp;
                
                cpy.right = 340;
                cpy.bottom = cpy.bottom *3/5;
                cpy.top = 2;
                cpy.left = 2;
                
                //int frameBinder = FrameRect(hdc, &cpy, hbrFrame);

                temp.bottom = temp.bottom *3/5;
                temp.right = temp.right - 102;
                temp.top = 2;
                temp.left = cpy.right +5;

                int frameCards = FrameRect(hdc, &temp, hbrFrame);


            }

            //int frameBinder;
            //RECT frameBinderPos(3, 3, 250, 500);
            //RECT* pR = (RECT(3, 3, 120, 250));

            // TODO: Ajoutez ici le code de dessin qui utilise hdc...

            

            //frameBinder = FrameRect(hdc, &frameBinderPos, hbrFrame);

            EndPaint(hWnd, &ps);

        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//Procedure fenetre enfant
LRESULT CALLBACK BinderFrameProc(HWND hWnd, UINT message, WPARAM wP, LPARAM lP) {
    switch (message) {
    case WM_COMMAND:
        {
            int wmBwd = LOWORD(lP);
            switch (wmBwd) {
            case IDM_NEWFILE:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_NEWFILEBOX), hWnd, NewFileDialog);
                break;
            }
        }
        break;
    case WM_PAINT:
        {
        //pour verifier le fonctionnement
            PAINTSTRUCT psB;
            HDC hdcB = BeginPaint(hWnd, &psB);

            FillRect(hdcB, &psB.rcPaint, (HBRUSH)(0x00FF0000));

            EndPaint(hWnd, &psB);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wP, lP);
    }
    return 0;
}

// Gestionnaire de messages pour la boîte de dialogue À propos de.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

//Gestionnaire de la boîte de dialogue New File
INT_PTR CALLBACK NewFileDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK) {
            HWND nameEdit = GetDlgItem(hDlg, 201);
            HWND pathEdit = GetDlgItem(hDlg, 202);

            WCHAR * lpStringName = (wchar_t *) malloc(MAX_LOADSTRING);
            WCHAR * lpStringPath = (wchar_t*) malloc(MAX_LOADPATH);
            int lengthName = GetWindowTextLength(nameEdit);
            int lengthPath = GetWindowTextLength(pathEdit);
            if ((lpStringName != NULL)&& (lpStringPath !=NULL)){
            //if ((lpStringName != NULL) && (lpStringName[0] == '\0') && ((lpStringPath != NULL) && (lpStringPath[0] == '\0'))) {
                //alert(here);
                if ((GetWindowText(nameEdit, lpStringName, lengthName+16) > 0) && (GetWindowText(pathEdit, lpStringPath, lengthPath+16) > 0)) {

                    //wcout << lpStringName[0] << L"\n";
                    createBinderW(lpStringName, lpStringPath);

                    wchar_t mes[] = L"Success!";
                    alert(mes);
                    //alert(L"Binder Créé\n");

                    free(lpStringName);
                    free(lpStringPath);
                    //EndDialog(hDlg, LOWORD(wParam));
                    EndDialog(hDlg, LOWORD(wParam));
                    return (INT_PTR)TRUE;
                }
            }
            
            free(lpStringName);
            free(lpStringPath);
            return (INT_PTR)TRUE;

        }

        if (LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
    }

    return (INT_PTR)FALSE;
}






//Création d'un nouveau Binder sous format .csv (version wchar_t)
INT createBinderW(WCHAR* wnameBinder, WCHAR* wpath)
{
    int status = 0;

    std::ofstream newBinder;

    char* path = (char*) malloc(MAX_LOADPATH+16);
    
    const wchar_t* wpathadd = wpath;
    size_t lenP = wcslen(wpath)+16;
    size_t l = wcstombs(path, wpathadd, lenP);
    fs::path fsPath{ path };
//    printf("%s",path);

    char* name = (char*) malloc(MAX_LOADSTRING+16);
    const wchar_t* wnameadd = wnameBinder;
    size_t lenN = wcslen(wnameBinder) + 16;
    size_t n = wcstombs(name, wnameadd, lenN);

    //std::string pathStr;
    cout << path << endl;
    if (!(fs::is_directory(path))) {
        wcout << _T("Wrong Path : does not exist\n");
        
        //alert(L"Wrong Path : does not exist\n");
        return -1;
    }

    if ((name != NULL) && (name[0] == '\0')) {
        wcout << _T("Wrong Name : Blank Name");
        wchar_t mes[] = L"WP";
        alert(mes);
        return -2;
    }


    char* newBinderFile;
    newBinderFile = (char*)malloc(strlen(name) + strlen(path) + 1 + 4);
    strcpy(newBinderFile, path);
    strcat(newBinderFile, "\\");
    strcat(newBinderFile, name);
    strcat(newBinderFile, ".csv");

    newBinder.open(newBinderFile, ios::app | ios::out);

    newBinder.close();

    free(path);
    free(name);
    //free(newBinderFile);

    return 0;


    //if (status == -1) {
        //return -1;
    //}

    //return 0;
}

//alert for error/warning purpose
void alert( wchar_t item[])
{

    MessageBoxW(NULL, item, L"Message", MB_OK | MB_ICONINFORMATION);
}