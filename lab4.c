#include<stdio.h>
#include<string.h>
#include<time.h>

//lab4.c
//Andrew Miller
//This is a program that recursively searches for files
//It takes a required search term and a starting directory
//The search will be done via a depth first search algorithm



int main(int argc, char *argv[])
{
	//Failiure condition for incorrect number of arguements
	//gives an error message and fails gracefully
	if (argc != 3)
	{
		printf("Error. Improper number of arguements.");
		printf(" Please enter only the search term follwed by the");
		printf(" starting directory\n");
		return 1;
	}
	//Failure condition for starting directory not being an absolute path
	//Checks for first and last character '/'
	if (argv[2][0] != '/' || argv[2][(int)strlen(argv[2]) - 1] == '/')
	{
		printf("Error. Please ensure that the starting directory");
		printf(" is an absoulte directory\n");
		return 1;
	}

	



	//Starts the search now. Begins the timer with begin
	clock_t begin = clock();















	//End of the search, get a time for the end to find the runtime
	clock_t end = clock();
	double runtime = (double)(end-begin) / CLOCKS_PER_SEC;

	






	return 0;
}



