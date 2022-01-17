

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
	// 返回结果Code
	std::wstring code;
	// 写入失败时的msg返回详细描述信息
	std::wstring msg;
	// 
	std::wstring data;
};

// 环境参数
struct SEnvPara
{

	// 开始时间
	COleDateTime odtbegin;
	// 结束时间
	COleDateTime odtEnd;
	std::wstring JYY;
	std::wstring JYYSFZH;
	//hjwd	环境温度（摄氏度）	String	可空	小数点后保留一位
	std::wstring hjwd;
	//scwd	实测温度读数（摄氏度）	String	可空	小数点后保留一位
	std::wstring scwd;
	//hjsd	环境湿度（%）	String	可空	小数点后保留一位
	std::wstring hjsd;
	//scsd	实测湿度读数	String	可空	小数点后保留一位
	std::wstring scsd;
	//hjdqy	环境大气压kPa	String	可空	小数点后保留一位
	std::wstring hjdqy;
	//scdqy	实测大气压读数kPa	String	可空	小数点后保留一位
	std::wstring scdqy;
};
// 转速
struct STachometer
{
	// 开始时间
	COleDateTime odtbegin;
	// 结束时间
	COleDateTime odtEnd;
	std::wstring JYY;
	std::wstring JYYSFZH;
	//bzzs1	1000r/min光学转速表读数	String	可空	小数点后保留一位
	std::wstring bzzs1;
	//sc1	1000r/min实测读数	String	可空	小数点后保留一位
	std::wstring sc1;
	//bzzs2	1500r/min光学转速表读数	String	可空	小数点后保留一位
	std::wstring bzzs2;
	//sc2	1500r/min实测读数	String	可空	小数点后保留一位
	std::wstring sc2;
	//bzzs3	2000r/min光学转速表读数	String	可空	小数点后保留一位
	std::wstring bzzs3;
	//sc3	2000r/min实测读数	String	可空	小数点后保留一位
	std::wstring sc3;
	//bzzs4	2500r/min光学转速表读数	String	可空	
	std::wstring bzzs4;
	//sc4	2500r/min实测读数	String	可空	
	std::wstring sc4;
	//bzzs5	3000r/min光学转速表读数	String	可空	
	std::wstring bzzs5;
	//sc5	3000r/min实测读数	String	可空	
	std::wstring sc5;
};


// 现在对Json的处理是简单的把所有值都当作字符串，如果联网平台做了类型检验可能有问题，要改为原生Json作为参数
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
	// 禁用拷贝构造函数和拷贝赋值函数
	CZYHttp_PAI(const CZYHttp_PAI&);
	CZYHttp_PAI & operator = (const CZYHttp_PAI&);


