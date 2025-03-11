#include "headers/dictionary-simple.hpp"
#include "headers/set-linked.hpp"
#include "headers/set-simple.hpp"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

void createDirectoryIfNotExists(const std::string &path) {
  struct stat st = {0};
  if (stat(path.c_str(), &st) == -1) {
    mkdir(path.c_str(), 0700);
  }
}

size_t getCurrentRSS() {
  std::ifstream statm("/proc/self/statm");
  long pages;
  statm >> pages;
  return pages * sysconf(_SC_PAGESIZE);
}

class SetSimpleBenchmark {
  private:
  std::string implementation_name;
  std::mt19937 rng;

  void runSingleTest(const std::string &operation, int size,
                     std::ofstream &timeFile,
                     std::ofstream &memoryFile,
                     std::ofstream &logFile) {
    std::uniform_int_distribution<int> dist(0, 1000000);
    SetSimple set;
    std::vector<int> values;

    for (int i = 0; i < size; i++) {
      values.push_back(dist(rng));
    }

    auto startMem = getCurrentRSS();
    auto startTime = std::chrono::high_resolution_clock::now();

    if (operation == "Insert") {
      for (int val : values) {
        set.insert(val);
      }
    } else if (operation == "Search") {
      for (int val : values) {
        set.contains(val);
      }
    } else if (operation == "Remove") {
      for (int val : values) {
        set.remove(val);
      }
    } else if (operation == "Union") {
      SetSimple set2;
      for (size_t i = 0; i < values.size(); i++) {
        if (i % 2 == 0) {
          set.insert(values[i]);
        } else {
          set2.insert(values[i]);
        }
      }
      auto result = set + set2;
    } else if (operation == "Intersection") {
      SetSimple set2;
      for (int val : values) {
        set.insert(val);
        if (dist(rng) % 2 == 0) {
          set2.insert(val);
        }
      }
      auto result = set * set2;
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    auto endMem = getCurrentRSS();

    double duration = std::chrono::duration<double, std::milli>(endTime - startTime).count();
    double memoryUsage = (endMem - startMem) / 1024.0;

    timeFile << size << " " << duration << std::endl;
    memoryFile << size << " " << memoryUsage << std::endl;
    logFile << size << " " << duration << std::endl;
  }

  public:
  SetSimpleBenchmark(const std::string &name) : implementation_name(name) {
    std::random_device rd;
    rng = std::mt19937(rd());
  }

  void runAllTests() {
    std::vector<int> sizes = {100, 500, 1000, 2000, 5000, 10000};
    std::vector<std::string> operations = {
        "Insert", "Search", "Remove", "Union", "Intersection"};

    for (const auto &op : operations) {
      std::string timeFilename = "data/time_" + implementation_name + "_" + op + ".txt";
      std::string memoryFilename = "data/memory_" + implementation_name + "_" + op + ".txt";
      std::string logFilename = "data/log_" + implementation_name + "_" + op + ".txt";

      std::ofstream timeFile(timeFilename);
      std::ofstream memoryFile(memoryFilename);
      std::ofstream logFile(logFilename);

      timeFile << "# Size Time(ms)" << std::endl;
      memoryFile << "# Size Memory(KB)" << std::endl;
      logFile << "# Size Time(ms)" << std::endl;

      for (int size : sizes) {
        runSingleTest(op, size, timeFile, memoryFile, logFile);
      }

      timeFile.close();
      memoryFile.close();
      logFile.close();
    }
  }
};

class SetLinkedBenchmark {
  private:
  std::string implementation_name;
  std::mt19937 rng;

