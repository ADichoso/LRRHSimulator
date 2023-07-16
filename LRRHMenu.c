/*
* Programmed By: Aaron Gabrielle Dichoso 
*
* Description:
* 	LRRHMenu.c: Contains the functions used to display the main menu of the application, as well as it's settings
*                       and functions used to get user input with error handling.
*
* Last Modified: 06/02/2022
* Acknowledgements:
*       Terminating Programs - https://www.tutorialspoint.com/c_standard_library/c_function_exit.htm
*       Length of a Character Array - https://www.programiz.com/c-programming/library-function/string.h/strlen
*       Checking if a string can be converted to a number - https://stackoverflow.com/questions/17292545/how-to-check-if-the-input-is-a-number-or-not-in-c
*       strtol() function - https://www.tutorialspoint.com/c_standard_library/c_function_strtol.htm
*       <string.h> library - https://www.tutorialspoint.com/c_standard_library/string_h.htm
*   
*/

#include "LRRHGame.c"
#include "LRRHMenu_prototype.h"

/* This function opens the main menu of the application
        @param   (int *)  pJumboMode is the pointer to nJumboMode
                (tells whether or not the grid will be standard or jumbo mode)
        @param   (int *)  pStepByStepMode is the pointer to nStepByStepMode
                (tells whether or not LRRH will move automatically)
        @param   (int *)  pHasSounds is the pointer to nHasSounds
                (tells whether or not to play sound)
        @param   (int *)  pSmartMode is the pointer to nSmartMode
                (tells whether or not LRRH will move intelligently or randomly)

        @return  (void) no return value
*/
void 
displayMainMenu (int * pJumboMode, int * pStepByStepMode, int * pHasSounds, int * pSmartMode)
{
        //Variable used to hold the user input
	char cChosenAction;
	
	while (1)
	{
                //Main Menu GUI
		printf ("\n-=-=-=-=-=-=-=-=-=Welcome to LRRH Simulator!=-=-=-=-=-=-=-=-=-\n");
		printf ("-=-Please enter the corresponding character to your selection-=-\n\n");
		printf ("=-=-=-=-=-=PLAY (P)-=-=-=-SETTINGS (S)-=-=-=-QUIT (Q)=-=-=-=-=-=\n");
		
		cChosenAction = scanCharacter();
		if (*pHasSounds) 
                        playAlertSound ();
		displayLargeDivider ();
		
		
		switch (cChosenAction){
		case 'P':
		case 'p':
			//Run the game
			runGame (*pJumboMode, *pStepByStepMode, *pHasSounds, *pSmartMode);
			break;
		case 'S':
		case 's':
			//Open the settings
			showSettings (pJumboMode, pStepByStepMode, pHasSounds, pSmartMode);
			break;
		case 'Q':
		case 'q':
			//Quit
			exit (0);
			break;
		default:
			printf ("\nPlease enter a valid character.\n\n\n");
		}
	}
}

/* This function opens the settings of the application
        @param   (int *)  pJumboMode is the pointer to nJumboMode
                (tells whether or not the grid will be standard or jumbo mode)
        @param   (int *)  pStepByStepMode is the pointer to nStepByStepMode
                (tells whether or not LRRH will move automatically)
        @param   (int *)  pHasSounds is the pointer to nHasSounds
                (tells whether or not to play sound)
        @param   (int *)  pSmartMode is the pointer to nSmartMode
                (tells whether or not LRRH will move intelligently or randomly)
        
        @return  (void) no return value
*/
void 
showSettings (int * pJumboMode, int * pStepByStepMode, int * pHasSounds, int * pSmartMode)
{
        //Variable used to hold the user input
	char cChosenAction;
	
	while (1)
	{
		printf ("\n-=-=-=-=-=-=-=-=-=-=-=-=-=Settings=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n");

		printf ("Enabled Jumbo Mode? (J): %c\n", getMode (*pJumboMode));
		printf ("Enabled Step-By-Step Mode? (S): %c\n", getMode (*pStepByStepMode));
		printf ("Enabled Sound Effects? (X): %c\n", getMode (*pHasSounds));
                printf ("Enabled Smart Mode? (I): %c\n", getMode (*pSmartMode));
		printf ("QUIT? (Q)\n");
		cChosenAction = scanCharacter();
		if (*pHasSounds)
                        playAlertSound ();

		displayLargeDivider ();
		
		
		switch (cChosenAction){
		case 'J':
		case 'j':
			//Enable Jumbo / Standard Mode
			setJumboMode (pJumboMode, *pHasSounds);
			break;
		case 'S':
		case 's':
			//Enable Step By Step / Fast Mode
			setStepByStepMode (pStepByStepMode, *pHasSounds);
			break;
		case 'X':
		case 'x':
			//Enable / Disable Sounds
			setSounds (pHasSounds);	
                        break;
                case 'I':
		case 'i':
			//Enable Smart / Random Mode
			setSmartMode (pSmartMode, *pHasSounds);	
                        break;		
		case 'Q':
		case 'q':
			//Quit and return to the main menu
			displayMainMenu (pJumboMode, pStepByStepMode, pHasSounds, pSmartMode);
			break;
		default:
			printf ("\nPlease enter a valid character.\n\n\n");
		}
	}
}

