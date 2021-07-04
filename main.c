/*
 ============================================================================
 Name        : Progetto.c
 Author      : Matteo Marraffa
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include "Progetto.h"
/**
 *
 * main
 */
int main(void) {

	system("color 5f");

	Utente utenti[MAX_UTENTI];
	int n_utenti = 0;

	///leggere da file utenti e salva i dati nell'array utenti
	if (leggi_file_utenti(&n_utenti, utenti))
		printf("Utenti acquisiti con successo!\n");
	else {
		printf("Errore apertura file utenti! Fine del programma.");
		system("PAUSE");
		return 0;
	}

	///leggere da file registrazioni e salva i dati nell'array utenti
	if (leggi_file_registrazioni(utenti, n_utenti))
		printf("Registrazioni acquisite con successo!\n");
	else {
		printf("Errore apertura file registrazioni! Fine del programma.");
		system("PAUSE");
		return 0;
	}

	int fine = 1;
	int scelta;

	///ciclo con sentinella, si ferma se scelta = 5
	while (fine == 1) {
		do {
			stampa_menu();
			scelta = valuta_intero_input();
		} while (scelta < 1 || scelta > 5);

		switch (scelta) {
		case 1:
			///chiamata alla funzione per gestire aggiunte modifiche e eliminazioni
			aggiungi_cancella_modifica(utenti, &n_utenti);
			break;
		case 2:
			///chiamata alla funzione per ricercare registrazioni utente in base a un criterio
			ricerca_per_criterio(utenti, n_utenti);
			break;
		case 3:
			///chiamata alla funzione per visualizzare le statistiche delle registrazioni di un utente
			statistiche(utenti, n_utenti);
			break;
		case 4:
			///chiamata alla funzione per visualizzare le registrazioni di un utente, ordinate
			mostra_registrazioni_ordinate(utenti, n_utenti);
			break;
		case 5:
			printf("Chiusura programma...");
			fine = 0;
			break;
		}
	}
	system("PAUSE");
	return 0;
}
