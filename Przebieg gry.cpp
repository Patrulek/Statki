
string plik(300, (char)32);
string plik_ostatnich(350, (char)32);
string pamiec_pliku(300, (char)32);
string pamiec_ostatnich(350, (char)32);
extern plansza *PLANSZA;
extern menu *MENU;
int komparator(const void *a, const void *b);

enum przebieg                {         czysty = 0,    //-------------na mapie '~'--------------//
                                                                        pudlo,  //-------------na mapie '@'-------------//
                                                                        trafiony,   //---------na mapie '*' ------------//
                                                                        zatopiony, // --------na mapie '#'------------//
                                                                        statek, //-------------na mapie 'X' --------(widaæ tylko na swojej planszy xd)------//
                                                                        };

enum rodzaj_statku  { jednomaszt = 1,                       // s³u¿y do ustalenia jaki statek stoi na danym polu //
                                                dwumaszt,
                                                trojmaszt,
                                                czteromaszt,
                                                wrak        };


class przebieg_gry
{
            private:
                        time_t czas_st;                         //-----------pomocnicza dana do losowania wspolrzednych -------------//
                        time_t czas;                                    //--------pomocnicza dana do losowania wspolrzednych--------------//
                        char wspolrzedna_x;                 //---------wspolrzedna x jaka podaje gracz -----------//
                        char wspolrzedna_y;                 //----------wspolrzedna y jaka podaje gracz-----------//
                        string do_wspolrzednej_x;           //--------string przechowujacy wpisana wspolrzedna----//
                        stringstream do_wspolrzednej_xx;  //----strumieñ przesy³aj¹cy ze stringu do char=-----//
                        string do_wspolrzednej_y;                       //---------
                        stringstream do_wspolrzednej_yy;    //----------
                        string do_napisu_koncowego;
                        stringstream s_do_napisu;
                        stringstream s_do_napisu_2;
                        int tab_wynik[11];
                        int wynik_ostatnich[11];
                        static int ktora_gra_wygrana;
                        static int ktora_gra_przegrana;


                        string do_zapisu_pliku;
                        fstream zapis_do_pliku;                        //-------strumien do zapisywania wynikow do pliku ----- //
                        fstream zapis_do_ostatnich;
                        stringstream s_do_zapisu_pliku;
                        char tablica_wynikow[65];
                        char tablica_wynikow_cala[300];
                        char tablica_ostatnich[350];



                        int jaki_punkt[24][12];             // ----------tablica do okreslania co w danym momencie oznacza pkt na planszy(enum przebieg)----------//
                        int jaki_punkt_komp[24][12]; //----------jak wy¿ej dla CPU----------//
                        int jaki_statek[24][12];             //----------tablica do okreslania jaki statek znajduje sie w danym momencie na pkcie planszy -----------//
                        int jaki_statek_komp[24][12]; //--------jak wyzej dla CPU-----------//
                        int ruchy;              //maksymalna liczba strzalow graczy
                        int liczba_wrakow;                  //liczba okreslajaca ile 'wrakow' na planszy posiada gracz
                        int liczba_wrakow_komp;

                        void odstrzal();                    //-----------------funkcja do zmiany statusu pól gracza -------------------//
                        void odstrzal_statku();             //-----------funkcja do zmiany statusu pól dla statków gracza ------//
                        void odstrzal_komp();               // -analogicznie z powy¿szym----/
                        void odstrzal_statku_komp(); // ---patrz wyzej-----//
                        void wykonaj_strzal_x();            //---------funkcja sprawdzajaca poprawnosc podanej wspolrzednej x----------//
                        void wykonaj_strzal_y();            //---------funkcja sprawdzajaca poprawnosc podanej wspolrzednej y-----------//
                        void wykonaj_strzal();              // ----------funkcja sluzaca do oddania strzalu przez gracza-------------//
                        void przeciwnik_strzal();           // ---------funkcja sluzaca do oddania strzalu przez komputer-----//
                        void zapisz_wyniki();
                        void wypisz_ekran_koncowy();
                        string koncowy_napis(string a, string b);  // ---------------podmienia string z koncowym napisem --------------//




            public:
                        void akcja_gry();                               //--------funkcja sluzaca do okreslenia skutku strzalu------------//
                        przebieg_gry();                                             //--konstruktor--//
                        friend class jednomasztowiec;
                        friend class dwumasztowiec;
                        friend class trojmasztowiec;
                        friend class czteromasztowiec;
                        friend class plansza;

};

extern przebieg_gry *GRA;
int przebieg_gry::ktora_gra_wygrana = 0;
int przebieg_gry::ktora_gra_przegrana = 0;

int komparator(const void* a, const void* b)
{
   if (* (int *)a> * (int *) b) return 1;
   if (* (int *)a == * (int *) b) return 0;
   if (* (int *)a <* (int *) b) return -1;
}




przebieg_gry::przebieg_gry() : liczba_wrakow(268), liczba_wrakow_komp(268), do_napisu_koncowego(80, '#'), ruchy(100) //inicjalizuje czas startowy dla funkcji srand oraz ustawia ka¿de pole do gry jako(czysty) oraz ka¿de pole na planszy jako(wrak)
{
            time(&czas_st);

            for(int i = 0; i < 24; i++)
                        for(int k = 0; k < 12; k++)
                        {
                                    jaki_punkt[i][k] = czysty;
                                    jaki_statek[i][k] = wrak;
                                    jaki_punkt_komp[i][k] = czysty;
                                    jaki_statek_komp[i][k] = wrak;
                        }

            for(int i = 0; i < 24; i++)
            {
                        jaki_punkt[i][0] = pudlo;
                        jaki_punkt[i][11] = pudlo;
                        jaki_punkt_komp[i][0] = pudlo;
                        jaki_punkt_komp[i][11] = pudlo;
            }
            for(int k = 0; k < 12; k++)
            {
                        jaki_punkt[0][k] = pudlo;
                        jaki_punkt[22][k] = pudlo;
                        jaki_punkt_komp[0][k] = pudlo;
                        jaki_punkt_komp[22][k] = pudlo;
                        for(int z = 0; z < 12; z++)
                        {
                                                jaki_punkt[2*k+1][z] = pudlo;
                                                jaki_punkt_komp[2*k+1][z] = pudlo;
                        }
            }

}

void przebieg_gry::wykonaj_strzal()
{
                       wykonaj_strzal_x();
                       wykonaj_strzal_y();
                        odstrzal_komp();


}


