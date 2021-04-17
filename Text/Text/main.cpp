#include "text.h"

TMem TTextLink::mem;

void main()
{
	TTextLink::InitMem(100);
	TText text;
	text.Read("E:\Data\CProjects\mp2-lab-text\Text\Text\manuscript.txt");
	text.Print();
	//TTextLink::PrintFree();
	//TTextLink::Clean("E:\Data\CProjects\mp2-lab-text\Text\Text\manuscript.txt");
	//TTextLink::PrintFree();
}