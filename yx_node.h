#ifndef _YX_NODE_H_
#define _YX_NODE_H_

#include <windows.h>
#include <studio.h>
#include <math.h>
#include <map>
#include <set>
#include "yx_grid.h"

using namespace std;

namespace yx
{
	class  CNode {
		public:
			CNode()
			 	:m_Pre(NULL), m_Next(NULL), m_G(0), m_H(0)
			{}
			
			int getG() const {
				return m_G;
			}
			void setG(const int paramG) {
				m_G = paramG;
			}

			int getH() const {
				return m_H;
			}
			void setH(const int paramH) {
				m_H = paramH;
			}

			int getF() const {
				return m_G + m_H;
			}

			const SPoint & getGridXY() const {
				return m_GridXY;			
			}
			void setGridXY(cons SPoint & paramXY) {
				m_GridXY = paramXY;
			}

			CNode * getPre() {
				return m_Pre;
			}
			void setPre(CNode * paramPre) {
				m_Pre = paramPre;
			}
			CNode * getNext() {
				return m_Next;
			}
			void setNext(CNode * paramNext) {
				m_Next = paramNext;
			}

		private:
			SPoint m_GridXY;
			CNode * m_Pre;
			CNode * m_Next;
			int m_G;
			int m_H;
	};


	//OpenList
	//find the minimum
	//can add and deleted in a specific order
	//The Key value is F, can be duplicated
	class COpenList {
		public:
			COpenList()
				:m_Head(NULL)
			{}


		//Find the smallest Node
		/**
			@return CNode * smallest Node
					- NULL no Node anymore
					- Not NULL the smallest Node
		*/
		CNode * Pop(){	
			CNode *pRet = m_Head;
			if (m_Head != NULL){
				m_Head = m_Head->getNext();
				if (m_Head != NULL){
					m_Head->setPre(NULL);
				}
				pRet->setNext(NULL);
			}
			return pRet;
		}

		/**
			insert a node to the current list
		*/
		void insert_to_open_list(CNode * paramNode) {
			do{
				if (m_Head == NULL){
					m_Head = paramNode;
					paramNode->setNext(NULL);
					paramNode->setPre(NULL);
					break;
				}

				CNode * pFind = m_Head;
				while(pFind != NULL){
					if (paramNode->getF() < pFind->getF()){
						if (pFind == m_Head){
							paramNode->setNext(pFind);
							pFind->setPre(paramNode);
							paramNode->setPre(NULL);
							m_Head = paramNode;
						}else{
							paramNode->setNext(pFind);
							paramNode->setPre(pFind->getPre());
							pFind->setPre(paramNode);
							pFind->getPre()->setNext(paramNode);
						}
						break;
					}else{
						if (pFind->getNext() == NULL){
							paramNode->setPre(pFind);
							pFind->setNext(paramNode);
							paramNode->setNext(NULL);
							break;
						}
						pFind = pFind->getNext();
					}				
				}
			}while(false);
		}

		private:
			CNode * m_Head;

	};


	//CloseList
	//check if the node is exist
	//can add and deleted in a specific order
	//The comparable Key value is Grid, no replications 
	class CCloseList{
		public:
			typedef set<SPoint> TSetGrid;
			CCloseList()
			{}

			bool is_exist(SPoint paramXY){
				if(!m_CloseList.empty()){
					TSetGrid::iterator iter = m_CloseList.find(paramXY);
					if(iter != m_CloseList.end())  //Find the target
						return true;
					else
						return false;
				}else
					return false;
			}

			void Insert_to_close_list(CNode * paramNode){
				m_CloseList.insert(paramNode->getGridXY());
			}

			void remove_from_close_list(CNode * paramNode){
				m_CloseList.erase(paramNode->getGridXY);
			}
		private:
			TSetGrid m_CloseList;
	};


	//A Star Algorithm
	class CAStar{
		public:
			CAStar()
			{}

				int Cal_H(SPoint & paramCur);	//caculate the G value
				void Start(const SPoint & paramStart, const SPoint & paramEnd, CGridList * paramGridList);	//start point

				void PrintOpenList();				//for testing
				void PrintNode(CNode * paramNode);

		private:
			void PushNode_ToOpenList(SPoint & paramCur, int paramG, CNode * paramPath);	//insert the node into the open list
			bool handle_neighbor(CNode * paramCenterNode);								//handle the neighbor node around the current node

		private:
			COpenList m_OpenList;
			CCloseList m_CloseList;
			CGridList * m_GridList;
			SPoint m_Start;
			SPoint m_End;
	};

}
#endif