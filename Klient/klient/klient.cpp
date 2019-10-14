
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
#include <string>
#include <conio.h>
#include <ctime>

#pragma comment (lib, "Ws2_32.lib")

using namespace std;


string get_godzina() {

	time_t currentTime;
	struct tm* localTime;

	time(&currentTime);                   // Get the current time
	localTime = localtime(&currentTime);  // Convert the current time to the local time

	int hour = localTime->tm_hour;
	int minute = localTime->tm_min;

	string godzina = "";
	godzina += to_string(hour)+ ":" + to_string(minute);

	return godzina;
}

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
	//oper#mnozenie@stat#NULL@iden#sesja0(numer w tablicy 0)#12:00#3#3#3@
	//tworzenie komunikatu
	//praca na tablcy char - Buffer
	//--tworzenie stringa - komunikatu do tablicy Buffer
	static string operacja = "oper#";
	static string status = "stat#NULL@"; //klient wysyla zawsze NULL
	string iden = "iden#";
	string komunikat_koncowy = " ";

	bool dzielenie = false;
	char wybor;

	....//wstawic while true
	//-- test dla jednej operacji -- 
	cout << "Jaka operacja " << endl;
	cout << "-----------------"<<endl;
	cout << "1. Dodawanie " << endl;
	cout << "2. Odejmowanie " << endl;
	cout << "3. Mno¿enie " << endl;
	cout << "4. Dzielenie " << endl;

	wybor = getch();
	switch(wybor) {
	case '1':
		operacja += "dodawanie@";
		break;
	case '2':
		operacja += "odejmowanie@";
			break;
	case '3':
		operacja += "mnozenie@";
		break;
	case '4':
		operacja += "dzielenie@";
		dzielenie = true;
		//--sprawdzenie warunku potem - string iden
		break;
	}
	//1. ---- doklejenie numeru sesji klienta do stringa operacja, wstawic tam gdzie inicjalizacja gniazda, poniewaz klient TYLKO RAZ otrzymuje nr sesji

	
		.
		.
		.
		.
		.



	//2. ---- doklejenie godziny do stringa operacja----
	iden += get_godzina()+"#";

	//3. ---- doklejenie liczb do stringa operacja-----

	//dodawanie liczb do iden (moze byc poza switch, jedynie co trzeba dodac to sprawdzenie 0 gdy operacja jest dzielenie)
	//---- w dzieleniu tylko PIERWSZA liczba moze byc 0, reszta musi byc rozna od 0----
	//wpisanie 3 liczb do dzialania
	int liczba;
		for (int i = 0; i < 3;) {
			//wprowadzanie liczby do zmiennej tymczasowej
			cin >> liczba;
			//warunek sprawdzajacy czy dana operacja jest dzieleniem lub, w przypadku dzielenia, sprawdza czy aktualnie ustawiana jest pierwsza liczba (w przypadku dzielenia moze byc to 0)
			if (i == 0 || !dzielenie) {
				//wpisanie liczby do operacji iden
				iden += to_string(liczba);
				iden += (i != 2) ? "#" : "@";
				i++;
			}
			else {
				//wykrycie czy dana operacja jest dzieleniem
				if (dzielenie) {
					//wykrycie czy zostalo wpsiane 0 dla drugiej lub trzeciej liczby, jesli nie, liczba zostanie dopisana, jesli tak, krok dopisania zostanie pominiety
					if (liczba != 0) {
						iden += to_string(liczba);
						iden += (i != 2) ? "#" : "@";
						i++;
					}
				}
			}
		}


	//----wyslanie pe³nego komunikatu

	//--zamiana string na char array : Buffer 
	strcpy(Buffer, komunikat_koncowy.c_str());
	//---
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