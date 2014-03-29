#include "yx_grid.h"
#include "yx_common.h"


namespace yx
{
	//get the position of the left top of each grid
	SPoint CGridView::GetLeftTop(int paramGridX, int paramGridY){
		
		SPoint stRet;
		stRet.x = paramGridX * (EDC_W + EDC_LINE_WIDTH) + EDC_LINE_WIDTH;
		stRet.y = paramGridY * (EDC_H + EDC_LINE_WIDTH) + EDC_LINE_WIDTH;

		return stRet;
	}

	//get the position of the right bottom of each grid
	SPoint CGridView::GetRightBottom(int paramGridX, int paramGridY){
		
		SPoint stRet = GetLeftTop(paramGridX, paramGridY);
		stRet.x += EDC_W;
		stRet.y += EDC_H;
		return stRet;
	}

	//get the X position of the grid
	int CGridView::GetGridX(int paramMouseX){

		int iRet = paramMouseX / (EDC_W + EDC_LINE_WIDTH);
		if (iRet < 0 || iRet >= EDC_COL_COUNT){
			iRet = 0;
		}
		return iRet;
	}

	//get the Y position of the grid
	int CGridView::GetGridY(int paramMouseY){

		int iRet = paramMouseY / (EDC_W + EDC_LINE_WIDTH);
		if (iRet < 0 || iRet >= EDC_COL_COUNT){
			iRet = 0;
		}
		return iRet;
	}

	//get the grid which is selected by mouse
	CGrid & CGridList::getGrid(int paramGridX, int paramGridY){

		return m_Grid[paramGridX][paramGridY];
	}

	//get the grid which is selected by mouse
	CGrid & CGridList::getGrid(const SPoint & paramCoord){

		return m_Grid[paramCoord.x][paramCoord.y];
	}	

	//get the total number of column
	int CGridList::getColCount() const {

		return EDC_COL_COUNT;
	}


	//get the total number of rows
	int CGridList::getRowCount() const {

		return EDC_ROW_COUNT;
	}

	//check if we pick up a validation node
	bool CGridList::isValidGridCoord(const SPoint & paramCoord) const {

		return (paramCoord.x >= 0 && paramCoord.x < getColCount()) && (paramCoord.y >= 0 && paramCoord.y < getRowCount());

	}

	//draw each grid(including the change after a mouse click)------------------------------------------------------------
	RECT CGridControl::DrawGrid(HDC hdc, int paramGridX, int paramGridY){

		CGrid & stGrid = m_GridList.getGrid(paramGridX, paramGridY);
		SPoint sRectPosLT = m_GridView.GetLeftTop(paramGridX, paramGridY);
		SPoint sRectPosRB = m_GridView.GetRightBottom(paramGridX, paramGridY);

		RECT r;
		r.left 	 = sRectPosLT.x;
		r.top 	 = sRectPosLT.y;
		r.right  = sRectPosRB.x;
		r.bottom = sRectPosRB.y;

		switch(stGrid.Flag){
			case EGF_NORMAL:
			{
				Rectangle(hdc, sRectPosLT.x, sRectPosLT.y, sRectPosRB.x, sRectPosRB.y);
			}
			break;

			case EGF_START:
			{
				HBRUSH hbrush = CreateSolidBrush(RGB(0, 255, 0)); 	//Green
				Rectangle(hdc, sRectPosLT.x, sRectPosLT.y, sRectPosRB.x, sRectPosRB.y);
				FillRect(hdc, &r, hbrush);
				DeletObject(hbrush);
			}
			break;

			case EGF_END:
			{
				HBRUSH hbrush = CreateSolidBrush(RGB(255, 0, 0)); 	//Red
				Rectangle(hdc, sRectPosLT.x, sRectPosLT.y, sRectPosRB.x, sRectPosRB.y);
				FillRect(hdc, &r, hbrush);
				DeletObject(hbrush);
			}
			break;

			case EGF_MD_SELECT:
			{
				HBRUSH hbrush = CreateSolidBrush(RGB(0, 0, 255)); 	//Blue
				Rectangle(hdc, sRectPosLT.x, sRectPosLT.y, sRectPosRB.x, sRectPosRB.y);
				FillRect(hdc, &r, hbrush);
				DeletObject(hbrush);
			}
			break;

			case EGF_OPENLIST:
			{	
				HBRUSH hbrush = CreateSolidBrush(RGB(0, 255, 255)); 	//light green
				Rectangle(hdc, sRectPosLT.x, sRectPosLT.y, sRectPosRB.x, sRectPosRB.y);
				FillRect(hdc, &r, hbrush);
				DeletObject(hbrush);
			}
			break;

			case EGF_CLOSELIST:
			{	
				HBRUSH hbrush = CreateSolidBrush(RGB(255, 255, 0)); 	//yellow
				Rectangle(hdc, sRectPosLT.x, sRectPosLT.y, sRectPosRB.x, sRectPosRB.y);
				FillRect(hdc, &r, hbrush);
				DeletObject(hbrush);
			}
			break;

			case EGF_CURRENT:
			{	
				HBRUSH hbrush = CreateSolidBrush(RGB(255, 0, 255)); 	//purple
				Rectangle(hdc, sRectPosLT.x, sRectPosLT.y, sRectPosRB.x, sRectPosRB.y);
				FillRect(hdc, &r, hbrush);
				DeletObject(hbrush);
			}
			break;

			case EGF_PATH:
			{	
				HBRUSH hbrush = CreateSolidBrush(RGB(0, 0, 0)); 	//black
				Rectangle(hdc, sRectPosLT.x, sRectPosLT.y, sRectPosRB.x, sRectPosRB.y);
				FillRect(hdc, &r, hbrush);
				DeletObject(hbrush);
			}
			break;
		}
		return r;
	}


