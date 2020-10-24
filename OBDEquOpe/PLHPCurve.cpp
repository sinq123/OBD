// PLHPCurve.cpp : ʵ���ļ�
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
	// ��������
	m_penBlack.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	m_penBlue.CreatePen(PS_SOLID, 2, RGB(0, 102, 204));
	m_penWhite.CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
	m_penGreen.CreatePen(PS_SOLID, 2, RGB(0, 204, 102));
}

CPLHPCurve::~CPLHPCurve()
{
	// ���ٻ���
	m_penBlack.DeleteObject();
	m_penBlue.DeleteObject();
	m_penWhite.DeleteObject();
	m_penGreen.DeleteObject();
}


BEGIN_MESSAGE_MAP(CPLHPCurve, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CPLHPCurve ��Ϣ�������



void CPLHPCurve::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CStatic::OnPaint()

	//
	//
	//
	//(�ؼ��߿�)				(Y��)
	//  |						|
	//  |						|
	//  |-- ����߿�50������ ---|
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
	//	|				  (ԭ��)|________________________________________________________________________(X��)
	//	|							|
	//	|						
	//	|						����߿�40������
	//	|						
	//	|___________________________|____________________________________________________________________(�ؼ��߿�)
	//
	//
	//

	// �ؼ�����
	GetClientRect(&m_rcClient);
	// x������
	m_rcXAxis.SetRect(m_rcClient.left+50, m_rcClient.bottom-40, m_rcClient.right, m_rcClient.bottom);
	// y������
	m_rcYAxis.SetRect(m_rcClient.left, m_rcClient.top, m_rcClient.left+50, m_rcClient.bottom-40);

	// �ؼ������ɫ
	dc.FillSolidRect(&m_rcClient, RGB(230, 234, 243));
	// X�������ɫ
	dc.FillSolidRect(&m_rcXAxis, RGB(234, 234, 234));
	// Y�������ɫ
	dc.FillSolidRect(&m_rcYAxis, RGB(234, 234, 234));

	// ���ѡ֧��,��¼�ɱ�
	CPen *ppenOld = dc.SelectObject(&m_penBlack);

	// ��X�����
	DrawXAxis(dc);
	// ��Y�����
	DrawYAxis(dc);

	if (m_bDrawCurve)
	{
		// ������
		DrawCurve(dc);
	}

	if (m_bDrawAnotherCurve)
	{
		// ����һ������
		DrawAnotherCurve(dc);
	}

	dc.SelectObject(ppenOld);
}

void CPLHPCurve::DrawXAxis(CPaintDC &dc)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////X������begin
	// ���X��
	//dc.SelectObject(&m_penBlack);
	//dc.MoveTo(m_rcXAxis.right, m_rcXAxis.top);
	//dc.LineTo(m_rcXAxis.left, m_rcXAxis.top);
	// ���X��̶�
	for (int i=0; i<=(m_rcXAxis.Width()/m_nTickInterval); i++)
	{
		// �̶����ڵ�X����
		int nXCoordinate = m_rcXAxis.left + i*m_nTickInterval;
		if (0 == i%8)
		{
			// ��̶�(��8),����:20������
			dc.SelectObject(&m_penBlack);
			dc.MoveTo(nXCoordinate, m_rcXAxis.top);
			dc.LineTo(nXCoordinate, m_rcXAxis.top+20);

			// ��̶���������,�γ�����
			dc.SelectObject(&m_penWhite);
			dc.MoveTo(nXCoordinate, m_rcXAxis.top);
			dc.LineTo(nXCoordinate, m_rcClient.top);

			
			// ��̶ȸ������Ͽ̶�ֵ,ֻ�굽120
			if (i <= 120)
			{
				// �̶�ֵ����
				CRect rcTickValue(nXCoordinate-15, m_rcXAxis.top+20, nXCoordinate+15, m_rcXAxis.bottom);
				//rcTickValue.SetRect(nXCoordinate-15, m_rcXAxis.top+20, nXCoordinate+15, m_rcXAxis.bottom);
				// ��ֹ�������򳬳��ؼ�
				if (rcTickValue.right <= m_rcClient.right)
				{
					dc.SelectObject(&m_penBlack);
					CString strTickText;
					strTickText.Format(L"%d", i);
					// �̶�ֵˮƽ�������,��ֱ�������
					dc.DrawText(strTickText, rcTickValue, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
				}
			}
		}
		else
		{
			// С�̶�,����:10������
			dc.SelectObject(&m_penBlack);
			dc.MoveTo(nXCoordinate, m_rcXAxis.top);
			dc.LineTo(nXCoordinate, m_rcXAxis.top+10);
		}
	}
	// ���X������,����X��10������
	dc.SelectObject(&m_penBlack);
	CRect rcXAxisName(m_rcXAxis.right-90, m_rcXAxis.top-30, m_rcXAxis.right, m_rcXAxis.top-10);
	dc.DrawText(L"�ٶ�[km/h]", rcXAxisName, DT_CENTER|DT_SINGLELINE|DT_BOTTOM);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////X������end
}

