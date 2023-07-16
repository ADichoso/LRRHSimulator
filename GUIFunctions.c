/*
* Programmed By: Aaron Gabrielle Dichoso 
*
* Description:
* 	GUIFunctions.c: Contains mostly all of the functions used for the GUI of the application.
*
* Last Modified: 04/02/2022
*/

#include "SoundSystem.c"
#include "GUIFunctions_prototype.h"

#define DIRECTION_NORTH 'N'
#define DIRECTION_EAST 'E'
#define DIRECTION_WEST 'W'
#define DIRECTION_SOUTH 'S'

#define LRRH_DIRECTION_NORTH '^'
#define LRRH_DIRECTION_EAST '>'
#define LRRH_DIRECTION_WEST '<'
#define LRRH_DIRECTION_SOUTH 'v'

#define EMPTY_TILE '_'
#define UNWALKABLE_PATH_TILE 'x'
#define SENSED_PATH_TILE 'o'
#define EMPTY_SENSE_TILE 'n'
#define GRANDMA_TILE 'G'
#define GRANDMA_SENSE_TILE 'l'
#define WOLF_TILE 'W'
#define WOLF_SENSE_TILE 'h'
#define BAKERY_TILE 'B'
#define BAKERY_SENSE_TILE 'H'
#define PIT_TILE 'P'
#define PIT_SENSE_TILE 'b'
#define WOODSMAN_TILE 'M'
#define WOODSMAN_SENSE_TILE 's'
#define FLOWER_TILE 'F'
#define FLOWER_SENSE_TILE 'c'

/*This gives the name of the direction given its character symbol on screen
        @param   (char)  cSymbol is the associated symbol with the direction
     
        @return  (const char *) The name of the direction
*/
const char * 
directionSymbolToName (char cSymbol)
{
	switch (cSymbol){
	case DIRECTION_NORTH:
		return "North";
	case DIRECTION_EAST:
		return "East";
	case DIRECTION_WEST:
		return "West";
	case DIRECTION_SOUTH:
		return "South";
	default:
		return "Nothing";
	}
}

/* This gives the name of the object given its character symbol on screen
        @param   (char)  cSymbol is the associated symbol with the object
     
        @return  (const char *) The name of the object
*/
const char * 
objectSymbolToName (char cSymbol)
{
	switch (cSymbol){
	case PIT_TILE:
		return "Pit";
	case BAKERY_TILE:
		return "Bakeshop";
	case FLOWER_TILE:
		return "Flower";
	case GRANDMA_TILE:
		return "Granny";
	case WOLF_TILE:
		return "Wolf";
	case WOODSMAN_TILE:
		return "Woodsman";
	default:
		return "Forest";
	}
}

/* This gives the name of the sense LRRH felt given its character symbol on screen
        @param   (char)  cSymbol is the associated symbol with the object
     
        @return  (const char *) The name of the object
*/
const char * 
senseSymbolToName (char cSymbol)
{
	switch (cSymbol){
	case PIT_SENSE_TILE:
		return "Upward BREEZE";
	case BAKERY_SENSE_TILE:
		return "Warm HEAT";
	case FLOWER_SENSE_TILE:
		return "Fragrant SCENT";
	case GRANDMA_SENSE_TILE:
		return "Faint LIGHT";
	case WOLF_SENSE_TILE:
		return "Terrifying HOWL";
	case WOODSMAN_SENSE_TILE:
		return "SOUND of Chopping Wood";
	default:
		return "Nothing in particular";
	}
}

/* This function displays the playing grid to the user
        @param   (char *)  pPlayGrid is the pointer to carrPlayGrid
        @param   (int)  nGridSize is the size of the playing grid
        @param   (int)  nNumOfSenses is the current number of senses LRRH has done
        @param   (int)  nNumOfRotations is the current number of rotations LRRH has done
        @param   (int)  nNumOfMovements is the current number of movements LRRH has done
        
        @return  (void) no return value 
*/

