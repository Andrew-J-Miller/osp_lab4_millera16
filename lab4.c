#include<stdio.h>
#include<string.h>

//lab4.c
//Andrew Miller
//This is a program that recursively searches for files
//It takes a required search term and a starting directory
//The search will be done via a depth first search algorithm



int main(int argc, char *argv)
{
	//Failiure condition for incorrect number of arguements
	//gives an error message and fails gracefully
	if (argc != 3)
	{
		printf("Error. Improper number of arguements.");
		printf("Please enter only the search term follwed by the");
		printf("starting directory\n");
		return 1;
	}








	return 0;
}



