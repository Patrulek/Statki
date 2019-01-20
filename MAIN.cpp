#ifndef NAGL_HPP
#include "NAGL.hpp"
HANDLE hOut = GetStdHandle (STD_OUTPUT_HANDLE);




menu *MENU;                         //stworzenie wskaźnika do obiektu MENU
plansza *PLANSZA;                   //stworzenie wskaźnika do PLANSZY do gry
przebieg_gry *GRA;                  //stworzenie wskaźnika do mechanizmu przebiegu gry




                        jednomasztowiec JEDEN, DWA, TRZY, CZTERY, KJEDEN, KDWA, KTRZY, KCZTERY;                                    //---------------------------------------stworzenie-------------------------------------------------
                        dwumasztowiec JEDENDWA, DWADWA, TRZYDWA, KJEDENDWA, KDWADWA, KTRZYDWA;                     //-----------------------------------------statkow------------------------------------------------------
                        trojmasztowiec JEDENTRZY, DWATRZY, KJEDENTRZY, KDWATRZY;                                                                    //--------------------------------------------do-------------------------------------------------------
                        czteromasztowiec JEDENCZTERY, KJEDENCZTERY;                                                                                                        //--------------------------------------------gry-----------------------------------------------------


extern void ustaw_zero_statkow();
extern int ile_statkow();
void ustaw_statki();    //funkcja do ustawienia statkow na planszy
void inicjalizacja_obiektow(); //funkcja do stworzenia obiektow do gry
void stworzenie_ini();
void ekran_powitalny();
void nazwa_ini(string & nazwa);
string pasek_stanu;
string pasek_stanu_pelny(20, 'X');

using namespace std;




int main()
{
            // inicjalizja obiektu MENU wraz z akcjami zwiazanymi z nim
            MENU = new menu;
            MENU->wypisz_start();
            stworzenie_ini();
            MENU->wypisz_menu();
            MENU->akcja();


            return 1;
}

void ustaw_statki()
{
            while(statki::ile_statkow() != 20)
            {
                        PLANSZA = new plansza;
                        GRA = new przebieg_gry;
                        pasek_stanu = "|====================|";
                        SetConsoleTextAttribute( hOut,15  );
                        cout << " ... trwa losowanie pozycji statkow ..." << endl << endl << pasek_stanu;
                        SetConsoleTextAttribute( hOut,9  );
                        JEDENCZTERY.ustawianie_statku();
                        pasek_stanu.replace(1, 1, pasek_stanu_pelny, 0, 1);
                        cout << "\r" << pasek_stanu;
                        JEDENTRZY.ustawianie_statku();
                        pasek_stanu.replace(1, 2, pasek_stanu_pelny, 0, 2);
                        cout << "\r" << pasek_stanu;
                        DWATRZY.ustawianie_statku();
                        pasek_stanu.replace(1, 3, pasek_stanu_pelny, 0, 3);
                        cout << "\r" << pasek_stanu;
                        JEDENDWA.ustawianie_statku();
                        pasek_stanu.replace(1, 4, pasek_stanu_pelny, 0, 4);
                        cout << "\r" << pasek_stanu;
                        DWADWA.ustawianie_statku();
                        pasek_stanu.replace(1, 5, pasek_stanu_pelny, 0, 5);
                        cout << "\r" << pasek_stanu;
                        TRZYDWA.ustawianie_statku();
                        pasek_stanu.replace(1, 6, pasek_stanu_pelny, 0, 6);
                        cout << "\r" << pasek_stanu;
                        JEDEN.ustawianie_statku();
                        pasek_stanu.replace(1, 7, pasek_stanu_pelny, 0, 7);
                        cout << "\r" << pasek_stanu;
                        DWA.ustawianie_statku();
                        pasek_stanu.replace(1, 8, pasek_stanu_pelny, 0, 8);
                        cout << "\r" << pasek_stanu;
                        TRZY.ustawianie_statku();
                        pasek_stanu.replace(1, 9, pasek_stanu_pelny, 0, 9);
                        cout << "\r" << pasek_stanu;
                        CZTERY.ustawianie_statku();
                        pasek_stanu.replace(1,10, pasek_stanu_pelny, 0, 10);
                        cout << "\r" << pasek_stanu;



                        KJEDENCZTERY.ustawianie_statku_wroga();
                        pasek_stanu.replace(1, 11, pasek_stanu_pelny, 0, 11);
                        cout << "\r" << pasek_stanu;
                        KJEDENTRZY.ustawianie_statku_wroga();
                        pasek_stanu.replace(1, 12, pasek_stanu_pelny, 0, 12);
                        cout << "\r" << pasek_stanu;
                        KDWATRZY.ustawianie_statku_wroga();
                        pasek_stanu.replace(1, 13, pasek_stanu_pelny, 0, 13);
                        cout << "\r" << pasek_stanu;
                        KJEDENDWA.ustawianie_statku_wroga();
                        pasek_stanu.replace(1, 14, pasek_stanu_pelny, 0, 14);
                        cout << "\r" << pasek_stanu;
                        KDWADWA.ustawianie_statku_wroga();
                        pasek_stanu.replace(1, 15, pasek_stanu_pelny, 0, 15);
                        cout << "\r" << pasek_stanu;
                        KTRZYDWA.ustawianie_statku_wroga();
                        pasek_stanu.replace(1, 16, pasek_stanu_pelny, 0, 16);
                        cout << "\r" << pasek_stanu;
                        KJEDEN.ustawianie_statku_wroga();
                        pasek_stanu.replace(1, 17, pasek_stanu_pelny, 0, 17);
                        cout << "\r" << pasek_stanu;
                        KDWA.ustawianie_statku_wroga();
                        pasek_stanu.replace(1, 18, pasek_stanu_pelny, 0, 18);
                        cout << "\r" << pasek_stanu;
                        KTRZY.ustawianie_statku_wroga();
                        pasek_stanu.replace(1, 19, pasek_stanu_pelny, 0, 19);
                        cout << "\r" << pasek_stanu;
                        KCZTERY.ustawianie_statku_wroga();
                        pasek_stanu.replace(1, 20, pasek_stanu_pelny, 0, 20);
                        cout << "\r" << pasek_stanu;
                        if(statki::ile_statkow() != 20)
                        {
                                    SetConsoleTextAttribute( hOut,15  );
                                    cin.sync();
                                    cout << endl << endl << "Zaladowano " <<statki::ile_statkow()<<"/20 statkow, nacisnij dowolny klawisz, by losowac raz jeszcze ...";
                                    statki::ustaw_zero_statkow();
                                    delete GRA;
                                    delete PLANSZA;
                                    getch();
                                    system("cls");
                        }
                        else
                        {
                                    SetConsoleTextAttribute( hOut,15  );
                                    cin.sync();
                                    cout << endl << endl << "Zaladowano " << statki::ile_statkow() << "/20 statkow, nacisnij dowolny klawisz, by rozpoczac gre ...";
                                    getch();
                                    system("cls");
                        }

            }

}



