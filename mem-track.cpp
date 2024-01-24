#include <iostream>
#include <fstream>
#include <iomanip>
#include <unistd.h>
#include <ncurses.h>
#include <cfloat>

using namespace std;

int main() {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    nodelay(stdscr, TRUE);
    int DELAY = 1000;
    int key_press;

    ofstream log("/tmp/mem_usage.log", ofstream::trunc);

    float total, max, avg = 0.0;
    float min = FLT_MAX;
    int count = 0;

    bool run = true;
    while (run) {
        ifstream mem_info("/proc/meminfo");

        if (mem_info.is_open()) {
            string mem_avail;

            for (size_t i = 0; i < 8; i++) {
                mem_info >> mem_avail;
            }

            float mem_avail_gb = stof(mem_avail) / 1024 / 1024;

            total += mem_avail_gb;
            count++;
            min = std::min(min, mem_avail_gb);
            max = std::max(max, mem_avail_gb);
            avg = total / count;

            clear();
            printw("Available Memory Statistics:\n");
            printw("%-15s %-15s %-15s %-15s\n", "Current (GB)", "Min (GB)", "Max (GB)", "Average (GB)");
            printw("%-15.1f %-15.1f %-15.1f %-15.1f\n", mem_avail_gb, min, max, avg);
            refresh();

            for (int i = 0; i < DELAY / 100; i++) {
                usleep(100000); // Sleep for 100 milliseconds
                key_press = getch();
                if (key_press == 'q' || key_press == 'Q') {
                    clear();
                    run = false;
                    break;
                }
            }
        } else {
            cerr << "Error reading memory usage." << endl;
            log.close();
            endwin();
            return 1;
        }
        mem_info.close();
    }

    endwin();

    cout << "Final Memory Usage Statistics:" << endl;
    cout << left << setw(15) << "Min (GB)" << setw(15) << "Max (GB)" << setw(15) << "Average (GB)" << endl;
    cout << fixed << setprecision(2) << setw(15) << min << setw(15) << max << setw(15) << avg << endl;
    log.close();

    return 0;
}
