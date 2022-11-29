#include <curses.h>

#include <chrono>
#include <string>
#include <thread>
#include <vector>

#include "format.h"
#include "ncurses_display.h"
#include "system.h"

using namespace std;

// -------- Macros ---------------------------------------------------------------------------------------------------
#define ll                     long long int
#define ull                     unsingned long long
#define ff                      first
#define ss                      second
#define pii                     pair<int,int>
#define vii                     vector<int>
#define usii                    unordered_set<int>
#define sii                     set<int>
#define mii                     map<int, int>
#define umii                    unordered_map<int, int>
#define effi(x)                 x.max_load_factor(0.2);     // takes greater space to avoid collisions
#define pb                      push_back
#define sp                      " "
#define yep                     cout<<"YES"<<endl;
#define nope                    cout<<"NO"<<endl;
#define neg1                    cout<<-1<<endl;
#define boost                   ios_base::sync_with_stdio(false);cin.tie(NULL);cout.tie(NULL);
#define forl(i,n)               for(int i=0;i<n;i++)
#define itr(i,a,b)              for(int i=a;(b>=a)?i<b:i>b;(b>=a)?i++:i--)
#define print_arr2d(arr,n,m)    forl(i,n){forl(j,m){cout<<arr[i][j]<<' ';}cout<<endl;}
#define input(arr,n)            forl(i,n){cin>>arr[i];}
#define input_arr2d(arr, n, m)  forl(i,n){forl(j,m){cin>>arr[i][j];}}
#define sprec(x,y)              fixed<<setprecision(y)<<x
#define all(v)                  v.begin(),v.end()
#define rall(v)                 v.rbegin(),v.rend()
#define gcd(a,b)                __gcd(a,b)
#define lcm(a,b)                ((a)*(b)) / __gcd((a),(b))
#define min3(a,b,c)             min(c, min(a, b))
#define min4(a,b,c,d)           min(d, min(c, min(a, b)))
#define max3(a,b,c)             max(c, max(a, b))
#define max4(a,b,c,d)           max(d, max(c, max(a, b)))
// --------------------------------------------------------------------------------------------------------------------


string NCursesDisplay::ProgressBar(float percent) {
  string result{""};

  int size{100};

  float bars{percent * size};

  forl(i,size) {
    result += i <= bars ? '|' : '_';
  }

  string display{to_string(percent * 100).substr(0, 4)};

  if (percent < 0.1 || percent == 1.0)

    display = " " + to_string(percent * 100).substr(0, 3);

  return result + " " + display + "%%";
}

void NCursesDisplay::DisplaySystem(System& system, WINDOW* window) {
  int row{1};

  int progress_pad = 15;

  mvwprintw(window, ++row, 2, "CPU usage : ");

  wattron(window, COLOR_PAIR(1));

  mvwprintw(window, row, progress_pad, "");

  wprintw(window, ProgressBar(system.Cpu().Utilization()).c_str());

  wattroff(window, COLOR_PAIR(1));

  mvwprintw(window, ++row, 2, "Memory usage : ");

  wattron(window, COLOR_PAIR(1));

  mvwprintw(window, row, progress_pad, "");

  wprintw(window, ProgressBar(system.MemoryUtilization()).c_str());

  wattroff(window, COLOR_PAIR(1));

  row += 2;

  mvwprintw(
      window, row, 2,

      ("Total Processes : " + to_string(system.TotalProcesses())).c_str());

  mvwprintw(

      window, ++row, 2,

      ("Running Processes : " + to_string(system.RunningProcesses())).c_str());

  mvwprintw(
      window, ++row, 2,

      ("System Up Time : " + Format::ElapsedTime(system.UpTime())).c_str());

  wrefresh(window);
}


void NCursesDisplay::DisplaySystemInfo(System& system, WINDOW* window) {
  int row{0};

  int progress_pad = 15;
  
  wattron(window, COLOR_PAIR(3));

  mvwprintw(window, ++row, 2,
            ("Operating System: " + system.OperatingSystem()).c_str());

  mvwprintw(window, ++row, 2, ("Kernel: " + system.Kernel()).c_str());
  
  wattroff(window, COLOR_PAIR(3));
  wrefresh(window);
}

void NCursesDisplay::DisplayProcesses(vector<Process>& processes,

                                      WINDOW* window, int n) {
  int row{0};

  int const pid_column{2};

  int const user_column{10};

  int const cpu_column{25};

  int const ram_column{35};

  int const time_column{55};

  int const command_column{70};

  double total_cpu = 0;

  int total_mem = 0;

  wattron(window, COLOR_PAIR(2));

  mvwprintw(window, ++row, pid_column, "Pid");

  mvwprintw(window, row, user_column, "User");

  mvwprintw(window, row, cpu_column, "CPU [%%]");

  mvwprintw(window, row, ram_column, "Memory Usage[MB]");

  mvwprintw(window, row, time_column, "Arrival Time");

  mvwprintw(window, row++, command_column, "Path");

  wattroff(window, COLOR_PAIR(2));

  forl(i,n) {
    mvwprintw(window, ++row, pid_column, to_string(processes[i].Pid()).c_str());

    mvwprintw(window, row, user_column, processes[i].User().c_str());

    float cpu = processes[i].CpuUtilization() * 100;

    int mem = stoi(processes[i].Ram());

    total_cpu += cpu;

    total_mem += mem;

    mvwprintw(window, row, cpu_column, (to_string(cpu).substr(0, 4)+"%%").c_str());

    mvwprintw(window, row, ram_column, processes[i].Ram().c_str());

    mvwprintw(window, row, time_column,

              Format::ElapsedTime(processes[i].UpTime()).c_str());

    mvwprintw(window, row, command_column,

              processes[i].Command().substr(0, window->_maxx - 85).c_str());
  }

}

void NCursesDisplay::Display(System& system, int n) {
  initscr();  // start ncurses

  noecho();  // do not print input values

  cbreak();  // terminate ncurses on ctrl + c

  start_color();  // enable color

  int x_max{getmaxx(stdscr)};

  WINDOW* system_info_window = newwin(4, x_max - 4, 1, 2);
  
  WINDOW* system_window = newwin(10, x_max - 4, system_info_window->_maxy + 3, 2);

  WINDOW* process_window = newwin(4 + n, x_max - 4, system_window->_maxy + 8, 2);

  while (true) {
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);

    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    
    box(system_info_window, 0, 0);

    box(system_window, 0, 0);

    box(process_window, 0, 0);

    DisplaySystemInfo(system, system_info_window);

    DisplaySystem(system, system_window);

    DisplayProcesses(system.Processes(), process_window, n);

    wrefresh(system_info_window);

    wrefresh(system_window);

    wrefresh(process_window);

    refresh();

    this_thread::sleep_for(chrono::seconds(1));
  }

  endwin();
}





































































