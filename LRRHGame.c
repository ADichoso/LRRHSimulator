/*
* Programmed By: Aaron Gabrielle Dichoso 
*
* Description:
* 	LRRHGame.c: This file contains the LRRH Game and the logic used to run the game. 
*
* Last Modified: 07/02/2022
* Acknowledgements: 
*       Pointers and 2D arrays - https://overiq.com/c-programming-101/pointers-and-2-d-arrays/
*       Examples of Pathfinding Algorithms - http://theory.stanford.edu/~amitp/GameProgramming/AStarComparison.html
*       Breadth First Search Algorithm - https://www.youtube.com/watch?v=oDqjPvD54Ss
*       Breadth First Search Algorithm on a grid - https://www.youtube.com/watch?v=KiCBXu4P-2Y
*       A* Pathfinding algorithm - https://www.youtube.com/watch?v=-L-WgKMFuhE
*       The Story of Little Red Riding Hood - https://americanliterature.com/author/the-brothers-grimm/fairy-tale/little-red-cap
*/
#include "LRRHFunctions.c"
#include "LRRHGame_prototype.h"

#define MAX_INPUT_LENGTH 100

//FUNCTION PROTOTYPES
/* This function setups the playing grid for the game
        @param   (int *)  pGridSize is the pointer to nGridSize
        @param   (int)  nJumboMode tells whether or not the grid will be standard or jumbo mode
        @param   (int)  nHasSounds tells whether or not to play sound
        
        @return  (void) no return value
*/
void
getGridSize (int * pGridSize, int nJumboMode, int nHasSounds)
{
	printf ("\n");
	displayDivider ();
	
        //Contains the minimum and maximum grid size depending on the game mode
	int nMinGridSize, nMaxGridSize;
	
        
	
        //If it is jumbo mode, the grid can be between 15 - 25, else, it is between 8 - 15
        if (nJumboMode){
		nMinGridSize = STANDARD_MODE_MAX_GRID_SIZE;
		nMaxGridSize = JUMBO_MODE_MAX_GRID_SIZE;
	} else {
		nMinGridSize = STANDARD_MODE_MIN_GRID_SIZE;
		nMaxGridSize = STANDARD_MODE_MAX_GRID_SIZE;
	}

        //Prompt the user to enter a size for the playing grid
        //Do not stop asking until the application gets an appropriate value
	do
	{
		printf ("\nPlease enter the size of the playing grid  (Between %d - %d): ", nMinGridSize, nMaxGridSize);
	
                *pGridSize = scanNumber ();

		if (nHasSounds) 
                        playAlertSound ();

		if (!(*pGridSize >= nMinGridSize && *pGridSize <= nMaxGridSize)){
			*pGridSize = 0;
			printf ("\nPlease input an integer between %d and %d. Try again.\n", nMinGridSize, nMaxGridSize);
		}

	}while (*pGridSize == 0);
	
        //Display a divider to organize the GUI
        printf ("\n");
	displayDivider ();
}

