#include <iostream>
#include <fstream>
#include <iomanip>
#include <unistd.h>

using namespace std;
  
int main() {
  ofstream log("/tmp/mem_usage.log", ofstream::trunc);

  int count = 0;
  float total, min, max, avg = 0.0;

  while (true) {
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

      cout << fixed << setprecision(2) << mem_avail_gb << endl; 
      log << fixed << setprecision(2) << mem_avail_gb << endl;
      sleep(3);

    } else {
      cerr << "Error reading memory usage." << endl;
      log.close();
      return 1;
    }
  }
  log.close();

  return 0;
}