  void runSingleTest(const std::string &operation, int size,
                     std::ofstream &timeFile,
                     std::ofstream &memoryFile,
                     std::ofstream &logFile) {
    std::uniform_int_distribution<int> dist(0, 1000000);
    SetLinked set;
    std::vector<int> values;

    for (int i = 0; i < size; i++) {
      values.push_back(dist(rng));
    }

    auto startMem = getCurrentRSS();
    auto startTime = std::chrono::high_resolution_clock::now();

    if (operation == "Insert") {
      for (int val : values) {
        set.insert(val);
      }
    } else if (operation == "Search") {
      for (int val : values) {
        set.contains(val);
      }
    } else if (operation == "Remove") {
      for (int val : values) {
        set.remove(val);
      }
    } else if (operation == "Union") {
      SetLinked set2;
      for (size_t i = 0; i < values.size(); i++) {
        if (i % 2 == 0) {
          set.insert(values[i]);
        } else {
          set2.insert(values[i]);
        }
      }
      auto result = set + set2;
    } else if (operation == "Intersection") {
      SetLinked set2;
      for (int val : values) {
        set.insert(val);
        if (dist(rng) % 2 == 0) {
          set2.insert(val);
        }
      }
      auto result = set * set2;
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    auto endMem = getCurrentRSS();

    double duration = std::chrono::duration<double, std::milli>(endTime - startTime).count();
    double memoryUsage = (endMem - startMem) / 1024.0;

    timeFile << size << " " << duration << std::endl;
    memoryFile << size << " " << memoryUsage << std::endl;
    logFile << size << " " << duration << std::endl;
  }

  public:
  SetLinkedBenchmark(const std::string &name) : implementation_name(name) {
    std::random_device rd;
    rng = std::mt19937(rd());
  }

  void runAllTests() {
    std::vector<int> sizes = {100, 500, 1000, 2000, 5000, 10000};
    std::vector<std::string> operations = {
        "Insert", "Search", "Remove", "Union", "Intersection"};

    for (const auto &op : operations) {
      std::string timeFilename = "data/time_" + implementation_name + "_" + op + ".txt";
      std::string memoryFilename = "data/memory_" + implementation_name + "_" + op + ".txt";
      std::string logFilename = "data/log_" + implementation_name + "_" + op + ".txt";

      std::ofstream timeFile(timeFilename);
      std::ofstream memoryFile(memoryFilename);
      std::ofstream logFile(logFilename);

      timeFile << "# Size Time(ms)" << std::endl;
      memoryFile << "# Size Memory(KB)" << std::endl;
      logFile << "# Size Time(ms)" << std::endl;

      for (int size : sizes) {
        runSingleTest(op, size, timeFile, memoryFile, logFile);
      }

      timeFile.close();
      memoryFile.close();
      logFile.close();
    }
  }
};

class DictionarySimpleBenchmark {
  private:
  std::string implementation_name;
  std::mt19937 rng;

  std::string generateRandomString() {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string result(50, ' ');
    for (int i = 0; i < 50; i++) {
      result[i] = alphanum[rng() % (sizeof(alphanum) - 1)];
    }
    return result;
  }

