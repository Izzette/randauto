#include <stdlib.h>
#include <time.h>

#include <gmp.h>

#include "randauto.h"

#define __RA_RULE_DEFAULT 1436965290UL
#define __RA_STEP_DEFAULT 4U
#define __RA_LEN_RULE 32U
#define __RA_BITS_RULE 0x1FU

unsigned char __ra_Rule[__RA_LEN_RULE];

int __ra_Len_Automaton;
unsigned char *__ra_Automaton;

void ra_init_rule (unsigned long rule_num) {
	if (0UL == rule_num) rule_num = __RA_RULE_DEFAULT;
	
	unsigned int i;
	for (i = 0U; __RA_LEN_RULE > i; ++i) {
		__ra_Rule[i] = (unsigned char)(rule_num & 0x1UL);
		rule_num >>= 1;
	}
}

int ra_init_auto (int bit_cnt) {
	if (0 >= bit_cnt) return 1;

	__ra_Len_Automaton = bit_cnt;

	if (NULL == __ra_Automaton) {
		__ra_Automaton = malloc (bit_cnt * sizeof (unsigned char));
	} else {
		__ra_Automaton = realloc (__ra_Automaton, bit_cnt * sizeof (unsigned char));
	}

	return 0;
}

int ra_free () {
	if (NULL == __ra_Automaton) return 1;

	free (__ra_Automaton);
	__ra_Automaton = NULL;

	return 0;
}

void ra_seed (time_t seed) {
	srand48 (seed);
	
	long working = 0;
	
	int i;
	for (i = 0; __ra_Len_Automaton > i; ++i) {
		if (! (i % 31)) working = lrand48 ();

		__ra_Automaton[i] = (unsigned char)(working & 0x1L);

		working >>= 1;
	}
}

void ra_step (unsigned int step) {
	if (0U == step) step = __RA_STEP_DEFAULT;

	unsigned char nh = 0;
	unsigned char next[__ra_Len_Automaton];

	int i, j, top;
	for (; step; --step) {
		for (i = 0; __ra_Len_Automaton > i; ++i) {
			top = i + 3;
			for (j = i - 2; top > j; ++j) {
				nh <<= 1;
				nh |= __ra_Automaton[j % __ra_Len_Automaton];
			}

			next[i] = __ra_Rule[nh & __RA_BITS_RULE];
		}
		
		for (i = 0; __ra_Len_Automaton > i; ++i) __ra_Automaton[i] = next[i];
	}
}

char *ra_string () {
	char *srep;

	int mem_len = 1 + __ra_Len_Automaton * sizeof (char);

	srep= malloc (mem_len);

	int i;
	for (i = 0; __ra_Len_Automaton > i; ++i) {
		srep[i] = (__ra_Automaton[i]) ? '1' : '0';
	}

	srep[mem_len - 1] = '\0';

	return srep;
}

unsigned char *ra_bytes () {
	unsigned char *brep;

	int mem_len = 1 + ((__ra_Len_Automaton * sizeof (char)) / 8);

	brep = malloc (mem_len);

	int i, j;
	for (i = 0; mem_len > i; ++i) {
		for (j = 0; 8 > j; ++j) {
			brep[i] <<= 1;
			brep[i] |= __ra_Automaton [8 * i + j];
		}
	}

	brep[mem_len - 1] = '\0';

	return brep;
}

void ra_mpz (mpz_t mrep) {
	mpz_t tmp;
	mpz_init (tmp);

	int i;
	for (i = 0; __ra_Len_Automaton > i; ++i) {
		mpz_add_ui (tmp, mrep, (unsigned long)__ra_Automaton[i]);
		mpz_swap (mrep, tmp);
	}

	mpz_clear (tmp);
}
 