/* This function initializes the location of the specific objects to be placed in the playing grid
        @param   (char *)  pPlayGrid is the pointer to carrMultipleObjSymbols
        @param   (int)  nGridSize is the size of the playing grid
        @param   (int)  nJumboMode tells whether or not the grid will be standard or jumbo mode
        @param   (int)  nHasSounds tells whether or not to play sound

        @return  (void) no return value
*/
void
initializeObjects (char * pPlayGrid, int nGridSize, int nJumboMode, int nHasSounds)
{	
	//Contains the symbols of the different objects needed to be placed that can have MULTIPLE INSTANCES
        char carrMultipleObjSymbols[3] = {PIT_TILE, BAKERY_TILE, FLOWER_TILE};	
	//Contains the symbols of the different objects needed
	char carrSingularObjSymbols[3] = {GRANDMA_TILE, WOLF_TILE, WOODSMAN_TILE};
	
        //Contains the maximum number of objects that can have multiple instances can have.
	int nMaxMultipleObjects;

	if (nJumboMode) 
		nMaxMultipleObjects = JUMBO_MODE_MAX_ELEMENTS;
	else 
		nMaxMultipleObjects = STANDARD_MODE_MAX_ELEMENTS;
	
	int i = 0;
        //Ask the input for each element in carrMultipleObjSymbols

        //Not using a for loop because it should only increment at certain conditions
	while (i < 3)
	{
                //Ask the user how many instances of a specific object it wants
		int nNumOfObjects;
		
		printf ("\n");
		displayDivider ();
		printf ("\nPlease input how many %ss do you want to place  (Maximum of %d): ", 
				objectSymbolToName (carrMultipleObjSymbols[i]), 
				nMaxMultipleObjects);
		
		//Input the number of objects to be placed
		nNumOfObjects = scanNumber ();
                
                //Play an alert sound only if the mode can
		if (nHasSounds) 
                        playAlertSound ();

                //If the specified number of objects is within the correct range...
                //Else, ask the user to input a new value again
		if  (nNumOfObjects > 0 && nNumOfObjects <= nMaxMultipleObjects){	
			int j = 0;	
			
			///Ask the user to input and X and Y locations of each object
			while (j < nNumOfObjects)
			{
				int nXLocation = -1;
				int nYLocation = -1;
				
                                //XLocation Prompt
				printf ("\nPlease input the X location of %s #%d: ",
						objectSymbolToName (carrMultipleObjSymbols[i]),
						j + 1);
				
			        nXLocation = scanNumber ();

                                //Play an alert sound only if the mode can
                                if (nHasSounds) 
                                        playAlertSound ();


                                //YLocation Prompt
                                printf ("\nPlease input the Y location of %s #%d: ",
						objectSymbolToName (carrMultipleObjSymbols[i]),
						j + 1);
				
			        nYLocation = scanNumber ();


				//Play an alert sound only if the mode can
                                if (nHasSounds) 
                                        playAlertSound ();
				

				//Specified Grid Locations should be greater than 0 and less than or equal to nGridSize.
                                //Else, ask the user to input a new value again
				if (isLocationValid (nXLocation - 1, nYLocation - 1, nGridSize)){

                                        //Get the location of the specified tile			
					char * pTileLocation = (pPlayGrid +  (nXLocation - 1) * nGridSize +  (nYLocation - 1));
					
                                        //Grid Location should be empty before assignment
					if (*pTileLocation == EMPTY_TILE){			
                                                //Assign the symbol on that tile location									
						*pTileLocation = carrMultipleObjSymbols[i];

                                                //Increment to the next object instance
						j++;
					} else 
                                                printf ("\nThe selected square is not available! Try again!");
                                } else 
					printf ("\nPlease enter an integer greater than 0 and less than %d. Try again.",
                                                 nGridSize + 1);
			}
			i++;
		} else 
			printf ("\nPlease enter an integer between 1 and %d. Try again.", nMaxMultipleObjects);
	}
	
	
	
	//Ask the input for each element in carrSingularObjSymbols

        //Not using a for loop because it should only increment at certain conditions
	i = 0;
        ///Ask the user to input and X and Y locations of each singular object
	while (i < 3)
	{
		//Input the number of objects to be placed
		int nXLocation = -1;
		int nYLocation = -1;
		
		printf ("\n");
		displayDivider ();

		printf ("\nPlease input the X location of %s: ", objectSymbolToName (carrSingularObjSymbols[i]));
				
		nXLocation = scanNumber ();

                //Play an alert sound only if the mode can
                if (nHasSounds) 
                        playAlertSound ();

                printf ("\nPlease input the Y location of %s: ", objectSymbolToName (carrSingularObjSymbols[i]));
				
		nYLocation = scanNumber ();

                //Play an alert sound only if the mode can
		if (nHasSounds) 
                        playAlertSound ();

                //Specified Grid Locations should be greater than 0 and less than or equal to nGridSize.
                //Else, ask the user to input a new value again
		if (isLocationValid (nXLocation - 1, nYLocation - 1, nGridSize)){	
                        //Get the location of the specified tile			
			char * pTileLocation = (pPlayGrid +  (nXLocation - 1) * nGridSize +  (nYLocation - 1));		
                        
			//Grid Location should be empty before assignment
			if (*pTileLocation == EMPTY_TILE){		
                                //Assign the symbol on that tile location										
				*pTileLocation = carrSingularObjSymbols[i];

                                //Increment to the next object instance
				i++;
			} else 
				printf ("\nThe selected square is not available! Try again!");
		} else
			printf ("\nPlease enter an input greater than 0 and less than %d. Try again.", nGridSize + 1);
	}
	
        //Display a divider at the end
	displayDivider ();
}