void 
displayPlayGrid (char * pPlayGrid, int nGridSize, int nNumOfSenses, int nNumOfRotations, int nNumOfMovements)
{
	char cCurrentTile;
	int i;
	
	printf ("\n");
	displayDivider ();
	printf ("\n");
	for (i = 0; i < nGridSize; i++)
        {
		
		int j;
		for (j = 0; j < nGridSize; j++)
                {
			cCurrentTile = * (pPlayGrid + j * nGridSize + i);
			printf ("[%c]", cCurrentTile);
		}	
                
		printf ("\n");
	}
	displayDivider ();
	displayStats (nNumOfSenses, nNumOfRotations, nNumOfMovements);
	displayLegend ();
}

/* This function displays the playing grid to the user, together with her current sense
        @param   (char *)  pPlayGrid is the pointer to carrPlayGrid
        @param   (int)  nGridSize is the size of the playing grid
        @param   (int)  nNumOfSenses is the current number of senses LRRH has done
        @param   (int)  nNumOfRotations is the current number of rotations LRRH has done
        @param   (int)  nNumOfMovements is the current number of movements LRRH has done
        
        @return  (void) no return value 
*/

void 
displayPlayGridWithSense (char * pPlayGrid,
                                int nGridSize, 
                                int nNumOfSenses, 
                                int nNumOfRotations, 
                                int nNumOfMovements, 
                                char cLRRHSense)
{
	displayPlayGrid (pPlayGrid, nGridSize, nNumOfSenses, nNumOfRotations, nNumOfMovements);

        printf("\nCurrent Sense: %s\n", senseSymbolToName(cLRRHSense));
        displayDivider();
        printf("\n");
}


/* This function displays the playing grid to the user, together with the path grid which shows LRRH's A* path generation
        @param   (char *)  pPlayGrid is the pointer to carrPlayGrid (Contains the main play grid of the game)
        @param   (char *)  pPathGrid is the pointer to carrPathGrid (Used at LRRHFunctions -> generateAStarPath)
        @param   (int)  nGridSize is the size of the playing grid
        @param   (int [])  narrActionsArray is the actions queued inside the Action Queue
        @param   (int)  nNumOfActions is the length of the narrActionsArray
        
        @return  (void) no return value
*/
void 
displayPlayAndPathGrid (char * pPlayGrid, char * pPathGrid, int nGridSize, int narrActionsArray[], int nNumOfActions)
{
	char cCurrentTile;
	int i;
	
	printf ("\n");
	displayDivider ();
	printf ("\n");
	for (i = 0; i < nGridSize; i++)
        {
		
		int j;
		for (j = 0; j < nGridSize; j++)
                {
			cCurrentTile = * (pPlayGrid + j * nGridSize + i);
			printf ("[%c]", cCurrentTile);
		}
		printf (" | ");

                for (j = 0; j < nGridSize; j++)
                {
			cCurrentTile = * (pPathGrid + j * nGridSize + i);
			printf ("[%c]", cCurrentTile);
		}

		printf ("\n");
	}
        displayDivider ();
        printf ("\nLeft: The Play Grid | Right: LRRH's Path Generation\n");

        printf( "LRRH's Path: ");
        for (i = 0; i < nNumOfActions; i++)
        {
		printf ("%s", directionSymbolToName((char) narrActionsArray[i]));

                if(i % 8 == 0)
                        printf("\n");
                if (i != nNumOfActions - 1)
                        printf ("-");
	}
        
        printf ("\n");

	displayDivider ();
}


/* This function displays the total number of senses, rotations, and movements of LRRH to the user
        @param   (int)  nNumOfSenses is the total number of senses done by LRRH
        @param   (int)  nNumOfRotations is the total number of rotations done by LRRH
        @param   (int)  nNumOfMovements is the total number of movements done by LRRH

        @return  (void) no return value
*/
void 
displayStats (int nNumOfSenses, int nNumOfRotations, int nNumOfMovements)
{
	printf ("\n");
	displayDivider ();
	printf ("\n| Senses: %d | Rotations: %d | Movements: %d |\n", nNumOfSenses, nNumOfRotations, nNumOfMovements);
	displayDivider ();
        printf ("\n");
}

