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
 * Funzione che garantisce la corretta acquisizione di un intero attraverso la scanf.
 * L'utilizzo di questa funzione è dovuto ai comportamenti "imprevedibili" della funzione scanf(),
 * quando invece che un numero, per esempio, inseriamo un carattere o una stringa.
 * Una volta avvenuta la corretta acquisizione da tastiera, la funzione ritorna il valore intero inserito, se
 * non è intero, continuerà a chiedere di inserirlo in modo corretto
 */
int valuta_intero_input() {
	int numero_da_prendere_in_input;
	while (scanf("%d", &numero_da_prendere_in_input) != 1) {
		printf("Inserisci un intero non una stringa:\n");
		/* rimuovo eventuali caratteri rimasti nel buffer fino al newline */
		while (getchar() != '\n')
			;
	}
	return numero_da_prendere_in_input;
}

/**
 * Funzione per leggere il file utenti presenti all'indirizzo definito in "path_utenti"
 *
 * La funzione sfrutta la composizione del file di testo, i cui campi sono suddividi da virgole
 *
 * Se il numero di utenti nel file supera il numero massimo di utenti che è possibile allocare nell'array utenti[]
 * il programma termina l'esecuzione dando un messaggio di avviso
 */
/**
 *
 * @param n_utenti
 * @param utenti
 * @return
 */
bool leggi_file_utenti(int *n_utenti, Utente utenti[]) {
	FILE *fp;
	fp = fopen(path_utenti, "r");
	if (fp == NULL)
		return false;

	char riga[MAX_BUFFER];
	char *nome;
	char *cognome;

	while (fgets(riga, sizeof(riga), fp) != NULL) {
		if (*n_utenti < MAX_UTENTI) {
			nome = strtok(riga, ",");
			cognome = strtok(NULL, "\n");
			strcpy(utenti[*n_utenti].nome, nome);
			strcpy(utenti[*n_utenti].cognome, cognome);
			///settiamo a 0 il numero di registrazioni per ciascun utente
			utenti[*n_utenti].n_registrazioni = 0;
			*n_utenti = *n_utenti + 1;
		} else {
			printf(
					"Numero di utenti troppo grande. Aumentare dimensione array utenti\nFINE PROGRAMMA\n");
			return false;
		}
	}

	return true;
}

/**
 * Funzione per leggere il file registrazioni presenti all'indirizzo definito in "path_utenti"
 *
 * La funzione sfrutta la composizione del file di testo, i cui campi sono suddividi da virgole
 *
 * Se il numero di registrazioni nel file supera il numero massimo di registrazione che è possibile allocare nell'array registrazioni di quell'utente
 * il programma termina l'esecuzione dando un messaggio di avviso.
 *
 * Il programma termina l'esecuzione anche se l'utente presente nella registrazione non è stato registrato nell'array degli utenti
 */
/**
 *
 * @param utenti
 * @param n_utenti
 * @return
 */
bool leggi_file_registrazioni(Utente utenti[], const int n_utenti) {
	FILE *fp;
	fp = fopen(path_registrazioni, "r");
	if (fp == NULL)
		return false;

	char riga[MAX_BUFFER];

	char *tipologia;
	char *descrizione;
	char *data;
	char *orario;
	char *nome;
	char *cognome;
	char *prioritario;
	int giorno;
	int mese;
	int anno;
	int minuto;
	int ora;

	int trovato;

	while (fgets(riga, sizeof(riga), fp) != NULL) {
		tipologia = strtok(riga, ",");
		descrizione = strtok(NULL, ",");
		data = strtok(NULL, ",");
		orario = strtok(NULL, ",");
		nome = strtok(NULL, ",");
		cognome = strtok(NULL, ",");
		prioritario = strtok(NULL, "\n");

		trovato = 0;
///		printf("%s - %s - %s - %s\n", tipologia, data, orario, prioritario);

		for (int i = 0; i < n_utenti && trovato == 0; i++) {

			if ((strcmp(utenti[i].nome, nome) == 0)
					&& (strcmp(utenti[i].cognome, cognome) == 0)) {
				if (utenti[i].n_registrazioni < MAX_REGISTRAZIONI) {
					strcpy(
							utenti[i].registrazioni[utenti[i].n_registrazioni].tipologia,
							tipologia);
					strcpy(
							utenti[i].registrazioni[utenti[i].n_registrazioni].descrizione,
							descrizione);
					giorno = atoi(strtok(data, "/"));
					mese = atoi(strtok(NULL, "/"));
					anno = atoi(strtok(NULL, "/"));
					utenti[i].registrazioni[utenti[i].n_registrazioni].data.giorno =
							giorno;
					utenti[i].registrazioni[utenti[i].n_registrazioni].data.mese =
							mese;
					utenti[i].registrazioni[utenti[i].n_registrazioni].data.anno =
							anno;
					ora = atoi(strtok(orario, ":"));
					minuto = atoi(strtok(NULL, ":"));
					utenti[i].registrazioni[utenti[i].n_registrazioni].orario.ore =
							ora;
					utenti[i].registrazioni[utenti[i].n_registrazioni].orario.minuti =
							minuto;
					strcpy(
							utenti[i].registrazioni[utenti[i].n_registrazioni].prioritario,
							prioritario);

					///incremento n_prenotazioni per quell'utente
					utenti[i].n_registrazioni = utenti[i].n_registrazioni + 1;
				} else {
					printf(
							"Numero di registrazioni per l'utente %s %s troppo elevata. Aumentare dimensione array registrazione degli utenti\nFINE PROGRAMMA\n",
							utenti[i].nome, utenti[i].cognome);
					return false;
				}
				//
				trovato = 1;
			}
		}

		if (trovato == 0) {
			printf(
					"Errore, l'utente %s %s nel file delle registrazioni non registrato tra gli utenti",
					nome, cognome);
			return false;
		}
	}

	return true;
}

