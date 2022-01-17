

#include <memory>
#include <string>
#include <vector>
#include <map>

#include "..\JsonnLib\JsonnLib.h"

#include "..\NHWin32Lib\NHWin32Lib.h"

#include "..\NHDetCommModu\NHDetCommModu.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHDetCommModu_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHDetCommModu.lib")
#endif

#include "..\JsonnLib\JsonnLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\JsonnLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\JsonnLib.lib")
#endif

struct SHBMsg
{
	// ���ؽ��Code
	std::wstring code;
	// д��ʧ��ʱ��msg������ϸ������Ϣ
	std::wstring msg;
	// 
	std::wstring data;
};

// ��������
struct SEnvPara
{

	// ��ʼʱ��
	COleDateTime odtbegin;
	// ����ʱ��
	COleDateTime odtEnd;
	std::wstring JYY;
	std::wstring JYYSFZH;
	//hjwd	�����¶ȣ����϶ȣ�	String	�ɿ�	С�������һλ
	std::wstring hjwd;
	//scwd	ʵ���¶ȶ��������϶ȣ�	String	�ɿ�	С�������һλ
	std::wstring scwd;
	//hjsd	����ʪ�ȣ�%��	String	�ɿ�	С�������һλ
	std::wstring hjsd;
	//scsd	ʵ��ʪ�ȶ���	String	�ɿ�	С�������һλ
	std::wstring scsd;
	//hjdqy	��������ѹkPa	String	�ɿ�	С�������һλ
	std::wstring hjdqy;
	//scdqy	ʵ�����ѹ����kPa	String	�ɿ�	С�������һλ
	std::wstring scdqy;
};
// ת��
struct STachometer
{
	// ��ʼʱ��
	COleDateTime odtbegin;
	// ����ʱ��
	COleDateTime odtEnd;
	std::wstring JYY;
	std::wstring JYYSFZH;
	//bzzs1	1000r/min��ѧת�ٱ����	String	�ɿ�	С�������һλ
	std::wstring bzzs1;
	//sc1	1000r/minʵ�����	String	�ɿ�	С�������һλ
	std::wstring sc1;
	//bzzs2	1500r/min��ѧת�ٱ����	String	�ɿ�	С�������һλ
	std::wstring bzzs2;
	//sc2	1500r/minʵ�����	String	�ɿ�	С�������һλ
	std::wstring sc2;
	//bzzs3	2000r/min��ѧת�ٱ����	String	�ɿ�	С�������һλ
	std::wstring bzzs3;
	//sc3	2000r/minʵ�����	String	�ɿ�	С�������һλ
	std::wstring sc3;
	//bzzs4	2500r/min��ѧת�ٱ����	String	�ɿ�	
	std::wstring bzzs4;
	//sc4	2500r/minʵ�����	String	�ɿ�	
	std::wstring sc4;
	//bzzs5	3000r/min��ѧת�ٱ����	String	�ɿ�	
	std::wstring bzzs5;
	//sc5	3000r/minʵ�����	String	�ɿ�	
	std::wstring sc5;
};


// ���ڶ�Json�Ĵ����Ǽ򵥵İ�����ֵ�������ַ������������ƽ̨�������ͼ�����������⣬Ҫ��Ϊԭ��Json��Ϊ����
typedef std::map<CString, CString>	JsonMap;
typedef std::vector<JsonMap>		JsonMapVec;


class AFX_EXT_CLASS CZYHttp_PAI
{
	friend class std::auto_ptr<CZYHttp_PAI>;


public:
	static CZYHttp_PAI& GetInstance(void);

public:
	CZYHttp_PAI(void);
	virtual ~CZYHttp_PAI(void);

private:
	// ���ÿ������캯���Ϳ�����ֵ����
	CZYHttp_PAI(const CZYHttp_PAI&);
	CZYHttp_PAI & operator = (const CZYHttp_PAI&);


public:
	// ��ѯ��ӿ� ��ʼ

