/*****************************************************************************
 File:   p4.c
 Author: Adela Yang and Jack Truskowski
 Date:   November 18 2015

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
#include <unistd.h>
#include "extern.h"  /* Genesis external definitions */

/*****************************************************************************/
/* defines -- no semicolons at the ends of the lines */
#define DATA_FILE "cancer.data"  /* hardwired into this program */

/*****************************************************************************/
/* constants */
#define TRUE 1
#define FALSE 0
#define STRLEN 250    /* length (in chars) for strings */

/*****************************************************************************/
/* data structures */
typedef struct Example* ExampleP;  /* pointer to a example in the linked list */
struct Example {
 int idNum;         /* the ID number for this line */
  int attributes[STRLEN];   /* the attributes for this line of data */
  int class;         /* 2=benign; 4=malignant */
  ExampleP next;     /* next line of data */
};


/*****************************************************************************/
/* function prototypes */
double eval(char *str, int length, double *vect, int genes);  /* Genesis */
void openFile(FILE **fileptr, char *filename, char *mode);
ExampleP newExample(int numAttrs);      /* get a new Example struct */
ExampleP fauxData ();       /* populate it with fake data */
void *emalloc(long size);   /* memory allocation, plus checking */
ExampleP readFile(); /* reads the file and stores it in a linked list */
void printLinkedList(ExampleP temp); /* prints the linked list */


/*****************************************************************************/
/* global variables */
int NumAttr;
int NumLines;

/*****************************************************************************
 Function: readFile
 Inputs:   nothing
 Returns:  head of linked list
 Description: reads file and stores it in a linked list
 *****************************************************************************/
ExampleP readFile(){
  char *loc;
  int locCounter;
  /* default file */
  char *infilename = "bc-numeric.v3.data";
  FILE *infile;
  int i=0;
  char tempLine[STRLEN]; /* temporary storage for the attributes */
  ExampleP head = NULL;
  const char comma[2] = ",";
  
  /* open the file */
  infile = fopen(infilename, "r");
  
  /* checks that the file exists */
  if (infile == NULL){
      printf("Can't open that file!\n");
      exit(1);
  }
  
  /* get the number of attributes and lines */
  fscanf(infile, "attributes: %d\nlines: %d\n", &NumAttr, &NumLines); 

  /* the attributes is one less because the class is included */
  NumAttr--;

  /*Loop through the lines and make the examples*/
  while((fgets(tempLine, STRLEN, infile) != NULL) && (i<NumLines)){
      /* make a new example */
      ExampleP example = newExample(NumAttr);
      example->idNum = i;
      
      /* seperate line by commas */
      locCounter = 0;
      loc = strtok(tempLine, comma);

      while (loc != NULL){
        /* add it to the example's class */
        if(locCounter == 0){
          example->class = atoi(loc);
          loc = strtok(NULL, comma);
          locCounter++;
        }
        /* add it to the example's attributes*/
        else{
          example->attributes[locCounter-1] = atoi(loc);
          loc = strtok(NULL, comma);
          locCounter++;
        }
      }
      
      /*Add the new example to the linked list check if its the first node*/
      if(head == NULL){
        head = example;
      }
      else{
	      /*update this to be the head*/
        ExampleP tempEx = head;
        example->next = tempEx;
        head = example;
      }

      i++;
  }

  fclose(infile);
  return head; /*return the head pointer*/
}

/*****************************************************************************
 Function: printLinkedList
 Inputs:   linked list
 Returns:  nothing
 Description: prints the linked list
 *****************************************************************************/
void printLinkedList(ExampleP temp){
  int i;
  printf("printing...\n");

  /* prints the attributes and class */
  while(temp != NULL){
    for(i=0; i < NumAttr; i++){
      printf(" %d ", temp->attributes[i]);
    }

    printf("\n Class Attr: %d \n", temp->class);
    temp = temp->next;
  }
}


/*****************************************************************************/
double eval(char *str, int length, double *vect, int genes) {

  static double bestSoFar; /* best odds ratio found so far by this function */
  double fitness;          /* the fitness of this string */
  register int i;          /* loop counter */
  ExampleP iterator;
  static ExampleP data;    /* a pointer to the first piece of the data in the LL */
  
  /* reset the odds ratio vars for each run of the fitness function */
  int aTruePos = 0;
  int bFalseNeg = 0;
  int cFalsePos = 0;
  int dTrueNeg = 0;
  double oddsRatio;

  /* Things to do for each new run of the GA */
  if ((Experiment == 0) && (Trials == 0)) {
    /*read in data here*/
    data = readFile();
  }

  /* Things to do for each new Experiment */
  if (Trials == 0) {
    bestSoFar = 0;
    printf("\n\n\n");  /* blank lines between experiments */
  }
 

  iterator = data;
  while(iterator){ /*go thru LL and compare */
    int dataMatches = 1; /* boolean to store if the rule matches the example */
    for(i=0; i<genes; i++){
      if(vect[i] < 10 && vect[i] != iterator->attributes[i]){ /*this is not a
							    match */
	dataMatches = 0;
	break;
      }
      /* else, this attribute matches, continue checking remaining attributes */
    }

    /* update the odds ratio counts */
    if(dataMatches == 0){ /*attributes did not match */
    
      if(iterator->class == 4){ /*Data doesn't match, is non-malignant*/
	bFalseNeg++;
      }else{ /*Data doesn't match, is malignant*/
	dTrueNeg++; 
      }
    }else{ /* data matches */
      if(iterator->class == 4){ /* matches, is malignant */
	aTruePos++;
      }else{ /*matches is non-malignant */
	cFalsePos++;
      }
    }
    
    iterator = iterator->next;
  }

  /*calculate odds ratio*/
  oddsRatio = (double)((0.5+aTruePos) * (0.5+dTrueNeg))/(double)((0.5+cFalsePos) * (0.5+bFalseNeg)); 
  fitness = oddsRatio; 


  /* check if this is the best so far */
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

    printf("\nClassifications: TruePos=%d, TrueNeg=%d, FalsePos=%d, FalseNeg=%d\n", aTruePos, dTrueNeg, cFalsePos, bFalseNeg);
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
    
  // printf("making new example\n");

  /* initialize all fields of the new struct */
  example->idNum = 0;
  // example->attributes = (int*) emalloc (sizeof(int) * (numAttrs));
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
