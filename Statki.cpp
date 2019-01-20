#ifndef NAGL_HPP
extern plansza *PLANSZA;
extern przebieg_gry *GRA;



enum kierunek           {  gora = 0,                                    // -------------- kierunek statku----------------//
                                                prawo = 1,
                                                dol = 2,
                                                lewo = 3
                                             };




using namespace std;


//---------------------------------------------------------------------//
// ----------------------abstrakcyjna klasa------------------------//
//---------------------------------------------------------------------//


class statki
{
            protected:
                        static int ilosc_statkow;                       //----------------dana do okreslania ile statkow zostalo juz stworzonych----------//
                        int ilosc_masztow;                                     //------------nieprzydatne-------------//
                        int kierunek;                                                 //--------------kierunek statku------//
                        int x, y;                                                               //-----------wspolrzedne glownej czesci statku-----------//
                        time_t czas;

            public:
                        void ustawianie_statku();               //----------funkcja do narysowania statku na mapie (dziala w klasach pochodnych)--------------//
                        static void ustaw_zero_statkow();               //------------ustawia poczatkowa wartosc statkow do zera ------------------//
                        static int ile_statkow();
                        friend class plansza;
                        friend class przebieg_gry;



};

int statki::ilosc_statkow = 0;

void statki::ustaw_zero_statkow()
{
            ilosc_statkow = 0;
}

int statki::ile_statkow()
{
            return ilosc_statkow;
}



//---------------------------------------------------------------------//
//-----------------------klasa jednomasztowców----------------//                    // komentarze tutaj zgadzają się analogicznie do następnych rodzajów statków //
//---------------------------------------------------------------------//


class jednomasztowiec : public statki
{
            private:
                        void losuj_kierunek();                          //------------ funkcja do losowania kierunku statku-------------//
                        int losuj_wspolrz();                     //-----------funkcja do losowania wspolrzednych glownej czesci statku----------//
                        int losuj_wspolrz_wroga();         //-----to samo co wyżej tylko dla CPU ---------------------//
            public:
                        void ustawianie_statku();
                        void ustawianie_statku_wroga();

};

void jednomasztowiec::losuj_kierunek()
{
            srand(rand()*time(&czas));
            kierunek = rand() % 4;
}

int jednomasztowiec::losuj_wspolrz()  // losuje wspolrzedne do czasu az nei trafi na miejsce gdzei mozna postawic glowna czesc statku //
{
            int obieg = 0;
            do
            {
            srand(rand()*time(&czas));
            x = rand() % 24;
            y = rand() % 12;
            if(PLANSZA->mapka[x][y] == true)        break;
            obieg++ ;
            }
            while(obieg < 150) ;
            if(obieg == 150)
            {
                        ilosc_statkow-- ;
                        return obieg;
            }
}

int jednomasztowiec::losuj_wspolrz_wroga()
{
            int obieg = 0;
            do
            {
            srand(rand()*time(&czas));
            x = rand() % 24;
            y = rand() % 12;
            if(PLANSZA->mapka_wroga[x][y] == true)        break;
            obieg++ ;
            }
            while(obieg < 150) ;
            if(obieg == 150)
            {
                        ilosc_statkow-- ;
                        return obieg;
            }
}



void jednomasztowiec::ustawianie_statku()
{
            ilosc_masztow = 1;
            losuj_kierunek();
            if(losuj_wspolrz() == 150) return;
            for(int xx =x-2;   xx<x+3 ;   xx++)                                                             // ta pętla ustawia wartości false dla miejsc leżących o 1 pkt obok statku aby uniemozliwic
            {                                                                                                                                   // tam postawienie innego statku
                          for(int yy = y-1;  yy<y+2; yy++)
                                    {
                                                if(PLANSZA->mapka[xx][yy] == false)
                                                            continue;
                                                else
                                                {
                                                            PLANSZA->mapka[xx][yy] = false;
                                                }
                                    }
            }
            if(ilosc_statkow==0)                                                                                            // jeśli jeden z jednomasztowcow jest pierwszym statkiem, rysuje czysta plansze do gry
                        PLANSZA->rysuj_mapke();
            PLANSZA->mapa.replace(y*PLANSZA->szerokosc*3+x+1, 1, "X", 0, 1);   // stawia krzyżyk w miejscu gdzie jest statek
            ilosc_statkow++ ;                                                                                                           // zwiększa statyczną daną o 1
            GRA->jaki_punkt[x][y] = statek;                                                                                 // informuje grę o ustawieniu w tym miejscu statku
            GRA->jaki_statek[x][y] = jednomaszt;                                                                      // informuje grę o ustawieniu w miejscu gdzie jest statek, że jest to jednomasztowiec
}

