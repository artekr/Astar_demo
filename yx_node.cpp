#include "yx_node.h"
#include "yx_common.h"

#include <sstream>
using namespace std;

namespace yx
{
	//open list initialization
	void COpenList::Init(){
		Clear();
	}

	//relese the current nodes
	void COpenList::Clear(){
		
		CNode * p = m_Head;
		while(p != NULL){
			CNode * pDelete = p;
			p = p->getNext();
			delete pDelete;
		}
		m_Head = NULL;
	}

	//get the head of the open list
	CNode * COpenList::getHead(){
		return m_Head;
	}

	//get the min of the F value
	/**
		@return CNode * the minimal node
			- NULL no node left
			- Not NULL the minimal node
	*/
	CNode * COpenList::Pop_the_first_node(){

		CNode * pRet = m_Head;
		if (m_Head != NULL){
			m_Head = m_Head->getNext();
			if (m_Head != NULL){
				m_Head->setPre(NULL);
			}
			pRet->setNext(NULL);
		}
		return pRet;
	}

	//find the position of a specific node
	CNode * COpenList::FindByCoord(const SPoint & paramCoord){

		CNode * pRet = NULL;
		CNode * pFind = m_Head;

		while(pFind != NULL){
			if (pFind->getGridXY() == paramCoord){
				pRet = pFind;
				break;
			}else{
				pFind =  pFind->getNext();
			}
		}
		return pRet;
	}

	//insert the node to the open list
	void COpenList::Insert_to_open_list(CNode * paramNode){
		
		do{
			if (m_Head == NULL){
				//if it is a empty list
				m_Head = paramNode;
				paramNode->setNext(NULL);
				paramNode->setPre(NULL);
				break;
			}

			CNode * pFind = m_Head;
			CNode * pTail = NULL;

			while(pFind != NULL){
				if(paramNode->GetF() < pFind->GetF()){
					break;
				}else{
					if(pFind->getNext() == NULL){
						pTail = pFind;
						pFind = NULL;
						break;
					}
					pFind = pFind->getNext();
				}

			}

			if (pFind != NULL){
				if(pFind == m_Head){
					paramNode->setNext(pFind);
					pFind->setPre(paramNode);
					paramNode->setPre(NULL);
					m_Head = paramNode;
				}else{
					CNode * pPre = pFind->getPre();
					pPre->setNext(paramNode);
					paramNode->setPre(pPre);
					paramNode->setNext(pFind);
					pFind->setPre(paramNode);
				}
			}else if (pTail != NULL){
				paramNode->setPre(pTail);
				pTail->setNext(paramNode);
				paramNode->setNext(NULL);
			}

		}while(false);
	}


	//remove from the open list
	void COpenList::Remove_from_open_list(CNode * paramNode){

		CNode * pFind = paramNode;
		if (pFind == m_Head){
			m_Head = paramNode->getNext();
			if(m_Head != NULL){
				m_Head->setPre(NULL);
			}
		}else{
			CNode * pPre = paramNode->getPre();
			CNode * pNext = paramNode->getNext();
			pPre->setNext(pNext);
			if (pNext != NULL){
				pNext->setPre(pPre);
			}
		}
		paramNode->setPre(NULL);
		paramNode->setNext(NULL);
	}



	//-----------------Close List-------------------------------------------------------------------
	//close list initialization
	void CCloseList::Init(){
		
		m_CloseList.clear();
	}

	//clear the current exist nodes 
	void CCloseList::Clear(){
		
		m_CloseList.clear();
	}

	//check if the node is already in the close list
	bool CCloseList::is_exist(SPoint paramXY){

		if (!m_CloseList.empty()){
			TMapNode::interator iter = m_CloseList.find(paramXY);
			if (iter != m_CloseList.end())
				return true; 				//find the tartget
			else
				return false;
		}else
			return false;
	}

	//find the position of a specific node
	CNode * CCloseList::FindByCoord(const SPoint & paramCoord){

		CNode * pFind = NULL;
		if(!m_CloseList.empty()){
			TMapNode::interator iter = m_CloseList.find(paramNode);
			if(iter != m_CloseList.end()){	//find the target
				pFind = iter->second;
			}
		}
		return pFind;
	}

	//insert the node to the close list
	void CCloseList::Insert_to_close_list(CNode * paramNode){

		m_CloseList[paramNode->getGridXY()] = paramNode;
	}

	//remove from the close list
	void CCloseList::Remove_from_close_list(CNode * paramNode){

		m_CloseList.erase(paramNode->getGridXY());
	}


	//caculate the H value, which is cost from the current node to the end point
	int CAStar::Cal_H(SPoint & paramCur){

		//return (abs(m_End.y - paramCur.y) + abs(m_End.x - paramCur.x)) * 10; //1st algorithm

		int detaY = (m_End.y - paramCur.y) * 10;
		int detaX = (m_End.x - paramCur.x) * 10;
		int H = detaX * detaX + detaY * detaY;
		
		return H;	//2nd algorithm, the wight of H value is bigger, less grids visited 
		//return (int)sqrt(H*1.0);	//3rd algorithm, the wight of G value is bigger, shorter distance
	}

