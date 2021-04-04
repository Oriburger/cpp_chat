#pragma once
#include <deque>
#define ME 1
#define OTHER 0
using namespace std;

struct Message
{
	int user_idx;
	std::string str;
};

typedef deque<Message>::iterator qIter;

const int MAX_MESSAGE_CNT = 20;
const int MAX_MESSAGE_LEN = 40;
const string BLANK_STR(MAX_MESSAGE_LEN + 10, ' ');

void gotoxy(int y, int x);

void ClearScreen();

void UpdateChatInterface(deque<Message>& stringArr);