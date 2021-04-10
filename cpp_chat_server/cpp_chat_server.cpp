#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <thread>
#include <winsock2.h>
#include "chatUI.h"

SOCKET serverSocket, clientSocket;
SOCKADDR_IN serverAddress, clientAddress;

bool exitFlag = false;
const int serverPort = 9876;
deque<Message> stringArr;

void ShowErrorMessage(const char * msg);
bool InitServer();
void ConnectClient();
void CloseServer();
void RecvStr();
void SendStr();

int main()
{
	if (!InitServer()) return -1;

	/* ------ Send & Recv ---------------- */
	thread t1(RecvStr);
	thread t2(SendStr);

	t1.join();
	t2.join();

	CloseServer();
	
	return 0;
}

void ShowErrorMessage(const char * msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

bool InitServer()
{
	// ------ Init Winsock --------------------
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
		ShowErrorMessage("WSAStartup()");
	cout << "Server is running .. \n";

	// ------ Create TCP Socket --------------------
	serverSocket = socket(PF_INET, SOCK_STREAM, 0); //TCP 소켓 생성, PF_INET? : IPv4 인터넷 프로토콜
	if (serverSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	// ------ Bind Socket ------------------------
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET; //AF_INET? : IPv4 주소 체계 
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); //4byte 정수를 네트워크 바이트 형식으로
													   //INADDR_ANY ? : 서버의 IP주소를 자동으로 찾아서 대입
	serverAddress.sin_port = htons(serverPort); //2byte 정수를 네트워크 바이트 형식으로 

	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("bind()");
	cout << "Current State : bind()\n";

	ConnectClient();

	return true;
}

void CloseServer()
{
	/* ----- Close Socket & Cleanup WSAC --------- */
	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	system("pause");
}

void ConnectClient()
{
	// ------ Listen --------------------- 
	if (listen(serverSocket, 5) == SOCKET_ERROR)
		ShowErrorMessage("listen()");
	cout << "Current State : listen()\n";

	// ------ Accept -------------------- 
	int sizeClientAddress = sizeof(clientAddress);

	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	cout << "Current State : accept()\n";
	cout << " Client Address : " << clientAddress.sin_addr.s_addr << '\n';

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept()");

	cout << "---Connection Established---\n";
}

void RecvStr()
{
	while (1)
	{
		gotoxy(MAX_MESSAGE_CNT + 10, 15);
		if (exitFlag) break;

		char received[256];
		memset(received, 0, sizeof(received));
		int length = recv(clientSocket, received, sizeof(received), 0); //수신한 길이를 return

		received[length] = NULL;
		stringArr.push_back({ OTHER, (string)received });
		if (stringArr.size() > MAX_MESSAGE_CNT)	stringArr.pop_front();
		UpdateChatInterface(stringArr);
	}
}

void SendStr()
{
	while (1)
	{
		string sent;
		gotoxy(MAX_MESSAGE_CNT + 10, 0);

		cout << "Send Message : ";
		getline(cin, sent);

		if (sent == "") continue;
		else if (sent == "exit")
		{
			exitFlag = true;
			return;
		}
		send(clientSocket, sent.c_str(), sent.length(), 0);

		stringArr.push_back({ ME, sent });
		if (stringArr.size() > MAX_MESSAGE_CNT)	stringArr.pop_front();
		UpdateChatInterface(stringArr);
		gotoxy(MAX_MESSAGE_CNT + 10, 0);
		cout << BLANK_STR;
	}
}