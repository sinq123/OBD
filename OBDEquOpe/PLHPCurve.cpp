// PLHPCurve.cpp : 实现文件
//

#include "stdafx.h"
#include "PLHPCurve.h"


// CPLHPCurve

IMPLEMENT_DYNAMIC(CPLHPCurve, CStatic)

CPLHPCurve::CPLHPCurve()
: m_da(0.0)
, m_db(0.0)
, m_dc(0.0)
, m_dd(0.0)
, m_dAnothera(0.0)
, m_dAnotherb(0.0)
, m_dAnotherc(0.0)
, m_dAnotherd(0.0)
, m_bDrawCurve(false)
, m_bDrawAnotherCurve(false)
, m_nTickInterval(4)
, m_nScale(10)
{
	// 创建画笔
	m_penBlack.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	m_penBlue.CreatePen(PS_SOLID, 2, RGB(0, 102, 204));
	m_penWhite.CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
	m_penGreen.CreatePen(PS_SOLID, 2, RGB(0, 204, 102));
}

CPLHPCurve::~CPLHPCurve()
{
	// 销毁画笔
	m_penBlack.DeleteObject();
	m_penBlue.DeleteObject();
	m_penWhite.DeleteObject();
	m_penGreen.DeleteObject();
}


BEGIN_MESSAGE_MAP(CPLHPCurve, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CPLHPCurve 消息处理程序



void CPLHPCurve::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CStatic::OnPaint()

	//
	//
	//
	//(控件边框)				(Y轴)
	//  |						|
	//  |						|
	//  |-- 距离边框50个像素 ---|
	//	|						|
	//	|						|
	//	|						|
	//	|						|
	//	|						|
	//	|						|
	//	|						|
	//	|						|
	//	|						|
	//	|						|
	//	|						|
	//	|						|
	//	|						|
	//	|						|
	//	|						|
	//	|				  (原点)|________________________________________________________________________(X轴)
	//	|							|
	//	|						
	//	|						距离边框40个像素
	//	|						
	//	|___________________________|____________________________________________________________________(控件边框)
	//
	//
	//

	// 控件区域
	GetClientRect(&m_rcClient);
	// x轴区域
	m_rcXAxis.SetRect(m_rcClient.left+50, m_rcClient.bottom-40, m_rcClient.right, m_rcClient.bottom);
	// y轴区域
	m_rcYAxis.SetRect(m_rcClient.left, m_rcClient.top, m_rcClient.left+50, m_rcClient.bottom-40);

	// 控件区域底色
	dc.FillSolidRect(&m_rcClient, RGB(230, 234, 243));
	// X轴区域底色
	dc.FillSolidRect(&m_rcXAxis, RGB(234, 234, 234));
	// Y轴区域底色
	dc.FillSolidRect(&m_rcYAxis, RGB(234, 234, 234));

	// 随便选支笔,记录旧笔
	CPen *ppenOld = dc.SelectObject(&m_penBlack);

	// 画X轴相关
	DrawXAxis(dc);
	// 画Y轴相关
	DrawYAxis(dc);

	if (m_bDrawCurve)
	{
		// 画曲线
		DrawCurve(dc);
	}

	if (m_bDrawAnotherCurve)
	{
		// 画另一条曲线
		DrawAnotherCurve(dc);
	}

	dc.SelectObject(ppenOld);
}

void CPLHPCurve::DrawXAxis(CPaintDC &dc)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////X轴区域begin
	// 描绘X轴
	//dc.SelectObject(&m_penBlack);
	//dc.MoveTo(m_rcXAxis.right, m_rcXAxis.top);
	//dc.LineTo(m_rcXAxis.left, m_rcXAxis.top);
	// 描绘X轴刻度
	for (int i=0; i<=(m_rcXAxis.Width()/m_nTickInterval); i++)
	{
		// 刻度所在的X坐标
		int nXCoordinate = m_rcXAxis.left + i*m_nTickInterval;
		if (0 == i%8)
		{
			// 大刻度(逢8),长度:20个像素
			dc.SelectObject(&m_penBlack);
			dc.MoveTo(nXCoordinate, m_rcXAxis.top);
			dc.LineTo(nXCoordinate, m_rcXAxis.top+20);

			// 大刻度向上延伸,形成网格
			dc.SelectObject(&m_penWhite);
			dc.MoveTo(nXCoordinate, m_rcXAxis.top);
			dc.LineTo(nXCoordinate, m_rcClient.top);

			
			// 大刻度附近标上刻度值,只标到120
			if (i <= 120)
			{
				// 刻度值区域
				CRect rcTickValue(nXCoordinate-15, m_rcXAxis.top+20, nXCoordinate+15, m_rcXAxis.bottom);
				//rcTickValue.SetRect(nXCoordinate-15, m_rcXAxis.top+20, nXCoordinate+15, m_rcXAxis.bottom);
				// 防止文字区域超出控件
				if (rcTickValue.right <= m_rcClient.right)
				{
					dc.SelectObject(&m_penBlack);
					CString strTickText;
					strTickText.Format(L"%d", i);
					// 刻度值水平方向居中,垂直方向居中
					dc.DrawText(strTickText, rcTickValue, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
				}
			}
		}
		else
		{
			// 小刻度,长度:10个像素
			dc.SelectObject(&m_penBlack);
			dc.MoveTo(nXCoordinate, m_rcXAxis.top);
			dc.LineTo(nXCoordinate, m_rcXAxis.top+10);
		}
	}
	// 描绘X轴名称,距离X轴10个像素
	dc.SelectObject(&m_penBlack);
	CRect rcXAxisName(m_rcXAxis.right-90, m_rcXAxis.top-30, m_rcXAxis.right, m_rcXAxis.top-10);
	dc.DrawText(L"速度[km/h]", rcXAxisName, DT_CENTER|DT_SINGLELINE|DT_BOTTOM);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////X轴区域end
}

