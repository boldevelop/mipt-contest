#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>

#include <string.h>

#include "hwf.h"

int half_max = INT_MAX / 2;
int util_min(int left, int right);
int prompt();
int read_int();

int initial_coins;
enum Player initial_turn;

enum Player {
    HUMAN,
    BOT,
};

struct GameState {
    int coins;
    int possible;
    enum Player turn;
};

void init_game_state(struct GameState *gs, int coins, enum Player turn)
{
    gs->coins = coins;
    gs->possible = coins - 1;
    gs->turn = turn;
    initial_coins = gs->coins;
    initial_turn = gs->turn;
}

void is_game_over(struct GameState *gs)
{
    int yes_no, res;
    if (gs->coins == 0) {
        if (gs->turn == BOT) {
            printf("You win!\n");
        } else {
            printf("Bot win! :(\n");
        }

        yes_no = prompt("Restart with same condition?");

        if (yes_no == 0) {
            printf
                ("Type coins number. For example: 17, 23, 1969 or your number\n");
            initial_coins = read_int(2, INT_MAX);

            yes_no = prompt("Will you be the first to take coins?");
            initial_turn = yes_no == 1 ? HUMAN : BOT;
        }

        init_game_state(gs, initial_coins, initial_turn);
    }
}

void print_state(struct GameState *gs)
{
    printf("Total: %d. ", gs->coins);

    if (gs->possible == 1) {
        printf("Possible take: %d\n", gs->possible);
    } else {
        printf("Possible take: 1 - %d\n", gs->possible);
    }
}

void update_state(struct GameState *gs, int taken)
{
    gs->coins -= taken;
    gs->possible = util_min(half_max > taken ? taken * 2 : INT_MAX,
                            gs->coins);
    gs->turn = gs->turn == HUMAN ? BOT : HUMAN;
}

int bot_turn(struct GameState *gs)
{
    int taken = next_turn(gs->coins, gs->possible);
    printf("Bot take <- %d\n", taken);
    return taken;
}

int player_turn(struct GameState *gs)
{
    int taken = read_int(1, gs->possible);
    printf("You take <- %d\n", taken);
    return taken;
}

void game_loop(struct GameState *gs)
{
    int taken;
    for (;;) {
        print_state(gs);

        if (gs->turn == HUMAN) {
            taken = player_turn(gs);
        } else {
            taken = bot_turn(gs);
        }

        update_state(gs, taken);
        is_game_over(gs);
    }
}

void start_game(int coins, int is_player_first)
{
    struct GameState gs = { 0 };
    init_mfib();
    init_game_state(&gs, coins, is_player_first == 1 ? HUMAN : BOT);
    game_loop(&gs);
}

int main()
{
    int coins, is_player_first;

    printf("Fibonacci Nim Game!\n");
    printf
        ("Type coins number. For example: 17, 23, 1969 or your number\n");

    coins = read_int(2, INT_MAX);
    is_player_first = prompt("Will you be the first to take coins?");

    start_game(coins, is_player_first);

    return 0;
}

int util_min(int left, int right)
{
    return left > right ? right : left;
}

void trancuate(char* input, char* trancuated)
{
    while (*input) {
        if (isspace(*input)) {
            input++;
            continue;
        }
        *trancuated = *input;
        trancuated++;
        input++;
    }
}

int is_equal(char* l, char* r) {
    while (*l && *r) {
        if (*l != *r) {
            return 0;
        }
        l++;
        r++;
    }
    return 1;
}

void flush() {
    while (getchar() != '\n');
}

// 2147483648
/* borders [min, max) */
int read_int(int min, int max)
{
    int val, size;
    char input[15];
    char trancuated[15] = {0};
    char *input_end;
    char *s;

    for (;;) {
        for (;;) {
            errno = 0;
            if (fgets(input, sizeof(input), stdin) == NULL) {
                exit(0);
            }

            printf("input %s\n", input);
            val = strtol(input, &input_end, 10);
            printf("val %d\n", val);

            if (errno == ERANGE) {
                printf("Wrong input. Too big number\n");
                continue;
            }

            if (*input_end != '\n') {
                flush();
                printf("Wrong input\n");
                continue;
            }

            if (input_end <= input) {
                printf("LEss\n");
                continue;
            }

            if (*input_end == '\0') {
                printf("Wrong input. Too big number\n");
                continue;
            }

            trancuate(input, trancuated);
            sprintf(input, "%d", val);
            if (is_equal(input, trancuated) == 0) {
                printf("Wrong input. Overflow number\n");
                continue;
            }

            break;
        }

        if (val < min) {
            size = input_end - input;
            printf("Size %d\n", size);

            if (size >= 10) {
                printf("Wrong input. Too big number\n");
            } else {
                printf("Wrong input. Should more than %d\n", min - 1);
            }
            continue;
        }

        if (val > max) {
            printf("Wrong input. Should less than %d\n", max);
            continue;
        }

        break;
    }

    return val;
}

int prompt(const char *question)
{
    int c;
    printf("%s [y/n] ", question);
    do {
        c = getchar();
        if (c == EOF) {
            printf("\n");
            exit(0);
        }
    } while (isspace(c) || (c != 'y' && c != 'n' && c != 'Y' && c != 'N'));
    flush();
    return c == 'y' || c == 'Y';
}