/* This function displays a legend to guide the user which elements on the play grid are which.

        @return  (void) no return value
*/
void 
displayLegend ()
{
	printf ("\n");
	displayDivider ();
	printf ("\n|Legend: %c %c %c %c - Little Red Riding Hood|", LRRH_DIRECTION_NORTH, LRRH_DIRECTION_EAST, LRRH_DIRECTION_SOUTH, LRRH_DIRECTION_WEST);
	printf ("\n|%c - Pit, %c - Flower, %c - Bakery|", PIT_TILE, FLOWER_TILE, BAKERY_TILE);
	printf ("\n|%c - Wolf, %c - Woodsman, %c - Granny|\n", WOLF_TILE, WOODSMAN_TILE, GRANDMA_TILE);
	displayDivider ();
}

/* This displays a movement message depending on the tile that LRRH moves into
        @param   (char)  cSymbol is the associated symbol with the object

        @return  (void) no return value
*/
void 
displayMovementMessage (char cSymbol)
{
	printf ("\nLittle Red Riding Hood moves toward the %s\n", objectSymbolToName (cSymbol));
	
        switch (cSymbol){
	case EMPTY_TILE:
		printf ("\nLittle Red Riding Hood moves somewhere new into the forest\n");
		break;
	case PIT_TILE:
		printf ("\nLittle Red Riding Hood spots a pit. She accidentally slips...\n");
		break;
	case BAKERY_TILE:
		printf ("\nLittle Red Riding Hood visits the bakery and buys a piece of bread.\n");
		break;
	case FLOWER_TILE:
		printf ("\nLittle Red Riding Hood visits a field of flowers and picks one for her grandmother.\n");
		break;
	case GRANDMA_TILE:
		printf ("\nLittle Red Riding Hood visits her grandmother's house.\n");
		break;
	case WOLF_TILE:
		printf ("\nLittle Red Riding Hood is seen by the hungry wolf. Trembling with fear, the wolf slowly approaches her...\n");
		break;
	case WOODSMAN_TILE:
		printf ("\nLittle Red Riding Hood meets the lumberjack.\nThe lumberjack joins Little Red Riding Hood on her way to her grandmother's house.\n");
		break;
	}
}

/* This displays a sense message depending on the tile that LRRH senses
        @param   (char)  cSymbol is the associated symbol with the object
	 
        Pit - LRRH feels a breeze
        Wolf - LRRH hears a howl
        Flower - LRRH smells a nice scent
        Bakery - LRRH feels a warm heat
        Woodsman / Lumberjack - LRRH hears the sound of the chopping of wood 
        Granny - LRRH sees the light in Granny's house
        Nothing - nothing.

        @return  (void) no return value
*/
void 
displaySenseMessage (char cSymbol)
{	
	printf ("\nLittle Red Riding Hood is now checking the tile in front of her.");
		
	switch (cSymbol){
	case EMPTY_TILE:
		printf ("\nLittle Red Riding Hood sees NOTHING in particular.\n");
		break;
	case PIT_TILE:
		printf ("\nLittle Red Riding Hood feels a BREEZE blowing upwards. She turns away and moves on.\n");
		break;
	case BAKERY_TILE:
		printf ("\nLittle Red Riding Hood feels the warm HEAT.\n");
		break;
	case FLOWER_TILE:
		printf ("\nLittle Red Riding Hood smells a nice SCENT towards her.\n");
		break;
	case GRANDMA_TILE:
		printf ("\nLittle Red Riding Hood sees a faint LIGHT in a wooden cabin.\n");
		break;
	case WOLF_TILE:
		printf ("\nLittle Red Riding Hood is frightened by a bone-chilling HOWL. She slowly and quietly leaves the vicinity.\n");
		break;
	case WOODSMAN_TILE:
		printf ("\nLittle Red Riding Hood hears the SOUND of an axe, chopping wood.\n");
		break;
	}
}

/* This displays a rotation message when she rotates to the right
        @param   (char)  cDirection is the direction that LRRH turns to

        @return  (void) no return value
*/
void 
displayRightRotationMessage (char cDirection)
{
	//Print the current rotation and number of rotations
	printf ("\nLittle Red Riding Hood turns to her right.");
	printf ("\nLittle Red Riding Hood is now looking to the %s.", directionSymbolToName (cDirection));
}

