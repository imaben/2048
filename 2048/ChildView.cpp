
// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "2048.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BACKGROUND_COLOR RGB(184, 175, 158)			//背景颜色
#define NONE_SQUARE_COLOR RGB(204, 192, 178)		//空块颜色

//颜色类型
enum COLOR_TYPE{
	COLOR_SQUARE,	//方块 
	COLOR_TEXT		//数字
};

//根据数字n次方找出方块颜色, 0为超出4096的颜色
const DWORD dwSquareColors[] = {
	RGB(255,   0,   0),	
	RGB(238, 228, 218), //1次方	2
	RGB(236, 236, 200), //2次方	4
	RGB(242, 177, 119), //3次方	8
	RGB(236, 141,  83), //4次方	16
	RGB(245, 124,  95), //5次方	32
	RGB(233,  89,  55), //6次方	64
	RGB(243, 217, 107), //7次方	128
	RGB(241, 208,  75), //8次方	256
	RGB(228, 192,  42), //9次方	512
	RGB(227, 186,  20), //10次方	1024
	RGB(236, 196,   0), //11次方	2048
	RGB( 94, 218, 146), //12次方	4096

};

//根据数字n次方找出数字颜色, 0为超出4096的颜色
const DWORD dwTextColors[] = {
	RGB(  0,   0,   0),	
	RGB(119, 110, 101), //1次方	2
	RGB(119, 110, 101), //2次方	4
	RGB(251, 255, 253), //3次方	8
	RGB(251, 255, 253), //4次方	16
	RGB(251, 255, 253), //5次方	32
	RGB(251, 255, 253), //6次方	64
	RGB(251, 255, 253), //7次方	128
	RGB(251, 255, 253), //8次方	256
	RGB(251, 255, 253), //9次方	512
	RGB(251, 255, 253), //10次方	1024
	RGB(251, 255, 253), //11次方	2048
	RGB(251, 255, 253), //12次方	4096
};
// CChildView

CChildView::CChildView()
{

}

CChildView::~CChildView()
{

}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_RESTART, &CChildView::OnRestart)
END_MESSAGE_MAP()



// CChildView 消息处理程序

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	// TODO: 在此处添加消息处理程序代码
	// 不要为绘制消息而调用 CWnd::OnPaint()
	StartGame();
}

void CChildView::StartGame()
{
	//初始化数组
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			m_aData[i][j] = 0;
	m_nNoneCnt = 4*4;
	DrawBkgrd();
	RandDrawNum();
	RandDrawNum();
	DrawFromData();
}

void CChildView::DrawBkgrd(void)
{
	CDC *pDC = GetDC();
	//绘制游戏区域500*500背景
	CRect rect(CPoint(0, 0), CPoint(500, 500));
	CBrush brush(BACKGROUND_COLOR);
	pDC->FillRect(rect,&brush);
	ReleaseDC(pDC);
}

void CChildView::RandDrawNum()
{
	if(m_nNoneCnt == 0)
		return;
	//申请内存存储所有空余的位置
	/*
	矩阵位置表示如下
	11  12  13  14
	21  22  23  24
	31  32  33  34
	41  42  43  44
	*/
	int *pNones = new int[m_nNoneCnt];
	int p = 0;
	for (int i=0; i<4; i++)
	{
		for (int j=0; j<4; j++)
		{
			if(m_aData[i][j] == 0)
			{
				*(pNones+p) = (i+1)*10+j+1;
				p++;
			}
		}
	}
	//取随机数
	srand((unsigned)time(NULL)); 
	int nRand = rand() % m_nNoneCnt;
	//取随机位置
	int nPos = pNones[nRand];
	//取“个”位，判断属于某列
	int nX = nPos % 10;
	//取“十”位，判断属于某行
	int nY = (nPos-nX)/10;
	//随机出填充数字，2或4  出现4的机率为1/4
	srand((unsigned)time(NULL)); 
	int nFillNum = rand() % 4 == 0 ? 4 : 2;
	//填充矩阵数组
	m_aData[nY-1][nX-1] = nFillNum;
	delete[] pNones;
	m_nNoneCnt--;
}

