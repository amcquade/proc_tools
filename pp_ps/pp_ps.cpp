// Aaron McQuade
// CS 4420
// Assignment 1 - Part 2
// pp_ps.cpp - C++ implementation

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

int main(int argc, char** argv) {
  // determine arg option
  //bool cpu, mem, pid, com;
  /*
  if (argc > 1) {
    string cmd = argv[1];
    switch (cmd) {
    case "-cpu":
      cpu = true;
      break;

    case "-mem":
      mem = true;
      break;

    case "-pid":
      pid = true;
      break;

    case "-com":
      com = true;
      break;
    }
  } else {
    perror("Error: Command must contains arg");
    exit(-1);
  }
  */

  DIR *dp;
  struct dirent *ep;
  int long pid;
  string f;

  // open proc folder
  dp = opendir ("/proc");
  if (dp != NULL ) {
    // while readbale dirs
    while ( (ep = readdir (dp)) ) {
      pid = strtol(ep->d_name, NULL, 10);

      // check if pid is owned
      if ( ( ep->d_type == DT_DIR ) && ( pid > 0) ) {
        // open stat file of pid
        string p, line;
        DIR *proc_dir;
        p = ep->d_name;
        p = "/proc/" + p + "/stat";

        // read from stat file
        ifstream inp;
        inp.open(p.c_str());
        getline(inp, line);

        while ( !inp.eof() ) {
          stringstream ss(line);
          string s;
          vector<string> data;
          int i = 0;
          while ( !ss.eof() ) {
            ss >> s;
            cout << s << endl;
          }
          cout << endl;
          exit(-1);
          getline(inp, line);
        }
        inp.close();
      }
    }

  } else {
    perror("Error: Could not access /proc/");
    exit(-1);
  }
  closedir(dp);
  return 0;
}