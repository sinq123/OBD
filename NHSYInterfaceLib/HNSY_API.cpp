#include "StdAfx.h"
#include "HNSY_API.h"

#include "gsoap\soapDeviceSwapIfaceImplServiceSoapBindingProxy.h"
#include "gsoap\DeviceSwapIfaceImplServiceSoapBinding.nsmap"

std::wstring CHNSY_API::m_strLogFilePath = L"";

CHNSY_API::CHNSY_API(void)
{
	// Ĭ����־λ��
	m_strLogFilePath = LogFilePath().c_str();
}

CHNSY_API::~CHNSY_API(void)
{
}

std::wstring CHNSY_API::LogFilePath(void)
{
	// ��־�ļ������ļ���·��
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetCDFilePath(L"", wchLogFileFolderPath, true);

	COleDateTime odtNow(COleDateTime::GetCurrentTime());

	CStringW strFileName;
	strFileName.Format(L"HNSY%s.log", odtNow.Format(L"%Y-%m-%d"));

	CStringW strLogFilePath;
	strLogFilePath = wchLogFileFolderPath;
	strLogFilePath += strFileName;

	return strLogFilePath.GetString();
}

DWORD CHNSY_API::SetLogFilePath(const std::wstring& strLogFilePath)
{
	if (strLogFilePath.empty())
	{
		m_strLogFilePath = LogFilePath();
	}
	else
	{
		m_strLogFilePath = strLogFilePath;
	}
	return 0x00;
}

