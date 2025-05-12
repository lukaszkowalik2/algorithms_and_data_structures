#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

std::unordered_set<std::string> load_dictionary(const std::string& filename) {
  std::unordered_set<std::string> dict;
  std::ifstream file(filename);
  
  if (!file.is_open()) {
    std::cerr << "Błąd: Nie można otworzyć pliku słownika: " << filename << std::endl;
    return dict;
  }

  std::string word;
  while (file >> word) {
    if (word.length() == 4) {
      for (char& c : word) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
      }
      dict.insert(word);
    }
  }
  file.close();
  return dict;
}

std::vector<std::string> find_shortest_path(
    const std::string& start_word_orig,
    const std::string& end_word_orig,
    const std::unordered_set<std::string>& dictionary) {
  std::vector<std::string> path;

  std::string start_word = start_word_orig;
  std::string end_word = end_word_orig;

  for (char& c : start_word) {
    c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
  }
  for (char& c : end_word) {
    c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
  }

  if (dictionary.find(start_word) == dictionary.end()) {
    std::cerr << "Błąd: Słowo startowe \"" << start_word_orig
              << "\" nie znajduje się w słowniku 4-literowych słów." << std::endl;
    return path;
  }
  if (dictionary.find(end_word) == dictionary.end()) {
    std::cerr << "Błąd: Słowo końcowe \"" << end_word_orig
              << "\" nie znajduje się w słowniku 4-literowych słów." << std::endl;
    return path;
  }

  if (start_word == end_word) {
    path.push_back(start_word);
    return path;
  }

  std::queue<std::string> q;
  std::unordered_map<std::string, std::string> predecessor;
  std::unordered_set<std::string> visited;

  q.push(start_word);
  visited.insert(start_word);

  bool found = false;
  while (!q.empty() && !found) {
    std::string current_word = q.front();
    q.pop();

    for (int i = 0; i < 4; ++i) {
      std::string temp_word = current_word;
      char original_char = temp_word[i];

      for (char c_val = 'a'; c_val <= 'z'; ++c_val) {
        if (c_val == original_char) {
          continue;
        }
        temp_word[i] = c_val;

        if (dictionary.count(temp_word)) {
          if (visited.find(temp_word) == visited.end()) {
            visited.insert(temp_word);
            predecessor[temp_word] = current_word;
            q.push(temp_word);

            if (temp_word == end_word) {
              found = true;
              break;
            }
          }
        }
      }
      if (found) {
        break;
      }
    }
  }

  if (found) {
    std::string curr = end_word;
    while (true) {
      path.push_back(curr);
      if (curr == start_word) {
        break;
      }
      curr = predecessor.at(curr);
    }
    std::reverse(path.begin(), path.end());
  }
  return path;
}

int main() {
  std::unordered_set<std::string> dictionary = load_dictionary("slowa.txt");

  if (dictionary.empty()) {
    std::cout << "Słownik jest pusty lub nie zawiera słów 4-literowych. "
              << "Nie można znaleźć ścieżek." << std::endl;
    return 1;
  }

  std::string start_word_input, end_word_input;
  std::cout << "Podaj słowo startowe (4 litery): ";
  std::cin >> start_word_input;
  std::cout << "Podaj słowo końcowe (4 litery): ";
  std::cin >> end_word_input;

  if (start_word_input.length() != 4) {
    std::cerr << "Błąd: Słowo startowe \"" << start_word_input
              << "\" nie ma 4 liter." << std::endl;
    return 1;
  }
  if (end_word_input.length() != 4) {
    std::cerr << "Błąd: Słowo końcowe \"" << end_word_input
              << "\" nie ma 4 liter." << std::endl;
    return 1;
  }

  std::vector<std::string> shortest_path = find_shortest_path(
      start_word_input, end_word_input, dictionary);

  if (shortest_path.empty()) {
    std::string temp_s = start_word_input;
    for (char& c : temp_s) {
      c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    std::string temp_e = end_word_input;
    for (char& c : temp_e) {
      c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    if (dictionary.count(temp_s) && dictionary.count(temp_e)) {
      std::cout << "Nie znaleziono ścieżki od " << start_word_input
                << " do " << end_word_input << "." << std::endl;
    }
  } else {
    std::cout << "Najkrótsza ścieżka: ";
    for (size_t i = 0; i < shortest_path.size(); ++i) {
      std::cout << shortest_path[i]
                << (i == shortest_path.size() - 1 ? "" : " -> ");
    }
    std::cout << std::endl;
  }

  return 0;
}