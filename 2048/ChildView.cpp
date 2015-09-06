
// ChildView.cpp : CChildView ���ʵ��
//

#include "stdafx.h"
#include "2048.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BACKGROUND_COLOR RGB(184, 175, 158)			//������ɫ
#define NONE_SQUARE_COLOR RGB(204, 192, 178)		//�տ���ɫ

//��ɫ����
enum COLOR_TYPE{
	COLOR_SQUARE,	//���� 
	COLOR_TEXT		//����
};

//��������n�η��ҳ�������ɫ, 0Ϊ����4096����ɫ
const DWORD dwSquareColors[] = {
	RGB(255,   0,   0),	
	RGB(238, 228, 218), //1�η�	2
	RGB(236, 236, 200), //2�η�	4
	RGB(242, 177, 119), //3�η�	8
	RGB(236, 141,  83), //4�η�	16
	RGB(245, 124,  95), //5�η�	32
	RGB(233,  89,  55), //6�η�	64
	RGB(243, 217, 107), //7�η�	128
	RGB(241, 208,  75), //8�η�	256
	RGB(228, 192,  42), //9�η�	512
	RGB(227, 186,  20), //10�η�	1024
	RGB(236, 196,   0), //11�η�	2048
	RGB( 94, 218, 146), //12�η�	4096

};

//��������n�η��ҳ�������ɫ, 0Ϊ����4096����ɫ
const DWORD dwTextColors[] = {
	RGB(  0,   0,   0),	
	RGB(119, 110, 101), //1�η�	2
	RGB(119, 110, 101), //2�η�	4
	RGB(251, 255, 253), //3�η�	8
	RGB(251, 255, 253), //4�η�	16
	RGB(251, 255, 253), //5�η�	32
	RGB(251, 255, 253), //6�η�	64
	RGB(251, 255, 253), //7�η�	128
	RGB(251, 255, 253), //8�η�	256
	RGB(251, 255, 253), //9�η�	512
	RGB(251, 255, 253), //10�η�	1024
	RGB(251, 255, 253), //11�η�	2048
	RGB(251, 255, 253), //12�η�	4096
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



// CChildView ��Ϣ�������

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
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������

	// TODO: �ڴ˴������Ϣ����������
	// ��ҪΪ������Ϣ������ CWnd::OnPaint()
	StartGame();
}

void CChildView::StartGame()
{
	//��ʼ������
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
	//������Ϸ����500*500����
	CRect rect(CPoint(0, 0), CPoint(500, 500));
	CBrush brush(BACKGROUND_COLOR);
	pDC->FillRect(rect,&brush);
	ReleaseDC(pDC);
}

