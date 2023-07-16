/*
* Programmed By: Aaron Gabrielle Dichoso 
*
* Description:
* 	QueueSystem_prototype.h: Contains the function prototypes of QueueSystem.c
*
* Last Modified: 04/02/2022
*/


typedef struct 
{
  int * pArray;
  int nCount;
  
} Queue;

void    initQueue                               (Queue * pQueue);
void    enqueue                                 (Queue * pQueue, 
                                                int element);
int     dequeue                                 (Queue * pQueue);
void    reverseQueue                            (Queue * pQueue);
void    clearQueue                              (Queue * pQueue);

void    swapCoordinates                         (int * pX1, 
                                                int * pX2, 
                                                int * pY1, 
                                                int * pY2);
void    sortCoordinatesByDistance               (Queue * pXQueue, 
                                                Queue * pYQueue, 
                                                int nLRRHXLocation, 
                                                int nLRRHYLocation);

void    removeWalkedCoordinatesFromQueue        (Queue * pXQueue, 
                                                Queue * pYQueue, 
                                                int * pWalkedTiles, 
                                                int nGridSize);
void    removeLocationFromQueues                (int nIndex, 
                                                Queue * pXQueue, 
                                                Queue * pYQueue);