// 5.1.1��ȡ�������ƽӿ� getAccessToken
int CHNSY_API::getAccessToken(const char * pchURL, const std::wstring& strjcxbh, std::wstring& strRetStr)
{
	int nRet;

	DeviceSwapIfaceImplServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;


	ns1__getAccessToken send;
	ns1__getAccessTokenResponse sret;

	std::wstring m_strjcxbh = strjcxbh;
	send.jcxbh = &m_strjcxbh;

	CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"getAccessToken", send.jcxbh->c_str());

	nRet = toasb.getAccessToken(&send, sret);

	if (nRet == SOAP_OK)
	{
		strRetStr = sret.return_->c_str();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"getAccessToken", strRetStr.c_str());
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"getAccessToken", L"����ʧ��");
	}

	return nRet;
}
// 5.1.2��ȡ���쳵���ӿ� 
int CHNSY_API:: getDjzt(const char * pchURL, const std::wstring& straccessToken, std::wstring& strRetStr)
{
	int nRet;

	DeviceSwapIfaceImplServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;


	ns1__getDjzt send;
	ns1__getDjztResponse sret;

	std::wstring m_straccessToken = straccessToken;


	send.accessToken = &m_straccessToken;

	CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"getDjzt", send.accessToken->c_str());

	nRet = toasb.getDjzt(&send, sret);

	if (nRet == SOAP_OK)
	{
		strRetStr = sret.return_->c_str();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"getDjzt", strRetStr.c_str());
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"getDjzt", L"����ʧ��");
	}

	return nRet;
}
// 5.1.3��ȡ������Ϣ�ӿ� 
int CHNSY_API:: getDjclxx(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, std::wstring& strRetStr)
{
	int nRet;

	DeviceSwapIfaceImplServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;


	ns1__getDjclxx send;
	ns1__getDjclxxResponse sret;

	std::wstring m_straccessToken = straccessToken;
	std::wstring m_strlsh = strlsh;
	send.accessToken = &m_straccessToken;
	send.lsh = &m_strlsh;

	CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"getDjclxx", send.accessToken->c_str());

	nRet = toasb.getDjclxx(&send, sret);

	if (nRet == SOAP_OK)
	{
		strRetStr = sret.return_->c_str();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"getDjclxx", strRetStr.c_str());
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"getDjclxx", L"����ʧ��");
	}

	return nRet;
}
// 5.1.4������Ϣ֪ͨ�ӿ�
int CHNSY_API:: xxtz(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strcode,
	const std::wstring& strmessage, std::wstring& strRetStr)
{
	int nRet;
	
	DeviceSwapIfaceImplServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	ns1__xxtz send;
	ns1__xxtzResponse sret;

	std::wstring m_straccessToken = straccessToken;
	std::wstring m_strlsh = strlsh;
	std::wstring m_strcode = strcode;
	std::wstring m_strmessage = strmessage;

	send.accessToken = &m_straccessToken;
	send.lsh = &m_strlsh;
	send.code = &m_strcode;
	send.message = &m_strmessage;

	CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"xxtz", send.accessToken->c_str());

	nRet = toasb.xxtz(&send, sret);

	if (nRet == SOAP_OK)
	{
		strRetStr = sret.return_->c_str();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"xxtz", strRetStr.c_str());
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"xxtz", L"����ʧ��");
	}

	return nRet;
}
// 5.1.5�ϴ���������汾�Žӿ�
int CHNSY_API:: gkrjbbh(const char * pchURL, const std::wstring& straccessToken, const std::wstring& strbbh, std::wstring& strRetStr)
{
	int nRet;
	
	DeviceSwapIfaceImplServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	ns1__gkrjbbh send;
	ns1__gkrjbbhResponse sret;

	std::wstring m_straccessToken = straccessToken;
	std::wstring m_strbbh = strbbh;

	send.accessToken = &m_straccessToken;
	send.bbh = &m_strbbh;


	CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"gkrjbbh", send.accessToken->c_str());

	nRet = toasb.gkrjbbh(&send, sret);

	if (nRet == SOAP_OK)
	{
		strRetStr = sret.return_->c_str();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"gkrjbbh", strRetStr.c_str());
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"gkrjbbh", L"����ʧ��");
	}

	return nRet;
}
// 5.1.6������ʱ��ͬ���ӿ�
int CHNSY_API:: fwqsjtb(const char * pchURL, std::wstring& strRetStr)
{
	int nRet;
	
	DeviceSwapIfaceImplServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	ns1__fwqsjtb send;
	ns1__fwqsjtbResponse sret;

	CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"fwqsjtb", L"");

	nRet = toasb.fwqsjtb(&send, sret);

	if (nRet == SOAP_OK)
	{
		strRetStr = sret.return_->c_str();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"fwqsjtb", strRetStr.c_str());
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"fwqsjtb", L"����ʧ��");
	}

	return nRet;
}
// 5.1.7���վ�����Ա�б�ӿ� //û�иýӿ�
int CHNSY_API:: jcrylb(const char * pchURL, const std::wstring& strjczbh, const std::wstring& strjsdm, std::wstring& strRetStr)
{
	int nRet;
	//
	//DeviceSwapIfaceImplServiceSoapBindingProxy toasb;
	//soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	//toasb.soap_endpoint = pchURL;

	//ns1__gkrjbbh send;
	//ns1__gkrjbbhResponse sret;

	//std::wstring m_straccessToken = straccessToken;
	//std::wstring m_strbbh = strbbh;

	//send.accessToken = &m_straccessToken;
	//send.bbh = &m_strbbh;


	//CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"jcrylb", send.accessToken->c_str());

	//nRet = toasb.jcrylb(&send, sret);

	//if (nRet == SOAP_OK)
	//{
	//	strRetStr = sret.return_->c_str();
	//	CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"jcrylb", strRetStr.c_str());
	//}
	//else
	//{
	//	CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"jcrylb", L"����ʧ��");
	//}

	return nRet;
}
// 5.1.8OBD�����б�ӿ�
int CHNSY_API:: obdDjlb(const char * pchURL, const std::wstring& strjczbh, const std::wstring& strcphm, std::wstring& strRetStr)
{
	int nRet;
	
	DeviceSwapIfaceImplServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	ns1__obdDjlb send;
	ns1__obdDjlbResponse sret;

	std::wstring m_strjczbh = strjczbh;
	std::wstring m_strcphm = strcphm;

	send.jczbh = &m_strjczbh;
	send.cphm = &m_strcphm;


	CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"obdDjlb", send.jczbh->c_str());

	nRet = toasb.obdDjlb(&send, sret);

	if (nRet == SOAP_OK)
	{
		strRetStr = sret.return_->c_str();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"obdDjlb", strRetStr.c_str());
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"obdDjlb", L"����ʧ��");
	}

	return nRet;
}
// 5.2.1�⹦�����ػ��м�����ӿ�
//accessToken	��������
//gsjzllsj	���ټ��ػ�������ʱ��
//gsjzsjsj	���ټ��ػ���ʵ��ʱ��
//dsjzllsj	���ټ��ػ�������ʱ��
//dsjzsjsj	���ټ��ػ���ʵ��ʱ��
//gsjzns	���ټ��ػ���������
//dsjzns	���ټ��ػ���������
//gsjzhxgl	���ټ��ػ���ָʾ����
//dsjzhxgl	���ټ��ػ���ָʾ����
//jbgl	��������
//gsjzjcjg	���ټ��ػ��н��
//dsjzjcjg	���ټ��ػ��н��
//jcjg	�����
//kssj	��鿪ʼʱ��
//jssj	������ʱ��
//bz	��ע
int CHNSY_API:: cgjzj(const char * pchURL, const std::wstring& straccessToken, const std::wstring& strgsjzllsj, const std::wstring& strgsjzsjsj,
	const std::wstring& strdsjzllsj, const std::wstring& strdsjzsjsj, const std::wstring& strgsjzns, const std::wstring& strdsjzns,
	const std::wstring& strgsjzhxgl, const std::wstring& strdsjzhxgl, const std::wstring& strjbgl, const std::wstring& strgsjzjcjg,
	const std::wstring& strdsjzjcjg, const std::wstring& strjcjg, const std::wstring& strkssj, const std::wstring& strjssj,
	const std::wstring& strbz, std::wstring& strRetStr)
{
	int nRet;
	
	DeviceSwapIfaceImplServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	ns1__cgjzj send;
	ns1__cgjzjResponse sret;

	std::wstring m_straccessToken = straccessToken;
	std::wstring m_strdsjzjcjg = strdsjzjcjg;
	std::wstring m_strjcjg = strjcjg;
	std::wstring m_strkssj = strkssj;
	std::wstring m_strjssj = strjssj;
	std::wstring m_strbz = strbz;

	send.accessToken = &m_straccessToken;
	send.gsjzllsj = _wtof(strgsjzllsj.c_str());
	send.gsjzsjsj = _wtof(strgsjzsjsj.c_str());
	send.dsjzllsj = _wtof(strdsjzllsj.c_str());
	send.dsjzsjsj = _wtof(strdsjzsjsj.c_str());
	send.gsjzns = _wtof(strgsjzns.c_str());
	send.dsjzns = _wtof(strdsjzns.c_str());
	send.gsjzhxgl = _wtof(strgsjzhxgl.c_str());
	send.dsjzhxgl = _wtof(strdsjzhxgl.c_str());
	send.jbgl = _wtof(strjbgl.c_str());
	send.dsjzjcjg = &m_strdsjzjcjg;
	send.jcjg = &m_strjcjg;
	send.kssj = &m_strkssj;
	send.jssj = &m_strjssj;
	send.bz = &m_strbz;

	CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"cgjzj", send.accessToken->c_str());

	nRet = toasb.cgjzj(&send, sret);

	if (nRet == SOAP_OK)
	{
		strRetStr = sret.return_->c_str();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"cgjzj", strRetStr.c_str());
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"cgjzj", L"����ʧ��");
	}

	return nRet;
}
// 5.2.2����������й¶���ӿ�
//accessToken	��������
//jcjg	�ܷ��Լ����
//kssj	��鿪ʼʱ��
//jssj	������ʱ��
//bz	��ע
int CHNSY_API:: wqfxyzj(const char * pchURL, const std::wstring& straccessToken, const std::wstring& strjcjg, const std::wstring& strkssj,
	const std::wstring& strjssj, const std::wstring& strbz, std::wstring& strRetStr)
{
	int nRet;
	
	DeviceSwapIfaceImplServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	ns1__wqfxyzj send;
	ns1__wqfxyzjResponse sret;

	std::wstring m_straccessToken = straccessToken;
	std::wstring m_strjcjg = strjcjg;
	std::wstring m_strkssj = strkssj;
	std::wstring m_strjssj = strjssj;
	std::wstring m_strbz = strbz;

	send.accessToken = &m_straccessToken;
	send.jcjg = &m_strjcjg;
	send.kssj = &m_strkssj;
	send.jssj = &m_strjssj;
	send.bz = &m_strbz;

	CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"wqfxyzj", send.accessToken->c_str());

	nRet = toasb.wqfxyzj(&send, sret);

	if (nRet == SOAP_OK)
	{
		strRetStr = sret.return_->c_str();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"wqfxyzj", strRetStr.c_str());
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"wqfxyzj", L"����ʧ��");
	}

	return nRet;
}
// 5.2.3��͸���̶ȼ�У׼������ӿ�
//accessToken	��������
//ldjcjg	��͸���̶ȼ��������
//lgpz1	��͸���̶ȼ�1�˹�Ƭֵ
//lgpz2	��͸���̶ȼ�2�˹�Ƭֵ
//jcz1	��͸���̶ȼ�1���ֵ
//jcz2	��͸���̶ȼ�2���ֵ
//wcl1	��͸���̶ȼ�1�����
//wcl2	��͸���̶ȼ�2�����
//jcjg	�����
//kssj	��鿪ʼʱ��
//jssj	������ʱ��
//bz	��ע
int CHNSY_API:: ydjzj(const char * pchURL, const std::wstring& straccessToken, const std::wstring& strldjcjg, const std::wstring& strlgpz1,
	const std::wstring& strlgpz2, const std::wstring& strjcz1, const std::wstring& strjcz2, const std::wstring& strwcl1,
	const std::wstring& strwcl2, const std::wstring& strjcjg,
	const std::wstring& kssj, const std::wstring& jssj, const std::wstring& strbz, std::wstring& strRetStr)
{
	int nRet;
	
	DeviceSwapIfaceImplServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	ns1__ydjzj send;
	ns1__ydjzjResponse sret;

	std::wstring m_straccessToken = straccessToken;
	std::wstring m_strldjcjg = strldjcjg;
	std::wstring m_strjcjg = strjcjg;
	std::wstring m_strkssj = kssj;
	std::wstring m_strjssj = jssj;
	std::wstring m_strbz = strbz;

	send.accessToken = &m_straccessToken;
	send.jcjg = &m_strjcjg;
	send.ldjcjg = &m_strldjcjg;
	send.lgpz1 = _wtof(strlgpz1.c_str());
	send.lgpz2 = _wtof(strlgpz2.c_str());
	send.jcz1 = _wtof(strjcz1.c_str());
	send.jcz2 = _wtof(strjcz2.c_str());
	send.wcl1 = _wtof(strwcl1.c_str());
	send.wcl2 = _wtof(strwcl2.c_str());
	send.kssj = &m_strkssj;
	send.kssj = &m_strkssj;
	send.jcjg = &m_strjcjg;
	send.bz = &m_strbz;

	CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"ydjzj", send.accessToken->c_str());

	nRet = toasb.ydjzj(&send, sret);

	if (nRet == SOAP_OK)
	{
		strRetStr = sret.return_->c_str();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"ydjzj", strRetStr.c_str());
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"ydjzj", L"����ʧ��");
	}

	return nRet;
}
// 5.2.4����������(����վ)У׼���ӿ�
//accessToken	��������
//sjhjwd	ʵ�ʻ����¶�
//clhjwd	���������¶�
//sjhjsd	ʵ�ʻ������ʪ��
//clhjsd	�����������ʪ��
//sjdqyl	ʵ�ʻ�������ѹ��
//cldqyl	������������ѹ��
//kssj	��鿪ʼʱ��
//jssj	������ʱ��
//bz	��ע
int CHNSY_API:: hjcsyzj(const char * pchURL, const std::wstring& straccessToken, const std::wstring& strsjhjwd, const std::wstring& strclhjwd,
	const std::wstring& strsjhjsd, const std::wstring& strclhjsd, const std::wstring& strsjdqyl, const std::wstring& strcldqyl,
	const std::wstring& kssj, const std::wstring& jssj, const std::wstring& strbz, std::wstring& strRetStr)
{
	int nRet;
	
	DeviceSwapIfaceImplServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	ns1__hjcsyzj send;
	ns1__hjcsyzjResponse sret;

	std::wstring m_straccessToken = straccessToken;
	std::wstring m_strkssj = kssj;
	std::wstring m_strjssj = jssj;
	std::wstring m_strbz = strbz;

	send.accessToken = &m_straccessToken;
	send.sjhjwd = _wtof(strsjhjwd.c_str());
	send.clhjwd = _wtof(strclhjwd.c_str());
	send.sjhjsd = _wtof(strsjhjsd.c_str());
	send.clhjsd = _wtof(strclhjsd.c_str());
	send.sjdqyl = _wtof(strsjdqyl.c_str());
	send.cldqyl = _wtof(strcldqyl.c_str());
	send.kssj = &m_strkssj;
	send.kssj = &m_strkssj;
	send.bz = &m_strbz;

	CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"hjcsyzj", send.accessToken->c_str());

	nRet = toasb.hjcsyzj(&send, sret);

	if (nRet == SOAP_OK)
	{
		strRetStr = sret.return_->c_str();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"hjcsyzj", strRetStr.c_str());
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"hjcsyzj", L"����ʧ��");
	}

	return nRet;
}
// 5.2.5����������У׼����ӿ�
//accessToken	��������
//bqlb	�������
//bzCO2	CO2����Ũ��ֵ
//sjCO2	CO2ʵ��Ũ��ֵ
//bzCO	CO����Ũ��ֵ
//sjCO	COʵ��Ũ��ֵ
//bzNO	NO����Ũ��ֵ
//sjNO	NOʵ��Ũ��ֵ
//bzHC	HC����Ũ��ֵ
//sjHC	HCʵ��Ũ��ֵ
//bzO2	O2����Ũ��ֵ
//sjO2	O2ʵ��Ũ��ֵ
//sjPEF	������鵱��ϵ��
//bzC3H8	��������Ũ��ֵ
//jcjg	У׼���
//kssj	У׼��ʼʱ��
//jssj	У׼����ʱ��
//bz	��ע
//bz	��ע
int CHNSY_API:: wqfxybd(const char * pchURL, const std::wstring& straccessToken, const std::wstring& strbqlb, const std::wstring& strbzCO2,
	const std::wstring& strsjCO2, const std::wstring& strbzCO, const std::wstring& strsjCO, const std::wstring& strbzNO,
	const std::wstring& strsjNO, const std::wstring& strbzHC, const std::wstring& strsjHC, const std::wstring& strbzO2,
	const std::wstring& strsjO2, const std::wstring& strsjPEF, const std::wstring& strbzC3H8, const std::wstring& strjcjg,
	const std::wstring& kssj, const std::wstring& jssj, const std::wstring& strbz, std::wstring& strRetStr)
{
	int nRet;
	
	DeviceSwapIfaceImplServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	ns1__wqfxybd send;
	ns1__wqfxybdResponse sret;

	std::wstring m_straccessToken = straccessToken;
	std::wstring m_strkssj = kssj;
	std::wstring m_strjssj = jssj;
	std::wstring m_strbz = strbz;
	std::wstring m_strjcjg = strjcjg;
	std::wstring m_strbqlb = strbqlb;

	send.accessToken = &m_straccessToken;
	send.bqlb = &m_strbqlb;
	send.bzCO2 = _wtof(strbzCO2.c_str());
	send.sjCO2 = _wtof(strsjCO2.c_str());
	send.bzCO = _wtof(strbzCO.c_str());
	send.sjCO = _wtof(strsjCO.c_str());
	send.bzNO = _wtoi(strbzNO.c_str());
	send.sjNO = _wtoi(strsjNO.c_str());
	send.bzHC = _wtoi(strbzHC.c_str());
	send.sjHC = _wtoi(strsjHC.c_str());
	send.bzO2 = _wtof(strbzO2.c_str());
	send.sjO2 = _wtof(strsjO2.c_str());
	send.sjPEF = _wtof(strsjPEF.c_str());
	send.bzC3H8 = _wtof(strbzC3H8.c_str());
	send.jcjg = &m_strjcjg;
	send.kssj = &m_strkssj;
	send.jssj = &m_strjssj;
	send.bz = &m_strbz;

	CString strMsg;
	strMsg.Format(L"accessToken:%s, bqlb:%s, bzCO2:%.2f, sjCO2:%.2f, bzCO:%.2f, sjCO:%.2f, bzNO:%.2f, sjNO:%.2f, bzHC:%.2f, sjHC:%.2f, bzO2:%.2f, sjO2:%.2f, sjPEF:%.3f, bzC3H8:%.2f\
				   jcjg:%s,,kssj:%s, jssj:%s, bz:%s", 
		send.accessToken->c_str(), send.bqlb->c_str(), send.bzCO2, send.sjCO2, send.bzCO, send.sjCO, send.bzNO, send.sjNO, send.bzHC, send.sjHC, 
		send.bzO2, send.sjO2, send.sjPEF, send.bzC3H8, send.jcjg->c_str(), send.kssj->c_str(), send.jssj->c_str(), send.bz->c_str());

	CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"wqfxybd", strMsg);

	nRet = toasb.wqfxybd(&send, sret);

	if (nRet == SOAP_OK)
	{
		strRetStr = sret.return_->c_str();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"wqfxybd", strRetStr.c_str());
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"wqfxybd", L"����ʧ��");
	}

	return nRet;
}
// 5.2.6�������豸�Լ����ӿ�
//accessToken	��������
//jqglljcjg	���������������
//lljylcjcjg	�����������̼����
//yqglcbz	���������̱�ֵ
//yqglcclz	���������̲���ֵ
//yqglcwc	�������������
//yqdlcbz	���������̱�ֵ
//yqdlcclz	���������̲���ֵ
//yqdlcwc	�������������
//jcjg	�����
//kssj	��鿪ʼʱ��
//jssj	������ʱ��
//remark	��ע
int CHNSY_API:: lljzj(const char * pchURL, const std::wstring& straccessToken, const std::wstring& strjqglljcjg, const std::wstring& strlljylcjcjg,
	const std::wstring& stryqglcbz, const std::wstring& stryqglcclz, const std::wstring& stryqglcwc, const std::wstring& stryqdlcbz,
	const std::wstring& stryqdlcclz, const std::wstring& stryqdlcwc, const std::wstring& strjcjg,
	const std::wstring& kssj, const std::wstring& jssj, const std::wstring& strbz, std::wstring& strRetStr)
{
	int nRet;
	
	DeviceSwapIfaceImplServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	ns1__lljzj send;
	ns1__lljzjResponse sret;

	std::wstring m_straccessToken = straccessToken;
	std::wstring m_strkssj = kssj;
	std::wstring m_strjssj = jssj;
	std::wstring m_strbz = strbz;
	std::wstring m_strjcjg = strjcjg;

	send.accessToken = &m_straccessToken;

	send.jqglljcjg = _wtof(strjqglljcjg.c_str());
	send.lljylcjcjg = _wtof(strlljylcjcjg.c_str());
	send.yqglcbz = _wtof(stryqglcbz.c_str());
	send.yqglcclz = _wtof(stryqglcclz.c_str());
	send.yqglcwc = _wtof(stryqglcwc.c_str());
	send.yqdlcbz = _wtof(stryqdlcbz.c_str());
	send.yqdlcclz = _wtof(stryqdlcclz.c_str());
	send.yqdlcwc = _wtof(stryqdlcwc.c_str());

	send.jcjg = &m_strjcjg;
	send.kssj = &m_strkssj;
	send.kssj = &m_strkssj;
	send.bz = &m_strbz;

	CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"lljzj", send.accessToken->c_str());

	nRet = toasb.lljzj(&send, sret);

	if (nRet == SOAP_OK)
	{
		strRetStr = sret.return_->c_str();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"lljzj", strRetStr.c_str());
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"lljzj", L"����ʧ��");
	}

	return nRet;
}
// 5.3.1OBD���ټ���������ϴ��ӿ�
//lsh	������ˮ��
//clsbm	VIN
//obdzsbz	OBDִ�б�׼
//clxslc	������ʻ���
//fdjkzdycid	���������Ƶ�Ԫ�궨ʶ����cid
//fdjkzdycvn	���������Ƶ�ԪУ׼�˲���� 
//hclkzdycid	������Ƶ�Ԫ�궨ʶ����cid
//hclkzdycvn	������Ƶ�ԪУ׼�˲���� 
//qtkzdycid	�������Ƶ�Ԫ�궨ʶ����cid
//qtkzdycvn	�������Ƶ�ԪУ׼�˲���� 
//obdgzzsq	OBDϵͳ����ָʾ��
//obdtxqk	OBDͨѶ���
//obdzzqsfdl	OBD����ָʾ���Ƿ����
//obdjxzt	����״̬δ�����Ŀ
//obdjcjg	OBD�����
//obdgzxx	OBD������Ϣ
//jyymc	����Ա����
//jckssj	��⿪ʼʱ��
//Jjcjssj	������ʱ��
int CHNSY_API:: obdjJgsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& strclsbm, const std::wstring& strobdzsbz, const std::wstring& strclxslc,
	const std::wstring& strfdjkzdycid, const std::wstring& strfdjkzdycvn, const std::wstring& strhclkzdycid, const std::wstring& strhclkzdycvn,
	const std::wstring& strqtkzdycid, const std::wstring& strqtkzdycvn, const std::wstring& strobdgzzsq, const std::wstring& strobdtxqk,
	const std::wstring& strobdzzqsfdl, const std::wstring& strobdjxzt, const std::wstring& strobdjcjg,const std::wstring& strobdgzxx,
	const std::wstring& strjyymc, const std::wstring& strjckssj, const std::wstring& strJjcjssj, std::wstring& strRetStr)
{
	int nRet;
	
	DeviceSwapIfaceImplServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	ns1__obdJgsj send;
	ns1__obdJgsjResponse sret;

	std::wstring m_strlsh = strlsh;
	std::wstring m_strclsbm = strclsbm;
	std::wstring m_strobdzsbz = strobdzsbz;
	std::wstring m_strclxslc = strclxslc;
	std::wstring m_strfdjkzdycid = strfdjkzdycid;
	std::wstring m_strfdjkzdycvn = strfdjkzdycvn;
	std::wstring m_strhclkzdycid = strhclkzdycid;
	std::wstring m_strhclkzdycvn = strhclkzdycvn;
	std::wstring m_strqtkzdycid = strqtkzdycid;
	std::wstring m_strqtkzdycvn = strqtkzdycvn;
	std::wstring m_strobdgzzsq = strobdgzzsq;
	std::wstring m_strobdtxqk = strobdtxqk;
	std::wstring m_strobdzzqsfdl = strobdzzqsfdl;
	std::wstring m_strobdjxzt = strobdjxzt;
	std::wstring m_strobdjcjg = strobdjcjg;
	std::wstring m_strobdgzxx = strobdgzxx;
	std::wstring m_strjyymc = strjyymc;
	std::wstring m_strjckssj = strjckssj;
	std::wstring m_strJjcjssj = strJjcjssj;

	send.lsh = &m_strlsh;
	send.clsbm = &m_strclsbm;
	send.obdzsbz = &m_strobdzsbz;
	send.clxslc = _wtoi(m_strclxslc.c_str());
	send.fdjkzdycid = &m_strfdjkzdycid;
	send.fdjkzdycvn = &m_strfdjkzdycvn;
	send.hclkzdycid = &m_strhclkzdycid;
	send.hclkzdycvn = &m_strhclkzdycvn;
	send.qtkzdycid = &m_strqtkzdycid;
	send.qtkzdycvn = &m_strqtkzdycvn;
	send.obdgzzsq = &m_strobdgzzsq;
	send.obdtxqk = &m_strobdtxqk;
	send.obdzzqsfdl = &m_strobdzzqsfdl;
	send.obdjxzt = &m_strobdjxzt;
	send.obdjcjg = &m_strobdjcjg;
	send.obdgzxx = &m_strobdgzxx;
	send.jyymc = &m_strjyymc;
	send.jckssj = &m_strjckssj;
	send.jcjssj = &m_strJjcjssj;

	CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"obdjJgsj", send.lsh->c_str());

	nRet = toasb.obdJgsj(&send, sret);

	if (nRet == SOAP_OK)
	{
		strRetStr = sret.return_->c_str();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"obdjJgsj", strRetStr.c_str());
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"obdjJgsj", L"����ʧ��");
	}

	return nRet;
}
// 5.3.2������OBD�������ϴ��ӿ�
//lsh	������ˮ��
//accessToken	��������
//qssx	ȫʱʱ��
//Jcxshjcsxh	���˳���
//jqmjdkd	�����ž��Կ���
//fhz	����ֵ
//ycgqxh	ǰ���������ź�
//glkqxs	��������ϵ��
//cs	����
//fdjgl	�������������
//fdjzs	������ת��
//jql	������
//jqyl	����ѹ��
//zyyl	��ѹѹ��
//ymkd	���ž��Կ���
//hyl	������
//nycgqnd	����������Ũ��
//nspsl	����������
//pqwd	�����¶�
//klbzqyc	������׽��ѹ��
//egrkd	EGR����
//rypsyl	ȼ������ѹ��
int CHNSY_API:: obdgGcsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strqssx, const std::wstring& strJcxshjcsxh,
		 const std::wstring& strjqmjdkd, const std::wstring& strfhz, const std::wstring& strycgqxh, const std::wstring& strglkqxs,
		 const std::wstring& strcs, const std::wstring& strfdjgl, const std::wstring& strfdjzs, const std::wstring& strjql,
		 const std::wstring& strjqyl, const std::wstring& strzyyl, const std::wstring& strymkd, const std::wstring strhyl, const std::wstring& strnycgqnd, const std::wstring& strnspsl,
		 const std::wstring& strpqwd, const std::wstring& strklbzqyc, const std::wstring& stregrkd, const std::wstring& strrypsyl, std::wstring& strRetStr)
{
	int nRet;

	
	DeviceSwapIfaceImplServiceSoapBindingProxy toasb;
	soap_set_mode(&toasb, SOAP_C_UTFSTRING);
	toasb.soap_endpoint = pchURL;

	ns1__obdGcsj send;
	ns1__obdGcsjResponse sret;

	std::wstring m_strlsh = strlsh;
	std::wstring m_straccessToken = straccessToken;
	std::wstring m_strqssx = strqssx;
	std::wstring m_strJcxshjcsxh = strJcxshjcsxh;

	send.accessToken = &m_straccessToken;
	send.lsh = &m_strlsh;
	send.qssx = &m_strqssx;
	send.jcsxh = &m_strJcxshjcsxh;

	send.jqmjdkd = _wtof(strjqmjdkd.c_str());
	send.fhz = _wtof(strfhz.c_str());
	send.ycgqxh = _wtof(strycgqxh.c_str());
	send.glkqxs = _wtof(strglkqxs.c_str());
	send.cs = _wtof(strcs.c_str());
	send.fdjgl = _wtof(strfdjgl.c_str());
	send.fdjzs = _wtof(strfdjzs.c_str());
	send.jql = _wtof(strjql.c_str());
	send.jqyl = _wtof(strjqyl.c_str());
	send.zyyl = _wtof(strzyyl.c_str());
	send.ymkd = _wtof(strymkd.c_str());
	send.hyl = _wtof(strhyl.c_str());
	send.nycgqnd = _wtof(strnycgqnd.c_str());
	send.nspsl = _wtof(strnspsl.c_str());
	send.pqwd = _wtof(strpqwd.c_str());
	send.klbzqyc = _wtof(strklbzqyc.c_str());
	send.egrkd = _wtof(stregrkd.c_str());
	send.rypsyl = _wtof(strrypsyl.c_str());

	CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"obdgGcsj", send.lsh->c_str());

	nRet = toasb.obdGcsj(&send, sret);

	if (nRet == SOAP_OK)
	{
		strRetStr = sret.return_->c_str();
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"obdgGcsj", strRetStr.c_str());
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath.c_str(), L"����", L"obdgGcsj", L"����ʧ��");
	}

	return nRet;
}
// 5.3.3���ͳ�OBD-IUPR�������
//lsh	������ˮ��
//chqz1jccs	�߻�����1�����ɴ���
//chqz1fhtjcs	�߻�����1������������
//chqz1iuprl	�߻�����1IUPR��
//chqz2jccs	�߻�����2�����ɴ���
//chqz2fhtjcs	�߻�����2������������
//chqz2iuprl	�߻�����2IUPR��
//qycgqz1jccs	ǰ����������1�����ɴ���
//qycgqz1fhtjcs	ǰ����������1������������
//qycgqz1iuprl	ǰ����������1 IUPR��
//qycgqz2jccs	ǰ����������2�����ɴ���
//qycgqz2fhtjcs	ǰ����������2������������
//qycgqz2iuprl	ǰ����������2 IUPR��
//hycgqz1jccs	������������1�����ɴ���
//hycgqz1fhtjcs	������������1������������
//hycgqz1iuprl	������������1 IUPR��
//hycgqz2jccs	������������2�����ɴ���
//hycgqz2fhtjcs	������������2������������
//hycgqz2iuprl	������������2 IUPR��
//evapjccs	EVAP�����ɴ���
//evapfhtjcs	EVAP������������
//evapiuprl	EVAP IUPR��
//egrvvtjccs	EGR��VVT�����ɴ���
//egrvvtfhtjcs	EGR��VVT������������
//egrvvtiuprl	EGR��VVT IUPR��
//gpfz1jccs	GPF��1�����ɴ���
//gpfz1fhtjcs	GPF��1������������
//gpfz1iuprl	GPF��1IUPR��
//gpfz2jccs	GPF��2�����ɴ���
//gpfz2fhtjcs	GPF��2������������
//gpfz2iuprl	GPF��2IUPR��
//eckqpsxtjccs	���ο�������ϵͳ�����ɴ���
//eckqpsxtfhtjcs	���ο�������ϵͳ������������
//eckqpsxtiuprl	���ο�������ϵͳIUPR��
int CHNSY_API:: qycObdIupr(const char * pchURL, const std::wstring& strlsh, const std::wstring& strchqz1jccs, const std::wstring& strchqz1fhtjcs, const std::wstring& strchqz1iuprl,
	const std::wstring& strchqz2jccs, const std::wstring& strchqz2fhtjcs, const std::wstring& strchqz2iuprl, const std::wstring& strqycgqz1jccs,
	const std::wstring& strqycgqz1fhtjcs, const std::wstring& strqycgqz1iuprl, const std::wstring& strqycgqz2jccs, const std::wstring& strqycgqz2fhtjcs,
	const std::wstring& strqycgqz2iuprl, const std::wstring& strhycgqz1jccs, const std::wstring& strhycgqz1fhtjcs,const std::wstring& strhycgqz1iuprl, const std::wstring& strhycgqz2jccs,
	const std::wstring& strhycgqz2fhtjcs, const std::wstring& strhycgqz2iuprl, const std::wstring& strevapjccs, const std::wstring& strevapfhtjcs, 
	const std::wstring& strevapiuprl, const std::wstring& stregrvvtjccs, const std::wstring& stregrvvtfhtjcs, const std::wstring& stregrvvtiuprl, 
	const std::wstring& strgpfz1jccs, const std::wstring& strgpfz1fhtjcs, const std::wstring& strgpfz1iuprl, const std::wstring& strgpfz2jccs, 
	const std::wstring& strgpfz2fhtjcs, const std::wstring& strgpfz2iuprl, const std::wstring& streckqpsxtjccs, const std::wstring& streckqpsxtfhtjcs, 
	const std::wstring& streckqpsxtiuprl, std::wstring& strRetStr)
{
	int nRet;

	return nRet;
}
// 5.3.4���ͳ�OBD-IUPR�������
//lsh	������ˮ��
//nmhcchqjccs	NMHC�߻��������ɴ���
//nmhcchqfhtjcs	NMHC�߻���������������
//nmhcchqiuprl	NMHC�߻���IUPR��
//noxchqjccs	NOx�߻��������ɴ���
//noxchqfhtjcs	NOx�߻���������������
//noxchqiuprl	NOx�߻���IUPR��
//noxxfqjccs	NOx�����������ɴ���
//noxxfqfhtjcs	NOx������������������
//noxxfqiuprl	NOx������IUPR��
//pmbjqjccs	PM�����������ɴ���
//pmbjqfhtjcs	PM������������������
//pmbjqiuprl	PM������IUPR��
//fqcgqjccs	���������������ɴ���
//fqcgqfhtjcs	����������������������
//fqcgqiuprl	����������IUPR��
//egrvvtjccs	EGR��VVT�����ɴ���
//egrvvtfhtjcs	EGR��VVT������������
//egrvvtiuprl	EGR��VVT IUPR��
//zyyljccs	��ѹѹ���������ɴ���
//zyylfhtjcs	��ѹѹ����������������
//zyyliuprl	��ѹѹ�����IUPR��
int CHNSY_API:: cycObdIupr(const char * pchURL, const std::wstring& strlsh, const std::wstring& strnmhcchqjccs, const std::wstring& strnmhcchqfhtjcs, const std::wstring& strnmhcchqiuprl,
	const std::wstring& strnoxchqjccs, const std::wstring& strnoxchqfhtjcs, const std::wstring& strnoxchqiuprl, const std::wstring& strnoxxfqjccs,
	const std::wstring& strnoxxfqfhtjcs, const std::wstring& strnoxxfqiuprl, const std::wstring& strpmbjqjccs, const std::wstring& strpmbjqfhtjcs,
	const std::wstring& strpmbjqiuprl, const std::wstring& strfqcgqjccs, const std::wstring& strfqcgqfhtjcs,const std::wstring& strfqcgqiuprl, const std::wstring& stregrvvtjccs,
	const std::wstring& stregrvvtfhtjcs, const std::wstring& stregrvvtiuprl, const std::wstring& strzyyljccs, const std::wstring& strzyylfhtjcs, 
	const std::wstring& strzyyliuprl, std::wstring& strRetStr)
{
	int nRet;

	return nRet;
}
// 5.3.5ȼ�������ŷſ���ϵͳ���������ϴ��ӿ�
//lsh	������ˮ��
//accessToken	��������
//jykcsyl	���Ϳڳ�ʼѹ��
//jykssyl	���Ϳ���ʧѹ��
//Jjykcsjg	���Ϳڲ��Խ��
//yxgxlll	�����й¶����
//yxgcsyl	����ǳ�ʼѹ��
//yxgssyl	�����10����ʧѹ��
//yxgcsjg	����ǲ��Խ��
//kssj	��⿪ʼʱ��
//jssj	������ʱ��
int CHNSY_API:: ryzfJysj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strjykcsyl, const std::wstring& strjykssyl,
	const std::wstring& strjykcsjg, const std::wstring& stryxgxlll, const std::wstring& stryxgcsyl, const std::wstring& stryxgssyl,
	const std::wstring& stryxgcsjg, const std::wstring& strkssj, const std::wstring& strjssj, std::wstring& strRetStr)
{
	int nRet;

	return nRet;
}
// 5.4.1�ϴ�����˲̬������������ӿ�
//lsh	������ˮ��
//accessToken	������������
//co	һ����̼���ֵ
//hc	̼�⻯������ֵ
//no	������������ֵ
//sdgl	�⹦���趨����
//sjxsjl	����ʵ����ʻ����
//cssj	����ʱ��
//plcs	���Թ���ƫ�����
//zplsj	�ܼ�ƫ��ʱ��
//wd	�����¶�
//sd	����ʪ��
//dqy	��������ѹ
//ynd	������Ũ��
//glkqxs	��������ϵ��
//kssj	��⿪ʼʱ��
//jssj	������ʱ��
int CHNSY_API:: jystJgsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strco, const std::wstring& strhc,
	const std::wstring& strno, const std::wstring& strsdgl, const std::wstring& strsjxsjl, const std::wstring& strcssj,
	const std::wstring& strplcs, const std::wstring& strzplsj, const std::wstring& strwd, const std::wstring& strsd,
	const std::wstring& strdqy, const std::wstring& strynd, const std::wstring& strglkqxs,
	const std::wstring& stryxgcsjg, const std::wstring& strkssj, const std::wstring& strjssj, std::wstring& strRetStr)
{
	int nRet;

	return nRet;
}
// 5.4.2�ϴ�����˲̬���������������ݽӿ�
//lsh	������ˮ��
//accessToken	������������
//qssx	ȫ��ʱ��
//gklx	��������
//jcsxh	���˳���
//wxzhc	HCŨ��ÿ������(δ����)
//hc	HCŨ��ÿ������(ϡ��������)
//wxzco	COŨ��ÿ������(δ����)
//co	COŨ��ÿ������(ϡ��������)
//co2	CO2Ũ��ÿ������
//wxznox	NOxŨ��ÿ������(δ����)
//nox	NOxŨ��ÿ������(ϡ��������)
//o2	O2Ũ��ÿ������
//sjll	ʵ������ÿ������
//bzll	��׼����ÿ������
//wqll	����β������ÿ������
//cs	����ÿ������
//bzss	��׼ʱ��ÿ������
//fdjzs	������ת��ÿ������
//glkqxs	��������ϵ��
//xsxzxs	ϡ��ϵ��
//nosdxzxs	NOʪ������ϵ��
//xsb	ϡ�ͱ�ÿ������
//jzgl	���Թ������ع���ÿ������
//jsgl	���Թ�����������ÿ������
//zsgl	���Թ���ָʾ����ÿ������
//lljqy	���Թ�����������ѹ��ÿ������
//lljwd	���Թ����������¶�ÿ������
//nl	Ť��ÿ������
//fxyglyl	�����ǹ�·ѹ��
//scfz	ʵ����̲⹦������
//wd	�����¶�ÿ������
//sd	���Թ����������ʪ��ÿ������
//dqy	���Թ�����������ѹ��ÿ������
int CHNSY_API:: jystGcsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strqssx, const std::wstring& strgklx,
	const std::wstring& strjcsxh, const std::wstring& strwxzhc, const std::wstring& strhc, const std::wstring& strwxzco, const std::wstring& strco,
	const std::wstring& strco2, const std::wstring& strwxznox, const std::wstring& strnox, const std::wstring& stro2, const std::wstring& strsjll,
	const std::wstring& strbzll, const std::wstring& strwqll, const std::wstring& strcs, const std::wstring& strbzss, const std::wstring& strfdjzs,
	const std::wstring& strglkqxs, const std::wstring& strxsxzxs, const std::wstring& strnosdxzxs, const std::wstring& strxsb, const std::wstring& strjzgl,
	const std::wstring& strjsgl, const std::wstring& strzsgl, const std::wstring& strlljqy, const std::wstring& strlljwd, const std::wstring& strnl,
	const std::wstring& strfxyglyl, const std::wstring& strscfz, const std::wstring& strwd, const std::wstring& strsd, const std::wstring& strdqy,
	std::wstring& strRetStr)
{
	int nRet;

	return nRet;
}
// 5.4.3�ϴ���̬������������ӿ�
//lsh	������ˮ��
//accessToken	������������
//hc5025	5025����hc���ֵ
//co5025	5025����co ���ֵ
//no5025	5025����no ���ֵ
//fdjzs5025	5025����������ת��
//fdjyw5025	5025����������
//pjcs5025	5025ƽ������
//zgl5025	5025�׶ε��̲⹦�������ص��ܹ���
//hc2540	2540����hc���ֵ
//co2540	2540����co ���ֵ
//no2540	2540����no ���ֵ
//fdjzs2540	2540����������ת��
//fdjyw2540	2540����������
//pjcs2540	2540ƽ������
//zgl2540	2540�׶ε��̲⹦�������ص��ܹ���
//glkqxs	��������ϵ��
//wd	�¶�
//sd	ʪ��
//dqy	��ѹ
//kssj	��⿪ʼʱ��
//jssj	������ʱ��
int CHNSY_API:: wtJgsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strhc5025, const std::wstring& strco5025,
	const std::wstring& strno5025, const std::wstring& strfdjzs5025, const std::wstring& strfdjyw5025, const std::wstring& strpjcs5025,
	const std::wstring& strzgl5025, const std::wstring& strhc2540, const std::wstring& strco2540, const std::wstring& strno2540,
	const std::wstring& strfdjzs2540, const std::wstring& strfdjyw2540, const std::wstring& strpjcs2540, const std::wstring& strzgl2540,
	const std::wstring& strglkqxs, const std::wstring& strwd, const std::wstring& strsd, const std::wstring& strdqy, 
	const std::wstring& strkssj, const std::wstring& strjssj, std::wstring& strRetStr)
{
	int nRet;

	return nRet;
}
// 5.4.4�ϴ���̬���������������ݽӿ�
//lsh	������ˮ��
//accessToken	������������
//qssx	ȫʱʱ��
//gklx	��������
//jcsxh	���˳���
//wxzhc	HC����ֵÿ������(δ����)
//wxzco	CO����ֵÿ������(δ����)
//wxzno	NO����ֵÿ������(δ����)
//hc	HC����ֵÿ������
//co	CO����ֵÿ������
//no	NO����ֵÿ������
//o2	O2����ֵÿ������
//co2	CO2����ֵÿ������
//cs	����ÿ������
//zs	������ת��ÿ������
//xsxzxs	ϡ������ϵ��ÿ������
//sdxzxs	ʪ������ϵ��ÿ������
//glkqxs	��������ϵ��
//dpcgjfz	������̲⹦������
//zjzgl	���Թ����ܼ��ع���ÿ������
//jsgl	���Թ�����������ÿ������
//zsgl	���Թ���ָʾ����ÿ������
//nl	Ť��ÿ������
//fxyglyl	�����ǹ�·ѹ��
//wd	���Թ��������¶�ÿ������
//sd	���Թ����������ʪ��ÿ������
//dqy	���Թ�����������ѹ��ÿ������
//yw	����ÿ������
int CHNSY_API:: wtGcsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strqssx, const std::wstring& strgklx,
	const std::wstring& strjcsxh, const std::wstring& strwxzhc, const std::wstring& strwxzco, const std::wstring& strwxzno, const std::wstring& strhc,
	const std::wstring& strco, const std::wstring& strno, const std::wstring& strsdxzxs, const std::wstring& strglkqxs, const std::wstring& strdpcgjfz,
	const std::wstring& strzjzgl, const std::wstring& strjsgl, const std::wstring& strzsgl, const std::wstring& strnl, const std::wstring& strfxyglyl,
	const std::wstring& strwd, const std::wstring& strsd, const std::wstring& strdqy, const std::wstring& stryw,
	std::wstring& strRetStr)
{
	int nRet;

	return nRet;
}
// 5.4.5�ϴ�˫���ٷ�������ӿ�
//lsh	������ˮ��
//accessToken	��������
//dszs	���ٷ�����ת��
//dsyw	��������
//gdszs	�ߵ��ٷ�����ת��
//gdsyw	�ߵ�������
//dsco	�͵���COֵ
//dshc	�͵��� HCֵ
//gdsco	�ߵ���COֵ
//gdshc	�ߵ��� HCֵ
//glkqxs	��������ϵ��
//wd	�¶�
//sd	ʪ��
//dqy	��ѹ
//kssj	��⿪ʼʱ��
//jssj	������ʱ��
int CHNSY_API:: sdsJgsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strdszs, const std::wstring& strdsyw,
	const std::wstring& strgdszs, const std::wstring& strgdsyw, const std::wstring& strdsco, const std::wstring& strdshc,
	const std::wstring& strgdsco, const std::wstring& strgdshc, const std::wstring& strglkqxs, const std::wstring& strwd,
	const std::wstring& strsd, const std::wstring& strdqy,
	const std::wstring& strkssj, const std::wstring& strjssj, std::wstring& strRetStr)
{
	int nRet;

	return nRet;
}
// 5.4.6�ϴ�˫���ٷ����������ݽӿ�
//lsh	������ˮ��
//accessToken	��������
//qssx	ȫ��ʱ��
//gklx	��������
//jcsxh	���˳���
//hc	���Թ���[HC]ÿ������
//co	���Թ���[CO]ÿ������
//co2	���Թ���[CO2]ÿ������
//o2	���Թ���������[O2]ÿ������
//glkqxs	��������ϵ�����ˣ�
//jyyw	��������
//fdjzs	������ת��ÿ�����ݻ�������������ʽ
//wd	�����¶�ÿ������
//sd	�������ʪ��ÿ������
//dqy	��������ѹ��ÿ������
int CHNSY_API:: sdsGcsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strqssx, const std::wstring& strgklx,
	const std::wstring& strjcsxh, const std::wstring& strhc, const std::wstring& strco, const std::wstring& strco2,
	const std::wstring& stro2, const std::wstring& strglkqxs, const std::wstring& strjyyw, const std::wstring& strfdjzs,
	const std::wstring& strwd, const std::wstring& srtsd, const std::wstring& strdqy, std::wstring& strRetStr)
{
	int nRet;

	return nRet;
}
// 5.4.7�ϴ����ؼ��ٷ�������ӿ�
//lsh	������ˮ��
//accessToken	��������
//k100	100%���̶�ֵ
//k80	80%���̶�ֵ
//nox80	80%�㵪��ֵ
//zdlbgl	����ֱ߹���
//zdsd	����ֱ߹��ʶ�Ӧ���ٶ�
//fdjzs	����ֱ߹��ʶ�Ӧ�ķ�����ת��
//dszs	����������ת��
//fdjzssx	�������ת������
//fdjzsxx	�������ת������
//wd	�¶�
//sd	ʪ��
//dqy	��ѹ
//kssj	��⿪ʼʱ��
//jssj	������ʱ��
int CHNSY_API:: jzjsJgsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strk100, const std::wstring& strk80,
	const std::wstring& strnox80, const std::wstring& strzdlbgl, const std::wstring& strzdsd, const std::wstring& strfdjzs,
	const std::wstring& strdszs, const std::wstring& strfdjzssx, const std::wstring& strfdjzsxx, const std::wstring& strwd,
	const std::wstring& strsd, const std::wstring& strdqy,
	const std::wstring& strkssj, const std::wstring& strjssj, std::wstring& strRetStr)
{
	int nRet;

	return nRet;
}
// 5.4.8�ϴ����ؼ��ٷ����������ݽӿ�
//lsh	������ˮ��
//accessToken	��������
//qssx	ȫ��ʱ��
//gklx	��������
//jcsxh	���˳���
//gxsxs	������ϵ��ÿ������
//ydz	��͸���
//sscs	ʵʱ����ÿ������
//fdjzs	������ת��ÿ������
//nl	�⹦��Ť��
//nj	Ť��
//scgl	�⹦�����ɣ�ʵ�⹦�ʣ�
//glxzxs	��������ϵ��
//xzhgl	��������
//nox	���뵪������Ũ��
//co2	����CO2Ũ��
//wd	�����¶�ÿ������
//sd	�������ʪ��ÿ������
//dqy	��������ѹ��ÿ������
int CHNSY_API:: jzjsGcsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strqssx, const std::wstring& strgklx,
	const std::wstring& strjcsxh, const std::wstring& strgxsxs, const std::wstring& strydz, const std::wstring& strsscs,
	const std::wstring& strglxzxs, const std::wstring& strxzhgl, const std::wstring& strnox, const std::wstring& strco2,
	const std::wstring& strwd, const std::wstring& strsd, const std::wstring& strdqy, std::wstring& strRetStr)
{
	int nRet;

	return nRet;
}
// 5.4.9�ϴ���͸���̶ȷ�������ӿ�
//lsh	������ˮ��
//accessToken	��������
//ydz1	��һ�� �̶�ֵ
//zs1	��һ�� ת��
//ydz2	�ڶ��� �̶�ֵ
//zs2	�ڶ��� ת��
//ydz3	������ �̶�ֵ
//zs3	������ ת��
//dszs	����ת��
//wd	�¶�
//sd	ʪ��
//dqy	��ѹ
//kssj	��⿪ʼʱ��
//jssj	������ʱ��
int CHNSY_API:: btgJgsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strydz1, const std::wstring& strzs1,
	const std::wstring& strydz2, const std::wstring& strzs2, const std::wstring& strydz3, const std::wstring& strzs3,
	const std::wstring& strdszs, const std::wstring& strwd, const std::wstring& strsd, const std::wstring& strdqy,
	const std::wstring& strkssj, const std::wstring& strjssj, std::wstring& strRetStr)
{
	int nRet;

	return nRet;
}
// 5.4.10�ϴ���͸���̶ȷ����������ݽӿ�
//lsh	������ˮ��
//accessToken	��������
//qssx	ȫ��ʱ��
//gklx	��������
//jcsxh	���˳���
//ydz	�̶�ֵ
//fdjzs	������ת������
//yw	������������
//wd	�����¶�ÿ������
//sd	�������ʪ��ÿ������
//dqy	��������ѹ��ÿ������
int CHNSY_API:: btgGcsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strqssx, const std::wstring& strgklx,
	const std::wstring& strjcsxh, const std::wstring& strydz, const std::wstring& strfdjzs, const std::wstring& stryw,
	const std::wstring& strwd, const std::wstring& strsd, const std::wstring& strdqy, std::wstring& strRetStr)
{
	int nRet;

	return nRet;
}
// 5.4.11�ϴ��ָ����ڶȷ�������ӿ�
//lsh	������ˮ��
//accessToken	��������
//lgmhd	�ָ����ڶ�
//fdjzs	�ڶ����ֵ��Ӧ�ķ�����ת��
//wd	�¶�
//sd	ʪ��
//dqy	��ѹ
//kssj	��⿪ʼʱ��
//jssj	������ʱ��
int CHNSY_API:: lgmhdJgsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strlgmhd, const std::wstring& strfdjzs,
	const std::wstring& strwd, const std::wstring& strsd, const std::wstring& strdqy, const std::wstring& strkssj, const std::wstring& strjssj, std::wstring& strRetStr)
{
	int nRet;

	return nRet;
}
// 5.4.12�ϴ��ָ����ڶȷ����������ݽӿ�
//lsh	������ˮ��
//accessToken	��������
//qssx	ȫ��ʱ��
//jcsxh	���˳���
//lgmhd	�ָ����ڶ�ֵ
//fdjzs	������ת������
//wd	�����¶�ÿ������
//sd	�������ʪ��ÿ������
//dqy	��������ѹ��ÿ������
int CHNSY_API:: lgmhdGcsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strqssx, const std::wstring& strjcsxh,
	const std::wstring& strlgmhd, const std::wstring& strfdjzs,
	const std::wstring& strwd, const std::wstring& strsd, const std::wstring& strdqy, std::wstring& strRetStr)
{
	int nRet;

	return nRet;
}