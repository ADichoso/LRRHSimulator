/*
* Programmed By: Aaron Gabrielle Dichoso 
*
* Description:
* 	QueueSystem.c: A queueing system to be used for the Machine Project. 
*       An Array is used to be able to dynamically store objects.
*
* Last Modified: 04/02/2022
* Acknowledgements: 
*	Pointers and 2D arrays - https://overiq.com/c-programming-101/pointers-and-2-d-arrays/
*       Dynamic Arrays - https://stackoverflow.com/questions/3536153/c-dynamically-growing-array
*       Reversing Arrays - https://www.techiedelight.com/reverse-array-c/
*       Selection Sorting Algorithm - https://www.geeksforgeeks.org/selection-sort/
*   
*/

#include "MathFunctions.c"
#include "QueueSystem_prototype.h"

/* This function initializes the queue array
        @param   (Queue *)  pQueue is the pointer to the selected queue
     
        @return  (void) no return value
*/
void 
initQueue (Queue * pQueue) 
{
        //Allocate memory big enough to store 1 integer into the array
        pQueue->pArray = malloc (1 * sizeof(int));
        pQueue->nCount = 0;
}


/* This function gets an element and equeues it to the end of the array
        @param   (Queue *)  pQueue is the pointer to the selected queue
        @param   (int)  nNewElement is the new element to be added to the array
     
        @return  (void) no return value
*/
void 
enqueue (Queue * pQueue, int nNewElement) 
{
        //Increment the capacity of the queue
        pQueue->nCount++;

        //Reallocate new memory for the increase in size
        pQueue->pArray = realloc (pQueue->pArray, pQueue->nCount * sizeof(int));

        //Add the new element to the end of the array
        pQueue->pArray[pQueue->nCount - 1] = nNewElement;
}

/* This function takes out an element that is inside an array
        @param   (Queue *)  pQueue is the pointer to the selected queue
     
        @return  (int) returns the dequeued element from the array
*/
int 
dequeue (Queue *pQueue)
{

        //Only dequeue an element if the capacity is greater than 0, else return a negative value
	if (pQueue->nCount > 0){

                //Get the value of the element in the front of the array
		int nDequeuedElement = pQueue->pArray[0];
		

                //Offset the array to make the elements in the back to the front.
		int i = 0;
		do
                {
		        pQueue->pArray[i] = pQueue->pArray[i + 1];
                        i++;
		} while (i < pQueue->nCount);

                //decrease the queue's capacity
		pQueue->nCount--;

                //Reallocate memory of the queue for the new size
		pQueue->pArray = realloc (pQueue->pArray, pQueue->nCount * sizeof(int));
		
                //Return the dequeued element
		return nDequeuedElement;
	} else
                return -1;	
}
/* This function reverses the order of elements in the queue
        @param   (Queue *)  pQueue is the pointer to the selected queue
     
        @return  (void) no return value
*/
void 
reverseQueue (Queue *pQueue)
{
	int tempArray[pQueue->nCount];
	
	int i;
        for (i = 0; i < pQueue->nCount; i++) 
        {
                tempArray[pQueue->nCount - 1 - i] = pQueue->pArray[i];
        }
        
        for (i = 0; i < pQueue->nCount; i++) 
        {
                pQueue->pArray[i] = tempArray[i];
        }
}

/* This function clears the queue and its elements
        @param   (Queue *)  pQueue is the pointer to the selected queue
     
        @return  (void) no return value
*/
void 
clearQueue (Queue * pQueue) 
{
        //Free up the space taken up by the queue
        free (pQueue->pArray);

        //Remove the array and set the count back to 0
        pQueue->pArray = NULL;
        pQueue->nCount = 0;
}

/* This function swaps the position of two locations between the X and Y queues
        @param   (int *)  pX1 is the pointer to the X coordinate of the 1st location in the X queue
        @param   (int *)  pX2 is the pointer to the X coordinate of the 2nd location in the X queue     
        @param   (int *)  pY1 is the pointer to the Y coordinate of the 1st location in the Y queue  
        @param   (int *)  pY2 is the pointer to the Y coordinate of the 2nd location in the Y queue

     
        @return  (void) no return value
*/
void 
swapCoordinates (int * pX1, int * pX2, int * pY1, int * pY2)
{
        //Store the 1st location coordinates in a temporary variable
	int nTempX = *pX1;
	int nTempY = *pY1;
	
        //store the values of the 2nd location to the address of the 1st location and vice versa.

	*pX1 = *pX2;
	*pX2 = nTempX;

	*pY1 = *pY2;
	*pY2 = nTempY;	
}

