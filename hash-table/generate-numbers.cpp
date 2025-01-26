#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <number_of_numbers>\n";
    return EXIT_FAILURE;
  }

  int count = std::atoi(argv[1]);
  if (count <= 0) {
    std::cerr << "Number of numbers must be positive\n";
    return EXIT_FAILURE;
  }

  std::srand(std::time(nullptr));
  std::ofstream outFile("numbers.txt");

  if (!outFile.is_open()) {
    std::cerr << "Failed to open numbers.txt for writing\n";
    return EXIT_FAILURE;
  }

  for (int i = 0; i < count; ++i) {
    outFile << (std::rand() % 100) << '\n';
  }

  outFile.close();
  return EXIT_SUCCESS;
}
