#ifndef NAGL_HPP
class menu;
extern HANDLE hOut;
extern void inicjalizacja_gry();
extern menu *MENU;



using namespace std;

class menu
{
            private:
            // sk³adniki klasy
                        char znak_akcji;                                                                                                                                //dana sk³adowa do okreœlania akcji w menu
                        char znak_wyjscia;                                                                                                                           //dana sk³adowa do okreœlania akcji podczas zamykania programu
                        char znak_wyniku;                                                                                                                            //dana sk³adowa do okreœlania akcji w sprawdzaniu wyników
                        char najlepsze_wyniki[300];                                                                                                         //tablica w której bêd¹ zapisywane najlepsze wyniki
                        char ostatnie_wyniki[350];                                                                                                           //tablica w której bêd¹ zapisywane ostatnie wyniki
                        char tab_ini[150];
                        string plik_najlepsze;                                                                                                                          //string w którym bêdzie utworzona nazwa pliku dla najlepszych wyników
                        string plik_ostatnie;                                                                                                                             //string w którym bêdzie utworzona nazwa pliku dla ostatnich wyników
                        string plik_ini;
                        fstream wyniki;                                                                                                                                       //strumieñ do zapisywania i odczytywania wyników z plików
                        fstream INI;
                        stringstream do_ini;
            //funkcje sk³adowe klasy
                        void nazwa_katalogu(string & nazwa , string & nazwa_ost);                                        //-----------------------uzupe³nia string nazw¹ œcie¿ki gry----------------------------//
                        void open_ost();                                                                                                                               //---------------------otwiera plik z wynikami----------------------//
                        void open_naj();                                                                                                                               //--------------------------------jak wy¿ej------------------------------------//

            public:
            //funkcje sk³adowe klasy
                        void wypisz_start();                                                                                                                     //----------------------wypisuje ekran startowy gry-----------------------------------//
                        void wypisz_menu();                                                                                                                 //----------------------wypisuje menu gry----------------------------------------------//
                        void akcja();                                                                                                                                   //-----------------------spe³nia ¿¹dan¹ rzecz wybran¹ w menu gry------------------//
                        friend class przebieg_gry;
                        friend void stworzenie_ini();
};