	// 90Y01-���ƽ̨ʱ��ͬ��
	// ����˵�������ݼ��������ŵ���Ϣ�ӻ������ƽ̨���ϵͳʱ�䡣
	bool GetPlatFormTime(SHBMsg& sHBMsg);
	// 90Y02-������������Ϣ��ѯ
	// ����˵�������ݼ��������š����ƺ��롢�������ࡢ����ʶ����ŵ���Ϣ�ӻ������ƽ̨��ó���������Ϣ��
	bool GetVehInfo(const CStringW& hphm, const CStringW& hpzl, const CStringW& hpys, const CStringW& clsbdh, SHBMsg& sHBMsg);
	// 90Y03-��ֵ��ȡ�ӿ�
	// ����˵�������ڻ�ȡ���������ֵ��Ϣ��
	bool GetLimitValue(const CStringW& jyff, SHBMsg& sHBMsg);
	// 90Y04-�����Ա��Ϣ���ؽӿ�
	// ����˵�������ݼ��վ��ȡ�����Ա��Ϣ������Ҫdata������
	bool GetPersonnelInfo(SHBMsg& sHBMsg);
	//90Y06-���鷽����ѯ
	// �ڵ�¼���ڡ����黷�ڵ��ü��ƽ̨���鷽����ѯ�ӿڣ��ɲ�ѯ������ʷ�����������鷽����¼�����ؼ����һ�εļ��鷽������
	bool GetTestMethod(const CStringW& hphm, const CStringW& hpzl, const CStringW& clsbdh, SHBMsg& sHBMsg);
	// 90Y07-������Ϣ�б����ؽӿ�
	// ����������Ϣ���ɸ���ʱ��λ�ȡ������Ϣ��Ĭ������200����������������ʧ�ܣ�����������ʱ��Ρ�
	bool GetVehAccInfo(const CStringW& jyff, const CStringW& slkssj, const CStringW& sljssj, const CStringW& hphm, const CStringW& hpzl, const CStringW& rllb, 
		std::list<TESTLOG> &lsTestLog ,SHBMsg& sHBMsg);
	// 90Y08-������Ϣ�������ؽӿ�
	// ���ݼ�����ˮ�Ż�ȡ����������Ϣ���顣
	bool GetAccInfo(const CStringW& jylsh, TESTLOG& sTestLog, VEHICLEINFO& sVehInfo, SHBMsg& sHBMsg);
	// 90Y09-������˲�ͨ��ԭ���ѯ
	// ����������ݼ�����ˮ�ţ����������ó�����˲�ͨ��ԭ����Ϣ��
	bool GetVehFailPass(const CStringW& jylsh, const CStringW& jycs, SHBMsg& sHBMsg);
	// ��ѯ��ӿ� ����

