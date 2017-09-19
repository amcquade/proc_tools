// Aaron McQuade
// CS 4420
// Assignment 1 - Part 1
// pp_map.cpp - C++ implementation

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

void print_data(vector<string> d);
int main(int argc, char** argv) {
	// check number of args
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
			printf("Start\tEnd\tSize\tPerm\tPath\n");

			// read processes
			while (ep = readdir (dp)) {
				pid = strtol(ep->d_name, NULL, 10);
				// match pid and p && dir type
				if ( ( ep->d_type == DT_DIR ) && ( pid > 0) && (p == pid)) {
					// open specified proc file
					char path[50];
					DIR *proc_dir;
					strcpy(path, "/proc/");
					strcat(path, argv[1]);
					printf("%s\n", path);

					// open proc dir and check
					proc_dir = opendir(path);
					if (proc_dir == NULL) {
						printf("Error: Process could not be accessed\n");
						closedir(dp);
						closedir(proc_dir);
						exit(-1);
					} else {
						// read from maps file
						// add maps to path param
						// strcat(path, "maps");

						// declare stream + string vars
						ifstream inp;
						string line, pp, path2;
						vector<string> numbers;

						// set path and open stream to file
						pp = argv[1];
						path2 = "/proc/" + pp + "/maps";
						inp.open(path2.c_str());
						getline(inp, line);

						while ( !inp.eof() ) {
							// read line and tokenize
							stringstream ss(line);
							string s;
							vector<string> data;
							int i = 0;
							while ( !ss.eof() ) {
								ss >> s;
								if (i == 0 || i == 1 || (i == 5 && s != "0")) {
									if (i == 0 ) {
										string start, end;
										size_t found = s.find('-');
										if (found) {
											start = s.substr(0, found);
											//cout << start << endl;
											end = s.substr(found + 1, s.length() - found);
											//cout << end << endl;
											data.push_back(start);
											data.push_back(end);
										} else {
										exit(-1);
										// error looking for address
										// implement later
										}
									} else {
										data.push_back(s);
									}
								}

								++i;
							}
							print_data(data);
							cout << endl;
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

// print formatted data
void print_data(vector<string> d) {
	for (int x = 0; x < d.size(); ++x) {
		cout << d[x] << ' ';
	}
}