/**
 * Funzione per riscrivere il file degli utenti e delle registrazioni.
 * Quando viene chiamata, il file di testo verrà riscritto con i dati presenti nell'array utenti passato come parametro
 *
 */
/**
 *
 * @param utenti
 * @param n_utenti
 * @return
 */
bool scrivi_su_file(const Utente utenti[], const int *n_utenti) {
	FILE *fp1;
	FILE *fp2;

	/** apre il file in scrittura */
	fp1 = fopen(path_utenti, "w");
	if (fp1 == NULL) {
		printf("Errore in apertura del file utenti in scrittura");
		return false;
	}

	for (int i = 0; i < *n_utenti; i++) {
		fprintf(fp1, "%s,%s\n", utenti[i].nome, utenti[i].cognome);
	}

	fclose(fp1);

	/** apre il file in scrittura */
	fp2 = fopen(path_registrazioni, "w");
	if (fp2 == NULL) {
		printf("Errore in apertura del file registrazioni in scrittura");
		return false;
	}

	Utente u;
	int n_reg;

	for (int i = 0; i < *n_utenti; i++) {
		u = utenti[i];
		n_reg = utenti[i].n_registrazioni;

		for (int j = 0; j < n_reg; j++) {
			fprintf(fp2, "%s,%s,%d/%d/%d,%d:%d,%s,%s,%s\n",
					u.registrazioni[j].tipologia,
					u.registrazioni[j].descrizione,
					u.registrazioni[j].data.giorno,
					u.registrazioni[j].data.mese, u.registrazioni[j].data.anno,
					u.registrazioni[j].orario.ore,
					u.registrazioni[j].orario.minuti, u.nome, u.cognome,
					u.registrazioni[j].prioritario);
		}

	}

	fclose(fp2);

	return true;
}

/**
 * Funzione per la stampa di una singola registrazione di un utente
 */
/**
 *
 * @param reg
 * @param utente
 */
void stampa_registrazione_utente(Registrazione reg, Utente utente) {
	printf("Nome: %s\nCognome: %s\n", utente.nome, utente.cognome);
	printf("Tipologia: %s\n", reg.tipologia);
	printf("Descrizione: %s\n", reg.descrizione);
	printf("Data: %d/%d/%d\n", reg.data.giorno, reg.data.mese, reg.data.anno);
	printf("Orario: %d:%d\n", reg.orario.ore, reg.orario.minuti);
	printf("Priorita': %s\n", reg.prioritario);
}

/**
 * Funzione per la stampa del menu per scegliere se aggiungere modicare o elimare un utente o una registrazione
 *
 * Ritorna la scelta effettuata
 */
/**
 *
 * @param p
 * @return
 */
int stampa_aggiungi_cancella_modifica(char p[]) {
	int scelta_opzione;
	do {
		printf("Cosa vuoi fare?\n");
		printf("1)Aggiungi %s\n", p);
		printf("2)Modifica %s\n", p);
		printf("3)Elimina %s\n", p);
		printf("Scelta: ");
		scelta_opzione = valuta_intero_input();
	} while (scelta_opzione < 1 || scelta_opzione > 3);
	return scelta_opzione;
}

/**
 * Funzione per la stampa del menù principale
 */
void stampa_menu() {
	printf("\n\n\n*--------MENU'-------*\n");
	printf("[1] Aggiungi - Modifica - Cancella\n");
	printf("[2] Ricerca registrazione\n");
	printf("[3] Statistiche\n");
	printf("[4] Mostra registrazioni utente ordinate\n");
	printf("[5] FINE/ESCI\n");

	printf("Scegli opzione: ");
}

/**
 * Funzione per la stampa del menù dei criteri di ricerca
 */

void stampa_criteri_ricerca() {
	printf("Con quale criterio vuoi effettuare la ricerca?\n");
	printf("[1] Ricerca per data\n");
	printf("[2] Ricerca in un range di date\n");
	printf("[3] Ricerca per tipologia di registrazione\n");
	printf("[4] Ricerca per descrizione registrazione\n");

	printf("Scegli opzione: ");
}

/**
 * Funzione che data in ingresso una data e un utente, ricerca tutte le registrazioni corrispondenti a quella data per quell'utente
 */
/**
 *
 * @param d
 * @param utente
 */
void ricerca_per_data(const Data d, const Utente utente) {
	printf("Le registrazioni della data: %d/%d/%d sono: \n", d.giorno, d.mese,
			d.anno);
	int cont = 0;
	for (int i = 0; i < utente.n_registrazioni; i++) {
		if (utente.registrazioni[i].data.giorno == d.giorno
				&& utente.registrazioni[i].data.mese == d.mese
				&& utente.registrazioni[i].data.anno == d.anno) {
			printf("------------------------\n%d:\n", (cont + 1));
			stampa_registrazione_utente(utente.registrazioni[i], utente);
			cont++;
		}
	}

	if (cont == 0) {
		printf("Nessuna registrazione trovata in data %d/%d/%d\n", d.giorno,
				d.mese, d.anno);
	}
}

/**
 * Funzione per mostrare a video le statistiche relative alle registrazioni di un utente.
 *
 * Ha come argomenti l'utente, gli indici delle registrazioni per quell'utente da mostrare, e un parametro
 * per distinguere che tipo di statistica si vuole mostrare
 */
/**
 *
 * @param utente
 * @param index_registrioni_trovate
 * @param cont_index
 * @param par
 */