	// д����ӿ� ��ʼ
	// 90Y24-������������Ŀ��ʼ��Ϣ
	// �������������顢OBD������Ŀ��ʼʱ���ϴ�������Ŀ��ʼ��Ϣ��
	bool SetVehItemStart(const CStringW& jylsh, const CStringW& jycs, const CStringW& jyxm, SHBMsg& sHBMsg);
	// 90Y25-������������Ŀ������Ϣ
	// �������������顢OBD������Ŀ��ʼʱ���ϴ�������Ŀ������Ϣ��
	bool SetVehItemEnd(const CStringW& jylsh, const CStringW& jycs, const CStringW& jyxm, SHBMsg& sHBMsg);
	// 90Y26-�ϱ�����ź���Ϣ
	// �������У��Ӳ�������ܿ�ʼ���������鷽���Ĺؼ��ڵ㶼Ӧ��ƽ̨�ϱ�����źš�
	bool SetTestingSignalInfo(const CStringW& jylsh, const CStringW& jycs, const CStringW& jygcbs, SHBMsg& sHBMsg);
	// 90Y27-OBD��������Ϣ�ϱ�
	// ����OBD���������ϱ�OBD�������Ϣ�����ƽ̨��
	bool SetOBDItemEnd(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg);
	// 90Y28-OBDϵͳ������������ϱ�
	// OBDϵͳ���������ϱ����ƽ̨
	bool SetOBDProData(const TestLog &sTestLog, SHBMsg& sHBMsg);
	// 90Y29-��̬��������������Ϣ�ϱ�
	// ��̬���������������ϱ����ƽ̨
	bool SetASMItemEnd(SHBMsg& sHBMsg);
	// 90Y30-��̬������������������ϱ�
	// ��̬���������������ϱ����ƽ̨
	bool SetASMProData(SHBMsg& sHBMsg);
	// 90Y31-����˲̬��������������Ϣ�ϱ�
	// ����˲̬���������������ϱ����ƽ̨��
	bool SetVMASItemEnd(SHBMsg& sHBMsg);
	// 90Y32-����˲̬������������������ϱ�
	// ����˲̬���������������ϱ����ƽ̨��
	bool SetVMASProData(SHBMsg& sHBMsg);
	// 90Y33-˫���ٷ���������Ϣ�ϱ�
	// ˫���ٷ����������ϱ����ƽ̨��
	bool SetDISItemEnd(SHBMsg& sHBMsg);
	// 90Y34-˫���ٷ�������������ϱ�
	// ˫���ٷ����������ϱ����ƽ̨
	bool SetDISProData(SHBMsg& sHBMsg);
	// 90Y35-���ؼ��ٷ���������Ϣ�ϱ�
	// ���ؼ��ٷ����������ϱ����ƽ̨��
	bool SetLugItemEnd(SHBMsg& sHBMsg);
	// 90Y36-���ؼ��ٷ�������������ϱ�
	// ���ؼ��ٷ����������ϱ����ƽ̨��
	bool SetLugProData(SHBMsg& sHBMsg);
	// 90Y37-���ɼ��ٷ���������Ϣ�ϱ�
	// ���ɼ��ٷ����������ϱ����ƽ̨��
	bool SetOpaItemEnd(SHBMsg& sHBMsg);
	// 90Y38-���ɼ��ٷ�������������ϱ�
	// ���ɼ��ٷ����������ϱ����ƽ̨��
	bool SetOpaProData(SHBMsg& sHBMsg);
	// 90Y39-�ָ����̶ȷ���������Ϣ�ϱ�
	// �ָ����̶ȷ����������ϱ����ƽ̨��
	bool SetLinItemEnd(SHBMsg& sHBMsg);
	// 90Y40-ȼ����������װ�ü�������Ϣ�ϱ�
	// ȼ����������װ�ü��������ϱ����ƽ̨��
	bool SetEvaItemEnd(SHBMsg& sHBMsg);

	// 90Y43-���ͳ����̲⹦�����м����Ϣ�ϱ�
	// ��ʽ���ǰ���豸ÿ�ջ�������ʵʱ�ϱ����ƽ̨��
	bool SetGasConstLoad(SHBMsg& sHBMsg);

	// 90Y44-���ͳ����̲⹦��������ʧ���Լ����Ϣ�ϱ�
	// ��ʽ���ǰ���豸ÿ�ջ�������ʵʱ�ϱ����ƽ̨��
	bool SetGasPLHP(SHBMsg& sHBMsg);

	// 90Y45-���ͳ����̲⹦�����м����Ϣ�ϱ�
	// ��ʽ���ǰ���豸ÿ�ջ�������ʵʱ�ϱ����ƽ̨��
	bool SetDieConstLoad(SHBMsg& sHBMsg);

	// 90Y46-���ͳ����̲⹦�����ӹ�����ʧ���Լ����Ϣ�ϱ�
	// ��ʽ���ǰ���豸ÿ�ջ�������ʵʱ�ϱ����ƽ̨��
	bool SetDiePLHP(SHBMsg& sHBMsg);

	// 90Y47-���������Ǽ����Ϣ�ϱ�
	// ��ʽ���ǰ���豸ÿ�ջ�������ʵʱ�ϱ����ƽ̨��
	bool SetGasCheck(const CStringW& strjclx,SHBMsg& sHBMsg);

