#ifndef TREE_PRINTER_H
#define TREE_PRINTER_H

#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Node {
  int value;
  int height;
  std::unique_ptr<Node> left;
  std::unique_ptr<Node> right;

  Node(int v)
      : value(v),
        height(1),
        left(nullptr),
        right(nullptr) {
  }
};

// Printing tree implemented from https://www.geeksforgeeks.org/print-binary-tree-2-dimensions/

int findHeight(const std::unique_ptr<Node>& root) {
  if (!root) {
    return -1;
  }

  int leftHeight = findHeight(root->left);
  int rightHeight = findHeight(root->right);

  return std::max(leftHeight, rightHeight) + 1;
}

void inorder(const std::unique_ptr<Node>& root, int row, int col, int height,
             std::vector<std::vector<std::string>>& ans) {
  if (!root) {
    return;
  }

  int offset = std::pow(2, height - row - 1);

  if (root->left) {
    inorder(root->left, row + 1, col - offset, height, ans);
  }

  ans[row][col] = std::to_string(root->value);

  if (root->right) {
    inorder(root->right, row + 1, col + offset, height, ans);
  }
}

std::vector<std::vector<std::string>> treeToMatrix(const std::unique_ptr<Node>& root) {
  int height = findHeight(root);
  int rows = height + 1;
  int cols = std::pow(2, height + 1) - 1;

  std::vector<std::vector<std::string>> ans(rows, std::vector<std::string>(cols, ""));
  inorder(root, 0, (cols - 1) / 2, height, ans);
  return ans;
}

void print2DArray(const std::vector<std::vector<std::string>>& arr) {
  for (const auto& row : arr) {
    for (const auto& cell : row) {
      if (cell.empty()) {
        std::cout << " ";
      } else {
        std::cout << cell;
      }
    }
    std::cout << std::endl;
  }
}

#endif  // TREE_PRINTER_H