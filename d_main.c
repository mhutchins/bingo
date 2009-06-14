#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

char draw[99];
#define N 100

int get_rnd(int lo, int hi);

void shuffle(unsigned char data[], int len);
int add_to_card(int card, int num);


char cards[6][3][9];
char blanks[18];


int main(int argc, char *argv[])
{
    int card;
    int i;
    long seed = 1;
    int blnk;
	int rowtot, coltot[18];
    int num, row, col;




    while (seed++) {

	if (seed == 4)
	    seed = 1;
	srand48(seed);
	if (seed % 5000 == 0)
	    printf("Seed: %ld\n", seed);

	/* Clear the card */
	memset(cards, 0, sizeof(cards));

	/* Add blanks */
	for (col = 0; col < 9; col++) {
	    blnk = 8;
	    if (col == 0)
		blnk = 9;
	    if (col == 8)
		blnk = 7;
	    memset(blanks, 0, sizeof(blanks));
	    for (i = 0; i < blnk; i++)
		blanks[i] = 100;
		for(i = 0 ; i < sizeof(blanks) ; i++)
			printf("%02x ", blanks[i]);
		printf("\n");
	    shuffle((unsigned char *) blanks, sizeof(blanks)+1);
		for(i = 0 ; i < sizeof(blanks) ; i++)
			printf("%02x ", blanks[i]);
		printf("\n");
	    i = 0;
	    for (card = 0; card < 6; card++)
		for (row = 0; row < 3; row++)
		    cards[card][row][col] = blanks[i++];

	}
	for (card = 0; card < 6; card++) {
	    printf("Card: %d\n", card);
	    for (row = 0; row < 3; row++) {
		for (col = 0; col < 9; col++) {
		    num = cards[card][row][col];
		    switch (num) {
		    case 100:
			printf("XX ");
			break;
		    case 0:
			printf("   ");
			break;
		    default:
			printf("%02d ", num);
			break;
		    }
		}
		printf("\n");
	    }
	}


	/* Add numbers */
	for (num = 1; num < 91; num++) {
	    card = get_rnd(0, 5);
	    i = 0;
	    while (1) {
		if (i++ > 7) {
		    for (card = 0; card < 6; card++) {
			printf("Card: %d\n", card);
			for (row = 0; row < 3; row++) {
			    for (col = 0; col < 9; col++) {
				num = cards[card][row][col];
				switch (num) {
				case 100:
				    printf("XX ");
				    break;
				case 0:
				    printf("   ");
				    break;
				default:
				    printf("%02d ", num);
				    break;
				}
			    }
			    printf("\n");
			}
		    }
		    exit(1);
		}
		if (add_to_card(card, num) == 1)
		    break;
		printf("%d ", card);
		if (++card == 6)
		    card = 0;
	    }

	}

	for (card = 0; card < 6; card++) {
	    printf("Card: %d\n", card);
	    for (row = 0; row < 3; row++) {
		for (col = 0; col < 9; col++) {
		    num = cards[card][row][col];
		    switch (num) {
		    case 100:
			printf("XX ");
			break;
		    case 0:
			printf("   ");
			break;
		    default:
			printf("%02d ", num);
			break;
		    }
		}
		printf("\n");
	    }
	}


    }



    return (0);
}

int add_to_card(int card, int num)
{
    int row, col, ok = 0;

    col = (int) (num / 10);
    if (num == 90)
	col = 8;

    printf("Fitting num %d onto card %d col %d\n", num, card, col);
    for (row = 0; row < 3; row++) {
	if (cards[card][row][col] == 0) {
	    cards[card][row][col] = num;
	    ok = 1;
	    break;
	}
    }

    return (ok);
}

/* Random shuffle */
void shuffle(unsigned char data[], int len)
{
    unsigned char save;
    int i, r;

    for (i = 0; i < len - 1; i++) {
	r = get_rnd(i, len) - i;
	save = data[i];
	data[i] = data[i + r];
	data[i + r] = save;

    }
}

int get_rnd_seeded = 0;
int get_rnd(int lo, int hi)
{
    FILE *fd;
    long seed;
    char *l_ptr;
    unsigned long lrnd;

    double rnd;

    if (get_rnd_seeded == 123) {
	if ((fd = fopen("/dev/random", "rb")) == NULL) {
	    fprintf(stderr, "Failed to open /dev/random\n");
	    exit(1);
	}
	l_ptr = (char *) &seed;
	*(l_ptr++) = fgetc(fd);
	*(l_ptr++) = fgetc(fd);
	*(l_ptr++) = fgetc(fd);
	*(l_ptr) = fgetc(fd);
	srand48(seed);
	get_rnd_seeded = 1;
    }
    rnd = drand48();
    lrnd = rnd * 1000000000;

    rnd = lrnd % ((hi - lo) + 1);
    return (rnd + lo);
}
