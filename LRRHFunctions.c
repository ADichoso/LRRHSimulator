/*
* Programmed By: Aaron Gabrielle Dichoso 
*
* Description:
* 	LRRHFunctions.c: This file contains the behavior that LRRH has for sensing, rotation, movement, and intelligence
*
* Last Modified: 06/02/2022
* Acknowledgements: 
*       Pointers and 2D arrays - https://overiq.com/c-programming-101/pointers-and-2-d-arrays/
*       getchar() function - https://stackoverflow.com/questions/18801483/press-any-key-to-continue-function-in-c/18801616
*       Examples of Pathfinding Algorithms - http://theory.stanford.edu/~amitp/GameProgramming/AStarComparison.html
*       Breadth First Search Algorithm - https://www.youtube.com/watch?v=oDqjPvD54Ss
*       Breadth First Search Algorithm on a grid - https://www.youtube.com/watch?v=KiCBXu4P-2Y
*       A* Pathfinding algorithm - https://www.youtube.com/watch?v=-L-WgKMFuhE
*       <limits.h> library - https://www.tutorialspoint.com/c_standard_library/limits_h.htm
*/

#include "QueueSystem.c"
#include "GUIFunctions.c"
#include "LRRHFunctions_prototype.h"

#define STANDARD_MODE_MIN_GRID_SIZE 8
#define STANDARD_MODE_MAX_GRID_SIZE 15
#define JUMBO_MODE_MAX_GRID_SIZE 25

#define STANDARD_MODE_MAX_ELEMENTS 3 
#define JUMBO_MODE_MAX_ELEMENTS 9

#define UNEXPLORED_TILE_VALUE 0
#define UNWALKABLE_TILE_VALUE -1
#define WALKABLE_TILE_VALUE 1
#define GRANDMA_TILE_VALUE 5
#define WOLF_TILE_VALUE 10
#define BAKERY_TILE_VALUE 9
#define WOODSMAN_TILE_VALUE 3
#define FLOWER_TILE_VALUE 2

/* This function converts the current direction of LRRH into its X value  (1 if right, -1 if left, 0 if none)
        @param   (char)  cCurrentDirection is the current tile in front of LRRH
        
        East gives 1
        West gives -1
        North and South gives 0
        
        @return  (int) returns an integer corresponding to the direction
*/
int 
convertDirectionSymbolToXMovement (char cCurrentDirection)
{
	switch (cCurrentDirection){
	case DIRECTION_EAST:
		return 1;
	case DIRECTION_WEST:
		return -1;
	default:
		return 0;
	}
}

/* This function converts the current direction of LRRH into its Y value  (1 if down, -1 if up, 0 if none)
        @param   (char)  cCurrentDirection is the current tile in front of LRRH
        
        South gives 1
        North gives -1
        East and West gives 0
        
        @return  (int) returns an integer corresponding to the direction
*/
int 
convertDirectionSymbolToYMovement (char cCurrentDirection)
{
	switch (cCurrentDirection){
	case DIRECTION_SOUTH:
		return 1;
	case DIRECTION_NORTH:
		return -1;
	default:
		return 0;
	}
}

/* This gives the opposite direction of the given direction
        @param   (char)  cSymbol is the associated symbol with the direction
        
        @return  (char) The opposite direction
*/
char 
getOppositeDirection (char cDirection)
{
	switch (cDirection){
	case DIRECTION_NORTH:
		return DIRECTION_SOUTH;
		break;
	case DIRECTION_EAST:
		return DIRECTION_WEST;
		break;
	case DIRECTION_SOUTH:
		return DIRECTION_NORTH;
		break;
	case DIRECTION_WEST:
		return DIRECTION_EAST;
		break;
	default:
		return cDirection;
		break;
	}
}

/* This gives the symbol associated with the direction given the x and y directions

        @param   (int)  xMovement is the x component of the direction
        @param   (int)  yMovement is the y component of the direction
        
        @return  (char) The character associated with the components
*/
char 
directionVectorToSymbol (int xMovement, int yMovement)
{
	if (xMovement == 1 && yMovement == 0) 
                return DIRECTION_EAST;		
	else if (xMovement == -1 && yMovement == 0) 
                return DIRECTION_WEST;		
	else if (xMovement == 0 && yMovement == 1) 
                return DIRECTION_SOUTH;		
        else if (xMovement == 0 && yMovement == -1) 
                return DIRECTION_NORTH;		
	else 
                return EMPTY_TILE;

}

/* This computes the distance between the woodsman and grandmother's house

        @param   (char *)  pPlayGrid is the pointer to carrPlayGrid 

        @param   (int)  nGridSize is the grid size of the play grid
        
        @return  (int) The distance between the woodsman and grandmother's house
*/
int 
computeWoodsmanDistance (char * pPlayGrid, int nGridSize)
{
        //Woodsman and Granny Coordinates
	int nWoodsmanXCoord, nWoodsmanYCoord;
	int nGrannyXCoord, nGrannyYCoord;
	
        //Get the location of the granny and the woodsman from the play grid
        //Do not use the sensed tiles array, since LRRH might meet the woodsman first before granny

        int i, j;
	//Loop through the sensed tiles array
	for (i = 0; i < nGridSize; i++)
        {
		for (j = 0; j < nGridSize; j++)
                {		
			if ( *(pPlayGrid + i * nGridSize + j) == WOLF_TILE)
                        {
                                nWoodsmanXCoord = i;
                                nWoodsmanYCoord = j;
                        } else if ( *(pPlayGrid + i * nGridSize + j) == GRANDMA_TILE)
                        {
                                nGrannyXCoord = i;
                                nGrannyYCoord = j;
                        }			
		}
	}
        //Compute the distance between the woodsman and granny
	int nWoodsmanDistance = getDistance (nWoodsmanXCoord, nWoodsmanYCoord, nGrannyXCoord, nGrannyYCoord);
	
	return nWoodsmanDistance;
}


/* This function rotates the viewing direction of LRRH
        @param   (int *)  pNumOfMovements is the pointer to nNumOfMovements
        @param   (int *)  pLRRHXLocation is the pointer to nLRRHXLocation
        @param   (int *)  pLRRHYLocation is the pointer to nLRRHYLocation
        @param   (char *)  pLRRHTile is the pointer to LRRH's current location in carrPlayGrid
        @param   (char *)  pCurrentDirection is the pointer to LRRH's current viewing direction
        @param   (int)  nGridSize is the size of the playing grid
        
        @return  (char) The value on the tile LRRH will move into
*/
char 
forwardLRRH (int * pNumOfMovements,
                int * pLRRHXLocation, 
                int * pLRRHYLocation, 
                char * pLRRHTile, 
                char * pCurrentDirection, 
                int nGridSize, 
                char cPreviousTileSymbol)
{
	//Convert the character Direction to X and Y components
	int nXDirection = convertDirectionSymbolToXMovement (*pCurrentDirection);
	int nYDirection = convertDirectionSymbolToYMovement (*pCurrentDirection);
	
        //Get the symbol on the tile which LRRH is about to move to
	char cMovedTileElement = * (pLRRHTile + nXDirection * nGridSize + nYDirection);
	
        //Get the current symbol of LRRH
        char cLRRHSymbol = *pLRRHTile;
	
	//Move LRRH into the new tile
	* (pLRRHTile + nXDirection * nGridSize + nYDirection) = cLRRHSymbol;
	
	//Return the original tile to empty tile
	*pLRRHTile = cPreviousTileSymbol;
	
        //Update LRRH's location
	*pLRRHXLocation += nXDirection;
	*pLRRHYLocation += nYDirection;
	
        //Increment her movements
        *pNumOfMovements += 1;	
	
        //Show the movement message
	displayMovementMessage (cMovedTileElement);
        
        //Return the previous value the tile which LRRH has moved to. Value will be saved to remember the tile's value
	return cMovedTileElement;
}


/* This function simulates a right rotation. LRRH does not physically turn, rather, she does the rotation in her mind.
        @param   (char)  cTestDirection is the test value for the right rotation simulation
        
        @return  (char) the value of the simulated right rotation
*/
char 
simulateRightLRRHRotation (char cTestDirection)
{
	switch (cTestDirection){
	case DIRECTION_NORTH:
		return DIRECTION_EAST;
	case DIRECTION_EAST:
		return DIRECTION_SOUTH;
	case DIRECTION_SOUTH:
		return DIRECTION_WEST;
	case DIRECTION_WEST:
		return DIRECTION_NORTH;
	}

        return 'x';
}

/* This function simulates a left rotation. LRRH does not physically turn, rather, she does the rotation in her mind.
        @param   (char)  cTestDirection is the test value for the left rotation simulation
        
        @return  (char) the value of the simulated left rotation
*/
char 
simulateLeftLRRHRotation (char cTestDirection)
{
	switch (cTestDirection){
	case DIRECTION_NORTH:
		return DIRECTION_WEST;
	case DIRECTION_EAST:
		return  DIRECTION_SOUTH;
	case DIRECTION_SOUTH:
		return  DIRECTION_EAST;
	case DIRECTION_WEST:
		return  DIRECTION_NORTH;
	}

        return 'x';
}