	// 90Y48-������������Ǽ����Ϣ�ϱ�
	// ��ʽ���ǰ���豸ÿ�ջ�������ʵʱ�ϱ����ƽ̨��
	bool SetDieCheck(const CStringW& strjclx,SHBMsg& sHBMsg);

	// 90Y49-й©�����Ϣ�ϱ�
	// ��ʽ���ǰ���豸ÿ�ջ�������ʵʱ�ϱ����ƽ̨��
	bool SetLeakCheck(const CStringW& sblx, const CStringW& jyy, const CStringW& jyysfzh,SHBMsg& sHBMsg);

	// 90Y50-�������������Ǽ����Ϣ�ϱ�
	// ��ʽ���ǰ���豸ÿ�ջ�������ʵʱ�ϱ����ƽ̨��
	bool SetFloCheck(SHBMsg& sHBMsg);

	// 90Y51-��͸���̶ȼƼ����Ϣ�ϱ�
	// ��ʽ���ǰ���豸ÿ�ջ�������ʵʱ�ϱ����ƽ̨��
	bool SetOpaCheck(SHBMsg& sHBMsg);

	// 90Y52-���ӻ��������Ǽ����Ϣ�ϱ�
	// ��ʽ���ǰ���豸ÿ�ջ�������ʵʱ�ϱ����ƽ̨��
	bool SetParaCheck(const SEnvPara& sEnvPara,SHBMsg& sHBMsg);

	// 90Y53-ת�ٴ����������Ϣ�ϱ�
	// ��ʽ���ǰ���豸ÿ�ջ�������ʵʱ�ϱ����ƽ̨����
	bool SetSpeCheck(const STachometer& sTachometer, SHBMsg& sHBMsg);

	// д����ӿ� ����
private:
	int WriteCutl(const std::wstring wstrJkid, Json::Value &root, std::wstring& wstrRet);
	int QueryCutl(const std::wstring wstrJkid, Json::Value &root, std::wstring& wstrRet);


	CString Map2Json(JsonMap mapPost);
	CString MapVec2Json(JsonMapVec vmPost);

	// Json��ʽת��
	CString JsonValueToCString(Json::Value root);

	// ���鷽��ת��
	CStringW TestTypeNameToCode(const CString strTestTypeName);
	CStringW TestTypeCodeToName(const CString strTestTypeCode);
	// ������ɫ
	CStringW PlateTypeNameToCode(const CString strName);
	CStringW PlateTypeCodeToName(const CString strCode);
	// 3.6 ȼ�����
	CStringW FuleNameToCode(const CString strName);
	CStringW FuleCodeToName(const CString strCode);
	//3.12 ������ɫ
	CStringW VehColourNameToCode(const CString strName);
	CStringW VehColourCodeToName(const CString strCode);
	//	3.5 ������ʽ
	CStringW AirIntakeModeNameToCode(const CString strName);
	CStringW AirIntakeModeCodeToName(const CString strCode);
	//3.7 ���ͷ�ʽ
	CStringW OilSupplyModeNameToCode(const CString strName);
	CStringW OilSupplyModeCodeToName(const CString strCode);
	// 3.8 ������ʽ
	CStringW DriveTypeNameToCode(const CString strName);
	CStringW DriveTypeCodeToName(const CString strCode);
	// 3.9 ȼ�͹��
	CStringW FuelMarkNameToCode(const CString strName);
	CStringW FuelMarkCodeToName(const CString strCode);

private:
	// URLд���ַ
	std::wstring wstrWURL;
	// URL��ѯ��ַ
	std::wstring wstrQURL;
	// �������к�
	std::wstring wstryhxlh;
	// ����������
	std::wstring wstrjyjgbh;
	// ����߱��
	std::wstring wstrjcxdh;

private:
	// ��־�ļ�·����������
	CStringW m_strLogFilePath;
	// ��ģ����
	CStringW m_strModName;

private:
	// ���������ļ�
	void LoadConfig(void);
	// ������־�ļ�
	void GenLogFile(void);
	// ��ʼ������
	void InitData(void);


};