#pragma once
#include <stdlib.h>
#include <string>
#include "tstack.h"
#include <iostream>
#include <fstream>

class TTextLink;
class TText;

struct TMem
{
	TTextLink* pFirst, * pLast, * pFree;
};

class TTextLink
{
public:
	TTextLink* pNext, * pDown;
	char str[80];
	bool flag;
	static TMem mem;
	TTextLink(char* s = NULL, TTextLink* pn = NULL, TTextLink* pd = NULL)
	{
		pNext = pn;
		pDown = pd;
		if (!s)
			str[0] = '\0';
		else
			strcpy_s(str, s);
	}

	static void InitMem(int size = 100);

	static void PrintFree(TText& t);

	static void Clean(TText& t);

	void* operator new(size_t n)
	{
		TTextLink* pC = mem.pFree;
		if (mem.pFree != NULL)
			mem.pFree = pC->pNext;
		return pC;
	}

	void operator delete(void* p)
	{
		TTextLink* pC = (TTextLink*)p;
		pC->pNext = mem.pFree;
		mem.pFree = pC;
	}
};

class TText
{
	TTextLink* pFirst, * pCurr;
	TStack<TTextLink*> st;
	int level;
public:
	void GoFirstLink()
	{
		pCurr = pFirst;
		st.Clear();
	}

	void GoNextLink()
	{
		if (pCurr->pNext != NULL)
		{
			st.Push(pCurr);
			pCurr = pCurr->pNext;
		}
	}

	void GoDownLink()
	{
		if (!st.Empty())
			pCurr = st.Pop();
	}

	void GoPrevLink()
	{
		if (pFirst != pCurr)
			pCurr = st.Pop();
	}

	void InsNextLine(char* s)
	{
		TTextLink* t;
		t = new TTextLink;
		strcpy_s(t->str, s);
		t->pNext = pCurr->pNext;
		t->pDown = NULL;
		pCurr->pNext = t;
	}

	void InsNextSection(char* s)
	{
		TTextLink* t;
		t = new TTextLink(s, NULL, pCurr->pNext);
		pCurr->pNext = t;
	}

	void InsDownSection(char* s)
	{
		TTextLink* t = new TTextLink(s, NULL, pCurr->pDown);
		pCurr->pDown = t;
	}

	void InsDownLine(char* s)
	{
		TTextLink* t = (TTextLink*)(TTextLink::operator new(1));
		strcpy_s(t->str, s);
		t->pNext = pCurr->pDown;
		t->pDown = NULL;
		pCurr->pDown = t;
	}

	void DelNextLine()
	{
		if (pCurr->pNext)
		{
			TTextLink* t = pCurr->pNext;
			pCurr->pNext = t->pNext;
			delete t;
		}
	}

	void DelDownLine()
	{
		if (pCurr->pDown)
		{
			TTextLink* t = pCurr->pDown;
			pCurr->pDown = t->pNext;
			delete t;
		}
	}

	void Print()
	{
		level = 0;
		PrintRec(pFirst);
	}

	void PrintRec(TTextLink* t)
	{
		if (t != NULL)
		{
			for (int i = 0; i < level; i++)
				cout << " ";
			cout << t->str << endl;
			level++;
			PrintRec(t->pDown);
			level--;
			PrintRec(t->pNext);
		}
	}

	void Save(char* fn)
	{
		ofstream osf;
		osf.open(fn);
		SaveRec(pFirst, osf);
		osf.close();
	}

	void SaveRec(TTextLink* t, ofstream& osf)
	{
		if (t != NULL)
		{
			osf << t->str << endl;
		}
		if (t->pDown != NULL)
		{
			osf << '{' << endl;
			SaveRec(t->pDown, osf);
			osf << '}' << endl;
		}
		if (t->pNext != NULL)
		{
			SaveRec(t->pNext, osf);
		}
	}

	void Read(const char* fn)
	{
		ifstream ifs(fn);
		pFirst = ReadRec(ifs);
		ifs.close();
	}

	TTextLink* ReadRec(istream& ifs)
	{
		TTextLink* pF = NULL, * pC = NULL;
		char buff[80];
		while (!ifs.eof())
		{
			ifs.getline(buff, 80, '\n');
			if (buff[0] == '}')
				break;
			else
				if (!pC)
				{
					TTextLink* t = (TTextLink*)(TText::operator new(1));
					strcpy_s(t->str, buff);
					t->pDown = NULL;
					t->pNext = NULL;
					pF = t;
					pC = pF;
				}
				else
				{
					TTextLink* t = (TTextLink*)(TText::operator new(1));
					strcpy_s(t->str, buff);
					t->pDown = NULL;
					t->pNext = NULL;
					pC->pNext = t;
					pC = t;
				}
				/*if (buff[0] == '{')
				{
					pC->pDown = ReadRec(ifs);
				}
				else
				{
					TTextLink* tmp = new TTextLink(buff);
					if (pC == NULL)
					{
						pF = pC = tmp;
					}
					else
					{
						pC->pNext = tmp;
						pC = tmp;
					}
				}*/
		}
		return pF;
	}

	void Reset()
	{
		if (pFirst)
		{
			st.Clear();
			pCurr = pFirst;
			st.Push(pFirst);
			if (pFirst->pNext)
				st.Push(pFirst->pNext);
			if (pFirst->pDown)
				st.Push(pFirst->pDown);
		}
	}

	void GoNext()
	{
		pCurr = st.Pop();
		if (pCurr != pFirst)
		{
			if (pCurr->pNext)
				st.Push(pCurr->pNext);
			if (pCurr->pDown)
				st.Push(pCurr->pDown);
		}
	}

	bool IsEnd()
	{
		return st.Empty();
	}

	void SetCurrFlagTrue()
	{
		pCurr->flag = true;
	}
};

void TTextLink::Clean(TText& t)
{
	TTextLink* tmp = mem.pFree;
	while (tmp != NULL)
	{
		tmp->flag = true;
		tmp = tmp->pNext;
	}
	for (t.Reset(); !t.IsEnd(); t.GoNext())
	{
		t.SetCurrFlagTrue();
	}
	tmp = mem.pFirst;
	while (tmp <= mem.pLast)
	{
		if (tmp->flag)
			tmp->flag = false;
		else
			delete tmp;
	}
}

void TTextLink::InitMem(int size)
{
	int Size = sizeof(TTextLink) * size;
	mem.pFirst = mem.pFree = (TTextLink*)malloc(Size);
	mem.pLast = mem.pFirst + (size - 1); //Size or size?
	TTextLink * tmp = mem.pFirst;
	while (tmp != mem.pLast)
	{
		tmp->pNext = tmp + 1;
		tmp->flag = false;
		tmp->str[0] = '\0';
		tmp = tmp->pNext;
	}
	tmp->pNext = NULL;
	tmp->flag = false;
}

void TTextLink::PrintFree(TText& t)
{
	TTextLink* tmp = mem.pFree;
	while (tmp != mem.pLast)
	{
		if (tmp->str[0] != '\0')
			std::cout << tmp->str << ' ';
		tmp = tmp->pNext;
	}
	if (tmp->str[0] != '\0')
		std::cout << tmp->str << ' ';
}
