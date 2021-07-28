#ifndef INTINFO_H
#define INTINFO_H

#include <string>

// ���������Ϣ Waiting queue information
struct SWaitQueInfo
{
	//1.tsno varchar2(15) ���վ��� 
	std::wstring strtsno;
	//2 jylsh varchar2(50) ������ˮ�� 
	std::wstring strjylsh;
	//3 testtimes number(3) ������� 1,2,3 
	std::wstring strtesttimes;
	//4 license varchar2(10) ���ƺ��� 
	std::wstring strlicense;
	//5 licensecode varchar2(2) �������� �� GA 24.7 
	std::wstring strlicensecode;
	//6 testtype varchar2(1) ��ⷽ�� ��¼ 3.1 
	std::wstring strtesttype;
	//7 dlsj date ��¼ʱ�� yyyy-mm-dd 24h:mi:ss 
	std::wstring strdlsj;
	//8 zt char(1) ״̬ 0 ����ۼ�⡢1 �� OBD ��⡢2 �����߼�⡢3 ���߼���С�4 �� ȼ���������
	std::wstring strzt;
};

// ������Ϣ
struct SIntVehInfo
{
	//1 jylsh varchar2(50) ������ˮ�� ���վ���(8 λ)+������ (yymmdd 6 λ)+�����ۼ��� ˮ(5 λ) 
	std::wstring strjylsh;
	//	2 testtimes number(3) ������� �����������ڼ���������� ��Ϊ 0 
	std::wstring strtesttimes;
	//	3 license varchar2(20) ���ƺ��� ³ B19928������λ���֣� ��������������ĩλ���� 
	std::wstring strlicense;
	//	4 licensetype number(1) ������ɫ ��¼ 3.4 
	std::wstring strlicensetype;
	//	5 licensecode varchar2(2) �������� �� GA 24.7 
	std::wstring strlicensecode;
	//	6 vehicletype varchar2(3) �������� �� GA24.4-2005 
	std::wstring strvehicletype;
	//	7 vehiclemodel varchar2(30) �����ͺ� 
	std::wstring strvehiclemodel;
	//	8 cpmc varchar2(30) ��������/�̱� �磺ɣ���� 
	std::wstring strcpmc;
	//	9 vin varchar2(30) ����ʶ����� ��д VIN ����߳��ܺ� 
	std::wstring strvin;
	//	10 clscqy varchar2(50) ���������� 
	std::wstring strclscqy;
	//	11 engine varchar2(30) �������ͺ� 
	std::wstring strengine;
	//	12 enginemanuf varchar2(50) ������������ 
	std::wstring strenginemanuf;
	//	13 fdjh varchar2(20) ��������� 
	std::wstring strfdjh;
	//	14 ed number(5,3) ���������� ��(L) 
	std::wstring stred;
	//	15 enginespeed number(4) �������ת�� תÿ��(r/min) 
	std::wstring strenginespeed;
	//	16 enginepower number(4) ����������� ǧ��(kw)
	std::wstring strenginepower;
	//	17 usetype varchar2(3) ʹ������ GA24.3 
	std::wstring strusetype;
	//	18 registerdate date ע������/���εǼ��� �� ����YYYYMMDD����ʽ��д (yyyy-mm-dd) 
	std::wstring strregisterdate;
	//	19 mdate date ��������/�������� ����YYYYMMDD����ʽ��д (yyyy-mm-dd) 
	std::wstring strmdate;
	//	20 owner varchar2(150) ��������/��λ���� 
	std::wstring strowner;
	//	21 lxdh varchar2(15) �����绰 
	std::wstring strlxdh;
	//	22 lxdz varchar2(150) ������ַ 
	std::wstring strlxdz;
	//	23 standard number(1) �ŷű�׼�׶� ��¼ 3.2 
	std::wstring strstandard;
	//	24 gear number(1) ��������ʽ ��¼ 3.8 
	std::wstring strgear;
	//	25 airin number(1) ������ʽ 1 ��Ȼ���� 2 ������ѹ 
	std::wstring strairin;
	//	26 fueltype varchar2(3) ȼ������ �ɶ�ѡ��ÿ�ְ� GA24.9 
	std::wstring strfueltype;
	//	27 fuelsupply number(2) ȼ�Ϲ���ϵͳ��ʽ ��¼ 3.9 
	std::wstring strfuelsupply;
	//	28 drivemode varchar2(1) ������ʽ 1 ǰ�� 2 ���� 3 ���� 4 ȫʱ���� 
	std::wstring strdrivemode;
	//	29 gvm number(6) ��������� ǧ�ˣ�kg�� 
	std::wstring strgvm;
	//	30 rm number(6) ��׼���� ǧ�ˣ�kg�� 
	std::wstring strrm;
	//	31 zbzl number(8) �������� ��λ kg 
	std::wstring strzbzl;
	//	32 egr varchar2(1) �Ƿ��� EGR Y/N ��ʾ��/�� 
	std::wstring stregr;
	//	33 hcl varchar2(1) �Ƿ��к���װ�� Y/N ��ʾ��/�� 
	std::wstring strhcl;
	//	34 hcltype varchar2(1) �������� ��¼ 3.13 
	std::wstring strhcltype;
	//	35 dk varchar2(1) �Ƿ��� Y/N ��ʾ��/�� 
	std::wstring strdk;
	//	36 odometer number(8) ��̱���� ���� 
	std::wstring strodometer;
	//	37 cylinders number(2) ������ 
	std::wstring strcylinders;
	//	38 obd varchar2(1) �Ƿ��� OBD Y/N ��ʾ��/�� 
	std::wstring strobd;
	//	39 gbwdzz varchar2(1) �Ƿ��ܹرճ����ȶ� װ��(�� ESP) Y/N ��ʾ��/��
	std::wstring strgbwdzz;
	//	40 zt varchar2(1) ������״̬ �� GA24.17 
	std::wstring strzt;
	//	41 jdcxh varchar2(1) ��������� �� GA 329.2 
	std::wstring strjdcxh;
	//	42 dczz number(8) �������� ��λ kg 
	std::wstring strdczz;
	//	43 dpxh varchar2(20) �����ͺ� 
	std::wstring strdpxh;
	//	44 qdltqy number(6,2) ������̥��ѹ kPa 
	std::wstring strqdltqy;
	//	45 tg varchar2(1) �Ƿ���ȼ���������� װ�� Y/N ��ʾ��/�� 
	std::wstring strtg;
	//	46 chzhq varchar2(2) �߻�ת���� Y/N ��ʾ��/�� 
	std::wstring strchzhq;
	//	47 rygg varchar2(5) ȼ�͹�� 
	std::wstring strrygg;
	//	48 sjcys number(4) ��Ƴ�Ա�� 
	std::wstring strsjcys;
	//	49 ssxq varchar2(6) ����Ͻ�� �������� ʡ+��+��(��)�� ������-1 
	std::wstring strssxq;
	//	50 hbflbz char(1) ���������־ 
	std::wstring strhbflbz;
	//	51 gb varchar2(2) ���� ���루0-������1-���ڡ�2- ���ʣ� 
	std::wstring strgb;
	//	52 sfjrcz varchar2(1) �Ƿ������� 0 �� 1 �� 
	std::wstring strsfjrcz;
	//	53 fdjccs number(8) ����������� 
	std::wstring strfdjccs;
	//	54 dws number(4) ��λ�� 
	std::wstring strdws;
	//	55 yxrq date ��Ч���� 
	std::wstring stryxrq;
	//	56 testtype varchar2(1) ��ⷽ�� ��¼ 3.1 
	std::wstring strtesttype;
	//	57 tsno varchar2(15) ���������� 
	std::wstring strtsno;
	//	58 dlsj date ��¼ʱ�� 
	std::wstring strdlsj;
	//	59 jclx char(1) ������� ��¼ 3.19 
	std::wstring strjclx;
	//	60 jclry varchar2(50) ���¼��Ա 
	std::wstring strjclry;
	//	61 pfpdyj varchar2(600) �ŷ��ж����� 
	std::wstring strpfpdyj;
	//	62 rllb varchar2(3) ȼ����� 1��ȼ�ͣ� 2����һȼ�� 3������ȼ�� 4����϶����綯 
	std::wstring strrllb;
	//	63 fueltype1 varchar2(3) ȼ������ 2 GA24.9
	std::wstring strfueltype1;
	//	64 ddjxh varchar2(100) �綯���ͺ� 
	std::wstring strddjxh;
	//	65 cnzzxh varchar2(100) ����װ���ͺ� 
	std::wstring strcnzzxh;
	//	66 dcrl number(6,2) ������� kwh 
	std::wstring strdcrl;
	//	67 chzhqxh varchar2(100) �߻�ת�����ͺ�
	std::wstring strchzhqxh;
	//	68 lambdadown number(6,2) ��������ϵ����ֵ�� �� 
	std::wstring strlambdadown;
	//	69 lambdaup number(6,2) ��������ϵ����ֵ�� �� 
	std::wstring strlambdaup;
	//	70 hclmodel varchar(30) ����װ���ͺ�
	std::wstring strhclmodel;
};

















#endif