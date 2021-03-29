#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <thread>
#include <winsock2.h>
using namespace std;

WSADATA wsaData;
SOCKET clientSocket;
SOCKADDR_IN serverAddress;

bool exitFlag = false;
const int serverPort = 9876;
char received[256];

void ShowErrorMessage(string message);
bool InitClient();
void CloseClient();
bool ConnectToServer();
void RecvStr();
void SendStr();

int main()
{
	if (!InitClient()) return 0;
	if(!ConnectToServer()) return 0;

	thread t1(RecvStr);
	thread t2(SendStr);

	t1.join();
	t2.join();
	
	CloseClient();
	return 0;
}

void ShowErrorMessage(string message)
{
	cout << "Error occured! : " << message << '\n';
	system("pause");
	exit(1);
}

bool InitClient()
{
	/* ------ Init Winsock -------------------- */
	cout << "Initialize Winsock2...   ";
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ShowErrorMessage("WSAStartup()");
	cout << "Complete!\n";

	/* ------ Create TCP Socket -------------------- */
	cout << "Create TCP socket...     ";
	clientSocket = socket(PF_INET, SOCK_STREAM, 0); // TCP 소켓을생성합니다.

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");
	cout << "Complete!\n";

	return true;
}

void CloseClient()
{
	closesocket(clientSocket);
	WSACleanup();
	system("pause");
}

bool ConnectToServer()
{
	/* ------ Connect to server ------------------------*/
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // 문자열IP를네트워크바이트형식으로
	serverAddress.sin_port = htons(serverPort); // 2바이트정수네트워크바이트형식으로

	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("connect()");
	cout << "Current State : connect()\n";

	return true;
}

void RecvStr()
{
	while (!exitFlag)
	{
		int length = recv(clientSocket, received, sizeof(received), 0);

		received[length] = '\0';

		if (strcmp(received, "exit") == 0)
		{
			cout << "Server is shutting down...\n";
			break;
		}
		cout << "Server : " << received << '\n';
	}
}

void SendStr()
{
	while (1)
	{
		string sent; 

		cout << "Send Message : ";
		getline(cin, sent);

		if (sent == "") continue;
		else if (sent == "exit")
		{
			exitFlag = true;
			return;
		}

		send(clientSocket, sent.c_str(), sent.length(), 0);
	}
}