void CPLHPCurve::DrawYAxis(CPaintDC &dc)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Y轴区域begin
	// 描绘Y轴
	//dc.SelectObject(&m_penBlack);
	//dc.MoveTo(m_rcYAxis.right, m_rcYAxis.top);
	//dc.LineTo(m_rcYAxis.right, m_rcYAxis.bottom);
	// 描绘Y轴刻度
	for (int i=0; i<=(m_rcYAxis.Height()/m_nTickInterval); i++)
	{
		// 刻度所在的Y坐标
		int nYCoordinate = m_rcYAxis.bottom - i*m_nTickInterval;
		if (0 == i%10)
		{
			// 大刻度(逢10),长度:10个像素
			dc.SelectObject(&m_penBlack);
			dc.MoveTo(m_rcYAxis.right, nYCoordinate);
			dc.LineTo(m_rcYAxis.left+30, nYCoordinate);

			// 大刻度向右延伸,形成网格
			dc.SelectObject(&m_penWhite);
			dc.MoveTo(m_rcYAxis.right, nYCoordinate);
			dc.LineTo(m_rcClient.right, nYCoordinate);

			// 大刻度附近标上刻度值
			// 刻度值区域
			CRect rcTickValue(m_rcYAxis.left, nYCoordinate-10, m_rcYAxis.left+30, nYCoordinate+10);
			// 防止文字区域超出控件
			if (rcTickValue.top >= m_rcClient.top)
			{
				dc.SelectObject(&m_penBlack);
				CString strTickValue;
				strTickValue.Format(L"%d", i/m_nScale);
				// 刻度值水平方向靠右,垂直方向居中
				dc.DrawText(strTickValue, rcTickValue, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
			}
		}
		else if (0 == i%5)
		{
			// 中刻度(逢5),长度:15个像素
			dc.SelectObject(&m_penBlack);
			dc.MoveTo(m_rcYAxis.right, nYCoordinate);
			dc.LineTo(m_rcYAxis.left+35, nYCoordinate);
		}
		else
		{
			// 小刻度,长度:10个像素
			dc.SelectObject(&m_penBlack);
			dc.MoveTo(m_rcYAxis.right, nYCoordinate);
			dc.LineTo(m_rcYAxis.left+40, nYCoordinate);
		}
	}
	// 描绘Y轴名称,距离Y轴10个像素
	dc.SelectObject(&m_penBlack);
	CRect rcYAxisName(m_rcYAxis.right+10, m_rcYAxis.top, m_rcYAxis.right+100, m_rcYAxis.top+20);
	dc.DrawText(L"功率[kW]", rcYAxisName, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Y轴区域end
}

void CPLHPCurve::DrawCurve(CPaintDC &dc)
{
	dc.SelectObject(&m_penBlue);

	int nX = m_rcXAxis.left;
	int nY(0);
	nY = Conversion(nX);
	dc.MoveTo(nX, nY);
	for(nX=m_rcXAxis.left+1; nX<m_rcXAxis.right; nX++)
	{
		nY = Conversion(nX);
		// 不画超出控件范围的部分
		if (nY < m_rcClient.top)
		{
			break;
		}
		dc.LineTo(nX, nY);
	}
}

void CPLHPCurve::DrawAnotherCurve(CPaintDC &dc)
{
	dc.SelectObject(&m_penGreen);

	int nX = m_rcXAxis.left;
	int nY(0);
	nY = AnotherConversion(nX);
	dc.MoveTo(nX, nY);
	for(nX=m_rcXAxis.left+1; nX<m_rcXAxis.right; nX++)
	{
		nY = AnotherConversion(nX);
		// 不画超出控件范围的部分
		if (nY < m_rcClient.top)
		{
			break;
		}
		dc.LineTo(nX, nY);
	}
}

int CPLHPCurve::Conversion(const int nX)
{
	double dX = (double)(nX-m_rcXAxis.left) / m_nTickInterval;
	double dY = m_da*dX*dX*dX + m_db*dX*dX + m_dc*dX + m_dd;
	if (dY < 10e-6)
	{
		dY = 0.0;
	}
	int nY = m_rcYAxis.bottom-(int)(dY*m_nTickInterval*m_nScale + 0.5);
	return nY;
}

int CPLHPCurve::AnotherConversion(const int nX)
{
	double dX = (double)(nX-m_rcXAxis.left) / m_nTickInterval;
	double dY = m_dAnothera*dX*dX*dX + m_dAnotherb*dX*dX + m_dAnotherc*dX + m_dAnotherd;
	if (dY < 10e-6)
	{
		dY = 0.0;
	}
	int nY = m_rcYAxis.bottom-(int)(dY*m_nTickInterval*m_nScale + 0.5);
	return nY;
}

void CPLHPCurve::ShowCurve(bool bShow)
{
	m_bDrawCurve = bShow;
	RedrawWindow();
}

void CPLHPCurve::ShowAnotherCurve(bool bShow)
{
	m_bDrawAnotherCurve = bShow;
	RedrawWindow();
}

void CPLHPCurve::SetParams(double da, double db, double dc, double dd)
{
	m_da = da;
	m_db = db;
	m_dc = dc;
	m_dd = dd;
}

void CPLHPCurve::SetAnotherParams(double da, double db, double dc, double dd)
{
	m_dAnothera = da;
	m_dAnotherb = db;
	m_dAnotherc = dc;
	m_dAnotherd = dd;
}