void mostra_statistica(const Utente utente,
		const int index_registrioni_trovate[], const int cont_index,
		const int par) {
	int ind;
	if (par == 1) {
		int cont_evento = 0, cont_appuntamento = 0, cont_prome = 0;
		for (int i = 0; i < cont_index; i++) {
			ind = index_registrioni_trovate[i];
			if (strcmp(utente.registrazioni[ind].tipologia, "evento") == 0) {
				cont_evento++;
			} else if (strcmp(utente.registrazioni[ind].tipologia,
					"appuntamento") == 0) {
				cont_appuntamento++;
			} else {
				cont_prome++;
			}
		}

		double n = (double) cont_index;
		printf("Numero totale registrazioni in agenda tra queste date: %d\n",
				cont_index);
		printf("Numero eventi: %d -> %.2lf %%\n", cont_evento,
				(cont_evento * 100 / n));
		printf("Numero promemoria: %d -> %.2lf %%\n", cont_prome,
				(cont_prome * 100 / n));
		printf("Numero appuntamento: %d -> %.2lf %%\n", cont_appuntamento,
				(cont_appuntamento * 100 / n));
	} else if (par == 2) {
		int cont_prioritario = 0;
		for (int i = 0; i < cont_index; i++) {
			ind = index_registrioni_trovate[i];
			if (strcmp(utente.registrazioni[ind].prioritario, "prioritario")
					== 0) {
				cont_prioritario++;
			}
		}

		double n = (double) cont_index;
		printf("Numero totale registrazioni in agenda tra queste date: %d\n",
				cont_index);
		printf("Numero registrazioni con priorita': %d -> %.2lf %%\n",
				cont_prioritario, (cont_prioritario * 100 / n));
		printf("Numero registrazioni senza priorita': %d -> %.2lf %%\n",
				(cont_index - cont_prioritario),
				((cont_index - cont_prioritario) * 100 / n));

	}
}

/**
 * Funzione che prese due date, e una terza data da valutare, ritorna true e la data da valutare
 * è compresa tra le due date. False altrimenti
 */
/**
 *
 * @param d1
 * @param d2
 * @param dx
 * @return
 */
bool data_compresa(const Data d1, const Data d2, const Data dx) {
	bool compreso = false;

	if (d1.anno < dx.anno && d2.anno > dx.anno) {
		compreso = true;
	} else {
		if (d1.anno == dx.anno || d2.anno == dx.anno) {
			if (d1.mese < dx.mese && d2.mese > dx.mese) {
				compreso = true;
			} else {
				if (d1.mese == dx.mese || d2.mese == dx.mese) {
					if (d1.giorno <= dx.giorno && d2.giorno >= dx.giorno) {
						compreso = true;
					}
				}
			}
		}
	}

	return compreso;
}

/**
 * Funzione per mostrare all'utente le statistiche relative le registrazioni dell'utente in un certo intervallo di tempo
 */
/**
 *
 * @param utenti
 * @param n_utenti
 */
void statistiche(const Utente utenti[], const int n_utenti) {
	Data d1, d2;
	int scelta_utente;
	int scelta_stat;
	int index_registrazioni_trovate[MAX_REGISTRAZIONI];
	int cont_index = 0;
	int index;
	printf("Inserisci nome utente di cui visualizzare le statistiche: \n");
	do {
		mostra_utenti(utenti, n_utenti);
		scelta_utente = valuta_intero_input();
	} while (scelta_utente < 1 || scelta_utente > n_utenti);
	index = scelta_utente - 1;

	printf("Inserisci data inizio: ");
	d1 = inserisci_data();
	printf("Inserisci data fine: ");
	d2 = inserisci_data();

	for (int i = 0; i < utenti[index].n_registrazioni; i++) {
		if (data_compresa(d1, d2, utenti[index].registrazioni[i].data)) {
			index_registrazioni_trovate[cont_index] = i;
			cont_index++;
		}
	}

	do {
		printf("\nQuali statistiche vuoi visualizzare?\n");
		printf("1) Statistica su tipologia di registrazione\n");
		printf("2) Percentuale registrazioni con priorita'\n");
		printf("Scelta: ");
		scelta_stat = valuta_intero_input();
	} while (scelta_stat < 1 || scelta_stat > 2);

	if (scelta_stat == 1) {
		mostra_statistica(utenti[index], index_registrazioni_trovate,
				cont_index, 1);
	} else if (scelta_stat == 2) {
		mostra_statistica(utenti[index], index_registrazioni_trovate,
				cont_index, 2);
	}

}

/**
 * Funzione che data in ingresso due date e un utente, ricerca tutte le registrazioni tra quelle date per quell'utente
 */
/**
 *
 * @param d1
 * @param d2
 * @param utente
 */
void ricerca_per_range_date(const Data d1, const Data d2, const Utente utente) {
	printf("\nLe registrazioni tra le date: %d/%d/%d e %d/%d/%d sono: \n",
			d1.giorno, d1.mese, d1.anno, d2.giorno, d2.mese, d2.anno);
	int cont = 0;
	for (int i = 0; i < utente.n_registrazioni; i++) {
		if (data_compresa(d1, d2, utente.registrazioni[i].data)) {
			printf("------------------------\n%d:\n", (cont + 1));
			stampa_registrazione_utente(utente.registrazioni[i], utente);
			cont++;
		}
	}

	if (cont == 0) {
		printf(
				"Nessuna registrazione trovata tra le date: %d/%d/%d e %d/%d/%d\n",
				d1.giorno, d1.mese, d1.anno, d2.giorno, d2.mese, d2.anno);
	}
}

/**
 * Funzione per stampare tutte le registrazioni di un utente corrispondenti a una determinata tipologia
 * passata come parametro
 */
/**
 *
 * @param tipologia
 * @param utente
 */