	//draw all the grids--------------------------------------------------------------------------------------------------
	void CGridControl::DrawAllGrids(HWND hWnd){

		HDC hdc;
		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd, &ps);

		for (int x = 0; x < EDC_COL_COUNT; x++){
			for (int y = 0; y < EDC_ROW_COUNT; y++){
				DrawGrid(hdc, x, y);
			}
		}
		EndPaint(hWnd, &ps);
	}


	//left mouse triggle event
	bool CGridControl::OnMouseLeftDownEvent(HWND hWnd, int paramMouseX, int paramMouseY){

		bool bRet = false;
		int iGridX = m_GridView.GetGridX(paramMouseX);
		int iGridY = m_GridView.GetGridY(paramMouseY);

		//clear the original status
		for(int x = 0; x < m_GridList.getColCount(); x++){
			for (int y = 0; y < CGridList.getRowCount; y++){
				CGrid & stGrid = m_GridList.getGrid(x, y);
				if (stGrid.Flag != EGF_MD_SELECT){
					m_GridList.getGrid(x, y).Flag = EGF_NORMAL;
				}
			}
		}

		//set to begin
		CGrid & stGrid = m_GridList.getGrid(iGridX, iGridY);
		if (stGrid.Flag != EGF_MD_SELECT){
			stGird.Flag = EGF_START;
			bRet = true;
		}
		hdc = BeginPaint(hWnd, &ps);
		InvalidateRect(hWnd, NULL, false);
		EndPaint(hWnd, &ps);

		return bRet;
	}


	//right mouse click event
	bool CGridControl::OnMouseRightDownEvent(HWND hWnd, int paramMouseX, int paramMouseY){

		bool bRet = false;
		int iGridX = m_GridView.GetGridX(paramMouseX);
		int iGridY = m_GridView.GetGridY(paramMouseY);
		CGrid & stGrid = m_GridList.getGrid(iGridX, iGridY);

		if (stGird.Flag == EGF_NORMAL){
		 	stGrid.Flag = EGF_END;
		 	bRet = true;
		}
		
		hdc = BeginPaint(hWnd, &ps);
		InvalidateRect(hWnd, NULL, false);
		EndPaint(hWnd, &ps);
		
		return bRet;
	}


	//middle mouse click event
	void CGridControl::OnMouseMiddleDownEvent(HWND hWnd, int paramMouseX, int paramMouseY){

		int iGridX = m_GridList.GetGridX(paramMouseX);
		int iGridY = m_GridList.GetGridY(paramMouseY);
		CGrid & stGrid = m_GridList.getGrid(iGridX, iGridY);

		if (stGrid.Flag != EGF_MD_SELECT){
			stGrid.Flag = EGF_MD_SELECT;
		}

		hdc = BeginPaint(hWnd, &ps);
		InvalidateRect(hWnd, & DrawGrid(hdc, iGridX, iGridY), false);
		EndPaint(hWnd, &ps);
	}


	//get the position of the  mouse-selected grid
	SPoint CGridControl::getGridXY(SPoint & paramMouseXY){

		SPoint stPoint;
		stPoint.x = m_GridView.GetGridX(paramMouseXY.x);
		stPoint.y = m_GridView.GetGridY(paramMouseXY.y);
		return stPoint;
	}
	

}