/* This function is the main method of rotation by LRRH. 
        @param   (int *)  pNumOfSenses is the pointer to nNumOfSenses
        @param   (int *)  pNumOfRotations is the pointer to nNumOfRotations
        @param   (int *)  pNumOfMovements is the pointer to nNumOfMovements
        @param   (char *) pCurrentDirection is the pointer to LRRH's current viewing direction
        @param   (char)  cNewDirection is the target direction that LRRH is aiming for
        @param   (char *)  pLRRHTile is the pointer to LRRH's current location in carrPlayGrid
        @param   (char *)  pPlayGrid is the pointer to carrPlayGrid
        @param   (int)  nGridSize is the size of the playing grid
        @param   (int)  nStepByStepMode tells whether or not LRRH will move automatically
        @param   (int)  nHasSounds tells whether or not to play sound
        
        @return  (void) no return value
*/
void 
smartRotateLRRH (int * pNumOfSenses, 
                int * pNumOfRotations, 
                int * pNumOfMovements,
                int * pNumOfTurns, 
                char * pCurrentDirection, 
                char cNewDirection,
                char * pLRRHTile,
                char * pPlayGrid, 
                int nGridSize, 
                int nStepByStepMode, 
                int nHasSounds)
 {
	
	//Simulate a left rotation
	char cTestLeftRotation = simulateLeftLRRHRotation (*pCurrentDirection);
	
        //While LRRH's current direction is not the target direction.
	while (*pCurrentDirection != cNewDirection)
	{
                //Test if the simulated left direction is equal to the target direction.
                //If so, rotate to the left. Else, rotate to the right.

                /*There are only 3 cases that can happen by doing this method
                        1. The simulated left rotation is the target, LRRH only rotates once.
                        2. The right rotation brings LRRH to the target direction, LRRH only rotates once.
                        3. The target direction is directly opposite of LRRH, whether or not she rotates right or left,
                           2 rotations is required.
                */

		if (cTestLeftRotation == cNewDirection)
                        rotateLeftLRRH (pNumOfRotations, pCurrentDirection, pLRRHTile);
                else
                        rotateRightLRRH (pNumOfRotations, pCurrentDirection, pLRRHTile);
		
                //Display the play grid and wait for the next turn after every rotation
		displayPlayGrid (pPlayGrid, nGridSize, *pNumOfSenses, *pNumOfRotations, *pNumOfMovements);
		waitForNextTurn (pNumOfTurns, nStepByStepMode, nHasSounds);
	}
}


/* This function rotates the viewing direction of LRRH to the right
        @param   (int *)  pNumOfMovements is the pointer to nNumOfMovements
        @param   (char *)  cCurrentDirection is the current viewing direction of LRRH
        @param   (char *)  pLRRHTile is the pointer to LRRH's current location in carrPlayGrid

        rotation occurs clockwise
        
        North -> East
        East -> South
        South -> West
        West -> North
        
        
        @return  (void) no return value
*/
void 
rotateRightLRRH (int * pNumOfRotations, char * pCurrentDirection, char * pLRRHTile)
{
	
	
	char cChosenSymbol;
	//The Little Red Riding Hood's rotation is determined by a character.  (N - North, E - East, S - South, W - West)
        //Change LRRH's icon to face the new direction
	//Rotation will happen CLOCKWISE
	*pNumOfRotations += 1;
	switch (*pCurrentDirection){
	case DIRECTION_NORTH:
		*pCurrentDirection = DIRECTION_EAST;
		cChosenSymbol = LRRH_DIRECTION_EAST;
		break;
	case DIRECTION_EAST:
		*pCurrentDirection =  DIRECTION_SOUTH;
		cChosenSymbol = LRRH_DIRECTION_SOUTH;
		break;
	case DIRECTION_SOUTH:
		*pCurrentDirection =  DIRECTION_WEST;
		cChosenSymbol = LRRH_DIRECTION_WEST;
		break;
	case DIRECTION_WEST:
		*pCurrentDirection =  DIRECTION_NORTH;
		cChosenSymbol = LRRH_DIRECTION_NORTH;
		break;
	}
	*pLRRHTile = cChosenSymbol;
	
	displayRightRotationMessage (*pCurrentDirection);
}

/* This function rotates the viewing direction of LRRH to the left
        @param   (int *)  pNumOfMovements is the pointer to nNumOfMovements
        @param   (char *)  cCurrentDirection is the current viewing direction of LRRH
        @param   (char *)  pLRRHTile is the pointer to LRRH's current location in carrPlayGrid

        rotation occurs counter-clockwise
        
        North -> West
        West -> South
        South -> East
        East -> North
        
        
        @return  (void) no return value
*/
void 
rotateLeftLRRH (int * pNumOfRotations, char * pCurrentDirection, char * pLRRHTile)
{
	
	char cChosenSymbol;
	//The Little Red Riding Hood's rotation is determined by a character.  (N - North, E - East, S - South, W - West)
        //Change LRRH's icon to face the new direction
	//Rotation will happen COUNTER-CLOCKWISE
	*pNumOfRotations += 1;
	switch (*pCurrentDirection){
	case DIRECTION_NORTH:
		*pCurrentDirection = DIRECTION_WEST;
		cChosenSymbol = LRRH_DIRECTION_WEST;
		break;
	case DIRECTION_EAST:
		*pCurrentDirection =  DIRECTION_SOUTH;
		cChosenSymbol = LRRH_DIRECTION_SOUTH;
		break;
	case DIRECTION_SOUTH:
		*pCurrentDirection =  DIRECTION_EAST;
		cChosenSymbol = LRRH_DIRECTION_EAST;
		break;
	case DIRECTION_WEST:
		*pCurrentDirection =  DIRECTION_NORTH;
		cChosenSymbol = LRRH_DIRECTION_NORTH;
		break;
	}
	*pLRRHTile = cChosenSymbol;
	
	//Print the current rotation and number of rotations
	displayLeftRotationMessage (*pCurrentDirection);
}


/* This function allows LRRH to sense the tile in front of her
        @param   (int *)  pNumOfSenses is the pointer to nNumOfSenses
        @param   (int *)  pLRRHXLocation is the pointer to nLRRHXLocation
        @param   (int *)  pLRRHYLocation is the pointer to nLRRHYLocation
        @param   (char *)  pLRRHTile is the pointer to LRRH's location in carrPlayGrid
        @param   (char *)  pCurrentDirection is the pointer to cCurrentDirection
        @param   (int)  nGridSize is the size of the playing grid
        
        
        @return  (char) returns the sense LRRH gets
        
        Different cases can happen depending on the tile:
        
        Pit - LRRH feels a breeze  (PIT_SENSE_TILE)
        Wolf - LRRH hears a howl  (WOLF_SENSE_TILE)
        Flower - LRRH smells a nice scent  (FLOWER_SENSE_TILE)
        Bakery - LRRH feels a warm heat  (BAKERY_SENSE_TILE)
        Woodsman / Lumberjack - LRRH hears the sound of the chopping of wood  (WOODSMAN_SENSE_TILE)
        Granny - LRRH sees the light in Granny's house  (GRANDMA_SENSE_TILE)
        Nothing - nothing.  (EMPTY_SENSE_TILE)
*/
char 
senseLRRH (int * pNumOfSenses, 
                int * pLRRHXLocation, 
                int * pLRRHYLocation, 
                char * pLRRHTile, 
                char * pCurrentDirection, 
                int nGridSize)
{
	
        //Get the X and Y components of the direction LRRH will sense into.
	int nXSense = convertDirectionSymbolToXMovement (*pCurrentDirection);
	int nYSense = convertDirectionSymbolToYMovement (*pCurrentDirection);
	
        //Increment the number of senses
	*pNumOfSenses += 1;
	
	//Check what is currently on the sensed tile
        char cSensedTile = * (pLRRHTile + nXSense * nGridSize + nYSense);
		
        //Display the sense message
	displaySenseMessage (cSensedTile);
	
        //Return the appropriate sense value
	switch (cSensedTile){
	case PIT_TILE:
		return PIT_SENSE_TILE;
                break;
	case WOLF_TILE:
		return WOLF_SENSE_TILE;
                break;
	case FLOWER_TILE:
		return FLOWER_SENSE_TILE;
                break;
	case BAKERY_TILE:
		return BAKERY_SENSE_TILE;
                break;
	case WOODSMAN_TILE:
		return WOODSMAN_SENSE_TILE;
                break;
	case GRANDMA_TILE:
		return GRANDMA_SENSE_TILE;
                break;
	default:
		return EMPTY_SENSE_TILE;
                break;
	}
}