/* This function is used to initialize, run the game, make LRRH do actions, and end the game once finished.
        @param   (int)  nJumboMode tells whether or not the grid will be standard or jumbo mode
        @param   (int)  nStepByStepMode tells whether or not LRRH will move automatically
        @param   (int)  nHasSounds tells whether or not to play sound
        @param   (int)  nSmartMode tells whether or not LRRH will move intelligently or randomly

        @return  (void) no return value
*/
void 
runGame (int nJumboMode, int nStepByStepMode, int nHasSounds, int nSmartMode)
{
	//Determines the size of the playing grid
	int nGridSize;
	
	//Initial Locations of LRRH
	int nLRRHXLocation = 0;
        int nLRRHYLocation = 0;
	
	//Counter for Senses, Rotations, and Movements of LRRH
	int nNumOfSenses = 0;
	int nNumOfRotations = 0;
	int nNumOfMovements = 0;
	int nNumOfTurns = 0;
	
        //Used to tell whether or not LRRH has gotten a certain element
	int nHasFlower = 0;
	int nHasBread = 0; 
	int nVisitedWoodsman = 0;
        
        //Get the size of the grid from the user
	getGridSize (&nGridSize, nJumboMode, nHasSounds);
	
        //Initialize the Playing Grid and the Sensed and Walked Tiles Array size.

	//Contains the entire playing grid
	char carrPlayGrid[nGridSize][nGridSize];	

	//Contains the tiles that LRRH has already sensed
	int narrSensedTiles[nGridSize][nGridSize];
	int narrWalkedTiles[nGridSize][nGridSize];
	
	//The viewing direction LRRH is currently facing. Default is south  (DIRECTION_SOUTH)
	char cLRRHViewDirection = DIRECTION_SOUTH;
	
        //Contains the previous tile value that LRRH is currently standing on
	char cPreviousWalkedTileValue = EMPTY_TILE;
	
	//Loop through and Initialize the Grid values
	int i, j;
	for (i = 0; i < nGridSize; i++)
	{
		for (j = 0; j < nGridSize; j++)
		{
			carrPlayGrid[i][j] = EMPTY_TILE;
			narrSensedTiles[i][j] = 0;
			narrWalkedTiles[i][j] = 0;
		}
	}
	
	/* PLACEMENT OF OBJECTS:
	1. LRRH
	2. Objects with possible multiple instances
	3. Objects with 1 possible instance
	*/
	
        //Initialize LRRH's location
	//LRRH will always start at  (1,1)
	carrPlayGrid[nLRRHXLocation][nLRRHYLocation] = LRRH_DIRECTION_SOUTH;
	narrSensedTiles[nLRRHXLocation][nLRRHYLocation] = 1;
	narrWalkedTiles[nLRRHXLocation][nLRRHYLocation] = 1;

        //Render (Print) the grid at the very end after all the elements on the Grid have been initialized
	displayPlayGrid (&carrPlayGrid[0][0], nGridSize, nNumOfSenses, nNumOfRotations, nNumOfMovements);

	//Initialize the locations of the objects on the grid
	initializeObjects (&carrPlayGrid[0][0], nGridSize, nJumboMode, nHasSounds);


        waitForNextTurn (&nNumOfTurns, nStepByStepMode, nHasSounds);
        //If smart mode is enabled, make LRRH move intelligently. Else, She will move randomly.
        if (nSmartMode){
                runSmartLRRH (&nLRRHXLocation, 
                                &nLRRHYLocation, 
                                nGridSize,
                                &cLRRHViewDirection,
                                &cPreviousWalkedTileValue,
                                &carrPlayGrid[0][0], 
                                &narrSensedTiles[0][0], 
                                &narrWalkedTiles[0][0], 
                                &nNumOfSenses, 
                                &nNumOfRotations, 
                                &nNumOfMovements, 
                                &nNumOfTurns,
                                &nHasFlower,
                                &nHasBread,
                                &nVisitedWoodsman,
                                nStepByStepMode,
                                nHasSounds);
        } else 
        {
                runRandomLRRH(&nLRRHXLocation, 
                                &nLRRHYLocation, 
                                nGridSize,
                                &cLRRHViewDirection,
                                &cPreviousWalkedTileValue,
                                &carrPlayGrid[0][0],
                                &nNumOfSenses, 
                                &nNumOfRotations, 
                                &nNumOfMovements, 
                                &nNumOfTurns,
                                &nHasFlower,
                                &nHasBread,
                                &nVisitedWoodsman,
                                nStepByStepMode,
                                nHasSounds);
        }


        displayEndGameMessage(cPreviousWalkedTileValue, nNumOfSenses, nNumOfRotations, nNumOfMovements, nHasBread, nHasFlower, nVisitedWoodsman);
}


