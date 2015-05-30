#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>

#include "randauto.h"

int main (int argc, char *argv[]) {
	if (4 != argc) {
		printf ("ERROR: Specified %d arguments, requires exactly 3 arguments, ex: `randbytes <bit_cnt> <iter> <outfile/->`\n", argc - 1);
		return 1;
	}

	int range[2];
	int i;
	for (i = 0; 2 > i; ++i) {
		range[i] = atoi (*(argv + i + 1));

		if (EINVAL == errno || ERANGE == errno || 1 > range[i]) {
			printf ("ERROR: Argument %d not a valid integer\n", i);
			return 1;
		}
	}

	if (5U > range[0]) {
		printf ("ERROR: bit_cnt %d less than minimum 5\n", range[0]);
		return 1;
	}

	FILE *out = ('-' == *argv[3]) ? stdout : fopen (argv[3], "w");

	if (NULL == out) {
		printf ("Unable to open file %s for writing\n", argv[3]);
		return 2;
	}

	int bit_cnt = range[0], iter = range[1];

	ra_init_rule (0UL);
	ra_init_auto (bit_cnt);
	ra_seed (time (NULL));

	unsigned char *rand[2];
	int j;
	for (i = 0; iter > i; ++i) {
		ra_step (16);
		rand[0] = ra_bytes ();

		ra_step (16);
		rand[1] = ra_bytes ();

		for (j = 0; bit_cnt / 8 > j; ++j)
			fprintf (out, "%c", rand[0][j] ^ rand[1][j]);

		free (rand[0]);
		free (rand[1]);
	}

	ra_free ();

	if ('-' == *argv[3]) {
		printf ("\n");
	} else {
		fclose (out);
	}

	return 0;
}

