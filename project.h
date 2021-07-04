/*
 * Progetto.h
 *
 *  Created on: 20 mar 2021
 *  Author: Matteo Marraffa
 */

#ifndef PROGETTO_H_
#define PROGETTO_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**definizione delle costanti per:
 * Lunghezza massima di una stringa breve
 * Lunghezza massima di una stringa
 * Numero massimo di registrazioni per un utente
 * Numero massimo di utenti
 * Lunghezza massima del buffer utilizzato dalla funzione per leggere il file
 * Path del file utenti
 * Path del file delle registrazioni
 */

#define MAX_STRING_SHORT 15
#define MAX_STRING 50
#define MAX_REGISTRAZIONI 200
#define MAX_UTENTI 50
#define MAX_BUFFER 128
#define path_utenti "nomi.txt"
#define path_registrazioni "registrazioni.txt"

/**
 * Struttura per rappresentare una data
 */

typedef struct {
	int giorno;
	int mese;
	int anno;
} Data;


/**
 * Struttura per rappresentare un orario
 */
typedef struct {
	int minuti;
	int ore;
} Orario;

/**
 * Struttura per rappresentare una registrazione di un utente.
 * Composta da:
 * tipologia -> evento / promemoria / appuntamento
 * descrizione -> descrizione della registrazione
 * data -> data della registrazione
 * orario -> orario della registrazione
 * prioritario -> priorità della registrazione
 * */

typedef struct {
	char tipologia[MAX_STRING_SHORT];
	char descrizione[MAX_STRING];
	Data data;
	Orario orario;
	char prioritario[MAX_STRING_SHORT];
} Registrazione;

/**
 * Struttura principale, per rappresentare un utente con le registrazioni a lui relative.
 * Composta da:
 * nome -> nome dell'utente
 * cognome -> cognome dell'utente
 * registrazioni[] -> array delle registrazioni relatice all'utente
 * n_registrazioni -> variabile intera riportante il numero di registrazioni per quell'utente
 * */

typedef struct {
	char nome[MAX_STRING];
	char cognome[MAX_STRING];
	Registrazione registrazioni[MAX_REGISTRAZIONI];
	int n_registrazioni;
} Utente;

int valuta_intero_input();
bool leggi_file_utenti(int *n_utenti, Utente utenti[]);
bool leggi_file_registrazioni(Utente utenti[], const int n_utenti);
bool scrivi_su_file(const Utente utenti[], const int *n_utenti);
void stampa_registrazione_utente(const Registrazione reg, const Utente utente);
void stampa_menu();
void stampa_criteri_ricerca();
void ricerca_per_data(const Data d, const Utente utente);
void ricerca_per_range_date(const Data d1, const Data d2, const Utente utente);
void ricerca_per_tipologia(char tipologia[], const Utente utente);
void ricerca_per_utente(const Utente ut, const Utente utenti[],
		const int n_utenti);
Data inserisci_data();
void ricerca_per_criterio(const Utente utenti[], const int n_utenti);
void modifica_registrazione(Utente utenti[], int index_utente,
		int index_registrazione);
void inserisci_registrazione(Registrazione *r, Utente utente, int par);
void mostra_utenti(const Utente utenti[], const int n_utenti);
void ricerca_per_criterio(const Utente utenti[], const int n_utenti);
void aggiungi_cancella_modifica(Utente utenti[], int *n_utenti);
int stampa_aggiungi_cancella_modifica(char p[]);
void elimina_dati(Utente utenti[], int *n_utenti, int par);
void modifica_dati(Utente utenti[], int *n_utenti, int par);
void inserisci_dati(Utente utenti[], int *n_utenti, int par);
void elimina_utente(Utente utenti[], int *n_utenti, int index_utente);
void modifica_utente(Utente utenti[], const int *n_utenti, int index_utente);
void mostra_registrazioni(Utente utenti[], int *n_utenti, char mod[]);
void elimina_registrazione(Utente utenti[], int *n_utenti, int index_utente,
		int index_registrazione);
void stampa_utenti(const Utente utenti[], const int n);
void ricerca_per_descrizione(const Utente ut, char descrizione[]);
bool registrazione_presente(const int ind_reg[], const int cont_index,
		const int p);
void statistiche(const Utente utenti[], const int n_utenti);
void mostra_statistica(const Utente utente,
		const int index_registrioni_trovate[], const int cont_index,
		const int par);
void mostra_registrazioni_ordinate(const Utente utenti[], const int n_utenti);
void stampa_ordinate(const Utente utente, const int par);
void ordina(Registrazione reg[], int n_reg, int par);
void scambia(Registrazione *r1, Registrazione *r2);
void stampa_ordinate(const Utente utente, const int par);
bool data_compresa(const Data d1,const  Data d2, const Data dx);

#endif /* PROGETTO_H_ */