/* This function calculates the distance of two points
        @param   (Queue *)  pXQueue is the pointer to the queue which stores the selected X coordinates
        @param   (Queue *)  pYQueue is the pointer to the queue which stores the selected Y coordinates
        @param   (int)  nLRRHXLocation is the X location of LRRH
        @param   (int)  nLRRHYLocation is the Y location of LRRH
     
        @return  (void) no return value
*/
void 
sortCoordinatesByDistance (Queue * pXQueue, Queue * pYQueue, int nLRRHXLocation, int nLRRHYLocation)
{
	int i, j;
	
	int nMinimumDistIndex;

        //Perform a selection sort

        //Loop through the elements of the array
	for (i = 0; i < pXQueue->nCount - 1; i++)
	{
                //Get the distance of the currently selected X and Y coordinates of the array
		int nCurrentCoordDist = getDistance (nLRRHXLocation, 
                                                        nLRRHYLocation, 
                                                        pXQueue->pArray[i], 
                                                        pYQueue->pArray[i]);
		
                //Find the index of the location that is closest to LRRH
		nMinimumDistIndex = i;

                //Loop through the elements of the array again that DOES not include the currently selected element
		for (j = i + 1; j < pXQueue->nCount; j++)
		{
                        //Get the distances of the other locations
			int nNextCoordDist = getDistance (nLRRHXLocation,
                                                                nLRRHYLocation, 
                                                                pXQueue->pArray[j], 
                                                                pYQueue->pArray[j]);
			
                        //If this location is lower than what was initially selected, it means that it is closer to LRRH
			if (nNextCoordDist < nCurrentCoordDist)
				nMinimumDistIndex = j;			
		}
		
                //Swap the positions of the coordinates, put the location that is closest to LRRH to the front of the array.
		swapCoordinates (&pXQueue->pArray[nMinimumDistIndex],
                                &pXQueue->pArray[i], 
                                &pYQueue->pArray[nMinimumDistIndex], 
                                &pYQueue->pArray[i]);
	}
}

/* This function removes tiles that LRRH has already explored while moving to a location from the X and Y queues

        @param   (Queue *)  pXQueue is the pointer to the queue which stores the selected X coordinates
        @param   (Queue *)  pYQueue is the pointer to the queue which stores the selected Y coordinates
        @param   (int *)  pWalkedTiles is the pointer to narrWalkedTiles (contains all the tiles LRRH has walked on)
        @param   (int)  nGridSize is the size of the play grid
     
        @return  (void) no return value
*/
void 
removeWalkedCoordinatesFromQueue (Queue * pXQueue, Queue * pYQueue, int * pWalkedTiles, int nGridSize)
{
	int i;
	int nNumOfWalkedTiles = 0;
	int narrWalkedTilesArray[pXQueue->nCount];
	
        //Loop through the queues
	for (i = 0; i < pXQueue->nCount; i++)
	{	
                /*Check if the value of the location at the walked tiles array is not equal to 0.
                  If so, that means that the location has already been explored.
                  If so, add the index of that location and increment the number of walked tiles.
                  it has not been explored and should not be removed.
                */	
		if (*(pWalkedTiles + pXQueue->pArray[i] * nGridSize + pYQueue->pArray[i]) != 0)
		{			
			narrWalkedTilesArray[nNumOfWalkedTiles] = i;
			nNumOfWalkedTiles++;
		}
		
	}
	
        //Seperate the checking and removal of walked tiles in different loops.
        //So to prevent changing the values while looping in the X and Y queues.
	for (i = 0; i < nNumOfWalkedTiles; i++)
	{
		removeLocationFromQueues(narrWalkedTilesArray[i], pXQueue, pYQueue);				
	}
}

/* This function removes 1 location from the X and Y queues.
        
        @param   (int)  nIndex is the index of the location to remove
        @param   (Queue *)  pXQueue is the pointer to the queue which stores the selected X coordinates
        @param   (Queue *)  pYQueue is the pointer to the queue which stores the selected Y coordinates
     
        @return  (void) no return value
*/
void 
removeLocationFromQueues (int nIndex, Queue * pXQueue, Queue * pYQueue)
{
        //Place the location to remove at the very start of the queues
	swapCoordinates (&pXQueue->pArray[nIndex], &pXQueue->pArray[0], &pYQueue->pArray[nIndex], &pYQueue->pArray[0]);
	
        //Once placed at the start, dequeue the X and Y array but do not use their values.
        //This is just to remove them from the queue.
	dequeue (pXQueue);
	dequeue (pYQueue);
}