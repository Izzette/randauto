#include <time.h>

void ra_init_rule (unsigned long rule_num);

int ra_init_auto (int bit_cnt);
int ra_free ();

void ra_seed (time_t seed);

void ra_step (unsigned int step);

char *ra_string ();
unsigned char *ra_bytes ();