/* This displays a rotation message when she rotates to the left
        @param   (char)  cDirection is the direction that LRRH turns to

        @return  (void) no return value
*/
void 
displayLeftRotationMessage (char cDirection)
{
	//Print the current rotation and number of rotations
	printf ("\nLittle Red Riding Hood turns to her left.");
	printf ("\nLittle Red Riding Hood is now looking to the %s.", directionSymbolToName (cDirection));
}

/* This displays the distance between the woodsman and grandmother's house
        @param   (int)  nWoodsmanDistance is the distance between the woodsman and grandmother's house

        @return  (void) no return value
*/
void 
displayWoodsmanLocation (int nWoodsmanDistance)
{
	printf ("\nThe lumberjack's house is currently %d tiles away from grandmother's house", nWoodsmanDistance);
}

/* This displays a message saying that the wolf has eaten LRRH's bread

        @return  (void) no return value
*/
void 
displayWolfEatingBread ()
{
	printf ("\nSuddenly, the wolf pounces on her! He eats her piece of bread and scurries away.\n");
}

/* This displays a message saying that the path LRRH is taking to grandmother's house has been blocked
        @param   (int)  nNumOfSenses is the total number of senses done by LRRH
        @param   (int)  nNumOfRotations is the total number of rotations done by LRRH
        @param   (int)  nNumOfMovements is the total number of movements done by LRRH

        @return  (void) no return value
*/
void 
displayTrappedGameOverMessage (int nNumOfSenses, int nNumOfRotations, int nNumOfMovements)
{
	printf ("\n\n");
	displayLargeDivider ();
	
	displayGameOverHeader ();
	displayStats (nNumOfSenses, nNumOfRotations, nNumOfMovements);
		
	printf ("Little Red Riding Hood planned to visit her ill grandmother this morning.\n");
	printf ("She takes her usual path through the forest.\n");
	printf ("However, due to unforseen circumstances, her route was blocked by dangers surrounding it.\n");
	printf ("Because of this, Little Red cannot save her grandmother..");
	
	displayBadEndFooter ();
	displayLargeDivider ();
	printf ("\n\n\n\n\n");
}

/* This displays a message saying that LRRH has fallen into a pit and has died
        @param   (int)  nNumOfSenses is the total number of senses done by LRRH
        @param   (int)  nNumOfRotations is the total number of rotations done by LRRH
        @param   (int)  nNumOfMovements is the total number of movements done by LRRH

        @return  (void) no return value
*/
void 
displayPitDeathGameOverMessage (int nNumOfSenses, int nNumOfRotations, int nNumOfMovements)
{
	printf ("\n\n");
	displayLargeDivider ();
	
	displayGameOverHeader ();
	displayStats (nNumOfSenses, nNumOfRotations, nNumOfMovements);
		
	printf ("Little Red Riding Hood planned to visit her ill grandmother this morning.\n");
	printf ("She takes her usual path through the forest.\n");
	printf ("A sudden breeze blows off her hood. Little Red sees a pit directly beneath her.\n");
	printf ("In a moment, the ground under Little Red Riding Hood gives way. She slips and falls into a deep, dark pit.\n");
	printf ("No one heard from Little Red Riding Hood ever again...\n");
	
	displayBadEndFooter ();
	displayLargeDivider ();
	printf ("\n\n\n\n\n");
}

/* This displays a message saying that LRRH gets eaten by the wolf
        @param   (int)  nNumOfSenses is the total number of senses done by LRRH
        @param   (int)  nNumOfRotations is the total number of rotations done by LRRH
        @param   (int)  nNumOfMovements is the total number of movements done by LRRH

        @return  (void) no return value
*/
void 
displayWolfDeathGameOverMessage (int nNumOfSenses, int nNumOfRotations, int nNumOfMovements)
{
	printf ("\n\n");
	displayLargeDivider ();
	
	displayGameOverHeader ();
	displayStats (nNumOfSenses, nNumOfRotations, nNumOfMovements);
		
	printf ("Little Red Riding Hood planned to visit her ill grandmother this morning.\n");
	printf ("She takes her usual path through the forest.\n");
	printf ("A terrifying howl shakes Little Red's bones. It is the wolf.\n");
	printf ("The ravenous wolf pounces on Little Red Riding Hood.\n");
	printf ("No one heard from Little Red Riding Hood ever again...");
	
	displayBadEndFooter ();
	displayLargeDivider ();
	printf ("\n\n\n\n\n");
}