/* This function allows LRRH to sense the four adjacent tiles (UP, DOWN, LEFT, RIGHT) around her and determine whether or not
   She can explore those tiles later on.
        @param   (int *)  pLRRHXLocation is the pointer to nLRRHXLocation (LRRH's X Location)
        @param   (int *)  pLRRHYLocation is the pointer to nLRRHYLocation (LRRH's Y Location)

        @param   (int *)  pWolfXCoord is the pointer to nWolfXCoord (Wolf's X Location)
        @param   (int *)  pWolfYCoord is the pointer to nWolfYCoord (Wolf's Y Location)

        @param   (int *)  pSensedTiles is the pointer to the narrSensedTiles Array 
                (Contains information on which tiles LRRH has already sensed)

        @param   (char *)  pLRRHViewDirection is the pointer to cLRRHviewDirection (LRRH's viewing direction)

        @param   (int)  nGridSize is the size of the playing grid

        @param   (int *)  pNumOfSenses is the pointer to nNumOfSenses (Keeps track of LRRH's sense count)
        @param   (int *)  pNumOfRotations is the pointer to nNumOfRotations (Keeps track of LRRH's rotation count)
        @param   (int *)  pNumOfMovements is the pointer to nNumOfMovements (Keeps track of LRRH's movement count)
        @param   (int *)  pNumOfTurns is the pointer to nNumOfTurns (Keeps track of LRRH's turn count)

        @param   (int *)  pIsGrannyApproachable is the pointer to nIsGrannyApproachable 
                (Tells whether or not LRRH has all objectives already complete to go to granny's house)
                
        @param   (int *)  pHasBread is the pointer to nHasBread 
                (Tells whether or not LRRH already has went to a bakery and the bread has not been eaten yet)

        @param   (int *)  pHasSensedBread is the pointer to nHasSensedBread 
                (Tells whether or not LRRH has already spotted a bakery)
        @param   (int *)  pHasSensedFlower is the pointer to nHasSensedFlower 
                (Tells whether or not LRRH has already spotted a flower field)
        @param   (int *)  pHasSensedWoodsman is the pointer to nHasSensedWoodsman 
                (Tells whether or not LRRH has already spotted the woodsman's house)
        @param   (int *)  pHasSensedGrandma is the pointer to nHasSensedGrandma 
                (Tells whether or not LRRH has already spotted granny's house)

        @param   (Queue *)  pXGridQueue is the pointer to XGridQueue
                (The queue which stores the X coordinates of the locations to explore later on)
        @param   (Queue *)  pYGridQueue is the pointer to YGridQueue 
                (The queue which stores the Y coordinates of the locations to explore later on)

        @param   (int)  nStepByStepMode tells whether or not to wait for user input until the next turn
        @param   (int)  nHasSounds tells whether or not the application should play sound for each turn

        @return  (void) no return value
*/
void 
senseAndQueueLRRHAdjacentTiles (int * pLRRHXLocation, 
                        int * pLRRHYLocation,
                        int * pWolfXCoord, 
                        int * pWolfYCoord,
                        int * pSensedTiles, 
                        char * pPlayGrid, 
                        char * pLRRHViewDirection, 
                        int nGridSize,
                        int * pNumOfSenses, 
                        int * pNumOfRotations, 
                        int * pNumOfMovements, 
                        int * pNumOfTurns,
                        int * pIsGrannyApproachable, 
                        int * pHasBread, 
                        int * pHasSensedBread, 
                        int * pHasSensedFlower, 
                        int * pHasSensedWoodsman, 
                        int * pHasSensedGrandma,
                        Queue * pXGridQueue, 
                        Queue * pYGridQueue, 
                        int nStepByStepMode, 
                        int nHasSounds)
{
	
        //Sense the tiles in each direction around LRRH
	char carrDirectionSymbols[4] = {DIRECTION_NORTH, DIRECTION_EAST, DIRECTION_WEST, DIRECTION_SOUTH};
	int i;
	
	for (i = 0; i < 4; i++){
		senseAndQueueLRRHForwardTile (pLRRHXLocation, 
                                                pLRRHYLocation,
                                                pWolfXCoord, 
                                                pWolfYCoord, 
                                                pSensedTiles,
                                                pPlayGrid, 
                                                pLRRHViewDirection, 
                                                nGridSize,
                                                pNumOfSenses, 
                                                pNumOfRotations, 
                                                pNumOfMovements, 
                                                pNumOfTurns,
                                                pIsGrannyApproachable, 
                                                pHasBread, 
                                                pHasSensedBread, 
                                                pHasSensedFlower, 
                                                pHasSensedWoodsman, 
                                                pHasSensedGrandma,
                                                pXGridQueue, 
                                                pYGridQueue, 
                                                nStepByStepMode, 
                                                nHasSounds, 
                                                carrDirectionSymbols[i]);
	}
}

/* This function allows LRRH to sense the tile in front of her and determine whether or not
   She can explore those tiles later on.
        @param   (int *)  pLRRHXLocation is the pointer to nLRRHXLocation (LRRH's X Location)
        @param   (int *)  pLRRHYLocation is the pointer to nLRRHYLocation (LRRH's Y Location)

        @param   (int *)  pWolfXCoord is the pointer to nWolfXCoord (Wolf's X Location)
        @param   (int *)  pWolfYCoord is the pointer to nWolfYCoord (Wolf's Y Location)

        @param   (int *)  pSensedTiles is the pointer to the narrSensedTiles Array 
                (Contains information on which tiles LRRH has already sensed)

        @param   (char *)  pLRRHViewDirection is the pointer to cLRRHviewDirection (LRRH's viewing direction)

        @param   (int)  nGridSize is the size of the playing grid

        @param   (int *)  pNumOfSenses is the pointer to nNumOfSenses (Keeps track of LRRH's sense count)
        @param   (int *)  pNumOfRotations is the pointer to nNumOfRotations (Keeps track of LRRH's rotation count)
        @param   (int *)  pNumOfMovements is the pointer to nNumOfMovements (Keeps track of LRRH's movement count)
        @param   (int *)  pNumOfTurns is the pointer to nNumOfTurns (Keeps track of LRRH's turn count)

        @param   (int *)  pIsGrannyApproachable is the pointer to nIsGrannyApproachable 
                (Tells whether or not LRRH has all objectives already complete to go to granny's house)

        @param   (int *)  pHasBread is the pointer to nHasBread 
                (Tells whether or not LRRH already has went to a bakery and the bread has not been eaten yet)

        @param   (int *)  pHasSensedBread is the pointer to nHasSensedBread 
                (Tells whether or not LRRH has already spotted a bakery)
        @param   (int *)  pHasSensedFlower is the pointer to nHasSensedFlower 
                (Tells whether or not LRRH has already spotted a flower field)
        @param   (int *)  pHasSensedWoodsman is the pointer to nHasSensedWoodsman 
                (Tells whether or not LRRH has already spotted the woodsman's house)
        @param   (int *)  pHasSensedGrandma is the pointer to nHasSensedGrandma 
                (Tells whether or not LRRH has already spotted granny's house)

        @param   (Queue *)  pXGridQueue is the pointer to XGridQueue
                (The queue which stores the X coordinates of the locations to explore later on)
        @param   (Queue *)  pYGridQueue is the pointer to YGridQueue 
                (The queue which stores the Y coordinates of the locations to explore later on)

        @param   (int)  nStepByStepMode tells whether or not to wait for user input until the next turn
        @param   (int)  nHasSounds tells whether or not the application should play sound for each turn 

        @return  (void) no return value
*/
void 
senseAndQueueLRRHForwardTile (int * pLRRHXLocation, 
                        int * pLRRHYLocation,
                        int * pWolfXCoord, 
                        int * pWolfYCoord,
                        int * pSensedTiles, 
                        char * pPlayGrid, 
                        char * pLRRHViewDirection, 
                        int nGridSize,
                        int * pNumOfSenses, 
                        int * pNumOfRotations, 
                        int * pNumOfMovements, 
                        int * pNumOfTurns,
                        int * pIsGrannyApproachable, 
                        int * pHasBread, 
                        int * pHasSensedBread, 
                        int * pHasSensedFlower, 
                        int * pHasSensedWoodsman, 
                        int * pHasSensedGrandma,
                        Queue * pXGridQueue, 
                        Queue * pYGridQueue, 
                        int nStepByStepMode, 
                        int nHasSounds, 
                        char cDirectionSymbol)
{
	//Sense what is currently in front of LRRH
	int nXSense = convertDirectionSymbolToXMovement (cDirectionSymbol);
	int nYSense = convertDirectionSymbolToYMovement (cDirectionSymbol);
	
        //Check first whether or not the sense location is valid (does not go over / under the grid size)
	if (isLocationValid (*pLRRHXLocation + nXSense, *pLRRHYLocation + nYSense, nGridSize - 1)){
		
                //If so, get the pointer of the tile to sense
		int * pTileToSense =  (pSensedTiles +  (*pLRRHXLocation + nXSense) * nGridSize +  (*pLRRHYLocation + nYSense));
		
                //Then, check whether or not the adjacent tile LRRH is currently facing was not yet sensed OR
                //If the tile has already been sensed, check if it was the wolf's tile.
                if (*pTileToSense == UNEXPLORED_TILE_VALUE || (*pTileToSense == -WOLF_TILE_VALUE) || (*pTileToSense == WOLF_TILE_VALUE)){

                        //If yes, rotate LRRH to face that tile first.
			smartRotateLRRH (pNumOfSenses, 
                                        pNumOfRotations, 
                                        pNumOfMovements, 
                                        pNumOfTurns,
                                        pLRRHViewDirection, 
                                        cDirectionSymbol,
                                        (pPlayGrid +  (*pLRRHXLocation) * nGridSize +  (*pLRRHYLocation)), 
                                        pPlayGrid,
                                        nGridSize,
                                        nStepByStepMode, 
                                        nHasSounds);
				
			//Then, Sense what is on that tile.
			char cLRRHSense = senseLRRH (pNumOfSenses,
                                                        pLRRHXLocation,
                                                        pLRRHYLocation, 
                                                        (pPlayGrid +  (*pLRRHXLocation) * nGridSize +  (*pLRRHYLocation)), 
                                                        pLRRHViewDirection, 
                                                        nGridSize);
			
                        displayPlayGridWithSense (pPlayGrid, 
                                                        nGridSize, 
                                                        *pNumOfSenses, 
                                                        *pNumOfRotations, 
                                                        *pNumOfMovements, 
                                                        cLRRHSense);
                        waitForNextTurn (pNumOfTurns, nStepByStepMode, nHasSounds);
			/*
				Assign a positive value if LRRH can walk on the tile and negative if not
				Flower: FLOWER_TILE_VALUE
				Woodsman: WOODSMAN_TILE_VALUE
				Bakery: BAKERY_TILE_VALUE
				Grandma: GRANDMA_TILE_VALUE / -GRANDMA_TILE_VALUE
				Wolf: WOLF_TILE_VALUE / -WOLF_TILE_VALUE
				Empty Tile: 1
				Pit: UNWALKABLE_TILE_VALUE
			*/
			

                        /*When spotting a special element, mark that element has sensed and assign its corresponding 
                          Sense value into the narrSensedTiles array. this will allow LRRH to access the array later on,
                          making her to able to remember which tiles has she sensed and where on the gird they are.*/
			switch (cLRRHSense){
			case FLOWER_SENSE_TILE:
				*pHasSensedFlower = 1;
				*pTileToSense = FLOWER_TILE_VALUE;						
				break;
			case WOODSMAN_SENSE_TILE:
				*pHasSensedWoodsman = 1;
				*pTileToSense = WOODSMAN_TILE_VALUE;						
				break;
			case BAKERY_SENSE_TILE:
				*pHasSensedBread = 1;
				*pTileToSense = BAKERY_TILE_VALUE;						
				break;
			case GRANDMA_SENSE_TILE:
				*pHasSensedGrandma = 1;	

                                /*Make granny's house unapproachable when objectives are not fulfilled yet by using 
                                a negative value to differentiate walkable and unwalkable tiles. */				
				if  (*pIsGrannyApproachable == 1) 
                                        *pTileToSense = GRANDMA_TILE_VALUE;
				else 
                                        *pTileToSense = -GRANDMA_TILE_VALUE;						
				
                                break;
			case WOLF_SENSE_TILE:	
                                /*Make the wolf unapproachable when LRRH does not have bread by using
                                a negative value to differentiate walkable and unwalkable tiles. */				
				if  (*pHasBread == 1) 
                                        *pTileToSense = WOLF_TILE_VALUE;
				else 
                                        *pTileToSense = -WOLF_TILE_VALUE;		
				
                                /*LRRH will be avoiding the wolf constantly, so it is better to have  
                                variables for it's location instead*/
                                
                                *pWolfXCoord = *pLRRHXLocation + nXSense;
				*pWolfYCoord = *pLRRHYLocation + nYSense;		
				
                                break;
			case EMPTY_SENSE_TILE:
				*pTileToSense = WALKABLE_TILE_VALUE;						
				break;
			case PIT_SENSE_TILE:
				*pTileToSense = UNWALKABLE_TILE_VALUE;						
				break;
			}						


			//If the sensed tile is walkable, queue that tile for viewing later.
			if (*pTileToSense > UNEXPLORED_TILE_VALUE){
				enqueue (pXGridQueue, *pLRRHXLocation + nXSense);
				enqueue (pYGridQueue, *pLRRHYLocation + nYSense);
			}			
		}
	}
}


