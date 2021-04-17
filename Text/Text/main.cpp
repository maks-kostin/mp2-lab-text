#include "text.h"

TMem TTextLink::mem;

void main()
{
	TTextLink::InitMem(100);
	TText text;
	int exit = -1;
	text.Read("manuscript.txt");
	text.Print();

	while (exit != 0)
	{
		cout << "Print 0 to stop" << endl;
		cout << "Print 1 to read text from manuscript.txt" << endl;
		cout << "Print 2 to write text and save it in origin.txt" << endl;
		cin >> exit;
		if (exit == 0)
			break;
		if (exit == 1)
		{
			TTextLink::InitMem(100);
			TText text;
			text.Read("manuscript.txt");
			text.Print();
		}
		if (exit == 2)
		{
			TText text;
			string buff;
			char str[] = "Try this";
			cin >> buff;
			//cout << buff << endl;
			//cout << str << endl;
			//text.InsNextLine(str);
			//text.Print();
			//text.Save("origin.txt");
		}
	}

	//TTextLink::PrintFree();
	//TTextLink::Clean("E:\Data\CProjects\mp2-lab-text\Text\Text\manuscript.txt");
	//TTextLink::PrintFree();
}