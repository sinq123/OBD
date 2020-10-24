/************************************************* 
  Copyright (C), 2009-2019, HuaBei 
  文件名: PLHPCurve.h
  作者: Cui
  版本: V1.0.0
  日期: 2009-06-01
  描述: 以坐标轴形式,显示测功机寄生功率曲线。
		其中，x轴为速度，单位:km/h；y轴为寄生功率，单位:kW
		最多可以显示2条曲线，可以用于同时显示带剩磁力和不带剩磁力2条曲线，进行比较
  其它: 无
  历史:
  1. 时间：2009-06-01
	 作者：Cui
	 描述：完成，进行测试
  2. 时间：2009-06-01
     作者：Cui
	 描述：改成可以同时显示2条曲线
*************************************************/

#pragma once


// CPLHPCurve

class CPLHPCurve : public CStatic
{
	DECLARE_DYNAMIC(CPLHPCurve)

public:
	CPLHPCurve();
	virtual ~CPLHPCurve();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

public:
	// 刷新,并决定是否显示曲线
	// (in)bShow:是否显示曲线,true:显示,false:不显示
	// (return)无
	void ShowCurve(bool bShow);

	// 刷新,并决定是否显示另一曲线
	// (in)bShow:是否显示曲线,true:显示,false:不显示
	// (return)无
	void ShowAnotherCurve(bool bShow);

	// 设置方程参数,参数按降幂排序
	// 三阶方程
	// x单位:km/h,
	// y单位:kW
	// y = a*x*x*x + b*x*x + c*x + d;
	// (in)a:3阶参数
	// (in)b:2阶参数
	// (in)c:1阶参数
	// (in)d:0阶参数(常数)
	// (return)无
	void SetParams(double da, double db, double dc, double dd=0.0);

	// 设置另一方程方程参数,参数按降幂排序
	// 三阶方程
	// x单位:km/h,
	// y单位:kW
	// y = a*x*x*x + b*x*x + c*x + d;
	// (in)a:3阶参数
	// (in)b:2阶参数
	// (in)c:1阶参数
	// (in)d:0阶参数(常数)
	// (return)无
	void SetAnotherParams(double da, double db, double dc, double dd=0.0);

private:
	// 三阶函数
	// y = a*x*x*x + b*x*x + c*x + d;
	double m_da, m_db, m_dc, m_dd;
	double m_dAnothera, m_dAnotherb, m_dAnotherc, m_dAnotherd;
	// 是否显示曲线
	bool m_bDrawCurve;
	// 是否显示另一条曲线
	bool m_bDrawAnotherCurve;
	// 记号间隔,单位:像素
	const int m_nTickInterval;
	// 控件区域
	CRect m_rcClient;
	// x轴区域
	CRect m_rcXAxis;
	// y轴区域
	CRect m_rcYAxis;
	// y轴比例
	const int m_nScale;

	// 画笔
	CPen m_penBlack;
	CPen m_penBlue;
	CPen m_penWhite;
	CPen m_penGreen;
private:
	// 转换成屏幕坐标
	inline int Conversion(const int nX);
	// 转换成屏幕坐标
	inline int AnotherConversion(const int nX);
	// 画X轴相关
	inline void DrawXAxis(CPaintDC &dc);
	// 画Y轴相关
	inline void DrawYAxis(CPaintDC &dc);
	// 画曲线
	inline void DrawCurve(CPaintDC &dc);
	// 画另一条曲线
	inline void DrawAnotherCurve(CPaintDC &dc);
};