void ricerca_per_tipologia(char tipologia[], const Utente utente) {
	Registrazione r;
	int cont = 0;
	printf("Le registrazioni corrispondenti alla tipologia %s sono:\n",
			tipologia);
	for (int i = 0; i < utente.n_registrazioni; i++) {
		r = utente.registrazioni[i];
		if (strcmp(r.tipologia, tipologia) == 0) {
			printf("------------------------\n%d:\n", (cont + 1));
			stampa_registrazione_utente(utente.registrazioni[i], utente);
			cont++;
		}
	}
	if (cont == 0) {
		printf("Nessuna registrazione trovata di tipologia %s\n", tipologia);
	}

}

/**
 * Funzione per stampare tutte le registrazioni di un utente corrispondenti a una determinata descrizione
 * passata come parametro. La funzione se non trova un matching esatto con la descrizione passata,
 * Effettuerà una ricerca anche sulle sottostringhe che matchano
 */
/**
 *
 * @param ut
 * @param descrizione
 */
void ricerca_per_descrizione(const Utente ut, char descrizione[]) {
	int cont = 0;

	int trovato_matching_esatto = 0;
	int trovato_submatching = 0;
	printf("Le registrazione corrispondenti sono: \n");
	for (int j = 0; j < ut.n_registrazioni; j++) {
		if (strcmp(ut.registrazioni[j].descrizione, descrizione) == 0) {
			printf("------------------------\n%d:\n", (cont + 1));
			stampa_registrazione_utente(ut.registrazioni[j], ut);
			cont++;
			trovato_matching_esatto = 1;

		}
	}

	int index_registrioni_trovate[MAX_REGISTRAZIONI];
	int cont_index = 0;

	if (trovato_matching_esatto == 0) {
		char *token;
		token = strtok(descrizione, " ");

		if (token != NULL) {
			for (int j = 0; j < ut.n_registrazioni; j++) {
				if (strstr(ut.registrazioni[j].descrizione, token) != NULL) {
					trovato_submatching = 1;
					if (!registrazione_presente(index_registrioni_trovate,
							cont_index, j)) {
						index_registrioni_trovate[cont_index] = j;
						cont_index++;
					}
				}
			}
		}

		while (token != NULL) {
			token = strtok(NULL, " ");

			if (token != NULL) {
				for (int j = 0; j < ut.n_registrazioni; j++) {
					if (strstr(ut.registrazioni[j].descrizione, token) != NULL) {
						trovato_submatching = 1;
						if (!registrazione_presente(index_registrioni_trovate,
								cont_index, j)) {
							index_registrioni_trovate[cont_index] = j;
							cont_index++;
						}
					}
				}
			}
		}
	}

	if (cont_index > 0) {
		cont = 0;
		int id;
		for (int i = 0; i < cont_index; i++) {
			id = index_registrioni_trovate[i];
			printf("------------------------\n%d:\n", (cont + 1));
			stampa_registrazione_utente(ut.registrazioni[id], ut);
			cont++;
		}
	} else if (trovato_matching_esatto == 0 && trovato_submatching == 0) {
		printf("Nessuna corrispondenza trovata\n");
	}

}


/**
 * Funzione che data un array di indici ind_reg e un indice p, torna true se p è già presente.
 * False altrimenti
 */
/**
 *
 * @param ind_reg
 * @param cont_index
 * @param p
 * @return presente
 */
bool registrazione_presente(const int ind_reg[], const int cont_index,
		const int p) {
	bool presente = false;

	for (int i = 0; i < cont_index; i++) {
		if (ind_reg[i] == p)
			presente = true;
	}
	return presente;
}

/**
 * Funzione per l'inserimento di una data. Ritorna la data inserita dopo aver fatto diversi controlli
 * sull'esistenza effettiva della data inserita
 */

Data inserisci_data() {
	Data d;
	int gg, mm, aa;
	printf("Anno: ");
	do {
		aa = valuta_intero_input();
	} while (aa < 2000);

	d.anno = aa;

	printf("Mese: ");
	do {
		mm = valuta_intero_input();
	} while (mm < 1 || mm > 12);

	d.mese = mm;

	printf("Giorno: ");
	if (mm == 4 || mm == 6 || mm == 9 || mm == 11) {
		do {
			gg = valuta_intero_input();
		} while (gg < 1 || gg > 30);
	} else if (mm == 2
			&& (aa % 400 == 0 || (aa % 4 == 0 && !(aa % 100 == 0)))) {
		do {
			gg = valuta_intero_input();
		} while (gg < 1 || gg > 29);
	} else if (mm == 2) {
		do {
			gg = valuta_intero_input();
		} while (gg < 1 || gg > 28);
	} else {
		do {
			gg = valuta_intero_input();
		} while (gg < 1 || gg > 31);
	}

	d.giorno = gg;

	return d;
}

/**
 * Funzione per la gestione della funzionalità di ricerca registrazioni in base a un criterio
 */
/**
 *
 * @param utenti
 * @param n_utenti
 */
