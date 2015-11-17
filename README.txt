Jack Truskowski
Adela Yang

Project 4

The files we created were script (a script for running the program), and all of the input files in the InputFiles/ directory (various combinations of crossover and mutation parameters)

We modified p4.c (from the fitness function in lab7) to read in cancer data in the correct format and to have the correct fitness function.

---

Running the program:

(1) Pull out all files from the InputFiles/ folder to its parent directory (The InputFiles/ directory is just for organizational purposes)

(2) The script titled "script" will run all the experiments and print the results to a file in OutputFiles/
For more information on which output file refers to which test, please refer to the bottom of this README file

---

To run a set of parameters individually:

go p4 in.p41 >& OutputFiles/output.p41 &

Change the .p41 in both places according to which set of parameters you'd like to run (again, refer to the bottom of this file for more info)

- - - 

****  README for P4 ****

This directory contains the file input and output file naming table:

		Crossover 	Mutation
p41 		.3			.0001
p42		.3			.001
p43		.3			.01
p44		.6			.0001
p45		.6			.001
p46		.6 			.01
p47		.9			.0001
p48		.9			.001
p49		.9			.01