void przebieg_gry::przeciwnik_strzal()
{
            // losowanie wspolrzednych do oddania strzalu
                        do
                        {
                                    srand(rand()*time(&czas));
                                    wspolrzedna_x = rand() %10+48 ;
                                    if(wspolrzedna_x >47 && wspolrzedna_x < 58)  break;
                        }
                        while(1);

                        do
                        {
                                    srand(rand()*time(&czas));
                                    wspolrzedna_y = rand() %10+97 ;
                                    wspolrzedna_y = (char)tolower(wspolrzedna_y);
                                    if((wspolrzedna_y > 96 && wspolrzedna_y < 107)) break;
                        }
                        while(1);

                        odstrzal();
}



void przebieg_gry::akcja_gry()
{
            liczba_wrakow_komp = 268;
            liczba_wrakow = 268;
            do
            {

                        ruchy-- ;
                        wykonaj_strzal();
                        if(liczba_wrakow == 288 || liczba_wrakow_komp == 288)
                                                break;

                        przeciwnik_strzal();
                        if(liczba_wrakow == 288 || liczba_wrakow_komp == 288)
                                                break;


            }
            while(ruchy > 0);

            wypisz_ekran_koncowy();

}

void przebieg_gry::odstrzal()
{

                       // konsekwencje oddanego strzalu
                                   // jeœli pkt w który strzelono zosta³ wczeœniej trafiony, spud³owany lub gdy zatopiono tam statek
                        if(jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] == pudlo || jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] == trafiony
                                       || jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] == zatopiony)
                                    {
                                                do
                                                {
                                                            srand(rand()*czas_st);
                                                            wspolrzedna_x = rand() %10+48;
                                                            wspolrzedna_y = rand() %10+97;
                                                            if(jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] != pudlo && jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] != trafiony
                                                            && jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] != zatopiony) break;
                                                }
                                                while(1) ;
                                    if(jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] == statek)
                                    {
                                                odstrzal_statku();
                                    }
                                    else
                                    {
                                                PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+((wspolrzedna_x-48)*2+2), 1, "@", 0, 1);
                                                system("cls");
                                                cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                PLANSZA->rysuj_ze_statkami();
                                                cout  << "\n\nKomputer strzelil w: " << (char)wspolrzedna_x << " " << (char)wspolrzedna_y << " i spudlowal." << endl;
                                                getchar();
                                                jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = pudlo;
                                    }

                                    }

            // co siê stanie gdy strzelono w pkt ktory nie by³ wczeœniej strzelony, ani gdzie nie jest ustawiony statek

                     if(jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] == czysty)
                        {
                                                PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-(46-wspolrzedna_x)-48), 1, "@", 0, 1);
                                                system("cls");
                                                cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                PLANSZA->rysuj_ze_statkami();
                                                cout  << "\n\nKomputer strzelil w: " << (char)wspolrzedna_x << " " << (char)wspolrzedna_y << " i spudlowal." << endl;
                                                getchar();
                                                jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = pudlo;
                        }

                        odstrzal_statku();

}

void przebieg_gry::odstrzal_statku()
{
            // jeœli statek jest jednomasztowcem
                                                if(jaki_statek[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] == jednomaszt)
                                            {
                                                            PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-(46-wspolrzedna_x)-48), 1, "#", 0, 1);
                                                            system("cls");
                                                            cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                            PLANSZA->rysuj_ze_statkami();
                                                            cout  << "\n\nKomputer strzelil w: " << (char)wspolrzedna_x << " " << (char)wspolrzedna_y << " i zatopil Twoj jednomasztowiec!!!" << endl;
                                                            getchar();
                                                            jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                            jaki_statek[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = wrak;
                                                            liczba_wrakow ++ ;
                                            }
            // jeœli statek jest dwumasztowcem
                                                if(jaki_statek[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] == dwumaszt)
                                                {
                                                                       PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-(46-wspolrzedna_x)-48), 1, "*", 0, 1);
                                                                        system("cls");
                                                                        cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                        PLANSZA->rysuj_ze_statkami();
                                                                        cout  << "\n\nKomputer strzelil w: " << (char)wspolrzedna_x << " " << (char)wspolrzedna_y << " i trafil Twoj dwumasztowiec." << endl;
                                                                        getchar();
                                                                        jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = trafiony;
                                                                        jaki_statek[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = wrak;
                                                                        liczba_wrakow ++;

   /*po prawej*/                                        if(jaki_punkt[(wspolrzedna_x-48)*2+4][wspolrzedna_y-96] == trafiony)
                                                                        {
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+4, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout  << "\n\nKomputer zatopil Twoj dwumasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+4][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                        }

 /* po lewej*/                                            if(jaki_punkt[(wspolrzedna_x-48)*2][wspolrzedna_y-96] == trafiony)
                                                                        {
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout  << "\n\nKomputer zatopil Twoj dwumasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                        }
                                                                        if(jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-97] == trafiony)
                                                                        {
 /* z góry*/                                                            PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-97)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout  << "\n\nKomputer zatopil Twoj dwumasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-97] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                        }
                                                                        if(jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-95] == trafiony)
                                                                        {
/* z do³u */                                                            PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-95)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout  << "\n\nKomputer zatopil Twoj dwumasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-95] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                        }
                                                           }

            // jeœli statek jest trójmasztowcem
                                    if(jaki_statek[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] == trojmaszt)
                                                {
                                                                       PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "*", 0, 1);
                                                                        system("cls");
                                                                        cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                        PLANSZA->rysuj_ze_statkami();
                                                                        cout  << "\n\nKomputer strzelil w: " << (char)wspolrzedna_x << " " << (char)wspolrzedna_y << " i trafil Twoj trojmasztowiec." << endl;
                                                                        getchar();
                                                                        jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = trafiony;
                                                                        jaki_statek[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = wrak;
                                                                        liczba_wrakow ++;

   /*po prawej i lewej*/                          if(jaki_punkt[(wspolrzedna_x-48)*2+4][wspolrzedna_y-96] == trafiony && jaki_punkt[(wspolrzedna_x-48)*2][wspolrzedna_y-96] == trafiony)
                                                                        {
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+4, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout  << "\n\nKomputer zatopil Twoj trojmasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+4][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2][wspolrzedna_y-96] = zatopiony;
                                                                        }

 /* dwa po prawej*/                                if(jaki_punkt[(wspolrzedna_x-48)*2+4][wspolrzedna_y-96] == trafiony && jaki_punkt[(wspolrzedna_x-48)*2+6][wspolrzedna_y-96] == trafiony)
                                                                        {
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+4, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+6, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout  << "\n\nKomputer zatopil Twoj trojmasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+4][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+6][wspolrzedna_y-96] = zatopiony;
                                                                        }
                                                                        if(jaki_punkt[(wspolrzedna_x-48)*2][wspolrzedna_y-96] == trafiony && jaki_punkt[(wspolrzedna_x-48)*2-2][wspolrzedna_y-96] == trafiony)
                                                                        {
 /* dwa po lewej*/                                              PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2-2, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                     cout  << "\n\nKomputer zatopil Twoj trojmasztowiec!!!" << endl;
                                                                                     getchar();
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2-2][wspolrzedna_y-96] = zatopiony;
                                                                        }
                                                                        if(jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-95] == trafiony && jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-97] == trafiony)
                                                                        {
/* z do³u i góry */                                               PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-95)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-97)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                     cout  << "\n\nKomputer zatopil Twoj trojmasztowiec!!!" << endl;
                                                                                     getchar();
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-95] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-97] = zatopiony;
                                                                        }
                                                                        if(jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-95] == trafiony && jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-94] == trafiony)
                                                                        {
/* dwa z do³u */                                                   PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-95)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-94)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                     cout  << "\n\nKomputer zatopil Twoj trojmasztowiec!!!" << endl;
                                                                                     getchar();
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-95] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-94] = zatopiony;
                                                                        }
                                                                        if(jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-97] == trafiony && jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-98] == trafiony)
                                                                        {
/* dwa z góry */                                                  PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-97)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-98)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout  << "\n\nKomputer zatopil Twoj trojmasztowiec!!!" << endl;
                                                                                     getchar();
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-97] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-98] = zatopiony;
                                                                        }
                                                           }

            // jeœli statek jest czteromasztowcem
                                    if(jaki_statek[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] == czteromaszt)
                                                {
                                                                       PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "*", 0, 1);
                                                                        system("cls");
                                                                        cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                        PLANSZA->rysuj_ze_statkami();
                                                                         cout  << "\n\nKomputer strzelil w: " << (char)wspolrzedna_x << " " << (char)wspolrzedna_y << " i trafil Twoj czteromasztowiec." << endl;
                                                                         getchar();
                                                                        jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = trafiony;
                                                                        jaki_statek[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = wrak;
                                                                        liczba_wrakow ++;

   /*2 po prawej i 1 po lewej*/            if(jaki_punkt[(wspolrzedna_x-48)*2+4][wspolrzedna_y-96] == trafiony && jaki_punkt[(wspolrzedna_x-48)*2+6][wspolrzedna_y-96] == trafiony && jaki_punkt[(wspolrzedna_x-48)*2][wspolrzedna_y-96] == trafiony)
                                                                        {
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+4, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+6, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                     cout  << "\n\nKomputer zatopil Twoj czteromasztowiec!!!" << endl;
                                                                                     getchar();
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+4][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+6][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2][wspolrzedna_y-96] = zatopiony;
                                                                        }

 /* 2 po lewej i 1 po prawej*/             if(jaki_punkt[(wspolrzedna_x-48)*2+4][wspolrzedna_y-96] == trafiony && jaki_punkt[(wspolrzedna_x-48)*2][wspolrzedna_y-96] == trafiony && jaki_punkt[(wspolrzedna_x-48)*2-2][wspolrzedna_y-96] == trafiony)
                                                                        {
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+4, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2-2, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout  << "\n\nKomputer zatopil Twoj czteromasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+4][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2-2][wspolrzedna_y-96] = zatopiony;
                                                                        }