void jednomasztowiec::ustawianie_statku_wroga()
{
            {
            ilosc_masztow = 1;
            losuj_kierunek();
            if(losuj_wspolrz_wroga() == 150) return;
            for(int xx =x-2;   xx<x+3 ;   xx++)                                                             // ta pętla ustawia wartości false dla miejsc leżących o 1 pkt obok statku aby uniemozliwic
            {                                                                                                                                    // tam postawienie innego statku
                          for(int yy = y-1;  yy<y+2; yy++)
                                    {
                                                if(PLANSZA->mapka_wroga[xx][yy] == false)
                                                            continue;
                                                else
                                                {
                                                            PLANSZA->mapka_wroga[xx][yy] = false;
                                                }
                                    }
            }
            if(ilosc_statkow==0)                                                                                            // jeśli jeden z jednomasztowcow jest pierwszym statkiem, rysuje czysta plansze do gry
                        PLANSZA->rysuj_mapke();
            ilosc_statkow++ ;                                                                                                           // zwiększa statyczną daną o 1
            GRA->jaki_punkt_komp[x][y] = statek;                                                                                 // informuje grę o ustawieniu w tym miejscu statku
            GRA->jaki_statek_komp[x][y] = jednomaszt;                                                                      // informuje grę o ustawieniu w miejscu gdzie jest statek, że jest to jednomasztowiec
}
}

//------------------------------------------------------------------------------------//
//------------------------------klasa dwumasztowców--------------------------//
//------------------------------------------------------------------------------------//


class dwumasztowiec : public statki
{
            private:
                        int losuj_wspolrz();
                        void losuj_kierunek();
                        int losuj_wspolrz_wroga();
            public:
                        void ustawianie_statku();
                        void ustawianie_statku_wroga();

};

void dwumasztowiec::losuj_kierunek()
{
            kierunek = rand() % 4;
}

int dwumasztowiec::losuj_wspolrz()
{
            int obieg = 0;
            do
            {
            srand(rand()*time(&czas));
            x = rand() % 24;
            y = rand() % 12;
            if(PLANSZA->mapka[x][y] == true)        break;
            obieg++ ;
            }
            while(obieg < 150) ;
            if(obieg == 150)
            {
                        ilosc_statkow-- ;
                        return obieg;
            }
}

int dwumasztowiec::losuj_wspolrz_wroga()
{
            int obieg = 0;
            do
            {
            srand(rand()*time(&czas));
            x = rand() % 24;
            y = rand() % 12;
            if(PLANSZA->mapka_wroga[x][y] == true)        break;
            obieg ++ ;
            }
            while(obieg < 150) ;
            if(obieg == 150)
            {
                        ilosc_statkow-- ;
                        return obieg;
            }
}




void dwumasztowiec::ustawianie_statku_wroga()
{
            losuj_kierunek();
            if(losuj_wspolrz_wroga() == 150) return;
            if(kierunek==gora)
            {
                        if(PLANSZA->mapka_wroga[x][y-1]  == false)
                        {
                                    while(PLANSZA->mapka_wroga[x][y-1] != true)
                                                if(losuj_wspolrz_wroga() == 150) return;
                        }
                        for(int xx = x-2;  xx<=x+2;  xx++)
                                    for(int yy = y+1;  yy>=y-2;  yy--)
                                                PLANSZA->mapka_wroga[xx][yy] = false;

                        if(ilosc_statkow==0)
                                    PLANSZA->rysuj_mapke();
                        ilosc_statkow++ ;
                        GRA->jaki_punkt_komp[x][y] = statek;
                        GRA->jaki_punkt_komp[x][y-1] = statek;
                        GRA->jaki_statek_komp[x][y] = dwumaszt;
                        GRA->jaki_statek_komp[x][y-1] = dwumaszt;

            }
            if(kierunek==prawo)
            {
                        if(PLANSZA->mapka_wroga[x+2][y]  == false)
                        {
                                    while(PLANSZA->mapka_wroga[x+2][y] != true)
                                                if(losuj_wspolrz_wroga() == 150) return;
                        }
                         for(int xx = x-2;  xx<=x+4;  xx++)
                                   for(int yy = y-1;  yy<=y+1;  yy++)
                                                PLANSZA->mapka_wroga[xx][yy] = false;

                        if(ilosc_statkow==0)
                                    PLANSZA->rysuj_mapke();
                        ilosc_statkow++ ;
                        GRA->jaki_punkt_komp[x][y] = statek;
                        GRA->jaki_punkt_komp[x+2][y] = statek;
                        GRA->jaki_statek_komp[x][y] = dwumaszt;
                        GRA->jaki_statek_komp[x+2][y] = dwumaszt;
            }
            if(kierunek==dol)
            {
                        if(PLANSZA->mapka_wroga[x][y+1]  == false)
                        {
                                    while(PLANSZA->mapka_wroga[x][y+1] != true)
                                                if(losuj_wspolrz_wroga() == 150) return;
                        }
                        for(int xx = x-2;  xx<=x+2;  xx++)
                                    for(int yy = y-1;  yy<=y+2;  yy++)
                                                PLANSZA->mapka_wroga[xx][yy] = false;

                        if(ilosc_statkow==0)
                                    PLANSZA->rysuj_mapke();
                        ilosc_statkow++ ;
                        GRA->jaki_punkt_komp[x][y] = statek;
                        GRA->jaki_punkt_komp[x][y+1] = statek;
                        GRA->jaki_statek_komp[x][y] = dwumaszt;
                        GRA->jaki_statek_komp[x][y+1] = dwumaszt;
            }
            if(kierunek==lewo)
            {
                        if(PLANSZA->mapka_wroga[x-2][y]  == false)
                        {
                                    while(PLANSZA->mapka_wroga[x-2][y] != true)
                                                if(losuj_wspolrz_wroga() == 150) return;
                        }
                        for(int xx = x+2;  xx>=x-4;  xx--)
                                    for(int yy = y+1;  yy>=y-1;  yy--)
                                                PLANSZA->mapka_wroga[xx][yy] = false;

                        if(ilosc_statkow==0)
                                    PLANSZA->rysuj_mapke();
                        ilosc_statkow++ ;
                        GRA->jaki_punkt_komp[x][y] = statek;
                        GRA->jaki_punkt_komp[x-2][y] = statek;
                        GRA->jaki_statek_komp[x][y] = dwumaszt;
                        GRA->jaki_statek_komp[x-2][y] = dwumaszt;
            }

}