void ricerca_per_criterio(const Utente utenti[], int n_utenti) {
	int scelta;
	int scelta_tipologia;
	char descrizione[MAX_STRING];
	int scelta_utente;
	int index;
	printf("Inserisci nome utente di cui visualizzare le registrazioni: \n");
	do {
		mostra_utenti(utenti, n_utenti);
		scelta_utente = valuta_intero_input();
	} while (scelta_utente < 1 || scelta_utente > n_utenti);
	index = scelta_utente - 1;
	Utente ut = utenti[index];

	Data d1;
	Data d2;

	do {
		stampa_criteri_ricerca();
		scelta = valuta_intero_input();
	} while (scelta < 1 || scelta > 4);

	switch (scelta) {
	case 1:
		printf("Data da ricercare:\n");
		d1 = inserisci_data();
		ricerca_per_data(d1, ut);
		break;
	case 2:
		printf("Data inizio: \n");
		d1 = inserisci_data();
		printf("Data fine: \n");
		d2 = inserisci_data();
		ricerca_per_range_date(d1, d2, ut);
		break;
	case 3:
		printf(
				"Tipologia da ricercare:\n 1)evento 2)appuntamento 3)promemoria");
		do {
			scelta_tipologia = valuta_intero_input();
		} while (scelta_tipologia < 1 || scelta_tipologia > 3);
		if (scelta_tipologia == 1) {
			ricerca_per_tipologia("evento", ut);
		} else if (scelta_tipologia == 2) {
			ricerca_per_tipologia("appuntamento", ut);
		} else {
			ricerca_per_tipologia("promemoria", ut);
		}

		break;
	case 4:
		printf("Inserisci descrizione da cercare nelle registrazioni: ");
		while (getchar() != '\n')
			;
		scanf("%[^\n]", descrizione);
		ricerca_per_descrizione(ut, descrizione);
		break;
	}
}

/**
 * Funzione per la stampa di una lista di utenti
 */
/**
 *
 * @param utenti
 * @param n_utenti
 */
void mostra_utenti(const Utente utenti[], const int n_utenti) {
	for (int i = 0; i < n_utenti; i++) {
		printf("%d %s %s\n", (i + 1), utenti[i].nome, utenti[i].cognome);
	}
}

/**
 * Funzione per la stampa di una lista di registrazioni da modificare o eliminare
 */
/**
 *
 * @param utenti
 * @param n_utenti
 * @param mod
 */
void mostra_registrazioni(Utente utenti[], int *n_utenti, char mod[]) {
	int scelta_utente;
	int scelta_registrazione;
	printf("Seleziona l'utente di cui vuoi %s una registrazione: \n", mod);
	printf("%5s %15s %15s %5s\n", "codice", "nome", "cognome",
			"numero_registrazioni");
	for (int i = 0; i < *n_utenti; i++) {
		if (utenti[i].n_registrazioni > 0)
			printf("%5d %15s %15s %5d\n", (i + 1), utenti[i].nome,
					utenti[i].cognome, utenti[i].n_registrazioni);
	}

	do {
		printf("Scelta codice utente: ");
		scelta_utente = valuta_intero_input();
	} while (scelta_utente < 1 || scelta_utente > *n_utenti
			|| utenti[scelta_utente - 1].n_registrazioni == 0);

	int index_scelta_utente = scelta_utente - 1;

	printf("Quale delle seguenti registrazione di %s %s vuoi %s?\n",
			utenti[index_scelta_utente].nome,
			utenti[index_scelta_utente].cognome, mod);
	for (int i = 0; i < utenti[index_scelta_utente].n_registrazioni; i++) {
		printf("Codice: %d\n", (i + 1));
		printf("Tipologia: %s\n",
				utenti[index_scelta_utente].registrazioni[i].tipologia);
		printf("Descrizione: %s\n",
				utenti[index_scelta_utente].registrazioni[i].descrizione);
		printf("Data: %d/%d/%d\n",
				utenti[index_scelta_utente].registrazioni[i].data.giorno,
				utenti[index_scelta_utente].registrazioni[i].data.mese,
				utenti[index_scelta_utente].registrazioni[i].data.anno);
		printf("Orario: %d:%d\n",
				utenti[index_scelta_utente].registrazioni[i].orario.ore,
				utenti[index_scelta_utente].registrazioni[i].orario.minuti);
		printf("Priorita': %s\n",
				utenti[index_scelta_utente].registrazioni[i].prioritario);
		printf("---------------------------------\n");
	}

	do {
		printf("Codice registrazione da %s: ", mod);
		scelta_registrazione = valuta_intero_input();
	} while (scelta_registrazione < 1
			|| scelta_registrazione
					> utenti[index_scelta_utente].n_registrazioni);

	int index_scelta_registrazione = scelta_registrazione - 1;

	if (strcmp(mod, "eliminare") == 0) {
		elimina_registrazione(utenti, n_utenti, index_scelta_utente,
				index_scelta_registrazione);
	} else if (strcmp(mod, "modificare") == 0) {
		modifica_registrazione(utenti, index_scelta_utente,
				index_scelta_registrazione);
	}

}

/**
 * Funzione che permette l'inserimento di una nuova registrazione per un utente
 */
/**
 *
 * @param r
 * @param utente
 * @param par
 */
void inserisci_registrazione(Registrazione *r, Utente utente, int par) {
	int ora;
	int minuto;
	int priorita;
	int scelta_tipologia;

	printf("Tipologia: 1)evento 2)appuntamento 3)promemoria:");
	do {
		scelta_tipologia = valuta_intero_input();
	} while (scelta_tipologia < 1 || scelta_tipologia > 3);

	if (scelta_tipologia == 1) {
		strcpy(r->tipologia, "evento");
	} else if (scelta_tipologia == 2) {
		strcpy(r->tipologia, "appuntamento");
	} else if (scelta_tipologia == 3) {
		strcpy(r->tipologia, "promemoria");
	}

	printf("Descrizione: ");
	while (getchar() != '\n')
		;
	scanf("%[^\n]", r->descrizione);

	if (par == 1) {
		printf("Data:\n");
		inserisci_data(&r->data);

		printf("Ora (0-23): ");
		do {
			ora = valuta_intero_input();
		} while (ora < 0 || ora > 23);

		printf("Minuto inizio (0-59): ");
		do {
			minuto = valuta_intero_input();
		} while (minuto < 0 || minuto > 59);
		r->orario.ore = ora;
		r->orario.minuti = minuto;
	}

	printf("\nInserire priorita' della registrazione: 1)standard 2)prioritario:");
	do {
		priorita = valuta_intero_input();
	} while (priorita != 1 && priorita != 2);

	if (priorita == 1)
		strcpy(r->prioritario, "standard");
	else if (priorita == 2)
		strcpy(r->prioritario, "prioritario");
}