/* 3 po prawej*/                                      if(jaki_punkt[(wspolrzedna_x-48)*2+4][wspolrzedna_y-96] == trafiony && jaki_punkt[(wspolrzedna_x-48)*2+6][wspolrzedna_y-96] == trafiony && jaki_punkt[(wspolrzedna_x-48)*2+8][wspolrzedna_y-96] == trafiony)
                                                                        {
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+4, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+6, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+8, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout  << "\n\nKomputer zatopil Twoj czteromasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+4][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+6][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+8][wspolrzedna_y-96] = zatopiony;
                                                                        }
/* 3 po lewej*/                                      if(jaki_punkt[(wspolrzedna_x-48)*2][wspolrzedna_y-96] == trafiony && jaki_punkt[(wspolrzedna_x-48)*2-2][wspolrzedna_y-96] == trafiony && jaki_punkt[(wspolrzedna_x-48)*2-4][wspolrzedna_y-96] == trafiony)
                                                                        {
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2-2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2-4, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout  << "\n\nKomputer zatopil Twoj czteromasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2-2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2-4][wspolrzedna_y-96] = zatopiony;
                                                                        }
/* 3 z gory*/                                             if(jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-97] == trafiony && jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-98] == trafiony && jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-99] == trafiony)
                                                                        {
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-97)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-98)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-99)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout  << "\n\nKomputer zatopil Twoj czteromasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-97] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-98] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-99] = zatopiony;
                                                                        }
/* 3 z do³u*/                                              if(jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-95] == trafiony && jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-94] == trafiony && jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-93] == trafiony)
                                                                        {
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-95)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-94)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-93)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout  << "\n\nKomputer zatopil Twoj czteromasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-95] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-94] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-93] = zatopiony;
                                                                        }
/* 2 z do³u i 1 z góry*/                           if(jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-95] == trafiony && jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-94] == trafiony && jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-97] == trafiony)
                                                                        {
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-95)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-94)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-97)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout  << "\n\nKomputer zatopil Twoj czteromasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-95] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-94] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-97] = zatopiony;
                                                                        }
/* 2 z gory i 1 z do³u*/                           if(jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-95] == trafiony && jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-97] == trafiony && jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-98] == trafiony)
                                                                        {
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-95)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-97)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-98)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+2, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout  << "\n\nKomputer zatopil Twoj czteromasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-95] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-97] = zatopiony;
                                                                                    jaki_punkt[(wspolrzedna_x-48)*2+2][wspolrzedna_y-98] = zatopiony;
                                                                        }
                                                           }
}


void przebieg_gry::wykonaj_strzal_x()
{
            cin.sync();
             do
                        {
                                    cin.sync();
                                    system("cls");
                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                    PLANSZA->rysuj_ze_statkami();
                                    cout <<endl <<endl << "Podaj pierwsza wspolrzedna strzalu: ";
                                    getline(cin, do_wspolrzednej_x);
                                    if(do_wspolrzednej_x.length() == 1)
                                    {
                                                do_wspolrzednej_xx << do_wspolrzednej_x;
                                                do_wspolrzednej_xx >> wspolrzedna_x;
                                                do_wspolrzednej_x.clear();
                                                if(isdigit((int)wspolrzedna_x)) break;
                                                else
                                                {
                                                             system("cls");
                                                            cout << "Bledna wspolrzedna!!!";
                                                            cin.ignore();
                                                            system("cls");
                                                            cin.sync();
                                                }

                                    }


                                    else
                                    {
                                                system("cls");
                                                cout << "Bledna wspolrzedna!!!";
                                                cin.ignore();
                                                system("cls");
                                    }
                        }
                        while(1);
}