/* This displays a message saying that Grandmother dies as she dies either to the wolf or to disease
        @param   (int)  nNumOfSenses is the total number of senses done by LRRH
        @param   (int)  nNumOfRotations is the total number of rotations done by LRRH
        @param   (int)  nNumOfMovements is the total number of movements done by LRRH
        @param   (int)  nHasBread tells whether or not LRRH has picked up bread
        @param   (int)  nHasFlower tells whether or not LRRH has picked up a flower
        @param   (int)  nMetWoodsman tells whether or not LRRH has picked up the woodsman
        
        @return  (void) no return value
*/
void 
displayGrannyDeathGameOverMessage (int nNumOfSenses,
                                        int nNumOfRotations, 
                                        int nNumOfMovements, 
                                        int nHasBread, 
                                        int nHasFlower, 
                                        int nMetWoodsman)
{
	printf ("\n\n");
	displayLargeDivider ();
	
	displayGameOverHeader ();
	displayStats (nNumOfSenses, nNumOfRotations, nNumOfMovements);

	displayIntroduction ();
	
	if (nHasBread) 	
                displayFoundBakery ();
	if (nHasFlower) 
                displayFoundFlower ();
	if (nMetWoodsman) 
                displayFoundWoodsman ();
	
        //Wolf eats grandma
	displayVisitingGrandma ();

	if (nMetWoodsman){
		//Met woodsman, little red and grandma gets saved
		displayWoodsmanSavesGrandma ();

		//Without bread or the flower, however, grandma still dies
		if (!nHasBread) 
                        printf ("Without the bread, grandmother will stay hungry.\n");
		if (!nHasFlower) 
                        printf ("Without the flowers, grandmother's spirits will stay down.\n");
		
		printf ("Little Red's grandmother cannot hold on...\n\n");
	} else {
		//Did not meet woodsman, little red just dies
		printf ("Little Red Riding Hood, with no strength to fight back, gets eaten by the wolf.\n");
		printf ("The young girl and her grandmother were never to be seen again...");
	}
	
	displayBadEndFooter ();
	displayLargeDivider ();
	printf ("\n\n\n\n\n");
}

/* This displays a message saying that Grandmother lives and that LRRH has succeeded on her mission
        @param   (int)  nNumOfSenses is the total number of senses done by LRRH
        @param   (int)  nNumOfRotations is the total number of rotations done by LRRH
        @param   (int)  nNumOfMovements is the total number of movements done by LRRH
        
        @return  (void) no return value
*/
void 
displayGrannyWinGameOverMessage (int nNumOfSenses, int nNumOfRotations, int nNumOfMovements)
{
	printf ("\n\n");
	displayLargeDivider ();
	
	displayYouWinHeader ();
	displayStats (nNumOfSenses, nNumOfRotations, nNumOfMovements);
	
	printf ("Little Red Riding Hood planned to visit her ill grandmother this morning.\n");
	printf ("She takes her usual path through the forest.\n");
	
	displayFoundBakery ();
	displayFoundFlower ();
	displayFoundWoodsman ();

	//Wolf eats grandma
	displayVisitingGrandma ();


	//Met woodsman, little red and grandma gets saved
	displayWoodsmanSavesGrandma ();
	
	printf ("Little Red Riding gives the bread to her grandmother and feeds her.\n");
	printf ("She gives the bouquet of flowers to her grandmother to cheer her spirits.\n");
	
	
	printf ("With a full stomach and the courage to push through, Little Red's grandmother makes a steady recovery.\n");
	printf ("And they all lived happily ever after...\n\n");
	
	displayGoodEndFooter ();
	displayLargeDivider ();
	printf ("\n\n\n\n\n");
}

