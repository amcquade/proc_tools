// Aaron McQuade
// CS 4420
// Assignment 1 - Part 2
// pp_ps.cpp - C++ implementation

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std;

class ProcData {
public:
  string pid, cmd, state, cpu, mem, vsz, rss;
};


void print_data(vector<ProcData> d);
void compute_data(ProcData &p_data);
int proc_count();

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
        vector<ProcData> data;

        while ( !inp.eof() ) {
          stringstream ss(line);
          string s;
          ProcData pd;
          int i = 0;
          while ( !ss.eof() && i != 24 ) {
            ss >> s;
            // capture PID, command, state, VSZ, RSS
            if (i == 0  ) {
              pd.pid = s;
            } else if ( i == 1 ) {
              pd.cmd = s;
            } else if ( i == 2) {
              pd.state = s;
            } else if ( i == 22 ) {
              pd.vsz = s;
            } else if ( i == 23 ) {
              pd.rss = s;
            }

            ++i;
          } // end line input
          compute_data(pd);
          data.push_back(pd);
          getline(inp, line);
        } // end file input
        inp.close();
        print_data(data);
      }
      // sort
      //print_data(data);
    }

  } else {
    perror("Error: Could not access /proc/");
    exit(-1);
  }
  closedir(dp);
  return 0;
}

// print formatted data
void print_data(vector<ProcData> d) {
  //d.push_back("meow");
  for (size_t x = 0; x < d.size(); ++x) {
    cout << d[x].pid << ' ' << d[x].cmd << ' ';
    cout << d[x].state << ' ' << d[x].cpu << ' ';
    cout << d[x].mem << ' ' << d[x].vsz << ' ';
    cout << d[x].rss << endl;
  }
}

int proc_count() {
  int count = 0;
  DIR *d;
  struct dirent *e;
  d = opendir ("/proc");
  if (d != NULL ) {
    // while readbale dirs
    while ( (e = readdir (d)) ) {
      ++count;
    }
    return count;
  }
  closedir(d);
}

// add %mem and %cpu fields
void compute_data(ProcData &p_data) {
  long mem, RSS;
  long phys_page = sysconf(_SC_PHYS_PAGES);
  long page_size = sysconf(_SC_PAGE_SIZE);
  long phys_mem_size = phys_page * page_size;
  string s_rss;
  s_rss = p_data.rss;

  RSS = strtol(s_rss.c_str(), NULL, 10);
  mem = (RSS * page_size * 100) / phys_mem_size;

  string s_mem = to_string(mem);
  p_data.mem = s_mem;
}
