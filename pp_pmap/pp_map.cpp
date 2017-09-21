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
#include <iomanip>
using namespace std;

void print_data(vector<string> d);
string make_size(string s, string e);
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

		dp = opendir ("/proc");
		if (dp != NULL) {
			cout << "Start" << setw(18) << "End" << setw(21) << "Size" << setw(21) << "Perms" << setw(19) << "Path" << endl;
			cout << setfill('-') << setw(95) << "-" << endl;
			// read processes
			while ( (ep = readdir (dp)) ) {
				// match pid and p && dir type
				pid = strtol(ep->d_name, NULL, 10);
				if ( ( ep->d_type == DT_DIR ) && ( pid > 0) && (p == pid)) {
					// open specified proc file
					char path[50];
					DIR *proc_dir;
					strcpy(path, "/proc/");
					strcat(path, argv[1]);

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
									// split hex addr
									if (i == 0 ) {
										string start, end;
										size_t found = s.find('-');
										if (found) {
											start = s.substr(0, found);
											end = s.substr(found + 1, s.length() - found);
											data.push_back(start);
											data.push_back(end);

											// get size from hex addresses
											string size;
											size = make_size(start, end);
											data.push_back(size);
										} else {
											perror ("Error: Couldn't split hex address");
											exit(-1);
										}
									} else {
										data.push_back(s);
									}
								}
								++i;
							} // end line tokenize while
							print_data(data);
							cout << endl;
							getline(inp, line);
						} // end file read while
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
	for (size_t x = 0; x < d.size(); ++x) {
		cout << setfill(' ') << setw(20) << left << d[x];
	}
}

// takes input of two string hex addresses
// function computes the size from addresses
// returns the size of the process as a string
string make_size(string s, string e) {
	int long start_hex, end_hex, size_hex;

	// convert e and s to hex
	start_hex = strtol(s.c_str(), NULL, 16);
	end_hex = strtol(e.c_str(), NULL, 16);

	// compute hex and convert to string
	size_hex = end_hex - start_hex;
	stringstream convert;
	convert >> std::hex >> size_hex;
	size_hex = size_hex / 1000;
	string size = to_string(size_hex);
	return size;
}