void przebieg_gry::wykonaj_strzal_y()
{

                        do
                        {
                                    cin.sync();
                                    cout  << endl << endl << "Podaj druga wspolrzedna strzalu: ";
                                    getline(cin, do_wspolrzednej_y);
                                    if(do_wspolrzednej_y.length() == 1)
                                    {
                                                do_wspolrzednej_yy << do_wspolrzednej_y;
                                                do_wspolrzednej_yy >> wspolrzedna_y;
                                                do_wspolrzednej_y.clear();
                                                if(isalpha((int)wspolrzedna_y))
                                                            if(wspolrzedna_y > 96 && wspolrzedna_y < 107) break;
                                                            else
                                                            {
                                                                        system("cls");
                                                                        cout << "Bledna wspolrzedna!!!";
                                                                        cin.ignore();
                                                                        system("cls");
                                                                        cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                        PLANSZA->rysuj_ze_statkami();
                                                                        cout <<endl <<endl << "Podaj pierwsza wspolrzedna strzalu: " << wspolrzedna_x << endl;
                                                            }
                                                else
                                                {
                                                            system("cls");
                                                            cout<< "Bledna wspolrzedna!!!";
                                                            cin.ignore();
                                                            system("cls");
                                                            cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                            PLANSZA->rysuj_ze_statkami();
                                                            cout <<endl <<endl << "Podaj pierwsza wspolrzedna strzalu: " << wspolrzedna_x << endl;
                                                }
                                    }
                                    else
                                    {
                                                system("cls");
                                                cout << "Bledna wspolrzedna!!!";
                                                cin.ignore();
                                                system("cls");
                                                cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                PLANSZA->rysuj_ze_statkami();
                                                cout <<endl <<endl << "Podaj pierwsza wspolrzedna strzalu: " << wspolrzedna_x << endl;
                                    }
                        }
                        while(1);
}


void przebieg_gry::odstrzal_komp()
{
                                   // konsekwencje oddanego strzalu
                                   // jeœli pkt w który strzelono zosta³ wczeœniej trafiony, spud³owany lub gdy zatopiono tam statek
                        if(jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] == pudlo || jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] == trafiony
                                       || jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] == zatopiony)
                                    {
                                                do
                                                {
                                                            if(jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] != pudlo && jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] != trafiony
                                                            && jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] != zatopiony) break;
                                                            else
                                                            {
                                                                        system("cls");
                                                                        cout << "\nTo miejsce zostalo juz ostrzelane, sprobuj gdzie indziej\n";
                                                                        getchar();
                                                                        system("cls");
                                                                        wykonaj_strzal_x();
                                                                        wykonaj_strzal_y();
                                                            }
                                                }
                                                while(1) ;
                                                if(jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] == statek)
                                                {
                                                            odstrzal_statku_komp();
                                                }
                                                else
                                                {
                                                            PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "@", 0, 1);
                                                            system("cls");
                                                            cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                            PLANSZA->rysuj_ze_statkami();
                                                            cout << "\n\nStrzeliles w " << (char)wspolrzedna_x << " " << (char)wspolrzedna_y <<" i spudlowales" << endl;
                                                            getchar();
                                                            jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = pudlo;
                                                }

                                    }

            // co siê stanie gdy strzelono w pkt ktory nie by³ wczeœniej strzelony, ani gdzie nie jest ustawiony statek

                     if(jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] == czysty)
                        {
                                                PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "@", 0, 1);
                                                system("cls");
                                                cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                PLANSZA->rysuj_ze_statkami();
                                                cout << "\n\nStrzeliles w " << (char)wspolrzedna_x << " " << (char)wspolrzedna_y <<" i spudlowales" << endl;
                                                getchar();
                                                jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = pudlo;
                        }

                        odstrzal_statku_komp();
}

