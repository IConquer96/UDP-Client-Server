
#ifndef UNICODE
#define UNICODE
#endif
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN



#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <Windows.h>
#include <winsock2.h>

#pragma comment (lib, "Ws2_32.lib")

using namespace std;

int main()
{
	//inicjalizacja zmiennych


	static char IP[] = "127.0.0.1"; //host IP
	unsigned short PORT = 8000;
	WORD versionRequested = MAKEWORD(2, 2); //wersja WinSock

	cout << "-----UDP CLIENT----- " << endl;

	WSADATA WinSockData;
	int iWsaStartup;
	int iWsaCleanup;

	SOCKET UDPSocketClient;
	struct sockaddr_in UDPServer;

	char Buffer[512] = "Wiadomosc do wyslania";  //wiadomosc do przeslania 
	int iSendTo;
	
	int iBuffferLength = strlen(Buffer) + 1; //dlugosc buffera 
	int iUDPServerLength = sizeof(UDPServer);
	int iCloseSocket;

	//--------------------------------
	//inicjalizacja WinSock

	iWsaStartup = WSAStartup(versionRequested, &WinSockData);//wersja winsock

	if (iWsaStartup != 0) {
		cout << "Inicjalizacja WinSock nie powiodla sie " << endl;
	}
	cout << "Inicjalizacja WinSock powiodla sie " << endl;
	//------------------------------
	//konfiguracja gniazda------------
	UDPServer.sin_family = AF_INET;
	UDPServer.sin_addr.s_addr = inet_addr(IP);
	UDPServer.sin_port = htons(PORT);
	//--------------------------------
	//stworzenie gniazda ---------------
	UDPSocketClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //protokó³ UDP
	if (UDPSocketClient == INVALID_SOCKET) {
		cout << "Blad w tworzeniu gniazda o numerze " << WSAGetLastError() << endl;
	}
	cout << "Utworzenie gniazda pomyslne " << endl;
	//---------------------------
	//wysylanie wiadomosic do serwera
	iSendTo = sendto(
		UDPSocketClient,
		Buffer,
		iBuffferLength,
		MSG_DONTROUTE,
		(SOCKADDR*)&UDPServer,
		sizeof(UDPServer));
	//---------------------------d

	if (iSendTo == SOCKET_ERROR) {
		cout << "Blad wysylania wiadomosci o numerze " << WSAGetLastError() << endl;
	}
	cout << "Wiadomosc wyslana pomyslne " << endl;
	//---------------------
	//zamkniecie gniazda
	iCloseSocket = closesocket(UDPSocketClient);

	if (iCloseSocket == SOCKET_ERROR) {
		cout << "Blad zamkniecia gniazda o numerze " << WSAGetLastError() << endl;

	}
	cout << "Gniazdo zamknieto pomyslnie " << endl;

	//----------------
	//zamkniecie WinSock
	iWsaCleanup = WSACleanup();

	if (iWsaCleanup == SOCKET_ERROR) {
		cout << "Blad zamkniecia WinSock o numerze " << WSAGetLastError() << endl;

	}
	cout << "WinSock zamknieto pomyslnie " << endl;

	system("PAUSE");
	return 0;

	//----------------
}
//.server