/* This function allows LRRH to generate a path towards a destinatioin using the tiles that she has sensed before
        @param   (int)  nLRRHXLocation is the pointer to nLRRHXLocation (LRRH's X Location)
        @param   (int)  nLRRHYLocation is the pointer to nLRRHYLocation (LRRH's Y Location)

        @param   (int)  nNewLRRHXLocation is the pointer to nLRRHXLocation (LRRH's X Location)
        @param   (int)  nNewLRRHYLocation is the pointer to nLRRHYLocation (LRRH's Y Location)

        @param   (int *)  pSensedTiles is the pointer to the narrSensedTiles Array 
                (Contains information on which tiles LRRH has already sensed)      
        @param   (char *)  pPlayGrid is the pointer to carrPlayGrid

        @param   (int)  nGridSize is the size of the playing grid

        @param   (Queue *)  pActionQueue is the pointer to ActionQueue
                (The queue which stores the actions LRRH will execute later on)

        @param   (int *)  pIsDestinationValid is the pointer to nNumOfSenses (Keeps track of LRRH's sense count)
        
        @return  (void) no return value
*/
void 
generateAStarPath (int nLRRHXLocation, 
                        int nLRRHYLocation,
                        int nNewLRRHXLocation, 
                        int nNewLRRHYLocation,
                        int * pSensedTiles, 
                        char * pPlayGrid,
                        int nGridSize, 
                        Queue * pActionQueue, 
                        int * pIsDestinationValid)
{
        //Clear and reinitialize the action queue again before generating the path to clear it of any previous actions
	clearQueue (pActionQueue);
	initQueue (pActionQueue);
	
        //Save the coordinates of the wolf IF he happens to be passed by during the path generation
	int nWolfXCoord = -1;
	int nWolfYCoord = -1;
	int nPathMeetsWolf = 0;

        //For determining whether or not a destination is impossible to go to without passing through the wolf
	int nShouldGeneratePath = 1;

	//H costs and G costs arrays, used for path generation
	int narrGCosts[nGridSize][nGridSize];
	int narrHCosts[nGridSize][nGridSize];
	
	//Stores the tiles already viewed by LRRH
	int narrViewedTiles[nGridSize][nGridSize];

        //Stores the path that will be generated by LRRH
	char carrPathGrid[nGridSize][nGridSize];
	int i, j;

        /*If the number of checks LRRH does while checking the path goes over the maximum allowed of checks,
          that means that going to the destination is impossible*/
	int nMaxAllowedChecks = nGridSize * nGridSize;

	for (i = 0; i < nGridSize; i++)
        {
		for (j = 0; j < nGridSize; j++)
                {
			if (* (pSensedTiles + i * nGridSize + j) > 0){
                                //Initialize the tiles as unexplored
				narrGCosts[i][j] = UNEXPLORED_TILE_VALUE;
				narrHCosts[i][j] = UNEXPLORED_TILE_VALUE;

				narrViewedTiles[i][j] = UNEXPLORED_TILE_VALUE;
                                carrPathGrid[i][j] = SENSED_PATH_TILE;
			} else {
                                //Mark with a negative value to prevent using those tiles for path generation
				narrGCosts[i][j] = -1;
				narrHCosts[i][j] = -1;

                                //Subtract the number of max allowed checks, as she cannot check unsensed tiles
				nMaxAllowedChecks--;

                                //Make those tiles unwalkable
                                narrViewedTiles[i][j] = UNWALKABLE_TILE_VALUE;
                                carrPathGrid[i][j] = UNWALKABLE_PATH_TILE;
			}
			
			
		}
	}
	
        //Prevent backtrack to the starting point by marking it unwalkable
	narrViewedTiles[nLRRHXLocation][nLRRHYLocation] = UNWALKABLE_TILE_VALUE;

	//A* Path Generation

        /*
                A* Path generation uses costs assigned on each tile. The tile with the lowest cost is determined to be
                the best step to take next. This continues until the path reaches the destination.

                Steps of A* path generation:

                1. At the starting point, assign cost values to the adjacent tiles 
                        (Standard A* path generation assigns cost values to all 8 surrounding tiles,
                        This algorithm has been modified to only assign values to the 4 cardinal directions,
                        As LRRH can only move in those directions at a time)

                        (Standard A* path generation also requires the agent to have knowledge of the entire grid.
                        This part is modified, by only using the sensed tiles, or what LRRH only knows.)
                        3 types of cost values will be used:
                        1. The GCosts - The distance of the adjacent tile to the starting point
                        2. The HCosts - The distance of the adjacent tile to the destination
                        3. The FCosts - The sum of the GCosts and the HCosts
                2. After assigning the cost values, get the tile with the LOWEST FCost, if multiple tiles have
                   the same LOWEST FCost, get the LOWEST HCosts (closest to destination) from them. This ensures
                   that LRRH will take the shortest path based on the tiles she has already explored.
                3. After getting the tile with the LOWEST FCost, repeat steps 1 and 2, only now, use the adjacent tiles
                   of the tile with the LOWEST FCost.
                4. Repeat this process until the tile with the LOWEST FCost REACHES THE DESTINATION.
                5. Once it has, starting from the destination, retrace the steps taken to generate the path.
                6. Finally, make the agent perform the steps in reverse.
        */

        //Saves the coordinates of the tile that will be used for generating adjacent FCosts
        //Start at the starting point
	int nFCostXCoord = nLRRHXLocation;
	int nFCostYCoord = nLRRHYLocation;
	
        //Will be used for retracing the steps
	int nPreviousXCoord = 0;
        int nPreviousYCoord = 0;	
	
        
	
        //Used to check if a destination can be reached by LRRH
	int nNumOfChecks = 0;

        //Loop until we have reached the max amount of checks
	while (nNumOfChecks != nMaxAllowedChecks){
		
                //Will be used for getting the tile with the lowest F Cost.
                //Intialize variables to the highest value for sorting later.
		int nLowestFCost = INT_MAX;
		int nLowestHCost = INT_MAX;

                //Start checking the adjacent tiles.
		char cCurrentDirection = DIRECTION_SOUTH;

		/*Check each adjacent tile by looking around each direction of the FCost Tile
		NOTE: LRRH IS NOT YET MOVING AROUND HERE, 
                SHE IS SIMPLY LOOKING AT TILES SHE HAS ALREADY SENSED TO FIND THE SHORTEST PATH TO HER DESTINATION. */
		for (i = 0; i < 4; i++)
                {							
			
                        //Get the X and Y components of the direction
			int nXDirection = convertDirectionSymbolToXMovement (cCurrentDirection);
			int nYDirection = convertDirectionSymbolToYMovement (cCurrentDirection);
			
                        //Get the coordinates of the adjacent tile
                        int nAdjacentTileX = nFCostXCoord + nXDirection;
                        int nAdjacentTileY = nFCostYCoord + nYDirection;

			//Check if the adjacent tile is not out of bounds
			if (isLocationValid (nAdjacentTileX, nAdjacentTileY, nGridSize - 1)){				
				
				//Calculate the G and H costs of the adjacent tiles around the current tile
				int nNewGCost = getDistance (nAdjacentTileX, 
                                                                nAdjacentTileY, 
                                                                nLRRHXLocation, 
                                                                nLRRHYLocation);
				int nNewHCost = getDistance (nAdjacentTileX, 
                                                                nAdjacentTileY, 
                                                                nNewLRRHXLocation, 
                                                                nNewLRRHYLocation);
				
				
                                //Assign the G and H costs to the tile IF it is not already been viewed during path generation
				if (narrViewedTiles[nAdjacentTileX][nAdjacentTileY] == 
                                        UNEXPLORED_TILE_VALUE){	
					narrGCosts[nAdjacentTileX][nAdjacentTileY] = nNewGCost;
					narrHCosts[nAdjacentTileX][nAdjacentTileY] = nNewHCost;
					
					//Store the CurrentDirection in order to determine the path from the starting point
					carrPathGrid[nAdjacentTileX][nAdjacentTileY] = cCurrentDirection;

                                        //Allow the adjacent tile to be used for the path
					narrViewedTiles[nAdjacentTileX][nAdjacentTileY] = WALKABLE_TILE_VALUE;
				}				
			}
			
			//Switch the direction and go to view the next tile
			cCurrentDirection = simulateRightLRRHRotation (cCurrentDirection);		
		}
		
                //After checking each adjacent tile, save the previous F costs first before calculating the new one
		nPreviousXCoord = nFCostXCoord;
		nPreviousYCoord = nFCostYCoord;
		
		//Loop through the H and C Costs
		for (i = 0; i < nGridSize; i++)
                {
			for (j = 0; j < nGridSize; j++)
                        {
				int nCurrentHCost = narrHCosts[i][j];
				int nCurrentFCost = narrGCosts[i][j] + nCurrentHCost;
				
                                
				if (nCurrentFCost >= 0 && narrViewedTiles[i][j] == WALKABLE_TILE_VALUE){
					
                                        //If the current F Cost is lower than what is assigned in nLowestFCost
                                        //Update it to be the lower value and save its coordinates.

					if (nCurrentFCost < nLowestFCost){
						nLowestFCost = nCurrentFCost;
						nLowestHCost = nCurrentHCost;
						nFCostXCoord = i;
						nFCostYCoord = j;						
					} else if  (nCurrentFCost == nLowestFCost){
						//If the current FCost is equal to the Lowest FCost, Check their H costs.
                                                //If the current HCost is lower or equal to the Lowest FCost, 
                                                //Update the lowest FCost to be the current FCost and save its coordinates.						
						if (nCurrentHCost <= nLowestHCost){
							nLowestFCost = nCurrentFCost;
							nLowestHCost = nCurrentHCost;
							nFCostXCoord = i;
							nFCostYCoord = j;			
						}
					}
				}	
			}
		}
		
		//Once the spot with the lowest FCost has been determined, prevent that same tile from being viewed again
		narrViewedTiles[nFCostXCoord][nFCostYCoord] = UNWALKABLE_TILE_VALUE;
		

                //IF the lowest FCost HAPPENS TO BE THE WOLF TILE, Update the wolf coords and remember that the path
                //Intersects with the wolf.
		if (* (pSensedTiles + nFCostXCoord * nGridSize + nFCostYCoord) == WOLF_TILE_VALUE){
			nWolfXCoord = nFCostXCoord;
			nWolfYCoord = nFCostYCoord;
			nPathMeetsWolf = 1;
		}

		/*Once we have reached the destination, use the previous X and Y FCost coords 
                  to save the direction between the last and second to last tile and break from the loop.
		  This will be used to generate the path LRRH will take to the destination. */
		if (nFCostXCoord == nNewLRRHXLocation && nFCostYCoord == nNewLRRHYLocation){
			carrPathGrid[nFCostXCoord][nFCostYCoord] = directionVectorToSymbol (nFCostXCoord - nPreviousXCoord,
                                                                                                nFCostYCoord - nPreviousYCoord);
			break;
		}

                //Increment the number of check after each check
		nNumOfChecks++;
	}

        //If the number of checks is equal to the max allowed of checks, that means that the destination is impossible to reach.
	if (nNumOfChecks == nMaxAllowedChecks){

                //Prevent that tile from being reached ever again, and do not generate the path
		* (pSensedTiles + nNewLRRHXLocation * nGridSize + nNewLRRHYLocation) = UNWALKABLE_TILE_VALUE;
	
		*pIsDestinationValid = 0;
		nShouldGeneratePath = 0;
	} else if (nPathMeetsWolf){
		//If the path meets the wolf, try to generate a path that does not intersect the wolf.
		* (pSensedTiles + nWolfXCoord * nGridSize + nWolfYCoord) = -WOLF_TILE_VALUE;
		
                generateAStarPath (nLRRHXLocation, 
                                        nLRRHYLocation, 
                                        nNewLRRHXLocation, 
                                        nNewLRRHYLocation, 
                                        pSensedTiles, 
                                        pPlayGrid,
                                        nGridSize, 
                                        pActionQueue, 
                                        pIsDestinationValid);

                /*If the destination is valid, that means it is possible to reach it without crossing on the wolf tile.
                  Do not generate a new path if so. else, that means it is REQUIRED to cross the wolf tile to reach
                  the destination, so the path with the wolf tile has to be generated.*/
		if (*pIsDestinationValid == 1)
                        nShouldGeneratePath = 0;
                else 
                        nShouldGeneratePath = 1;
	}
	
	if (nShouldGeneratePath){
                /*After generating the path, start backtracking from the end.
                  Starting from the end is important as the path may not neccessarily 
                  be complete if we started at the starting point.
                  This is because the assigned directions on the path grid may lead to other destinations as well.*/

                //Trace the path, starting at the destination.
                int nTracingXLocation = nNewLRRHXLocation;
                int nTracingYLocation = nNewLRRHYLocation;

		//Get the saved direction in the path grid
		char cCurrentTracingDirection = carrPathGrid[nTracingXLocation][nTracingYLocation];

		//Retrace the steps taken
		//Do this while you are still not in the starting point
		while (nTracingXLocation != nLRRHXLocation || nTracingYLocation != nLRRHYLocation)
                {
			
			//Store the traced direction into a queue, then move opposite that direction
			enqueue (pActionQueue,  (int) cCurrentTracingDirection);
			
                        //Get the direction opposite to the saved direction
                        char cOppositeDirection = getOppositeDirection (cCurrentTracingDirection);
			
			/*Move the tracing location opposite to the direction in the Path Grid.
                          This is because the Path Grid saved the directions of the tiles that
                          started from the starting point towards the destination. However, we
                          are moving the opposite direction.*/
			int nXDirection = convertDirectionSymbolToXMovement (cOppositeDirection);
			int nYDirection = convertDirectionSymbolToYMovement (cOppositeDirection);
			
			nTracingXLocation += nXDirection;
			nTracingYLocation += nYDirection;
			
			//Get the new tracing direction and repeat.
			cCurrentTracingDirection = carrPathGrid[nTracingXLocation][nTracingYLocation];
		}
		
                //Set the destination as valid, as the path has reached the starting point
		*pIsDestinationValid = 1;
		
                //Reverse the order of directions in the action queue, as the generation started from the destination
                reverseQueue (pActionQueue);

                //Display the play grid and Path generation to the user
                printf ("\n");
                displayDivider ();
                printf ("\nStarting A* algorithm\n");
                printf ("Current Location: %d, %d\n", nLRRHXLocation + 1, nLRRHYLocation + 1);
                printf ("Destination: %d, %d\n", nNewLRRHXLocation + 1, nNewLRRHYLocation + 1);
                displayDivider ();
                printf ("\n");
                displayPlayAndPathGrid(pPlayGrid, &carrPathGrid[0][0], nGridSize, pActionQueue->pArray, pActionQueue->nCount);
	}	
}