void przebieg_gry::odstrzal_statku_komp()
{
            // jeœli statek jest jednomasztowcem
                                                if(jaki_statek_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] == jednomaszt)
                                            {
                                                            PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                            system("cls");
                                                            cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                            PLANSZA->rysuj_ze_statkami();
                                                            cout << "\n\nStrzeliles w " << (char)wspolrzedna_x << " " << (char)wspolrzedna_y <<" i zatopiles wrogi jednomasztowiec!!!" << endl;
                                                            getchar();
                                                            jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                            jaki_statek_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = wrak;
                                                            liczba_wrakow_komp ++;
                                            }
            // jeœli statek jest dwumasztowcem
                                                if(jaki_statek_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] == dwumaszt)
                                                {
                                                                       PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "*", 0, 1);
                                                                        system("cls");
                                                                        cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                        PLANSZA->rysuj_ze_statkami();
                                                                        cout << "\n\nStrzeliles w " << (char)wspolrzedna_x << " " << (char)wspolrzedna_y <<" i trafiles wrogi statek." << endl;
                                                                        getchar();
                                                                        jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = trafiony;
                                                                        jaki_statek_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = wrak;
                                                                        liczba_wrakow_komp ++;

   /*po prawej*/                                        if(jaki_punkt_komp[(wspolrzedna_x-48)*2+4][wspolrzedna_y-96] == trafiony)
                                                                        {
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+52, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout << "\n\nZatopiles wrogi dwumasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+4][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                        }

 /* po lewej*/                                            if(jaki_punkt_komp[(wspolrzedna_x-48)*2][wspolrzedna_y-96] == trafiony)
                                                                        {
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+48, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout << "\n\nZatopiles wrogi dwumasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                        }
                                                                        if(jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-97] == trafiony)
                                                                        {
 /* z góry*/                                                            PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-97)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout << "\n\nZatopiles wrogi dwumasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-97] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                        }
                                                                        if(jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-95] == trafiony)
                                                                        {
/* z do³u */                                                            PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-95)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout << "\n\nZatopiles wrogi dwumasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-95] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                        }
                                                           }

            // jeœli statek jest trójmasztowcem
                                    if(jaki_statek_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] == trojmaszt)
                                                {
                                                                       PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "*", 0, 1);
                                                                        system("cls");
                                                                        cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                        PLANSZA->rysuj_ze_statkami();
                                                                        cout << "\n\nStrzeliles w " << (char)wspolrzedna_x << " " << (char)wspolrzedna_y <<" i trafiles wrogi statek." << endl;
                                                                        getchar();
                                                                        jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = trafiony;
                                                                        jaki_statek_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = wrak;
                                                                        liczba_wrakow_komp ++;

   /*po prawej i lewej*/                          if(jaki_punkt_komp[(wspolrzedna_x-48)*2+4][wspolrzedna_y-96] == trafiony && jaki_punkt_komp[(wspolrzedna_x-48)*2][wspolrzedna_y-96] == trafiony)
                                                                        {
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+52, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+48, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                   cout << "\n\nZatopiles wrogi trojmasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+4][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2][wspolrzedna_y-96] = zatopiony;
                                                                        }

 /* dwa po prawej*/                                if(jaki_punkt_komp[(wspolrzedna_x-48)*2+4][wspolrzedna_y-96] == trafiony && jaki_punkt_komp[(wspolrzedna_x-48)*2+6][wspolrzedna_y-96] == trafiony)
                                                                        {
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+52, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+54, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout << "\n\nZatopiles wrogi trojmasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+4][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+6][wspolrzedna_y-96] = zatopiony;
                                                                        }
                                                                        if(jaki_punkt_komp[(wspolrzedna_x-48)*2][wspolrzedna_y-96] == trafiony && jaki_punkt_komp[(wspolrzedna_x-48)*2-2][wspolrzedna_y-96] == trafiony)
                                                                        {
 /* dwa po lewej*/                                              PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+48, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+46, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout << "\n\nZatopiles wrogi trojmasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2-2][wspolrzedna_y-96] = zatopiony;
                                                                        }
                                                                        if(jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-95] == trafiony && jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-97] == trafiony)
                                                                        {
/* z do³u i góry */                                               PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-95)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-97)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout << "\n\nZatopiles wrogi trojmasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-95] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-97] = zatopiony;
                                                                        }
                                                                        if(jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-95] == trafiony && jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-94] == trafiony)
                                                                        {
/* dwa z do³u */                                                   PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-95)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-94)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout << "\n\nZatopiles wrogi trojmasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-95] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-94] = zatopiony;
                                                                        }
                                                                        if(jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-97] == trafiony && jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-98] == trafiony)
                                                                        {
/* dwa z góry */                                                  PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-97)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-98)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout << "\n\nZatopiles wrogi trojmasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-97] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-98] = zatopiony;
                                                                        }
                                                           }

            // jeœli statek jest czteromasztowcem
                                    if(jaki_statek_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] == czteromaszt)
                                                {
                                                                       PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "*", 0, 1);
                                                                        system("cls");
                                                                        cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                        PLANSZA->rysuj_ze_statkami();
                                                                        cout << "\n\nStrzeliles w " << (char)wspolrzedna_x << " " << (char)wspolrzedna_y <<" i trafiles wrogi statek." << endl;
                                                                        getchar();
                                                                        jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = trafiony;
                                                                        jaki_statek_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = wrak;
                                                                        liczba_wrakow_komp ++;

   /*2 po prawej i 1 po lewej*/            if(jaki_punkt_komp[(wspolrzedna_x-48)*2+4][wspolrzedna_y-96] == trafiony && jaki_punkt_komp[(wspolrzedna_x-48)*2+6][wspolrzedna_y-96] == trafiony && jaki_punkt_komp[(wspolrzedna_x-48)*2][wspolrzedna_y-96] == trafiony)
                                                                        {
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+52, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+54, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+48, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout << "\n\nZatopiles wrogi czteromasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+4][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+6][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2][wspolrzedna_y-96] = zatopiony;
                                                                        }

 /* 2 po lewej i 1 po prawej*/             if(jaki_punkt_komp[(wspolrzedna_x-48)*2+4][wspolrzedna_y-96] == trafiony && jaki_punkt_komp[(wspolrzedna_x-48)*2][wspolrzedna_y-96] == trafiony && jaki_punkt_komp[(wspolrzedna_x-48)*2-2][wspolrzedna_y-96] == trafiony)
                                                                        {
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+52, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+48, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+46, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout << "\n\nZatopiles wrogi czteromasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+4][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2-2][wspolrzedna_y-96] = zatopiony;
                                                                        }
/* 3 po prawej*/                                      if(jaki_punkt_komp[(wspolrzedna_x-48)*2+4][wspolrzedna_y-96] == trafiony && jaki_punkt_komp[(wspolrzedna_x-48)*2+6][wspolrzedna_y-96] == trafiony && jaki_punkt_komp[(wspolrzedna_x-48)*2+8][wspolrzedna_y-96] == trafiony)
                                                                        {
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+52, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+54, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+56, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout << "\n\nZatopiles wrogi czteromasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+4][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+6][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+8][wspolrzedna_y-96] = zatopiony;
                                                                        }
/* 3 po lewej*/                                      if(jaki_punkt_komp[(wspolrzedna_x-48)*2][wspolrzedna_y-96] == trafiony && jaki_punkt_komp[(wspolrzedna_x-48)*2-2][wspolrzedna_y-96] == trafiony && jaki_punkt_komp[(wspolrzedna_x-48)*2-4][wspolrzedna_y-96] == trafiony)
                                                                        {
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+48, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+46, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+44, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout << "\n\nZatopiles wrogi czteromasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2-2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2-4][wspolrzedna_y-96] = zatopiony;
                                                                        }
/* 3 z gory*/                                             if(jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-97] == trafiony && jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-98] == trafiony && jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-99] == trafiony)
                                                                        {
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-97)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-98)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-99)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout << "\n\nZatopiles wrogi czteromasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-97] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-98] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-99] = zatopiony;
                                                                        }
/* 3 z do³u*/                                              if(jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-95] == trafiony && jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-94] == trafiony && jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-93] == trafiony)
                                                                        {
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-95)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-94)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-93)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout << "\n\nZatopiles wrogi czteromasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-95] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-94] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-93] = zatopiony;
                                                                        }
/* 2 z do³u i 1 z góry*/                           if(jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-95] == trafiony && jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-94] == trafiony && jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-97] == trafiony)
                                                                        {
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-95)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-94)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-97)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout << "\n\nZatopiles wrogi czteromasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-95] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-94] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-97] = zatopiony;
                                                                        }