/**
 * Funzione per l'immissione di nuovi dati per gli utenti o per le registrazioni.
 *
 * Il parametro in ingresso par sarà 1 se si vuole aggiungere un utente 2 per le registrazioni.
 *
 * Se l'array degli utenti ha raggiunto la capienza massima verrà mostrato a video.
 * Se l'array delle registrazioni per un utente ha raggiunto la capienza massima verrà mostrato a video.
 *
 * Se si tenda di inserire una registrazione in uno slot già occupato per quell'utente, verrà segnalato errore a video
 *
 *
 */
/**
 *
 * @param utenti
 * @param n_utenti
 * @param par
 */
void inserisci_dati(Utente utenti[], int *n_utenti, int par) {
	int scelta;
	int index_utente;
	Data d;
	int ora;
	int minuto;

	while (getchar() != '\n')
		;
	if (par == 1) {
		if (*n_utenti < MAX_UTENTI) {
			printf("Inserisci utente\nNome: ");
			scanf("%[^\n]", utenti[*n_utenti].nome);
			printf("Cognome: ");
			while (getchar() != '\n')
				;
			scanf("%[^\n]", utenti[*n_utenti].cognome);

			*n_utenti = *n_utenti + 1;
		} else {
			printf(
					"Impossibile aggiungere altri utenti. Raggiunto il limite. Si consiglia di eliminarne qualcuno per liberare spazio\n");
			return;
		}
	} else {
		printf("A quale utente vuoi aggiungere una registrazione?\n");
		mostra_utenti(utenti, *n_utenti);
		do {
			printf("Inserisci il numero corrispondente: ");
			scelta = valuta_intero_input();
		} while (scelta <= 1 && scelta > *n_utenti);
		index_utente = scelta - 1;

		if (utenti[index_utente].n_registrazioni < MAX_REGISTRAZIONI) {
			printf("Data:\n");
			d = inserisci_data();

			printf("Ora (0-23): ");

			do {
				ora = valuta_intero_input();
			} while (ora < 0 || ora > 23);

			printf("Minuto inizio (0-59): ");
			do {
				minuto = valuta_intero_input();
			} while (minuto < 0 || minuto > 59);

			int slot_occupato = 0;
			for (int i = 0; i < utenti[index_utente].n_registrazioni; i++) {
				if (utenti[index_utente].registrazioni[i].data.anno == d.anno
						&& utenti[index_utente].registrazioni[i].data.mese
								== d.mese
						&& utenti[index_utente].registrazioni[i].data.giorno
								== d.giorno
						&& utenti[index_utente].registrazioni[i].orario.ore
								== ora
						&& utenti[index_utente].registrazioni[i].orario.minuti
								== minuto) {
					slot_occupato = 1;
				}
			}

			if (slot_occupato == 1) {
				printf(
						"\nLo slot orario che hai selezionato e' gia' occupato per questo utente.\nCancellalo per avere la possibilità di inserirne uno nuovo\no seleziona un diverso orario");
			} else {

				Utente *ut = &utenti[index_utente];
				int n_reg = ut->n_registrazioni;
				Registrazione *r = &ut->registrazioni[n_reg];
				r->data = d;
				r->orario.ore = ora;
				r->orario.minuti = minuto;
				inserisci_registrazione(r, utenti[index_utente], 0);
				utenti[index_utente].n_registrazioni++;
			}
		} else {
			printf(
					"\nImpossibile aggiungere altre registrazioni per questo utente. Raggiunto il limite. Si consiglia di eliminarne qualcuna per liberare spazio\n");
			return;
		}
	}
}

/**
 * Funzione per eliminare una registrazione per un utente
 */
/**
 *
 * @param utenti
 * @param n_utenti
 * @param index_utente
 * @param index_registrazione
 */
void elimina_registrazione(Utente utenti[], int *n_utenti, int index_utente,
		int index_registrazione) {

	utenti[index_utente].n_registrazioni = utenti[index_utente].n_registrazioni
			- 1;

	for (int i = index_registrazione; i < utenti[index_utente].n_registrazioni;
			i++) {
		utenti[index_utente].registrazioni[i] =
				utenti[index_utente].registrazioni[i + 1];
	}

}

/**
 * Funzione per eliminare un utente
 */
/**
 *
 * @param utenti
 * @param n_utenti
 * @param index_utente
 */
void elimina_utente(Utente utenti[], int *n_utenti, int index_utente) {
	*n_utenti = *n_utenti - 1;

	for (int i = index_utente; i < (*n_utenti); i++) {
		utenti[i] = utenti[i + 1];
	}
}


/**
 * Funzione per gestire interfaccia di eliminazione dati
 */
/**
 *
 * @param utenti
 * @param n_utenti
 * @param par
 */
void elimina_dati(Utente utenti[], int *n_utenti, int par) {
	int scelta;
	if (par == 1) {
		printf("Quale utente vuoi eliminare?\n");
		mostra_utenti(utenti, *n_utenti);
		do {
			printf("Inserisci il numero corrispondente: ");
			scelta = valuta_intero_input();
		} while (scelta <= 1 || scelta > *n_utenti);

		elimina_utente(utenti, n_utenti, (scelta - 1));
	} else {
		printf("Quale registrazione vuoi eliminare?");
		mostra_registrazioni(utenti, n_utenti, "eliminare");
	}
}

