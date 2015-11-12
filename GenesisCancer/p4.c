/*****************************************************************************
 File:   p4.c
 Author: Adela Yang and Jack Truskowski
 Date:   November 8 2015

 Description:
  Read cancer data from file
   Starter fitness function for Project 4
   template.p4 and in.p4
   and then run with:
     go p4 p4

Data Structures:
  This program stores information breast cancer in a linked list
  Uses GA to find one best rule

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
// #define DATA_FILE "cancer.data"  /* hardwired into this program */

/*****************************************************************************/
/* constants */
#define TRUE 1
#define FALSE 0
#define STRLEN 250    /* length (in chars) for strings *

/*****************************************************************************/
/* data structures */
typedef struct Example* ExampleP;  /* pointer to a example in the linked list */
struct Example {
  int idNum;         /* the ID number for this line */
  char attributes[STRLEN];   /* the attributes for this line of data */
  char class;         /* b=benign; d=malignant */
  ExampleP next;     /* next line of data */
};


/*****************************************************************************/
/* function prototypes */
double eval(char *str, int length, double *vect, int genes);  /* Genesis */
void openFile(FILE **fileptr, char *filename, char *mode);
ExampleP newExample();      /* get a new Example struct */
ExampleP fauxData ();       /* populate it with fake data */
void *emalloc(long size);   /* memory allocation, plus checking */
ExampleP readFile();

/*****************************************************************************/
/* global variables */
int NumAttr;
int NumLines;

ExampleP readFile(){
  char* infilename = "bc.data";
  FILE *infile;
  int i=0;
  char tempLine[STRLEN]; //temporary storage for the attributes
  ExampleP head = NULL;
  
  //open the file
  infile = fopen(infilename, "r");
  
  if (infile == NULL){
      printf("Can't open that file!\n");
      exit(1);
  }
  
  //get the number of attributes and lines
  fscanf(infile, "attributes: %d\nlines: %d\n", &NumAttr, &NumLines); 
 
  //Loop through the lines and make the objects
  while(fgets(tempLine, STRLEN, infile) != NULL && i<NumLines){

      //make a new person
      ExampleP example = newExample();

      example->idNum = i;
      
      //seperate line by commas
      char *loc;
      int locCounter = 0;
      const char comma[2] = ",";
      loc = strtok(tempLine, comma);
      
      while (loc != NULL){
        //add it to the aObject's attributes
        if(locCounter == 0){
          example->class = *loc;
          loc = strtok(NULL, comma);
          locCounter++;
        }
        else{
          example->attributes[locCounter-1] = *loc;
          loc = strtok(NULL, comma);
          locCounter++;
        }
      }
      
      //Add the new person to the linked list
      //check if its the first node...
      if(head == NULL){
          head = example;
      }
      else{
          //update this to be the head
          ExampleP tempEx = head;
          example->next = tempEx;
          head = example;
      }

      i++;
  }

  fclose(infile);
  return head; //return the head pointer
}


/*****************************************************************************/
double eval(char *str, int length, double *vect, int genes) {

  static double bestSoFar; /* best odds ratio found so far by this function */
  double fitness;          /* the fitness of this string */
  register int i;          /* loop counter */

  static ExampleP data;    /* a hardwired piece of data */


  /* Things to do for each new run of the GA */
  if ((Experiment == 0) && (Trials == 0)) {
    data = readFile();  /* just one for now, and faked instead of read from
                           file */
  }



  /* Things to do for each new Experiment */
  if (Trials == 0) {
    bestSoFar = 0;
    printf("\n\n\n");  /* blank lines between experiments */
  }
        
  fitness = Trials;

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
