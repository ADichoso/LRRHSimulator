/*
* Programmed By: Aaron Gabrielle Dichoso 
*
* Description:
* 	GUIFunctions_prototype.h: Contains the function prototypes of GUIFunctions.c
*
* Last Modified: 28/01/2022
*/

const char *    directionSymbolToName                   (char cSymbol);
const char *    objectSymbolToName                      (char cSymbol);
const char *    senseSymbolToName                       (char cSymbol);
void            displayPlayGrid                         (char * pPlayGrid,
                                                        int nGridSize, 
                                                        int nNumOfSenses, 
                                                        int nNumOfRotations, 
                                                        int nNumOfMovements);
void            displayPlayGridWithSense                (char * pPlayGrid,
                                                        int nGridSize, 
                                                        int nNumOfSenses, 
                                                        int nNumOfRotations, 
                                                        int nNumOfMovements, 
                                                        char cLRRHSense);
void            displayPlayAndPathGrid                  (char * pPlayGrid, 
                                                        char * pPathGrid,
                                                        int nGridSize,
                                                        int narrActionsArray[], 
                                                        int nNumOfActions);
void            displayStats                            (int nNumOfRotations, 
                                                        int nNumOfMovements, 
                                                        int nNumOfSenses);
void            displayLegend                           ();
void            displayMovementMessage                  (char cSymbol);
void            displaySenseMessage                     (char cSymbol);
void            displayTrappedGameOverMessage           (int nNumOfSenses, 
                                                        int nNumOfRotations, 
                                                        int nNumOfMovements);
void            displayPitDeathGameOverMessage          (int nNumOfSenses, 
                                                        int nNumOfRotations, 
                                                        int nNumOfMovements);
void            displayWolfDeathGameOverMessage         (int nNumOfSenses, 
                                                        int nNumOfRotations, 
                                                        int nNumOfMovements);
void            displayGrannyDeathGameOverMessage       (int nNumOfSenses, 
                                                        int nNumOfRotations, 
                                                        int nNumOfMovements, 
                                                        int nHasBread, 
                                                        int nHasFlower, 
                                                        int nMetWoodsman);
void            displayGrannySurroundedGameOvermessage  (int nNumOfSenses, 
                                                        int nNumOfRotations, 
                                                        int nNumOfMovements);
void            displayGrannyWinGameOverMessage         (int nNumOfSenses, 
                                                        int nNumOfRotations, 
                                                        int nNumOfMovements);
void            displayIntroduction                     ();
void            displayFoundBakery                      ();
void            displayFoundFlower                      ();
void            displayFoundWoodsman                    ();
void            displayWoodsmanSavesGrandma             ();
void            displayVisitingGrandma                  ();
void            displayGameOverHeader                   ();
void            displayYouWinHeader                     ();
void            displayBadEndFooter                     ();
void            displayGoodEndFooter                    ();
void            displayDivider                          ();
void            displayLargeDivider                     ();