#include <stdint.h>

#define ROWS 6
#define COLS 7

#define TOTAL (ROWS * COLS)

typedef struct User_table {
	uint64_t u_table;
} user_table;

typedef struct Computer_table {
	uint64_t c_table;
} computer_table;

typedef struct Table {
	uint8_t last_row;
	uint8_t last_col;

	user_table u;
	computer_table c;

} table;

void init_table(table *t);
void print_table(table *t);

int8_t get_free_row(uint64_t table, uint8_t col);
int8_t user_input(table *t);

uint8_t is_full(table *t);

uint8_t win(table *t, uint8_t user);
