// Aaron McQuade
// CS 4420
// Assignment 1 - Part 1
// pp_map.cc

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char** argv) {
	// input handling
	if (argc != 2) {
		printf("Error: Enter process number\n");
		exit(-1);
	} else if (argc > 2) {
		printf("Error: Too many args\n");
		exit(-1);
	} else {
		DIR *dp;
		struct dirent *ep;
		int long p, pid;

		p = strtol(argv[1], NULL, 10);
		if (p == 0) {
			printf("Error: Input was not a number\n");
			exit(-1);
		}
		// printf("%lu\n", p);
		dp = opendir ("/proc");
		if (dp != NULL) {
			// read processes
			printf("Start\tEnd\tSize\tPerm\tPath\n");
			while (ep = readdir (dp)) {
				pid = strtol(ep->d_name, NULL, 10);
				// match pid and p
				if ( ( ep->d_type == DT_DIR ) && ( pid > 0) && (p == pid)) {
					// open specified proc file
					//char path[50];
					char buffer[25];
					DIR *proc_dir;
					int n;
					n = sprintf(buffer, "/proc/%lu/maps", pid);

					//strcpy(path2, argv[1]);
					//strcpy(path, "/proc/");
					//strcat(path, argv[1]);
					printf("%s\n", buffer);

					proc_dir = opendir(buffer);

					if (proc_dir == NULL) {
						printf("Error: Process could not be accessed\n");
						closedir(dp);
						closedir(proc_dir);
						exit(-1);
					} else {
						// read from maps file
						FILE *fptr;
						char ch;
						fptr = fopen(buffer, "r");
						if (fptr == NULL) {
							printf("Error: Could not read maps\n");
							closedir(dp);
							closedir(proc_dir);
							exit(-1);
						}


					}
				}
			}
			closedir(dp);
			return 0;
		} else {
			perror ("Couldn't open the directory");
			exit(-1);
		}
	}
	return 0;
}
