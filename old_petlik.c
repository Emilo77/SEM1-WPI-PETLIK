#include <stdio.h>
#include <stdlib.h>

typedef struct zmienna {
	int *wartosc;
	int dlugosc_calkowita;
	int dlugosc;
} zmienna;
int rozmiar_pocz = 200;

/*
Inicjalizacja tablic, jednej występującej w strukturze zmienna,
drugiej do przechowywania znaków
*/
void init(zmienna *x) {
	x->wartosc = (int *) malloc(sizeof(int) * (long unsigned int) rozmiar_pocz);
	x->dlugosc_calkowita = rozmiar_pocz;
	x->dlugosc = 1;
	for (int i = 0; i < rozmiar_pocz; i++) {
		x->wartosc[i] = 0;
	}
}

void init_2(char **T, int *dlugosc, int *dlugosc_calkowita) {
	*dlugosc_calkowita = 64;
	*T = (char *) malloc(sizeof(char) * (long unsigned int) *dlugosc_calkowita);
	*dlugosc = 1;
}

void uninit(zmienna x) {
	free(x.wartosc);
}

void dopisz_na_koniec(zmienna *x) {
	if (x->dlugosc == x->dlugosc_calkowita) {
		int nowa_dlugosc = 2 * (x->dlugosc_calkowita);
		x->wartosc = (int *) realloc(x->wartosc, sizeof(int) *
		                                         (long unsigned int) nowa_dlugosc);
		x->dlugosc_calkowita = nowa_dlugosc;

	}
	x->wartosc[x->dlugosc] = 1;
	(x->dlugosc)++;

	for (int j = x->dlugosc; j < x->dlugosc_calkowita; j++) {
		(x->wartosc[j]) = 0;

	}
}

//POWIEKSZENIE TABLICY DWUKROTNIE
void
zwieksz_rozmiar(char **T, const int *ile_zajetych, int *rozmiar_prawdziwy) {
	if (*ile_zajetych == *rozmiar_prawdziwy) {

		int rozmiar_nowy = 2 * (*rozmiar_prawdziwy);
		*T = realloc(*T,
		                      sizeof(char) * (long unsigned int) rozmiar_nowy);
		*rozmiar_prawdziwy = rozmiar_nowy;
	}
}

//POWIEKSZENIE TABLICY X DO TAKIEJ SAMEJ ILOSCI KOMOREK CO W TABLICY Y
void zmien_rozmiar(zmienna *x, zmienna *y) {
	(x->wartosc) = (int *) realloc((x->wartosc), sizeof(int) *
	                                             (long unsigned int) (y->dlugosc_calkowita));

	(x->dlugosc_calkowita) = (y->dlugosc_calkowita);

	for (int j = x->dlugosc; j < x->dlugosc_calkowita; j++) {
		(x->wartosc[j]) = 0;
	}
}

//GŁÓWNA FUNKCJA DO WYRÓWNYWANA ELEMENTÓW TABLICY NA CYFRY ZMIENNEJ
void optymalizuj(zmienna *x) {

	for (int i = 0; i < x->dlugosc - 1; i++) {
		if ((x->wartosc[i]) > 9999) {
			(x->wartosc[i + 1]) += ((x->wartosc[i]) / 10000);
			(x->wartosc[i]) %= 10000;
		}

	}
	if (x->wartosc[(x->dlugosc_calkowita) - 1] > 9999) {
		x->wartosc[x->dlugosc_calkowita - 1] -= 10000;
		dopisz_na_koniec(x);
	} else if (x->wartosc[x->dlugosc - 1] > 9999) {
		x->wartosc[x->dlugosc - 1] -= 10000;
		(x->wartosc[x->dlugosc]) += 1;
		(x->dlugosc)++;

	}
	if ((x->wartosc[(x->dlugosc) - 1] == 0 && x->dlugosc > 1)) {
		(x->dlugosc)--;
	}
}

//FUNKCJE WYKONUJĄCE SIĘ NA PODSTAWIE INSTRUKCJI
void INC(zmienna *x) {
	(x->wartosc[0])++;
	optymalizuj(x);
}

void ADD(zmienna *x, struct zmienna *y) {
	if ((y->dlugosc) > (x->dlugosc_calkowita)) {
		(x->dlugosc) = (y->dlugosc);
		zmien_rozmiar(x, y);

	}
	for (int i = 0; i < (y->dlugosc); i++) {
		// printf("\nDLUGOSC  %d  WARTOSC X  %d  WARTOSC Y  %d  indeks %d ",x->dlugosc,x->wartosc[i],y->wartosc[i],i);
		(x->wartosc[i]) += (y->wartosc[i]);
	}
	(x->dlugosc) = (y->dlugosc);
	optymalizuj(x);
}

void CLR(zmienna *x) {
	free(x->wartosc);
	init(x);
}

void DJZ(zmienna *x, int *i) {
	if (x->wartosc[0] != 0 || x->dlugosc != 1) {
		(x->wartosc[0])--;
		optymalizuj(x);
	} else {
		(*i) = (int) (parametr_1[*i] - 1);
	}
}

