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
	// 5.1.1获取访问令牌接口 getAccessToken
	static int getAccessToken(const char * pchURL, const std::wstring& strjcxbh, std::wstring& strRetStr);
	// 5.1.2获取待检车辆接口 
	static int getDjzt(const char * pchURL, const std::wstring& straccessToken, std::wstring& strRetStr);
	// 5.1.3获取车辆信息接口 
	static int getDjclxx(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, std::wstring& strRetStr);
	// 5.1.4发送消息通知接口
	static int getDjclxx(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strcode,
		const std::wstring& strmessage, std::wstring& strRetStr);
	// 5.1.5上传工控软件版本号接口
	static int gkrjbbh(const char * pchURL, const std::wstring& straccessToken, const std::wstring& strbbh, std::wstring& strRetStr);
	// 5.1.6服务器时间同步接口
	static int fwqsjtb(const char * pchURL, std::wstring& strRetStr);
	// 5.1.7检测站检测人员列表接口
	static int jcrylb(const char * pchURL, const std::wstring& strjczbh, const std::wstring& strjsdm, std::wstring& strRetStr);
	// 5.1.8OBD待检列表接口
	static int obdDjlb(const char * pchURL, const std::wstring& strjczbh, const std::wstring& strcphm, std::wstring& strRetStr);
	// 5.2.1测功机加载滑行检查结果接口
	//accessToken	访问令牌
	//gsjzllsj	高速加载滑行理论时间
	//gsjzsjsj	高速加载滑行实际时间
	//dsjzllsj	低速加载滑行理论时间
	//dsjzsjsj	低速加载滑行实际时间
	//gsjzns	高速加载滑行内损功率
	//dsjzns	低速加载滑行内损功率
	//gsjzhxgl	高速加载滑行指示功率
	//dsjzhxgl	低速加载滑行指示功率
	//jbgl	基本惯量
	//gsjzjcjg	高速加载滑行结果
	//dsjzjcjg	低速加载滑行结果
	//jcjg	检查结果
	//kssj	检查开始时间
	//jssj	检查结束时间
	//bz	备注
	static int cgjzj(const char * pchURL, const std::wstring& straccessToken, const std::wstring& strgsjzllsj, const std::wstring& strgsjzsjsj,
		 const std::wstring& strdsjzllsj, const std::wstring& strdsjzsjsj, const std::wstring& strgsjzns, const std::wstring& strdsjzns,
		 const std::wstring& strgsjzhxgl, const std::wstring& strdsjzhxgl, const std::wstring& strjbgl, const std::wstring& strgsjzjcjg,
		 const std::wstring& strdsjzjcjg, const std::wstring& strjcjg, const std::wstring& strkssj, const std::wstring& strjssj,
		 const std::wstring& strbz, std::wstring& strRetStr);
	// 5.2.2五气分析仪泄露检查接口
	//accessToken	访问令牌
	//jcjg	密封性检测结果
	//kssj	检查开始时间
	//jssj	检查结束时间
	//bz	备注
	static int wqfxyzj(const char * pchURL, const std::wstring& straccessToken, const std::wstring& strjcjg, const std::wstring& strkssj,
		 const std::wstring& strjssj, const std::wstring& strbz, std::wstring& strRetStr);
	// 5.2.3不透光烟度计校准检查结果接口
	//accessToken	访问令牌
	//ldjcjg	不透光烟度计零点检查结果
	//lgpz1	不透光烟度计1滤光片值
	//lgpz2	不透光烟度计2滤光片值
	//jcz1	不透光烟度计1检查值
	//jcz2	不透光烟度计2检查值
	//wcl1	不透光烟度计1误差率
	//wcl2	不透光烟度计2误差率
	//jcjg	检查结果
	//kssj	检查开始时间
	//jssj	检查结束时间
	//bz	备注
	static int ydjzj(const char * pchURL, const std::wstring& straccessToken, const std::wstring& strldjcjg, const std::wstring& strlgpz1,
		 const std::wstring& strlgpz2, const std::wstring& strjcz1, const std::wstring& strjcz2, const std::wstring& strwcl1,
		 const std::wstring& strwcl2, const std::wstring& strjcjg,
		 const std::wstring& kssj, const std::wstring& jssj, const std::wstring& strbz, std::wstring& strRetStr);
	// 5.2.4环境参数仪(气象站)校准检查接口
	//accessToken	访问令牌
	//sjhjwd	实际环境温度
	//clhjwd	测量环境温度
	//sjhjsd	实际环境相对湿度
	//clhjsd	测量环境相对湿度
	//sjdqyl	实际环境大气压力
	//cldqyl	测量环境大气压力
	//kssj	检查开始时间
	//jssj	检查结束时间
	//bz	备注
	static int hjcsyzj(const char * pchURL, const std::wstring& straccessToken, const std::wstring& strsjhjwd, const std::wstring& strclhjwd,
		 const std::wstring& strsjhjsd, const std::wstring& strclhjsd, const std::wstring& strsjdqyl, const std::wstring& strcldqyl,
		 const std::wstring& kssj, const std::wstring& jssj, const std::wstring& strbz, std::wstring& strRetStr);
	// 5.2.5五气分析仪校准结果接口
	//accessToken	访问令牌
	//bqlb	标气类别
	//bzCO2	CO2标气浓度值
	//sjCO2	CO2实测浓度值
	//bzCO	CO标气浓度值
	//sjCO	CO实测浓度值
	//bzNO	NO标气浓度值
	//sjNO	NO实测浓度值
	//bzHC	HC标气浓度值
	//sjHC	HC实测浓度值
	//bzO2	O2标气浓度值
	//sjO2	O2实测浓度值
	//sjPEF	名义丙烷当量系数
	//bzC3H8	标气丙烷浓度值
	//jcjg	校准结果
	//kssj	校准开始时间
	//jssj	校准结束时间
	//bz	备注
	//bz	备注
	static int wqfxybd(const char * pchURL, const std::wstring& straccessToken, const std::wstring& strbqlb, const std::wstring& strbzCO2,
		 const std::wstring& strsjCO2, const std::wstring& strbzCO, const std::wstring& strsjCO, const std::wstring& strbzNO,
		 const std::wstring& strsjNO, const std::wstring& strbzHC, const std::wstring& strsjHC, const std::wstring& strbzO2,
		 const std::wstring& strsjO2, const std::wstring& strsjPEF, const std::wstring& strbzC3H8, const std::wstring& strjcjg,
		 const std::wstring& kssj, const std::wstring& jssj, const std::wstring& strbz, std::wstring& strRetStr);
	// 5.2.6流量计设备自检结果接口
	//accessToken	访问令牌
	//jqglljcjg	集气管流量检查结果
	//lljylcjcjg	流量计氧量程检查结果
	//yqglcbz	氧气高量程标值
	//yqglcclz	氧气高量程测量值
	//yqglcwc	氧气高量程误差
	//yqdlcbz	氧气低量程标值
	//yqdlcclz	氧气低量程测量值
	//yqdlcwc	氧气低量程误差
	//jcjg	检查结果
	//kssj	检查开始时间
	//jssj	检查结束时间
	//remark	备注
	static int lljzj(const char * pchURL, const std::wstring& straccessToken, const std::wstring& strjqglljcjg, const std::wstring& strlljylcjcjg,
		 const std::wstring& stryqglcbz, const std::wstring& stryqglcclz, const std::wstring& stryqglcwc, const std::wstring& stryqdlcbz,
		 const std::wstring& stryqdlcclz, const std::wstring& stryqdlcwc, const std::wstring& strjcjg,
		 const std::wstring& kssj, const std::wstring& jssj, const std::wstring& strbz, std::wstring& strRetStr);
	// 5.3.1OBD快速检查结果数据上传接口
	//lsh	检验流水号
	//clsbm	VIN
	//obdzsbz	OBD执行标准
	//clxslc	车辆行驶里程
	//fdjkzdycid	发动机控制单元标定识别码cid
	//fdjkzdycvn	发动机控制单元校准核查号码 
	//hclkzdycid	后处理控制单元标定识别码cid
	//hclkzdycvn	后处理控制单元校准核查号码 
	//qtkzdycid	其他控制单元标定识别码cid
	//qtkzdycvn	其他控制单元校准核查号码 
	//obdgzzsq	OBD系统故障指示器
	//obdtxqk	OBD通讯情况
	//obdzzqsfdl	OBD故障指示器是否点亮
	//obdjxzt	就绪状态未完成项目
	//obdjcjg	OBD检查结果
	//obdgzxx	OBD故障信息
	//jyymc	检验员名称
	//jckssj	检测开始时间
	//Jjcjssj	检测结束时间
	static int obdjJgsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& strclsbm, const std::wstring& strobdzsbz, const std::wstring& strclxslc,
		 const std::wstring& strfdjkzdycid, const std::wstring& strfdjkzdycvn, const std::wstring& strhclkzdycid, const std::wstring& strhclkzdycvn,
		 const std::wstring& strqtkzdycid, const std::wstring& strqtkzdycvn, const std::wstring& strobdgzzsq, const std::wstring& strobdtxqk,
		 const std::wstring& strobdzzqsfdl, const std::wstring& strobdjxzt, const std::wstring& strobdjcjg,const std::wstring& strobdgzxx,
		 const std::wstring& strjyymc, const std::wstring& strjckssj, const std::wstring& strJjcjssj, std::wstring& strRetStr);
	// 5.3.2检测过程OBD数据流上传接口
	//lsh	检验流水号
	//accessToken	访问令牌
	//qssx	全时时序
	//Jcxshjcsxh	检测顺序号
	//jqmjdkd	节气门绝对开度
	//fhz	负荷值
	//ycgqxh	前氧传感器信号
	//glkqxs	过量空气系数
	//cs	车速
	//fdjgl	发动机输出功率
	//fdjzs	发动机转速
	//jql	进气量
	//jqyl	进气压力
	//zyyl	增压压力
	//ymkd	油门绝对开度
	//hyl	耗油量
	//nycgqnd	氮氧传感器浓度
	//nspsl	尿素喷射量
	//pqwd	排气温度
	//klbzqyc	颗粒捕捉器压差
	//egrkd	EGR开度
	//rypsyl	燃油喷射压力
	static int obdgGcsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strqssx, const std::wstring& strJcxshjcsxh,
		 const std::wstring& strjqmjdkd, const std::wstring& strfhz, const std::wstring& strycgqxh, const std::wstring& strglkqxs,
		 const std::wstring& strcs, const std::wstring& strfdjgl, const std::wstring& strfdjzs, const std::wstring& strjql,
		 const std::wstring& strjqyl, const std::wstring& strzyyl, const std::wstring& strymkd,const std::wstring& strnycgqnd, const std::wstring& strnspsl,
		 const std::wstring& strpqwd, const std::wstring& strklbzqyc, const std::wstring& stregrkd, const std::wstring& strrypsyl, std::wstring& strRetStr);
	// 5.3.3汽油车OBD-IUPR检测数据
	//lsh	检验流水号
	//chqz1jccs	催化器组1监测完成次数
	//chqz1fhtjcs	催化器组1符合条件次数
	//chqz1iuprl	催化器组1IUPR率
	//chqz2jccs	催化器组2监测完成次数
	//chqz2fhtjcs	催化器组2符合条件次数
	//chqz2iuprl	催化器组2IUPR率
	//qycgqz1jccs	前氧传感器组1监测完成次数
	//qycgqz1fhtjcs	前氧传感器组1符合条件次数
	//qycgqz1iuprl	前氧传感器组1 IUPR率
	//qycgqz2jccs	前氧传感器组2监测完成次数
	//qycgqz2fhtjcs	前氧传感器组2符合条件次数
	//qycgqz2iuprl	前氧传感器组2 IUPR率
	//hycgqz1jccs	后氧传感器组1监测完成次数
	//hycgqz1fhtjcs	后氧传感器组1符合条件次数
	//hycgqz1iuprl	后氧传感器组1 IUPR率
	//hycgqz2jccs	后氧传感器组2监测完成次数
	//hycgqz2fhtjcs	后氧传感器组2符合条件次数
	//hycgqz2iuprl	后氧传感器组2 IUPR率
	//evapjccs	EVAP监测完成次数
	//evapfhtjcs	EVAP符合条件次数
	//evapiuprl	EVAP IUPR率
	//egrvvtjccs	EGR和VVT监测完成次数
	//egrvvtfhtjcs	EGR和VVT符合条件次数
	//egrvvtiuprl	EGR和VVT IUPR率
	//gpfz1jccs	GPF组1监测完成次数
	//gpfz1fhtjcs	GPF组1符合条件次数
	//gpfz1iuprl	GPF组1IUPR率
	//gpfz2jccs	GPF组2监测完成次数
	//gpfz2fhtjcs	GPF组2符合条件次数
	//gpfz2iuprl	GPF组2IUPR率
	//eckqpsxtjccs	二次空气喷射系统监测完成次数
	//eckqpsxtfhtjcs	二次空气喷射系统符合条件次数
	//eckqpsxtiuprl	二次空气喷射系统IUPR率
	static int qycObdIupr(const char * pchURL, const std::wstring& strlsh, const std::wstring& strchqz1jccs, const std::wstring& strchqz1fhtjcs, const std::wstring& strchqz1iuprl,
		 const std::wstring& strchqz2jccs, const std::wstring& strchqz2fhtjcs, const std::wstring& strchqz2iuprl, const std::wstring& strqycgqz1jccs,
		 const std::wstring& strqycgqz1fhtjcs, const std::wstring& strqycgqz1iuprl, const std::wstring& strqycgqz2jccs, const std::wstring& strqycgqz2fhtjcs,
		 const std::wstring& strqycgqz2iuprl, const std::wstring& strhycgqz1jccs, const std::wstring& strhycgqz1fhtjcs,const std::wstring& strhycgqz1iuprl, const std::wstring& strhycgqz2jccs,
		 const std::wstring& strhycgqz2fhtjcs, const std::wstring& strhycgqz2iuprl, const std::wstring& strevapjccs, const std::wstring& strevapfhtjcs, 
		 const std::wstring& strevapiuprl, const std::wstring& stregrvvtjccs, const std::wstring& stregrvvtfhtjcs, const std::wstring& stregrvvtiuprl, 
		 const std::wstring& strgpfz1jccs, const std::wstring& strgpfz1fhtjcs, const std::wstring& strgpfz1iuprl, const std::wstring& strgpfz2jccs, 
		 const std::wstring& strgpfz2fhtjcs, const std::wstring& strgpfz2iuprl, const std::wstring& streckqpsxtjccs, const std::wstring& streckqpsxtfhtjcs, 
		 const std::wstring& streckqpsxtiuprl, std::wstring& strRetStr);
	// 5.3.4柴油车OBD-IUPR检测数据
	//lsh	检验流水号
	//nmhcchqjccs	NMHC催化器监测完成次数
	//nmhcchqfhtjcs	NMHC催化器符合条件次数
	//nmhcchqiuprl	NMHC催化器IUPR率
	//noxchqjccs	NOx催化器监测完成次数
	//noxchqfhtjcs	NOx催化器符合条件次数
	//noxchqiuprl	NOx催化器IUPR率
	//noxxfqjccs	NOx吸附器监测完成次数
	//noxxfqfhtjcs	NOx吸附器符合条件次数
	//noxxfqiuprl	NOx吸附器IUPR率
	//pmbjqjccs	PM捕集器监测完成次数
	//pmbjqfhtjcs	PM捕集器符合条件次数
	//pmbjqiuprl	PM捕集器IUPR率
	//fqcgqjccs	废气传感器监测完成次数
	//fqcgqfhtjcs	废气传感器符合条件次数
	//fqcgqiuprl	废气传感器IUPR率
	//egrvvtjccs	EGR和VVT监测完成次数
	//egrvvtfhtjcs	EGR和VVT符合条件次数
	//egrvvtiuprl	EGR和VVT IUPR率
	//zyyljccs	增压压力监测监测完成次数
	//zyylfhtjcs	增压压力监测符合条件次数
	//zyyliuprl	增压压力监测IUPR率
	static int cycObdIupr(const char * pchURL, const std::wstring& strlsh, const std::wstring& strnmhcchqjccs, const std::wstring& strnmhcchqfhtjcs, const std::wstring& strnmhcchqiuprl,
		 const std::wstring& strnoxchqjccs, const std::wstring& strnoxchqfhtjcs, const std::wstring& strnoxchqiuprl, const std::wstring& strnoxxfqjccs,
		 const std::wstring& strnoxxfqfhtjcs, const std::wstring& strnoxxfqiuprl, const std::wstring& strpmbjqjccs, const std::wstring& strpmbjqfhtjcs,
		 const std::wstring& strpmbjqiuprl, const std::wstring& strfqcgqjccs, const std::wstring& strfqcgqfhtjcs,const std::wstring& strfqcgqiuprl, const std::wstring& stregrvvtjccs,
		 const std::wstring& stregrvvtfhtjcs, const std::wstring& stregrvvtiuprl, const std::wstring& strzyyljccs, const std::wstring& strzyylfhtjcs, 
		 const std::wstring& strzyyliuprl, std::wstring& strRetStr);
	// 5.3.5燃油蒸发排放控制系统检验数据上传接口
	//lsh	检验流水号
	//accessToken	访问令牌
	//jykcsyl	进油口初始压力
	//jykssyl	进油口损失压力
	//Jjykcsjg	进油口测试结果
	//yxgxlll	油箱盖泄露流量
	//yxgcsyl	油箱盖初始压力
	//yxgssyl	油箱盖10秒损失压力
	//yxgcsjg	油箱盖测试结果
	//kssj	检测开始时间
	//jssj	检测结束时间
	static int ryzfJysj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strjykcsyl, const std::wstring& strjykssyl,
		 const std::wstring& strjykcsjg, const std::wstring& stryxgxlll, const std::wstring& stryxgcsyl, const std::wstring& stryxgssyl,
		 const std::wstring& stryxgcsjg, const std::wstring& strkssj, const std::wstring& strjssj, std::wstring& strRetStr);
	// 5.4.1上传简易瞬态工况法检测结果接口
	//lsh	检验流水号
	//accessToken	启动访问令牌
	//co	一氧化碳检测值
	//hc	碳氢化合物检测值
	//no	氮氧化合物检测值
	//sdgl	测功机设定功率
	//sjxsjl	车辆实际行驶距离
	//cssj	测试时间
	//plcs	测试过程偏离次数
	//zplsj	总计偏离时间
	//wd	环境温度
	//sd	环境湿度
	//dqy	环境大气压
	//ynd	环境氧浓度
	//glkqxs	过量空气系数
	//kssj	检测开始时间
	//jssj	检测结束时间
	static int jystJgsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strco, const std::wstring& strhc,
		 const std::wstring& strno, const std::wstring& strsdgl, const std::wstring& strsjxsjl, const std::wstring& strcssj,
		 const std::wstring& strplcs, const std::wstring& strzplsj, const std::wstring& strwd, const std::wstring& strsd,
		 const std::wstring& strdqy, const std::wstring& strynd, const std::wstring& strglkqxs,
		 const std::wstring& stryxgcsjg, const std::wstring& strkssj, const std::wstring& strjssj, std::wstring& strRetStr);
	// 5.4.2上传简易瞬态工况法检测过程数据接口
	//lsh	检验流水号
	//accessToken	启动访问令牌
	//qssx	全程时序
	//gklx	工况类型
	//jcsxh	检测顺序号
	//wxzhc	HC浓度每秒数据(未修正)
	//hc	HC浓度每秒数据(稀释修正后)
	//wxzco	CO浓度每秒数据(未修正)
	//co	CO浓度每秒数据(稀释修正后)
	//co2	CO2浓度每秒数据
	//wxznox	NOx浓度每秒数据(未修正)
	//nox	NOx浓度每秒数据(稀释修正后)
	//o2	O2浓度每秒数据
	//sjll	实际流量每秒数据
	//bzll	标准流量每秒数据
	//wqll	汽车尾气流量每秒数据
	//cs	车速每秒数据
	//bzss	标准时速每秒数据
	//fdjzs	发动机转速每秒数据
	//glkqxs	过量空气系数
	//xsxzxs	稀释系数
	//nosdxzxs	NO湿度修正系数
	//xsb	稀释比每秒数据
	//jzgl	测试工况加载功率每秒数据
	//jsgl	测试工况寄生功率每秒数据
	//zsgl	测试工况指示功率每秒数据
	//lljqy	测试工况流量计气压力每秒数据
	//lljwd	测试工况流量计温度每秒数据
	//nl	扭力每秒数据
	//fxyglyl	分析仪管路压力
	//scfz	实测底盘测功机负载
	//wd	环境温度每秒数据
	//sd	测试工况环境相对湿度每秒数据
	//dqy	测试工况环境大气压力每秒数据
	static int jystGcsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strqssx, const std::wstring& strgklx,
		 const std::wstring& strjcsxh, const std::wstring& strwxzhc, const std::wstring& strhc, const std::wstring& strwxzco, const std::wstring& strco,
		 const std::wstring& strco2, const std::wstring& strwxznox, const std::wstring& strnox, const std::wstring& stro2, const std::wstring& strsjll,
		 const std::wstring& strbzll, const std::wstring& strwqll, const std::wstring& strcs, const std::wstring& strbzss, const std::wstring& strfdjzs,
		 const std::wstring& strglkqxs, const std::wstring& strxsxzxs, const std::wstring& strnosdxzxs, const std::wstring& strxsb, const std::wstring& strjzgl,
		 const std::wstring& strjsgl, const std::wstring& strzsgl, const std::wstring& strlljqy, const std::wstring& strlljwd, const std::wstring& strnl,
		 const std::wstring& strfxyglyl, const std::wstring& strscfz, const std::wstring& strwd, const std::wstring& strsd, const std::wstring& strdqy,
		 std::wstring& strRetStr);
	// 5.4.3上传稳态工况法检测结果接口
	//lsh	检验流水号
	//accessToken	启动访问令牌
	//hc5025	5025工况hc检测值
	//co5025	5025工况co 检测值
	//no5025	5025工况no 检测值
	//fdjzs5025	5025工况发动机转速
	//fdjyw5025	5025发动机油温
	//pjcs5025	5025平均车速
	//zgl5025	5025阶段底盘测功机所加载的总功率
	//hc2540	2540工况hc检测值
	//co2540	2540工况co 检测值
	//no2540	2540工况no 检测值
	//fdjzs2540	2540工况发动机转速
	//fdjyw2540	2540发动机油温
	//pjcs2540	2540平均车速
	//zgl2540	2540阶段底盘测功机所加载的总功率
	//glkqxs	过量空气系数
	//wd	温度
	//sd	湿度
	//dqy	气压
	//kssj	检测开始时间
	//jssj	检测结束时间
	static int wtJgsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strhc5025, const std::wstring& strco5025,
		 const std::wstring& strno5025, const std::wstring& strfdjzs5025, const std::wstring& strfdjyw5025, const std::wstring& strpjcs5025,
		 const std::wstring& strzgl5025, const std::wstring& strhc2540, const std::wstring& strco2540, const std::wstring& strno2540,
		 const std::wstring& strfdjzs2540, const std::wstring& strfdjyw2540, const std::wstring& strpjcs2540, const std::wstring& strzgl2540,
		 const std::wstring& strglkqxs, const std::wstring& strwd, const std::wstring& strsd, const std::wstring& strdqy, 
		 const std::wstring& strkssj, const std::wstring& strjssj, std::wstring& strRetStr);
	// 5.4.4上传稳态工况法检测过程数据接口
	//lsh	检验流水号
	//accessToken	启动访问令牌
	//qssx	全时时序
	//gklx	工况类型
	//jcsxh	检测顺序号
	//wxzhc	HC测量值每秒数据(未修正)
	//wxzco	CO测量值每秒数据(未修正)
	//wxzno	NO测量值每秒数据(未修正)
	//hc	HC测量值每秒数据
	//co	CO测量值每秒数据
	//no	NO测量值每秒数据
	//o2	O2测量值每秒数据
	//co2	CO2测量值每秒数据
	//cs	车速每秒数据
	//zs	发动机转速每秒数据
	//xsxzxs	稀释修正系数每秒数据
	//sdxzxs	湿度修正系数每秒数据
	//glkqxs	过量空气系数
	//dpcgjfz	逐秒底盘测功机负载
	//zjzgl	测试工况总加载功率每秒数据
	//jsgl	测试工况寄生功率每秒数据
	//zsgl	测试工况指示功率每秒数据
	//nl	扭力每秒数据
	//fxyglyl	分析仪管路压力
	//wd	测试工况环境温度每秒数据
	//sd	测试工况环境相对湿度每秒数据
	//dqy	测试工况环境大气压力每秒数据
	//yw	油温每秒数据
	static int wtGcsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strqssx, const std::wstring& strgklx,
		 const std::wstring& strjcsxh, const std::wstring& strwxzhc, const std::wstring& strwxzco, const std::wstring& strwxzno, const std::wstring& strhc,
		 const std::wstring& strco, const std::wstring& strno, const std::wstring& strsdxzxs, const std::wstring& strglkqxs, const std::wstring& strdpcgjfz,
		 const std::wstring& strzjzgl, const std::wstring& strjsgl, const std::wstring& strzsgl, const std::wstring& strnl, const std::wstring& strfxyglyl,
		 const std::wstring& strwd, const std::wstring& strsd, const std::wstring& strdqy, const std::wstring& stryw,
		 std::wstring& strRetStr);
	// 5.4.5上传双怠速法检测结果接口
	//lsh	检验流水号
	//accessToken	访问令牌
	//dszs	怠速发动机转速
	//dsyw	怠速油温
	//gdszs	高怠速发动机转速
	//gdsyw	高怠速油温
	//dsco	低怠速CO值
	//dshc	低怠速 HC值
	//gdsco	高怠速CO值
	//gdshc	高怠速 HC值
	//glkqxs	过量空气系数
	//wd	温度
	//sd	湿度
	//dqy	气压
	//kssj	检测开始时间
	//jssj	检测结束时间
	static int sdsJgsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strdszs, const std::wstring& strdsyw,
		 const std::wstring& strgdszs, const std::wstring& strgdsyw, const std::wstring& strdsco, const std::wstring& strdshc,
		 const std::wstring& strgdsco, const std::wstring& strgdshc, const std::wstring& strglkqxs, const std::wstring& strwd,
		 const std::wstring& strsd, const std::wstring& strdqy,
		 const std::wstring& strkssj, const std::wstring& strjssj, std::wstring& strRetStr);
	// 5.4.6上传双怠速法检测过程数据接口
	//lsh	检验流水号
	//accessToken	访问令牌
	//qssx	全程时序
	//gklx	工况类型
	//jcsxh	检测顺序号
	//hc	测试工况[HC]每秒数据
	//co	测试工况[CO]每秒数据
	//co2	测试工况[CO2]每秒数据
	//o2	测试工况分析仪[O2]每秒数据
	//glkqxs	过量空气系数（λ）
	//jyyw	机油油温
	//fdjzs	发动机转速每秒数据或“跳过”测量方式
	//wd	环境温度每秒数据
	//sd	环境相对湿度每秒数据
	//dqy	环境大气压力每秒数据
	static int sdsGcsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strqssx, const std::wstring& strgklx,
		 const std::wstring& strjcsxh, const std::wstring& strhc, const std::wstring& strco, const std::wstring& strco2,
		 const std::wstring& stro2, const std::wstring& strglkqxs, const std::wstring& strjyyw, const std::wstring& strfdjzs,
		 const std::wstring& strwd, const std::wstring& srtsd, const std::wstring& strdqy, std::wstring& strRetStr);
	// 5.4.7上传加载减速法检测结果接口
	//lsh	检验流水号
	//accessToken	访问令牌
	//k100	100%点烟度值
	//k80	80%点烟度值
	//nox80	80%点氮氧值
	//zdlbgl	最大轮边功率
	//zdsd	最大轮边功率对应的速度
	//fdjzs	最大轮边功率对应的发动机转速
	//dszs	发动机怠速转速
	//fdjzssx	发动机额定转速上限
	//fdjzsxx	发动机额定转速下限
	//wd	温度
	//sd	湿度
	//dqy	气压
	//kssj	检测开始时间
	//jssj	检测结束时间
	static int jzjsJgsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strk100, const std::wstring& strk80,
		 const std::wstring& strnox80, const std::wstring& strzdlbgl, const std::wstring& strzdsd, const std::wstring& strfdjzs,
		 const std::wstring& strdszs, const std::wstring& strfdjzssx, const std::wstring& strfdjzsxx, const std::wstring& strwd,
		 const std::wstring& strsd, const std::wstring& strdqy,
		 const std::wstring& strkssj, const std::wstring& strjssj, std::wstring& strRetStr);
	// 5.4.8上传加载减速法检测过程数据接口
	//lsh	检验流水号
	//accessToken	访问令牌
	//qssx	全程时序
	//gklx	工况类型
	//jcsxh	检测顺序号
	//gxsxs	光吸收系数每秒数据
	//ydz	不透光度
	//sscs	实时车速每秒数据
	//fdjzs	发动机转速每秒数据
	//nl	测功机扭力
	//nj	扭距
	//scgl	测功机负荷（实测功率）
	//glxzxs	功率修正系数
	//xzhgl	修正后功率
	//nox	逐秒氮氧化物浓度
	//co2	逐秒CO2浓度
	//wd	环境温度每秒数据
	//sd	环境相对湿度每秒数据
	//dqy	环境大气压力每秒数据
	static int jzjsGcsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strqssx, const std::wstring& strgklx,
		 const std::wstring& strjcsxh, const std::wstring& strgxsxs, const std::wstring& strydz, const std::wstring& strsscs,
		 const std::wstring& strglxzxs, const std::wstring& strxzhgl, const std::wstring& strnox, const std::wstring& strco2,
		 const std::wstring& strwd, const std::wstring& strsd, const std::wstring& strdqy, std::wstring& strRetStr);
	// 5.4.9上传不透光烟度法检测结果接口
	//lsh	检验流水号
	//accessToken	访问令牌
	//ydz1	第一次 烟度值
	//zs1	第一次 转速
	//ydz2	第二次 烟度值
	//zs2	第二次 转速
	//ydz3	第三次 烟度值
	//zs3	第三次 转速
	//dszs	怠速转速
	//wd	温度
	//sd	湿度
	//dqy	气压
	//kssj	检测开始时间
	//jssj	检测结束时间
	static int btgJgsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strydz1, const std::wstring& strzs1,
		 const std::wstring& strydz2, const std::wstring& strzs2, const std::wstring& strydz3, const std::wstring& strzs3,
		 const std::wstring& strdszs, const std::wstring& strwd, const std::wstring& strsd, const std::wstring& strdqy,
		 const std::wstring& strkssj, const std::wstring& strjssj, std::wstring& strRetStr);
	// 5.4.10上传不透光烟度法检测过程数据接口
	//lsh	检验流水号
	//accessToken	访问令牌
	//qssx	全程时序
	//gklx	工况类型
	//jcsxh	检测顺序号
	//ydz	烟度值
	//fdjzs	发动机转速数据
	//yw	机油油温数据
	//wd	环境温度每秒数据
	//sd	环境相对湿度每秒数据
	//dqy	环境大气压力每秒数据
	static int btgGcsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strqssx, const std::wstring& strgklx,
		 const std::wstring& strjcsxh, const std::wstring& strydz, const std::wstring& strfdjzs, const std::wstring& stryw,
		 const std::wstring& strwd, const std::wstring& strsd, const std::wstring& strdqy, std::wstring& strRetStr);
	// 5.4.11上传林格曼黑度法检测结果接口
	//lsh	检验流水号
	//accessToken	访问令牌
	//lgmhd	林格曼黑度
	//fdjzs	黑度最大值对应的发动机转速
	//wd	温度
	//sd	湿度
	//dqy	气压
	//kssj	检测开始时间
	//jssj	检测结束时间
	static int lgmhdJgsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strlgmhd, const std::wstring& strfdjzs,
		 const std::wstring& strwd, const std::wstring& strsd, const std::wstring& strdqy, const std::wstring& strkssj, const std::wstring& strjssj, std::wstring& strRetStr);
	// 5.4.12上传林格曼黑度法检测过程数据接口
	//lsh	检验流水号
	//accessToken	访问令牌
	//qssx	全程时序
	//jcsxh	检测顺序号
	//lgmhd	林格曼黑度值
	//fdjzs	发动机转速数据
	//wd	环境温度每秒数据
	//sd	环境相对湿度每秒数据
	//dqy	环境大气压力每秒数据
	static int lgmhdGcsj(const char * pchURL, const std::wstring& strlsh, const std::wstring& straccessToken, const std::wstring& strqssx, const std::wstring& strjcsxh,
		 const std::wstring& strlgmhd, const std::wstring& strfdjzs,
		 const std::wstring& strwd, const std::wstring& strsd, const std::wstring& strdqy, std::wstring& strRetStr);
};