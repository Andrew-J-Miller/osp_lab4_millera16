#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include<time.h>

//lab4.c
//Andrew Miller
//This is a program that recursively searches for files
//It takes a required search term and a starting directory
//The search will be done via a depth first search algorithm


//So this code doesn't actually work. I was not able to get the DFS portion working.
//I am not entirely sure what is wrong at the moment. It took a good deal of trial
//and error to get where I currently am, but I think the issue may have something 
//to do with the way directories are strcatted together.

//In its current state the result array is also not dynamically allocated. I had SO
//many issues trying to realloc the array. I initally tried making it a 2 D pointer
//array passed by reference, making a total of 3 pointers, but I very quickly got lost
//in pointer arithmetic. I tried a lot of different nonworking implementations for 
//this lab. None were pushed to github bvecause none actuially worked. This is as close as I could get unfortuneately.





//DFS will be the actual recursive function for the search itself
//Takes a string containing the current directory
int DFS(char *dir, char *target, char *results[], int num);


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

	

	char *strings[1024];
	int start = 0;
	int i = 0;
	DIR *curdir;
	//Starts the search now. Begins the timer with begin
	clock_t begin = clock();



	start =  DFS(argv[2], argv[1], strings, start);







	//End of the search, get a time for the end to find the runtime
	clock_t end = clock();
	double runtime = (double)(end-begin) / CLOCKS_PER_SEC;

	//Loop to check for directories and append colons
	for (i = 0; i < start; i++)
	{
		curdir = opendir(strings[i]);
		if(curdir != NULL)
		{
			strcat(strings[i], ":");
		}
	}
	//print all found terms
	printf("Search terms found: ");
	for (i = 0; i < start; i++)
	{
		printf("%s\n",strings[i]);
	}
	//print search time
	printf("Search time: %d\n", runtime);




	return 0;
}

//Will return the number of matched terms as an int
int DFS(char *dir, char *target, char *results[], int num)
{
	DIR *cdir;
	char *newdir;
	struct dirent *entry;
	cdir = opendir(dir);
	//printf("%s\n",dir);	

	char *ret = NULL;
	//Check to make sure the target is not larger than the directory
	if (strlen(dir) > strlen(target))
	{
		ret = strstr(dir, target);
	
	}
	


	//Regardless of whether the current location is a file or directory
	//It needs to be checked against the search term
	if (ret != NULL)
	{
		//reallocate space for the results array and add the current directory/file
		num++;	
		results[num] = strdup(dir);
		printf("%s\n", results[num]);
	}
	//The directory passed in must be able to be opened
	if (cdir == NULL)
	{
		return 0;
	}
	else
	{
	//loop through all directory entries running DFS again	
		while ((entry = readdir(cdir)) != NULL)
		{
			if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
			{
				newdir = strdup(dir);
				//printf("%s/%s\n",dir, entry->d_name);
				strcat(newdir, "/");
				strcat(newdir, entry->d_name);
				printf("%s\n", newdir);
				DFS(newdir, target, results, num);
			}
		}
	}
	//return the number of found terms
	return num;

}




