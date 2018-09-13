//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <limits.h>
#include "hackmap.h"

void StoreConfigVar(ConfigVar *var, int index, short *arrays, char *p)
{
	if (*arrays < 0) index *= -(*arrays++);
	if (*arrays) {
		if (skipspace(p) == '[') {
			char *p5 = ++p;
			while (*p && (*p != ']')) p++;
			if (*p) skipspace(++p);//p++;

			int num1, num2;
			while (strtol2(num1, p5, 0)) {
				num2 = num1;
				if ((skipspace(p5) == '-') || (*p5 == '+') || (*p5 == '.')) {
					do p5++; while (*p5 == '.');
					if (!strtol2(num2, p5, 0)) num2 = INT_MAX;
				}
				if (num1 > num2) SwapInt(num1, num2);
				num2 = min(num2, *arrays);
				for (int j = max(num1, 1); j <= num2; j++) {
					StoreConfigVar(var, index*(*arrays)+j-1, arrays+1, p);
				}
				while (isspace(*p5) || (*p5 == ',')) p5++;
			}
		} else {
			for (int j = 1; j <= *arrays; j++) {
				StoreConfigVar(var, index*(*arrays)+j-1, arrays+1, p);
			}
		}

	} else {
		if (skipspace(p) && ispunct(*p) && !isquote(*p)) p++;

		for (int i = 0; i < var->nSize2; i++) {
			if (var->nSize) {
				int num;
				if (strtol2(num, p, 0)) memcpy((char *)var->pVar+(index*var->nSize2+i)*var->nSize, &num, var->nSize);
			} else if (skipspace(p) && isquote(*p)) {
				char quote = *p++, *p3 = p;
				while ((*p >= ' ') && (*p != quote)) p++;
				AllocStrN(var->pszVar+(index*var->nSize2+i), p3, p-p3);
				if (*p) p++;
			}
			if (skipspace(p) != ',') break; else p++;
		}
	}
}

char *GetNextWord(char *dest, char *&p)
{
	char *p1 = dest, *num;
	if (isspace(*p)) {
		do p++; while (isspace(*p));
		*p1++ = ' ';
	} else if (isquote(*p)) {
		char quote = *p;
		do *p1++ = *p++; while ((*p >= ' ') && (*p != quote));
		if (*p == quote) p++;
		*p1++ = quote;
	} else if (((num = isnumber(p, 0)) != p) && !isalnum(*num)) {
		while (p < num) *p1++ = *p++;
	} else if (isalnum(*p)) {
		do {
			if (!isspace(*p) && (*p != '_')) *p1++ = toupper(*p);
			p++;
			if ((*p == '\'') && (toupper(p[1]) == 'S')) p++;
		} while (isalnum(*p) || isspace(*p) || (*p == '_'));
	} else if (*p && !((*p == '/') && (p[1] == '/'))) {
		*p1++ = *p++;
	}
	*p1 = 0;
	return dest;
}

TreeNode *pTreeRoot = 0;
TreeNode *&SearchTree(TreeNode *&node, char *key)
{
	if (!node) return node;
	int res = strcmp(key, node->key);
	if (res < 0) return SearchTree(node->left, key);
	if (res > 0) return SearchTree(node->right, key);
	return node;
}

void ReplaceVocab(char *dest, int max, char *src)
{
	char temp[1024], *limit = dest+max-1, *p2 = dest;
	*dest = 0;
	TreeNode **node;
	while (*(GetNextWord(temp, src))) {
		char *p = (isalnum(*temp) && *isnumber(temp, 0) && *(node = &SearchTree(pTreeRoot, temp))) ? (*node)->str : temp;
		if (p && (dest+strlen(p) < limit)) dest = strcpy(dest, p)+strlen(p);
	}
	trimspaces(p2);
}

void DeleteTree(TreeNode *&node)
{
	if (!node) return;
	AllocStrN(&node->key, 0, 0);
	AllocStrN(&node->str, 0, 0);
	DeleteTree(node->left);
	DeleteTree(node->right);
	delete node;
}

void LoadConfig(ConfigVar* config, int num)
{
	char line[1024], line2[1024], temp[1024];
	HANDLE hFile;

	hFile = OpenFileReadHelper("VCB");
	while (ReadFileLine(line, sizeof(line), hFile)) {
		char *p = line, *p2 = line;
		TreeNode **pLeaf;
		while (*(GetNextWord(temp, p2)) && (isalnum(*temp) || (*temp == ',') || (*temp == ' '))) ;

		ReplaceVocab(line2, sizeof(line2), p2);
		while (*(GetNextWord(temp, p)) && (isalnum(*temp) || (*temp == ',') || (*temp == ' '))) {
			if (isalnum(*temp) && !*(pLeaf = &SearchTree(pTreeRoot, temp))) {
				*pLeaf = new TreeNode;
				(*pLeaf)->key = AllocStrN(temp, strlen(temp));
				(*pLeaf)->str = AllocStrN(line2, strlen(line2));
				(*pLeaf)->left = (*pLeaf)->right = 0;
			}
		}
	}
	CloseHandle(hFile);

	hFile = OpenFileReadHelper("CFG");
	while (ReadFileLine(line, sizeof(line), hFile)) {
		ReplaceVocab(line2, sizeof(line2), line);
		char *p = line2;
		GetNextWord(temp, p);
		for (int i = 0; i < num; i++) {
			if (!stricmp(config[i].szName1, temp) || (config[i].szName2 && !stricmp(config[i].szName2, temp))) {
				StoreConfigVar(&config[i], 0, config[i].anArrayMax, p);
			}
		}
	}
	CloseHandle(hFile);

	DeleteTree(pTreeRoot);
}

void DeleteConfigVar(ConfigVar *var, int index, short *arrays)
{
	if (*arrays < 0) index *= -(*arrays++);
	if (*arrays) {
		for (int j = 1; j <= *arrays; j++) {
			DeleteConfigVar(var, index*(*arrays)+j-1, arrays+1);
		}
	} else {
		for (int i = 0; i < var->nSize2; i++) {
			AllocStrN(var->pszVar+(index*var->nSize2+i), 0, 0);
		}
	}
}

void DeleteConfig(ConfigVar* config, int num)
{
	for (int i = 0; i < num; i++) {
		if (!config[i].nSize) DeleteConfigVar(&config[i], 0, config[i].anArrayMax);
	}
}
