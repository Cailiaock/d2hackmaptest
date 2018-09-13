//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
class CConfigLoader
{
	struct ConfigInfo
	{
		int lineno;
		std::string linestr;
	};
	typedef std::multimap<std::string, ConfigInfo> ConfigMap;
	BOOL BuildConfigMap(LPCSTR lpFileName, ConfigMap& configMap);
	BOOL FurtherFindInConfig(const ConfigMap& configMap, const std::string& key);
public:
	CConfigLoader(void);
	~CConfigLoader(void);
	BOOL LoadConfig(LPCSTR lpFileName);
	BOOL UpgradeConfig(LPCSTR lpFileName, LPCSTR lpTemplateFileName);
	UINT nBaseTimeSlice;
	UINT nIScanFreq;
	UINT nAutoRevealMapFreq;
	BYTE fAutoLoadHackmap;
	BYTE fScanGameDllsOnly;
	BYTE fAutoRevealMap;
	BYTE fReportIScanInGame;
};

extern CConfigLoader g_config;