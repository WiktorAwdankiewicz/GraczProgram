#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// funkcja sprawdzajaca czy gracz chce wydac kolejny rozkaz
void czyDalej(bool &rozAkt)
{
	char x;
	cout << "Czy chcesz wydac koleny rozkaz? T/N: ";
	cin >> x;
	cout << endl;
	if (x == 'N')
		rozAkt = false;
}

int main(int argc, char* argv[])
{
	fstream rozkazyFile;
	string mapaS, zlotoS, statusS, bazaS, bazaS2, akcjaS, unitBuildS;
	string baseIdS = "P B "; // szukany ciag znakow w pliku status, oznaczajacy baze gracza
	char akcjaC, czyRoz, unitBuildC;
	bool rozkazyAktywne = true;
	long zloto;
	int baseId = 0; // id bazy gracza
	int unitId = 0; // jednostka atakujaca
	int unitIdA = 0; // atakowana jednostka
	int unitIdRuch = 0; // przemieszczana jednostka
	int ruchX = 0; // wspolrzedne na ktore przemieszcza sie jednostka
	int ruchY = 0; 

	// wczytywanie mapy z pliku i rysowanie jej
	cout << "Mapa" << endl << endl;
	ifstream mapaFile(argv[1]);
	while (getline(mapaFile, mapaS, ','))
		cout << mapaS;
	cout << endl << endl;

	// wczytywanie statusu gry z pliku i wypisywanie go
	ifstream statusFile(argv[2]);
	getline(statusFile, zlotoS); // wczytywanie tylko pierwszej linii z pliku, czyli ilosci zlota
	zloto = stoi(zlotoS);
	cout << "Dostepne zloto: " << zloto << endl;
	cout << "Status gry:" << endl;
	while (getline(statusFile, statusS, ','))
		cout << statusS;
	cout << endl << endl;

	// szukanie ID bazy gracza
	statusFile.clear();
	statusFile.seekg(0);
	int line = 0;
	while (getline(statusFile, bazaS))
	{
		line++;
		if (bazaS.find(baseIdS, 0) != string::npos)
		{
			statusFile.clear();
			statusFile.seekg(0);
			// poniewaz znaleziono szukany ciag znakow, program idzie w pliku do linii gdzie zostalo znalezione ID
			for (int i = 0; i < line - 1; i++)
			{
				getline(statusFile, bazaS2);
			}
			// odczyt z pliku w konkretnym miejscu i zapisanie ID bazy gracza do zmiennej
			statusFile.seekg(4, ios::cur);
			statusFile >> baseId;
		}
	}

	// otwieranie pliku rozkazy i okreslanie ich
	rozkazyFile.open(argv[3]);
	cout << "Czy chcesz wydac rozkazy? T/N" << endl;
	cin >> czyRoz;
	if (czyRoz == 'T')
	{
		while (rozkazyAktywne)
		{
			cout << "Dostepne rozkazy: M - ruch, A - atak, B - budowanie" << endl;
			cout << "Jaki rozkaz chcesz wydac: ";
			cin >> akcjaS;
			if (akcjaS.length() == 1)
			{
				akcjaC = akcjaS[0];
				switch (akcjaC)
				{
				case 'M':
					cout << "Podaj ID jednostki ktora chcesz przemiescic: ";
					cin >> unitIdRuch;
					if (cin.fail())
					{
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "Podaj poprawne ID jednostki" << endl;
					}
					else
					{
						cout << "Podaj wspolrzedne na ktore ma sie udac jednostka" << endl;
						cout << "X: ";
						cin >> ruchX;
						if (cin.fail())
						{
							cin.clear();
							cin.ignore(numeric_limits<streamsize>::max(), '\n');
							cout << "Podaj poprawne wspolrzedne X" << endl;
						}
						else
						{
							cout << "Y: ";
							cin >> ruchY;
							if (cin.fail())
							{
								cin.clear();
								cin.ignore(numeric_limits<streamsize>::max(), '\n');
								cout << "Podaj poprawne wspolrzedne Y" << endl;
							}
							else
							{
								rozkazyFile << unitIdRuch << " M " << ruchX << " " << ruchY;
								rozkazyFile << "\n";
							}
						}
					}

					czyDalej(rozkazyAktywne);
					unitIdRuch = 0;
					ruchX = 0;
					ruchY = 0;
					break;

				case 'A':
					cout << "Podaj ID jednostki ktora ma przeprowadzic atak: ";
					cin >> unitId;
					if (cin.fail())
					{
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "Podaj poprawne ID jednostki" << endl;
					}
					else
					{
						cout << "Podaj ID jednostki atakowanej: ";
						cin >> unitIdA;
						if (cin.fail())
						{
							cin.clear();
							cin.ignore(numeric_limits<streamsize>::max(), '\n');
							cout << "Podaj poprawne ID jednostki" << endl;
						}
						else
						{
							rozkazyFile << unitId << " A " << unitIdA;
							rozkazyFile << "\n";
						}

					}
					czyDalej(rozkazyAktywne);
					unitId = 0;
					unitIdA = 0;
					break;

				case 'B':
					cout << "Jaka jednostke chcesz zakupic: ";
					cin >> unitBuildS;
					if (unitBuildS.length() == 1)
					{
						unitBuildC = unitBuildS[0];
						switch (unitBuildC)
						{
						case 'K':
							if ((zloto - 400) >= 0)
							{
								rozkazyFile << baseId << " B " << unitBuildC;
								zloto -= 400;
								rozkazyFile << "\n";
							}
							else
								cout << "Za malo zlota do zakupu jednostki Knight, potrzeba jeszcze " << abs(zloto - 400) << " zlota." << endl;
							break;

						case 'S':
							if ((zloto - 250) >= 0)
							{
								rozkazyFile << baseId << " B " << unitBuildC;
								zloto -= 250;
								rozkazyFile << "\n";
							}
							else
								cout << "Za malo zlota do zakupu jednostki Swordsman, potrzeba jeszcze " << abs(zloto - 250) << " zlota." << endl;
							break;

						case 'A':
							if ((zloto - 250) >= 0)
							{
								rozkazyFile << baseId << " B " << unitBuildC;
								zloto -= 250;
								rozkazyFile << "\n";
							}
							else
								cout << "Za malo zlota do zakupu jednostki Archer, potrzeba jeszcze " << abs(zloto - 250) << " zlota." << endl;
							break;

						case 'P':
							if ((zloto - 200) >= 0)
							{
								rozkazyFile << baseId << " B " << unitBuildC;
								zloto -= 200;
								rozkazyFile << "\n";
							}
							else
								cout << "Za malo zlota do zakupu jednostki Pikeman, potrzeba jeszcze " << abs(zloto - 200) << " zlota." << endl;
							break;

						case 'R':
							if ((zloto - 500) >= 0)
							{
								rozkazyFile << baseId << " B " << unitBuildC;
								zloto -= 500;
								rozkazyFile << "\n";
							}
							else
								cout << "Za malo zlota do zakupu jednostki Ram, potrzeba jeszcze " << abs(zloto - 500) << " zlota." << endl;
							break;

						case 'C':
							if ((zloto - 800) >= 0)
							{
								rozkazyFile << baseId << " B " << unitBuildC;
								zloto -= 800;
								rozkazyFile << "\n";
							}
							else
								cout << "Za malo zlota do zakupu jednostki Catapult, potrzeba jeszcze " << abs(zloto - 800) << " zlota." << endl;
							break;

						case 'W':
							if ((zloto - 100) >= 0)
							{
								rozkazyFile << baseId << " B " << unitBuildC;
								zloto -= 100;
								rozkazyFile << "\n";
							}
							else
								cout << "Za malo zlota do zakupu jednostki Worker, potrzeba jeszcze " << abs(zloto - 100) << " zlota." << endl;
							break;

						default:
							cout << endl << "Nie ma takiej jednostki, sproboj ponownie" << endl;
							break;
						}
					}
					else
						cout << "Nie ma takiej jednostki, sproboj ponownie" << endl;

					czyDalej(rozkazyAktywne);
					break;

				default:
					cout << endl << "Bledny rozkaz, sproboj ponownie" << endl;
					break;
				}
			}
			else
				cout << "Bledny rozkaz, sproboj ponownie" << endl;
		}
	}
}