void CPLHPCurve::DrawYAxis(CPaintDC &dc)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Y������begin
	// ���Y��
	//dc.SelectObject(&m_penBlack);
	//dc.MoveTo(m_rcYAxis.right, m_rcYAxis.top);
	//dc.LineTo(m_rcYAxis.right, m_rcYAxis.bottom);
	// ���Y��̶�
	for (int i=0; i<=(m_rcYAxis.Height()/m_nTickInterval); i++)
	{
		// �̶����ڵ�Y����
		int nYCoordinate = m_rcYAxis.bottom - i*m_nTickInterval;
		if (0 == i%10)
		{
			// ��̶�(��10),����:10������
			dc.SelectObject(&m_penBlack);
			dc.MoveTo(m_rcYAxis.right, nYCoordinate);
			dc.LineTo(m_rcYAxis.left+30, nYCoordinate);

			// ��̶���������,�γ�����
			dc.SelectObject(&m_penWhite);
			dc.MoveTo(m_rcYAxis.right, nYCoordinate);
			dc.LineTo(m_rcClient.right, nYCoordinate);

			// ��̶ȸ������Ͽ̶�ֵ
			// �̶�ֵ����
			CRect rcTickValue(m_rcYAxis.left, nYCoordinate-10, m_rcYAxis.left+30, nYCoordinate+10);
			// ��ֹ�������򳬳��ؼ�
			if (rcTickValue.top >= m_rcClient.top)
			{
				dc.SelectObject(&m_penBlack);
				CString strTickValue;
				strTickValue.Format(L"%d", i/m_nScale);
				// �̶�ֵˮƽ������,��ֱ�������
				dc.DrawText(strTickValue, rcTickValue, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
			}
		}
		else if (0 == i%5)
		{
			// �п̶�(��5),����:15������
			dc.SelectObject(&m_penBlack);
			dc.MoveTo(m_rcYAxis.right, nYCoordinate);
			dc.LineTo(m_rcYAxis.left+35, nYCoordinate);
		}
		else
		{
			// С�̶�,����:10������
			dc.SelectObject(&m_penBlack);
			dc.MoveTo(m_rcYAxis.right, nYCoordinate);
			dc.LineTo(m_rcYAxis.left+40, nYCoordinate);
		}
	}
	// ���Y������,����Y��10������
	dc.SelectObject(&m_penBlack);
	CRect rcYAxisName(m_rcYAxis.right+10, m_rcYAxis.top, m_rcYAxis.right+100, m_rcYAxis.top+20);
	dc.DrawText(L"����[kW]", rcYAxisName, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Y������end
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
		// ���������ؼ���Χ�Ĳ���
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
		// ���������ؼ���Χ�Ĳ���
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