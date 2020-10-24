#ifndef INTINFO_H
#define INTINFO_H

#include <string>

// 待检队列信息 Waiting queue information
struct SWaitQueInfo
{
	//1.tsno varchar2(15) 检测站编号 
	std::wstring strtsno;
	//2 jylsh varchar2(50) 检验流水号 
	std::wstring strjylsh;
	//3 testtimes number(3) 检验次数 1,2,3 
	std::wstring strtesttimes;
	//4 license varchar2(10) 号牌号码 
	std::wstring strlicense;
	//5 licensecode varchar2(2) 号牌种类 按 GA 24.7 
	std::wstring strlicensecode;
	//6 testtype varchar2(1) 检测方法 附录 3.1 
	std::wstring strtesttype;
	//7 dlsj date 登录时间 yyyy-mm-dd 24h:mi:ss 
	std::wstring strdlsj;
	//8 zt char(1) 状态 0 待外观检测、1 待 OBD 检测、2 待上线检测、3 上线检测中、4 待 燃油蒸发检测
	std::wstring strzt;
};

// 车辆信息
struct SIntVehInfo
{
	//1 jylsh varchar2(50) 检验流水号 检测站编号(8 位)+年月日 (yymmdd 6 位)+当日累计流 水(5 位) 
	std::wstring strjylsh;
	//	2 testtimes number(3) 检验次数 本检验周期内检验次数，免 检为 0 
	std::wstring strtesttimes;
	//	3 license varchar2(20) 号牌号码 鲁 B19928，带首位汉字， 教练车、警车带末位汉字 
	std::wstring strlicense;
	//	4 licensetype number(1) 车牌颜色 附录 3.4 
	std::wstring strlicensetype;
	//	5 licensecode varchar2(2) 号牌种类 按 GA 24.7 
	std::wstring strlicensecode;
	//	6 vehicletype varchar2(3) 车辆类型 按 GA24.4-2005 
	std::wstring strvehicletype;
	//	7 vehiclemodel varchar2(30) 车辆型号 
	std::wstring strvehiclemodel;
	//	8 cpmc varchar2(30) 厂牌名称/商标 如：桑塔纳 
	std::wstring strcpmc;
	//	9 vin varchar2(30) 车辆识别代号 填写 VIN 码或者车架号 
	std::wstring strvin;
	//	10 clscqy varchar2(50) 车辆生产厂 
	std::wstring strclscqy;
	//	11 engine varchar2(30) 发动机型号 
	std::wstring strengine;
	//	12 enginemanuf varchar2(50) 发动机生产厂 
	std::wstring strenginemanuf;
	//	13 fdjh varchar2(20) 发动机编号 
	std::wstring strfdjh;
	//	14 ed number(5,3) 发动机排量 升(L) 
	std::wstring stred;
	//	15 enginespeed number(4) 发动机额定转速 转每分(r/min) 
	std::wstring strenginespeed;
	//	16 enginepower number(4) 发动机额定功率 千瓦(kw)
	std::wstring strenginepower;
	//	17 usetype varchar2(3) 使用性质 GA24.3 
	std::wstring strusetype;
	//	18 registerdate date 注册日期/初次登记日 期 按“YYYYMMDD”格式填写 (yyyy-mm-dd) 
	std::wstring strregisterdate;
	//	19 mdate date 出厂日期/制造日期 按“YYYYMMDD”格式填写 (yyyy-mm-dd) 
	std::wstring strmdate;
	//	20 owner varchar2(150) 车主姓名/单位名称 
	std::wstring strowner;
	//	21 lxdh varchar2(15) 车主电话 
	std::wstring strlxdh;
	//	22 lxdz varchar2(150) 车主地址 
	std::wstring strlxdz;
	//	23 standard number(1) 排放标准阶段 附录 3.2 
	std::wstring strstandard;
	//	24 gear number(1) 变速器型式 附录 3.8 
	std::wstring strgear;
	//	25 airin number(1) 进气方式 1 自然进气 2 涡轮增压 
	std::wstring strairin;
	//	26 fueltype varchar2(3) 燃油种类 可多选，每种按 GA24.9 
	std::wstring strfueltype;
	//	27 fuelsupply number(2) 燃料供给系统型式 附录 3.9 
	std::wstring strfuelsupply;
	//	28 drivemode varchar2(1) 驱动方式 1 前驱 2 后驱 3 四驱 4 全时四驱 
	std::wstring strdrivemode;
	//	29 gvm number(6) 最大总质量 千克（kg） 
	std::wstring strgvm;
	//	30 rm number(6) 基准质量 千克（kg） 
	std::wstring strrm;
	//	31 zbzl number(8) 整备质量 单位 kg 
	std::wstring strzbzl;
	//	32 egr varchar2(1) 是否有 EGR Y/N 表示有/无 
	std::wstring stregr;
	//	33 hcl varchar2(1) 是否有后处理装置 Y/N 表示有/无 
	std::wstring strhcl;
	//	34 hcltype varchar2(1) 后处理种类 附录 3.13 
	std::wstring strhcltype;
	//	35 dk varchar2(1) 是否电控 Y/N 表示有/无 
	std::wstring strdk;
	//	36 odometer number(8) 里程表读数 公里 
	std::wstring strodometer;
	//	37 cylinders number(2) 气缸数 
	std::wstring strcylinders;
	//	38 obd varchar2(1) 是否有 OBD Y/N 表示有/无 
	std::wstring strobd;
	//	39 gbwdzz varchar2(1) 是否能关闭车身稳定 装置(如 ESP) Y/N 表示有/无
	std::wstring strgbwdzz;
	//	40 zt varchar2(1) 机动车状态 按 GA24.17 
	std::wstring strzt;
	//	41 jdcxh varchar2(1) 机动车序号 按 GA 329.2 
	std::wstring strjdcxh;
	//	42 dczz number(8) 单车轴重 单位 kg 
	std::wstring strdczz;
	//	43 dpxh varchar2(20) 底盘型号 
	std::wstring strdpxh;
	//	44 qdltqy number(6,2) 驱动轮胎气压 kPa 
	std::wstring strqdltqy;
	//	45 tg varchar2(1) 是否有燃油蒸发控制 装置 Y/N 表示有/无 
	std::wstring strtg;
	//	46 chzhq varchar2(2) 催化转化器 Y/N 表示有/无 
	std::wstring strchzhq;
	//	47 rygg varchar2(5) 燃油规格 
	std::wstring strrygg;
	//	48 sjcys number(4) 设计成员数 
	std::wstring strsjcys;
	//	49 ssxq varchar2(6) 所属辖区 地区编码 省+市+县(区)， 其它用-1 
	std::wstring strssxq;
	//	50 hbflbz char(1) 环保分类标志 
	std::wstring strhbflbz;
	//	51 gb varchar2(2) 国别 代码（0-国产、1-进口、2- 合资） 
	std::wstring strgb;
	//	52 sfjrcz varchar2(1) 是否进入城镇 0 否 1 是 
	std::wstring strsfjrcz;
	//	53 fdjccs number(8) 发动机冲程数 
	std::wstring strfdjccs;
	//	54 dws number(4) 档位数 
	std::wstring strdws;
	//	55 yxrq date 有效日期 
	std::wstring stryxrq;
	//	56 testtype varchar2(1) 检测方法 附录 3.1 
	std::wstring strtesttype;
	//	57 tsno varchar2(15) 检验机构编号 
	std::wstring strtsno;
	//	58 dlsj date 登录时间 
	std::wstring strdlsj;
	//	59 jclx char(1) 检测类型 附录 3.19 
	std::wstring strjclx;
	//	60 jclry varchar2(50) 检测录入员 
	std::wstring strjclry;
	//	61 pfpdyj varchar2(600) 排放判定依据 
	std::wstring strpfpdyj;
	//	62 rllb varchar2(3) 燃料类别 1：燃油； 2：单一燃料 3：两用燃料 4：混合动力电动 
	std::wstring strrllb;
	//	63 fueltype1 varchar2(3) 燃料种类 2 GA24.9
	std::wstring strfueltype1;
	//	64 ddjxh varchar2(100) 电动机型号 
	std::wstring strddjxh;
	//	65 cnzzxh varchar2(100) 储能装置型号 
	std::wstring strcnzzxh;
	//	66 dcrl number(6,2) 电池容量 kwh 
	std::wstring strdcrl;
	//	67 chzhqxh varchar2(100) 催化转化器型号
	std::wstring strchzhqxh;
	//	68 lambdadown number(6,2) 过量空气系数限值下 限 
	std::wstring strlambdadown;
	//	69 lambdaup number(6,2) 过量空气系数限值上 限 
	std::wstring strlambdaup;
	//	70 hclmodel varchar(30) 后处理装置型号
	std::wstring strhclmodel;
};

















#endif