public:
	// 查询类接口 开始

	// 90Y01-监管平台时间同步
	// 功能说明：根据检验机构编号等信息从环保监管平台获得系统时间。
	bool GetPlatFormTime(SHBMsg& sHBMsg);
	// 90Y02-机动车基本信息查询
	// 功能说明：根据检验机构编号、号牌号码、号牌种类、车辆识别代号等信息从环保监管平台获得车辆基本信息。
	bool GetVehInfo(const CStringW& hphm, const CStringW& hpzl, const CStringW& hpys, const CStringW& clsbdh, SHBMsg& sHBMsg);
	// 90Y03-限值获取接口
	// 功能说明：用于获取车辆检测限值信息。
	bool GetLimitValue(const CStringW& jyff, SHBMsg& sHBMsg);
	// 90Y04-检测人员信息下载接口
	// 功能说明：根据检测站获取检测人员信息。不需要data参数；
	bool GetPersonnelInfo(SHBMsg& sHBMsg);
	//90Y06-检验方法查询
	// 在登录环节、检验环节调用监管平台检验方法查询接口，可查询车辆历史适用排气检验方法记录（返回检最近一次的检验方法）。
	bool GetTestMethod(const CStringW& hphm, const CStringW& hpzl, const CStringW& clsbdh, SHBMsg& sHBMsg);
	// 90Y07-受理信息列表下载接口
	// 车辆受理信息，可根据时间段获取受理信息，默认下载200条，超过提醒下载失败，请重新设置时间段。
	bool GetVehAccInfo(const CStringW& jyff, const CStringW& slkssj, const CStringW& sljssj, const CStringW& hphm, const CStringW& hpzl, const CStringW& rllb, 
		std::list<TESTLOG> &lsTestLog ,SHBMsg& sHBMsg);
	// 90Y08-受理信息详情下载接口
	// 根据检验流水号获取车辆受理信息详情。
	bool GetAccInfo(const CStringW& jylsh, TESTLOG& sTestLog, VEHICLEINFO& sVehInfo, SHBMsg& sHBMsg);
	// 90Y09-车辆审核不通过原因查询
	// 检验机构根据检验流水号，检验次数获得车辆审核不通过原因信息。
	bool GetVehFailPass(const CStringW& jylsh, const CStringW& jycs, SHBMsg& sHBMsg);
	// 查询类接口 结束

	// 写入类接口 开始
	// 90Y24-机动车检验项目开始信息
	// 机动车排气检验、OBD检验项目开始时，上传检验项目开始信息。
	bool SetVehItemStart(const CStringW& jylsh, const CStringW& jycs, const CStringW& jyxm, SHBMsg& sHBMsg);
	// 90Y25-机动车检验项目结束信息
	// 机动车排气检验、OBD检验项目开始时，上传检验项目结束信息。
	bool SetVehItemEnd(const CStringW& jylsh, const CStringW& jycs, const CStringW& jyxm, SHBMsg& sHBMsg);
	// 90Y26-上报检测信号信息
	// 检测过程中，从插入采样管开始，所属检验方法的关键节点都应向平台上报检测信号。
	bool SetTestingSignalInfo(const CStringW& jylsh, const CStringW& jycs, const CStringW& jygcbs, SHBMsg& sHBMsg);
	// 90Y27-OBD检验结果信息上报
	// 车辆OBD检测结束后，上报OBD检测结果信息到监管平台。
	bool SetOBDItemEnd(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg);
	// 90Y28-OBD系统检验过程数据上报
	// OBD系统检查结束后上报监管平台
	bool SetOBDProData(const TestLog &sTestLog, SHBMsg& sHBMsg);
	// 90Y29-稳态工况法检验结果信息上报
	// 稳态工况法检测结束后上报监管平台
	bool SetASMItemEnd(SHBMsg& sHBMsg);
	// 90Y30-稳态工况法检验过程数据上报
	// 稳态工况法检测结束后上报监管平台
	bool SetASMProData(SHBMsg& sHBMsg);
	// 90Y31-简易瞬态工况法检验结果信息上报
	// 简易瞬态工况法检测结束后上报监管平台。
	bool SetVMASItemEnd(SHBMsg& sHBMsg);
	// 90Y32-简易瞬态工况法检验过程数据上报
	// 简易瞬态工况法检测结束后上报监管平台。
	bool SetVMASProData(SHBMsg& sHBMsg);
	// 90Y33-双怠速法检验结果信息上报
	// 双怠速法检测结束后上报监管平台。
	bool SetDISItemEnd(SHBMsg& sHBMsg);
	// 90Y34-双怠速法检验过程数据上报
	// 双怠速法检测结束后上报监管平台
	bool SetDISProData(SHBMsg& sHBMsg);
	// 90Y35-加载减速法检验结果信息上报
	// 加载减速法检测结束后上报监管平台。
	bool SetLugItemEnd(SHBMsg& sHBMsg);
	// 90Y36-加载减速法检验过程数据上报
	// 加载减速法检测结束后上报监管平台。
	bool SetLugProData(SHBMsg& sHBMsg);
	// 90Y37-自由加速法检验结果信息上报
	// 自由加速法检测结束后上报监管平台。
	bool SetOpaItemEnd(SHBMsg& sHBMsg);
	// 90Y38-自由加速法检验过程数据上报
	// 自由加速法检测结束后上报监管平台。
	bool SetOpaProData(SHBMsg& sHBMsg);
	// 90Y39-林格曼烟度法检验结果信息上报
	// 林格曼烟度法检测结束后上报监管平台。
	bool SetLinItemEnd(SHBMsg& sHBMsg);
	// 90Y40-燃油蒸发控制装置检验结果信息上报
	// 燃油蒸发控制装置检测结束后上报监管平台。
	bool SetEvaItemEnd(SHBMsg& sHBMsg);

	// 90Y43-汽油车底盘测功机滑行检查信息上报
	// 正式检测前，设备每日或按需检查结果实时上报监管平台。
	bool SetGasConstLoad(SHBMsg& sHBMsg);

	// 90Y44-汽油车底盘测功机附加损失测试检查信息上报
	// 正式检测前，设备每日或按需检查结果实时上报监管平台。
	bool SetGasPLHP(SHBMsg& sHBMsg);

	// 90Y45-柴油车底盘测功机滑行检查信息上报
	// 正式检测前，设备每日或按需检查结果实时上报监管平台。
	bool SetDieConstLoad(SHBMsg& sHBMsg);

	// 90Y46-柴油车底盘测功机附加功率损失测试检查信息上报
	// 正式检测前，设备每日或按需检查结果实时上报监管平台。
	bool SetDiePLHP(SHBMsg& sHBMsg);

	// 90Y47-排气分析仪检查信息上报
	// 正式检测前，设备每日或按需检查结果实时上报监管平台。
	bool SetGasCheck(const CStringW& strjclx,SHBMsg& sHBMsg);

	// 90Y48-氮氧化物分析仪检查信息上报
	// 正式检测前，设备每日或按需检查结果实时上报监管平台。
	bool SetDieCheck(const CStringW& strjclx,SHBMsg& sHBMsg);

	// 90Y49-泄漏检查信息上报
	// 正式检测前，设备每日或按需检查结果实时上报监管平台。
	bool SetLeakCheck(const CStringW& sblx, const CStringW& jyy, const CStringW& jyysfzh,SHBMsg& sHBMsg);

	// 90Y50-气体流量分析仪检查信息上报
	// 正式检测前，设备每日或按需检查结果实时上报监管平台。
	bool SetFloCheck(SHBMsg& sHBMsg);

	// 90Y51-不透光烟度计检查信息上报
	// 正式检测前，设备每日或按需检查结果实时上报监管平台。
	bool SetOpaCheck(SHBMsg& sHBMsg);

	// 90Y52-电子环境参数仪检查信息上报
	// 正式检测前，设备每日或按需检查结果实时上报监管平台。
	bool SetParaCheck(const SEnvPara& sEnvPara,SHBMsg& sHBMsg);

	// 90Y53-转速传感器检查信息上报
	// 正式检测前，设备每日或按需检查结果实时上报监管平台。。
	bool SetSpeCheck(const STachometer& sTachometer, SHBMsg& sHBMsg);

	// 写入类接口 结束
