#include <stdio.h>

#include "game.h"

uint64_t cols_separator = 0;

void init_table(table *t){
	/* 
	 * setto tutte le variabili a zero 
	*/

	t->u.u_table = 0;
	t->c.c_table = 0;

	t->last_row = 0;
	t->last_col = 0;

	for (int8_t i = 0; i < COLS; i++) {
		cols_separator |= (1ULL << ((i * ROWS) + (ROWS - 1)));
	}
}

void print_table(table *t){

	printf("\n\n");
	for (int8_t row = ROWS - 1; row >= 0; row--) {

		if (row != 0 && (row + 1) % (ROWS) == 0) continue;

        for (int8_t col = 0; col < COLS; col++) {


            uint8_t idx = row + (ROWS * col);
			uint64_t mask = (uint64_t) 1 << idx;

			/*
			 * 0 0 0 0 0 0 0 1 -> 0 0 0 0 1 0 0 0 
			 * viene shiftato 1 di idx posizioni 
			*/

			if (t->u.u_table & mask) printf("O ");
			else if (t->c.c_table & mask) printf("X ");
			else printf(". ");

			/* 
			 * viene applicato un mask ad u_table 
			 * e c_table per capire se la casella 
			 * e' occupata dall'utente o dal 
			 * computer
			*/
		}
		printf("\n");
	}

	printf("\n");
	for (uint8_t i = 0; i < COLS; i++) printf("%d ", i);
	printf("\n");
}

uint8_t is_full(table *t){
	/* 
	 * faccio un OR tra le due tabelle (utente e computer) 
	 * per vedere se e' ci sono mosse possibili o 
	 * la tabella e' piena 
	*/

	uint64_t mask = t->u.u_table | t->c.c_table;

	if (mask == (((1ULL << TOTAL) - 1) ^ cols_separator)) return 1;

	/* corrisponde a fare 
	 * 0 0 0 0 0 0 0 1 -> 
	 * 1 0 0 0 0 0 0 0 ->
	 * 0 1 1 1 1 1 1 1 
	 *
	 * verifico se OR tra le due tabelle sia uguale
	 * ad una tabella con tutti 1
	*/
	return 0;
}

int8_t get_free_row(uint64_t table, uint8_t col){
	/* 
	 * ottengo la prima colonna libera della tabella
	*/


	for (int8_t row = 0; row < ROWS - 1; row++) {
//		uint8_t idx = (row * COLS) + col;
		uint8_t idx = (col * ROWS) + row;

		uint64_t mask = (uint64_t) 1 << idx;

		/* 
		 * 0 0 0 0 0 0 0 1 ->
		 * 1 0 0 0 0 0 0 0 
		 * 
		 * faccio un AND con la tabella e 
		 * vedo se ritorna 0 (ossia la casella 
		 * non e' occupata) ritorno la riga
		 * corrente che e' la prima vuota 
		 *
		 * 0 0 0 1 &
		 * 0 1 0 1 ->
		 * 0 0 0 1 == casella occupata
		 *
		 * 0 0 0 1 & 
		 * 0 1 1 0 ->
		 * 0 0 0 0 == casella vuota
		*/

		if ((mask & table) == 0) return row;
	}
	return -1;
}

int8_t user_input(table *t){
	/* prendo l'input dell'utente
	 * facendo un OR delle due tabelle
	 * (utente e computer) 
	*/

	int input_col = -1;
	int8_t col = -1;
	int8_t row = -1;

	uint64_t mask = t->u.u_table | t->c.c_table;

	while (1){
		printf("insert number (1-7) : ");

		if (scanf("%d", &input_col) != 1) {
            while (getchar() != '\n');
            continue;
        }

		col = (int8_t)input_col;

		/*
		 * prendo la colonna come input
		 * e la passo alla funzione get_free_row
		 * per ottente la prima riga vuota
		*/

		if (col >= 1 && col <= COLS) {
			col--;
			row = get_free_row(mask, col);

			if (row == -1) printf("full col\n");
			else break;
		}
	}

	if (col != -1 && row != -1) {
		t->u.u_table |= (uint64_t) 1 << ((col * ROWS) + row);

		/* 
		 * occupo la casella con un OR 
		 *
		 * 0 0 0 0 0 0 0 1 -> shiftato 
		 * 1 0 0 0 0 0 0 0 
		 *
		 * 1 0 0 0 0 0 0 0 |
		 * 0 1 0 0 1 1 1 0 ->
		 * 1 1 0 0 1 1 1 0
	   */


		t->last_col = col;
		t->last_row = row;
	}
	else return -1;

	return 0;
}

uint8_t win(table *t, uint8_t user){
	/* 
	 * controllo a win di un giocatore
	 * (utente o computer) 
	*/

	uint64_t t_table = (user == 'u') ? t->u.u_table : t->c.c_table;

	/* seleziono la tabella in base
	 * al carattere passato in user
	*/

	

	/* controllo orizzontale 
	 * shiftare di due verso sinistra e applicare un AND
	 * con la tabella originale
	*/

	uint64_t mask_h = (t_table) & ((t_table) << ROWS);
	mask_h &= ((mask_h) << (2 * ROWS));

	if ((mask_h) != 0) return 1;

	uint64_t mask_v = (t_table) & ((t_table) << 1);
	mask_v &= (mask_v << 2);

	if ((mask_v) != 0) return 1;

	uint64_t mask_d1 = (t_table) & ((t_table) << (ROWS + 1));
	mask_d1 &= (mask_d1 << (2 * (ROWS + 1)));

	if (mask_d1 != 0) return 1;

	uint64_t mask_d2 = (t_table) & ((t_table) << (ROWS - 1));
	mask_d2 &= (mask_d2 << (2 * (ROWS - 1)));

	if (mask_d2 != 0) return 1;

	return 0;
}