/**
 * Funzione per modicare dati di un utente
 */
/**
 *
 * @param utenti
 * @param n_utenti
 * @param index_utente
 */
void modifica_utente(Utente utenti[], const int *n_utenti, int index_utente) {
	printf("Vecchi valori utente: \n");
	printf("Nome: %s\n", utenti[index_utente].nome);
	printf("Cognome: %s\n", utenti[index_utente].cognome);

	printf("Inserisci nuovi valori: \n");
	while (getchar() != '\n')
		;
	printf("Nome: ");
	scanf("%[^\n]", utenti[index_utente].nome);
	while (getchar() != '\n')
		;

	printf("Cognome: ");
	scanf("%[^\n]", utenti[index_utente].cognome);
}

/**
 * Funzione per gestire interfaccia di modifica dati
 */
/**
 *
 * @param utenti
 * @param n_utenti
 * @param par
 */
void modifica_dati(Utente utenti[], int *n_utenti, int par) {
	int scelta;
	if (par == 1) {
		printf("Quale utente vuoi modificare?\n");
		mostra_utenti(utenti, *n_utenti);
		do {
			printf("Inserisci il numero corrispondente: ");
			scelta = valuta_intero_input();
		} while (scelta <= 1 && scelta > *n_utenti);

		modifica_utente(utenti, n_utenti, (scelta - 1));
	} else {
		printf("Quale registrazione vuoi modificare?\n");
		mostra_registrazioni(utenti, n_utenti, "modificare");
	}
}

/**
 * Funzione per modicare una determinata registrazione di un utente.
 *
 * Non sarà possibile modificarla settando una data e ora, già presente
 */
/**
 *
 * @param utenti
 * @param index_utente
 * @param index_registrazione
 */
void modifica_registrazione(Utente utenti[], int index_utente,
		int index_registrazione) {
	Data d;
	int ora;
	int minuto;
	printf("Vecchi valori: \n");
	stampa_registrazione_utente(
			utenti[index_utente].registrazioni[index_registrazione],
			utenti[index_utente]);
	printf("Inserisci nuovi valori: \n");

	printf("Data:\n");
	d = inserisci_data();

	printf("Ora (0-23): ");

	do {
		ora = valuta_intero_input();
	} while (ora < 0 || ora > 23);

	printf("Minuto inizio (0-59): ");
	do {
		minuto = valuta_intero_input();
	} while (minuto < 0 || minuto > 59);

	int slot_occupato = 0;
	for (int i = 0; i < utenti[index_utente].n_registrazioni; i++) {
		if (utenti[index_utente].registrazioni[i].data.anno == d.anno
				&& utenti[index_utente].registrazioni[i].data.mese == d.mese
				&& utenti[index_utente].registrazioni[i].data.giorno == d.giorno
				&& utenti[index_utente].registrazioni[i].orario.ore == ora
				&& utenti[index_utente].registrazioni[i].orario.minuti
						== minuto) {
			slot_occupato = 1;
		}
	}

	if (slot_occupato == 1) {
		printf(
				"Lo slot orario che hai selezionato e' gia' occupato per questo utente.\nCancellalo per avere la possibilita' di inserirne uno nuovo\no seleziona un diverso orario");
	} else {
		Registrazione *r =
				&utenti[index_utente].registrazioni[index_registrazione];
		r->data = d;
		r->orario.ore = ora;
		r->orario.minuti = minuto;
		inserisci_registrazione(r, utenti[index_utente], 0);
	}

}

/**
 * Funzione che gestisce l'interfaccia per aggiungere cancellare o modificare, utenti o registrazioni
 */
/**
 *
 * @param utenti
 * @param n_utenti
 */
void aggiungi_cancella_modifica(Utente utenti[], int *n_utenti) {
	int scelta;
	int scelta_opzione;

	do {
		printf("Selezionare con chi interagire:\n1)Utenti\n2)Registrazioni\n");
		printf("Scelta: ");
		scelta = valuta_intero_input();
	} while (scelta != 1 && scelta != 2);

	if (scelta == 1) {
		scelta_opzione = stampa_aggiungi_cancella_modifica("utente");
		if (scelta_opzione == 1) {
			inserisci_dati(utenti, n_utenti, 1);
		} else if (scelta_opzione == 2) {
			modifica_dati(utenti, n_utenti, 1);
		} else if (scelta_opzione == 3) {
			elimina_dati(utenti, n_utenti, 1);
		}

	} else {
		scelta_opzione = stampa_aggiungi_cancella_modifica("registrazione");
		if (scelta_opzione == 1) {
			inserisci_dati(utenti, n_utenti, 2);
		} else if (scelta_opzione == 2) {
			modifica_dati(utenti, n_utenti, 2);
		} else if (scelta_opzione == 3) {
			elimina_dati(utenti, n_utenti, 2);
		}
	}

	scrivi_su_file(utenti, n_utenti);
}

/**
 * Funzione per la copia delle registrazioni di un utente in un array di registrazioni
 */
/**
 *
 * @param utente
 * @param reg
 */
void copia_registrazioni(Utente utente, Registrazione reg[]) {
	for (int i = 0; i < utente.n_registrazioni; i++) {
		reg[i] = utente.registrazioni[i];
	}
}

/**
 * Funzione che scambia due registrazioni, utile per l'ordinamento
 */
/**
 *
 * @param r1
 * @param r2
 */
void scambia(Registrazione *r1, Registrazione *r2) {
	Registrazione temp;
	temp = *r1;
	*r1 = *r2;
	*r2 = temp;
}

/**
 * Funzione per l'ordinamento su una copia delle registrazioni, in base al parametro par
 * avremo differenti tipi di ordinamento
 */
