#include <iostream>
#include <string>
#include <vector>

#include "TreePrinter.h"

class SplayTree {
  Node* root;

 private:
  Node* rotateLeft(Node* node) {
    if (!node || !node->right) {
      return node;
    }

    Node* temp = node->right;
    node->right = temp->left;
    temp->left = node;
    return temp;
  }

  Node* rotateRight(Node* node) {
    if (!node || !node->left) {
      return node;
    }

    Node* temp = node->left;
    node->left = temp->right;
    temp->right = node;
    return temp;
  }

  Node* splay(int value, Node* node) {
    if (!node || node->value == value) {
      return node;
    }

    if (value < node->value) {
      if (!node->left) {
        return node;
      }

      if (value < node->left->value) {
        // Zig-Zig case
        node->left->left = splay(value, node->left->left);
        node = rotateRight(node);
      } else if (value > node->left->value) {
        // Zig-Zag case
        node->left->right = splay(value, node->left->right);
        if (node->left->right) {
          node->left = rotateLeft(node->left);
        }
      }

      if (node->left) {
        // Zig case
        node = rotateRight(node);
      }
    } else {
      if (!node->right) {
        return node;
      }

      if (value > node->right->value) {
        // Zag-Zag case
        node->right->right = splay(value, node->right->right);
        node = rotateLeft(node);
      } else if (value < node->right->value) {
        // Zag-Zig case
        node->right->left = splay(value, node->right->left);
        if (node->right->left) {
          node->right = rotateRight(node->right);
        }
      }

      if (node->right) {
        // Zag case
        node = rotateLeft(node);
      }
    }

    return node;
  }

  void insert(int value, Node*& node) {
    if (!node) {
      node = new Node(value);
      return;
    }

    node = splay(value, node);

    if (value < node->value) {
      insert(value, node->left);
    } else if (value > node->value) {
      insert(value, node->right);
    }
  }

  bool contains(int value, Node* node) const {
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

  Node*& findMin(Node*& node) {
    if (!node->left) {
      return node;
    }
    return findMin(node->left);
  }

  Node* find(int value, Node*& node) {
    if (!node || node->value == value) {
      if (node) {
        return new Node(node->value);
      }
      return nullptr;
    }

    Node* result = nullptr;
    if (value < node->value) {
      result = find(value, node->left);
    } else {
      result = find(value, node->right);
    }

    if (result) {
      node = splay(value, node);
    }
    return result;
  }

  void deleteNode(int value, Node*& node) {
    if (!node) {
      return;
    }

    if (value < node->value) {
      deleteNode(value, node->left);
    } else if (value > node->value) {
      deleteNode(value, node->right);
    } else {
      if (!node->left || !node->right) {
        Node* temp = node->left ? node->left : node->right;
        delete node;
        node = temp;
      } else {
        Node*& minNode = findMin(node->right);
        node->value = minNode->value;
        deleteNode(minNode->value, node->right);
      }
    }

    if (node) {
      node = splay(value, node);
    }
  }

  void deleteTree(Node* node) {
    if (node) {
      deleteTree(node->left);
      deleteTree(node->right);
      delete node;
    }
  }

 public:
  SplayTree()
      : root(nullptr) {
  }

  ~SplayTree() {
    deleteTree(root);
  }

  void insert(int value) {
    insert(value, root);
  }

  bool contains(int value) const {
    return contains(value, root);
  }

  Node* find(int value) {
    return find(value, root);
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
  std::cout << "Zig case (right rotation):\n";
  SplayTree zigTree;
  zigTree.insert(3);
  zigTree.insert(2);
  zigTree.insert(1);
  zigTree.printTree();
  zigTree.find(1);
  std::cout << "\nAfter splaying 1:\n";
  zigTree.printTree();

  std::cout << "\nZag case (left rotation):\n";
  SplayTree zagTree;
  zagTree.insert(1);
  zagTree.insert(2);
  zagTree.insert(3);
  zagTree.printTree();
  zagTree.find(3);
  std::cout << "\nAfter splaying 3:\n";
  zagTree.printTree();

  std::cout << "\nZig-zig case:\n";
  SplayTree zigZigTree;
  zigZigTree.insert(4);
  zigZigTree.insert(3);
  zigZigTree.insert(2);
  zigZigTree.insert(1);
  zigZigTree.printTree();
  zigZigTree.find(1);
  std::cout << "\nAfter splaying 1:\n";
  zigZigTree.printTree();

  std::cout << "\nZag-zag case:\n";
  SplayTree zagZagTree;
  zagZagTree.insert(1);
  zagZagTree.insert(2);
  zagZagTree.insert(3);
  zagZagTree.insert(4);
  zagZagTree.printTree();
  zagZagTree.find(4);
  std::cout << "\nAfter splaying 4:\n";
  zagZagTree.printTree();

  std::cout << "\nZig-zag case:\n";
  SplayTree zigZagTree;
  zigZagTree.insert(3);
  zigZagTree.insert(1);
  zigZagTree.insert(2);
  zigZagTree.printTree();
  zigZagTree.find(2);
  std::cout << "\nAfter splaying 2:\n";
  zigZagTree.printTree();

  std::cout << "\nZag-zig case:\n";
  SplayTree zagZigTree;
  zagZigTree.insert(1);
  zagZigTree.insert(3);
  zagZigTree.insert(2);
  zagZigTree.printTree();
  zagZigTree.find(2);
  std::cout << "\nAfter splaying 2:\n";
  zagZigTree.printTree();

  SplayTree insertTree;
  insertTree.insert(5);
  insertTree.insert(3);
  insertTree.insert(7);
  insertTree.insert(2);
  insertTree.insert(4);
  insertTree.insert(1);
  std::cout << "Tree after balanced insertions:\n";
  insertTree.printTree();

  std::cout << "Contains 4: " << (insertTree.contains(4) ? "true" : "false") << "\n";
  std::cout << "Contains 6: " << (insertTree.contains(6) ? "true" : "false") << "\n";

  SplayTree deleteTree;
  deleteTree.insert(5);
  deleteTree.insert(3);
  deleteTree.insert(7);
  deleteTree.insert(2);
  deleteTree.insert(4);
  deleteTree.insert(6);
  deleteTree.insert(8);
  std::cout << "Initial tree:\n";
  deleteTree.printTree();

  deleteTree.deleteNode(5);
  deleteTree.deleteNode(2);
  deleteTree.deleteNode(7);
  std::cout << "After deleting:\n";
  deleteTree.printTree();

  return 0;
}