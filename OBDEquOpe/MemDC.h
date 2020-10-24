/**
* @file MemDC.h
*
* ���ĵ�������CMemDC��
*
* @author ������
*/


#ifndef _MEMDC_H_
#define _MEMDC_H_

//////////////////////////////////////////////////
// CMemDC - memory DC
//
// Author: Keith Rule
// Email:  keithr@europa.com
// Copyright 1996-2002, Keith Rule
//
// You may freely use or modify this code provided this
// Copyright is included in all derived versions.
//
// History - 10/3/97 Fixed scrolling bug.
//                   Added print support. - KR
//
//           11/3/99 Fixed most common complaint. Added
//                   background color fill. - KR
//
//           11/3/99 Added support for mapping modes other than
//                   MM_TEXT as suggested by Lee Sang Hun. - KR
//
//           02/11/02 Added support for CScrollView as supplied
//                    by Gary Kirkham. - KR
//
// This class implements a memory Device Context which allows
// flicker free drawing.

/**
* @brief CMemDC�ڴ�DC��
*
* ��ֹ�ػ�ؼ�ʱ������˸
*/
class CMemDC : public CDC {
private:
	/// ��Ļƫ��λͼ
	CBitmap		m_bitmap;		// Offscreen bitmap
	/// ԭ�Ƚ�����λͼָ��
	CBitmap*	m_oldBitmap;	// bitmap originally found in CMemDC
	/// ���湹�캯�����ݵ�CDC
	CDC*		m_pDC;			// Saves CDC passed in constructor
	/// ��ͼ����
	CRect		m_rect;			// Rectangle of drawing area.
	/// �Ƿ��ڴ�DC
	BOOL		m_bMemDC;		// TRUE if CDC really is a Memory DC.
public:
	
	/// ���캯��
	CMemDC(CDC* pDC, const CRect* pRect = NULL) : CDC()
	{
		ASSERT(pDC != NULL); 

		// Some initialization

		m_pDC = pDC;
		if(m_pDC!=NULL)
		{
			m_oldBitmap = NULL;
			m_bMemDC = !m_pDC->IsPrinting();

			// Get the rectangle to draw
			if (pRect == NULL) {
				m_pDC->GetClipBox(&m_rect);
			} else {
				m_rect = *pRect;
			}

			if (m_bMemDC) {
				// Create a Memory DC
				CreateCompatibleDC(pDC);
				m_pDC->LPtoDP(&m_rect);

				m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
				m_oldBitmap = SelectObject(&m_bitmap);

				SetMapMode(m_pDC->GetMapMode());

				SetWindowExt(m_pDC->GetWindowExt());
				SetViewportExt(m_pDC->GetViewportExt());

				m_pDC->DPtoLP(&m_rect);
				SetWindowOrg(m_rect.left, m_rect.top);
			} else {
				// Make a copy of the relevent parts of the current DC for printing
				m_bPrinting = m_pDC->m_bPrinting;
				m_hDC       = m_pDC->m_hDC;
				m_hAttribDC = m_pDC->m_hAttribDC;
			}

			// Fill background 
			FillSolidRect(m_rect, m_pDC->GetBkColor());
		}
	}
	
	/// ��������
	~CMemDC()	
	{		
		if (m_bMemDC) {
			// Copy the offscreen bitmap onto the screen.
			m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
				this, m_rect.left, m_rect.top, SRCCOPY);			
			
			//Swap back the original bitmap.
			SelectObject(m_oldBitmap);		
		} else {
			// All we need to do is replace the DC with an illegal value,
			// this keeps us from accidently deleting the handles associated with
			// the CDC that was passed to the constructor.			
			m_hDC = m_hAttribDC = NULL;
		}	
	}
	
	// Allow usage as a pointer	
	CMemDC* operator->() 
	{
		return this;
	}	

	// Allow usage as a pointer	
	operator CMemDC*() 
	{
		return this;
	}
};

#endif