/* This function allows LRRH to search for the closest bakery and generate a path to get to there
        @param   (int)  nLRRHXLocation is the pointer to nLRRHXLocation (LRRH's X Location)
        @param   (int)  nLRRHYLocation is the pointer to nLRRHYLocation (LRRH's Y Location)

        @param   (int *)  nBakeryXCoord is the pointer to nLRRHXLocation (LRRH's X Location)
        @param   (int *)  nBakeryYCoord is the pointer to nLRRHYLocation (LRRH's Y Location)

        @param   (int *)  pHasSensedBread is the pointer to nHasSensedBread
                (Tells whether or not LRRH has already spotted a bakery)

        @param   (int *)  pSensedTiles is the pointer to the narrSensedTiles Array 
                (Contains information on which tiles LRRH has already sensed)
        @param   (char *)  pPlayGrid is the pointer to carrPlayGrid

        @param   (int)  nGridSize is the size of the playing grid

        @param   (Queue *)  pActionQueue is the pointer to ActionQueue
                (The queue which stores the actions LRRH will execute later on)

        @param   (int *)  pIsDestinationValid is the pointer to nNumOfSenses (Keeps track of LRRH's sense count)
        
        @return  (void) no return value
*/
void 
generatePathToClosestBakery (int nLRRHXLocation, 
                                int nLRRHYLocation, 
                                int * nBakeryXCoord, 
                                int * nBakeryYCoord,
                                int * pHasSensedBread,
                                int * pSensedTiles, 
                                char * pPlayGrid,
                                int nGridSize, 
                                Queue * pActionQueue, 
                                int * pIsDestinationValid)
{
	
	/*There can only be a maximum of 9 bakeries FOR ALL GAME MODES,
          therefore we can initialize the array having negative values.
	  A Negative Value can then mean that there is no bakery at that location.*/
	int narrDistances[JUMBO_MODE_MAX_ELEMENTS] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
	int narrBakeryXCoords[JUMBO_MODE_MAX_ELEMENTS] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
	int narrBakeryYCoords[JUMBO_MODE_MAX_ELEMENTS] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
	int nNumOfBakeries = 0;
	int i, j;
	
	for (i = 0; i < nGridSize; i++)
        {
		for (j = 0; j < nGridSize; j++)
                {		
			//Look for the bakeries in the sensed tiles
			if (* (pSensedTiles + i * nGridSize + j) == BAKERY_TILE_VALUE){
				//Calculate the distance between LRRH and this bakery and save its coordinates			
				narrDistances[nNumOfBakeries] = getDistance (i, j, nLRRHXLocation, nLRRHYLocation);
				narrBakeryXCoords[nNumOfBakeries] = i;
				narrBakeryYCoords[nNumOfBakeries] = j;
				
                                //Increment the number of bakeries
				nNumOfBakeries++;
			}
				
		}
	}
	

        //Go through the number of bakeries and find the closest one
	int nLowestDistance = INT_MAX;
	int nClosestBakeryIndex = -1;
	
	for (i = 0; i < nNumOfBakeries; i++)
        {
                //If the distance is lower than the one assigned in lwoest distance, update it to be the lower one.
                //Save its index in the array.
		if (narrDistances[i] < nLowestDistance){
				nLowestDistance = narrDistances[i];
				nClosestBakeryIndex = i;
		}
	}
	
        //Assign the bakery coordinates to the coordinates of the closest bakery
	*nBakeryXCoord = narrBakeryXCoords[nClosestBakeryIndex];
	*nBakeryYCoord = narrBakeryYCoords[nClosestBakeryIndex];
	
	//Proceed to A star while using the closest bakery as the new location.
	generateAStarPath (nLRRHXLocation, 
                                nLRRHYLocation, 
                                *nBakeryXCoord, 
                                *nBakeryYCoord,
                                pSensedTiles, 
                                pPlayGrid,
                                nGridSize, 
                                pActionQueue, 
                                pIsDestinationValid);
        
        //If the path generation has failed, that means that the closest bakery is unreachable.
        if(*pIsDestinationValid == 0)
        {

                //If so, test the locations of other bakeries as well until the destination becomes valid
                while(*pIsDestinationValid == 0)
                {
                        //Decrement the number of bakeries
                        nNumOfBakeries--;

                        //If there are no more reachable bakeries, LRRH cannot sense the bakeries anymore
                        if(nNumOfBakeries == 0)
                        {
                                *pHasSensedBread = 0;
                                break;
                        }

                        //Assign the bakery coordinates to the coordinates of the next bakery
                        *nBakeryXCoord = narrBakeryXCoords[nNumOfBakeries];
                        *nBakeryYCoord = narrBakeryYCoords[nNumOfBakeries];
                        
                        //Proceed to A star while using the new bakery as the new location.
                        generateAStarPath (nLRRHXLocation, 
                                                nLRRHYLocation, 
                                                *nBakeryXCoord, 
                                                *nBakeryYCoord,
                                                pSensedTiles, 
                                                pPlayGrid,
                                                nGridSize, 
                                                pActionQueue, 
                                                pIsDestinationValid);
                        
                        
                }
                
                //If the destination became valid, that means LRRH can still sense the bakeries
                if(*pIsDestinationValid == 1)    
                        *pHasSensedBread = 1; 
                
        }
}