/* This function makes LRRH perform actions intelligently in the playing grid to achieve her goal.
        @param   (int *)  pLRRHXLocation is the pointer to nLRRHXLocation (LRRH's X Location)
        @param   (int *)  pLRRHYLocation is the pointer to nLRRHYLocation (LRRH's Y Location)

        @param   (char *)  pLRRHViewDirection is the pointer to cLRRHviewDirection (LRRH's viewing direction)
        @param   (char *)  pPreviousWalkedTileValue is the pointer to cPreviousWalkedTileValue 
                (The previous value of the tile LRRH is standing)

        @param   (char *)  pPlayGrid is the pointer to carrPlayGrid
        @param   (int *)  pWalkedTiles is the pointer to the narrWalkedTiles Array 
                (Contains information on which tiles LRRH has already walked on)
        @param   (int *)  pSensedTiles is the pointer to the narrSensedTiles Array 
                (Contains information on which tiles LRRH has already sensed)

        @param   (int *)  pNumOfSenses is the pointer to nNumOfSenses (Keeps track of LRRH's sense count)
        @param   (int *)  pNumOfRotations is the pointer to nNumOfRotations (Keeps track of LRRH's rotation count)
        @param   (int *)  pNumOfMovements is the pointer to nNumOfMovements (Keeps track of LRRH's movement count)
        @param   (int *)  pNumOfTurns is the pointer to nNumOfTurns (Keeps track of LRRH's turn count)

        @param   (int)  nGridSize is the size of the playing grid         

        @param   (int *)  pHasFlower is the pointer to nHasFlower 
                (Tells whether or not LRRH already has went to a flower field)
        @param   (int *)  pHasBread is the pointer to nHasBread 
                (Tells whether or not LRRH already has went to a bakery and the bread has not been eaten yet)
        @param   (int *)  pVisitedWoodsman is the pointer to nHasBread 
                (Tells whether or not LRRH already has visited the woodsman's house)

        @param   (int)  nStepByStepMode tells whether or not to wait for user input until the next turn
        @param   (int)  nHasSounds tells whether or not the application should play sound for each turn 
        
        @return  (void) no return value
*/
void 
runSmartLRRH (int * pLRRHXLocation,
                int * pLRRHYLocation, 
                int nGridSize,
                char * pLRRHViewDirection,
                char * pPreviousWalkedTileValue,
                char * pPlayGrid, 
                int * pSensedTiles, 
                int * pWalkedTiles,
                int * pNumOfSenses,
                int * pNumOfRotations,
                int * pNumOfMovements,
                int * pNumOfTurns,
                int * pHasFlower,
                int * pHasBread,
                int * pVisitedWoodsman,
                int nStepByStepMode,
                int nHasSounds)
{
        //Queues used to store the coordinates that LRRH will go to.
	Queue XGridQueue, YGridQueue;

        //Queue used to store the actions LRRH will do.
	Queue ActionQueue;
	
        //Initialize the queues
	initQueue (&XGridQueue);
	initQueue (&YGridQueue);
	initQueue (&ActionQueue);


	//Enqueue the starting position to the queue
	enqueue (&XGridQueue, *pLRRHXLocation);
	enqueue (&YGridQueue, *pLRRHYLocation);


	//Used to tell whether or not LRRH has sensed a certain element
	int nHasSensedBread = 0;
	int nHasSensedFlower = 0;
	int nHasSensedWoodsman = 0;
	int nHasSensedGrandma = 0;
	
        //Used to tell whether or not LRRH's bread was taken by the wolf.
	int nWasBreadTaken = 0;
	
        //Used to hold the X and Y coordinates of the wolf
	int nWolfXCoord = -1;
	int nWolfYCoord = -1;

        //Used to tell whether or not a destination LRRH is going to can be went into.
	int nIsDestinationValid = 0;

	// Start of the Game Loop
	while (1)
	{

		//Dequeue coordinates from the X and Y queues.	
		int nXGridLocation = dequeue (&XGridQueue);
		int nYGridLocation = dequeue (&YGridQueue);

                //If the coordinates are equal to the wolf locations, then check first whether or not the wolf tile is skippable
		if (nXGridLocation == nWolfXCoord && nYGridLocation == nWolfYCoord){
			int nIsWolfTileSkippable = isWolfTileSkippable (*pLRRHXLocation, 
                                                                        *pLRRHYLocation,
					 				nXGridLocation, 
                                                                        nYGridLocation,
									pSensedTiles, 
                                                                        nGridSize,
                                                                        XGridQueue.nCount);
			
                        //If so, then enqueue the wolf tile back to the back of the queue and skip its movement
                        if (nIsWolfTileSkippable) 
                        {
                                enqueue (&XGridQueue, nXGridLocation);
				enqueue (&YGridQueue, nYGridLocation);
                                continue;
                        }                                
		}

                

                //Used to check whether or not all of the special tiles have been sensed by LRRH
                int nHasSensedAllElements =  (nHasSensedBread && nHasSensedFlower && nHasSensedWoodsman && nHasSensedGrandma);


                //If LRRH does not have a flower and has sensed a flower, then move to that location
		if (!*pHasFlower && nHasSensedFlower){

                        //Get the flower coordinates and then generate a path to there
			int nFlowerXCoord, nFlowerYCoord;

			recallSensedTile (FLOWER_TILE_VALUE, &nFlowerXCoord, &nFlowerYCoord, pSensedTiles, nGridSize);

			generateAStarPath (*pLRRHXLocation, 
                                                *pLRRHYLocation,
				 		nFlowerXCoord, 
                                                nFlowerYCoord, 
						pSensedTiles, 
                                                pPlayGrid,
                                                nGridSize,
						&ActionQueue, 
                                                &nIsDestinationValid);
			
                        //If the path generation succeeds, go there
			if (nIsDestinationValid){
				moveLRRHToPath (&*pLRRHXLocation, 
                                                pLRRHYLocation,
						nFlowerXCoord, 
                                                nFlowerYCoord,
						&nWolfXCoord, 
                                                &nWolfYCoord,
						&ActionQueue,
						pNumOfSenses,
                                                pNumOfRotations, 
                                                pNumOfMovements, 
                                                pNumOfTurns,
						nGridSize,
						pPlayGrid, 
                                                pWalkedTiles, 
                                                pSensedTiles,	
						pHasFlower, 
                                                pHasBread, 
                                                pVisitedWoodsman, 
                                                &nWasBreadTaken,
						pLRRHViewDirection, 
                                                pPreviousWalkedTileValue,
						nStepByStepMode, 
                                                nHasSounds);
			}
		}
		
                //If LRRH does not have a woodsman and has sensed the woodsman, then move to that location
		if (!*pVisitedWoodsman && nHasSensedWoodsman){
			
                        //Get the woodsman coordinates and then generate a path to there
			int nWoodsmanXCoord, nWoodsmanYCoord;

			recallSensedTile (3, &nWoodsmanXCoord, &nWoodsmanYCoord, pSensedTiles, nGridSize);

			generateAStarPath (*pLRRHXLocation,
					        *pLRRHYLocation,
				 		nWoodsmanXCoord, 
						nWoodsmanYCoord, 
						pSensedTiles,
                                                pPlayGrid, 
						nGridSize,
						&ActionQueue, 
						&nIsDestinationValid);
			
                        //If the path generation succeeds, go there
			if (nIsDestinationValid){
				moveLRRHToPath (pLRRHXLocation, 
						pLRRHYLocation,
						nWoodsmanXCoord, 
						nWoodsmanYCoord,
						&nWolfXCoord, 
						&nWolfYCoord,
						&ActionQueue,
						pNumOfSenses, 
						pNumOfRotations, 
						pNumOfMovements, 
						pNumOfTurns,
						nGridSize,
						pPlayGrid, 
						pWalkedTiles, 
						pSensedTiles,	
						pHasFlower, 
						pHasBread, 
						pVisitedWoodsman, 
						&nWasBreadTaken,
						pLRRHViewDirection, 
						pPreviousWalkedTileValue,
						nStepByStepMode, 
						nHasSounds);
			}
		}

                /*If LRRH has already sensed all elements and does not have bread, then go to the bakery
                  LRRH will only go to the bakery after sensing ALL elements because the wolf may or may not eat it.
                  Because of this, to prevent the constant going back of LRRH to the bakery, then LRRH will only do so. */

		if (nHasSensedAllElements && !*pHasBread){
			/*Bread can be stolen by the wolf. When that happens, 
                        LRRH has to go to the nearest bakery, 
                        so first determine the closest one using an FCost calculation.*/
			
                        //Get the bakery coordinates and then generate a path to there
                        int nBakeryXCoord, nBakeryYCoord;

			generatePathToClosestBakery (*pLRRHXLocation,
							*pLRRHYLocation,
							&nBakeryXCoord, 
							&nBakeryYCoord,
                                                        &nHasSensedBread,
							pSensedTiles, 
                                                        pPlayGrid,
							nGridSize,
							&ActionQueue, 
							&nIsDestinationValid);

                        //If the path generation succeeds, go there
			if (nIsDestinationValid){
				moveLRRHToPath (pLRRHXLocation, 
						pLRRHYLocation,
						nBakeryXCoord, 
						nBakeryYCoord,
						&nWolfXCoord,
						&nWolfYCoord,
						&ActionQueue,
						pNumOfSenses, 
						pNumOfRotations, 
						pNumOfMovements, 
						pNumOfTurns,
						nGridSize,
						pPlayGrid, 
						pWalkedTiles, 
						pSensedTiles,	
						pHasFlower, 
						pHasBread, 
						pVisitedWoodsman, 
						&nWasBreadTaken,
						pLRRHViewDirection, 
						pPreviousWalkedTileValue,
						nStepByStepMode, 
						nHasSounds);
			}
		}

                //Check whether or not LRRH has gotten all elements, is so, she can then go to Granny's house.
                int nIsGrannyApproachable =  (*pHasBread && *pHasFlower && *pVisitedWoodsman && nHasSensedGrandma);

		if (nIsGrannyApproachable){
                        
                        //Get the granny coordinates and then generate a path to there
			int nGrannyXCoord, nGrannyYCoord;

			recallSensedTile (GRANDMA_TILE_VALUE, &nGrannyXCoord, &nGrannyYCoord, pSensedTiles, nGridSize);
			
                        //Update the sensed tiles array to make granny's location approachable.
			
                        *(pSensedTiles + nGrannyXCoord * nGridSize + nGrannyYCoord) = GRANDMA_TILE_VALUE;

			generateAStarPath (*pLRRHXLocation, 
						*pLRRHYLocation, 
						nGrannyXCoord,
					        nGrannyYCoord, 
						pSensedTiles,
                                                pPlayGrid, 
						nGridSize, 
						&ActionQueue, 
						&nIsDestinationValid);

                        //If the path generation succeeds, go there
			if (nIsDestinationValid){
				moveLRRHToPath (pLRRHXLocation,
						pLRRHYLocation, 
						nGrannyXCoord,
						nGrannyYCoord,
						&nWolfXCoord, 
						&nWolfYCoord,
						&ActionQueue, 
						pNumOfSenses, 
						pNumOfRotations, 
						pNumOfMovements, 
						pNumOfTurns,
						nGridSize, 
						pPlayGrid,
						pWalkedTiles,	
						pSensedTiles,	
						pHasFlower, 
						pHasBread, 
						pVisitedWoodsman, 
						&nWasBreadTaken,
						pLRRHViewDirection, 
						pPreviousWalkedTileValue, 
						nStepByStepMode, 
						nHasSounds);
			}

                        //Break from the game loop and check the end game scenario
			break;
		}
		
	        //Check if LRRH is not currently in the dequeued location, then go to that location
		if ( (nXGridLocation != *pLRRHXLocation || nYGridLocation != *pLRRHYLocation)){
				
			//If LRRH is not there, move LRRH to that location
			generateAStarPath (*pLRRHXLocation,
					        *pLRRHYLocation, 
						nXGridLocation, 
						nYGridLocation, 
						pSensedTiles, 
                                                pPlayGrid,
						nGridSize, 
						&ActionQueue, 
						&nIsDestinationValid);
			
                        //If the path generation succeeds, go there while sensing the adjacent tiles
			if (nIsDestinationValid){
				moveLRRHToPathWhileSensing (pLRRHXLocation, 
						                pLRRHYLocation, 
						         	nXGridLocation, 
								nYGridLocation,
								&nWolfXCoord, 
							        &nWolfYCoord,
								&ActionQueue, 
								&XGridQueue, 
								&YGridQueue,
								pNumOfSenses, 
								pNumOfRotations, 
								pNumOfMovements, 
								pNumOfTurns,
								nGridSize, 
								pPlayGrid,	
								pWalkedTiles,	
								pSensedTiles,		
								pHasFlower, 
								pHasBread, 
								pVisitedWoodsman, 
								&nWasBreadTaken, 
								&nHasSensedBread, 
								&nHasSensedFlower, 
								&nHasSensedWoodsman, 
								&nHasSensedGrandma, 
								&nIsGrannyApproachable,
								pLRRHViewDirection, 
								pPreviousWalkedTileValue, 
								nStepByStepMode, 
								nHasSounds);
			}				
		}
		
                //If the current queue count is greater than 1, then remove tiles already walked on by LRRH from the queue.
                //Also, sort the coordinates in the queue by distance
		if (XGridQueue.nCount > 1){
			removeWalkedCoordinatesFromQueue (&XGridQueue, &YGridQueue, pWalkedTiles, nGridSize);
			sortCoordinatesByDistance (&XGridQueue, &YGridQueue, *pLRRHXLocation, *pLRRHYLocation);
		}
		

                //Prevent LRRH from rotating if she is about to view an already sensed tile
		//Add function in the movement system to allow LRRH to sense what is in front of her as well.
		senseAndQueueLRRHAdjacentTiles (pLRRHXLocation,
                                                pLRRHYLocation,
                                                &nWolfXCoord, 
                                                &nWolfYCoord,
                                                pSensedTiles, 
                                                pPlayGrid, 
                                                pLRRHViewDirection, 
                                                nGridSize,
                                                pNumOfSenses, 
                                                pNumOfRotations, 
                                                pNumOfMovements, 
                                                pNumOfTurns,
                                                &nIsGrannyApproachable, 
                                                pHasBread, 
                                                &nHasSensedBread, 
                                                &nHasSensedFlower, 
                                                &nHasSensedWoodsman, 
                                                &nHasSensedGrandma,
                                                &XGridQueue, 
                                                &YGridQueue, 
                                                nStepByStepMode, 
                                                nHasSounds);

		/*	
		If ANY OF THE POSSIBLE ENDINGS HAS OCCURED (Fall to Pit, Eaten by Wolf, Went to Grandma, Sensed Every Tile),
                BREAK FROM THE LOOP */
		int nHasScenarioHappened =  ((XGridQueue.nCount == 0) ||
					*pPreviousWalkedTileValue == PIT_TILE ||
					*pPreviousWalkedTileValue == GRANDMA_TILE ||
					(*pPreviousWalkedTileValue == WOLF_TILE && !nWasBreadTaken) ||
					sensedEveryTile (pSensedTiles, nGridSize));
		
		if (nHasScenarioHappened) 
                        break;
	}

	//ALL CODE HERE PERTAINS TO WHEN A SCENARIO HAS HAPPENED  (LITTLE RED RIDING HOOD HAS REACHED THE END OF HER STORY)

        //If LRRH has already sensed every tile & has sensed grandma & LRRH is currently not on grandma's doll
	if  (sensedEveryTile (pSensedTiles, nGridSize) && nHasSensedGrandma && *pPreviousWalkedTileValue != GRANDMA_TILE){

                //Look for the granny coordinates
		int nGrannyXCoord = -1;
                int nGrannyYCoord = -1;
			
		recallSensedTile (GRANDMA_TILE_VALUE, &nGrannyXCoord, &nGrannyYCoord, pSensedTiles, nGridSize);
			
		
                //AT THE START, DO NOT ALLOW LRRH do go to Granny's tile
		*(pSensedTiles + nGrannyXCoord * nGridSize + nGrannyYCoord) = -GRANDMA_TILE_VALUE;
		
                //If LRRH has sensed a bakery and has not gotten a bread, go to the closest bakery
		if (!*pHasBread && nHasSensedBread){

			/*Bread can be stolen by the wolf. When that happens, LRRH has to go to the nearest bakery.*/
			int nBakeryXCoord = -1;
                        int nBakeryYCoord = -1;

			generatePathToClosestBakery (*pLRRHXLocation,
							*pLRRHYLocation, 
							&nBakeryXCoord, 
							&nBakeryYCoord, 
                                                        &nHasSensedBread,
							pSensedTiles, 
                                                        pPlayGrid,
							nGridSize, 
							&ActionQueue, 
							&nIsDestinationValid);
			
                        //If the path generation succeeds, go to the bakery
			if (nIsDestinationValid){
				moveLRRHToPath (pLRRHXLocation,
						pLRRHYLocation, 
						nBakeryXCoord, 
						nBakeryYCoord,
						&nWolfXCoord, 
						&nWolfYCoord,
						&ActionQueue, 
						pNumOfSenses, 
						pNumOfRotations, 
						pNumOfMovements, 
						pNumOfTurns,
						nGridSize, 
						pPlayGrid,
						pWalkedTiles,	
						pSensedTiles,	
						pHasFlower, 
						pHasBread, 
						pVisitedWoodsman, 
						&nWasBreadTaken,
						pLRRHViewDirection, 
						pPreviousWalkedTileValue, 
						nStepByStepMode, 
						nHasSounds);
			}		
		}
		
                //After going to the bakery, then allow LRRH to go to Granny's house now
		*(pSensedTiles + nGrannyXCoord * nGridSize + nGrannyYCoord) = GRANDMA_TILE_VALUE;
		
		generateAStarPath (*pLRRHXLocation,
				        *pLRRHYLocation, 
					nGrannyXCoord, 
					nGrannyYCoord, 
					pSensedTiles,
                                        pPlayGrid, 
					nGridSize, 
					&ActionQueue, 
					&nIsDestinationValid);
		
		if (nIsDestinationValid){
			moveLRRHToPath (pLRRHXLocation,
					pLRRHYLocation, 
					nGrannyXCoord, 
					nGrannyYCoord,
					&nWolfXCoord, 
					&nWolfYCoord,
					&ActionQueue, 
					pNumOfSenses, 
					pNumOfRotations, 
					pNumOfMovements, 
					pNumOfTurns,
					nGridSize, 
					pPlayGrid,	
					pWalkedTiles, 
					pSensedTiles,			
					pHasFlower, 
					pHasBread, 
					pVisitedWoodsman, 
					&nWasBreadTaken,
					pLRRHViewDirection, 
					pPreviousWalkedTileValue, 
					nStepByStepMode, 
					nHasSounds);
		}
	}
}

