#pragma once
#include <string>

class CZipLib
{
public:
	CZipLib(void);
	~CZipLib(void);

public:
	std::string ZipData(const std::string &strData);
	std::string UnZipData(const std::string &strZip);
};

