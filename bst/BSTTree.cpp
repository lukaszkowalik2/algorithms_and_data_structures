#include <iomanip>
#include <iostream>
#include <memory>
#include <queue>
#include <vector>

struct Node {
  int value;
  std::unique_ptr<Node> left;
  std::unique_ptr<Node> right;

  Node(int val)
      : value(val), left(nullptr), right(nullptr) {
  }
};

class BSTTree {
  std::unique_ptr<Node> root;

 private:
  void insert(int value, std::unique_ptr<Node>& node) {
    if (!node) {
      node = std::make_unique<Node>(value);
    } else if (value < node->value) {
      insert(value, node->left);
    } else if (value > node->value) {
      insert(value, node->right);
    }
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
      if (!node->left) {
        node = std::move(node->right);
      } else if (!node->right) {
        node = std::move(node->left);
      } else {
        auto& minNode = findMin(node->right);
        node->value = minNode->value;
        deleteNode(minNode->value, node->right);
      }
    }
  }

 public:
  BSTTree()
      : root(nullptr) {
  }

  void insert(int value) {
    insert(value, root);
  }

  bool contains(int value) const {
    return contains(value, root);
  }

  void deleteNode(int value) {
    deleteNode(value, root);
  }

  void printTree() const {
    if (!root) {
      std::cout << "Puste drzewo\n";
      return;
    }

    std::queue<const Node*> nodes;
    nodes.push(root.get());
    std::vector<std::vector<std::string>> levels;

    while (!nodes.empty()) {
      size_t size = nodes.size();
      std::vector<std::string> level;

      for (size_t i = 0; i < size; ++i) {
        const Node* current = nodes.front();
        nodes.pop();

        if (current) {
          level.push_back(std::to_string(current->value));
          nodes.push(current->left.get());
          nodes.push(current->right.get());
        } else {
          level.push_back(" ");
        }
      }

      bool hasValidNodes = false;
      for (const auto& val : level) {
        if (val != " ") {
          hasValidNodes = true;
          break;
        }
      }
      if (hasValidNodes) {
        levels.push_back(level);
      }
    }

    size_t nodeWidth = 4;
    size_t spacing = 2;

    for (size_t i = 0; i < levels.size(); ++i) {
      size_t padding = (1 << (levels.size() - i - 1)) * (nodeWidth / 2);

      std::cout << std::string(padding, ' ');

      for (const auto& value : levels[i]) {
        std::cout << std::setw(nodeWidth) << value;
        std::cout << std::string(spacing, ' ');
      }
      std::cout << '\n';
    }
  }
};

int main() {
  BSTTree tree;

  tree.insert(10);
  tree.insert(5);
  tree.insert(20);
  tree.insert(15);

  tree.printTree();

  std::cout << "Zawiera 10: " << (tree.contains(10) ? "Tak" : "Nie") << std::endl;
  std::cout << "Zawiera 25: " << (tree.contains(25) ? "Tak" : "Nie") << std::endl;

  tree.deleteNode(10);
  tree.printTree();

  return 0;
}