void dwumasztowiec::ustawianie_statku()
{
            losuj_kierunek();
            if(losuj_wspolrz() == 150) return;
            if(kierunek==gora)
            {
                        if(PLANSZA->mapka[x][y-1]  == false)
                        {
                                    while(PLANSZA->mapka[x][y-1] != true)
                                                if(losuj_wspolrz() == 150) return;
                        }
                        for(int xx = x-2;  xx<=x+2;  xx++)
                                    for(int yy = y+1;  yy>=y-2;  yy--)
                                                PLANSZA->mapka[xx][yy] = false;

                        if(ilosc_statkow==0)
                                    PLANSZA->rysuj_mapke();
                        PLANSZA->mapa.replace(y*PLANSZA->szerokosc*3+x+1, 1, "X", 0, 1);
                        PLANSZA->mapa.replace((y-1)*PLANSZA->szerokosc*3+x+1, 1, "X", 0, 1);
                        ilosc_statkow++ ;
                        GRA->jaki_punkt[x][y] = statek;
                        GRA->jaki_punkt[x][y-1] = statek;
                        GRA->jaki_statek[x][y] = dwumaszt;
                        GRA->jaki_statek[x][y-1] = dwumaszt;

            }
            if(kierunek==prawo)
            {
                        if(PLANSZA->mapka[x+2][y]  == false)
                        {
                                    while(PLANSZA->mapka[x+2][y] != true)
                                                if(losuj_wspolrz() == 150) return;
                        }
                         for(int xx = x-2;  xx<=x+4;  xx++)
                                   for(int yy = y-1;  yy<=y+1;  yy++)
                                                PLANSZA->mapka[xx][yy] = false;

                        if(ilosc_statkow==0)
                                    PLANSZA->rysuj_mapke();
                        PLANSZA->mapa.replace(y*PLANSZA->szerokosc*3+x+1, 1, "X", 0, 1);
                        PLANSZA->mapa.replace(y*PLANSZA->szerokosc*3+x+3, 1, "X", 0, 1);
                        ilosc_statkow++ ;
                        GRA->jaki_punkt[x][y] = statek;
                        GRA->jaki_punkt[x+2][y] = statek;
                        GRA->jaki_statek[x][y] = dwumaszt;
                        GRA->jaki_statek[x+2][y] = dwumaszt;
            }
            if(kierunek==dol)
            {
                        if(PLANSZA->mapka[x][y+1]  == false)
                        {
                                    while(PLANSZA->mapka[x][y+1] != true)
                                                if(losuj_wspolrz() == 150) return;
                        }
                        for(int xx = x-2;  xx<=x+2;  xx++)
                                    for(int yy = y-1;  yy<=y+2;  yy++)
                                                PLANSZA->mapka[xx][yy] = false;

                        if(ilosc_statkow==0)
                                    PLANSZA->rysuj_mapke();
                        PLANSZA->mapa.replace(y*PLANSZA->szerokosc*3+x+1, 1, "X", 0, 1);
                        PLANSZA->mapa.replace((y+1)*PLANSZA->szerokosc*3+x+1, 1, "X", 0, 1);
                        ilosc_statkow++ ;
                        GRA->jaki_punkt[x][y] = statek;
                        GRA->jaki_punkt[x][y+1] = statek;
                        GRA->jaki_statek[x][y] = dwumaszt;
                        GRA->jaki_statek[x][y+1] = dwumaszt;
            }
            if(kierunek==lewo)
            {
                        if(PLANSZA->mapka[x-2][y]  == false)
                        {
                                    while(PLANSZA->mapka[x-2][y] != true)
                                                if(losuj_wspolrz() == 150) return;
                        }
                        for(int xx = x+2;  xx>=x-4;  xx--)
                                    for(int yy = y+1;  yy>=y-1;  yy--)
                                                PLANSZA->mapka[xx][yy] = false;

                        if(ilosc_statkow==0)
                                    PLANSZA->rysuj_mapke();
                        PLANSZA->mapa.replace(y*PLANSZA->szerokosc*3+x+1, 1, "X", 0, 1);
                        PLANSZA->mapa.replace(y*PLANSZA->szerokosc*3+x-1, 1, "X", 0, 1);
                        ilosc_statkow++ ;
                        GRA->jaki_punkt[x][y] = statek;
                        GRA->jaki_punkt[x-2][y] = statek;
                        GRA->jaki_statek[x][y] = dwumaszt;
                        GRA->jaki_statek[x-2][y] = dwumaszt;
            }

}