void menu::akcja()
{
            nazwa_katalogu(plik_najlepsze, plik_ostatnie);
            do
            {
                        znak_akcji = getch();
                        if(znak_akcji == '1')
                        {
                                    system("cls");
                                    inicjalizacja_gry();
                        }
                        if(znak_akcji == '2')
                        {
                                    system("cls");
                                    SetConsoleTextAttribute( hOut,7  );
                                    cout << "########################################\n### ";
                                    SetConsoleTextAttribute( hOut,15  );
                                    cout << "1 - by zobaczyc ostatnie wyniki  ";
                                    SetConsoleTextAttribute( hOut,7  );
                                    cout << "###\n### ";
                                    SetConsoleTextAttribute(hOut, 15);
                                    cout << "2 - by zobaczyc najlepsze wyniki ";
                                    SetConsoleTextAttribute( hOut,7  );
                                    cout << "###\n### ";
                                    SetConsoleTextAttribute(hOut, 15);
                                    cout << "0 - by wrocic do menu gry        ";
                                    SetConsoleTextAttribute( hOut,7  );
                                    cout << "###\n";
                                    cout << "######################################## ";
                                    do
                                    {
                                    znak_wyniku = getch();
                                    if(znak_wyniku == '1')
                                    {
                                                open_ost();
                                                if(getch())
                                                {
                                                            system("cls");
                                                            this->wypisz_menu();
                                                            break;
                                                }
                                    }

                                    if(znak_wyniku == '2')
                                    {
                                                open_naj();
                                                if(getch())
                                                {
                                                            system("cls");
                                                            this->wypisz_menu();
                                                            break;
                                                }
                                    }
                                    if(znak_wyniku == '0')
                                    {
                                                system("cls");
                                                this->wypisz_menu();
                                                break;
                                    }
                                    }
                                    while(1);
                        }
                        if(znak_akcji== '3')
                        {
                                    system("cls");
                                    SetConsoleTextAttribute( hOut,7  );
                                    cout << "########################################\n### ";
                                    SetConsoleTextAttribute( hOut,15  );
                                    cout << "Gra polega na zestrzeleniu stat-";
                                    SetConsoleTextAttribute( hOut,7  );
                                    cout << " ###\n### " ;
                                    SetConsoleTextAttribute( hOut,15  );
                                    cout << "kow nieprzyjaciela zanim on us- " ;
                                    SetConsoleTextAttribute( hOut,7  );
                                    cout << " ###\n### ";
                                    SetConsoleTextAttribute( hOut,15  );
                                    cout << "rzeli Ciebie.                    ";
                                    //cout << " ###\n### -------------------------------- ###" << endl;
                                    SetConsoleTextAttribute( hOut,7  );
                                    cout << "###                                  " << endl;
                                    cout << "########################################" ;
                                    if(getch())
                                    {
                                                system("cls");
                                                this->wypisz_menu();
                                    }
                        }
                        if(znak_akcji == '4')
                        {
                                    system("cls");
                                    SetConsoleTextAttribute( hOut,7  );
                                    cout << "########################################" << endl;
                                    cout << "###                           ##########\n### " ;
                                    SetConsoleTextAttribute( hOut,12  );
                                    cout << "Autor: Patryk Lewandowski" ;
                                    SetConsoleTextAttribute( hOut,7  );
                                    cout << " ##########\n###                           ##########" << endl;
                                    cout << "########################################" << endl;
                                    if(getch())
                                    {
                                                system("cls");
                                                this->wypisz_menu();
                                    }
                        }
                        if(znak_akcji =='0')
                        {
                                    system("cls");
                                    SetConsoleTextAttribute( hOut,7  );
                                    cout << "########################################\n### " ;
                                    SetConsoleTextAttribute( hOut,15  );
                                    cout <<"Czy na pewno chcesz opuscic gre?";
                                    SetConsoleTextAttribute( hOut,7  );
                                    cout << " ###\n###                                  ###\n###" ;
                                    SetConsoleTextAttribute( hOut,15  );
                                    cout << "               (t/n)              " ;
                                    SetConsoleTextAttribute( hOut,7  );
                                    cout << "###\n###                                  ###" << endl;
                                    cout << "########################################" << endl;
                                    do
                                    {
                                                znak_wyjscia = getch();
                                                if(znak_wyjscia == 't') { delete MENU; exit(1);}
                                                if(znak_wyjscia == 'n')
                                                {
                                                            system("cls");
                                                            this->wypisz_menu();
                                                            break;
                                                 }
                                    }
                                    while(1);
                        }
            }
            while(1);
}

void menu::nazwa_katalogu(string & nazwa , string & nazwa_ost )
{
            nazwa = __FILE__;
            string::size_type pozycja = nazwa.rfind("\\");
            nazwa.erase(pozycja+1);
            nazwa_ost = nazwa + "ostatnie_wyniki.txt";
            nazwa += "najlepsze_wyniki.txt";
}


void menu::open_ost()
{
            system("cls");
            wyniki.open(plik_ostatnie.c_str(), ios::in | ios::binary);
            if(wyniki)
            {
            wyniki.seekg(55);
            wyniki.get(ostatnie_wyniki, sizeof(ostatnie_wyniki), EOF);
            cout << "Oto Twoje ostatnie wyniki";
            for(int i = 0; i < sizeof(ostatnie_wyniki); i++)
            {
                        if(ostatnie_wyniki[i] == EOF) break;
                        if(ostatnie_wyniki[i] == '\n')
                        {
                                    cout << endl;
                                    continue;
                        }
                        cout << ostatnie_wyniki[i];
            }
            wyniki.close();
            }
            else cout << "Nie ma jeszcze takiego pliku!!!";
}