/* This function allows LRRH to move to a specific location using a generated path.
        @param   (int *)  pLRRHXLocation is the pointer to nLRRHXLocation (LRRH's X Location)
        @param   (int *)  pLRRHYLocation is the pointer to nLRRHYLocation (LRRH's Y Location)

        @param   (int)  nNewLRRHXLocation is the pointer to nLRRHXLocation (LRRH's X Location)
        @param   (int)  nNewLRRHYLocation is the pointer to nLRRHYLocation (LRRH's Y Location)

        @param   (int *)  pWolfXCoord is the pointer to nWolfXCoord (Wolf's X Location)
        @param   (int *)  pWolfYCoord is the pointer to nWolfYCoord (Wolf's Y Location)

        @param   (Queue *)  pActionQueue is the pointer to ActionQueue
                (The queue which stores the actions LRRH will execute later on)

        @param   (int *)  pNumOfSenses is the pointer to nNumOfSenses (Keeps track of LRRH's sense count)
        @param   (int *)  pNumOfRotations is the pointer to nNumOfRotations (Keeps track of LRRH's rotation count)
        @param   (int *)  pNumOfMovements is the pointer to nNumOfMovements (Keeps track of LRRH's movement count)
        @param   (int *)  pNumOfTurns is the pointer to nNumOfTurns (Keeps track of LRRH's turn count)

        @param   (int)  nGridSize is the size of the playing grid

        @param   (char *)  pPlayGrid is the pointer to carrPlayGrid
        @param   (int *)  pWalkedTiles is the pointer to the narrWalkedTiles Array 
                (Contains information on which tiles LRRH has already walked on)
        @param   (int *)  pSensedTiles is the pointer to the narrSensedTiles Array 
                (Contains information on which tiles LRRH has already sensed)

        @param   (int *)  pHasFlower is the pointer to nHasFlower 
                (Tells whether or not LRRH already has went to a flower field)
        @param   (int *)  pHasBread is the pointer to nHasBread 
                (Tells whether or not LRRH already has went to a bakery and the bread has not been eaten yet)
        @param   (int *)  pVisitedWoodsman is the pointer to nHasBread 
                (Tells whether or not LRRH already has visited the woodsman's house)

        @param   (int *)  pWasBreadTaken is the pointer to nWasBreadTaken 
                (Tells whether or not LRRH's bread was recently taken by the wolf last turn)

        @param   (char *)  pLRRHViewDirection is the pointer to cLRRHviewDirection (LRRH's viewing direction)
        @param   (char *)  pPreviousWalkedTileValue is the pointer to cPreviousWalkedTileValue 
                (The previous value of the tile LRRH is standing)

        @param   (int)  nStepByStepMode tells whether or not to wait for user input until the next turn
        @param   (int)  nHasSounds tells whether or not the application should play sound for each turn 

        @return  (void) no return value
*/
void 
moveLRRHToPath (int * pLRRHXLocation, 
                int * pLRRHYLocation,
                int nNewLRRHXLocation, 
                int nNewLRRHYLocation,
                int * pWolfXCoord, 
                int * pWolfYCoord,
                Queue * pActionQueue, 
                int * pNumOfSenses, 
                int * pNumOfRotations, 
                int * pNumOfMovements,
                int * pNumOfTurns, 
                int nGridSize, 
                char * pPlayGrid, 
                int * pWalkedTiles, 
                int * pSensedTiles,
                int * pHasFlower, 
                int * pHasBread, 
                int * pVisitedWoodsman, 
                int * pWasBreadTaken,
                char * pLRRHViewDirection, 
                char * pPreviousWalkedTileValue, 
                int nStepByStepMode, 
                int nHasSounds)
{
	
	//Check first whether or not LRRH has sensed the wolf
	int nHasSensedWolf;

	if (*pWolfXCoord == -1 && *pWolfYCoord == -1) 
                nHasSensedWolf = 0;
	else 
                nHasSensedWolf = 1;
	
        
	
        //Get the current location of LRRH
	char * pLRRHTile = (pPlayGrid +  (*pLRRHXLocation) * nGridSize +  (*pLRRHYLocation));

        //Keep moving LRRH until she has reached her destination
	while (1)
        {
                //Get the direction to move forward to
		char cMovingDirection = dequeue (pActionQueue);
			
		//Rotate LRRH until it is facing the same specified direction.
		smartRotateLRRH (pNumOfSenses, 
                                pNumOfRotations, 
                                pNumOfMovements, 
                                pNumOfTurns,
				pLRRHViewDirection, 
                                cMovingDirection,
				pLRRHTile, 
                                pPlayGrid,
				nGridSize, 
                                nStepByStepMode, 
                                nHasSounds);
			
		//Once LRRH is now looking at the right direction, move towards that direction.
		*pPreviousWalkedTileValue = forwardLRRH (pNumOfMovements, pLRRHXLocation, pLRRHYLocation, pLRRHTile, pLRRHViewDirection, nGridSize, *pPreviousWalkedTileValue);
		
                //Split LRRH's current direction to X and Y components
		int nXDirection = convertDirectionSymbolToXMovement (*pLRRHViewDirection);
		int nYDirection = convertDirectionSymbolToYMovement (*pLRRHViewDirection);
		
                //In the walked tiles array, assign that location as having been walked by LRRH
		* (pWalkedTiles +  (*pLRRHXLocation) * nGridSize +  (*pLRRHYLocation)) = 1;
		
                //Move the pointer of LRRH to her new location
                pLRRHTile = pLRRHTile + nXDirection * nGridSize + nYDirection;

                //Get the pointer to the wolf tile. 
		int * pWolfTile = pSensedTiles + *pWolfXCoord * nGridSize + *pWolfYCoord;
		
		/*After moving, check if LRRH is on a special tile  
                  (Flower, Bakery, Woodsman, Wolf w/ Bread).
                  Then, assign the booleans to say that she has walked over that tile already
		*/
                switch (*pPreviousWalkedTileValue){
		case FLOWER_TILE:
			*pHasFlower = 1;
			break;
		case BAKERY_TILE:
			*pHasBread = 1;

                        /*If LRRH has sensed the wolf and has gotten bread, 
                        assign a positive value so that it is possible for her to pass the wolf */
			if (nHasSensedWolf) 
                                *pWolfTile = WOLF_TILE_VALUE;
			*pWasBreadTaken = 0;
			break;
		case WOODSMAN_TILE:
			*pVisitedWoodsman = 1;
			
                        //Proceed to calculate the distance from the woodsman and grandma's house and display it to the user.
			int nWoodsmanDistance = computeWoodsmanDistance (pPlayGrid, nGridSize);
			displayWoodsmanLocation (nWoodsmanDistance);
			break;			
		case WOLF_TILE:
			
                        //If LRRH is on the wolf tile, check whether or not she has bread

			if (*pHasBread){
                                //If she does, she just loses the bread and cannot go to the wolf again

				*pHasBread = 0;
				*pWolfTile = -WOLF_TILE_VALUE;
				*pWasBreadTaken = 1;
				displayWolfEatingBread ();
			} else {

                                //If she doesn't, then she dies and the game is lost (break out of the loop later)
				*pWolfTile = WOLF_TILE_VALUE;
				*pWasBreadTaken = 0;
			}
				break;
		}		
		
		//Display the location and destination of LRRH, as well as the play grid
		printf ("\nCurrent Location: %d, %d\n", *pLRRHXLocation + 1, *pLRRHYLocation + 1);
		printf ("Destination: %d, %d\n", nNewLRRHXLocation + 1, nNewLRRHYLocation + 1);
                
		displayPlayGrid (pPlayGrid, nGridSize, *pNumOfSenses, *pNumOfRotations, *pNumOfMovements);
		waitForNextTurn (pNumOfTurns, nStepByStepMode, nHasSounds);
		
		//Break out of the loop once the queue has run out  (LRRH has reached the destination)
		if (*pLRRHXLocation == nNewLRRHXLocation && *pLRRHYLocation == nNewLRRHYLocation){
			printf ("\nDestination has been reached!\n ");
                        break;
		} else if (*pPreviousWalkedTileValue == GRANDMA_TILE || 
                        (*pWasBreadTaken == 0 && *pPreviousWalkedTileValue == WOLF_TILE))
                        break;	

                        
	}
}


