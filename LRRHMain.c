/*
* Programmed By: Aaron Gabrielle Dichoso 
*
* Description:
* 	LRRHMain.c: The main file of A Machine Project made for the completion requirements of CCPROG1. 
* 
* 	Make an Intelligent Little Red Riding Hood.
* 	Features (From https://dlsu.instructure.com/courses/77441/assignments/628555):
* 	1. Can sense what element is in front of her: 
*		breeze if there is a pit, 
*		howl if there is a Wolf, 
*		scent if there is a flower, 
*		heat if there is a bakeshop, 
*		sound if there is a Woodsman, 
*		light if it is the house of Granny, 
*		none if there is no element
*	2. Can move forward, rotate left or rotate right.
*	3. Can meet the Woodsman who can tell her how many steps are there to reach Granny's house 
*		but can't tell her which direction the house would be. 
*		The Woodsman can walk with her towards Granny's house. 
*		She can only meet the Woodsman if she steps on a square where the Woodsman is.
*	4. Can meet the Wolf which may or may not eat her. 
*		She will be eaten by the Wolf if she is only carrying a flower or nothing. 
*		She will not be eaten by the Wolf is she is carrying some bread. 
*		If she has it, the Wolf will eat it and will let her walk in the woods.
*	5. Can fall on a pit and die if she steps on a square with the pit
*	6. Can reach a bakeshop to buy some bread. She can only buy some bread if she steps on a square where the bakeshop is.
*	7. Can pick up some flowers. She can only pick a flower if she steps on a square with flowers.
*	8. Can reach her Granny.
* 
* 	Win Conditions: LRRH should reach Granny's house. However, she should first: Pick a flower, Buy bread, and visit the woodsman.
* 	Lose Conditions: LRRH will lose if: 
* 	1. Visit Granny's house without a flower, bread, or visiting the woodsman.
* 	2. Go to the Wolf's tile.
* 	3. Go to the Pit's tile.
*
* Last Modified: 04/02/2022
* Acknowledgements: 
*       <stdlib.h> library - https://www.tutorialspoint.com/c_standard_library/stdlib_h.htm
*       <time.h> library - https://www.tutorialspoint.com/c_standard_library/time_h.htm
*       <stdio.h> library - https://www.tutorialspoint.com/c_standard_library/stdio_h.htm
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "LRRHMenu.c"

/*MAIN FUNCTION!!!*/
int 
main()
{		
        //Set the seed for the RNG at the start
        srand(time(NULL));

        //Application settings
	int nJumboMode = 0;
	int nStepByStepMode = 0;
	int nHasSounds = 1;
        int nSmartMode = 1;

        //Display the main menu
	displayMainMenu(&nJumboMode, &nStepByStepMode, &nHasSounds, &nSmartMode);
	
	return 0;
}