//------------------------------------------------------------------------------------//
//------------------------------klasa trójmasztowców--------------------------//
//------------------------------------------------------------------------------------//


class trojmasztowiec : public statki
{
            private:
                        int losuj_wspolrz();
                        void losuj_kierunek();
                        int losuj_wspolrz_wroga();
            public:
                        void ustawianie_statku();
                        void ustawianie_statku_wroga();

};

void trojmasztowiec::losuj_kierunek()
{
            kierunek = rand() % 4;
}

int trojmasztowiec::losuj_wspolrz()
{
            int obieg = 0;
            do
            {
            srand(rand()*time(&czas));
            x = rand() % 24;
            y = rand() % 12;
            if(PLANSZA->mapka[x][y] == true)        break;
            obieg++ ;
            }
            while(obieg < 150) ;
            if(obieg == 150)
            {
                        ilosc_statkow-- ;
                        return obieg;
            }
}

int trojmasztowiec::losuj_wspolrz_wroga()
{
            int obieg = 0;
            do
            {
            srand(rand()*time(&czas));
            x = rand() % 24;
            y = rand() % 12;
            if(PLANSZA->mapka_wroga[x][y] == true)        break;
            obieg++ ;
            }
            while(obieg < 150) ;
            if(obieg == 150)
            {
                        ilosc_statkow-- ;
                        return obieg;
            }
}


