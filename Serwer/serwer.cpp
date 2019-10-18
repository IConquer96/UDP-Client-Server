
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
#include <vector>

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
	godzina += to_string(hour) + ":" + to_string(minute);

	return godzina;
}

vector<string> getContainer(char napis[]) {
	string temp = "";
	vector<string> tempVector;

	for (int i = 0; i < strlen(napis); i++) {
		if (napis[i] != '@') {
			temp += napis[i];
		}
		else {
			tempVector.push_back(temp);
			temp = "";
		}
	}
	return tempVector;
}

vector<int> getInts(string& iden) {
	int hashCounter = 0;
	vector<int> ints;
	iden += "@";
	int temp = 0;
	for (int i = 0; i <= iden.length(); i++) {
		if (hashCounter >= 3) {
			if (iden[i] != '#' && iden[i] != '@') {
				temp *= 10;
				temp += iden[i] - 48;
			}
			else {
				ints.push_back(temp);
				temp = 0;
			}
		}
		if (iden[i] == '#') {
			hashCounter++;
		}
	}
	return ints;
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
	string komunikat_zwrotny = "";
	string operacja = "";
	int liczba1, liczba2, liczba3;

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
	//::__________________________________::
	//odebranie danych
	while (true) {
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
		//-------------------------------
		//::__________________________________::
		//serwer odsy³a wiadomosc do klienta

		//operacja - rozpoznawanie operacji dostarczonej przez komunikat klienta po dwóch pierwszych charach operacji -np dodawanie : "do"
		operacja = "";
		operacja.push_back(BufferReceive[5]);
		operacja.push_back(BufferReceive[6]);

		//cout << BufferReceive[5]<< " " <<  BufferReceive[6];
		//cout << " OPERACJA: " + operacja << endl;

		vector<string> komunikaty = getContainer(BufferReceive);
		vector<int> liczby = getInts(komunikaty[2]);//tu wywala?

		liczba1 = liczby.at(0);
		liczba2 = liczby.at(1);
		liczba3 = liczby.at(2);
		/*for (auto a : liczby) {
			cout << a << " ";
		}*/
		//oper#mnozenie@stat#NULL@iden#sesja0#12:00#3#3#3@ odebrany komunikat
		komunikat_zwrotny += "oper#";


		float wynik = NULL;

		if (operacja == "mn") {
			/*wynik = 1;
			for (auto a : liczby) {
				wynik *= a;
			}*/
			wynik = liczba1 * liczba2 * liczba3;
			komunikat_zwrotny += "mnozenie@stat#OK@iden#" + get_godzina() + "#" + to_string(wynik) + "@";
		}
		else if (operacja == "dz") {
			/*for (auto a : liczby) {
				if (wynik == NULL) {
					wynik = a;
				}
				else {
					wynik *= 1.0;
					wynik /= a;
				}
			}*/
			wynik = liczba1 * 1.0 / liczba2 * 1.0 / liczba3 * 1.0;
			komunikat_zwrotny += "dzielenie@stat#OK@iden#" + get_godzina() + "#" + to_string(wynik) + "@";
		}
		else if (operacja == "do") {
			/*wynik = 0;
			for (auto a : liczby) {
				wynik += a;
			}*/
			wynik = liczba1 + liczba2 + liczba3;
			komunikat_zwrotny += "dodawanie@stat#OK@iden#" + get_godzina() + "#" + to_string(wynik) + "@";
		}
		else if (operacja == "od") {
			/*for (auto a : liczby) {
				if (wynik == NULL) {
					wynik = a;
				}
				else {
					wynik -= a;
				}
			}*/
			wynik = liczba1 - liczba2 - liczba3;
			komunikat_zwrotny += "odejmowanie@stat#OK@iden#" + get_godzina() + "#" + to_string(wynik) + "@";
		}

		else {
			//stat#ERROR 
			komunikat_zwrotny = "stat#ERROR@"; //+ numer sesji
		}



		//oper#mnozenie@stat#NULL@iden#sesja0#12:00#3#3#3@ odebrany komunikat

		//oper#mnozenie@:stat#
		//OK@iden#sesja0(numer w tablicy 0)#12:00#27@





		//::__________________________________::
		//------------------------------
		strcpy(BufferResponse, komunikat_zwrotny.c_str()); //zamiana stringa na tablicê char
		iBuffferResponseLength = strlen(BufferResponse) + 1; //dlugosc buffera do wyslania
		//----------------------------
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

	}
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