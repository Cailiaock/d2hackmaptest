#include <string>

#ifndef UNICODE
	typedef std::string tstring;
#else
	typedef std::wstring tstring;
#endif