void trojmasztowiec::ustawianie_statku()
{
            losuj_kierunek();
            if(losuj_wspolrz() == 150) return;
            if(kierunek==gora)
            {
                        while(PLANSZA->mapka[x][y-2] != true)
                                                if(losuj_wspolrz() == 150) return;

                        for(int xx = x-2;  xx<=x+2;  xx++)
                                    for(int yy = y+1;  yy>=y-3;  yy--)
                                                PLANSZA->mapka[xx][yy] = false;

                        if(ilosc_statkow==0)
                                    PLANSZA->rysuj_mapke();
                        PLANSZA->mapa.replace(y*PLANSZA->szerokosc*3+x+1, 1, "X", 0, 1);
                        PLANSZA->mapa.replace((y-1)*PLANSZA->szerokosc*3+x+1, 1, "X", 0, 1);
                        PLANSZA->mapa.replace((y-2)*PLANSZA->szerokosc*3+x+1, 1, "X", 0, 1);
                        ilosc_statkow++ ;
                        GRA->jaki_punkt[x][y] = statek;
                        GRA->jaki_punkt[x][y-1] = statek;
                        GRA->jaki_punkt[x][y-2] = statek;
                        GRA->jaki_statek[x][y] = trojmaszt;
                        GRA->jaki_statek[x][y-1] = trojmaszt;
                        GRA->jaki_statek[x][y-2] = trojmaszt;

            }
            if(kierunek==prawo)
            {
                        if(PLANSZA->mapka[x+4][y]  == false)
                        {
                                    while(PLANSZA->mapka[x+4][y] != true)
                                                if(losuj_wspolrz() == 150) return;
                        }
                         for(int xx = x-2;  xx<=x+6;  xx++)
                                   for(int yy = y-1;  yy<=y+1;  yy++)
                                                PLANSZA->mapka[xx][yy] = false;

                        if(ilosc_statkow==0)
                                    PLANSZA->rysuj_mapke();
                        PLANSZA->mapa.replace(y*PLANSZA->szerokosc*3+x+1, 1, "X", 0, 1);
                        PLANSZA->mapa.replace(y*PLANSZA->szerokosc*3+x+3, 1, "X", 0, 1);
                        PLANSZA->mapa.replace(y*PLANSZA->szerokosc*3+x+5, 1, "X", 0, 1);
                        ilosc_statkow++ ;
                        GRA->jaki_punkt[x][y] = statek;
                        GRA->jaki_punkt[x+2][y] = statek;
                        GRA->jaki_punkt[x+4][y] = statek;
                        GRA->jaki_statek[x][y] = trojmaszt;
                        GRA->jaki_statek[x+2][y] = trojmaszt;
                        GRA->jaki_statek[x+4][y] = trojmaszt;
            }
            if(kierunek==dol)
            {
                        if(PLANSZA->mapka[x][y+2]  == false)
                        {
                                    while(PLANSZA->mapka[x][y+2] != true)
                                                if(losuj_wspolrz() == 150) return;
                        }
                        for(int xx = x-2;  xx<=x+2;  xx++)
                                    for(int yy = y-1;  yy<=y+3;  yy++)
                                                PLANSZA->mapka[xx][yy] = false;

                        if(ilosc_statkow==0)
                                    PLANSZA->rysuj_mapke();
                        PLANSZA->mapa.replace(y*PLANSZA->szerokosc*3+x+1, 1, "X", 0, 1);
                        PLANSZA->mapa.replace((y+1)*PLANSZA->szerokosc*3+x+1, 1, "X", 0, 1);
                        PLANSZA->mapa.replace((y+2)*PLANSZA->szerokosc*3+x+1, 1, "X", 0, 1);
                        ilosc_statkow++ ;
                        GRA->jaki_punkt[x][y] = statek;
                        GRA->jaki_punkt[x][y+1] = statek;
                        GRA->jaki_punkt[x][y+2] = statek;
                        GRA->jaki_statek[x][y] = trojmaszt;
                        GRA->jaki_statek[x][y+1] = trojmaszt;
                        GRA->jaki_statek[x][y+2] = trojmaszt;
            }
            if(kierunek==lewo)
            {
                        if(PLANSZA->mapka[x-4][y]  == false)
                        {
                                    while(PLANSZA->mapka[x-4][y] != true)
                                                if(losuj_wspolrz() == 150) return;
                        }
                        for(int xx = x+2;  xx>=x-6;  xx--)
                                    for(int yy = y+1;  yy>=y-1;  yy--)
                                                PLANSZA->mapka[xx][yy] = false;

                        if(ilosc_statkow==0)
                                    PLANSZA->rysuj_mapke();
                        PLANSZA->mapa.replace(y*PLANSZA->szerokosc*3+x+1, 1, "X", 0, 1);
                        PLANSZA->mapa.replace(y*PLANSZA->szerokosc*3+x-1, 1, "X", 0, 1);
                        PLANSZA->mapa.replace(y*PLANSZA->szerokosc*3+x-3, 1, "X", 0, 1);
                        ilosc_statkow++ ;
                        GRA->jaki_punkt[x][y] = statek;
                        GRA->jaki_punkt[x-2][y] = statek;
                        GRA->jaki_punkt[x-4][y] = statek;
                        GRA->jaki_statek[x][y] = trojmaszt;
                        GRA->jaki_statek[x-2][y] = trojmaszt;
                        GRA->jaki_statek[x-4][y] = trojmaszt;
            }

}


