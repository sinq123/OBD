#pragma once

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#include <memory>
#include <string>

class CHNSY_API
{

public:
	CHNSY_API(void);
	virtual ~CHNSY_API(void);

private:
	CHNSY_API(const CHNSY_API &);
	CHNSY_API & operator =(const CHNSY_API &);


public:
	// 5.1.1��ȡ�������ƽӿ� getAccessToken
	static int getAccessToken(const char * pchURL, const std::wstring& strjcxbh, std::wstring& strRetStr);
	// 5.1.2��ȡ���쳵���ӿ� 
	static int getDjzt(const char * pchURL, const std::wstring& straccessToken, std::wstring& strRetStr);
	// 5.1.3��ȡ������Ϣ�ӿ� 
	static int getDjclxx(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, std::wstring& strRetStr);
	// 5.1.4������Ϣ֪ͨ�ӿ�
	static int getDjclxx(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strcode,
		const std::wstring& strmessage, std::wstring& strRetStr);
	// 5.1.5�ϴ���������汾�Žӿ�
	static int gkrjbbh(const char * pchURL, const std::wstring& straccessToken, const std::wstring& strbbh, std::wstring& strRetStr);
	// 5.1.6������ʱ��ͬ���ӿ�
	static int fwqsjtb(const char * pchURL, std::wstring& strRetStr);
	// 5.1.7���վ�����Ա�б�ӿ�
	static int jcrylb(const char * pchURL, const std::wstring& strjczbh, const std::wstring& strjsdm, std::wstring& strRetStr);
	// 5.1.8OBD�����б�ӿ�
	static int obdDjlb(const char * pchURL, const std::wstring& strjczbh, const std::wstring& strcphm, std::wstring& strRetStr);
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
	static int cgjzj(const char * pchURL, const std::wstring& straccessToken, const std::wstring& strgsjzllsj, const std::wstring& strgsjzsjsj,
		 const std::wstring& strdsjzllsj, const std::wstring& strdsjzsjsj, const std::wstring& strgsjzns, const std::wstring& strdsjzns,
		 const std::wstring& strgsjzhxgl, const std::wstring& strdsjzhxgl, const std::wstring& strjbgl, const std::wstring& strgsjzjcjg,
		 const std::wstring& strdsjzjcjg, const std::wstring& strjcjg, const std::wstring& strkssj, const std::wstring& strjssj,
		 const std::wstring& strbz, std::wstring& strRetStr);
	// 5.2.2����������й¶���ӿ�
	//accessToken	��������
	//jcjg	�ܷ��Լ����
	//kssj	��鿪ʼʱ��
	//jssj	������ʱ��
	//bz	��ע
	static int wqfxyzj(const char * pchURL, const std::wstring& straccessToken, const std::wstring& strjcjg, const std::wstring& strkssj,
		 const std::wstring& strjssj, const std::wstring& strbz, std::wstring& strRetStr);
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
	static int ydjzj(const char * pchURL, const std::wstring& straccessToken, const std::wstring& strldjcjg, const std::wstring& strlgpz1,
		 const std::wstring& strlgpz2, const std::wstring& strjcz1, const std::wstring& strjcz2, const std::wstring& strwcl1,
		 const std::wstring& strwcl2, const std::wstring& strjcjg,
		 const std::wstring& kssj, const std::wstring& jssj, const std::wstring& strbz, std::wstring& strRetStr);
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
	static int hjcsyzj(const char * pchURL, const std::wstring& straccessToken, const std::wstring& strsjhjwd, const std::wstring& strclhjwd,
		 const std::wstring& strsjhjsd, const std::wstring& strclhjsd, const std::wstring& strsjdqyl, const std::wstring& strcldqyl,
		 const std::wstring& kssj, const std::wstring& jssj, const std::wstring& strbz, std::wstring& strRetStr);
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
	static int wqfxybd(const char * pchURL, const std::wstring& straccessToken, const std::wstring& strbqlb, const std::wstring& strbzCO2,
		 const std::wstring& strsjCO2, const std::wstring& strbzCO, const std::wstring& strsjCO, const std::wstring& strbzNO,
		 const std::wstring& strsjNO, const std::wstring& strbzHC, const std::wstring& strsjHC, const std::wstring& strbzO2,
		 const std::wstring& strsjO2, const std::wstring& strsjPEF, const std::wstring& strbzC3H8, const std::wstring& strjcjg,
		 const std::wstring& kssj, const std::wstring& jssj, const std::wstring& strbz, std::wstring& strRetStr);
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
	static int lljzj(const char * pchURL, const std::wstring& straccessToken, const std::wstring& strjqglljcjg, const std::wstring& strlljylcjcjg,
		 const std::wstring& stryqglcbz, const std::wstring& stryqglcclz, const std::wstring& stryqglcwc, const std::wstring& stryqdlcbz,
		 const std::wstring& stryqdlcclz, const std::wstring& stryqdlcwc, const std::wstring& strjcjg,
		 const std::wstring& kssj, const std::wstring& jssj, const std::wstring& strbz, std::wstring& strRetStr);
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
	static int obdjJgsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& strclsbm, const std::wstring& strobdzsbz, const std::wstring& strclxslc,
		 const std::wstring& strfdjkzdycid, const std::wstring& strfdjkzdycvn, const std::wstring& strhclkzdycid, const std::wstring& strhclkzdycvn,
		 const std::wstring& strqtkzdycid, const std::wstring& strqtkzdycvn, const std::wstring& strobdgzzsq, const std::wstring& strobdtxqk,
		 const std::wstring& strobdzzqsfdl, const std::wstring& strobdjxzt, const std::wstring& strobdjcjg,const std::wstring& strobdgzxx,
		 const std::wstring& strjyymc, const std::wstring& strjckssj, const std::wstring& strJjcjssj, std::wstring& strRetStr);
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
	static int obdgGcsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strqssx, const std::wstring& strJcxshjcsxh,
		 const std::wstring& strjqmjdkd, const std::wstring& strfhz, const std::wstring& strycgqxh, const std::wstring& strglkqxs,
		 const std::wstring& strcs, const std::wstring& strfdjgl, const std::wstring& strfdjzs, const std::wstring& strjql,
		 const std::wstring& strjqyl, const std::wstring& strzyyl, const std::wstring& strymkd,const std::wstring& strnycgqnd, const std::wstring& strnspsl,
		 const std::wstring& strpqwd, const std::wstring& strklbzqyc, const std::wstring& stregrkd, const std::wstring& strrypsyl, std::wstring& strRetStr);
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
	static int qycObdIupr(const char * pchURL, const std::wstring& strlsh, const std::wstring& strchqz1jccs, const std::wstring& strchqz1fhtjcs, const std::wstring& strchqz1iuprl,
		 const std::wstring& strchqz2jccs, const std::wstring& strchqz2fhtjcs, const std::wstring& strchqz2iuprl, const std::wstring& strqycgqz1jccs,
		 const std::wstring& strqycgqz1fhtjcs, const std::wstring& strqycgqz1iuprl, const std::wstring& strqycgqz2jccs, const std::wstring& strqycgqz2fhtjcs,
		 const std::wstring& strqycgqz2iuprl, const std::wstring& strhycgqz1jccs, const std::wstring& strhycgqz1fhtjcs,const std::wstring& strhycgqz1iuprl, const std::wstring& strhycgqz2jccs,
		 const std::wstring& strhycgqz2fhtjcs, const std::wstring& strhycgqz2iuprl, const std::wstring& strevapjccs, const std::wstring& strevapfhtjcs, 
		 const std::wstring& strevapiuprl, const std::wstring& stregrvvtjccs, const std::wstring& stregrvvtfhtjcs, const std::wstring& stregrvvtiuprl, 
		 const std::wstring& strgpfz1jccs, const std::wstring& strgpfz1fhtjcs, const std::wstring& strgpfz1iuprl, const std::wstring& strgpfz2jccs, 
		 const std::wstring& strgpfz2fhtjcs, const std::wstring& strgpfz2iuprl, const std::wstring& streckqpsxtjccs, const std::wstring& streckqpsxtfhtjcs, 
		 const std::wstring& streckqpsxtiuprl, std::wstring& strRetStr);
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
	static int cycObdIupr(const char * pchURL, const std::wstring& strlsh, const std::wstring& strnmhcchqjccs, const std::wstring& strnmhcchqfhtjcs, const std::wstring& strnmhcchqiuprl,
		 const std::wstring& strnoxchqjccs, const std::wstring& strnoxchqfhtjcs, const std::wstring& strnoxchqiuprl, const std::wstring& strnoxxfqjccs,
		 const std::wstring& strnoxxfqfhtjcs, const std::wstring& strnoxxfqiuprl, const std::wstring& strpmbjqjccs, const std::wstring& strpmbjqfhtjcs,
		 const std::wstring& strpmbjqiuprl, const std::wstring& strfqcgqjccs, const std::wstring& strfqcgqfhtjcs,const std::wstring& strfqcgqiuprl, const std::wstring& stregrvvtjccs,
		 const std::wstring& stregrvvtfhtjcs, const std::wstring& stregrvvtiuprl, const std::wstring& strzyyljccs, const std::wstring& strzyylfhtjcs, 
		 const std::wstring& strzyyliuprl, std::wstring& strRetStr);
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
	static int ryzfJysj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strjykcsyl, const std::wstring& strjykssyl,
		 const std::wstring& strjykcsjg, const std::wstring& stryxgxlll, const std::wstring& stryxgcsyl, const std::wstring& stryxgssyl,
		 const std::wstring& stryxgcsjg, const std::wstring& strkssj, const std::wstring& strjssj, std::wstring& strRetStr);
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
	static int jystJgsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strco, const std::wstring& strhc,
		 const std::wstring& strno, const std::wstring& strsdgl, const std::wstring& strsjxsjl, const std::wstring& strcssj,
		 const std::wstring& strplcs, const std::wstring& strzplsj, const std::wstring& strwd, const std::wstring& strsd,
		 const std::wstring& strdqy, const std::wstring& strynd, const std::wstring& strglkqxs,
		 const std::wstring& stryxgcsjg, const std::wstring& strkssj, const std::wstring& strjssj, std::wstring& strRetStr);
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
	static int jystGcsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strqssx, const std::wstring& strgklx,
		 const std::wstring& strjcsxh, const std::wstring& strwxzhc, const std::wstring& strhc, const std::wstring& strwxzco, const std::wstring& strco,
		 const std::wstring& strco2, const std::wstring& strwxznox, const std::wstring& strnox, const std::wstring& stro2, const std::wstring& strsjll,
		 const std::wstring& strbzll, const std::wstring& strwqll, const std::wstring& strcs, const std::wstring& strbzss, const std::wstring& strfdjzs,
		 const std::wstring& strglkqxs, const std::wstring& strxsxzxs, const std::wstring& strnosdxzxs, const std::wstring& strxsb, const std::wstring& strjzgl,
		 const std::wstring& strjsgl, const std::wstring& strzsgl, const std::wstring& strlljqy, const std::wstring& strlljwd, const std::wstring& strnl,
		 const std::wstring& strfxyglyl, const std::wstring& strscfz, const std::wstring& strwd, const std::wstring& strsd, const std::wstring& strdqy,
		 std::wstring& strRetStr);
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
	static int wtJgsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strhc5025, const std::wstring& strco5025,
		 const std::wstring& strno5025, const std::wstring& strfdjzs5025, const std::wstring& strfdjyw5025, const std::wstring& strpjcs5025,
		 const std::wstring& strzgl5025, const std::wstring& strhc2540, const std::wstring& strco2540, const std::wstring& strno2540,
		 const std::wstring& strfdjzs2540, const std::wstring& strfdjyw2540, const std::wstring& strpjcs2540, const std::wstring& strzgl2540,
		 const std::wstring& strglkqxs, const std::wstring& strwd, const std::wstring& strsd, const std::wstring& strdqy, 
		 const std::wstring& strkssj, const std::wstring& strjssj, std::wstring& strRetStr);
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
	static int wtGcsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strqssx, const std::wstring& strgklx,
		 const std::wstring& strjcsxh, const std::wstring& strwxzhc, const std::wstring& strwxzco, const std::wstring& strwxzno, const std::wstring& strhc,
		 const std::wstring& strco, const std::wstring& strno, const std::wstring& strsdxzxs, const std::wstring& strglkqxs, const std::wstring& strdpcgjfz,
		 const std::wstring& strzjzgl, const std::wstring& strjsgl, const std::wstring& strzsgl, const std::wstring& strnl, const std::wstring& strfxyglyl,
		 const std::wstring& strwd, const std::wstring& strsd, const std::wstring& strdqy, const std::wstring& stryw,
		 std::wstring& strRetStr);
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
	static int sdsJgsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strdszs, const std::wstring& strdsyw,
		 const std::wstring& strgdszs, const std::wstring& strgdsyw, const std::wstring& strdsco, const std::wstring& strdshc,
		 const std::wstring& strgdsco, const std::wstring& strgdshc, const std::wstring& strglkqxs, const std::wstring& strwd,
		 const std::wstring& strsd, const std::wstring& strdqy,
		 const std::wstring& strkssj, const std::wstring& strjssj, std::wstring& strRetStr);
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
	static int sdsGcsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strqssx, const std::wstring& strgklx,
		 const std::wstring& strjcsxh, const std::wstring& strhc, const std::wstring& strco, const std::wstring& strco2,
		 const std::wstring& stro2, const std::wstring& strglkqxs, const std::wstring& strjyyw, const std::wstring& strfdjzs,
		 const std::wstring& strwd, const std::wstring& srtsd, const std::wstring& strdqy, std::wstring& strRetStr);
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
	static int jzjsJgsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strk100, const std::wstring& strk80,
		 const std::wstring& strnox80, const std::wstring& strzdlbgl, const std::wstring& strzdsd, const std::wstring& strfdjzs,
		 const std::wstring& strdszs, const std::wstring& strfdjzssx, const std::wstring& strfdjzsxx, const std::wstring& strwd,
		 const std::wstring& strsd, const std::wstring& strdqy,
		 const std::wstring& strkssj, const std::wstring& strjssj, std::wstring& strRetStr);
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
	static int jzjsGcsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strqssx, const std::wstring& strgklx,
		 const std::wstring& strjcsxh, const std::wstring& strgxsxs, const std::wstring& strydz, const std::wstring& strsscs,
		 const std::wstring& strglxzxs, const std::wstring& strxzhgl, const std::wstring& strnox, const std::wstring& strco2,
		 const std::wstring& strwd, const std::wstring& strsd, const std::wstring& strdqy, std::wstring& strRetStr);
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
	static int btgJgsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strydz1, const std::wstring& strzs1,
		 const std::wstring& strydz2, const std::wstring& strzs2, const std::wstring& strydz3, const std::wstring& strzs3,
		 const std::wstring& strdszs, const std::wstring& strwd, const std::wstring& strsd, const std::wstring& strdqy,
		 const std::wstring& strkssj, const std::wstring& strjssj, std::wstring& strRetStr);
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
	static int btgGcsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strqssx, const std::wstring& strgklx,
		 const std::wstring& strjcsxh, const std::wstring& strydz, const std::wstring& strfdjzs, const std::wstring& stryw,
		 const std::wstring& strwd, const std::wstring& strsd, const std::wstring& strdqy, std::wstring& strRetStr);
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
	static int lgmhdJgsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strlgmhd, const std::wstring& strfdjzs,
		 const std::wstring& strwd, const std::wstring& strsd, const std::wstring& strdqy, const std::wstring& strkssj, const std::wstring& strjssj, std::wstring& strRetStr);
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
	static int lgmhdGcsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strqssx, const std::wstring& strjcsxh,
		 const std::wstring& strlgmhd, const std::wstring& strfdjzs,
		 const std::wstring& strwd, const std::wstring& strsd, const std::wstring& strdqy, std::wstring& strRetStr);
};