	//begin the path finding!!!!!!!!!!
	void CAStar::Start(const SPoint & paramStart, const SPoint & paramEnd, CGridList * paramGridList){

		m_Start = paramStart;
		m_End	= paramEnd;
		m_GridList = paramGridList;

		//REMEMBER! Initialization
		m_OpenList.Clear();
		m_CloseList.Clear();

		for (int x = 0; x < paramGridList->getColCount(); x++){
			for (int y = 0; y < paramGridList->getRowCount(); y++){
				if (paramGridList->getGrid(x, y).Flag != EGF_MD_SELECT){
					paramGridList->getGrid(x, y).Flag = EGF_NORMAL;
				}
			}
		}

		paramGridList->getGrid(paramStart).Flag = EGF_START;
		paramGridList->getGrid(paramEnd).Flag = EGF_END;
		PushNode_ToOpenList(m_Start, 0, NULL);
	} 


	//the path finding algorithm
	bool CAStar::DoFind(){

		bool bRet = false;
		do{
			CNode * pCurrNode = m_OpenList.Pop_the_first_node();
			if (pCurrNode == NULL){
				//did not find any path
				bRet = true;
				break;
			}

			m_CloseList.Insert_to_close_list(pCurrNode);

			CGrid & stGrid = m_GridList->getGrid(pCurrNode->getGridXY());

			if (!handle_neighbor(pCurrNode)){
				//if did not find any path
				m_GridList->getGrid(pCurrNode->getGridXY()).Flag = EGF_CLOSELIST;
			}else{
				//if find the end point
				CNode * pPath = pCurrNode;
				while(pPath != NULL){
					m_GridList->getGrid(pPath->getGridXY()).Flag = EGF_PATH;
					pPath = pPath->getPath();
				}
				m_GridList->getGrid(m_Start).Flag = EGF_START;
				bRet = true;
				break;
			}
		}while(false);
		return bRet;
	}

	//put some node into the open list
	void CAStar::PushNode_ToOpenList(SPoint & paramCur, int paramG, CNode * paramPath){

		CNode * pNode = new CNode();
		pNode->setGridXY(paramCur);		//set the position of itself
		pNode->setH(Cal_H(paramCur));	//caculate the H value of this node
		pNode->setG(paramG);			//caculate the G value of this node
		pNode->setPath(paramPath);
		m_OpenList.Insert_to_open_list(pNode);
		if(paramCur != m_Start){
			m_GridList->getGrid(paramCur.x, paramCur.y).Flag = EGF_OPENLIST;
		}					
	}


	//check the neighbors around the center node-------------------------------------------------
	/**
		@param [in] paramCurrNode current node
		@return bool check its neighbors to see if there were a end point
			- true it has an end point
			- false no end point
	*/
	bool CAStar:: handle_neighbor(CNode * paramCenterNode){

		struct SRoundNode{
			int x;
			int y;
			int G;
		};

		static SRoundNode SRoundNode[8] = {
											{-1, -1, -14}, {0, -1, 10}, {1, -1, 14},
											{-1, 0, 10},				{1, 0, 10},
											{-1, 1, 14},   {0, 1, 10},  {1, 1, 14}	
											};

		for (int i = 0; i < 8; i++){
			SRoundNode & stNode = SRoundNode[i];
			SPoint stCurPos = paramCenterNode->getGridXY();
			stCurPos.x += stNode.x;
			stCurPos.y += stNode.y;

			if(!m_GridList->isValidGridCoord(stCurPos)){
				continue;
			}

			if (stCurPos == m_End){
				return true;
			}

			CGrid & stGrid = m_GridList->getGrid(stCurPos);

			if(stGrid.Flag == EGF_NORMAL){
			
				PushNode_ToOpenList(stCurPos, paramCenterNode->getG() + stNode.G, paramCenterNode);
			}else if(stGrid.Flag == EGF_OPENLIST){

				int iNewG = paramCenterNode->getG() + stNode.G;
				CNode * pOpenListNode = m_OpenList.FindByCoord(stCurPos);
				if (pOpenListNode != NULL){
					if(iNewG < (pOpenListNode->getG())){
						//if it needs to reput into Open list
						m_OpenList.Remove_from_open_list(pOpenListNode);
						pOpenListNode->setG(iNewG);
						pOpenListNode->setPath(paramCenterNode);
						m_OpenList.Insert_to_open_list(pOpenListNode);
					}
				}
			}else if(stGrid.Flag == EGF_CLOSELIST){
				continue;
			}
		}
		return false;
	}


	//print out the node info
	void CAStar::PrintNode(CNode * paramNode){

		stringstream ss;
		ss << " OpenList : X: " << paramNode->getGridXY().x
		   << " Y: " << paramNode->getGridXY().y
		   << " F= " << paramNode->getF()
		   << " G= " << paramNode->getG()
		   << " H= " << paramNode->getH()
		   << " \n";
		OutputDebugStringA(ss.str().c_str);
	}

	//print the open list info
	void CAStar::PrintOpenList(){
		OutputDebugStringA("Print OpenList Start");
		CNode * pNode = m_OpenList.getHead();
		while(pNode != NULL){
			PrintNode(pNode);
			pNode = pNode->getNext();
		}
		OutputDebugStringA("Print OpenList End");
	}
}