//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#include "stdhdrs.h"
#include <string>
#include <fstream>
#include <algorithm>
#include <map>
#include "configloader.h"

using std::string;
enum eType {eNumber, eString};
CConfigLoader g_config;
struct ConfigVar
{
	const char *szName;
	void* addr;
	int len;
	eType type;
} aConfigVars[] = {
	{"AutoLoadHackMapDll", &g_config.fAutoLoadHackmap, sizeof(g_config.fAutoLoadHackmap), eNumber},
	{"ScanGameDllsOnly", &g_config.fScanGameDllsOnly, sizeof(g_config.fScanGameDllsOnly), eNumber},
	{"AutoRevealMap", &g_config.fAutoRevealMap, sizeof(g_config.fAutoRevealMap), eNumber}
};

static char *isnumber(char *str, int base)
{
	strtol(str, &str, base);
	return str;
}

static char isquote(char c)
{
	return ((c) == '\'' || (c) == '"' || (c) == '`') ? c : 0;
}

static void trimspaces(char *str)
{
	char *p1 = str, *p2 = str+strlen(str);
	for( char *p = str; p < p2; p++)
	{
		if ( (*p < 0 || !isspace(*p)) && (*p != '_'))
			*p1++ = *p;
	}
	*p1 = '\0';
}

static int strtol2(int &num, const char *str, int base)
{
	char *p;
	num = strtol(str, &p, base);
	return (p != str);
}

static void trimspaces(string& str)
{
	char temp[1024];
	int size = str.length() < COUNTOF(temp)-1 ? str.length()+1 : COUNTOF(temp)-1;
	memcpy(temp, str.c_str(), size);
	temp[COUNTOF(temp)-1] = '\0';
	trimspaces(temp);
	str = temp;
}

CConfigLoader::CConfigLoader(void)
			:nBaseTimeSlice(1000)
			,nIScanFreq(10)
			,nAutoRevealMapFreq(5)
			,fAutoLoadHackmap(0)
			,fScanGameDllsOnly(1)
			,fAutoRevealMap(0)
			,fReportIScanInGame(0)
{
}

CConfigLoader::~CConfigLoader(void)
{
}

void StoreVar(ConfigVar& var, const string& val)
{
	int num;
	switch(var.type)
	{
	case eNumber:
		if(strtol2(num, val.c_str(), 10))
			memcpy(var.addr, &num, var.len);
		break;
	case eString:
	default:
		break;
	}
}

BOOL CConfigLoader::LoadConfig(LPCSTR lpFileName)
{
	std::ifstream istr(lpFileName);
	if(!istr) return FALSE;
	string line, param, value;
	while( std::getline( istr, line ) )
	{
		trimspaces(line);

		line = line.substr(0, line.find("//"));
		if( line.empty() ) continue;

		size_t pos = line.find( ':' );
		if( pos == std::string::npos ) continue;
		param = line.substr( 0, pos );
		value = line.substr( pos+1 );
		if(param.empty() || value.empty()) continue;

		for(int i = 0; i < COUNTOF(aConfigVars); i++)
		{
			if(stricmp(param.c_str(), aConfigVars[i].szName) == 0)
			{
				StoreVar(aConfigVars[i], value);
			}
		}
	}
	return TRUE;
}

BOOL CConfigLoader::FurtherFindInConfig(const ConfigMap& configMap, const std::string& key)
{
	size_t pos = key.find( '[' );
	if(pos == string::npos) return FALSE;
	std::string skey = key.substr(0, pos);
	ConfigMap::const_iterator tit = configMap.begin();
	ConfigMap::const_iterator tend = configMap.end();
	for(; tit != tend; ++tit)
	{
		if(tit->first.compare(0, pos, skey) == 0)
		{
			return TRUE;
		}
	}
		
	return FALSE;
}

BOOL CConfigLoader::UpgradeConfig(LPCSTR lpFileName, LPCSTR lpTemplateFileName)
{
	ConfigMap configMap, configTempMap;
	BuildConfigMap(lpFileName, configMap);
	if(!BuildConfigMap(lpTemplateFileName, configTempMap))
		return FALSE;
	std::ofstream ostr(lpFileName, std::ios_base::out|std::ios_base::app);
	if(!ostr) return FALSE;

	ConfigMap::const_iterator tit = configTempMap.begin();
	ConfigMap::const_iterator tend = configTempMap.end();
	ConfigMap::iterator end = configMap.end();
	BOOL bFirstAdded = TRUE;
	for(; tit != tend; ++tit)
	{
		if(configMap.find(tit->first) == end && !FurtherFindInConfig(configMap, tit->first))
		{
			if(bFirstAdded)
			{
				bFirstAdded = FALSE;
				ostr << "\n// The following are Added by d2hackmap upgrading tool\n\n";
			}
			ostr << tit->second.linestr;
		}
	}
	if(!bFirstAdded)
	{
		ostr << "\n// End of upgraded Content\n";
	}
	
	return TRUE;
}

BOOL CConfigLoader::BuildConfigMap(LPCSTR lpFileName, ConfigMap& configMap)
{
	std::fstream istr(lpFileName);
	if(!istr) return FALSE;
	int lineno;
	string line, key, temp;
	
	for( lineno = 0; std::getline( istr, line ); lineno++ )
	{
		temp += line;
		temp += '\n';
		trimspaces(line);
		
		line = line.substr(0, line.find("//"));
		if( line.empty() ) continue;
		
		size_t pos = line.find( ':' );
		if( pos == std::string::npos ) continue;
		key = line.substr( 0, pos );
		if(key.empty()) continue;
		ConfigInfo ci;
		ci.lineno = lineno;
		ci.linestr = temp;
		configMap.insert(ConfigMap::value_type(key, ci));
		temp = "";
	}
	return TRUE;
}