void TXT(zmienna x) {
	printf("%d", x.wartosc[x.dlugosc - 1]);
	for (int i = x.dlugosc - 2; i >= 0; i--) {
		if (x.wartosc[i] < 10) {
			printf("000%d", x.wartosc[i]);
		} else if (x.wartosc[i] < 100) {
			printf("00%d", x.wartosc[i]);
		} else if (x.wartosc[i] < 1000) {
			printf("0%d", x.wartosc[i]);
		} else
			printf("%d", x.wartosc[i]);
	}
	printf("\n");
}
/*
1:  INC
2:  ADD
3:  CLR
4:  DJZ
5:  JMP
6:  TXT
7:  HLT
*/
void petla(char tab_znakow[], int *i, int *j) {
	(*i)++;
	int pom = (*i);
	(*i)++;
	int pom2 = (*i);
	int czy_optymalizowac = 1;
	int adres;

	while (tab_znakow[pom2] != ')') {
		if (tab_znakow[pom2] == tab_znakow[pom] || tab_znakow[pom2] == '(') {
			czy_optymalizowac = 0;

		}
		pom2++;
	}
	if (czy_optymalizowac) {
		while (tab_znakow[*i] != ')') {
			nazwa[*j] = 2;
			parametr_0[*j] = (int) tab_znakow[*i];
			parametr_1[*j] = (int) tab_znakow[pom];
			(*j)++;
			(*i)++;
		}
		nazwa[*j] = 3;
		parametr_0[*j] = (int) tab_znakow[pom];
		parametr_1[*j] = -1;
		(*j)++;
	} else {

		nazwa[*j] = 4;
		parametr_0[*j] = (int) tab_znakow[pom];
		parametr_1[*j] = -1;
		adres = (*j);
		(*j)++;

		while (tab_znakow[*i] != ')') {

			if (tab_znakow[*i] == '(') {
				petla(tab_znakow, i, j);
			} else if (tab_znakow[*i] != '(' && tab_znakow[*i] != ')') {
				nazwa[*j] = 1;
				parametr_0[*j] = (int) tab_znakow[*i];
				parametr_1[*j] = -1;
				(*j)++;
				(*i)++;
			}
		}
		nazwa[*j] = 5;
		parametr_0[*j] = adres;
		parametr_1[*j] = -1;
		parametr_1[adres] = (*j) +
		                    1; //przekazanie wartości komórki j+1 dla instrukcji opcja 4
		(*j)++;
	}

	(*i)++;
}

/*
FUNKCJA TWORZĄCA INSTRUKCJE NA PODSTAWIE KODU PODANEGO Z PLIKU WEJŚCIOWEGO, WSZYSTKIE INSTRUKCJE WRZUCA DO TABLIC
*/
void wypisz_instrukcje(char tab_znakow[], int dlugosc_tab) {
	int i = 0;
	int j = 0;
	while (i < dlugosc_tab) {
		if (tab_znakow[i] == '=') {
			nazwa[j] = 6;
			parametr_0[j] = (int) tab_znakow[i + 1];
			parametr_1[j] = -1;
			i++;
			j++;
		} else if (tab_znakow[i] == '(') {
			petla(tab_znakow, &i, &j);
			i--;
		} else if (tab_znakow[i] == '\n') {
			break;
		} else {
			nazwa[j] = 1;
			parametr_0[j] = (int) tab_znakow[i];
			parametr_1[j] = -1;
			j++;
		}
		i++;
	}
	nazwa[j] = 7;
	parametr_0[j] = -1;
	parametr_1[j] = -1;

}

/*
FUNKCJA WYKONUJĄCA POSZCZEGÓLNE OPERACJE NA ZMIENNYCH A-Z, NA PODSTAWIE INSTRUKCJI
*/
void wykonaj(zmienna *tab) {
	// INC '1'
	// ADD '2'
	// CLR '3'
	// DJZ '4'
	// JMP '5'
	// TXT '6'
	// HLT '7'

	int i = 0;


	while (nazwa[i] != 7) {
		switch (nazwa[i]) {
			case 1: {

				INC(&tab[parametr_0[i] - 97]);
				break;
			}
			case 2: {
				ADD(&tab[parametr_0[i] - 97], &tab[parametr_1[i] - 97]);
				break;
			}
			case 3: {

				CLR(&tab[parametr_0[i] - 97]);
				break;
			}
			case 4: {
				DJZ(&tab[parametr_0[i] - 97], &i);
				break;
			}
			case 5: {
				i = parametr_0[i] - 1;
				break;
			}
			case 6: {
				TXT(tab[parametr_0[i] - 97]);
				break;
			}

		}
		i++;
	}

}

///////////////////////////////////////////////////////////////////////////////////////////
int main() {

	struct zmienna *tab = malloc(sizeof(struct zmienna) * 26);
	char *tab_znakow;
	int dlugosc_tab;
	int dlugosc_calkowita_tab;
	int licznik = 0;

///////////////////////////////////////////////////////////////////////////////////////////
//INICJALIZACJA TABLIC
	for (int i = 0; i < 26; i++) {
		init(&tab[i]);
	}

	init_2(&tab_znakow, &dlugosc_tab, &dlugosc_calkowita_tab);
///////////////////////////////////////////////////////////////////////////////////////////
//WRZUCENIE ZNAKÓW DO TABLICY ZNAKÓW

	while (scanf("%c", &tab_znakow[licznik]) == 1) {
		licznik++;
		dlugosc_tab++;
		if (dlugosc_tab == dlugosc_calkowita_tab) {
			zwieksz_rozmiar(&tab_znakow, &dlugosc_tab, &dlugosc_calkowita_tab);
		}

		if (tab_znakow[licznik - 1] == '\n') {
			licznik--;
			dlugosc_tab--;
		}

	}
	//printf("%dASDSADKLKAS", tab[0].dlugosc);
///////////////////////////////////////////////////////////////////////////////////////////

	wypisz_instrukcje(tab_znakow, dlugosc_tab);
	wykonaj(tab);




	//printf("%d", tab[2].dlugosc);
///////////////////////////////////////////////////////////////////////////////////////////
//ZWOLNIENIE PAMIĘCI
	for (int i = 0; i < 26; i++) {
		uninit(tab[i]);
	}
	free(tab);
	free(tab_znakow);
}