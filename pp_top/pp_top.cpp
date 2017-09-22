// Aaron McQuade
// CS 4420
// Assignment 1 - Part 3
// pp_top.cpp - C++ implementation

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

#define clear() printf("\033[H\033[J");
#define gotoxy(x,y) printf("\033[%d;%dH", (x), (y));


class ProcData {
public:
   string pid, cmd, state, cpu, mem, vsz, starttime;
   float rss, utime, stime;

   void print_data();
};

void get_data();
void print_data(vector<ProcData> d);
void compute_data(ProcData &p_data);
int proc_count();
void print_header();

int main(int argc, char** argv) {
   system("clear");
   while (true) {
      print_header();
      get_data();
      sleep(1);
      clear();
      gotoxy(0, 0);
      clear();
   }
   return 0;
}

void get_data() {
   DIR *dp;
   struct dirent *ep;
   int long pid;

   // open proc folder
   dp = opendir ("/proc");
   if (dp != NULL ) {
      // while readbale dirs
      int i = 0;
      int rows = 0;
      while ( (ep = readdir (dp)) && (rows < 5) ) {
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

            stringstream ss(line);
            string s;
            float f;
            f = 0.0;
            ProcData pd;
            ss >> s;
            while ( !ss.eof() ) {
               // capture PID, command, state, VSZ, RSS, utime, stime, starttime
               if ( i == 0  ) {
                  pd.pid = s;
               } else if ( i == 1 ) {
                  pd.cmd = s;
               } else if ( i == 2) {
                  pd.state = s;
               } else if ( i == 13) {
                  f = stof(s);
                  pd.utime = f;
               } else if ( i == 14) {
                  f = stof(s);
                  pd.stime = f;
               } else if ( i == 21 ) {
                  pd.starttime = s;
               } else if ( i == 22 ) {
                  pd.vsz = s;
               } else if ( i == 23 ) {
                  f = stof(s);
                  pd.rss = f;
               }
               ss >> s;
               ++i;
            } // end line input
            compute_data(pd);
            inp.close();
            pd.print_data();
            i = 0;
            ++rows;
         }
      } // end while

   } else {
      perror("Error: Could not access /proc/");
      exit(-1);
   }
   closedir(dp);
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
/*
// add %mem and %cpu fields
void compute_data(ProcData &p_data) {
// calculate %mem
long mem, RSS;
string s_rss;
long phys_page = sysconf(_SC_PHYS_PAGES);
long page_size = sysconf(_SC_PAGE_SIZE);
long phys_mem_size = phys_page * page_size;

// convert rss
s_rss = p_data.rss;
RSS = strtol(s_rss.c_str(), NULL, 10);
mem = (RSS * page_size * 100) / phys_mem_size;

// convert and save
string s_mem = to_string(mem);
p_data.mem = s_mem;

// calculate %cpu
long cpu, l_utime, l_stime, proc_time, l_starttime, real_time;
long clk_ticks = sysconf(_SC_CLK_TCK);

// convert stime, utime, and starttime to long
l_utime = strtol(p_data.utime.c_str(), NULL, 10);
l_stime = strtol(p_data.stime.c_str(), NULL, 10);
l_starttime = strtol(p_data.starttime.c_str(), NULL, 10);

// get proc time and proc time
proc_time = (l_utime / clk_ticks) + (l_stime / clk_ticks);
real_time = l_utime - (l_starttime / clk_ticks);

if ( real_time != 0 )
cpu = real_time + ((proc_time * 100) / real_time);
else
cpu = 0;

p_data.cpu = to_string(cpu);
}
*/

// add %mem and %cpu fields
void compute_data(ProcData &p_data) {
   // calculate %mem
   float m, c;
   long phys_page = sysconf(_SC_PHYS_PAGES);
   long page_size = sysconf(_SC_PAGE_SIZE);
   long clk_ticks = sysconf(_SC_CLK_TCK);

   long phys_mem_size = phys_page * page_size;

   // convert rss
   p_data.mem = ( (float) p_data.rss * (float) getpagesize() * 100.0) / (float)phys_mem_size;
   cout << p_data.rss << endl;

   // calculate %cpu
   //float proc_time;
   //proc_time = (l_utime / clk_ticks) + (l_stime / clk_ticks);

   /*
   // calculate %cpu
   long cpu, l_utime, l_stime, proc_time, l_starttime, real_time;


   // convert stime, utime, and starttime to long
   l_utime = strtol(p_data.utime.c_str(), NULL, 10);
   l_stime = strtol(p_data.stime.c_str(), NULL, 10);
   l_starttime = strtol(p_data.starttime.c_str(), NULL, 10);

   // get proc time and proc time
   real_time = l_utime - (l_starttime / clk_ticks);

   if ( real_time != 0 )
   cpu = real_time + ((proc_time * 100) / real_time);
   else
   cpu = 0;

   p_data.cpu = to_string(cpu);
   */
}



void print_header() {
   //cout << setfill('-') << setw(100) << "-" << endl;

   cout << "PID" << setw(15) << right << "Command" << setw(10);
   cout << "State" << setw(10) << "CPU" << setw(19) << "MEM" << setw(10);
   cout << "VSZ" << setw(19) << "RSS" << endl;
}

// print formatted data
void ProcData::print_data() {
   cout << "pid " << setfill(' ') << setw(20) << left << pid << ' ' ;
   cout << "cmd " << cmd << ' ';
   cout << "state " << state << ' ';
   cout << "cpu " << cpu << ' ';
   cout << "mem " << mem << ' ';
   cout << "vsz " << vsz << ' ';
   cout << "rss " << rss << endl;
}
