/*
* Programmed By: Aaron Gabrielle Dichoso 
*
* Description:
* 	LRRHGame_prototype.h: Contains the function prototypes of LRRHGame.c
* Last Modified: 28/01/2022
*/



//FUNCTION PROTOTYPES
void    getGridSize             (int * pGridSize,
                                int nJumboMode, 
                                int nHasSounds);

void    initializeObjects       (char * pPlayGrid, 
                                int nGridSize,
                                int nJumboMode, 
                                int nHasSounds);

void    runGame                 (int nJumboMode, 
                                int nStepByStepMode, 
                                int nHasSounds,
                                int nSmartMode);
void    runSmartLRRH            (int * pLRRHXLocation,
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
                                int nHasSounds);
void    runRandomLRRH           (int * pLRRHXLocation,
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
                                int nHasSounds);
void    displayEndGameMessage   (char cPreviousWalkedTileValue,
                                int nNumOfSenses, 
                                int nNumOfRotations, 
                                int nNumOfMovements, 
                                int nHasBread,
                                int nHasFlower,
                                int nVisitedWoodsman);
int     scanNumber              ();
char    scanCharacter           ();