/* This function allows LRRH to move to a specific location using a generated path while sensing the tiles adjacent to her.
        @param   (int *)  pLRRHXLocation is the pointer to nLRRHXLocation (LRRH's X Location)
        @param   (int *)  pLRRHYLocation is the pointer to nLRRHYLocation (LRRH's Y Location)

        @param   (int)  nNewLRRHXLocation is the pointer to nLRRHXLocation (LRRH's X Location)
        @param   (int)  nNewLRRHYLocation is the pointer to nLRRHYLocation (LRRH's Y Location)

        @param   (int *)  pWolfXCoord is the pointer to nWolfXCoord (Wolf's X Location)
        @param   (int *)  pWolfYCoord is the pointer to nWolfYCoord (Wolf's Y Location)

        @param   (Queue *)  pActionQueue is the pointer to ActionQueue
                (The queue which stores the actions LRRH will execute later on)
        @param   (Queue *)  pXGridQueue is the pointer to XGridQueue
                (The queue which stores the X coordinates of the locations to explore later on)
        @param   (Queue *)  pYGridQueue is the pointer to YGridQueue 
                (The queue which stores the Y coordinates of the locations to explore later on)

        @param   (int *)  pNumOfSenses is the pointer to nNumOfSenses (Keeps track of LRRH's sense count)
        @param   (int *)  pNumOfRotations is the pointer to nNumOfRotations (Keeps track of LRRH's rotation count)
        @param   (int *)  pNumOfMovements is the pointer to nNumOfMovements (Keeps track of LRRH's movement count)
        @param   (int *)  pNumOfTurns is the pointer to nNumOfTurns (Keeps track of LRRH's turn count)

        @param   (int)  nGridSize is the size of the playing grid

        @param   (char *)  pPlayGrid is the pointer to carrPlayGrid
        @param   (int *)  pWalkedTiles is the pointer to the narrWalkedTiles Array 
                (Contains information on which tiles LRRH has already walked on)
        @param   (int *)  pSensedTiles is the pointer to the narrSensedTiles Array 
                (Contains information on which tiles LRRH has already sensed)

        @param   (int *)  pHasFlower is the pointer to nHasFlower 
                (Tells whether or not LRRH already has went to a flower field)
        @param   (int *)  pHasBread is the pointer to nHasBread 
                (Tells whether or not LRRH already has went to a bakery and the bread has not been eaten yet)
        @param   (int *)  pVisitedWoodsman is the pointer to nHasBread 
                (Tells whether or not LRRH already has visited the woodsman's house)

        @param   (int *)  pWasBreadTaken is the pointer to nWasBreadTaken 
                (Tells whether or not LRRH's bread was recently taken by the wolf last turn)

        @param   (int *)  pHasSensedBread is the pointer to nHasSensedBread 
                (Tells whether or not LRRH has already spotted a bakery)
        @param   (int *)  pHasSensedFlower is the pointer to nHasSensedFlower 
                (Tells whether or not LRRH has already spotted a flower field)
        @param   (int *)  pHasSensedWoodsman is the pointer to nHasSensedWoodsman 
                (Tells whether or not LRRH has already spotted the woodsman's house)
        @param   (int *)  pHasSensedGrandma is the pointer to nHasSensedGrandma 
                (Tells whether or not LRRH has already spotted granny's house)

        @param   (int *)  pIsGrannyApproachable is the pointer to nIsGrannyApproachable 
                (Tells whether or not LRRH has all objectives already complete to go to granny's house)

        @param   (char *)  pLRRHViewDirection is the pointer to cLRRHviewDirection (LRRH's viewing direction)
        @param   (char *)  pPreviousWalkedTileValue is the pointer to cPreviousWalkedTileValue 
                (The previous value of the tile LRRH is standing)

        @param   (int)  nStepByStepMode tells whether or not to wait for user input until the next turn
        @param   (int)  nHasSounds tells whether or not the application should play sound for each turn 
        
        @return  (void) no return value
*/
void 
moveLRRHToPathWhileSensing (int * pLRRHXLocation, 
                                int * pLRRHYLocation,
                                int nNewLRRHXLocation, 
                                int nNewLRRHYLocation,
                                int * pWolfXCoord, 
                                int * pWolfYCoord,
                                Queue * pActionQueue,
                                Queue * pXGridQueue, 
                                Queue * pYGridQueue,
                                int * pNumOfSenses, 
                                int * pNumOfRotations, 
                                int * pNumOfMovements, 
                                int * pNumOfTurns, 
                                int nGridSize, 
                                char * pPlayGrid, 
                                int * pWalkedTiles, 
                                int * pSensedTiles,
                                int * pHasFlower, 
                                int * pHasBread, 
                                int * pVisitedWoodsman, 
                                int * pWasBreadTaken,
                                int * pHasSensedBread, 
                                int * pHasSensedFlower, 
                                int * pHasSensedWoodsman, 
                                int * pHasSensedGrandma, 
                                int * pIsGrannyApproachable,
                                char * pLRRHViewDirection, 
                                char * pPreviousWalkedTileValue, 
                                int nStepByStepMode, 
                                int nHasSounds)
{	
	
        //Check first whether or not LRRH has sensed the wolf
	int nHasSensedWolf;

	if (*pWolfXCoord == -1 && *pWolfYCoord == -1) 
                nHasSensedWolf = 0;
	else 
                nHasSensedWolf = 1;
	
        //Get the current location of LRRH
	char * pLRRHTile = (pPlayGrid +  (*pLRRHXLocation) * nGridSize +  (*pLRRHYLocation));

        //Keep moving LRRH until she has reached her destination
	while (1)
        {
                //Get the direction to move forward to
		char cMovingDirection = dequeue (pActionQueue);
			
		//Rotate LRRH until it is facing the same specified direction.
		smartRotateLRRH (pNumOfSenses, 
                                pNumOfRotations, 
                                pNumOfMovements, 
                                pNumOfTurns,
				pLRRHViewDirection, 
                                cMovingDirection,
				pLRRHTile, 
                                pPlayGrid,
				nGridSize, 
                                nStepByStepMode, 
                                nHasSounds);
			
		//Once LRRH is now looking at the right direction, move towards that direction.
		*pPreviousWalkedTileValue = forwardLRRH (pNumOfMovements, pLRRHXLocation, pLRRHYLocation, pLRRHTile, pLRRHViewDirection, nGridSize, *pPreviousWalkedTileValue);
		
                //Split LRRH's current direction to X and Y components
		int nXDirection = convertDirectionSymbolToXMovement (*pLRRHViewDirection);
		int nYDirection = convertDirectionSymbolToYMovement (*pLRRHViewDirection);
		
                //In the walked tiles array, assign that location as having been walked by LRRH
		* (pWalkedTiles +  (*pLRRHXLocation) * nGridSize +  (*pLRRHYLocation)) = 1;
		
                //Move the pointer of LRRH to her new location
                pLRRHTile = pLRRHTile + nXDirection * nGridSize + nYDirection;

                //Get the pointer to the wolf tile. 
		int * pWolfTile = pSensedTiles + *pWolfXCoord * nGridSize + *pWolfYCoord;
		
		/*After moving, check if LRRH is on a special tile  
                  (Flower, Bakery, Woodsman, Wolf w/ Bread).
                  Then, assign the booleans to say that she has walked over that tile already
		*/
                switch (*pPreviousWalkedTileValue){
		case FLOWER_TILE:
			*pHasFlower = 1;
			break;
		case BAKERY_TILE:
			*pHasBread = 1;

                        /*If LRRH has sensed the wolf and has gotten bread, 
                        assign a positive value so that it is possible for her to pass the wolf */
			if (nHasSensedWolf) 
                                *pWolfTile = WOLF_TILE_VALUE;
			*pWasBreadTaken = 0;
			break;
		case WOODSMAN_TILE:
			*pVisitedWoodsman = 1;
			
                        //Proceed to calculate the distance from the woodsman and grandma's house and display it to the user.
			int nWoodsmanDistance = computeWoodsmanDistance (pPlayGrid, nGridSize);
			displayWoodsmanLocation (nWoodsmanDistance);
			break;			
		case WOLF_TILE:
			
                        //If LRRH is on the wolf tile, check whether or not she has bread

			if (*pHasBread){
                                //If she does, she just loses the bread and cannot go to the wolf again

				*pHasBread = 0;
				*pWolfTile = -WOLF_TILE_VALUE;
				*pWasBreadTaken = 1;
				displayWolfEatingBread ();
			} else {

                                //If she doesn't, then she dies and the game is lost (break out of the loop later)
				*pWolfTile = WOLF_TILE_VALUE;
				*pWasBreadTaken = 0;
			}
				break;
		}
		
		//After moving, sense the tiles adjacent to the new tile.
		senseAndQueueLRRHAdjacentTiles (pLRRHXLocation, 
                                        pLRRHYLocation,
                                        pWolfXCoord, 
                                        pWolfYCoord, 
                                        pSensedTiles, 
                                        pPlayGrid, 
                                        pLRRHViewDirection, 
                                        nGridSize,
                                        pNumOfSenses, 
                                        pNumOfRotations, 
                                        pNumOfMovements, 
                                        pNumOfTurns,
                                        pIsGrannyApproachable, 
                                        pHasBread, 
                                        pHasSensedBread, 
                                        pHasSensedFlower, 
                                        pHasSensedWoodsman, 
                                        pHasSensedGrandma,
                                        pXGridQueue, 
                                        pYGridQueue, 
                                        nStepByStepMode, 
                                        nHasSounds);
		
		
		//Display the location and destination of LRRH, as well as the play grid
		printf ("\nCurrent Location: %d, %d\n", *pLRRHXLocation + 1, *pLRRHYLocation + 1);
		printf ("Destination: %d, %d\n", nNewLRRHXLocation + 1, nNewLRRHYLocation + 1);
                
		displayPlayGrid (pPlayGrid, nGridSize, *pNumOfSenses, *pNumOfRotations, *pNumOfMovements);
		waitForNextTurn (pNumOfTurns, nStepByStepMode, nHasSounds);
		
		//Break out of the loop once the queue has run out  (LRRH has reached the destination)
		if (*pLRRHXLocation == nNewLRRHXLocation && *pLRRHYLocation == nNewLRRHYLocation){
			printf ("\nDestination has been reached!\n ");
                        break;
		} else if (*pPreviousWalkedTileValue == GRANDMA_TILE || 
                        (*pWasBreadTaken == 0 && *pPreviousWalkedTileValue == WOLF_TILE))
                        break;	

                        
	}
}

