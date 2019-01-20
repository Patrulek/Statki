#ifndef NAGL_HPP

extern HANDLE hOut;


using namespace std;

class plansza
{
            friend class jednomasztowiec;
            friend class dwumasztowiec;
            friend class trojmasztowiec;
            friend class czteromasztowiec;
            friend class przebieg_gry;

            private:
                        const int szerokosc;                                //szerokoœæ planszy
                        const int dlugosc;                                   //d³ugoœæ planszy
                        int ile_znakow;                                          //ile znaków ma zostaæ wype³nione
                        bool mapka[24][12];                             //tablica bool'owska do okreœlania gdzie mo¿na postawiæ statki
                        bool mapka_wroga[24][12];                  // patrz powyzej
                        string mapa;                                                 //string do wygl¹du mapki


            public:
                        void rysuj_mapke();                               //-------------------------------------------------------rysuje mapke------------------------------------------------------//
                        void rysuj_ze_statkami();                       //--------------------------------------------rysuje mapę z ustawionymi już statkami---------------------------//
                        plansza();                                                    // -------------inicjalizuje wartosciami true/false tablice 'mapka' ktora posluzy do okreslenia miejsc statkow------------------//
};


plansza::plansza()  : szerokosc(24), dlugosc(12)  //inicjalizuje szerokosc i dlugosc planszy wartosciami tu podanymi oraz ustawia wartosci true dla miejsc gdzie mozna postawic statki //
{
            for(int i = 0; i < 24; i++)
                        for(int k = 0; k < 12; k++)
                        {
                                    mapka[i][k] = true;
                                    mapka_wroga[i][k] = true;
                        }



            for(int i = 0; i < 24; i++)
            {
                        mapka[i][0] = false;
                        mapka[i][11] = false;
                        mapka_wroga[i][0] = false;
                        mapka_wroga[i][11] = false;
            }
            for(int k = 0; k < 12; k++)
            {
                        mapka[0][k] = false;
                        mapka[22][k] = false;
                        mapka_wroga[0][k] = false;
                        mapka_wroga[22][k] = false;

                        for(int z = 0; z < 12; z++)
                        {
                                    mapka[2*k+1][z] = false;
                                    mapka_wroga[2*k+1][z] = false;
                        }


            }


}

void plansza::rysuj_mapke()  // rysuje planszę do gry //
{
            ile_znakow = szerokosc*dlugosc;
            mapa.assign(ile_znakow*3, 32);

            mapa.replace(2, 20, "0 1 2 3 4 5 6 7 8 9", 0, 20);
            mapa.replace(50, 20, "0 1 2 3 4 5 6 7 8 9", 0, 20);
            mapa.replace(73, 23, "A|~|~|~|~|~|~|~|~|~|~|A", 0, 23);
            mapa.replace(121, 23, "A|~|~|~|~|~|~|~|~|~|~|A", 0, 23);
            mapa.replace(145, 23, "B|~|~|~|~|~|~|~|~|~|~|B", 0, 23);
            mapa.replace(193, 23, "B|~|~|~|~|~|~|~|~|~|~|B", 0, 23);
            mapa.replace(217, 23, "C|~|~|~|~|~|~|~|~|~|~|C", 0, 23);
            mapa.replace(265, 23, "C|~|~|~|~|~|~|~|~|~|~|C", 0, 23);
            mapa.replace(289, 23, "D|~|~|~|~|~|~|~|~|~|~|D", 0, 23);
            mapa.replace(337, 23, "D|~|~|~|~|~|~|~|~|~|~|D", 0, 23);
            mapa.replace(361, 23, "E|~|~|~|~|~|~|~|~|~|~|E", 0, 23);
            mapa.replace(409, 23, "E|~|~|~|~|~|~|~|~|~|~|E", 0, 23);
            mapa.replace(433, 23, "F|~|~|~|~|~|~|~|~|~|~|F", 0, 23);
            mapa.replace(481, 23, "F|~|~|~|~|~|~|~|~|~|~|F", 0, 23);
            mapa.replace(505, 23, "G|~|~|~|~|~|~|~|~|~|~|G", 0, 23);
            mapa.replace(553, 23, "G|~|~|~|~|~|~|~|~|~|~|G", 0, 23);
            mapa.replace(577, 23, "H|~|~|~|~|~|~|~|~|~|~|H", 0, 23);
            mapa.replace(625, 23, "H|~|~|~|~|~|~|~|~|~|~|H", 0, 23);
            mapa.replace(649, 23, "I|~|~|~|~|~|~|~|~|~|~|I", 0, 23);
            mapa.replace(697, 23, "I|~|~|~|~|~|~|~|~|~|~|I", 0, 23);
            mapa.replace(721, 23, "J|~|~|~|~|~|~|~|~|~|~|J", 0, 23);
            mapa.replace(769, 23, "J|~|~|~|~|~|~|~|~|~|~|J", 0, 23);
            mapa.replace(795, 20, "0 1 2 3 4 5 6 7 8 9", 0, 20);
            mapa.replace(843, 20, "0 1 2 3 4 5 6 7 8 9", 0, 20);

            for(int i = 0; i < dlugosc; i++)
            {
                        mapa[(i+1) * (szerokosc*3)] = '\n';
            }

}

 void plansza::rysuj_ze_statkami() // rysuje planszę po ustawieniu statków czy oddaniu strzałów//
 {
             cout << mapa;
 }


#endif
