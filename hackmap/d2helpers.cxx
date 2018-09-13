//d2 helpers

#define D2TXTCODE(x) ( ((x)&0xff)<<24 | ((x)>>8&0xff)<<16 | ((x)>>16&0xff)<<8 | ((x)>>24&0xff) )


DWORD D2GetTextFileNo(wchar_t *str)
{
	DWORD width, fileno;
	D2WIN_GetTextWidthFileNo(str, &width, &fileno);
	return fileno;
}

DWORD D2GetTextWidth(wchar_t *str)
{
	DWORD width, fileno;
	D2WIN_GetTextWidthFileNo(str, &width, &fileno);
	return width;
}

void D2DrawText(DWORD size, wchar_t *str, int xpos, int ypos, DWORD col)
{
	DWORD oldsize = D2WIN_SetTextSize(size);
	D2WIN_DrawText(str, xpos, ypos, col, 0);
	D2WIN_SetTextSize(oldsize);
}

void D2DrawCenterText(DWORD size, wchar_t *str, int xpos, int ypos, DWORD col, int div = 1)
{
	DWORD oldsize = D2WIN_SetTextSize(size);
	D2WIN_DrawText(str, xpos-(D2GetTextWidth(str)>>div), ypos, col, 0);
	D2WIN_SetTextSize(oldsize);
}


wchar_t *ConvertToD2String(wchar_t *p, BYTE *str, BYTE normcol, wchar_t *pe)
{
	BYTE ch = 0, col = 0, last;
	
	for (last = ch, ch = *str++; ch && p < pe-7; last = ch, ch = *str++) {
		if (last == '%') {
			*p++ = 0xff;
			*p++ = 'c';
			col = ch = isalpha(ch) ? toupper(ch)-'A'+':' : ch;
		}
		if (ch != '%') *p++ = ch;
	}
	
	if (col && normcol != -1) {
		*p++ = 0xff;
		*p++ = 'c';
		*p++ = '0'+normcol;
	}
	
	*p = 0;
	return p;
}

wchar_t * __fastcall ColourD2String(wchar_t *str, DWORD col)
{
	memmove(str+3, str, (wcslen(str)+1)*sizeof(wchar_t));
	str[0] = 0xff;
	str[1] = 'c';
	str[2] = '0'+(int)col;
	return str;
}

wchar_t * __cdecl wsprintfW2Colour(wchar_t *dest, int col, char *fmt, ...)
{
	dest[0] = 0xff;
	dest[1] = 'c';
	dest[2] = '0'+(int)col;
	dest += 3;
	va_list va;
	va_start(va, fmt);
	int len = wvsprintf((char *)dest, fmt, va);
	for (int i = len; i >= 0; i--) {
		dest[i] = ((char *)dest)[i];
	}
	return dest;
}

void D2ExitGame(DWORD flag)
{
	D2CLIENT_SetExitApp(flag);
	SendMessage(D2GFX_GetHwnd(), WM_CLOSE, 0, 0);
}

CellFile *LoadBmpCellFile(BYTE *buf1, int width, int height)
{
	BYTE *buf2 = new BYTE[(width*height*2)+height], *dest = buf2;

	for (int i = 0; i < height; i++) {
		BYTE *src = buf1+(i*((width+3)&-4)), *limit = src+width;
		while (src < limit) {
			BYTE *start = src, *limit2 = min(limit, src+0x7f), trans = !*src;
			do src++; while ((trans == (BYTE)!*src) && (src < limit2));
			if (!trans || (src < limit)) *dest++ = (trans?0x80:0)+(src-start);
			if (!trans) while (start < src) *dest++ = *start++;
		}
		*dest++ = 0x80;
	}

	static DWORD dc6head[] = {6, 1, 0, 0xeeeeeeee, 1, 1, 0x1c,  0, -1, -1, 0, 0, 0, -1, -1};
	dc6head[8] = width;
	dc6head[9] = height;
	BYTE *ret = new BYTE[dc6head[13] = sizeof(dc6head)+(dc6head[14] = dest-buf2)+3];
	memset(memcpy2(memcpy2(ret, dc6head, sizeof(dc6head)), buf2, dc6head[14]), 0xee, 3);
	delete buf2;

	return (CellFile *)ret;
}

CellFile *LoadBmpCellFile(char *filename)
{
	BYTE *ret = 0;
		
	BYTE *buf1 = AllocReadFile(filename);
	BITMAPFILEHEADER *bmphead1 = (BITMAPFILEHEADER *)buf1;
	BITMAPINFOHEADER *bmphead2 = (BITMAPINFOHEADER *)(buf1+sizeof(BITMAPFILEHEADER));
	if (buf1 && (bmphead1->bfType == 'MB') && (bmphead2->biBitCount == 8) && (bmphead2->biCompression == BI_RGB)) {
		ret = (BYTE *)LoadBmpCellFile(buf1+bmphead1->bfOffBits, bmphead2->biWidth, bmphead2->biHeight);
	}
	delete buf1;

	return (CellFile *)ret;
}

CellFile *InitCellFile(CellFile *cf)
{
	if (cf) D2CMP_InitCellFile(cf, &cf, "?", 0, -1, "?");
	return cf;
}

void DeleteCellFile(CellFile *cf)
{
	if (cf) {
		D2CMP_DeleteCellFile(cf);
		delete cf;
	}
}

inline int D2GetScreenSizeX()
{
	static int nums[2] = {1280,720};
	return nums[0];// ? 800 : 640;
	//return nums[D2GFX_GetScreenSize()/2];// ? 800 : 640;
}

inline int D2GetScreenSizeY()
{
	static int nums[2] = {1280,720-16};
	//return nums[D2GFX_GetScreenSize()/2];// ? 600 : 480;
	return nums[1];// ? 600 : 480;
}

inline int D2GetScreenOffsX()
{
	static int nums[2] = {0,80};
	return nums[D2GFX_GetScreenSize()/2];// ? 80 : 0;
}

inline int D2GetScreenOffsY()
{
	static int nums[2] = {0,60};
	return nums[D2GFX_GetScreenSize()/2];// ? 60 : 0;
}
