#include <iostream>
#include <string>
#include <Windows.h>
#include "chatUI.h"
using namespace std;

void gotoxy(int y, int x)
{
	COORD pos = { (short)x, (short)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void ClearScreen()
{
	for (int i = 0; i < MAX_MESSAGE_CNT; i++)
	{
		gotoxy(i+8, 0);
		cout << BLANK_STR;
	}
}

void UpdateChatInterface(deque<Message> &stringArr)
{
	ClearScreen();

	for (qIter i = stringArr.begin(); i != stringArr.end(); i++)
	{
		gotoxy((int)(i-stringArr.begin())+8, 0);
		if (i->user_idx == ME) cout << "ME : ";
		else if (i->user_idx == OTHER) cout << "OTHER : ";
		else cout << "UNKNOWN : ";
		cout << i->str << '\n';
	}
}
