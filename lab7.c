/*****************************************************************************
 File:   lab7.c
 Author: Clare Bates Congdon
 Date:   Wed Oct 28 09:33:26 2015

 Description:
   Starter fitness function for Lab 7
   edit template.ex1 and in.ex1 into template.lab7 and in.lab7, respectively,
   and then run with:
     go lab8 lab8

 This fitness function has one "fake" piece of data from the cancer.data
 file. Here, it' is hardwired in, but in Project 4, you will read the data
 into a linked list the first time the eval function is called.

******************************************************************************/

/*****************************************************************************/
/* include files */
#include <stdio.h>   /* standard input and output */
#include <stdlib.h>  /* standard library, including memory allocation */
#include <math.h>    /* math library */
#include <string.h>  /* strings */
#include "extern.h"  /* Genesis external definitions */

/*****************************************************************************/
/* defines -- no semicolons at the ends of the lines */
#define DATA_FILE "cancer.data"  /* hardwired into this program */

/*****************************************************************************/
/* data structures */
typedef struct Example* ExampleP;  /* pointer to a example in the linked list */
struct Example {
  int idNum;         /* the ID number for this line */
  int *attributes;   /* the attributes for this line of data */
  int class;         /* 2=benign; 4=malignant */
  ExampleP next;     /* next line of data */
};


/*****************************************************************************/
/* function prototypes */
double eval(char *str, int length, double *vect, int genes);  /* Genesis */
void openFile(FILE **fileptr, char *filename, char *mode);
ExampleP newExample();      /* get a new Example struct */
ExampleP fauxData ();       /* populate it with fake data */
void *emalloc(long size);   /* memory allocation, plus checking */


/*****************************************************************************/
double eval(char *str, int length, double *vect, int genes) {

  static double bestSoFar; /* best odds ratio found so far by this function */
  double fitness;          /* the fitness of this string */
  register int i;          /* loop counter */

  static ExampleP data;    /* a hardwired piece of data */
  
  static double aTruePos;
  static double bFalseNeg;
  static double cFalsePos;
  static double dTrueNeg;


  /* Things to do for each new run of the GA */
  if ((Experiment == 0) && (Trials == 0)) {
    /*read in data here*/
    data = readInFile();
  }

  /* Things to do for each new Experiment */
  if (Trials == 0) {
    bestSoFar = 0;
    aTruePos = 0;
    bFalseNeg = 0;
    cFalsePos = 0;
    dTrueNeg = 0;
    printf("\n\n\n");  /* blank lines between experiments */
  }
        

  /* **************************************************
     here, will need to compare GA string to data.
     Question: does this GA string match this piece of data?
     loop through all attributes
       if GA and data match or if GA is a "don't care"
         then this attribute matches
       if any attribute doesn't match, the whole thing doesn't match
       
     Also keep a count of how many "don't cares" are in the GA string

     1. initially, make sure you get the matching working, and use a count of
        the number of matches as the fitness.

     2. secondly, count the number of don't cares, and just print that out

     3. finally, change the fitness function as follows:
     fitness = -1 if not a match
     fitness = 0 if all don't cares
     fitness = count of specific attribute matches otherwise

     When you do Projet 4, you will be reading in a full dataset, and will
     need to do the matching across ALL the data. You will not use the don't
     care count initially... but it may help the search process.
     **************************************************     
   */

  fitness = 0;
  for(i=1; i<genes; i++){
    if(vect[i] > 10){
      fitness += 1;
      dontCares++;
    }else if(vect[i] == data->attributes[i]){
      fitness+=1;
    }
  }

  /* do we need this?
  if(fitness != genes){
    fitness = -1;
  }else{
    fitness = fitness-dontCares;
  }
  */

  if(vect[0] == 2 && data->attributes[0] == 2){
    /*true pos*/
    aTruePos++;
  }else if(vect[0] == 4 && data->attributes[0] == 2){
    /*false neg*/
    bFalseNeg++;
  }else if(vect[0] == 2 && data->attributes[0] == 4){
    /*false pos*/
    cFalsePos++;
  }else{
    /*true neg*/
    dTrueNeg++;
  }

  /*calculate odds ratio? why?*/
  double oddsRatio = ((0.5+aTruePos) * (0.5+dTrueNeg))/((0.5+cFalsePos) * (0.5+dTrueNeg)); 

  if (fitness > bestSoFar) {
    bestSoFar = fitness;
        
    printf("New best found in generation %d; Fitness is %.2f \n", Gen, fitness);

    printf("Bits: ");
    for (i=0; i<length; i++) {         /* print bits one by one */
      printf("%c", (int) str[i]);
    }
    printf("\n");

    printf("Genes: ");
    for (i=0; i<genes; i++) {         /* print genes one by one */
      printf("%2d ", (int) vect[i]);
    }
    printf("\n\n");
  }
  
  return fitness;

} /* end eval */

/*****************************************************************************
 Function: openFile
 Inputs:   the file to open, the file name, mode to open the file
 Returns:  nothing
 Description: opens file in specified mode or issues error message
 *****************************************************************************/
void openFile(FILE **fileptr, char *filename, char *mode) {
  *fileptr = fopen(filename,mode);       /* open the file */

  if ( !(*fileptr)) {
    printf("ERROR: Failed to open file: %s with mode %s.  Exiting.\n\n",
           filename, mode);
    exit(0);
  }

} /* end openFile */

/*****************************************************************************
 Function: fauxData
 Inputs:   none
 Returns:  a new Example
 Description: 
   allocate memory for a new example and fill it with bogus values
 *****************************************************************************/
ExampleP fauxData () {
  ExampleP faux;  /* the faux data to return */

  faux = newExample(9); /* get a new Example with 9 attributes */

  /* first line of data from cancer.data file */
  faux->idNum = 1000025;
  faux->attributes[0] = 5;
  faux->attributes[1] = 1;
  faux->attributes[2] = 1;
  faux->attributes[3] = 1;
  faux->attributes[4] = 2;
  faux->attributes[5] = 1;
  faux->attributes[6] = 3;
  faux->attributes[7] = 1;
  faux->attributes[8] = 1;
  faux->class = 2;         /* 2 = benign; 4 = malignant */

  return faux;

} /* end openFile */


/*****************************************************************************
 Function: newExample
 Inputs:   integer number of attributes
 Returns:  a pointer to a new Example
 Description: allocates and initializes memory or fails with error message
 *****************************************************************************/
        
ExampleP newExample (int numAttrs) {
  int i;   /* loop counter */
  ExampleP example = (ExampleP) emalloc(sizeof(struct Example));
    
  /* initialize all fields of the new struct */
  example->idNum = 0;
  example->attributes = (int*) emalloc (sizeof(int) * (numAttrs));
  for (i=0; i<numAttrs; i++) {
    example->attributes[i]=0;
  }
  example->class = 0;
  example->next = NULL;
        
  return example;
}

/*****************************************************************************
 Function: emalloc
 Inputs:   the size of the block of memory needed
 Returns:  a pointer to the block of memory
 Description: calls malloc, but checks for failure
 *****************************************************************************/
void *emalloc(long size) {
  void *p = (void *) malloc(size);     /* request memory */

  if (p == NULL) {            /* quit if request failed */
    printf("Unable to intitialize...out of memory.");
    exit(0);
  }

  return p;

} /* end emalloc */

/************************************************ end of file ****/