
// ChildView.h : CChildView 类的接口
//


#pragma once

enum COLOR_TYPE;
// CChildView 窗口

class CChildView : public CWnd
{
// 构造
public:
	CChildView();

// 属性
public:

// 操作
public:

// 重写
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CChildView();

	int m_aData[4][4];						// 数字矩阵
	int m_nNoneCnt;							// 空余位个数
	void StartGame();						// 开始游戏
	void DrawBkgrd(void);					// 绘制游戏区背景颜色
	void RandDrawNum();						// 随机位置画出数字块
	void DrawFromData();					// 根据数组画出矩阵
	void CtrlDirection(UINT nDirection);	// 方向控制
	bool CanBeMove();						// 判断还能否继续操作
	int GetMaxNum();						// 获取最大数
	DWORD GetColor(int nNum, COLOR_TYPE nType);		// 获取颜色类型 方块or文字


	// 生成的消息映射函数
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRestart();
};