private:
	int WriteCutl(const std::wstring wstrJkid, Json::Value &root, std::wstring& wstrRet);
	int QueryCutl(const std::wstring wstrJkid, Json::Value &root, std::wstring& wstrRet);


	CString Map2Json(JsonMap mapPost);
	CString MapVec2Json(JsonMapVec vmPost);

	// Json格式转换
	CString JsonValueToCString(Json::Value root);

	// 检验方法转换
	CStringW TestTypeNameToCode(const CString strTestTypeName);
	CStringW TestTypeCodeToName(const CString strTestTypeCode);
	// 车牌颜色
	CStringW PlateTypeNameToCode(const CString strName);
	CStringW PlateTypeCodeToName(const CString strCode);
	// 3.6 燃料类别
	CStringW FuleNameToCode(const CString strName);
	CStringW FuleCodeToName(const CString strCode);
	//3.12 车身颜色
	CStringW VehColourNameToCode(const CString strName);
	CStringW VehColourCodeToName(const CString strCode);
	//	3.5 进气方式
	CStringW AirIntakeModeNameToCode(const CString strName);
	CStringW AirIntakeModeCodeToName(const CString strCode);
	//3.7 供油方式
	CStringW OilSupplyModeNameToCode(const CString strName);
	CStringW OilSupplyModeCodeToName(const CString strCode);
	// 3.8 驱动型式
	CStringW DriveTypeNameToCode(const CString strName);
	CStringW DriveTypeCodeToName(const CString strCode);
	// 3.9 燃油规格
	CStringW FuelMarkNameToCode(const CString strName);
	CStringW FuelMarkCodeToName(const CString strCode);

private:
	// URL写入地址
	std::wstring wstrWURL;
	// URL查询地址
	std::wstring wstrQURL;
	// 环保序列号
	std::wstring wstryhxlh;
	// 检验机构编号
	std::wstring wstrjyjgbh;
	// 检测线编号
	std::wstring wstrjcxdh;

private:
	// 日志文件路径（主程序）
	CStringW m_strLogFilePath;
	// 本模块名
	CStringW m_strModName;

private:
	// 加载配置文件
	void LoadConfig(void);
	// 生成日志文件
	void GenLogFile(void);
	// 初始化数据
	void InitData(void);


};