void trojmasztowiec::ustawianie_statku_wroga()
{
            losuj_kierunek();
            if(losuj_wspolrz_wroga() == 150) return;
            if(kierunek==gora)
            {
                        while(PLANSZA->mapka_wroga[x][y-2] != true)
                                                if(losuj_wspolrz_wroga() == 150) return;

                        for(int xx = x-2;  xx<=x+2;  xx++)
                                    for(int yy = y+1;  yy>=y-3;  yy--)
                                                PLANSZA->mapka_wroga[xx][yy] = false;

                        if(ilosc_statkow==0)
                                    PLANSZA->rysuj_mapke();
                        ilosc_statkow++ ;
                        GRA->jaki_punkt_komp[x][y] = statek;
                        GRA->jaki_punkt_komp[x][y-1] = statek;
                        GRA->jaki_punkt_komp[x][y-2] = statek;
                        GRA->jaki_statek_komp[x][y] = trojmaszt;
                        GRA->jaki_statek_komp[x][y-1] = trojmaszt;
                        GRA->jaki_statek_komp[x][y-2] = trojmaszt;

            }
            if(kierunek==prawo)
            {
                        if(PLANSZA->mapka_wroga[x+4][y]  == false)
                        {
                                    while(PLANSZA->mapka_wroga[x+4][y] != true)
                                                if(losuj_wspolrz_wroga() == 150) return;
                        }
                         for(int xx = x-2;  xx<=x+6;  xx++)
                                   for(int yy = y-1;  yy<=y+1;  yy++)
                                                PLANSZA->mapka_wroga[xx][yy] = false;

                        if(ilosc_statkow==0)
                                    PLANSZA->rysuj_mapke();
                        ilosc_statkow++ ;
                        GRA->jaki_punkt_komp[x][y] = statek;
                        GRA->jaki_punkt_komp[x+2][y] = statek;
                        GRA->jaki_punkt_komp[x+4][y] = statek;
                        GRA->jaki_statek_komp[x][y] = trojmaszt;
                        GRA->jaki_statek_komp[x+2][y] = trojmaszt;
                        GRA->jaki_statek_komp[x+4][y] = trojmaszt;
            }
            if(kierunek==dol)
            {
                        if(PLANSZA->mapka_wroga[x][y+2]  == false)
                        {
                                    while(PLANSZA->mapka_wroga[x][y+2] != true)
                                                if(losuj_wspolrz_wroga() == 150) return;
                        }
                        for(int xx = x-2;  xx<=x+2;  xx++)
                                    for(int yy = y-1;  yy<=y+3;  yy++)
                                                PLANSZA->mapka_wroga[xx][yy] = false;

                        if(ilosc_statkow==0)
                                    PLANSZA->rysuj_mapke();
                        ilosc_statkow++ ;
                        GRA->jaki_punkt_komp[x][y] = statek;
                        GRA->jaki_punkt_komp[x][y+1] = statek;
                        GRA->jaki_punkt_komp[x][y+2] = statek;
                        GRA->jaki_statek_komp[x][y] = trojmaszt;
                        GRA->jaki_statek_komp[x][y+1] = trojmaszt;
                        GRA->jaki_statek_komp[x][y+2] = trojmaszt;
            }
            if(kierunek==lewo)
            {
                        if(PLANSZA->mapka_wroga[x-4][y]  == false)
                        {
                                    while(PLANSZA->mapka_wroga[x-4][y] != true)
                                                if(losuj_wspolrz_wroga() == 150) return;
                        }
                        for(int xx = x+2;  xx>=x-6;  xx--)
                                    for(int yy = y+1;  yy>=y-1;  yy--)
                                                PLANSZA->mapka_wroga[xx][yy] = false;

                        if(ilosc_statkow==0)
                                    PLANSZA->rysuj_mapke();
                        ilosc_statkow++ ;
                        GRA->jaki_punkt_komp[x][y] = statek;
                        GRA->jaki_punkt_komp[x-2][y] = statek;
                        GRA->jaki_punkt_komp[x-4][y] = statek;
                        GRA->jaki_statek_komp[x][y] = trojmaszt;
                        GRA->jaki_statek_komp[x-2][y] = trojmaszt;
                        GRA->jaki_statek_komp[x-4][y] = trojmaszt;
            }

}

//------------------------------------------------------------------------------------//
//------------------------------klasa czteromasztowców--------------------------//
//------------------------------------------------------------------------------------//


class czteromasztowiec : public statki
{
            private:
                        int losuj_wspolrz();
                        void losuj_kierunek();
                        int losuj_wspolrz_wroga();
            public:
                        void ustawianie_statku();
                        void ustawianie_statku_wroga();



};

void czteromasztowiec::losuj_kierunek()
{
            kierunek = rand() % 4;
}

int czteromasztowiec::losuj_wspolrz()
{
            int obieg = 0;
            do
            {
            srand(rand()*time(&czas));
            x = rand() % 24;
            y = rand() % 12;
            if(PLANSZA->mapka[x][y] == true)        break;
            obieg++ ;
            }
            while(obieg < 150) ;
            if(obieg == 150)
            {
                        ilosc_statkow-- ;
                        return obieg;
            }
}

int czteromasztowiec::losuj_wspolrz_wroga()
{
            int obieg = 0;
            do
            {
            srand(rand()*time(&czas));
            x = rand() % 24;
            y = rand() % 12;
            obieg++ ;
            if(PLANSZA->mapka_wroga[x][y] == true)        break;
            }
            while(obieg < 150) ;
            if(obieg == 150)
            {
                        ilosc_statkow-- ;
                        return obieg;
            }
}