/* This function sets the step-by-step mode of the application
        @param   (int *)  pStepByStepMode is the pointer to nStepByStepMode
                (tells whether or not LRRH will move automatically)
        @param   (int)  nHasSounds tells whether or not to play sound
        
        @return  (void) no return value
*/
void 
setStepByStepMode (int * pStepByStepMode, int nHasSounds)
{
        //Variable used to hold the user input
        char cTempStepByStepMode;

	while (1)
	{
		
		displayDivider ();
		printf ("\nType (S) to enable step-by-step mode, (F) to enable fast mode, and (Q) to quit:\n");
		cTempStepByStepMode = scanCharacter();
		if (nHasSounds) 
                        playAlertSound ();

		if (cTempStepByStepMode == 'F' || cTempStepByStepMode == 'f'){
			//Enable fast mode
			*pStepByStepMode = 0;
			break;
		} else if (cTempStepByStepMode == 'S' || cTempStepByStepMode == 's') {
			//Enable step-by-step mode
			*pStepByStepMode = 1;
			break;
		} else if (cTempStepByStepMode == 'Q' || cTempStepByStepMode == 'q'){
                        //Just quit
                        break;
                } else 
                        printf ("\nPlease enter a valid character.\n");

	}
}

/* This function sets the jumbo mode of the application
        @param   (int *)  pJumboMode is the pointer to nJumboMode
                (tells whether or not the grid will be standard or jumbo mode)
        @param   (int)  nHasSounds tells whether or not to play sound
        
        @return  (void) no return value
*/
void 
setJumboMode (int * pJumboMode, int nHasSounds)
{
        //Variable used to hold the user input
        char cTempJumboMode;

	while (1)
	{
		
		displayDivider ();
		printf ("\nType (J) to enable jumbo mode, (S) to enable standard mode, and (Q) to quit:\n");
		
		cTempJumboMode = scanCharacter();
		if (nHasSounds) 
                        playAlertSound ();
		
		if (cTempJumboMode == 'J' || cTempJumboMode == 'j'){
			//Enable jumbo mode
			*pJumboMode = 1;
			break;
		} else if (cTempJumboMode == 'S' || cTempJumboMode == 's') {
			//Enable standard mode
			*pJumboMode = 0;
			break;
		} else if (cTempJumboMode == 'Q' || cTempJumboMode == 'q'){
                        //Just quit
                        break;                
                } else
			printf ("\nPlease enter a valid character.\n");
	}
}

/* This function sets the sound mode of the application
        @param   (int *)  pHasSounds is the pointer to nHasSounds
                (tells whether or not to play sound)
        
        @return  (void) no return value
*/
void setSounds (int * pHasSounds)
{
        //Variable used to hold the user input
        char cTempSounds;

	while (1)
	{
		
		displayDivider ();
		printf ("\nType (Y) to enable sounds, (N) to disable, and (Q) to quit:\n");
		
		cTempSounds = scanCharacter();
		if (*pHasSounds == 1) 
                        playAlertSound ();

		if (cTempSounds == 'Y' || cTempSounds == 'y'){
			//Enable Sounds
			*pHasSounds = 1;
			break;
		} else if (cTempSounds == 'N' || cTempSounds == 'n') {
			//Disable Sounds
			*pHasSounds = 0;
			break;
		} else if (cTempSounds == 'Q' || cTempSounds == 'q'){
                        //Just quit
                        break;
                } else 
                        printf ("\nPlease enter a valid character.\n");
	}
}


/* This function sets the step-by-step mode of the application
        @param   (int *)  pSmartMode is the pointer to nSmartMode
                (tells whether or not LRRH will move intelligently or randomly)
        @param   (int)  nHasSounds tells whether or not to play sound
        
        @return  (void) no return value
*/
void 
setSmartMode (int * pSmartMode, int nHasSounds)
{
        //Variable used to hold the user input
        char cTempSmartMode;

	while (1)
	{
		
		displayDivider ();
		printf ("\nType (S) to enable Smart mode, (R) to enable Random mode, and (Q) to quit:\n");
		cTempSmartMode = scanCharacter();
		if (nHasSounds) 
                        playAlertSound ();

		if (cTempSmartMode == 'R' || cTempSmartMode == 'r'){
			//Enable fast mode
			*pSmartMode = 0;
			break;
		} else if (cTempSmartMode == 'S' || cTempSmartMode == 's') {
			//Enable step-by-step mode
			*pSmartMode = 1;
			break;
		} else if (cTempSmartMode == 'Q' || cTempSmartMode == 'q'){
                        //Just quit
                        break;
                } else 
                        printf ("\nPlease enter a valid character.\n");

	}
}

/* This function sets the sound mode of the application
        @param   (int)  nCurrentMode is the boolean value of the mode chosen
        
        @return  (char) returns 'Y' is the mode is enabled, and 'N' if not
*/
char getMode (int nCurrentMode)
{
	if (nCurrentMode) 
                return 'Y';
	else 
                return 'N';
}