void CChildView::DrawFromData()
{
	CDC *pDC = GetDC();
	pDC->SetBkMode(TRANSPARENT);
	LOGFONT logFont = {0};
	logFont.lfHeight = -MulDiv(22, pDC->GetDeviceCaps(LOGPIXELSY), 72);
	_tcscpy_s(logFont.lfFaceName, _T("隶书"));
	CFont mFont;
	mFont.CreateFontIndirect(&logFont);
	pDC->SelectObject(&mFont);

	int nBold = 16;		//边框宽度
	int nArea = 105;	//块宽度
	CPoint start(0, 0),end(0, 0);
	start.x += nBold;
	start.y += nBold;
	//绘制数字块背景
	for (int i=0; i<4; i++)
	{
		for (int j=0; j<4; j++)
		{
			end.x = start.x + nArea;
			end.y = start.y + nArea;
			if(m_aData[i][j] == 0)
			{
				CBrush brush(NONE_SQUARE_COLOR);
				pDC->FillRect(CRect(start, end), &brush);
			}
			else
			{
				CBrush brush(GetColor(m_aData[i][j], COLOR_SQUARE));
				pDC->SetTextColor(GetColor(m_aData[i][j], COLOR_TEXT));
				pDC->FillRect(CRect(start, end), &brush);
				CString strNum;
				strNum.Format("%d", m_aData[i][j]);
				pDC->DrawText(strNum, CRect(start, end), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			start.x += nArea+nBold;
		}
		start.x = nBold;
		start.y += nArea+nBold;
	}
	ReleaseDC(pDC);
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nChar)
	{
	case VK_UP:
	case VK_DOWN:
	case VK_LEFT:
	case VK_RIGHT:
		CtrlDirection(nChar);
		break;
	default:
		break;
	}
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CChildView::CtrlDirection(UINT nDirection)
{
	bool bIsMoved = false; //判断是否移动了，如果移动了，则产生新数字
	if (nDirection == VK_UP)
	{
		//按向上方向键
		//从左自右，自上往下计算，
		for (int col=0; col<4; col++)
		{
			//如果当前列为空，则跳过
			if (m_aData[0][col] == 0 && 
				m_aData[1][col] == 0 && 
				m_aData[2][col] == 0 && 
				m_aData[3][col] == 0)
				continue;
			//先将数字堆到最顶端
			for(int row=0; row<4; row++)
			{
				if (m_aData[row][col] != 0)
					continue;
				for (int tmp=row+1; tmp<4; tmp++)
				{
					if (m_aData[tmp][col] != 0)
					{
						m_aData[row][col] = m_aData[tmp][col];
						m_aData[tmp][col] = 0;
						bIsMoved = true;
						break;
					}
				}
			}
			//再判断有没有重复，重复的合并
			for(int row=0; row<3; row++)
			{
				if (m_aData[row][col] == 0)
					continue;
				//如果不重复，则跳过继续向下判断
				if (m_aData[row][col] != m_aData[row+1][col])
					continue;
				//如果相等，值相加
				m_aData[row][col] += m_aData[row+1][col];
				m_aData[row+1][col] = 0;
				m_nNoneCnt++;
				bIsMoved = true;
				//并把其余的数字向上推
				for (int tmp=row+1; tmp<3; tmp++)
				{
					if (m_aData[tmp+1][col] != 0)
					{
						m_aData[tmp][col] = m_aData[tmp+1][col];
						m_aData[tmp+1][col] = 0;
						break;
					}
				}
			}
		}
	}
	else if (nDirection == VK_DOWN)
	{
		//按向下方向键
		//从左自右，自下往上计算，
		for (int col=0; col<4; col++)
		{
			//如果当前列为空，则跳过
			if (m_aData[0][col] == 0 &&
				m_aData[1][col] == 0 && 
				m_aData[2][col] == 0 && 
				m_aData[3][col] == 0)
				continue;
			//先将数字堆到最底端
			for(int row=3; row>=0; row--)
			{
				if (m_aData[row][col] != 0)
					continue;
				for (int tmp=row-1; tmp>=0; tmp--)
				{
					if (m_aData[tmp][col] != 0)
					{
						m_aData[row][col] = m_aData[tmp][col];
						m_aData[tmp][col] = 0;
						bIsMoved = true;
						break;
					}
				}
			}
			//再判断有没有重复，重复的合并
			for(int row=3; row>0; row--)
			{
				if (m_aData[row][col] == 0)
					continue;
				//如果不重复，则跳过继续向上判断
				if (m_aData[row][col] != m_aData[row-1][col])
					continue;
				//如果相等，值相加
				m_aData[row][col] += m_aData[row-1][col];
				m_aData[row-1][col] = 0;
				m_nNoneCnt++;
				bIsMoved = true;
				//并把其余的数字向下推
				for (int tmp=row-1; tmp>0; tmp--)
				{
					if (m_aData[tmp-1][col] != 0)
					{
						m_aData[tmp][col] = m_aData[tmp-1][col];
						m_aData[tmp-1][col] = 0;
						break;
					}
				}
			}
		}
	}
	else if (nDirection == VK_LEFT)
	{
		//按向左方向键
		//从上自下，自左往右计算，
		for (int row=0; row<4; row++)
		{
			//如果当前行为空，则跳过
			if (m_aData[row][0] == 0 && 
				m_aData[row][1] == 0 && 
				m_aData[row][2] == 0 && 
				m_aData[row][3] == 0)
				continue;
			//先将数字堆到最左端
			for(int col=0; col<4; col++)
			{
				if (m_aData[row][col] != 0)
					continue;
				for (int tmp=col+1; tmp<4; tmp++)
				{
					if (m_aData[row][tmp] != 0)
					{
						m_aData[row][col] = m_aData[row][tmp];
						m_aData[row][tmp] = 0;
						bIsMoved = true;
						break;
					}
				}
			}
			//再判断有没有重复，重复的合并
			for(int col=0; col<3; col++)
			{
				if (m_aData[row][col] == 0)
					continue;
				//如果不重复，则跳过继续向下判断
				if (m_aData[row][col] != m_aData[row][col+1])
					continue;
				//如果相等，值相加
				m_aData[row][col] += m_aData[row][col+1];
				m_aData[row][col+1] = 0;
				m_nNoneCnt++;
				bIsMoved = true;
				//并把其余的数字向左推
				for (int tmp=col+1; tmp<3; tmp++)
				{
					if (m_aData[row][tmp+1] != 0)
					{
						m_aData[row][tmp] = m_aData[row][tmp+1];
						m_aData[row][tmp+1] = 0;
						break;
					}
				}
			}
		}
	}
	else if (nDirection == VK_RIGHT)
	{
		//按向右方向键
		//从上自下，自右往左计算，
		for (int row=0; row<4; row++)
		{
			//如果当前行为空，则跳过
			if (m_aData[row][0] == 0 && 
				m_aData[row][1] == 0 && 
				m_aData[row][2] == 0 && 
				m_aData[row][3] == 0)
				continue;
			//先将数字堆到最右端
			for(int col=3; col>=0; col--)
			{
				if (m_aData[row][col] != 0)
					continue;
				for (int tmp=col-1; tmp>=0; tmp--)
				{
					if (m_aData[row][tmp] != 0)
					{
						m_aData[row][col] = m_aData[row][tmp];
						m_aData[row][tmp] = 0;
						bIsMoved = true;
						break;
					}
				}
			}
			//再判断有没有重复，重复的合并
			for(int col=3; col>0; col--)
			{
				if (m_aData[row][col] == 0)
					continue;
				//如果不重复，则跳过继续向下判断
				if (m_aData[row][col] != m_aData[row][col-1])
					continue;
				//如果相等，值相加
				m_aData[row][col] += m_aData[row][col-1];
				m_aData[row][col-1] = 0;
				m_nNoneCnt++;
				bIsMoved = true;
				//并把其余的数字向右推
				for (int tmp=col-1; tmp>0; tmp--)
				{
					if (m_aData[row][tmp-1] != 0)
					{
						m_aData[row][tmp] = m_aData[row][tmp-1];
						m_aData[row][tmp-1] = 0;
						break;
					}
				}
			}
		}
	}
	//重绘游戏区
	if (bIsMoved)
		RandDrawNum();
	DrawFromData();
	if (!CanBeMove())
	{
		CString strMsg;
		strMsg.Format("游戏结束，最大数字：%d，请重新开始！", GetMaxNum());
		MessageBox(strMsg);
	}
}

bool CChildView::CanBeMove()
{
	// 规则：如果还有空余块，则允许移动
	// 否则判断所有行列是否相邻有重复，有重复则允许，否则不允许
	if (m_nNoneCnt > 0)
		return true;
	//按行判断
	for (int row=0; row<4; row++)
	{
		if (m_aData[row][0] == m_aData[row][1] || 
			m_aData[row][1] == m_aData[row][2] ||
			m_aData[row][2] == m_aData[row][3])
		{
			return true;
		}
	}
	//按列判断
	for (int col=0; col<4; col++)
	{
		if (m_aData[0][col] == m_aData[1][col] || 
			m_aData[1][col] == m_aData[2][col] ||
			m_aData[2][col] == m_aData[3][col])
		{
			return true;
		}
	}
	return false;
}

int CChildView::GetMaxNum()
{
	int nMax = 0;
	for (int i=0; i<4; i++)
	{
		for (int j=0; j<4; j++)
		{
			if(m_aData[i][j] > nMax)
				nMax = m_aData[i][j];
		}
	}
	return nMax;
}
void CChildView::OnRestart()
{
	// TODO: 在此添加命令处理程序代码
	if (CanBeMove())
	{
		if (MessageBox("游戏还未结束，确定要重新开始吗？", NULL, MB_YESNO) == IDYES)
		{
			StartGame();
		}
	}
	else
	{
		StartGame();
	}
}

DWORD CChildView::GetColor(int nNum, COLOR_TYPE nType)
{
	//判断是不是2的N次方
	if(nNum & (nNum-1))
		return RGB(0,0,0);
	//判断是2的几次方
	int nPow = 0;
	while(nNum>1)
	{
		nNum >>= 1;
		nPow++;
	}
	if(nType == COLOR_SQUARE)
	{
		if (nPow > 12)
		{
			return dwSquareColors[0];
		}
		return dwSquareColors[nPow];
	}
	if (nType == COLOR_TEXT)
	{
		if (nPow > 12)
		{
			return dwTextColors[0];
		}
		return dwTextColors[nPow];
	}
	return RGB(0,0,0);
}