/* This displays a message introducing the story of LRRH
        
        @return  (void) no return value
*/
void 
displayIntroduction ()
{
	printf ("Little Red Riding Hood planned to visit her ill grandmother this morning.\n");
	printf ("She takes her usual path through the forest.\n\n");
}

/* This displays a message saying that LRRH bought some bread in the bakery
        
        @return  (void) no return value
*/
void 
displayFoundBakery ()
{
	printf ("On the way home, she visits the bakery and buys some bread for her grandmother.\n\n");	
}

/* This displays a message saying that LRRH picked out flowers in a flower field
        
        @return  (void) no return value
*/
void 
displayFoundFlower ()
{
	printf ("While traversing the forest, she spots a wolf.\n");
	printf ("The wolf, upon seeing the sad look on Little Red Riding Hood's face,\n"); 
	printf ("tells her to cheer up. He leads her to a flower field, %s", 
                "where the wolf tells her to pick flowers to boost her spirits.\n");
	printf ("Little did Little Red Riding Hood know that, the wolf meant to distract her.\n");
	printf ("The hungry wolf runs to her grandmother's house.\n");
	printf ("so to devour her whole.\n\n");
}

/* This displays a message saying that the woodsman joins LRRH on her way to grandmas house
        
        @return  (void) no return value
*/
void 
displayFoundWoodsman ()
{
	printf ("Little Red Riding Hood hears the sound of chopping wood.\n%s",
                "She visits the woodsman and he joins her to check up on the old woman.\n\n");	
}

/* This displays a message saying that the woodsman saves grandma from the wolf
        
        @return  (void) no return value
*/
void 
displayWoodsmanSavesGrandma ()
{
	printf ("The lumberjack spots the wolf. He uses the rifle to shoot and stop the wolf.\n");
	printf ("The lumberjack opens up the wolf's stomach and saves the grandmother.\n\n");
		
	printf ("However, grandmother's disease still persists.\n\n");
}

/* This displays a message saying that LRRH visits grandmother's house
        
        @return  (void) no return value
*/
void 
displayVisitingGrandma ()
{
	printf ("The wolf visits the old woman's home. He opens the door and, without a second for grandma to react, %s", 
                "swallows her whole.\n");
	printf ("The wolf dresses up as the grandma, and lies in bed, waiting for the Little Girl.\n");
	printf ("A lone light illuminates her grandmother's home. Little Red enters the cabin.\n");
	printf ("Little Red goes to her grandmother's bedroom to comfort her. However, she notices something unusual.\n");
        printf ("Little Red asks something to her supposed grandma:\n\n");

	printf ("\"Grandmother, what big arms you have!\"\n");	
	printf ("\"All the better to hug you with, my dear.\"\n");
	printf ("\"Grandmother, what big ears you have!\"\n");
	printf ("\"All the better to hear you with, my child.\"\n");
	printf ("\"Grandmother, what big eyes you have!\"\n");
	printf ("\"All the better to see you with, my child.\"\n");
	printf ("\"Grandmother, what big teeth you have got!\"\n");
	printf ("\"All the better to eat you up with.\"\n\n");
	
	printf ("The wolf opens his gaping mouth, ready to devour the Little Girl whole.\n\n");
}

/* This displays the losing message header
        
        @return  (void) no return value
*/
void 
displayGameOverHeader ()
{
	printf ("\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-GAME OVER-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n");
}

/* This displays the winning message header
        
        @return  (void) no return value
*/
void 
displayYouWinHeader ()
{
	printf ("\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=YOU WIN=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n");
}

/* This displays the losing message footer
        
        @return  (void) no return value
*/
void 
displayBadEndFooter ()
{
	printf ("\n\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=BAD END=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
}

/* This displays the winning message footer
        
        @return  (void) no return value
*/
void 
displayGoodEndFooter ()
{
	printf ("\n\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-GOOD END=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
}

/* This displays a divider to organize the GUI
        
        @return  (void) no return value
*/
void 
displayDivider (){
	printf ("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");
}

/* This displays a large divider to organize the GUI
        
        @return  (void) no return value
*/
void 
displayLargeDivider ()
{
	printf ("\n\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
}