/* 2 z gory i 1 z do³u*/                           if(jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-95] == trafiony && jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-97] == trafiony && jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-98] == trafiony)
                                                                        {
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-96)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-95)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-97)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    PLANSZA->mapa.replace((wspolrzedna_y-98)*PLANSZA->szerokosc*3+1+(wspolrzedna_x-48)*2+50, 1, "#", 0, 1);
                                                                                    system("cls");
                                                                                    cout << "Plansza z Twoimi statkami\t\t    Plansza ze statkami przeciwnika\n\n";
                                                                                    PLANSZA->rysuj_ze_statkami();
                                                                                    cout << "\n\nZatopiles wrogi czteromasztowiec!!!" << endl;
                                                                                    getchar();
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-96] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-95] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-97] = zatopiony;
                                                                                    jaki_punkt_komp[(wspolrzedna_x-48)*2+2][wspolrzedna_y-98] = zatopiony;
                                                                        }
                                                           }
}

void przebieg_gry::zapisz_wyniki()
{
                                    SYSTEMTIME st;
                                    GetSystemTime(&st);
                                    int rok=st.wYear;
                                    int mies=st.wMonth;
                                    int dzien=st.wDay;

                        if(ktora_gra_wygrana < 11)
                        {

                                    if(ktora_gra_wygrana == 1)
                                    {

                                                zapis_do_pliku.open(MENU->plik_najlepsze.c_str(), fstream::out | fstream::trunc | fstream::binary);
                                                zapis_do_ostatnich.open(MENU->plik_ostatnie.c_str(), fstream::out | fstream::trunc | fstream::binary);
                                                s_do_zapisu_pliku << (100-ruchy);
                                                if(ruchy > 0 && ruchy < 91)
                                                {
                                                            pamiec_pliku.replace(0, 2, s_do_zapisu_pliku.str(), 0, 2 );
                                                            pamiec_ostatnich.replace(0, 2, s_do_zapisu_pliku.str(), 0, 2 );
                                                }
                                                else
                                                {
                                                            pamiec_pliku.replace(0, 1, s_do_zapisu_pliku.str(), 0, 1 );
                                                            pamiec_ostatnich.replace(0, 1, s_do_zapisu_pliku.str(), 0, 1 );
                                                }


                                                s_do_zapisu_pliku.str("");
                                                s_do_zapisu_pliku << "\n\n" << ktora_gra_wygrana << ". miejsce - " << (100 - ruchy);
                                                pamiec_pliku.replace(55, 20, s_do_zapisu_pliku.str(), 0, 20);
                                                s_do_zapisu_pliku.str("");
                                                s_do_zapisu_pliku << "\n\n" << "Czas: " <<dzien << "/" << mies << "/" << rok << "\tWynik: " << (100-ruchy);
                                                pamiec_ostatnich.replace(55, 29, s_do_zapisu_pliku.str(), 0, 29);
                                                s_do_zapisu_pliku.str("");
                                                zapis_do_pliku << pamiec_pliku;
                                                zapis_do_ostatnich << pamiec_ostatnich;
                                                zapis_do_pliku.close();
                                    }
                                    else
                                    {
                                                pamiec_pliku.clear();
                                                pamiec_ostatnich.clear();

                                                pamiec_pliku = plik;
                                                pamiec_ostatnich = plik_ostatnich;

                                                zapis_do_pliku.open(MENU->plik_najlepsze.c_str(), fstream::in | fstream::binary);
                                                zapis_do_ostatnich.open(MENU->plik_ostatnie.c_str(), fstream::in | fstream::binary);

                                                zapis_do_pliku.get(tablica_wynikow_cala, sizeof(tablica_wynikow_cala), EOF);
                                                zapis_do_ostatnich.get(tablica_ostatnich, sizeof(tablica_ostatnich), EOF);

                                                zapis_do_pliku.clear();
                                                zapis_do_ostatnich.clear();

                                                zapis_do_pliku.close();
                                                zapis_do_ostatnich.close();

                                                zapis_do_pliku.open(MENU->plik_najlepsze.c_str(), fstream::out | fstream::trunc | fstream::binary);
                                                zapis_do_ostatnich.open(MENU->plik_ostatnie.c_str(), fstream::out | fstream::trunc | fstream::binary);

                                                for(int i = 0; i < sizeof(tablica_wynikow_cala); i++)
                                                {
                                                            pamiec_pliku[i] = tablica_wynikow_cala[i];
                                                }
                                                for(int i = 0; i < sizeof(tablica_ostatnich); i++)
                                                {
                                                            pamiec_ostatnich[i] = tablica_ostatnich[i];
                                                }

                                                s_do_zapisu_pliku << (100-ruchy);

                                                if(ruchy > 0 && ruchy < 91)
                                                {
                                                            pamiec_pliku.replace((ktora_gra_wygrana-1)*5, 2, s_do_zapisu_pliku.str(), 0, 2 );
                                                            pamiec_ostatnich.replace((ktora_gra_wygrana-1)*5, 2, s_do_zapisu_pliku.str(), 0, 2 );
                                                }

                                                else
                                                {
                                                            pamiec_pliku.replace((ktora_gra_wygrana-1)*5, 1, s_do_zapisu_pliku.str(), 0, 1 );
                                                            pamiec_pliku.replace(((ktora_gra_wygrana-1)*5+1), 1, " ", 0, 1 );
                                                            pamiec_ostatnich.replace((ktora_gra_wygrana-1)*5, 1, s_do_zapisu_pliku.str(), 0, 1 );
                                                            pamiec_ostatnich.replace(((ktora_gra_wygrana-1)*5+1), 1, " ", 0, 1 );
                                                }

                                                s_do_zapisu_pliku.str("");
                                                s_do_zapisu_pliku << "\n\n" << ktora_gra_wygrana << ". miejsce - " ;
                                                pamiec_pliku.replace((55+(ktora_gra_wygrana-1)*19), 20, s_do_zapisu_pliku.str(), 0, 20);
                                                s_do_zapisu_pliku.str("");
                                                s_do_zapisu_pliku << "\n\n" << "Czas: " <<dzien << "/" << mies << "/" << rok << "\tWynik: ";
                                                pamiec_ostatnich.replace((55+(ktora_gra_wygrana-1)*29), 29, s_do_zapisu_pliku.str(), 0, 29);

                                                s_do_zapisu_pliku.str("");
                                                do_zapisu_pliku = pamiec_pliku.substr(0, 55);
                                                s_do_zapisu_pliku << do_zapisu_pliku;
                                                do_zapisu_pliku.clear();
                                                for(int i = 0; i < ktora_gra_wygrana; i++)
                                                {
                                                            s_do_zapisu_pliku >>  tab_wynik[i];
                                                }
                                                s_do_zapisu_pliku.str("");
                                                do_zapisu_pliku = pamiec_ostatnich.substr(0, 55);
                                                s_do_zapisu_pliku << do_zapisu_pliku;
                                                do_zapisu_pliku.clear();
                                                for(int i = 0; i < ktora_gra_wygrana; i++)
                                                {
                                                            s_do_zapisu_pliku >>  wynik_ostatnich[i];
                                                }

                                                s_do_zapisu_pliku.str("");

                                                qsort(tab_wynik, ktora_gra_wygrana, sizeof(int), komparator);
                                                for(int i = 0; i < ktora_gra_wygrana; i++)
                                                {

                                                            s_do_zapisu_pliku << tab_wynik[i];
                                                            s_do_zapisu_pliku >> do_zapisu_pliku;

                                                            if(do_zapisu_pliku.size() == 2)
                                                                        if(i+1 == 10)
                                                                        {
                                                                                    pamiec_pliku.replace(55+(i)*19+16, 2, do_zapisu_pliku, 0, 2);
                                                                        }
                                                                        else
                                                                        {
                                                                                    pamiec_pliku.replace(55+(i)*19+15, 2, do_zapisu_pliku, 0, 2);
                                                                        }

                                                            else
                                                            {
                                                                        if(i+1 == 10)
                                                                        {
                                                                                    pamiec_pliku.replace(55+(i)*19+16, 1, do_zapisu_pliku, 0, 1);
                                                                                    pamiec_pliku.replace(55+(i)*19+17, 1, " ", 0, 1);
                                                                        }
                                                                        else
                                                                        {
                                                                                    pamiec_pliku.replace(55+(i)*19+15, 1, do_zapisu_pliku, 0, 1);
                                                                                    pamiec_pliku.replace(55+(i)*19+16, 1, " ", 0, 1);
                                                                        }

                                                            }

                                                            s_do_zapisu_pliku.str("");
                                                            s_do_zapisu_pliku.clear();
                                                            do_zapisu_pliku.clear();
                                                }
                                                for(int i = 0; i < ktora_gra_wygrana; i++)
                                                {
                                                            s_do_zapisu_pliku << wynik_ostatnich[i];
                                                            s_do_zapisu_pliku >> do_zapisu_pliku;

                                                            if(do_zapisu_pliku.size() == 2)
                                                                        pamiec_ostatnich.replace(55+(ktora_gra_wygrana-1-i)*29+24, 2, do_zapisu_pliku, 0, 2);
                                                            else
                                                            {
                                                                        pamiec_ostatnich.replace(55+(ktora_gra_wygrana-1-i)*29+24, 1, do_zapisu_pliku, 0, 1);
                                                                        pamiec_ostatnich.replace(55+(ktora_gra_wygrana-1-i)*29+25, 1, " ", 0, 1);
                                                            }

                                                            s_do_zapisu_pliku.str("");
                                                            s_do_zapisu_pliku.clear();
                                                            do_zapisu_pliku.clear();
                                                }
                                                zapis_do_pliku.clear();
                                                zapis_do_ostatnich.clear();

                                                zapis_do_pliku << pamiec_pliku;
                                                zapis_do_ostatnich << pamiec_ostatnich;

                                                zapis_do_pliku.close();
                                                zapis_do_ostatnich.close();
                                    }

                        }
                        else
                        {
                                                pamiec_pliku.clear();
                                                pamiec_ostatnich.clear();

                                                pamiec_pliku = plik;
                                                pamiec_ostatnich = plik_ostatnich;

                                                zapis_do_pliku.open(MENU->plik_najlepsze.c_str(), fstream::in | fstream::binary);
                                                zapis_do_ostatnich.open(MENU->plik_ostatnie.c_str(), fstream::in | fstream::binary);

                                                zapis_do_pliku.get(tablica_wynikow_cala, sizeof(tablica_wynikow_cala), EOF);
                                                zapis_do_ostatnich.get(tablica_ostatnich, sizeof(tablica_ostatnich), EOF);

                                                zapis_do_pliku.clear();
                                                zapis_do_ostatnich.clear();

                                                zapis_do_pliku.close();
                                                zapis_do_ostatnich.close();

                                                zapis_do_pliku.open(MENU->plik_najlepsze.c_str(), fstream::out | fstream::trunc | fstream::binary);
                                                zapis_do_ostatnich.open(MENU->plik_ostatnie.c_str(), fstream::out | fstream::trunc | fstream::binary);

                                                for(int i = 0; i < sizeof(tablica_wynikow_cala); i++)
                                                {
                                                            pamiec_pliku[i] = tablica_wynikow_cala[i];
                                                }
                                                for(int i = 0; i < sizeof(tablica_ostatnich); i++)
                                                {
                                                            pamiec_ostatnich[i] = tablica_ostatnich[i];
                                                }

                                                s_do_zapisu_pliku << (100-ruchy);

                                                if(ruchy > 0 && ruchy < 91)
                                                {
                                                            pamiec_pliku.replace(50, 2, s_do_zapisu_pliku.str(), 0, 2 );
                                                            pamiec_ostatnich.replace(50, 2, s_do_zapisu_pliku.str(), 0, 2 );
                                                }

                                                else
                                                {
                                                            pamiec_pliku.replace(50, 1, s_do_zapisu_pliku.str(), 0, 1 );
                                                            pamiec_pliku.replace(51, 1, " ", 0, 1 );
                                                            pamiec_ostatnich.replace(50, 1, s_do_zapisu_pliku.str(), 0, 1 );
                                                            pamiec_ostatnich.replace(51, 1, " ", 0, 1 );
                                                }

                                                s_do_zapisu_pliku.str("");
                                                do_zapisu_pliku = pamiec_pliku.substr(0, 55);
                                                s_do_zapisu_pliku << do_zapisu_pliku;
                                                do_zapisu_pliku.clear();
                                                for(int i = 0; i < 11; i++)
                                                {
                                                            s_do_zapisu_pliku >>  tab_wynik[i];
                                                }
                                                s_do_zapisu_pliku.str("");
                                                do_zapisu_pliku = pamiec_ostatnich.substr(0, 55);
                                                s_do_zapisu_pliku << do_zapisu_pliku;
                                                do_zapisu_pliku.clear();
                                                for(int i = 0; i < 11; i++)
                                                {
                                                            s_do_zapisu_pliku >>  wynik_ostatnich[i];
                                                }

                                                s_do_zapisu_pliku.str("");

                                                qsort(tab_wynik, 11, sizeof(int), komparator);
                                               for(int i = 0; i < 10; i++)
                                               {
                                                            s_do_zapisu_pliku << tab_wynik[i];
                                                            pamiec_pliku.replace(i*5, s_do_zapisu_pliku.str().size(), s_do_zapisu_pliku.str(), 0, s_do_zapisu_pliku.str().size());
                                                            pamiec_pliku.replace(55+(i)*19+16, s_do_zapisu_pliku.str().size(), s_do_zapisu_pliku.str(), 0, s_do_zapisu_pliku.str().size());
                                                            s_do_zapisu_pliku.str("");
                                                            s_do_zapisu_pliku.clear();
                                               }
                                               zapis_do_pliku << pamiec_pliku;
                                               zapis_do_pliku.close();


                                                for(int i = 1; i <= 9; i++)
                                                {
                                                            s_do_zapisu_pliku << wynik_ostatnich[i];
                                                            pamiec_ostatnich.replace((i-1)*5, s_do_zapisu_pliku.str().size(), s_do_zapisu_pliku.str(), 0, s_do_zapisu_pliku.str().size());
                                                            pamiec_ostatnich.replace(55 + i*29+24, 24, pamiec_ostatnich.substr(55+(i+1)*29 + 24, 24), 0,  24);

                                                            s_do_zapisu_pliku.str("");
                                                            s_do_zapisu_pliku.clear();
                                                            do_zapisu_pliku.clear();
                                                }

                                                zapis_do_ostatnich << pamiec_ostatnich;
                                                zapis_do_ostatnich.close();


                        }

}

