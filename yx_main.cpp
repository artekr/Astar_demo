#include "yx_grid.h"
#include "yx_node.h"
#include "yx_common.h"

using namespace std;

LRESULT CALLBACK WndProc(HWND hWnd, UNIT uMsg, WPARAM wParam, LPARAM lParam);
void CALLBACK OnTimerProc(HWND paramHWnd, UNIT paramMsg, UNIT_PTR paramID, DWORD paramTime);

CGridControl gGridCol;
CAStar gAstar;
EnumFindStatus gFindStatus;


//the Main function-------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR szCmdLine,
				   int nCmdShow
				   )
{
	//declare the window
	WNDCLASS wc;

	gFindStatus = EFS_NORMAL;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc; //callback function id, invoked by the system
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "Routing";
	//declare end

	//register the window
	RegisterClass(&wc);


	//Create the window
	HWND hWnd;	//create the window handler

	hWnd = CreateWindow("Routing",				//has to be the same with the lpszClassName
						"Routing Algorithm",	//name of window
						WS_OVERLAPPEDWINDOW,	//window style, this is the over lapped window style
						300,					//the initial position of the window in the screen
						100,					//the initial position of the window in the screen
						EDC_ALL_WIDTH,			//specify the width of the window
						EDC_ALL_HEIGHT,			//specify the height of the window
						NULL,					//point to the parent window, NULL means no parent window
						NULL,					//menu handler
						hInstance,				//window related module instance handler
						NULL					//point to the value of the WM_CREATE message
						);

	SetTimer(hWnd, 0, 5, &OnTimerProc); 		//set the Timer

	//display the window
	ShowWindow(hWnd, nCmdShow);

	//update the window
	UpdateWindow(hWnd);

	//declare a message structure
	MSG msg;

	//begin the mesage LOOP
	while(GetMessage(&msg,						//A pointer to an MSG structure that receives message information from the thread's message queue
					 NULL,						//A handle to the window whose messages are to be retrieved. The window must belong to the current thread.
					 0,							//The integer value of the lowest message value to be retrieved.
					 0							//The integer value of the highest message value to be retrieved.
					 ))							//GetMessage returns all available messages (that is, no range filtering is performed).
	{
		TranslateMessage(&msg);					//Translates virtual-key messages into character messages. The character messages are posted to the calling thread's message queue, to be read the next time the thread calls the GetMessage or PeekMessage function.
		DispatchMessage(&msg);					//Dispatches a message to a window procedure. It is typically used to dispatch a message retrieved by the GetMessage function.
	}

	return msg.wParam;

}

//the Timer function----------------------------------------------------------------------------------------
void CALLBACK OnTimerProc(HWND paramHWnd, UNIT paramMsg, UNIT_PTR paramID, DWORD paramTime){

	if(gFindStatus == EFS_FINDING){
		bool bFinish = gAstar.DoFind();
		if(bFinish){
			gFindStatus = EFS_NORMAL;
		}
		InvalidateRect(paramHWnd, NULL, false);
	}
}


//Callback function----------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd,			//message handler
					     UNIT uMsg, 		//message
					     WPARAM wParam, 	//additional message
					     LPARAM lParam 		//additional message
					     )
{
	static SPoint sStartPos;
	static SPoint sEndPos;

	switch(uMsg)
	{
		case WM_PAINT:						//draw the grids
			gGridCol.DrawAllGrids(hWnd);
			break;

		//left mouse, select start point
		case WM_LBUTTONDOWN:
			if(gFindStatus == EFS_NORMAL || gFindStatus == EFS_HAS_START){
				bool bRet = gGridCol.OnMouseLeftDownEvent(hWnd, LOWORD(lParam), HIWORD(lParam));
				if(bRet){
					sStartPos.x = LOWORD(lParam);
					sStartPos.y = HIWORD(lParam);
					gFindStatus = EFS_HAS_START;
				}else{
					gFindStatus = EFS_NORMAL;
				}
			}else if(gFindStatus == EFS_FINDING){
				gAstar.PrintOpenList();
			}
			break;

		//middle mouse, set the barrier
		case WM_MOUSEMOVE:
			if(gFindStatus == EFS_NORMAL){
				if(wParam &MK_MBUTTON){
					gGridCol.OnMouseMiddleDownEvent(hWnd, LOWORD(lParam), HIWORD(lParam));
				}
			}
			break;

		//right mouse, select end point
		case WM_RBUTTONDOWN:
			if(gFindStatus == EFS_HAS_START){
				bool bRet = gGridCol.OnMouseRightDownEvent(hWnd, LOWORD(lParam), HIWORD(lParam));
				if(bRet){
					sEndPos.x = LOWORD(lParam);
					sEndPos.y = HIWORD(lParam);
					SPoint stStartGridXY = gGridCol.getGridXY(sStartPos);
					SPoint stEndGridXY   = gGridCol.getGridXY(sEndPos);
					gAstar.Start(stStartGridXY, stEndGridXY, & gGridCol.getGridList());
					gFindStatus = EFS_FINDING;
				}
			}
			break;

		//close the window
		case WM_CLOSE:
			if(IDYES == MessageBox(hWnd, "Are you sure to quit?", "Quit", MB_YESNO)){
				DestroyWindow(hWnd);
			}
			break;

		//close the application
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);

	}
	
	return 0;
}


































