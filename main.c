#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"

int main(void) {
	table t;

	init_table(&t);
	print_table(&t);

	while (!is_full(&t)) {
		user_input(&t);
		print_table(&t);

	}

	return 0;
}
