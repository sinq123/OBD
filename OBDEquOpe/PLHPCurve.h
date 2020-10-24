/************************************************* 
  Copyright (C), 2009-2019, HuaBei 
  �ļ���: PLHPCurve.h
  ����: Cui
  �汾: V1.0.0
  ����: 2009-06-01
  ����: ����������ʽ,��ʾ�⹦�������������ߡ�
		���У�x��Ϊ�ٶȣ���λ:km/h��y��Ϊ�������ʣ���λ:kW
		��������ʾ2�����ߣ���������ͬʱ��ʾ��ʣ�����Ͳ���ʣ����2�����ߣ����бȽ�
  ����: ��
  ��ʷ:
  1. ʱ�䣺2009-06-01
	 ���ߣ�Cui
	 ��������ɣ����в���
  2. ʱ�䣺2009-06-01
     ���ߣ�Cui
	 �������ĳɿ���ͬʱ��ʾ2������
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
	// ˢ��,�������Ƿ���ʾ����
	// (in)bShow:�Ƿ���ʾ����,true:��ʾ,false:����ʾ
	// (return)��
	void ShowCurve(bool bShow);

	// ˢ��,�������Ƿ���ʾ��һ����
	// (in)bShow:�Ƿ���ʾ����,true:��ʾ,false:����ʾ
	// (return)��
	void ShowAnotherCurve(bool bShow);

	// ���÷��̲���,��������������
	// ���׷���
	// x��λ:km/h,
	// y��λ:kW
	// y = a*x*x*x + b*x*x + c*x + d;
	// (in)a:3�ײ���
	// (in)b:2�ײ���
	// (in)c:1�ײ���
	// (in)d:0�ײ���(����)
	// (return)��
	void SetParams(double da, double db, double dc, double dd=0.0);

	// ������һ���̷��̲���,��������������
	// ���׷���
	// x��λ:km/h,
	// y��λ:kW
	// y = a*x*x*x + b*x*x + c*x + d;
	// (in)a:3�ײ���
	// (in)b:2�ײ���
	// (in)c:1�ײ���
	// (in)d:0�ײ���(����)
	// (return)��
	void SetAnotherParams(double da, double db, double dc, double dd=0.0);

private:
	// ���׺���
	// y = a*x*x*x + b*x*x + c*x + d;
	double m_da, m_db, m_dc, m_dd;
	double m_dAnothera, m_dAnotherb, m_dAnotherc, m_dAnotherd;
	// �Ƿ���ʾ����
	bool m_bDrawCurve;
	// �Ƿ���ʾ��һ������
	bool m_bDrawAnotherCurve;
	// �Ǻż��,��λ:����
	const int m_nTickInterval;
	// �ؼ�����
	CRect m_rcClient;
	// x������
	CRect m_rcXAxis;
	// y������
	CRect m_rcYAxis;
	// y�����
	const int m_nScale;

	// ����
	CPen m_penBlack;
	CPen m_penBlue;
	CPen m_penWhite;
	CPen m_penGreen;
private:
	// ת������Ļ����
	inline int Conversion(const int nX);
	// ת������Ļ����
	inline int AnotherConversion(const int nX);
	// ��X�����
	inline void DrawXAxis(CPaintDC &dc);
	// ��Y�����
	inline void DrawYAxis(CPaintDC &dc);
	// ������
	inline void DrawCurve(CPaintDC &dc);
	// ����һ������
	inline void DrawAnotherCurve(CPaintDC &dc);
};
