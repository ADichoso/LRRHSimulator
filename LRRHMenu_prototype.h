/*
* Programmed By: Aaron Gabrielle Dichoso 
*
* Description:
* 	LRRHMenu_prototype.h: Contains the function prototypes of LRRHMenu.c
* Last Modified: 28/01/2022
*/

#include <string.h>

void    displayMainMenu         (int * pJumboMode, 
                                int *pStepByStepMode, 
                                int * pHasSounds,
                                int * pSmartMode);
void    showSettings            (int * pJumboMode, 
                                int * pStepByStepMode, 
                                int * pHasSounds,
                                int * pSmartMode);
void    setStepByStepMode       (int * pStepByStepMode, 
                                int nHasSounds);
void    setJumboMode            (int * pJumboMode, 
                                int nHasSounds);
void    setSounds               (int * pHasSounds);
void    setSmartMode            (int * pSmartMode, 
                                int nHasSounds);
char    getMode                 (int nCurrentMode);