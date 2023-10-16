#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#include "hwf.h"

int half_max = INT_MAX / 2;
int util_min(int left, int right);
int prompt();

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

        printf("Restart with same condition?\n");
        yes_no = prompt();

        if (yes_no == 1) {
            printf
                ("Type coins number. For example: 17, 23, 1969 or your number\n");
            res = scanf("%d", &initial_coins);
            if (res != 1) {
                abort();
            }

            printf("Will you be the first to take coins?\n");
            yes_no = prompt();
            // res = scanf("%c", &yes_no);
            // if (res != 1) {
            //     abort();
            // }
            initial_turn = yes_no == 1 ? HUMAN : BOT;
        }

        init_game_state(gs, initial_coins, initial_turn);
    }
}

void print_state(struct GameState *gs)
{
    printf("Total: %d\n", gs->coins);
    printf("Possible take: 1 - %d\n", gs->possible);
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
    printf("Take %d\n\n", taken);
    return taken;
}

int player_turn(struct GameState *gs)
{
    int taken;
    scanf("%d", &taken);
    printf("Take %d\n\n", taken);
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
    int coins, res;
    int yes_no;

    printf("Fibonacci Nim Game!\n");
    printf
        ("Type coins number. For example: 17, 23, 1969 or your number\n");

    res = scanf("%d", &coins);
    if (res != 1) {
        abort();
    }

    printf("Will you be the first to take coins?\n");
    yes_no = prompt();
    // res = scanf("%c", &yes_no);
    // if (res != 1) {
    //     abort();
    // }

    start_game(coins, yes_no);

    return 0;
}

int util_min(int left, int right)
{
    return left > right ? right : left;
}

int prompt()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF && c != ' ');
    return c == 'y';
}