void czteromasztowiec::ustawianie_statku()
{
            losuj_kierunek();
            if(losuj_wspolrz() == 150) return;
            if(kierunek==gora)
            {

                        while(PLANSZA->mapka[x][y-3] != true)
                                    if(losuj_wspolrz() == 150) return;

                        for(int xx = x-2;  xx<=x+2;  xx++)
                                    for(int yy = y+1;  yy>=y-4;  yy--)
                                                PLANSZA->mapka[xx][yy] = false;

                        if(ilosc_statkow==0)
                                    PLANSZA->rysuj_mapke();
                        PLANSZA->mapa.replace(y*PLANSZA->szerokosc*3+x+1, 1, "X", 0, 1);
                        PLANSZA->mapa.replace((y-1)*PLANSZA->szerokosc*3+x+1, 1, "X", 0, 1);
                        PLANSZA->mapa.replace((y-2)*PLANSZA->szerokosc*3+x+1, 1, "X", 0, 1);
                        PLANSZA->mapa.replace((y-3)*PLANSZA->szerokosc*3+x+1, 1, "X", 0, 1);
                        ilosc_statkow++ ;
                        GRA->jaki_punkt[x][y] = statek;
                        GRA->jaki_punkt[x][y-1] = statek;
                        GRA->jaki_punkt[x][y-2] = statek;
                        GRA->jaki_punkt[x][y-3] = statek;
                        GRA->jaki_statek[x][y] = czteromaszt;
                        GRA->jaki_statek[x][y-1] = czteromaszt;
                        GRA->jaki_statek[x][y-2] = czteromaszt;
                        GRA->jaki_statek[x][y-3] = czteromaszt;

            }
            if(kierunek==prawo)
            {

                        while(PLANSZA->mapka[x+6][y] != true || x+6 > 24)
                                    if(losuj_wspolrz() == 150) return;

                         for(int xx = x-2;  xx<=x+8;  xx++)
                                   for(int yy = y-1;  yy<=y+1;  yy++)
                                                PLANSZA->mapka[xx][yy] = false;

                        if(ilosc_statkow==0)
                                    PLANSZA->rysuj_mapke();
                        PLANSZA->mapa.replace(y*PLANSZA->szerokosc*3+x+1, 1, "X", 0, 1);
                        PLANSZA->mapa.replace(y*PLANSZA->szerokosc*3+x+3, 1, "X", 0, 1);
                        PLANSZA->mapa.replace(y*PLANSZA->szerokosc*3+x+5, 1, "X", 0, 1);
                        PLANSZA->mapa.replace(y*PLANSZA->szerokosc*3+x+7, 1, "X", 0, 1);
                        ilosc_statkow++ ;
                        GRA->jaki_punkt[x][y] = statek;
                        GRA->jaki_punkt[x+2][y] = statek;
                        GRA->jaki_punkt[x+4][y] = statek;
                        GRA->jaki_punkt[x+6][y] = statek;
                        GRA->jaki_statek[x][y] = czteromaszt;
                        GRA->jaki_statek[x+2][y] = czteromaszt;
                        GRA->jaki_statek[x+4][y] = czteromaszt;
                        GRA->jaki_statek[x+6][y] = czteromaszt;
            }
            if(kierunek==dol)
            {
                        while(PLANSZA->mapka[x][y+3] != true)
                                    if(losuj_wspolrz() == 150) return;

                        for(int xx = x-2;  xx<=x+2;  xx++)
                                    for(int yy = y-1;  yy<=y+4;  yy++)
                                                PLANSZA->mapka[xx][yy] = false;

                        if(ilosc_statkow==0)
                                    PLANSZA->rysuj_mapke();
                        PLANSZA->mapa.replace(y*PLANSZA->szerokosc*3+x+1, 1, "X", 0, 1);
                        PLANSZA->mapa.replace((y+1)*PLANSZA->szerokosc*3+x+1, 1, "X", 0, 1);
                        PLANSZA->mapa.replace((y+2)*PLANSZA->szerokosc*3+x+1, 1, "X", 0, 1);
                        PLANSZA->mapa.replace((y+3)*PLANSZA->szerokosc*3+x+1, 1, "X", 0, 1);
                        ilosc_statkow++ ;
                        GRA->jaki_punkt[x][y] = statek;
                        GRA->jaki_punkt[x][y+1] = statek;
                        GRA->jaki_punkt[x][y+2] = statek;
                        GRA->jaki_punkt[x][y+3] = statek;
                        GRA->jaki_statek[x][y] = czteromaszt;
                        GRA->jaki_statek[x][y+1] = czteromaszt;
                        GRA->jaki_statek[x][y+2] = czteromaszt;
                        GRA->jaki_statek[x][y+3] = czteromaszt;
            }
            if(kierunek==lewo)
            {
                        while(PLANSZA->mapka[x-6][y] != true)
                                    if(losuj_wspolrz() == 150) return;

                        for(int xx = x+2;  xx>=x-8;  xx--)
                                    for(int yy = y+1;  yy>=y-1;  yy--)
                                                PLANSZA->mapka[xx][yy] = false;

                        if(ilosc_statkow==0)
                                    PLANSZA->rysuj_mapke();
                        PLANSZA->mapa.replace(y*PLANSZA->szerokosc*3+x+1, 1, "X", 0, 1);
                        PLANSZA->mapa.replace(y*PLANSZA->szerokosc*3+x-1, 1, "X", 0, 1);
                        PLANSZA->mapa.replace(y*PLANSZA->szerokosc*3+x-3, 1, "X", 0, 1);
                        PLANSZA->mapa.replace(y*PLANSZA->szerokosc*3+x-5, 1, "X", 0, 1);
                        ilosc_statkow++ ;
                        GRA->jaki_punkt[x][y] = statek;
                        GRA->jaki_punkt[x-2][y] = statek;
                        GRA->jaki_punkt[x-4][y] = statek;
                        GRA->jaki_punkt[x-6][y] = statek;
                        GRA->jaki_statek[x][y] = czteromaszt;
                        GRA->jaki_statek[x-2][y] = czteromaszt;
                        GRA->jaki_statek[x-4][y] = czteromaszt;
                        GRA->jaki_statek[x-6][y] = czteromaszt;
            }

}

