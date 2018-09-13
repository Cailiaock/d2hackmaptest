//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include "hackmap.h"

//misc helpers

char isquote(char c)
{
	return ((c) == '\'' || (c) == '"' || (c) == '`') ? c : 0;
}

char *AllocStrN(char *str, int len)
{
	if (len <= 0) return 0;
	char *newstr = new char[len+1];
	memcpy(newstr, str, len);
	newstr[len] = 0;
	return newstr;
}

void AllocStrN(char **pstr, char *str, int len)
{
	if (*pstr) delete *pstr;
	*pstr = AllocStrN(str, len);
}

int strtol2(int &num, char *&str, int base)
{
	char *p = str;
	num = strtol(str, &str, base);
	return (p != str);
}

char *isnumber(char *str, int base)
{
	strtol(str, &str, base);
	return str;
}

void SwapInt(int &num1, int &num2)
{
	int temp = num1;
	num1 = num2;
	num2 = temp;
}

void *memcpy2(void *dest, const void *src, size_t count)
{
	return (char *)memcpy(dest, src, count)+count;
}

int strnicmp2(const char *str1, const char *str2, size_t count)
{
	int res = strnicmp(str1, str2, count);
	return res ? res : (strlen(str1) > count);
}

char skipspace(char *&p)
{
	while (isspace(*p)) p++;
	return *p;
}

wchar_t * __cdecl wsprintfW2(wchar_t *dest, char *fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	int len = wvsprintf((char *)dest, fmt, va);
	for (int i = len; i >= 0; i--) {
		dest[i] = ((char *)dest)[i];
	}
	return dest;
}

void wcscpy2(wchar_t *dest, char *src)
{
	do { *dest++ = *src; }while(*src++);
}

wchar_t * wcsrcat(wchar_t *dest, wchar_t *src)
{
	memmove(dest+wcslen(src), dest, (wcslen(dest)+1)*sizeof(wchar_t));
	memcpy(dest, src, wcslen(src)*sizeof(wchar_t));
	return dest;
}

void trimspaces(char *str)
{
	char *p1 = str, *p2 = str+strlen(str);
	while (isspace(*p1)) p1++;
	do p2--; while ((p2 >= p1) && isspace(*p2));
	*++p2 = 0;
	memmove(str, p1, (p2+1-p1)*sizeof(char));
}

void trimspaces(wchar_t *str)
{
	wchar_t *p1 = str, *p2 = str+wcslen(str);
	while (iswspace(*p1)) p1++;
	do p2--; while ((p2 >= p1) && iswspace(*p2));
	*++p2 = 0;
	memmove(str, p1, (p2+1-p1)*sizeof(wchar_t));
}

//file helpers

HANDLE OpenFileRead(char *filename)
{
	return CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
}

HANDLE OpenFileWrite(char *filename)
{
	return CreateFile(filename, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
}

DWORD WriteFile(HANDLE hFile, void *buf, DWORD len)
{
	DWORD numdone = 0;
	WriteFile(hFile, buf, len, &numdone, NULL);
	return numdone;
}

DWORD ReadFile(HANDLE hFile, void *buf, DWORD len)
//NOTE :- validates len bytes of buf
{
	DWORD numdone = 0;
	ReadFile(hFile, buf, len, &numdone, NULL);
	return numdone;
}

char *ReadFileLine(char *str, int len, HANDLE hFile)
{
	char ch, *p = str;
	while (--len) {
		if (!ReadFile(hFile, &ch, 1)) break;
		if ((*p++ = ch) == '\n') break;
	}
	*p = 0;
	return str[0]?str:0;
}

BYTE *AllocReadFile(char *filename)
{
	HANDLE hFile = OpenFileRead(filename);
	int filesize = GetFileSize(hFile, 0);
	if (filesize <= 0) return 0;
	BYTE *buf = new BYTE[filesize];
	ReadFile(hFile, buf, filesize);
	CloseHandle(hFile);
	return buf;
}

char *GetMyFileNameStrrchr(char *dest, char ch)
{
	GetModuleFileName(hInstDLL, dest, MAX_PATH);
	return strrchr(dest, ch)+1;
}

HANDLE OpenFileReadHelper(char *postfix)
{
	char filename[MAX_PATH];
	strcpy(GetMyFileNameStrrchr(filename, '.'), postfix);
	HANDLE hFile = OpenFileRead(filename);
	if (hFile == INVALID_HANDLE_VALUE) {
		const char *szDefaultFileName[] = {"d2maphack.", "maphack.", "d2hackmap.", "hackmap."};
		for (int i = 0; i < ARRAYSIZE(szDefaultFileName); i++) {
			strcpy(GetMyFileNameStrrchr(filename, '\\'), szDefaultFileName[i]);
			strcat(filename, postfix);
			hFile = OpenFileRead(filename);
			if (hFile != INVALID_HANDLE_VALUE) {
				break;
			}
		}
	}
	return hFile;
}