void menu::open_naj()
{
            system("cls");
            wyniki.open(plik_najlepsze.c_str(), ios::in| ios::binary);
            if(wyniki)
            {
            wyniki.seekg(55);
            wyniki.get(najlepsze_wyniki, sizeof(najlepsze_wyniki), EOF);
            cout << "Oto najlepsze Twoje wyniki";
            for(int i = 0; i < sizeof(najlepsze_wyniki); i++)
            {
                        if(najlepsze_wyniki[i] == EOF) break;
                        if(najlepsze_wyniki[i] == '\n')
                        {
                                    cout << endl;
                                    continue;
                        }
                        cout << najlepsze_wyniki[i];
            }
            wyniki.close();
            }
            else cout << "Nie ma jeszcze takiego pliku!!!";
}

void menu::wypisz_menu()
{
            SetConsoleTextAttribute( hOut,7  );
            cout << "########################################" << endl;
            cout << "###### :::::::: " ;
            SetConsoleTextAttribute( hOut,6  );
            cout << "MENU GRY";
            SetConsoleTextAttribute( hOut,7  );
            cout <<" :::::::: ######" << endl;
            cout << "########################################" << endl;
            cout << "###" ;
            SetConsoleTextAttribute( hOut,15  );
            cout <<  " 1. ";
            SetConsoleTextAttribute( hOut,7 );
            cout << "::::::::   ";
            SetConsoleTextAttribute( hOut,6 );
            cout << "START  ";
            SetConsoleTextAttribute( hOut,7 );
            cout << "::::::::::: ###" << endl;
            cout << "###" ;
            SetConsoleTextAttribute( hOut,15  );
            cout <<  " 2. ";
            SetConsoleTextAttribute( hOut,7 );
            cout << "::::::::  ";
            SetConsoleTextAttribute( hOut,6 );
            cout << "WYNIKI  ";
            SetConsoleTextAttribute( hOut,7 );
            cout << "::::::::::: ###" << endl;
            cout << "###" ;
            SetConsoleTextAttribute( hOut,15  );
            cout <<  " 3. ";
            SetConsoleTextAttribute( hOut,7 );
            cout << "::::::::  ";
            SetConsoleTextAttribute( hOut,6 );
            cout << "O GRZE  ";
            SetConsoleTextAttribute( hOut,7 );
            cout << "::::::::::: ###" << endl;
            cout << "###" ;
            SetConsoleTextAttribute( hOut,15  );
            cout <<  " 4. ";
            SetConsoleTextAttribute( hOut,7 );
            cout << "::::::::  ";
            SetConsoleTextAttribute( hOut,6 );
            cout << "AUTORZY ";
            SetConsoleTextAttribute( hOut,7 );
            cout << "::::::::::: ###" << endl;
            cout << "###" ;
            SetConsoleTextAttribute( hOut,15  );
            cout <<  " 0. ";
            SetConsoleTextAttribute( hOut,7 );
            cout << "::::::::  ";
            SetConsoleTextAttribute( hOut,6 );
            cout << "WYJSCIE ";
            SetConsoleTextAttribute( hOut,7 );
            cout << "::::::::::: ###" << endl;
            cout << "########################################" << endl;
            cout << "########################## " ;
            SetConsoleTextAttribute( hOut,12 );
            cout << "ver. 1.00";
            SetConsoleTextAttribute( hOut,7 );
            cout <<" ###" << endl;
            cout << "########################################" << endl;


}

void menu::wypisz_start()
{
            cout <<"\t\t\t###  ###   ##  ###  #  #   #\n\t\t\t#     #   #  #  #   #  #   #\n\t\t\t###   #   ####  #   ###    #\n\t\t\t"
            "  #   #   #  #  #   #  #   #\n\t\t\t###   #   #  #  #   #  #   #";



            for(int i = 0; ; i++)
            {
                        if(clock()/CLOCKS_PER_SEC == 3)
                        {
                                    system("cls");
                                    break;
                        }
            }
}


//
#endif
