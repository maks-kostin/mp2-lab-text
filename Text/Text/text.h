#pragma once
#include <fstream>
#include <iostream>
#include<stdio.h>
#include "tstack.h"

class TText;
class TTextLink;
struct TMem
{
	TTextLink* pFirst, * pLast, * pFree;
};

class TTextLink
{
public:
	bool flag;
	TTextLink* pNext, * pDown;
	char str[80];
	static TMem mem;
	TTextLink(char* s = NULL, TTextLink* pn = NULL, TTextLink* ph = NULL);
	void* operator new (std::size_t n);
	void operator delete (void* p);
	static void InitMem(int size = 100);
	static void PrintFree();
	static void Clean(TText& t);
};



class TText
{
	int level;
	TTextLink* pFirst;
	TTextLink* pCurr;
	TStack<TTextLink*> st;
	//static TMem mem;
public:
	void SetCurrFlagTrue()
	{
		pCurr->flag = true;
	}
	void GoFirstLink()
	{
		pCurr = pFirst;
		if (!st.Empty())
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
		if (pCurr->pDown != NULL)
		{
			st.Push(pCurr);
			pCurr = pCurr->pDown;
		}
	}
	void GoPrevLink()
	{
		if (!st.Empty())
		{
			pCurr = st.Top();
			st.Pop();
		}
	}
	void InsNextLine(char* s);
	void InsNextSection(char* s);
	void InsDownLine(char* s);
	void InsDownSection(char* s);
	void DelNextLine();
	void DelDownLine();
	void Print();
	void PrintRec(TTextLink* t);
	void Save(const char* fn);
	void SaveRec(TTextLink* t, std::ofstream& osf);
	void Read(const char* fn);
	TTextLink* ReadRec(std::ifstream& ifs);
	void Reset();
	void GoNext();
	bool IsEnd();
};

TTextLink::TTextLink(char* s, TTextLink* pn, TTextLink* ph)
{
	if (s == NULL)
		str[0] = '\0';
	else
	{
		int i = 0;
		do
		{
			str[i] = s[i];
			i++;
		} while (s[i - 1] != '\0');
	}
	pNext = pn;
	pDown = ph;
}

void* TTextLink::operator new (std::size_t n)
{
	TTextLink* pC = mem.pFree;
	if (mem.pFree->pNext != NULL)
	{
		mem.pFree = mem.pFree->pNext;
	}
	return pC;
}

void TTextLink::operator delete (void* p)
{
	TTextLink* pC = (TTextLink*)p;
	pC->pNext = mem.pFree;
	mem.pFree = pC;
}

void TTextLink::InitMem(int size)
{
	int esize = sizeof(TTextLink) * size;
	mem.pFirst = mem.pFree = (TTextLink*)malloc(esize);
	mem.pLast = mem.pFirst + (size - 1);
	TTextLink* tmp = mem.pFree;
	while (tmp != mem.pLast)
	{
		tmp->pNext = tmp + 1;
		tmp->flag = false;
		tmp->str[0] = '\0';
		tmp = tmp->pNext;
	}
	tmp->pNext = NULL;
	tmp->flag = true;
	tmp->str[0] = '\0';
}

void TTextLink::Clean(TText& t)
{
	TTextLink* tmp = mem.pFree;
	while (tmp != mem.pLast)
	{
		tmp->flag = true;
		tmp = tmp->pNext;
	}
	tmp->flag = true;//?
	for (t.Reset(); !t.IsEnd(); t.GoNext())
	{
		//t.pCurr->flag = 1;
		t.SetCurrFlagTrue();
	}
	tmp = mem.pFirst;
	while (tmp <= mem.pLast)
	{
		if (tmp->flag)
			tmp->flag = false;
		else
			delete tmp;
		tmp++;
	}
	if (tmp->flag)
		tmp->flag = false;
	else
		delete tmp;
}

void TTextLink::PrintFree()
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
	std::cout << std::endl;
}

