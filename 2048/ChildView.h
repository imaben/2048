
// ChildView.h : CChildView ��Ľӿ�
//


#pragma once

enum COLOR_TYPE;
// CChildView ����

class CChildView : public CWnd
{
// ����
public:
	CChildView();

// ����
public:

// ����
public:

// ��д
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CChildView();

	int m_aData[4][4];						// ���־���
	int m_nNoneCnt;							// ����λ����
	void StartGame();						// ��ʼ��Ϸ
	void DrawBkgrd(void);					// ������Ϸ��������ɫ
	void RandDrawNum();						// ���λ�û������ֿ�
	void DrawFromData();					// �������黭������
	void CtrlDirection(UINT nDirection);	// �������
	bool CanBeMove();						// �жϻ��ܷ��������
	int GetMaxNum();						// ��ȡ�����
	DWORD GetColor(int nNum, COLOR_TYPE nType);		// ��ȡ��ɫ���� ����or����


	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRestart();
};