void CChildView::RandDrawNum()
{
	if(m_nNoneCnt == 0)
		return;
	//�����ڴ�洢���п����λ��
	/*
	����λ�ñ�ʾ����
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
	//ȡ�����
	srand((unsigned)time(NULL)); 
	int nRand = rand() % m_nNoneCnt;
	//ȡ���λ��
	int nPos = pNones[nRand];
	//ȡ������λ���ж�����ĳ��
	int nX = nPos % 10;
	//ȡ��ʮ��λ���ж�����ĳ��
	int nY = (nPos-nX)/10;
	//�����������֣�2��4  ����4�Ļ���Ϊ1/4
	srand((unsigned)time(NULL)); 
	int nFillNum = rand() % 4 == 0 ? 4 : 2;
	//����������
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
	_tcscpy_s(logFont.lfFaceName, _T("����"));
	CFont mFont;
	mFont.CreateFontIndirect(&logFont);
	pDC->SelectObject(&mFont);

	int nBold = 16;		//�߿���
	int nArea = 105;	//����
	CPoint start(0, 0),end(0, 0);
	start.x += nBold;
	start.y += nBold;
	//�������ֿ鱳��
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	bool bIsMoved = false; //�ж��Ƿ��ƶ��ˣ�����ƶ��ˣ������������
	if (nDirection == VK_UP)
	{
		//�����Ϸ����
		//�������ң��������¼��㣬
		for (int col=0; col<4; col++)
		{
			//�����ǰ��Ϊ�գ�������
			if (m_aData[0][col] == 0 && 
				m_aData[1][col] == 0 && 
				m_aData[2][col] == 0 && 
				m_aData[3][col] == 0)
				continue;
			//�Ƚ����ֶѵ����
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
			//���ж���û���ظ����ظ��ĺϲ�
			for(int row=0; row<3; row++)
			{
				if (m_aData[row][col] == 0)
					continue;
				//������ظ������������������ж�
				if (m_aData[row][col] != m_aData[row+1][col])
					continue;
				//�����ȣ�ֵ���
				m_aData[row][col] += m_aData[row+1][col];
				m_aData[row+1][col] = 0;
				m_nNoneCnt++;
				bIsMoved = true;
				//�������������������
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
		//�����·����
		//�������ң��������ϼ��㣬
		for (int col=0; col<4; col++)
		{
			//�����ǰ��Ϊ�գ�������
			if (m_aData[0][col] == 0 &&
				m_aData[1][col] == 0 && 
				m_aData[2][col] == 0 && 
				m_aData[3][col] == 0)
				continue;
			//�Ƚ����ֶѵ���׶�
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
			//���ж���û���ظ����ظ��ĺϲ�
			for(int row=3; row>0; row--)
			{
				if (m_aData[row][col] == 0)
					continue;
				//������ظ������������������ж�
				if (m_aData[row][col] != m_aData[row-1][col])
					continue;
				//�����ȣ�ֵ���
				m_aData[row][col] += m_aData[row-1][col];
				m_aData[row-1][col] = 0;
				m_nNoneCnt++;
				bIsMoved = true;
				//�������������������
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
		//���������
		//�������£��������Ҽ��㣬
		for (int row=0; row<4; row++)
		{
			//�����ǰ��Ϊ�գ�������
			if (m_aData[row][0] == 0 && 
				m_aData[row][1] == 0 && 
				m_aData[row][2] == 0 && 
				m_aData[row][3] == 0)
				continue;
			//�Ƚ����ֶѵ������
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
			//���ж���û���ظ����ظ��ĺϲ�
			for(int col=0; col<3; col++)
			{
				if (m_aData[row][col] == 0)
					continue;
				//������ظ������������������ж�
				if (m_aData[row][col] != m_aData[row][col+1])
					continue;
				//�����ȣ�ֵ���
				m_aData[row][col] += m_aData[row][col+1];
				m_aData[row][col+1] = 0;
				m_nNoneCnt++;
				bIsMoved = true;
				//�������������������
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
		//�����ҷ����
		//�������£�����������㣬
		for (int row=0; row<4; row++)
		{
			//�����ǰ��Ϊ�գ�������
			if (m_aData[row][0] == 0 && 
				m_aData[row][1] == 0 && 
				m_aData[row][2] == 0 && 
				m_aData[row][3] == 0)
				continue;
			//�Ƚ����ֶѵ����Ҷ�
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
			//���ж���û���ظ����ظ��ĺϲ�
			for(int col=3; col>0; col--)
			{
				if (m_aData[row][col] == 0)
					continue;
				//������ظ������������������ж�
				if (m_aData[row][col] != m_aData[row][col-1])
					continue;
				//�����ȣ�ֵ���
				m_aData[row][col] += m_aData[row][col-1];
				m_aData[row][col-1] = 0;
				m_nNoneCnt++;
				bIsMoved = true;
				//�������������������
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
	//�ػ���Ϸ��
	if (bIsMoved)
		RandDrawNum();
	DrawFromData();
	if (!CanBeMove())
	{
		CString strMsg;
		strMsg.Format("��Ϸ������������֣�%d�������¿�ʼ��", GetMaxNum());
		MessageBox(strMsg);
	}
}

bool CChildView::CanBeMove()
{
	// ����������п���飬�������ƶ�
	// �����ж����������Ƿ��������ظ������ظ���������������
	if (m_nNoneCnt > 0)
		return true;
	//�����ж�
	for (int row=0; row<4; row++)
	{
		if (m_aData[row][0] == m_aData[row][1] || 
			m_aData[row][1] == m_aData[row][2] ||
			m_aData[row][2] == m_aData[row][3])
		{
			return true;
		}
	}
	//�����ж�
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
	// TODO: �ڴ���������������
	if (CanBeMove())
	{
		if (MessageBox("��Ϸ��δ������ȷ��Ҫ���¿�ʼ��", NULL, MB_YESNO) == IDYES)
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
	//�ж��ǲ���2��N�η�
	if(nNum & (nNum-1))
		return RGB(0,0,0);
	//�ж���2�ļ��η�
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