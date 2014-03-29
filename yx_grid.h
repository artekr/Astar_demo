#ifndef _YX_GRID_H_
#define _YX_GRID_H_

#include <windows.h>
#include <studio.h>
#include <math.h>
#include <time.h>
#include "yx_common.h"


namespace yx
{
	//single grid class
	class CGrid{
		public:
			CGrid()
				:Flag(EGF_NORMAL)
			{}

			int Flag;
	};

	//grid view class
	class CGridView{
		public:
			SPoint GetLeftTop(int paramGridX, int paramGridY);
			SPoint GetRightBottom(int paramGridX, int paramGridY);

			//change the mouse position to the grid position
			int GetGridX(int paramMouseX);
			int GetGridY(int paramMouseY);
	};

	//grid list class
	class CGridList{
		public:
			CGridList()
			{};

			CGrid & getGrid(int paramGridX, int paramGridY);
			CGrid & getGrid(const SPoint & paramCoord);
			int getColCount() const;
			int getRowCount() const;
			bool isValidGridCoord(const SPoint & paramCoord) const;
		private:
			CGrid m_Grid[EDC_COL_COUNT][EDC_ROW_COUNT];
	};

	//grid control class
	class CGridControl{
		public:
			CGridList & getGridList(){
				return m_GridList;
			}

			void DrawAllGrids(HWND hWnd);
			bool OnMouseLeftDownEvent(HWND hWnd, int paramMouseX, int paramMouseY);
			bool OnMouseRightDownEvent(HWND hWnd, int paramMouseX, int paramMouseY);
			void OnMouseMiddleDownEvent(HWND hWnd, int paramMouseX, int paramMouseY);
			SPoint getGridXY(SPoint & paramMouseXY);

		private:
			RECT DrawGrid(HDC hdc, int paramGridX, int paramGridY);

		private:
			HDC hdc;
			PAINTSTUCT ps;
			CGridList m_GridList;
			CGridView m_GridView;
	};


}
#endif