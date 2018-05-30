#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include <pthread.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>

//thread_search.c
//uses my<file_search.c as a baseline recursive search algorithm
//uses multiple threads to search for the specified file


//takes a file/dir as argument, recurses,
// prints name if empty dir or not a dir (leaves)
void *recur_file_search(char *file);

//share search term globally (rather than passing recursively)
const char *search_term;

const int THREADS = 4;

int isfile(const char *path);

int main(int argc, char **argv)
{
	if(argc != 3)
	{
		printf("Usage: my_file_search <search_term> <dir>\n");
		printf("Performs recursive file search for files/dirs matching\
				<search_term> starting at <dir>\n");
		exit(1);
	}

	//declares the array of pthreads
	pthread_t threads[THREADS];

	//a counter for the current number of working threads
	int threadCount = 0;

	int i = 0;


	//don't need to bother checking if term/directory are swapped, since we can't
	// know for sure which is which anyway
	search_term = argv[1];

	//open the top-level directory
	DIR *dir = opendir(argv[2]);

	//make sure top-level dir is openable (i.e., exists and is a directory)
	if(dir == NULL)
	{
		perror("opendir failed");
		exit(1);
	}

	struct dirent *cFile;
	
	//char *next;

	//start timer for recursive search
	struct timeval start, end;
	gettimeofday(&start, NULL);

	//loops through all entries of the staring directrory to assigne threads
	while ((cFile = readdir(dir)) != NULL)
	{
		//. and .. need to be excluded from the search
		if (strcmp(cFile->d_name, "..") != 0 && strcmp(cFile->d_name, ".")!= 0)
		{
			//this next section of code is formatting each file in the directory to be a full filepath
			char *next =malloc(sizeof(char)*strlen(cFile->d_name)+strlen(argv[2])+1);
			strncpy(next, argv[2], strlen(argv[2]));
			strncat(next,cFile->d_name, strlen(cFile->d_name));
			if (isfile(next) == 0 )
			{
				strncat(next, "/", 1);
			
			}
			printf("%s\n",next);
			//failing b/c files are getting a slash. check for directory here.
			//strncpy((next+strlen(argv[2])+1), cFile->d_name, strlen(cFile->d_name)+1);
			//Now that the string formatting is taken care of, threads can be assigned to recurse on these filepaths
			if (threadCount == THREADS)
			{
			//wait for threads to complete before reassignment
				for (i = 0; i < THREADS; i++)
				{
					pthread_join(threads[i], NULL);
				}
				//Reset threadCount to 0
				threadCount = 0;
			}
			pthread_create(&threads[threadCount], NULL, recur_file_search, (void*)next);
			threadCount++;
			free(next);
		}	
		closedir(dir);
	
	
	}
	




	


	gettimeofday(&end, NULL);
	printf("Time: %ld\n", (end.tv_sec * 1000000 + end.tv_usec)
			- (start.tv_sec * 1000000 + start.tv_usec));

	return 0;
}




int isfile(const char *path)
{
	struct stat path_stat;
	stat(path, &path_stat);
	return S_ISREG(path_stat.st_mode);



}



//This function takes a path to recurse on, searching for mathes to the
// (global) search_term.  The base case for recursion is when *file is
// not a directory.
//Parameters: the starting path for recursion (char *), which could be a
// directory or a regular file (or something else, but we don't need to
// worry about anything else for this assignment).
//Returns: nothing
//Effects: prints the filename if the base case is reached *and* search_term
// is found in the filename; otherwise, prints the directory name if the directory
// matches search_term.
void *recur_file_search(char *file)
{
	//check if directory is actually a file
	DIR *d = opendir(file);

	//NULL means not a directory (or another, unlikely error)
	if(d == NULL)
	{
		//opendir SHOULD error with ENOTDIR, but if it did something else,
		// we have a problem (e.g., forgot to close open files, got
		// EMFILE or ENFILE)
		if(errno != ENOTDIR)
		{	
			perror("Something weird happened!");
			fprintf(stderr, "While looking at: %s\n", file);
			exit(1);
		}

		//nothing weird happened, check if the file contains the search term
		// and if so print the file to the screen (with full path)
		if(strstr(file, search_term) != NULL)
			printf("%s\n", file);

		//no need to close d (we can't, it is NULL!)
		return;
	}

	//we have a directory, not a file, so check if its name
	// matches the search term
	if(strstr(file, search_term) != NULL)
		printf("%s/\n", file);

	//call recur_file_search for each file in d
	//readdir "discovers" all the files in d, one by one and we
	// recurse on those until we run out (readdir will return NULL)
	struct dirent *cur_file;
	while((cur_file = readdir(d)) != NULL)
	{
		//make sure we don't recurse on . or ..
		if(strcmp(cur_file->d_name, "..") != 0 &&\
				strcmp(cur_file->d_name, ".") != 0)
		{
			//we need to pass a full path to the recursive function, 
			// so here we append the discovered filename (cur_file->d_name)
			// to the current path (file -- we know file is a directory at
			// this point)
			char *next_file_str = malloc(sizeof(char) * \
					strlen(cur_file->d_name) + \
					strlen(file) + 2);

			strncpy(next_file_str, file, strlen(file));
			strncpy(next_file_str + strlen(file), \
					"/", 1);
			strncpy(next_file_str + strlen(file) + 1, \
					cur_file->d_name, \
					strlen(cur_file->d_name) + 1);

			//recurse on the file
			recur_file_search(next_file_str);

			//free the dynamically-allocated string
			free(next_file_str);
		}
	}

	//close the directory, or we will have too many files opened (bad times)
	closedir(d);
}
