
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
							
	cout << "-----UDP SERWER----- " << endl;

	WSADATA WinSockData;
	int iWsaStartup;
	int iWsaCleanup;

	SOCKET UDPSocketServer;
	struct sockaddr_in UDPClient;

	char Buffer[512]; //wiadomosc do przeslania 
	int iBuffferLength = strlen(Buffer) +1; //dlugosc buffera 

	int iBind;
	int iReceiveFrom;

	int iUDPClient = sizeof(UDPClient);
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
	UDPClient.sin_family = AF_INET;
	UDPClient.sin_addr.s_addr = inet_addr(IP);
	UDPClient.sin_port = htons(PORT);
	//--------------------------------
	//stworzenie gniazda ---------------
	UDPSocketServer = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //protokó³ UDP
	if (UDPSocketServer == INVALID_SOCKET) {
		cout << "Blad w tworzeniu gniazda o numerze " << WSAGetLastError() << endl;
	}
	cout << "Utworzenie gniazda pomyslne " << endl;
	//---------------------------
	//£¹czenie z serwerem
	iBind = bind(
		UDPSocketServer,
		(SOCKADDR*)&UDPClient,
		sizeof(UDPClient));

	if (iBind == SOCKET_ERROR) {
		cout << "Blad laczenia z serwerem o numerze " << WSAGetLastError() << endl;
	}
	cout << "Laczenie z serwerem pomyslne " << endl;
	cout << "Czekam ..." << endl;
	//---------------------
	
	iReceiveFrom = recvfrom(
		UDPSocketServer,
		Buffer,
		iBuffferLength,
		MSG_PEEK,
		(SOCKADDR*)&UDPClient,
		&iUDPClient);

	if (iReceiveFrom == SOCKET_ERROR) {
		cout << "Blad odebrania wiadomosci o numerze " << WSAGetLastError() << endl;

	}

	//odebranie danych od klienta
	cout << "Odebrano pomyslnie " << endl << endl;
	cout << "Odebrana wiadomosc: " << Buffer << endl << endl;

	//------------------------
	//zamkniecie gniazda
	iCloseSocket = closesocket(UDPSocketServer);

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