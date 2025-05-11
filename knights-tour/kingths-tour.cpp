#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <string>
#include <algorithm>
#include <filesystem>
#include <cstdlib>

const int N = 8;
const Point START_POSITION = {7, 7};

struct Point {
  int x, y;
  bool operator<(const Point& other) const {
    if (x != other.x) return x < other.x;
    return y < other.y;
  }
};

bool isSafe(int x, int y, const std::vector<std::vector<int>>& board) {
  return (x >= 0 && x < N && y >= 0 && y < N && board[x][y] == 0);
}

bool solveKTUtil(Point currentPos, int moveCount,
                 std::vector<std::vector<int>>& board,
                 const std::vector<Point>& moveOffsets,
                 std::vector<Point>& path) {
  board[currentPos.x][currentPos.y] = moveCount;
  path.push_back(currentPos);

  if (moveCount == N * N) {
    return true;
  }

  std::vector<std::pair<int, Point>> nextMoves;

  for (const auto& offset : moveOffsets) {
    Point nextPos = {currentPos.x + offset.x, currentPos.y + offset.y};
    if (isSafe(nextPos.x, nextPos.y, board)) {
      int count = 0;
      for (const auto& nextOffset : moveOffsets) {
        Point futurePos = {nextPos.x + nextOffset.x,
                           nextPos.y + nextOffset.y};
        if (isSafe(futurePos.x, futurePos.y, board)) {
          count++;
        }
      }
      nextMoves.push_back({count, nextPos});
    }
  }

  std::sort(nextMoves.begin(), nextMoves.end());

  for (const auto& movePair : nextMoves) {
    Point nextPos = movePair.second;
    if (solveKTUtil(nextPos, moveCount + 1, board, moveOffsets, path)) {
      return true;
    }
  }

  board[currentPos.x][currentPos.y] = 0;
  path.pop_back();
  return false;
}

std::vector<Point> findKnightsTour(Point startPos) {
  std::vector<std::vector<int>> board(N, std::vector<int>(N, 0));
  std::vector<Point> path;

  std::vector<Point> moveOffsets = {{2, 1},   {1, 2},   {-1, 2},  {-2, 1},
                                    {-2, -1}, {-1, -2}, {1, -2},  {2, -1}};

  if (startPos.x < 0 || startPos.x >= N || startPos.y < 0 ||
      startPos.y >= N) {
    std::cerr << "Nieprawidłowa pozycja startowa." << std::endl;
    return path;
  }

  if (solveKTUtil(startPos, 1, board, moveOffsets, path)) {
    std::cout << "Znaleziono trasę skoczka:" << std::endl;
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        std::cout << std::setw(3) << board[i][j] << " ";
      }
      std::cout << std::endl;
    }
    return path;
  } else {
    std::cout << "Rozwiązanie nie istnieje dla punktu startowego ("
              << startPos.x << ", " << startPos.y << ")" << std::endl;
    return path;
  }
}

void generateBoardGraphviz(const std::vector<std::vector<int>>& boardData,
                          const std::string& filename, Point startPos) {
  if (boardData.empty() || boardData[0].empty()) {
    std::cout << "Dane planszy są puste, nie można wygenerować grafu."
              << std::endl;
    return;
  }

  std::system("mkdir -p charts");
  std::string dotPath = "charts/" + filename;
  std::string pngPath = "charts/" + filename.substr(0, filename.find_last_of('.')) + ".png";

  std::ofstream dotFile(dotPath);
  if (!dotFile.is_open()) {
    std::cerr << "Nie można otworzyć pliku do zapisu: " << dotPath
              << std::endl;
    return;
  }

  int rows = boardData.size();
  int cols = boardData[0].size();

  dotFile << "digraph KnightsTourBoard {" << std::endl;
  dotFile << "  node [shape=plaintext];" << std::endl;
  dotFile << "  board_viz [label=<" << std::endl;
  dotFile << "    <TABLE BORDER=\"1\" CELLBORDER=\"1\" CELLSPACING=\"0\" "
             "CELLPADDING=\"8\">"
          << std::endl;

  for (int i = 0; i < rows; ++i) {
    dotFile << "      <TR>" << std::endl;
    for (int j = 0; j < cols; ++j) {
      std::string cell_content = std::to_string(boardData[i][j]);
      if (boardData[i][j] == 0) {
        cell_content = " ";
      }

      std::string bgcolor;
      if (i == startPos.x && j == startPos.y) {
        bgcolor = " BGCOLOR=\"#90EE90\"";
      } else if ((i + j) % 2 == 0) {
        bgcolor = " BGCOLOR=\"#FFFFFF\"";
      } else {
        bgcolor = " BGCOLOR=\"#D3D3D3\"";
      }

      dotFile << "        <TD WIDTH=\"30\" HEIGHT=\"30\" ALIGN=\"CENTER\" VALIGN=\"MIDDLE\""
              << bgcolor << ">" << cell_content << "</TD>" << std::endl;
    }
    dotFile << "      </TR>" << std::endl;
  }

  dotFile << "    </TABLE>" << std::endl;
  dotFile << "  >];" << std::endl;
  dotFile << "}" << std::endl;

  dotFile.close();
  std::cout << "Plik Graphviz (plansza) '" << dotPath << "' został wygenerowany."
            << std::endl;

  std::string command = "dot -Tpng " + dotPath + " -o " + pngPath;
  int result = std::system(command.c_str());
  
  if (result == 0) {
    std::cout << "Obraz PNG został wygenerowany: " << pngPath << std::endl;
  } else {
    std::cerr << "Błąd podczas generowania obrazu PNG. Upewnij się, że Graphviz jest zainstalowany." << std::endl;
  }
}

int main() {
  std::vector<std::vector<int>> board(N, std::vector<int>(N, 0));
  std::vector<Point> path = findKnightsTour(START_POSITION);

  if (!path.empty()) {
    for (size_t i = 0; i < path.size(); ++i) {
      board[path[i].x][path[i].y] = i + 1;
    }
    generateBoardGraphviz(board, "knights_tour_board.dot", START_POSITION);
  }

  return 0;
} 