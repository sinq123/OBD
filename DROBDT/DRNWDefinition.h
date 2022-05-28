#ifndef DRNWDefinition_H
#define DRNWDefinition_H

#include <string>

struct SOBDVEHLIST
{
	// 外观检查编号（唯一）
	std::wstring strRunningNumber;
	// 号牌号码
	std::wstring strPaletNumber;
	// 号牌种类名称
	std::wstring strPlateType;
	// 车架号
	std::wstring strVIN;
	// 车辆类别
	std::wstring strVehType;
};

#endif