/*
* Programmed By: Aaron Gabrielle Dichoso 
*
* Description:
* 	SoundSystem.c: A program file that is responsible for all of the audio in the application
*
* Last Modified: 04/02/2022
* Acknowledgements: 
*       Playing Sounds using Beep - https://www.geeksforgeeks.org/beep-function-in-c-with-examples/
*/

#include "SoundSystem_prototype.h"

/* This function plays a beep.

        @return  (void) no return value
*/
void 
playAlertSound()
{
	Beep(750, 500);
}