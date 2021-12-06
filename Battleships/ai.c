#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "save.h"
#include "player.h"
#include "board.h"

/*
	[x]	The state of the game given to the player A.I. 
	[x]	[state] 

			PRINT 0 

		[state] 
	3.	Then the actions of A.I. should be printed as [playerX] commands.
	[x]	step 2 again
*/

void run_ai(vector_t* v, vector_t* reefs, board_t** board, dim_t* dim,
	player_t** players, int* nextPlayer, int extendedShips, int* seed) {
	
	save_geme_state(v, reefs, board, dim, players, nextPlayer, extendedShips, seed);
	
	printf("%s\n %s\n %s\n", STATE_CHAR, "PRINT 0", STATE_CHAR);

	//do_ai();

	printf("%s\n %s\n %s\n", STATE_CHAR, "PRINT 0", STATE_CHAR);


}