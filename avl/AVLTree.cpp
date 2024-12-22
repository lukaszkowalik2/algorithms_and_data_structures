#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <vector>

#include "TreePrinter.h"

class AVLTree {
  std::unique_ptr<Node> root;

 private:
  int getHeight(const std::unique_ptr<Node>& node) const {
    return node ? node->height : 0;
  }

  int getBalanceFactor(const std::unique_ptr<Node>& node) const {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
  }

  void updateHeight(std::unique_ptr<Node>& node) {
    if (node) {
      node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
    }
  }

  std::unique_ptr<Node> rotateRight(std::unique_ptr<Node>& y) {
    std::unique_ptr<Node> x = std::move(y->left);
    std::unique_ptr<Node> T2 = std::move(x->right);

    x->right = std::move(y);
    x->right->left = std::move(T2);

    updateHeight(x->right);
    updateHeight(x);

    return x;
  }

  std::unique_ptr<Node> rotateLeft(std::unique_ptr<Node>& x) {
    std::unique_ptr<Node> y = std::move(x->right);
    std::unique_ptr<Node> T2 = std::move(y->left);

    y->left = std::move(x);
    y->left->right = std::move(T2);

    updateHeight(y->left);
    updateHeight(y);

    return y;
  }

  std::unique_ptr<Node> balance(std::unique_ptr<Node>& node) {
    if (!node) {
      return nullptr;
    }

    updateHeight(node);

    int balanceFactor = getBalanceFactor(node);

    // Left Left Case
    if (balanceFactor > 1 && getBalanceFactor(node->left) >= 0) {
      return rotateRight(node);
    }

    // Right Right Case
    if (balanceFactor < -1 && getBalanceFactor(node->right) <= 0) {
      return rotateLeft(node);
    }

    // Left Right Case
    if (balanceFactor > 1 && getBalanceFactor(node->left) < 0) {
      node->left = rotateLeft(node->left);
      return rotateRight(node);
    }

    // Right Left Case
    if (balanceFactor < -1 && getBalanceFactor(node->right) > 0) {
      node->right = rotateRight(node->right);
      return rotateLeft(node);
    }

    return std::move(node);
  }

  void insert(int value, std::unique_ptr<Node>& node) {
    if (!node) {
      node = std::make_unique<Node>(value);
      return;
    }

    if (value < node->value) {
      insert(value, node->left);
    } else if (value > node->value) {
      insert(value, node->right);
    } else {
      return;
    }

    node = balance(node);
  }

  bool contains(int value, const std::unique_ptr<Node>& node) const {
    if (!node) {
      return false;
    }
    if (node->value == value) {
      return true;
    }
    if (value < node->value) {
      return contains(value, node->left);
    }
    return contains(value, node->right);
  }

  std::unique_ptr<Node>& findMin(std::unique_ptr<Node>& node) {
    if (!node->left) {
      return node;
    }
    return findMin(node->left);
  }

  void deleteNode(int value, std::unique_ptr<Node>& node) {
    if (!node) {
      return;
    }

    if (value < node->value) {
      deleteNode(value, node->left);
    } else if (value > node->value) {
      deleteNode(value, node->right);
    } else {
      if (!node->left || !node->right) {
        std::unique_ptr<Node> temp = node->left ? std::move(node->left) : std::move(node->right);
        if (!temp) {
          node = nullptr;
        } else {
          node = std::move(temp);
        }
      } else {
        auto& minNode = findMin(node->right);
        node->value = minNode->value;
        deleteNode(minNode->value, node->right);
      }
    }

    if (!node) {
      return;
    }

    node = balance(node);
  }

 public:
  void insert(int value) {
    insert(value, root);
  }

  bool contains(int value) const {
    return contains(value, root);
  }

  void deleteNode(int value) {
    deleteNode(value, root);
  }

  void printTree() {
    if (!root) {
      std::cout << "Puste drzewo\n";
      return;
    }
    std::vector<std::vector<std::string>> result = treeToMatrix(root);

    print2DArray(result);
  }
};

int main() {
  AVLTree tree;

  tree.insert(5);
  tree.insert(1);
  tree.insert(20);
  tree.insert(15);
  tree.insert(123);
  tree.insert(31);
  tree.insert(1335);
  tree.printTree();
  tree.insert(10);
  tree.insert(2);
  tree.insert(3);
  tree.insert(4);

  tree.printTree();

  std::cout << "Zawiera 10: " << (tree.contains(10) ? "Tak" : "Nie") << std::endl;
  std::cout << "Zawiera 25: " << (tree.contains(25) ? "Tak" : "Nie") << std::endl;

  tree.deleteNode(10);
  tree.printTree();

  return 0;
}