/* This function makes LRRH do actions randomly around the playing grid
        @param   (int *)  pLRRHXLocation is the pointer to nLRRHXLocation (LRRH's X Location)
        @param   (int *)  pLRRHYLocation is the pointer to nLRRHYLocation (LRRH's Y Location)

        @param   (char *)  pLRRHViewDirection is the pointer to cLRRHviewDirection (LRRH's viewing direction)
        @param   (char *)  pPreviousWalkedTileValue is the pointer to cPreviousWalkedTileValue 
                (The previous value of the tile LRRH is standing)

        @param   (char *)  pPlayGrid is the pointer to carrPlayGrid

        @param   (int *)  pNumOfSenses is the pointer to nNumOfSenses (Keeps track of LRRH's sense count)
        @param   (int *)  pNumOfRotations is the pointer to nNumOfRotations (Keeps track of LRRH's rotation count)
        @param   (int *)  pNumOfMovements is the pointer to nNumOfMovements (Keeps track of LRRH's movement count)
        @param   (int *)  pNumOfTurns is the pointer to nNumOfTurns (Keeps track of LRRH's turn count)

        @param   (int)  nGridSize is the size of the playing grid         

        @param   (int *)  pHasFlower is the pointer to nHasFlower 
                (Tells whether or not LRRH already has went to a flower field)
        @param   (int *)  pHasBread is the pointer to nHasBread 
                (Tells whether or not LRRH already has went to a bakery and the bread has not been eaten yet)
        @param   (int *)  pVisitedWoodsman is the pointer to nHasBread 
                (Tells whether or not LRRH already has visited the woodsman's house)

        @param   (int)  nStepByStepMode tells whether or not to wait for user input until the next turn
        @param   (int)  nHasSounds tells whether or not the application should play sound for each turn 
        
        @return  (void) no return value
*/
void
runRandomLRRH (int * pLRRHXLocation,
                int * pLRRHYLocation, 
                int nGridSize,
                char * pLRRHViewDirection,
                char * pPreviousWalkedTileValue,
                char * pPlayGrid,
                int * pNumOfSenses,
                int * pNumOfRotations,
                int * pNumOfMovements,
                int * pNumOfTurns,
                int * pHasFlower,
                int * pHasBread,
                int * pVisitedWoodsman,
                int nStepByStepMode,
                int nHasSounds)
{

        //Used to tell whether or not LRRH's bread was taken by the wolf.
        int nWasBreadTaken = 0;

        //Holds a random number that will decide LRRH's next course of action
        int nRandomNumber = 0;
        while (1){

                //Generate a random number from 0 to 100
                nRandomNumber = generateRandomNumber(0, 100);

                //get LRRH's current location
                char * pLRRHTile = (pPlayGrid +  (*pLRRHXLocation) * nGridSize +  (*pLRRHYLocation));



                /*LRRH will perform a different action depending on the range of values generated by the RNG:
                0 - 16: Rotate to the left
                17 - 50: Move forward (If possible, else, do nothing)
                51 - 67: Rotate to the right
                68 - 100: Sense the tile she is facing*/

                if (nRandomNumber >= 0 && nRandomNumber <= 8)
                        rotateLeftLRRH(pNumOfRotations, pLRRHViewDirection, pLRRHTile);
                else if (nRandomNumber >= 9 && nRandomNumber <= 61) {
                        //Convert the character Direction to X and Y components
                        int nXDirection = convertDirectionSymbolToXMovement (*pLRRHViewDirection);
                        int nYDirection = convertDirectionSymbolToYMovement (*pLRRHViewDirection);

                        //Check first whether or not it is possible to move onto the tile before doing so.
                        if (isLocationValid(nXDirection + *pLRRHXLocation, nYDirection + *pLRRHYLocation, nGridSize - 1)){
                                *pPreviousWalkedTileValue = forwardLRRH(pNumOfMovements, 
                                                                        pLRRHXLocation, 
                                                                        pLRRHYLocation, 
                                                                        pLRRHTile, 
                                                                        pLRRHViewDirection, 
                                                                        nGridSize, 
                                                                        *pPreviousWalkedTileValue);

                                //Perform a switch case after moving to check whether or not LRRH has reached a special tile.
                                switch (*pPreviousWalkedTileValue){
                                case FLOWER_TILE:
                                *pHasFlower = 1;
                                        break;
                                case BAKERY_TILE:
                                        *pHasBread = 1;
                                        nWasBreadTaken = 0;
                                        break;
                                case WOODSMAN_TILE:
                                        *pVisitedWoodsman = 1;
                                                
                                        /*Proceed to calculate the distance from the woodsman and grandma's house 
                                        and display it to the user.*/
                                        int nWoodsmanDistance = computeWoodsmanDistance (pPlayGrid, nGridSize);
                                        displayWoodsmanLocation (nWoodsmanDistance);
                                        break;			
                                case WOLF_TILE:
                                        
                                        /*If LRRH is on the wolf tile, check whether or not she has bread
                                        If she does, she just loses the bread and cannot go to the wolf again or else,
                                        she will die and the game is lost.
                                        */
                                        if (*pHasBread){

                                                *pHasBread = 0;
                                                nWasBreadTaken = 1;
                                                displayWolfEatingBread ();
                                        } else 
                                                nWasBreadTaken = 0;
                                        break;
                                }
                        } else {
                                //If LRRH cannot move forward, rotate instead
                                rotateRightLRRH (pNumOfRotations, pLRRHViewDirection, pLRRHTile);
                        }
		} else if (nRandomNumber >= 62 && nRandomNumber <= 67)
                        rotateRightLRRH (pNumOfRotations, pLRRHViewDirection, pLRRHTile);
                else
                        senseLRRH (pNumOfSenses, pLRRHXLocation, pLRRHYLocation, pLRRHTile, pLRRHViewDirection, nGridSize);
                

                //Display the play grid to the user after every turn
                displayPlayGrid (pPlayGrid, nGridSize, *pNumOfSenses, *pNumOfRotations, *pNumOfMovements);
                waitForNextTurn (pNumOfTurns, nStepByStepMode, nHasSounds);
                        
                
                
                /*After performing an action, check first if you have triggered a tile event	
		If ANY OF THE POSSIBLE ENDINGS HAS OCCURED, BREAK FROM THE LOOP */
		int nHasScenarioHappened =  (*pPreviousWalkedTileValue == PIT_TILE ||
					        *pPreviousWalkedTileValue == GRANDMA_TILE ||
					        (*pPreviousWalkedTileValue == WOLF_TILE && !nWasBreadTaken));
		
		if (nHasScenarioHappened) 
                        break;
        }
}


