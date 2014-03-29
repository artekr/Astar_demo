#ifndef _YX_NODE_H_
#define _YX_NODE_H_

#include <windows.h>
#include <studio.h>
#include <math.h>
#include <map>
#include <set>
#include "yx_grid.h"
#include "yx_common.h"

using namespace std;

namespace yx
{		
	class CNode{
		public:
			CNode();
				:m_Pre(NULL), m_Next(NULL， m_G(0), m_H(0), m_Path(NULL))
			{}

			int getG() const{
				return m_G;
			}
	
			void setG(const int paramG){
				m_G = paramG;
			}

			int getH() const{
				return m_H;
			}
		
			void setH(const int paramH){
				m_H = paramH;
			}
	
			int GetF() const{
				return m_G + m_H;
			}

			const SPoint & getGridXY() const{
				return m_GridXY;
			}

			void setGridXY(const SPoint & paramXY){
				m_GridXY = paramXY;
			}

			CNode * getPre(){
				return m_Pre;
			}

			void setPre(CNode * paramPre){
				m_Pre = paramPre;
			}

			CNode * getNext(){
				return m_Next;
			}

			void setNext(CNode * paramNext){
				m_Next = paramNext;
			}

			CNode * getPath(){
				return m_path;
			}

			void setPath(CNode * paramPath){	//set the path node
				m_Path = paramPath;
			}	

		private:
			//used in the open list
			CNode * m_Pre;
			CNode * m_Next;
			CNode * m_Path;
			//used in a specific node
			SPoint m_GridXY;
			int m_G;
			int m_H;
			
	};
































	

}
#endif