void czteromasztowiec::ustawianie_statku_wroga()
{
            losuj_kierunek();
            if(losuj_wspolrz_wroga() == 150) return;
            if(kierunek==gora)
            {

                        while(PLANSZA->mapka_wroga[x][y-3] != true)
                                    if(losuj_wspolrz_wroga() == 150) return;

                        for(int xx = x-2;  xx<=x+2;  xx++)
                                    for(int yy = y+1;  yy>=y-4;  yy--)
                                                PLANSZA->mapka_wroga[xx][yy] = false;

                        if(ilosc_statkow==0)
                                    PLANSZA->rysuj_mapke();
                        ilosc_statkow++ ;
                        GRA->jaki_punkt_komp[x][y] = statek;
                        GRA->jaki_punkt_komp[x][y-1] = statek;
                        GRA->jaki_punkt_komp[x][y-2] = statek;
                        GRA->jaki_punkt_komp[x][y-3] = statek;
                        GRA->jaki_statek_komp[x][y] = czteromaszt;
                        GRA->jaki_statek_komp[x][y-1] = czteromaszt;
                        GRA->jaki_statek_komp[x][y-2] = czteromaszt;
                        GRA->jaki_statek_komp[x][y-3] = czteromaszt;

            }
            if(kierunek==prawo)
            {

                        while(PLANSZA->mapka_wroga[x+6][y] != true || x+6 > 24)
                                    if(losuj_wspolrz_wroga() == 150) return;

                         for(int xx = x-2;  xx<=x+8;  xx++)
                                   for(int yy = y-1;  yy<=y+1;  yy++)
                                                PLANSZA->mapka_wroga[xx][yy] = false;

                        if(ilosc_statkow==0)
                                    PLANSZA->rysuj_mapke();
                        ilosc_statkow++ ;
                        GRA->jaki_punkt_komp[x][y] = statek;
                        GRA->jaki_punkt_komp[x+2][y] = statek;
                        GRA->jaki_punkt_komp[x+4][y] = statek;
                        GRA->jaki_punkt_komp[x+6][y] = statek;
                        GRA->jaki_statek_komp[x][y] = czteromaszt;
                        GRA->jaki_statek_komp[x+2][y] = czteromaszt;
                        GRA->jaki_statek_komp[x+4][y] = czteromaszt;
                        GRA->jaki_statek_komp[x+6][y] = czteromaszt;
            }
            if(kierunek==dol)
            {
                        while(PLANSZA->mapka_wroga[x][y+3] != true)
                                    if(losuj_wspolrz_wroga() == 150) return;

                        for(int xx = x-2;  xx<=x+2;  xx++)
                                    for(int yy = y-1;  yy<=y+4;  yy++)
                                                PLANSZA->mapka_wroga[xx][yy] = false;

                        if(ilosc_statkow==0)
                                    PLANSZA->rysuj_mapke();
                        ilosc_statkow++ ;
                        GRA->jaki_punkt_komp[x][y] = statek;
                        GRA->jaki_punkt_komp[x][y+1] = statek;
                        GRA->jaki_punkt_komp[x][y+2] = statek;
                        GRA->jaki_punkt_komp[x][y+3] = statek;
                        GRA->jaki_statek_komp[x][y] = czteromaszt;
                        GRA->jaki_statek_komp[x][y+1] = czteromaszt;
                        GRA->jaki_statek_komp[x][y+2] = czteromaszt;
                        GRA->jaki_statek_komp[x][y+3] = czteromaszt;
            }
            if(kierunek==lewo)
            {
                        while(PLANSZA->mapka_wroga[x-6][y] != true || x-6 < 0)
                                    if(losuj_wspolrz_wroga() == 150) return;

                        for(int xx = x+2;  xx>=x-8;  xx--)
                                    for(int yy = y+1;  yy>=y-1;  yy--)
                                                PLANSZA->mapka_wroga[xx][yy] = false;

                        if(ilosc_statkow==0)
                                    PLANSZA->rysuj_mapke();
                        ilosc_statkow++ ;
                        GRA->jaki_punkt_komp[x][y] = statek;
                        GRA->jaki_punkt_komp[x-2][y] = statek;
                        GRA->jaki_punkt_komp[x-4][y] = statek;
                        GRA->jaki_punkt_komp[x-6][y] = statek;
                        GRA->jaki_statek_komp[x][y] = czteromaszt;
                        GRA->jaki_statek_komp[x-2][y] = czteromaszt;
                        GRA->jaki_statek_komp[x-4][y] = czteromaszt;
                        GRA->jaki_statek_komp[x-6][y] = czteromaszt;
            }

}



#endif