void TText::InsNextLine(char* s)
{
	TTextLink* t = (TTextLink*)TTextLink::operator new(1);
	strcpy_s(t->str, s);
	t->pNext = pCurr->pNext;
	t->pDown = NULL;
	pCurr->pNext = t;
}

void TText::InsNextSection(char* s)
{
	TTextLink* t = (TTextLink*)(TTextLink::operator new(1));
	strcpy_s(t->str, s);
	t->pNext = NULL;
	t->pDown = pCurr->pNext;
	pCurr->pNext = t;
}

void TText::InsDownLine(char* s)
{
	TTextLink* t = (TTextLink*)(TTextLink::operator new(1));
	strcpy_s(t->str, s);
	t->pNext = pCurr->pDown;
	t->pDown = NULL;
	pCurr->pDown = t;
}

void TText::InsDownSection(char* s)
{
	TTextLink* t = (TTextLink*)(TTextLink::operator new(1));
	strcpy_s(t->str, s);
	t->pNext = pCurr->pDown;
	t->pDown = NULL;
	pCurr->pDown = t;
}

void TText::DelNextLine()
{
	if (pCurr->pNext)
	{
		TTextLink* t = pCurr->pNext;
		pCurr->pNext = t->pNext;
		TTextLink::operator delete(t);
	}
}

void TText::DelDownLine()
{
	if (pCurr->pDown)
	{
		TTextLink* t = pCurr->pDown;
		pCurr->pDown = t->pNext;
		TTextLink::operator delete(t);
	}
}

void TText::Print()
{
	level = 0;
	PrintRec(pFirst);
}

void TText::PrintRec(TTextLink* t)
{
	if (t != NULL)
	{
		for (int i = 0; i < level; i++)
			std::cout << ' ';
		std::cout << t->str << '\n';
		level++;
		PrintRec(t->pDown);
		level--;
		PrintRec(t->pNext);
	}
}

void TText::Save(const char* fn)
{
	std::ofstream osf;
	osf.open(fn);
	SaveRec(pFirst, osf);
	osf.close();
}

void TText::SaveRec(TTextLink* t, std::ofstream& osf)
{
	if (t != NULL)
	{
		osf << t->str << std::endl;
		if (t->pDown != NULL)
		{
			osf << '{' << std::endl;
			SaveRec(t->pDown, osf);
			osf << '}' << std::endl;
		}
		if (t->pNext != NULL)
			SaveRec(t->pNext, osf);
	}
}

void TText::Read(const char* fn)
{
	std::ifstream ifs(fn);
	pFirst = ReadRec(ifs);
	ifs.close();
}

TTextLink* TText::ReadRec(std::ifstream& ifs)
{
	TTextLink* pF = NULL, * pC = NULL;
	char Buff[80];
	while (ifs.eof() == false)
	{
		ifs.getline(Buff, 80, '\n');
		if (Buff[0] == '}')
			break;
		else
			if (Buff[0] == '{')
			{
				pC->pDown = ReadRec(ifs);
			}
			else
			{
				if (pC == NULL)
				{
					TTextLink* tmp = (TTextLink*)TTextLink::operator new(1);
					strcpy_s(tmp->str, Buff);
					tmp->pDown = NULL;
					tmp->pNext = NULL;
					pF = pC = tmp;
				}
				else
				{
					TTextLink* tmp = (TTextLink*)TTextLink::operator new(1);
					strcpy_s(tmp->str, Buff);
					tmp->pDown = NULL;
					tmp->pNext = NULL;
					pC->pNext = tmp;
					pC = tmp;
				}
			}
	}
	return pF;
}

void TText::Reset()
{
	if (pFirst != NULL)
	{
		st.Clear();
	}
	pCurr = pFirst;
	st.Push(pFirst);
	if (pFirst->pNext)
		st.Push(pFirst->pNext);
	if (pFirst->pDown)
		st.Push(pFirst->pDown);
}

void TText::GoNext()
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

bool TText::IsEnd()
{
	return st.Empty();
}