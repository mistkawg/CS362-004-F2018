#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "rngs.h"
#include "assert.h"
#include <time.h>

int main() {
    srand(time(NULL));
    struct gameState *game;
    int i, j, testPassed = 0, testFailed = 0, player, players, seed, coinCount, newCoinCount;

    printf("\nStarting randomized test for Adventurer card.\n");

    //test cycle
    for(i = 0; i < 1000; i++) {
        //generate game
        int cards[10] = {adventurer, council_room, cutpurse, embargo, mine,
                         minion, sea_hag, smithy, tribute, village};
        players = rand() % MAX_PLAYERS; //max number of players is 4
        seed = rand();
        game = malloc(sizeof(struct gameState));
        initializeGame(players, cards, seed, game);

        //generate player state
        player = 0; //chooses first player because particular player order is irrelivant
        game->deckCount[player] = rand() % MAX_DECK;
        game->handCount[player] = rand() % MAX_HAND;

        //check to see if deck has coins
        coinCount = 0;
        for (j = 0; j < game->deckCount[player]; j++) {
            if(game->deck[player][j] == copper || game->deck[player][j] == silver
                || game->deck[player][j] == gold)
                coinCount++;
        }

        //if the deck has less than 2 coins, restart the test
        if(coinCount < 1) {
            printf("Test #%d failed: Not enough coins in random deck\n", i+1);
            testFailed++;
            free(game);
            continue;
        }

        //get the number of coins already in active players hand
        coinCount = 0;
        for(j = 0; j < game->handCount[player]; j++) {
            if(game->hand[player][j] == copper || game->hand[player][j] == silver
                || game->hand[player][j] == gold)
                coinCount++;
        }

        //call card
        cardAdventurer(game, player);

        //check if a coin was discarded
        for(j = 0; j < game->discardCount[player]; j++) {
            if(game->discard[player][j] == copper || game->discard[player][j] == silver
                || game->discard[player][j] == gold) {
                    j = -1;
                    break;
                }
        }

        if(j == -1) {
            printf("Test #%d failed: A coin was discarded\n", i+1);
            testFailed++;
            free(game);
            continue;
        }

        //get the number of coins after adventurer card is called in active players hand
        newCoinCount = 0;
        for(j = 0; j < game->handCount[player]; j++) {
            if(game->hand[player][j] == copper || game->hand[player][j] == silver
                || game->hand[player][j] == gold)
                newCoinCount++;
        }

        if(coinCount < newCoinCount) {
            testPassed++;
            printf("Test #%d passed\n", i+1);
        } else {
            testFailed++;
            printf("Test #%d failed: 2 coins were not added to the hand\n", i+1);
        }

        free(game);
    }

    printf("Adventurer test results: %d tests passed, %d tests failed\n\n", testPassed, testFailed);
}
