// Aaron McQuade
// CS 4420
// Assignment 1 - Part 1
// pp_map.cc

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>

using namespace std;
int main(int argc, char** argv) {
	// input handling
	if(argc != 2){
		printf("Error: Enter process number\n");
		exit(-1);
	} else if(argc > 2){
		printf("Error: Too many args\n");
		exit(-1);
	} else {
		DIR *dp;
		struct dirent *ep;
		int long p, pid;

		p = strtol(argv[1], NULL, 10);
		if(p == 0){
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
				if( ( ep->d_type == DT_DIR ) && ( pid > 0) && (p == pid)) {
					// open specified proc file
					char path[50];
					DIR *proc_dir;
					strcpy(path, "/proc/");
					strcat(path, argv[1]);
					printf("%s\n", path);
					proc_dir = opendir(path);
					if(proc_dir == NULL){
						printf("Error: Process could not be accessed\n");
						closedir(dp);
						closedir(proc_dir);
						exit(-1);
					} else {
						// read from maps file
						strcat(path, "maps");

						ifstream inp;
                  string line, pp, path2;
                  vector<string> info;

                  pp = argv[1];
                  path2 = "/proc/" + pp + "/maps";
                  inp.open(path2.c_str());
                  getline(inp, line);
                  cout << line << endl;

                  while( !inp.eof() ){
                     cout << line;
                     getline(inp, line);
                  }

                  inp.close();
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
