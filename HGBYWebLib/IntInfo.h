#ifndef INTINFO_H
#define INTINFO_H

#include <string>

// ���ض���
struct SHGBYMSG
{
	// code
	std::wstring strcode;
	// message
	std::wstring strmessage;
	// body
	std::wstring strbody;
};

struct SGetJobList
{
	//AUT_PLATE	String	���ƺ���
	std::wstring strAUT_PLATE;
	//AUT_PCLASS	String	��������
	std::wstring strAUT_PCLASS;
	//DETECT_ID	Long	�����ˮ��
	std::wstring strDETECT_ID;
	//DET_STATUS	Int	���״̬
	std::wstring strDET_STATUS;
	//DET_DATE	String	���ʱ��
	std::wstring strDET_DATE;
	//INSPECT_ID	String	�����ĿID//ASM  ��̬��������//LUGD ���ؼ��ٷ���//TSIC ˫���ٷ���//FRAO���ɼ���-��͸���̶ȼƷ���//FRAF ���ɼ���-��ֽ�̶ȷ���
	std::wstring strINSPECT_ID;
	//UNDO_ITEMS	String	δ�����Ŀ���ԡ�,���ֿ�,�硰V,O,G��//V ��ۼ��//O OBD���//G β�����
	std::wstring strUNDO_ITEMS;
	//TESTCATEGORY	String	��������,1- ���ڼ��顢2-ע��ǼǼ��顢3-ʵ��ȶ� 4- �ල�Գ�졢5- ����ǼǺ�ת�ƵǼǼ���
	std::wstring strTESTCATEGORY;
};

#endif