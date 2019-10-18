
#ifndef UNICODE
#define UNICODE
#endif
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

constexpr auto PORT = 8000;

#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <Windows.h>
#include <winsock2.h>
#include <ctime>
#include <string>

#pragma comment (lib, "Ws2_32.lib")

using namespace std;

int getId(bool id[]) {
	for(int i=0; i<8; i++) {
		if(id[i]) {
			return i;
		}
	}
	return -1;
}

string get_godzina() {

	time_t currentTime;
	struct tm* localTime;

	time(&currentTime);                   // Get the current time
	localTime = localtime(&currentTime);  // Convert the current time to the local time

	int hour = localTime->tm_hour;
	int minute = localTime->tm_min;

	string godzina = "";
	godzina += to_string(hour) + ":" + to_string(minute);

	return godzina;
}


int main()
{
	//inicjalizacja zmiennych


	static char IP[] = "127.0.0.1"; //host IP
	WORD versionRequested = MAKEWORD(2, 2); //wersja WinSock
							
	cout << "-----UDP SERWER----- " << endl;

	WSADATA WinSockData;
	int iWsaStartup;
	int iWsaCleanup;

	SOCKET UDPSocketServer;
	struct sockaddr_in UDPClient;

	char BufferReceive[800]; //wiadomosc doebrana 
	int iBuffferReceiveLength = strlen(BufferReceive) +1; //dlugosc buffera do odebrania
	

	int iBind;
	int iReceiveFrom;
	string komunikat_otrzymany;
	int iUDPClient = sizeof(UDPClient);
	int iCloseSocket;

	//do wys³ania zwrotnej wiaodmoœci
	int iSendTo;
	char BufferResponse[800]; //wiadomosc doebrana 
	int iBuffferResponseLength; //dlugosc buffera do odebrania
	string komunikat_zwrotny;

	/*
	bool id_client[8];
	int liczba_client = 0; //max 8 polaczonych klientow

	for(int i=0; i<8; i++) {
		id_client[i] = true;
	}
	//--------------------------------
	*/
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
	//--------------
	//odebranie komunikatu o id, nastêpnie odes³anie numeru id do klienta
	//odebranie komunikatu z proœb¹ o id



	//oper#mnozenie@:stat#OKEY@iden#sesja0(numer w tablicy 0)#12:00#27@

	strcpy(BufferReceive, komunikat_otrzymany.c_str());

	iReceiveFrom = recvfrom(
		UDPSocketServer,
		BufferReceive,
		iBuffferReceiveLength,
		MSG_PEEK,
		(SOCKADDR*)&UDPClient,
		&iUDPClient);

	if (iReceiveFrom == SOCKET_ERROR) {
		cout << "Blad odebrania wiadomosci o numerze " << WSAGetLastError() << endl;

	}

	//odebranie danych od klienta
	cout << "Odebrano pomyslnie " << endl << endl;
	cout << "Odebrana wiadomosc: " << BufferReceive << endl << endl;


	//serwer wysy³a wiadomosc do klienta
	//---
	komunikat_zwrotny = "Jestesmy zgubieni";
	

	strcpy(BufferResponse, komunikat_zwrotny.c_str());

	iBuffferResponseLength = strlen(BufferResponse) + 1; //dlugosc buffera do wyslania

	iSendTo = sendto(
		UDPSocketServer,
		BufferResponse,
		iBuffferResponseLength,
		MSG_DONTROUTE,
		(SOCKADDR*)&UDPClient,
		sizeof(UDPClient));
	//---------------------------

	if (iSendTo == SOCKET_ERROR) {
		cout << "Blad wysylania wiadomosci o numerze " << WSAGetLastError() << endl;
	}
	cout << "Wiadomosc wyslana pomyslne " << endl;

	//zamkniecie gniazda
	
	//jesli klient wysle END zamkniecie serwera 

	
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