/* This function updates the number of turns while waiting either for a next time or until the user presses the ENTER button
        
        @param   (int *)  pNumOfTurns is the pointer to nNumOfTurns (Keeps track of LRRH's turn count)

        @param   (int)  nStepByStepMode tells whether or not to wait for user input until the next turn
        @param   (int)  nHasSounds tells whether or not the application should play sound for each turn 
        
        @return  (void) no return value
*/
void 
waitForNextTurn (int * pNumOfTurns, int nStepByStepMode, int nHasSounds)
{
        //Increment the number of turns
	*pNumOfTurns += 1;
	
        //If step-by-step mode is enabled, wait for the user to press the ENTER button before executing the next turn 
	if (nStepByStepMode) {
		printf ("\nPress ENTER to start turn %d.\n", *pNumOfTurns);
		getchar ();
	}
	
        //If sounds are enabled, play a beep. else, sleep for a while
	if (nHasSounds)
                playAlertSound (); 
        else 
                Sleep (500);
	
        //Print the number of turns
	printf ("\nTURN %d\n", *pNumOfTurns);
	
        //Place a divider
	displayDivider ();
}

/* This function updates the number of turns while waiting either for a next time or until the user presses the ENTER button
        
        @param   (int *)  pSensedTiles is the pointer to the narrSensedTiles Array 
                (Contains information on which tiles LRRH has already sensed)
        @param   (int)  nGridSize is the size of the playing grid
        
        @return  (int) Gives a 0 if not every tile has been sensed, and a 1 if every tile has been sensed
*/
int 
sensedEveryTile (int * pSensedTiles, int nGridSize)
{
	//Loop through the sensed tiles array
	int i, j;
	for (i = 0; i < nGridSize; i++)
        {
		for (j = 0; j < nGridSize; j++)
                {		
			//If there is a tile that is yet to be sensed, return 0.
			if (* (pSensedTiles + i * nGridSize + j) == 0)
			        return 0;							
		}
	}
	
	//If you have not found one, return true
	return 1;
}

/* This function assigns the X and Y coordinates of a specific element that has already been sensed by LRRH. 
   Akin to LRRH remembering where something is.
        
        @param   (int)  nSenseValue is the sense value that is being recalled

        @param   (int *)  pXCoord is the pointer to the variable that will contain the X coordinate of the recalled element
        @param   (int *)  pYCoord is the pointer to the variable that will contain the Y coordinate of the recalled element

        @param   (int *)  pSensedTiles is the pointer to the narrSensedTiles Array 
                (Contains information on which tiles LRRH has already sensed)
        @param   (int)  nGridSize is the size of the playing grid
        
        @return  (void) no return value
*/
void recallSensedTile (int nSenseValue, int * pXCoord, int * pYCoord, int * pSensedTiles, int nGridSize){
	int nSearchForNegatives;
	
        //Also search for the negative values of the wolf and grandma tile if those are the targets
 	if (nSenseValue == WOLF_TILE_VALUE || 
                nSenseValue == -WOLF_TILE_VALUE || 
                nSenseValue == GRANDMA_TILE_VALUE || 
                nSenseValue == -GRANDMA_TILE_VALUE)
	{
		nSearchForNegatives = 1;
	} else 
                nSearchForNegatives = 0;
	
	
	int i,j;

        //Loop through the sensed tiles array
	for (i = 0; i < nGridSize; i++)
        {
		for (j = 0; j < nGridSize; j++)
                {		
			//Look for unsensed tiles in the sensed tiles

                        //If we are searching for negatives, then also consider the negative value of nSenseValue in the array
			if (nSearchForNegatives){
                                //If the sense value has been found, save the coordinates and break
				if ( (*(pSensedTiles + i * nGridSize + j) == nSenseValue ||
                                        *(pSensedTiles + i * nGridSize + j) == nSenseValue * -1)){
					
					//Found an instance of the tile we were searching for
					*pXCoord = i;
					*pYCoord = j;
					break;
				}	
			} else {
                        //If not, consider only the positive value.
                                //If the sense value has been found, save the coordinates and break
				if (*(pSensedTiles + i * nGridSize + j) == nSenseValue){
				
					//Found an instance of the tile we were searching for
					*pXCoord = i;
					*pYCoord = j;
					break;
				}	
			}						
		}
	}
	
}

/* This function assigns the X and Y coordinates of a specific element that has already been sensed by LRRH. 
   Akin to LRRH remembering where something is.
        
        @param   (int)  nLRRHXLocation is LRRH's X Location
        @param   (int)  nLRRHYLocation is LRRH's Y Location

        @param   (int)  nWolfXCoord is Wolf's X Location
        @param   (int)  nWolfYCoord is Wolf's Y Location

        @param   (int *)  pSensedTiles is the pointer to the narrSensedTiles Array 
                (Contains information on which tiles LRRH has already sensed)

        @param   (int)  nGridSize is the size of the playing grid
        @param   (int)  nQueueCapacity is the capacity of the coordinate queues

        @return  (int) Gives a 1 if the wolf tile is skippable, and a 0 if not
*/
int 
isWolfTileSkippable (int nLRRHXLocation, 
                        int nLRRHYLocation, 
                        int nWolfXCoord, 
                        int nWolfYCoord,
                        int * pSensedTiles, 
                        int nGridSize,
                        int nQueueCapacity)
{
        //If the wolf tile is the last one in the queue, then it is unskippable
        if(nQueueCapacity == 0) return 0;

        //Check all of the 8 surrounding tiles around the wolf
	int narrXDirectionCoordinates[8] = {1, -1, 1, -1, 1, -1, 0, 0};
	int narrYDirectionCoordinates[8] = {0, 0, 1, 1, -1, -1, 1, -1};

	int i;
        
        //Saves the amount of unwalkable tiles around the wolf
	int nNumUnwalkableTiles = 0;

        //For each surrounding tile...
	for (i = 0; i < 8; i++)
	{
                //Coordinates of the surrounding tile
		int nXCoord = (nWolfXCoord + narrXDirectionCoordinates[i]);
		int nYCoord = (nWolfYCoord + narrYDirectionCoordinates[i]);

                //Check if the surrounding tile is a valid location, if not, increment the number of unwalkable tiles
		if (isLocationValid (nXCoord, nYCoord, nGridSize - 1)){

                        //Check the value of the surrounding tile
			int * pSurroundingTile =  (pSensedTiles + nXCoord * nGridSize + nYCoord);
			
                        //If it is unwalkable, increment the number of unwalkable tiles
                        if (*pSurroundingTile < 0)
			        nNumUnwalkableTiles++;
			
		} else
			nNumUnwalkableTiles++;
	}

        /*If the amount of unwalkable tiles is greater than 1 (Meaning that a line of unwalkable tiles has been formed)
          It means that going through the wolf may just be the only option LRRH has. hence, the wolf tile is unskippable.
          Else, that means that there may be an opening around the wolf, so it can be skipped. */
          
	if (nNumUnwalkableTiles > 1) 
                return 0;
	else
                return 1;
}