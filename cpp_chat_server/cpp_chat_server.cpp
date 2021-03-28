#include <iostream>
#include <string>
#include <winsock2.h>
using namespace std;

void ShowErrorMessage(string message)
{
	cout << "[Error occured] : " << message << '\n';
	system("pause");
	exit(1);
}

int main()
{
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;
	string sent;

	const int serverPort = 9876;
	char received[256];

	/* ------ Init Winsock -------------------- */
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
		ShowErrorMessage("WSAStartup()");
	cout << "Sever is running .. \n";

	/* ------ Create TCP Socket -------------------- */
	serverSocket = socket(PF_INET, SOCK_STREAM, 0); //TCP 소켓 생성, PF_INET? : IPv4 인터넷 프로토콜
	if (serverSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	/* ------ Bind Socket ------------------------*/
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET; //AF_INET? : IPv4 주소 체계 
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); //4byte 정수를 네트워크 바이트 형식으로
												       //INADDR_ANY ? : 서버의 IP주소를 자동으로 찾아서 대입
	serverAddress.sin_port = htons(serverPort); //2byte 정수를 네트워크 바이트 형식으로 

	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("bind()");
	cout << "Current State : bind()\n";

	/* ------ Listen --------------------- */
	if (listen(serverSocket, 5) == SOCKET_ERROR)
		ShowErrorMessage("listen()");
	cout << "Current State : listen()\n";

	/* ------ Accept -------------------- */
	int sizeClientAddress = sizeof(clientAddress);
	
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	cout << "Current State : accept()\n";
	cout << " Client Address : " << clientAddress.sin_addr.s_addr << '\n';

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept()");


	/* ------ Send & Recv ---------------- */
	cout << "---Connection Established---\n";
	while (1)
	{
		getline(cin, sent);
		send(clientSocket, sent.c_str(), sent.length(), 0);

		int length = recv(clientSocket, received, sizeof(received), 0); //수신한 길이를 return
		cout << "Server " << received << '\n';
		received[length] = NULL;
		if (!strcmp(received, "exit"))
		{
			send(clientSocket, received, sizeof(received) - 1, 0);
			cout << "[Current State] Server is shutting down...\n";
			break;
		}
	}

	/* ----- Close Socket & Cleanup WSAC --------- */
	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	system("pause");
	
	return 0;
}