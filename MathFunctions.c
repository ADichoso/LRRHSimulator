/*
* Programmed By: Aaron Gabrielle Dichoso 
*
* Description:
* 	MathFunction.c: This file contains all of the general math and logic functions the application uses
*
* Last Modified: 04/02/2022
* Acknowledgements: 
*       <stdlib.h> library - https://www.tutorialspoint.com/c_standard_library/stdlib_h.htm
*/

#include "MathFunctions_prototype.h"
/* This function checks whether or not a location is valid / located in the playing grid.
        @param   (int)  nXLocation is the X location in the tile
        @param   (int)  nYLocation is the Y location in the tile
        @param   (int)  nGridSize is the size of the playing grid

        
        @return  (int) returns an integer  (1 for true, 0 for false)
*/
int 
isLocationValid (int nXLocation, int nYLocation, int nGridSize)
{
        //X and Y Values should be greater than or equal to 0 && less than or equal to grid size
	if ((nXLocation >= 0 && nYLocation >= 0 && nXLocation <= nGridSize && nYLocation <= nGridSize))
                return 1;
        else
                return 0;
}

/* This function calculates the distance of two points
        @param   (int)  x1 is the x position of the first point
        @param   (int)  x2 is the x position of the second point     
        @param   (int)  y1 is the y position of the first point     
        @param   (int)  y2 is the y position of the second point

     
        @return  (int) returns an integer which is the distance of the two points
*/
int 
getDistance (int x1, int y1, int x2, int y2)
{
        //A simple way to get the distance is to get the sum of the absolute values of the differences of the x and y coords
        //Ex: getDistance(1, 1, 4, 5) = abs (1 - 4) + abs (1 - 5) = abs(-3) + abs (-4) = 3 + 4 = 7
	return abs (x1 - x2) + abs (y1 - y2);
}


int 
generateRandomNumber (int nMin, int nMax)
{
        int nRange = nMax - nMin + 1;

        return rand() % nRange + nMin;
}