void inicjalizacja_gry()
{
            statki::ustaw_zero_statkow();
            ustaw_statki();
            GRA->akcja_gry();
}

void stworzenie_ini()
{
            nazwa_ini(MENU->plik_ini);
            int uruchomienia = 0;
            MENU->INI.open(MENU->plik_ini.c_str(), fstream::in | fstream::binary);
            MENU->INI.get(MENU->tab_ini, sizeof(MENU->tab_ini), '\n');
            MENU->INI.close();
            for(int i = 24; i < 29; i++)
            {
                        MENU->do_ini << MENU->tab_ini[i];
            }
            MENU->do_ini >> uruchomienia;
            MENU->do_ini.str("");
            MENU->do_ini.clear();
            if(uruchomienia == 0)
            {
                        MENU->INI.open(MENU->plik_ini.c_str(), fstream::out | fstream:: trunc | fstream::binary);
                        MENU->INI.clear();
                        string tekst;
                        string do_ss;
                        tekst = "Ktore uruchomienie gry: 0     \nIle wygranych gier: 0     \nIle przegranych gier: 0     ";
                        MENU->INI << tekst;
                        MENU->INI.close();
                        uruchomienia += 1;
                        MENU->do_ini << uruchomienia;
                        MENU->INI.open(MENU->plik_ini.c_str(), fstream::out | fstream::trunc | fstream::binary);
                        MENU->INI.clear();
                        MENU->do_ini.clear();
                        tekst.replace(24, MENU->do_ini.str().size(), MENU->do_ini.str(), 0, MENU->do_ini.str().size());
                        MENU->do_ini.str("");
                        MENU->INI << tekst;
                        MENU->INI.close();
                        ekran_powitalny();
            }
            else
            {
                        string pomoc;
                        stringstream zmiana_liczb;
                        MENU->INI.open(MENU->plik_ini.c_str(), fstream::in | fstream::binary);
                        MENU->INI.get(MENU->tab_ini, sizeof(MENU->tab_ini), EOF);
                        uruchomienia += 1;
                        MENU->INI.close();
                        MENU->INI.open(MENU->plik_ini.c_str(), fstream::out | fstream::trunc | fstream::binary);
                        pomoc =MENU->tab_ini;
                        zmiana_liczb << uruchomienia;
                        pomoc.replace(24, zmiana_liczb.str().size(), zmiana_liczb.str(), 0, zmiana_liczb.str().size());
                        MENU->INI << pomoc;
                        MENU->INI.clear();
                        MENU->INI.close();
            }


}

void nazwa_ini(string & nazwa )
{
            nazwa = __FILE__;
            string::size_type pozycja = nazwa.rfind("\\");
            nazwa.erase(pozycja+1);
            nazwa += "Statki.ini";
}

void ekran_powitalny()
{
            cout << "Jest to Twoje pierwsze uruchomienie tego programu, jesli chcesz mozesz pominac \nponizsze instrukcje naciskajac klawisz ESCAPE"
            "\n\nGra polega na zestrzeleniu, w jak najmniejszej ilosci ruchow, \nstatkow przeciwnika, podajac\nwspolrzedne planszy:"
            "\n- Pierwsza wspolrzedna (0-9)\n- Druga wspolrzedna (a-j)\nPlansza sklada sie ze 100 pol na ktorych zostana umieszczone statki:"
            "\n- Jeden czteromasztowiec\n- Dwa trzymasztowce\n- Trzy dwumasztowce\n- Cztery jednomasztowce\n\nPrzed startem gry mozna wybrac"
            " poziom trudnosci oraz sposob w jaki maja zostac \nustawione statki.\nW menu mozna takze sprawdzic swoje wyniki w grze.\n\nLEGENDA GRY:"
            "\nPuste, nieostrzelane pole - '~'\nPudlo - '@'\nTrafiony statek - '*'\nZatopiony statek - '#'\n\nTo juz wszystko co powinienes wiedziec o tej grze.";
            while(1)
            {
                        if(getch() == 27)
                        {
                                    cin.sync();
                                    system("cls");
                                    break;
                        }
                        else cin.sync();
            }
}
#endif
