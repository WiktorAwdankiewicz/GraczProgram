#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

// funkcja zwracajaca id szukanej jednostki
int szukajId(ifstream& statusPlik, bool czyGracz, string jednostka)
{
	string unitS;
	string szukaneIdS;
	if (czyGracz == true)
	{
		szukaneIdS = "P " + jednostka + " ";
	}
	else
	{
		szukaneIdS = "E " + jednostka + " ";
	}
	int szukaneId = 0;
	int line = 0;
	bool x = false;
	statusPlik.clear();
	statusPlik.seekg(0);
	while (getline(statusPlik, unitS) && x == false)
	{
		line++;
		if (unitS.find(szukaneIdS, 0) != string::npos)
		{
			statusPlik.clear();
			statusPlik.seekg(0);
			// poniewaz znaleziono szukany ciag znakow, program idzie w pliku do linii, gdzie jest szukane ID
			for (int i = 0; i < line - 1; i++)
			{
				getline(statusPlik, unitS);
			}
			// odczyt z pliku w konkretnym miejscu w linii i zapisanie szukanego ID do zmiennej
			statusPlik.seekg(4, ios::cur);
			statusPlik >> szukaneId;
			x = true;
		}
	}
	return szukaneId;
}

int main(int argc, char* argv[])
{
	string mapaS, mapaSX, mapaSY, zlotoS, bazaS, bazaSProd;
	string graczBazaIdS = "P B "; // szukany ciag znakow w pliku status, oznaczajacy baze gracza
	vector<string> mapa;
	long zloto;
	int mapaX = 0; // wymiary mapy
	int mapaY = 0;
	int graczBazaId = 0; // id bazy gracza
	bool x = false; // czy zostalo znalezione szukane ID

	// Wczytywanie rozmiaru mapy
	// szerokosc
	ifstream mapaFile(argv[1]);
	getline(mapaFile, mapaSX);
	mapaX = mapaSX.length();
	mapaFile.clear();
	mapaFile.seekg(0);
	// wysokosc
	while (getline(mapaFile, mapaSY))
		mapaY++;

	// Zapis mapy do vectora string
	mapaFile.clear();
	mapaFile.seekg(0);
	for (int i = 0; i < mapaY; i++)
	{
		getline(mapaFile, mapaS);
		mapa.push_back(mapaS);
	}

	// Wczytywanie statusu gry z pliku i wypisywanie go
	ifstream statusFile(argv[2]);
	getline(statusFile, zlotoS); // wczytywanie tylko pierwszej linii z pliku, czyli ilosci zlota
	zloto = stoi(zlotoS);

	// Szukanie ID bazy gracza
	graczBazaId = szukajId(statusFile, true, "B");

	// Sprawdzenie czy baza gracza juz cos produkuje; zapisanie lini ze statusem bazy do string
	statusFile.clear();
	statusFile.seekg(0);
	x = false;
	int line = 0;
	while (getline(statusFile, bazaSProd) && x == false)
		// dodanie dodatkowego warunku "x == false" okresla ze szukamy tylko pierwszego wystapienia
		// bez niego petla leci po calym pliku
	{
		line++;
		if (bazaSProd.find(graczBazaIdS, 0) != string::npos)
		{
			statusFile.clear();
			statusFile.seekg(0);
			// poniewaz znaleziono szukany ciag znakow, program idzie w pliku do linii gdzie zostalo znalezione ID
			for (int i = 0; i < line - 1; i++)
			{
				getline(statusFile, bazaSProd);
			}
			x = true;
		}
	}

	// Okreslanie rozkazow
	fstream rozkazyFile(argv[3]);
	// Budowanie
	if (bazaSProd.back() == '0')    // jesli baza nic nie produkuje; 0 na koncu statusu bazy okresla brak produkcji
	{
		if (zloto >= 1500)
		{
			rozkazyFile << graczBazaId << " B C\n";
			zloto -= 800;
		}
		else if (zloto >= 1000)
		{
			rozkazyFile << graczBazaId << " B R\n";
			zloto -= 500;
		}
		else if (zloto >= 600)
		{
			rozkazyFile << graczBazaId << " B K\n";
			zloto -= 400;
		}
		else if (zloto >= 400)
		{
			rozkazyFile << graczBazaId << " B A\n";
			zloto -= 250;
		}
		else if (zloto >= 100)
		{
			rozkazyFile << graczBazaId << " B W\n";
			zloto -= 100;
		}
	}
}