  void runSingleTest(const std::string &operation, int size,
                     std::ofstream &timeFile,
                     std::ofstream &memoryFile,
                     std::ofstream &logFile) {
    DictionarySimple dict(size * 2);
    std::vector<std::string> values;

    for (int i = 0; i < size; i++) {
      values.push_back(generateRandomString());
    }

    auto startMem = getCurrentRSS();
    auto startTime = std::chrono::high_resolution_clock::now();

    if (operation == "Insert") {
      for (const auto &val : values) {
        dict.insert(val);
      }
    } else if (operation == "Search") {
      for (const auto &val : values) {
        dict.contains(val);
      }
    } else if (operation == "Remove") {
      for (const auto &val : values) {
        dict.remove(val);
      }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    auto endMem = getCurrentRSS();

    double duration = std::chrono::duration<double, std::milli>(endTime - startTime).count();
    double memoryUsage = (endMem - startMem) / 1024.0;

    timeFile << size << " " << duration << std::endl;
    memoryFile << size << " " << memoryUsage << std::endl;
    logFile << size << " " << duration << std::endl;
  }

  public:
  DictionarySimpleBenchmark(const std::string &name) : implementation_name(name) {
    std::random_device rd;
    rng = std::mt19937(rd());
  }

  void runAllTests() {
    std::vector<int> sizes = {100, 250, 500, 750, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000, 5500, 6000, 6500, 7000, 7500, 8000, 8250, 8500, 8750, 9000, 9250, 9500, 9600, 9700, 9800, 9900, 10000};
    std::vector<std::string> operations = {
        "Insert", "Search", "Remove"};

    for (const auto &op : operations) {
      std::string timeFilename = "data/time_" + implementation_name + "_" + op + ".txt";
      std::string memoryFilename = "data/memory_" + implementation_name + "_" + op + ".txt";
      std::string logFilename = "data/log_" + implementation_name + "_" + op + ".txt";

      std::ofstream timeFile(timeFilename);
      std::ofstream memoryFile(memoryFilename);
      std::ofstream logFile(logFilename);

      timeFile << "# Size Time(ms)" << std::endl;
      memoryFile << "# Size Memory(KB)" << std::endl;
      logFile << "# Size Time(ms)" << std::endl;

      for (int size : sizes) {
        runSingleTest(op, size, timeFile, memoryFile, logFile);
      }

      timeFile.close();
      memoryFile.close();
      logFile.close();
    }
  }
};

void generateComparisonScripts() {
  std::ofstream insertTimeScript("data/plot_insert_time.gnu");
  insertTimeScript << "set terminal png size 1200,800 enhanced font 'Arial,12'\n";
  insertTimeScript << "set output 'charts/insert_time_chart.png'\n";
  insertTimeScript << "set style data linespoints\n";
  insertTimeScript << "set style line 1 lc rgb '#e41a1c' pt 7 ps 1.5 lw 2\n";
  insertTimeScript << "set style line 2 lc rgb '#377eb8' pt 9 ps 1.5 lw 2\n";
  insertTimeScript << "set style line 3 lc rgb '#4daf4a' pt 5 ps 1.5 lw 2\n";
  insertTimeScript << "set grid\n";
  insertTimeScript << "set key outside right box\n";
  insertTimeScript << "set border 3\n";
  insertTimeScript << "set tics nomirror\n";
  insertTimeScript << "set title 'Insert Operation - Time Comparison' font 'Arial,14'\n";
  insertTimeScript << "set xlabel 'Input Size' font 'Arial,12'\n";
  insertTimeScript << "set ylabel 'Time (ms)' font 'Arial,12'\n";
  insertTimeScript << "plot 'data/time_LinkedList_Insert.txt' using 1:2 with linespoints ls 1 title 'LinkedList', \\\n";
  insertTimeScript << "     'data/time_SimpleArray_Insert.txt' using 1:2 with linespoints ls 2 title 'SimpleArray', \\\n";
  insertTimeScript << "     'data/time_Dictionary_Insert.txt' using 1:2 with linespoints ls 3 title 'Dictionary'\n";
  insertTimeScript.close();

  std::ofstream searchTimeScript("data/plot_search_time.gnu");
  searchTimeScript << "set terminal png size 1200,800 enhanced font 'Arial,12'\n";
  searchTimeScript << "set output 'charts/search_time_chart.png'\n";
  searchTimeScript << "set style data linespoints\n";
  searchTimeScript << "set style line 1 lc rgb '#e41a1c' pt 7 ps 1.5 lw 2\n";
  searchTimeScript << "set style line 2 lc rgb '#377eb8' pt 9 ps 1.5 lw 2\n";
  searchTimeScript << "set style line 3 lc rgb '#4daf4a' pt 5 ps 1.5 lw 2\n";
  searchTimeScript << "set grid\n";
  searchTimeScript << "set key outside right box\n";
  searchTimeScript << "set border 3\n";
  searchTimeScript << "set tics nomirror\n";
  searchTimeScript << "set title 'Search Operation - Time Comparison' font 'Arial,14'\n";
  searchTimeScript << "set xlabel 'Input Size' font 'Arial,12'\n";
  searchTimeScript << "set ylabel 'Time (ms)' font 'Arial,12'\n";
  searchTimeScript << "plot 'data/time_LinkedList_Search.txt' using 1:2 with linespoints ls 1 title 'LinkedList', \\\n";
  searchTimeScript << "     'data/time_SimpleArray_Search.txt' using 1:2 with linespoints ls 2 title 'SimpleArray', \\\n";
  searchTimeScript << "     'data/time_Dictionary_Search.txt' using 1:2 with linespoints ls 3 title 'Dictionary'\n";
  searchTimeScript.close();

  std::ofstream removeTimeScript("data/plot_remove_time.gnu");
  removeTimeScript << "set terminal png size 1200,800 enhanced font 'Arial,12'\n";
  removeTimeScript << "set output 'charts/remove_time_chart.png'\n";
  removeTimeScript << "set style data linespoints\n";
  removeTimeScript << "set style line 1 lc rgb '#e41a1c' pt 7 ps 1.5 lw 2\n";
  removeTimeScript << "set style line 2 lc rgb '#377eb8' pt 9 ps 1.5 lw 2\n";
  removeTimeScript << "set style line 3 lc rgb '#4daf4a' pt 5 ps 1.5 lw 2\n";
  removeTimeScript << "set grid\n";
  removeTimeScript << "set key outside right box\n";
  removeTimeScript << "set border 3\n";
  removeTimeScript << "set tics nomirror\n";
  removeTimeScript << "set title 'Remove Operation - Time Comparison' font 'Arial,14'\n";
  removeTimeScript << "set xlabel 'Input Size' font 'Arial,12'\n";
  removeTimeScript << "set ylabel 'Time (ms)' font 'Arial,12'\n";
  removeTimeScript << "plot 'data/time_LinkedList_Remove.txt' using 1:2 with linespoints ls 1 title 'LinkedList', \\\n";
  removeTimeScript << "     'data/time_SimpleArray_Remove.txt' using 1:2 with linespoints ls 2 title 'SimpleArray', \\\n";
  removeTimeScript << "     'data/time_Dictionary_Remove.txt' using 1:2 with linespoints ls 3 title 'Dictionary'\n";
  removeTimeScript.close();

  std::ofstream unionTimeScript("data/plot_union_time.gnu");
  unionTimeScript << "set terminal png size 1200,800 enhanced font 'Arial,12'\n";
  unionTimeScript << "set output 'charts/union_time_chart.png'\n";
  unionTimeScript << "set style data linespoints\n";
  unionTimeScript << "set style line 1 lc rgb '#e41a1c' pt 7 ps 1.5 lw 2\n";
  unionTimeScript << "set style line 2 lc rgb '#377eb8' pt 9 ps 1.5 lw 2\n";
  unionTimeScript << "set grid\n";
  unionTimeScript << "set key outside right box\n";
  unionTimeScript << "set border 3\n";
  unionTimeScript << "set tics nomirror\n";
  unionTimeScript << "set title 'Union Operation - Time Comparison' font 'Arial,14'\n";
  unionTimeScript << "set xlabel 'Input Size' font 'Arial,12'\n";
  unionTimeScript << "set ylabel 'Time (ms)' font 'Arial,12'\n";
  unionTimeScript << "plot 'data/time_LinkedList_Union.txt' using 1:2 with linespoints ls 1 title 'LinkedList', \\\n";
  unionTimeScript << "     'data/time_SimpleArray_Union.txt' using 1:2 with linespoints ls 2 title 'SimpleArray'\n";
  unionTimeScript.close();

  std::ofstream intersectionTimeScript("data/plot_intersection_time.gnu");
  intersectionTimeScript << "set terminal png size 1200,800 enhanced font 'Arial,12'\n";
  intersectionTimeScript << "set output 'charts/intersection_time_chart.png'\n";
  intersectionTimeScript << "set style data linespoints\n";
  intersectionTimeScript << "set style line 1 lc rgb '#e41a1c' pt 7 ps 1.5 lw 2\n";
  intersectionTimeScript << "set style line 2 lc rgb '#377eb8' pt 9 ps 1.5 lw 2\n";
  intersectionTimeScript << "set grid\n";
  intersectionTimeScript << "set key outside right box\n";
  intersectionTimeScript << "set border 3\n";
  intersectionTimeScript << "set tics nomirror\n";
  intersectionTimeScript << "set title 'Intersection Operation - Time Comparison' font 'Arial,14'\n";
  intersectionTimeScript << "set xlabel 'Input Size' font 'Arial,12'\n";
  intersectionTimeScript << "set ylabel 'Time (ms)' font 'Arial,12'\n";
  intersectionTimeScript << "plot 'data/time_LinkedList_Intersection.txt' using 1:2 with linespoints ls 1 title 'LinkedList', \\\n";
  intersectionTimeScript << "     'data/time_SimpleArray_Intersection.txt' using 1:2 with linespoints ls 2 title 'SimpleArray'\n";
  intersectionTimeScript.close();

  std::ofstream insertMemoryScript("data/plot_insert_memory.gnu");
  insertMemoryScript << "set terminal png size 1200,800 enhanced font 'Arial,12'\n";
  insertMemoryScript << "set output 'charts/insert_memory_chart.png'\n";
  insertMemoryScript << "set style data linespoints\n";
  insertMemoryScript << "set style line 1 lc rgb '#e41a1c' pt 7 ps 1.5 lw 2\n";
  insertMemoryScript << "set style line 2 lc rgb '#377eb8' pt 9 ps 1.5 lw 2\n";
  insertMemoryScript << "set style line 3 lc rgb '#4daf4a' pt 5 ps 1.5 lw 2\n";
  insertMemoryScript << "set grid\n";
  insertMemoryScript << "set key outside right box\n";
  insertMemoryScript << "set border 3\n";
  insertMemoryScript << "set tics nomirror\n";
  insertMemoryScript << "set title 'Insert Operation - Memory Comparison' font 'Arial,14'\n";
  insertMemoryScript << "set xlabel 'Input Size' font 'Arial,12'\n";
  insertMemoryScript << "set ylabel 'Memory (KB)' font 'Arial,12'\n";
  insertMemoryScript << "plot 'data/memory_LinkedList_Insert.txt' using 1:2 with linespoints ls 1 title 'LinkedList', \\\n";
  insertMemoryScript << "     'data/memory_SimpleArray_Insert.txt' using 1:2 with linespoints ls 2 title 'SimpleArray', \\\n";
  insertMemoryScript << "     'data/memory_Dictionary_Insert.txt' using 1:2 with linespoints ls 3 title 'Dictionary'\n";
  insertMemoryScript.close();

  std::ofstream logTimeScript("data/plot_log_time.gnu");
  logTimeScript << "set terminal png size 1200,800 enhanced font 'Arial,12'\n";
  logTimeScript << "set output 'charts/log_time_chart.png'\n";
  logTimeScript << "set style data linespoints\n";
  logTimeScript << "set style line 1 lc rgb '#e41a1c' pt 7 ps 1.5 lw 2\n";
  logTimeScript << "set style line 2 lc rgb '#377eb8' pt 9 ps 1.5 lw 2\n";
  logTimeScript << "set style line 3 lc rgb '#4daf4a' pt 5 ps 1.5 lw 2\n";
  logTimeScript << "set grid\n";
  logTimeScript << "set key outside right box\n";
  logTimeScript << "set border 3\n";
  logTimeScript << "set tics nomirror\n";
  logTimeScript << "set logscale xy\n";
  logTimeScript << "set title 'Insert Operation - Log Scale Time Comparison' font 'Arial,14'\n";
  logTimeScript << "set xlabel 'Input Size (log scale)' font 'Arial,12'\n";
  logTimeScript << "set ylabel 'Time (ms) (log scale)' font 'Arial,12'\n";
  logTimeScript << "plot 'data/log_LinkedList_Insert.txt' using 1:($2 > 0 ? $2 : 0.001) with linespoints ls 1 title 'LinkedList', \\\n";
  logTimeScript << "     'data/log_SimpleArray_Insert.txt' using 1:($2 > 0 ? $2 : 0.001) with linespoints ls 2 title 'SimpleArray', \\\n";
  logTimeScript << "     'data/log_Dictionary_Insert.txt' using 1:($2 > 0 ? $2 : 0.001) with linespoints ls 3 title 'Dictionary'\n";
  logTimeScript.close();

  std::ofstream linkedAllScript("data/plot_linked_all.gnu");
  linkedAllScript << "set terminal png size 1200,800 enhanced font 'Arial,12'\n";
  linkedAllScript << "set output 'charts/linked_all_operations.png'\n";
  linkedAllScript << "set style data linespoints\n";
  linkedAllScript << "set style line 1 lc rgb '#e41a1c' pt 7 ps 1.5 lw 2\n";
  linkedAllScript << "set style line 2 lc rgb '#377eb8' pt 9 ps 1.5 lw 2\n";
  linkedAllScript << "set style line 3 lc rgb '#4daf4a' pt 5 ps 1.5 lw 2\n";
  linkedAllScript << "set style line 4 lc rgb '#984ea3' pt 13 ps 1.5 lw 2\n";
  linkedAllScript << "set style line 5 lc rgb '#ff7f00' pt 11 ps 1.5 lw 2\n";
  linkedAllScript << "set grid\n";
  linkedAllScript << "set key outside right box\n";
  linkedAllScript << "set border 3\n";
  linkedAllScript << "set tics nomirror\n";
  linkedAllScript << "set title 'LinkedList - All Operations Comparison' font 'Arial,14'\n";
  linkedAllScript << "set xlabel 'Input Size' font 'Arial,12'\n";
  linkedAllScript << "set ylabel 'Time (ms)' font 'Arial,12'\n";
  linkedAllScript << "plot 'data/time_LinkedList_Insert.txt' using 1:2 with linespoints ls 1 title 'Insert', \\\n";
  linkedAllScript << "     'data/time_LinkedList_Search.txt' using 1:2 with linespoints ls 2 title 'Search', \\\n";
  linkedAllScript << "     'data/time_LinkedList_Remove.txt' using 1:2 with linespoints ls 3 title 'Remove', \\\n";
  linkedAllScript << "     'data/time_LinkedList_Union.txt' using 1:2 with linespoints ls 4 title 'Union', \\\n";
  linkedAllScript << "     'data/time_LinkedList_Intersection.txt' using 1:2 with linespoints ls 5 title 'Intersection'\n";
  linkedAllScript.close();

  std::ofstream simpleAllScript("data/plot_simple_all.gnu");
  simpleAllScript << "set terminal png size 1200,800 enhanced font 'Arial,12'\n";
  simpleAllScript << "set output 'charts/simple_all_operations.png'\n";
  simpleAllScript << "set style data linespoints\n";
  simpleAllScript << "set style line 1 lc rgb '#e41a1c' pt 7 ps 1.5 lw 2\n";
  simpleAllScript << "set style line 2 lc rgb '#377eb8' pt 9 ps 1.5 lw 2\n";
  simpleAllScript << "set style line 3 lc rgb '#4daf4a' pt 5 ps 1.5 lw 2\n";
  simpleAllScript << "set style line 4 lc rgb '#984ea3' pt 13 ps 1.5 lw 2\n";
  simpleAllScript << "set style line 5 lc rgb '#ff7f00' pt 11 ps 1.5 lw 2\n";
  simpleAllScript << "set grid\n";
  simpleAllScript << "set key outside right box\n";
  simpleAllScript << "set border 3\n";
  simpleAllScript << "set tics nomirror\n";
  simpleAllScript << "set title 'SimpleArray - All Operations Comparison' font 'Arial,14'\n";
  simpleAllScript << "set xlabel 'Input Size' font 'Arial,12'\n";
  simpleAllScript << "set ylabel 'Time (ms)' font 'Arial,12'\n";
  simpleAllScript << "plot 'data/time_SimpleArray_Insert.txt' using 1:2 with linespoints ls 1 title 'Insert', \\\n";
  simpleAllScript << "     'data/time_SimpleArray_Search.txt' using 1:2 with linespoints ls 2 title 'Search', \\\n";
  simpleAllScript << "     'data/time_SimpleArray_Remove.txt' using 1:2 with linespoints ls 3 title 'Remove', \\\n";
  simpleAllScript << "     'data/time_SimpleArray_Union.txt' using 1:2 with linespoints ls 4 title 'Union', \\\n";
  simpleAllScript << "     'data/time_SimpleArray_Intersection.txt' using 1:2 with linespoints ls 5 title 'Intersection'\n";
  simpleAllScript.close();
}

void runGnuplotScripts() {
  system("gnuplot data/plot_insert_time.gnu");
  system("gnuplot data/plot_search_time.gnu");
  system("gnuplot data/plot_remove_time.gnu");
  system("gnuplot data/plot_union_time.gnu");
  system("gnuplot data/plot_intersection_time.gnu");
  system("gnuplot data/plot_insert_memory.gnu");
  system("gnuplot data/plot_log_time.gnu");
  system("gnuplot data/plot_linked_all.gnu");
  system("gnuplot data/plot_simple_all.gnu");
}

int main() {
  createDirectoryIfNotExists("data");
  createDirectoryIfNotExists("charts");

  std::cout << "Running benchmarks...\n";

  SetLinkedBenchmark linkedBench("LinkedList");
  linkedBench.runAllTests();

  SetSimpleBenchmark simpleBench("SimpleArray");
  simpleBench.runAllTests();

  DictionarySimpleBenchmark dictBench("Dictionary");
  dictBench.runAllTests();

  std::cout << "Benchmarks completed. Data saved to data/ directory.\n";

  std::cout << "Generating comparison scripts...\n";
  generateComparisonScripts();

  std::cout << "Running gnuplot scripts...\n";
  runGnuplotScripts();

  return 0;
}
