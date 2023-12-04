#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <ncurses.h>
#include <time.h>
#include <sstream>
#include <iomanip>
#include <future>
#include <thread>
#include "sys_info.h"
#include "process_container.h"


char* getCString(std::string str){
    char * cstr = new char [str.length()+1];
    std::strcpy (cstr, str.c_str());
    return cstr;
}

void writeSysInfoToConsole(SysInfo sys, WINDOW* sys_win)
{
    sys.setAttributes();

    wattron(sys_win,COLOR_PAIR(2));
    mvwprintw(sys_win,1,2,getCString( "Custom System Monitor" ));
    wattroff(sys_win,COLOR_PAIR(2));
    mvwprintw(sys_win,3,2,getCString(( "OS: " + sys.getOsName())));
    mvwprintw(sys_win,4,2,getCString(( "Kernel version: " + sys.getKernelVersion())));
    mvwprintw(sys_win,5,2,getCString( "CPU: "));
    wattron(sys_win,COLOR_PAIR(1));
    wprintw(sys_win,getCString(Util::getProgressBar(sys.getCpuPercent())));
    wattroff(sys_win,COLOR_PAIR(1));
    mvwprintw(sys_win,6,2,getCString(( "Other cores:")));
    wattron(sys_win,COLOR_PAIR(6));
    std::vector<std::string> val = sys.getCoresStats();
    for (int i = 0; i < val.size(); i++) {
        mvwprintw(sys_win,(7+i),2,getCString(val[i]));
    }
    wattroff(sys_win,COLOR_PAIR(6));
    mvwprintw(sys_win, 11, 2, getCString("Memory: "));
wattron(sys_win, COLOR_PAIR(3));  // Use color pair 3
wprintw(sys_win, getCString(Util::getProgressBar(sys.getMemPercent())));
wattroff(sys_win, COLOR_PAIR(3));
    mvwprintw(sys_win,12,2,getCString(( "Total Processes:" + sys.getTotalProc())));
    mvwprintw(sys_win,13,2,getCString(( "Running Processes:" + sys.getRunningProc())));
    mvwprintw(sys_win,14,2,getCString(( "Up Time: " + Util::convertToTime(sys.getUpTime()))));

}

void getProcessListToConsole(ProcessContainer procs,WINDOW* win)
{
    procs.refreshList();
    wattron(win,COLOR_PAIR(4));
    mvwprintw(win,1,2,"PID:");
    mvwprintw(win,1,9,"User:");
    mvwprintw(win,1,16,"CPU[%]:");
    mvwprintw(win,1,26,"RAM[MB]:");
    mvwprintw(win,1,36,"Uptime:");
    mvwprintw(win,1,45,"CMD:");
    wattroff(win, COLOR_PAIR(4));
    wattron(win,COLOR_PAIR(5));
    vector<std::string> processes = procs.getList();
    for(int i = 0; i < 15; i++) {
        mvwprintw(win,2 +i,2,getCString(processes[i]));
    }
    wattroff(win, COLOR_PAIR(5));
}

void printMain(SysInfo sys,ProcessContainer procs)
{
    initscr();
    noecho();
    cbreak();
    start_color();
    int yMax,xMax;
    getmaxyx(stdscr,yMax,xMax);
    WINDOW *sys_win = newwin(17,xMax-1,0,0);
    WINDOW *proc_win = newwin(18,xMax-1,18,0);

    init_pair(1,COLOR_BLUE,COLOR_BLACK);
    init_pair(2,COLOR_GREEN,COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(5, COLOR_YELLOW, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);

    while (true) {
        box(sys_win,0,0);
        box (proc_win,0,0);
        writeSysInfoToConsole(sys,sys_win);
        getProcessListToConsole(procs,proc_win);
        wrefresh(sys_win);
        wrefresh(proc_win);
        refresh();
        sleep(1);
    }
    endwin();
}

int main( int   argc, char *argv[] )
{
    ProcessContainer procs;
    SysInfo sys;
    printMain(sys,procs);
    return 0;
}