void przebieg_gry::wypisz_ekran_koncowy()
{
            if(liczba_wrakow == 288)
            {
                        int przegrana = 0;
                        string a(" Niestety nie udalo Ci sie pokonac floty nieprzyjaciela ");
                        string b(" Nie zostales przez to takze zapisany na liscie zwyciezcow ");
                        string c(" Liczba wykonanych ruchow wynosi: ");
                        string d;
                        s_do_napisu_2 << 100-ruchy << " ";
                        s_do_napisu << (koncowy_napis(do_napisu_koncowego,c)).replace(((koncowy_napis(do_napisu_koncowego,c)).find(":"))+1, s_do_napisu_2.str().size(),s_do_napisu_2.str(),0, 4 ) << " ";
                        system("cls");


                                    zapis_do_pliku.open(MENU->plik_ini.c_str(), fstream::in | fstream::binary);
                                    zapis_do_pliku.clear();
                                    zapis_do_pliku.get(MENU->tab_ini, sizeof(MENU->tab_ini), EOF);
                                    zapis_do_pliku.close();
                                    zapis_do_pliku.open(MENU->plik_ini.c_str(), fstream::trunc | fstream::out | fstream::binary);
                                    for(int i = 80; i < 87; i++)
                                    {
                                                s_do_zapisu_pliku << MENU->tab_ini[i];
                                    }
                                    s_do_zapisu_pliku >> przegrana;
                                    przegrana += 1;
                                    s_do_zapisu_pliku.str("");

                                    d = MENU->tab_ini;

                                    s_do_zapisu_pliku << przegrana;
                                    d.replace(80, s_do_zapisu_pliku.str().size(), s_do_zapisu_pliku.str(), 0, s_do_zapisu_pliku.str().size());
                                    zapis_do_pliku << d;
                                    zapis_do_pliku.close();




                        cout << do_napisu_koncowego << endl << koncowy_napis(do_napisu_koncowego, a) << endl;
                        cout << koncowy_napis(do_napisu_koncowego, b) << endl << do_napisu_koncowego << endl;
                        cout << s_do_napisu.str() << endl << do_napisu_koncowego << endl;
                        cout << endl << endl << endl << "Nacisnij dowolny klawisz ...";
                        s_do_napisu.str("");
                        getch();
                        system("cls");
                        delete PLANSZA;
                        delete GRA;
                        MENU->wypisz_menu();
                        MENU->akcja();
            }
            if(liczba_wrakow_komp == 288)
            {
                        int wygrana = 0;
                        string a(" Brawo!!! Udalo Ci sie pokonac flote wroga ");
                        string b(" Mozesz odnalezc sie teraz na liscie zwyciezcow ");
                        string c(" Liczba wykonanych ruchow wynosi: ");
                        string d;
                        s_do_napisu_2 << 100-ruchy << " ";
                        s_do_napisu << (koncowy_napis(do_napisu_koncowego,c)).replace(((koncowy_napis(do_napisu_koncowego,c)).find(":"))+1, s_do_napisu_2.str().size(),s_do_napisu_2.str(),0, 4 ) << " ";
                        system("cls");

                                    zapis_do_pliku.open(MENU->plik_ini.c_str(), fstream::in | fstream::binary);
                                    zapis_do_pliku.clear();
                                    zapis_do_pliku.get(MENU->tab_ini, sizeof(MENU->tab_ini), EOF);
                                    zapis_do_pliku.close();
                                    zapis_do_pliku.open(MENU->plik_ini.c_str(), fstream::trunc | fstream::out | fstream::binary);
                                    for(int i = 51; i < 58; i++)
                                    {
                                                s_do_zapisu_pliku << MENU->tab_ini[i];
                                    }
                                    s_do_zapisu_pliku >> wygrana;
                                    wygrana += 1;
                                    s_do_zapisu_pliku.str("");

                                    d = MENU->tab_ini;

                                    s_do_zapisu_pliku << wygrana;
                                    ktora_gra_wygrana =wygrana;
                                    d.replace(51, s_do_zapisu_pliku.str().size(), s_do_zapisu_pliku.str(), 0, s_do_zapisu_pliku.str().size());
                                    zapis_do_pliku << d;
                                    zapis_do_pliku.close();


                        cout << do_napisu_koncowego << endl << koncowy_napis(do_napisu_koncowego, a) << endl;
                        cout << koncowy_napis(do_napisu_koncowego, b) << endl << do_napisu_koncowego << endl;
                        cout << s_do_napisu.str() << endl << do_napisu_koncowego << endl;
                        s_do_napisu.str("");
                        s_do_zapisu_pliku.str("");

                        zapisz_wyniki();

                        cout << endl << endl << endl << "Nacisnij dowolny klawisz ...";
                        getch();
                        system("cls");
                        delete PLANSZA;
                        delete GRA;
                        MENU->wypisz_menu();
                        MENU->akcja();
            }

}


string przebieg_gry::koncowy_napis(string podmieniany, string nacopodmienic)
{
            podmieniany.replace((80-nacopodmienic.size())/2, nacopodmienic.size(), nacopodmienic, 0, nacopodmienic.size());
            return podmieniany;
}
