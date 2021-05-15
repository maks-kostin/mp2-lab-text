#include "text.h"

TMem TTextLink::mem;

void main()
{
	TTextLink::InitMem(100);
	TText text;
	int exit = -1;
	text.Read("manuscript.txt");
	text.Print();
	text.GoFirstLink();
	text.GoDownLink();
	while (exit != 0)
	{
		cout << "Print 0 to save changes in origin.txt and close the programm" << endl;
		cout << "Print 1 to go next line" << endl;
		cout << "Print 2 to go down line" << endl;
		cout << "Print 3 to insert next line" << endl;
		cout << "Print 4 to insert down line" << endl;
		cout << "Print 5 to insert next section" << endl;
		cout << "Print 6 to insert down section" << endl;
		cout << "Print 7 to delete next line" << endl;
		cout << "Print 8 to delete down line" << endl;
		cout << "Print 9 to clean" << endl;
		cout << "Print 10 to go first line" << endl;
		cout << "Print 11 to print text" << endl;
		cout << "Print 12 to print free" << endl;
		cin >> exit;
		if (exit == 0)
		{
			text.Save("origin.txt");
			break;
		}
		if (exit == 1)
		{
			text.GoNextLink();
		}
		if (exit == 2)
		{
			text.GoDownLink();
		}
		if (exit == 3)
		{
			char line[80];
			cin >> line;
			text.InsNextLine(line);
			text.Print();
			text.GoFirstLink();
			text.GoDownLink();
		}
		if (exit == 4)
		{
			char line[80];
			cin >> line;
			text.InsDownLine(line);
			text.Print();
			text.GoFirstLink();
			text.GoDownLink();
		}
		if (exit == 5)
		{
			char line[80];
			cin >> line;
			text.InsNextSection(line);
			text.Print();
			text.GoFirstLink();
			text.GoDownLink();
		}
		if (exit == 6)
		{
			char line[80];
			cin >> line;
			text.InsDownSection(line);
			text.Print();
			text.GoFirstLink();
			text.GoDownLink();
		}
		if (exit == 7)
		{
			text.DelNextLine();
			text.Print();
			text.GoFirstLink();
			text.GoDownLink();
		}
		if (exit == 8)
		{
			text.DelDownLine();
			text.Print();
			text.GoFirstLink();
			text.GoDownLink();
		}
		if (exit == 9)
		{
			TTextLink::Clean(text);
		}
		if (exit == 10)
		{
			text.GoFirstLink();
			text.GoDownLink();
		}
		if (exit == 11)
		{
			text.Print();
			text.GoFirstLink();
			text.GoDownLink();
		}
		if (exit == 12)
		{
			TTextLink::PrintFree();
		}

	}
}