/* This function makes LRRH perform actions intelligently in the playing grid to achieve her goal.

        @param   (char)  cPreviousWalkedTileValue is the previous value of the tile LRRH is standing on

        @param   (int)  nNumOfSenses keeps track of LRRH's sense count
        @param   (int)  nNumOfRotations keeps track of LRRH's rotation count
        @param   (int)  nNumOfMovements keeps track of LRRH's movement count

        @param   (int)  nGridSize is the size of the playing grid         

        @param   (int)  nHasFlower tells whether or not LRRH already has went to a flower field
        @param   (int)  nHasBread tells whether or not LRRH already has went to a bakery and the bread has not been eaten yet
        @param   (int)  nVisitedWoodsman tells whether or not LRRH already has visited the woodsman's house
        
        @return  (void) no return value
*/
void 
displayEndGameMessage(char cPreviousWalkedTileValue,
                        int nNumOfSenses, 
                        int nNumOfRotations, 
                        int nNumOfMovements, 
                        int nHasBread,
                        int nHasFlower,
                        int nVisitedWoodsman)
{
        //Check the LAST previously walked tile in case something special has happened  (LRRH dies, visits grandma)
	switch (cPreviousWalkedTileValue){
	case PIT_TILE:
		//LRRH falls to a pit
		displayPitDeathGameOverMessage (nNumOfSenses, nNumOfRotations, nNumOfMovements);				
		break;
	case GRANDMA_TILE:
		//If LRRH: has Bread, has Flowers, has met the woodsman, you win. else, you lose			
		if (nHasFlower && nHasBread && nVisitedWoodsman) 
                        displayGrannyWinGameOverMessage (nNumOfSenses, nNumOfRotations, nNumOfMovements);
		else{
			displayGrannyDeathGameOverMessage (nNumOfSenses,
							        nNumOfRotations, 
								nNumOfMovements, 
								nHasBread, 
								nHasFlower, 
								nVisitedWoodsman);				
		}
		break;
	case WOLF_TILE:
		//LRRH gets eaten
		displayWolfDeathGameOverMessage (nNumOfSenses, nNumOfRotations, nNumOfMovements);
		break;
	default:
                //LRRH was not able to get to grandma's house after checking everywhere.
		displayTrappedGameOverMessage (nNumOfSenses, nNumOfRotations, nNumOfMovements);
		break;
	}
}

