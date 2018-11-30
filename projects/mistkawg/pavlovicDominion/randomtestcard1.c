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
    int i, testPassed = 0, testFailed = 0, player, players, seed, handSize, deckSize, discardCount;

    printf("\nStarting randomized test for Smithy card.\n");

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

        //get initial values to check if test passed
        handSize = game->handCount[player];
        deckSize = game->deckCount[player];
        discardCount = game->playedCardCount;

        //simulating that the 0 hand position is the smithy card
        cardSmithy(game, 0, player);

        //check discard count after
        if(game->playedCardCount != (discardCount+1)) {
            printf("Test #%d failed: Smithy card not succesfully discarded\n", i+1);
            testFailed++;
        }

        //check hand size after
        else if(game->handCount[player] != (handSize+2)) {
            printf("Test #%d failed: not enough cards drawn to hand\n", i+1);
            testFailed++;
        }

        //check deck size after
        else if(game->deckCount[player] != (deckSize - 3)) {
            printf("Test #%d failed: not enough cards drawn from deck\n", i+1);
            testFailed++;
        }

        //all tests pass
        else {
            printf("Test #%d passed\n", i+1);
            testPassed++;
        }

        free(game);
    }

    printf("Smithy test results: %d tests passed, %d tests failed\n\n", testPassed, testFailed);
}
