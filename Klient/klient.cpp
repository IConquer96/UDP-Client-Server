
#ifndef UNICODE
#define UNICODE
#endif
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#define PORT 8000

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
	WORD versionRequested = MAKEWORD(2, 2); //wersja WinSock

	cout << "-----UDP CLIENT----- " << endl;

	WSADATA WinSockData;
	int iWsaStartup;
	int iWsaCleanup;

	SOCKET UDPSocketClient;
	struct sockaddr_in UDPServer;

	char Buffer[800] = "Wiadomosc do wyslania";  //wiadomosc do przeslania 
	int iSendTo;
	
 
	int iUDPServer= sizeof(UDPServer);
	int iCloseSocket;

	//do odebrania wiadomoœci od serwera

	int iReceiveFrom;
	char BufferReceive[800]; //wiadomosc doebrana 
	int iBuffferReceiveLength = strlen(BufferReceive) + 1;


	//

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

	//wstawic while true
	//
	//
	//

	//-- test dla jednej operacji -- 
	cout << "Jaka operacja " << endl;
	cout << "-----------------"<<endl;
	cout << "1. Dodawanie " << endl;
	cout << "2. Odejmowanie " << endl;
	cout << "3. Mnozenie " << endl;
	cout << "4. Dzielenie " << endl;
	

	cin >> wybor;
	cout << " Wpisz 3 liczby do operacji " << endl;

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

	
//
	//
	//
	//

	//2. ---- doklejenie godziny do stringa operacja----
	iden += get_godzina()+"##";
	//USUNAC JEDNEGO HASZA!!!!! KIEDY DODAMY SESJE


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
					//wykrycie czy zostalo wpisane 0 dla drugiej lub trzeciej liczby, jesli nie, liczba zostanie dopisana, jesli tak, krok dopisania zostanie pominiety
					if (liczba != 0) {
						iden += to_string(liczba);
						iden += (i != 2) ? "#" : "@";
						i++;
					}
					else {
						cout << "To nie moze byc zero! Wpisz ponownie" << endl;
					}
				}
			}
		}


	//----wyslanie pe³nego komunikatu
	komunikat_koncowy = operacja + status + iden;
	//--zamiana string na char array : Buffer 
	
	strcpy(Buffer, komunikat_koncowy.c_str());

	int iBuffferLength = strlen(Buffer) + 1; //dlugosc buffera

	iSendTo = sendto(
		UDPSocketClient,
		Buffer,
		iBuffferLength,
		MSG_DONTROUTE,
		(SOCKADDR*)&UDPServer,
		sizeof(UDPServer));
	//---------------------------
	

	if (iSendTo == SOCKET_ERROR) {
		cout << "Blad wysylania wiadomosci o numerze " << WSAGetLastError() << endl;
	}
	cout << "Wiadomosc wyslana pomyslne " << endl;
	//---------------------
	//odebranie komunikatu od serwera

	iReceiveFrom = recvfrom(
		UDPSocketClient,
		BufferReceive,
		iBuffferReceiveLength,
		MSG_PEEK,
		(SOCKADDR*)&UDPServer,
		&iUDPServer);

	if (iReceiveFrom == SOCKET_ERROR) {
		cout << "Blad odebrania wiadomosci o numerze " << WSAGetLastError() << endl;

	}

	//odebranie danych od klienta
	cout << "Odebrano pomyslnie " << endl << endl;
	cout << "Odebrana wiadomosc: " << BufferReceive << endl << endl;






	//::_________________________________________________________________________________::
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

	//::_________________________________________________________________________________::
}
//.server