/* This function is used for scanning numbers in the application
        
        @return  (int) returns -1 if the input is invalid, and the scanned number if it is valid
*/
int
scanNumber ()
{
        //Array used to store the user input
        char carrInputString[MAX_INPUT_LENGTH] = {0};

        //Scan the user input
        scanf (" %s", carrInputString);

        //Used to hold the pointer for the end of the string
        char * pEndStringPointer;
        
        //Use the strtol function to convert the inputted string into a long number in base 10
        //strol function(const char * _String, char ** _EndPtr, int _Radix)

        //lCharacterLongValue holds the number part of the string, and pEndStringPointer holds the non-number part
        long lCharacterLongValue = strtol (&carrInputString[0], &pEndStringPointer, 10);

        /*If the non-number part of the string is found at the start of the array OR
          If at the non-number part of the string, it holds some kind of value (meaning it is not null),
          Do not accept the input. (return -1 as invalid). Else, cast it to an int and return the number part.*/
        if ((pEndStringPointer == &carrInputString[0]) ||  (*pEndStringPointer != '\0'))
                return -1;
        else
                return  (int) lCharacterLongValue;
        
}

/* This function is used for scanning characters in the application
        
        @return  (char) returns 'x' if the input is invalid, and the scanned character if it is valid
*/
char
scanCharacter ()
{
        //Array used to store the user input
        char carrInputString[MAX_INPUT_LENGTH] = {0};

        //Scan the user input
        scanf (" %s", carrInputString);

        //Used to get the length of the string
        int nStringLength = strlen (carrInputString);

        //If the length of the string is just 1, then it is a character input. Else, return '\0' as invalid
        if (nStringLength == 1)
                return  carrInputString[0];
        else
                return  '\0';
        
}