/**
 *
 * @param reg
 * @param n_reg
 * @param par
 */
void ordina(Registrazione reg[], int n_reg, int par) {
	switch (par) {
	case 1:
		for (int i = 0; i < n_reg - 1; i++) {
			for (int j = 0; j < n_reg - 1; j++) {
				if (reg[j].data.anno > reg[j + 1].data.anno) {
					scambia(&reg[j], &reg[j + 1]);
				} else if (reg[j].data.anno == reg[j + 1].data.anno) {
					if (reg[j].data.mese > reg[j + 1].data.mese) {
						scambia(&reg[j], &reg[j + 1]);
					} else if (reg[j].data.mese == reg[j + 1].data.mese) {
						if (reg[j].data.giorno > reg[j + 1].data.giorno) {
							scambia(&reg[j], &reg[j + 1]);
						}
					}
				}
			}
		}
		break;
	case 2:
		for (int i = 0; i < n_reg - 1; i++) {
			for (int j = 0; j < n_reg - 1; j++) {
				if (strcmp(reg[j].tipologia, reg[j + 1].tipologia) > 0) {
					scambia(&reg[j], &reg[j + 1]);
				}
			}
		}
		break;

	case 3:
		for (int i = 0; i < n_reg - 1; i++) {
			for (int j = 0; j < n_reg - 1; j++) {
				if (reg[j].data.anno > reg[j + 1].data.anno) {
					scambia(&reg[j], &reg[j + 1]);
				} else if (reg[j].data.anno == reg[j + 1].data.anno) {
					if (reg[j].data.mese > reg[j + 1].data.mese) {
						scambia(&reg[j], &reg[j + 1]);
					} else if (reg[j].data.mese == reg[j + 1].data.mese) {
						if (reg[j].data.giorno > reg[j + 1].data.giorno) {
							scambia(&reg[j], &reg[j + 1]);
						} else if (reg[j].data.giorno
								== reg[j + 1].data.giorno) {
							if (reg[j].orario.ore > reg[j + 1].orario.ore) {
								scambia(&reg[j], &reg[j + 1]);
							} else if (reg[j].orario.ore
									== reg[j + 1].orario.ore) {
								if (reg[j].orario.minuti
										> reg[j + 1].orario.minuti) {
									scambia(&reg[j], &reg[j + 1]);
								}
							}
						}
					}
				}
			}
		}
		break;
	}
}

/**
 * Funzione per la stampa di una lista di registrazioni relative ad un utente
 */
/**
 *
 * @param reg
 * @param n_reg
 */
void stampa_registr(Registrazione reg[], int n_reg) {
	for (int i = 0; i < n_reg; i++) {
		printf("%d: \n", (i + 1));
		printf("Tipologia: %s\n", reg[i].tipologia);
		printf("Descrizione: %s\n", reg[i].descrizione);
		printf("Data: %d/%d/%d\n", reg[i].data.giorno, reg[i].data.mese,
				reg[i].data.anno);
		printf("Orario: %d:%d\n", reg[i].orario.ore, reg[i].orario.minuti);
		printf("Priorita': %s\n", reg[i].prioritario);
		printf("----------------------------\n");
	}
}

/**
 * Funzione per gestire l'interfaccia della funzionalità di stampa ordinata delle registrazioni di un utente.
 *
 * In base al parametro par passato in ingresso, il criterio di ordinamento sarà differente
 */
/**
 *
 * @param utente
 * @param par
 */
void stampa_ordinate(Utente utente, int par) {
	Registrazione reg_ordinate[MAX_REGISTRAZIONI];
	copia_registrazioni(utente, reg_ordinate);

	switch (par) {
	case 1:
		ordina(reg_ordinate, utente.n_registrazioni, par);
		printf("Le registrazioni ordinate per data sono: \n");
		stampa_registr(reg_ordinate, utente.n_registrazioni);
		break;
	case 2:
		ordina(reg_ordinate, utente.n_registrazioni, par);
		printf("Le registrazioni ordinate per tipologia sono: \n");
		stampa_registr(reg_ordinate, utente.n_registrazioni);
		break;
	case 3:
		ordina(reg_ordinate, utente.n_registrazioni, par);
		printf("Le registrazioni ordinate per data e ora sono: \n");
		stampa_registr(reg_ordinate, utente.n_registrazioni);
		break;
	}

}

/**Funzione per gestire l'interfaccia della funzionalità di stampa ordinata delle registrazioni
 *di un utente.
*/
/**
 *
 * @param utenti
 * @param n_utenti
 */
void mostra_registrazioni_ordinate(const Utente utenti[], const int n_utenti) {
	int index;
	int scelta_utente;
	int scelta_ord;
	printf("Inserisci nome utente di cui visualizzare le registrazioni: \n");
	do {
		mostra_utenti(utenti, n_utenti);
		scelta_utente = valuta_intero_input();
	} while (scelta_utente < 1 || scelta_utente > n_utenti);
	index = scelta_utente - 1;
	do {
		printf("In che ordine vuoi visualizzare le registrazioni di %s %s?\n",
				utenti[index].nome, utenti[index].cognome);
		printf("1) Ordinate per data\n");
		printf("2) Ordinate per tipologia\n");
		printf("3) Ordinate per data e ora\n");
		printf("Scelta: ");
		scelta_ord = valuta_intero_input();
	} while (scelta_ord < 1 || scelta_ord > 4);

	if (scelta_ord == 1) {
		stampa_ordinate(utenti[index], 1);
	} else if (scelta_ord == 2) {
		stampa_ordinate(utenti[index], 2);
	} else if (scelta_ord == 3) {
		stampa_ordinate(utenti[index], 3);
	}
}
