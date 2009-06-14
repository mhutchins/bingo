#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

char draw[99];
#define N 100

int get_rnd(int lo, int hi);

void shuffle(unsigned char data[], int len);
int add_to_card(int num);
void print_card();


char cards[18][9];
char blanks[18];


int main(int argc, char *argv[])
{
	char numbers[90];
    int card;
    int i;
    long seed = 1;
    int blnk;
    int oblnk;
    int rowtot, coltot;
    int num, row, col;
    int trow, tcol;
    int cnd;
    int broken = 0;
    int blnktot;




    while (seed++) {
	srand48(seed);
	if (seed % 5000 == 0)
	    fprintf(stderr, "Seed: %ld\n", seed);

	/* Clear the card */
	memset(cards, 0, sizeof(cards));

	/* Add blanks */
	broken = 0;
	blnk = 0;

	while (blnk < 72) {
	    oblnk = blnk;
	    for (col = 0; col < 9; col++) {
		blnktot = 8;
		if (col == 0)
		    blnktot = 9;
		if (col == 8)
		    blnktot = 7;
		memset(blanks, 0, sizeof(blanks));
		cnd = 0;
		coltot = 0;
		for (row = 0; row < 18; row++) {
		    /* Keep a count of blanks encountered */
		    if (cards[row][col])
			coltot = coltot + 1;
		    else {
			/* This is a potential candidate row */
			/* Check this rows population */
			rowtot = 0;
			for (i = 0; i < 9; i++)
			    if (cards[row][i])
				rowtot++;
			if (rowtot < 4) {
			    /* Candidate row found */
			    blanks[cnd++] = row;
			}
		    }
		}
		if (coltot < blnktot && cnd > 0) {	/* We found candiates for this column */
		    /* Take a random one */
		    row = blanks[get_rnd(0, cnd - 1)];
		    if (cards[row][col] == 100) {
			sprintf(stderr, "Yikes.Seed %ld,  Candidate already taken!\n", seed);
			broken = 1;
			break;
		    }
		    cards[row][col] = 100;
		    blnk++;
		}
	    }
	    if (oblnk == blnk) {
		printf("Help! Spinning on seed %ld\n", seed);
		print_card();
		broken = 1;
		break;
	    }
	}

	if (broken == 0) {
		for(i=0;i<90;i++)
			numbers[i]=i+1;
		shuffle(numbers, 89);
	    /* Add numbers */
	    for (num = 1; num < 91; num++) {
		    if (add_to_card(numbers[num]) == 0)
			{
				printf("Ouch! Could not add %d to card!\n", numbers[num]);
				break;
			}
		}

	    print_card();

	}
    }



    return (0);
}

int add_to_card(int num)
{
    int row, col, ok = 0;

    col = (int) (num / 10);
    if (num == 90)
	col = 8;

    for (row = 0; row < 18; row++) {
	if (cards[row][col] == 0) {
	    cards[row][col] = num;
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

void print_card()
{
    int row, col, num;

    for (row = 0; row < 18; row++) {
	if (row % 3 == 0) printf("Ticket %d:\n", (row/3));
	for (col = 0; col < 9; col++) {
	    num = cards[row][col];
	    switch (num) {
	    case 100:
		printf("   ");
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
    printf("\n");
}
