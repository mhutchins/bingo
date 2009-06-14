#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <md5.h>
#include <string.h>

char draw[99];
#define N 100

int get_rnd(int lo, int hi);

void shuffle(unsigned char data[], int len);
int add_to_card(int card, int num);


int cards[6][3][9];
struct md5s {
    long seed;
    unsigned char md5_fp[16];
    struct md5s *next;
};

void check_list(struct md5s *head);

int main(int argc, char *argv[])
{
    int card;
    int i;
    struct md5s *md5_list;
    struct md5s *md5_head;
    long seed = 1;
	int num, row, col;

    if ((md5_head = calloc(1, sizeof(struct md5s))) == 0) {
	printf("Out of memory\n");
	exit(1);
    }

    md5_list = md5_head;


    while (seed++) {

	if(seed == 4) seed = 1 ;
	srand48(seed);
	if (seed % 5000 == 0)
	    printf("Seed: %ld\n", seed);

	memset(cards, 0, sizeof(cards));

	for (i = 1; i < 91; i++) {
	do {
	    card = get_rnd(0, 5);
		}
	    while(add_to_card(card, i)==0);

	}
	md5_calc(md5_list->md5_fp, (unsigned char *) cards, sizeof(cards));
	md5_list->seed = seed;

    for (card = 0; card < 6; card++) {
	    printf("Card: %d\n", card);
	    for (row = 0; row < 3; row++) {
		for (col = 0; col < 9; col++) {
		    num = cards[card][row][col];
		    if (num > 0)
			printf("%02d ", num);
		    else
			printf("   ");

		}
		printf("\n");
	    }
	}

	printf("Sig: ");
	for (i = 0; i < 16; i++)
	    printf("%02x", md5_list->md5_fp[i]);
	printf("\n");

	if ((md5_list->next = calloc(1, sizeof(struct md5s))) == 0) {
	    printf("Out of memory\n");
	    exit(1);
	}
	md5_list = md5_list->next;
	check_list(md5_head);
    }



	return(0);
}

void check_list(struct md5s *head)
{
    struct md5s *md5_comp;
    struct md5s *md5_list;
	int i;

    md5_list = head;
    md5_comp = head->next;

    while (1) {
	if (md5_comp->seed == 0)
		break;
	if (memcmp
	    (md5_list->md5_fp, md5_comp->md5_fp,
	     sizeof(md5_list->md5_fp)) == 0) {
	    printf("Duplicate found!\n");
	    printf("Seed: %ld\tSig: ", md5_list->seed);
	    for (i = 0; i < 16; i++)
		printf("%02x", md5_list->md5_fp[i]);
	    printf("\n");
	    printf("Seed: %ld\tComp Sig: ", md5_comp->seed);
	    for (i = 0; i < 16; i++)
		printf("%02x", md5_comp->md5_fp[i]);
	    printf("\n");
	}
//	if (md5_comp->seed % 5000 == 0)
	if (md5_comp->next != 0)
	    md5_comp = md5_comp->next;
	else
	    break;
    }
}

int add_to_card(int card, int num)
{
    int row, col, ok=0;

    col = (int) (num / 10);
    if (num == 90)
	col = 8;

    for (row = 0; row < 3; row++) {
	if (cards[card][row][col] == 0) {
	    cards[card][row][col] = num;
		ok=1;
	    break;
	}
    }

	return(ok);
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
