#ifndef _YX_COMMON_H_
#define _YX_COMMON_H_


namespace yx
{
	//define the attributes of the grid
	enum EnumDrawConstant {
		EDC_H 		   = 10;	//height of each grid, unit pixel
		EDC_W 		   = 10;	//width of each grid, unit pixel
		EDC_ROW_COUNT  = 60;	//the number of rows in the entire map
		EDC_COL_COUNT  = 80;	//the number of columns in teh entire map
		EDC_LINE_WIDTH = 1;		//the width of the line, unit pixel
		EDC_ALL_WIDTH  = EDC_COL_COUNT * (EDC_W + EDC_LINE_WIDTH) + EDC_LINE_WIDTH;
		EDC_ALL_HEIGHT = EDC_ROW_COUNT * (EDC_H + EDC_LINE_WIDTH) + EDC_LINE_WIDTH;
	};


	//the state of the Grid
	enum EnumGridFlag{
		EGF_NORMAL,		//normal state
		EGF_START,		//left mouse select state
		EGF_END,		//right mouse select state
		EGF_MD_SELECT,	//middle mouse select state

		EGF_OPENLIST,	//in the open list
		EGF_CLOSELIST,	//in the close list
		EGF_CURRENT,	//current node
		EGF_PATH,		//the node in the path
	};


	//node point info
	struct SPoint{
		public:
			SPoint()
				:x(0),y(0)
			{}
			SPoint(const SPoint & paramXY)
				:x(paramXY.x),y(paramXY.y)
			{}

			inline bool operator != (const SPoint & paramP) const {
				return ((this->x != paramP.x) || (this->y != paramP.y));
			}
			inline bool operator == (const SPoint & paramP) const {
				return ((this->x == paramP.x) && (this->y == paramP.y));
			}
			inline bool operator > (const SPoint & paramP) const {
				return ((this->x > paramP.x) && (this->y > paramP.y));
			}
			inline bool operator < (const SPoint & paramP) const {
				return ((this->x < paramP.x) && (this->y < paramP.y));
			}
			inline bool operator >= (const SPoint & paramP) const {
				return ((this->x >= paramP.x) && (this->y >= paramP.y));
			}
			inline bool operator <= (const SPoint & paramP) const {
				return ((this->x <= paramP.x) && (this->y <= paramP.y));
			}


			int x;
			int y;
	};


	//path finding state
	enum EnumFindStatus{
		EFS_NORMAL,		//normal status
		EFS_HAS_START,	//begin to start
		EFS_FINDING,	//find status
	};


}
#endif

