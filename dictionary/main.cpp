#include <iostream>

#include "cursor_doubly_linked_list.cpp"
#include "cursor_doubly_linked_list_vector.cpp"
#include "cursor_singly_linked_list.cpp"
#include "cursor_singly_linked_list_vector.cpp"
#include "doubly_linked_list.cpp"
#include "singly_linked_list.cpp"

int main() {
  SinglyLinkedList singlyList;
  std::cout << "Singly Linked List" << std::endl;
  singlyList.insert(10);
  singlyList.insert(20);
  singlyList.insert(30);

  std::cout << "Contains 20: " << (singlyList.contains(20) ? "YES" : "NO") << std::endl;
  singlyList.remove(20);
  std::cout << "Contains 20 after deletion: " << (singlyList.contains(20) ? "YES" : "NO") << "\n";
  std::cout << "Singly Linked List contains: ";
  singlyList.printList();
  std::cout << std::endl;

  DoublyLinkedList doublyList;
  std::cout << "Doubly Linked List" << std::endl;
  doublyList.insert(10);
  doublyList.insert(20);
  doublyList.insert(30);

  std::cout << "Contains 20: " << (doublyList.contains(20) ? "YES" : "NO") << std::endl;
  doublyList.remove(20);
  std::cout << "Contains 20 after deletion: " << (doublyList.contains(20) ? "YES" : "NO") << "\n";
  std::cout << "Doubly Linked List contains: ";
  doublyList.printList();
  std::cout << std::endl;

  CursorSinglyLinkedListVector cursorSinglyListVector;
  std::cout << "Cursor Singly Linked List (Vector)" << std::endl;
  cursorSinglyListVector.insert(10);
  cursorSinglyListVector.insert(20);
  cursorSinglyListVector.insert(30);

  std::cout << "Contains 20: " << (cursorSinglyListVector.contains(20) ? "YES" : "NO") << std::endl;
  cursorSinglyListVector.remove(20);
  std::cout << "Contains 20 after deletion: " << (cursorSinglyListVector.contains(20) ? "YES" : "NO") << "\n";
  std::cout << "Cursor Singly Linked List (Vector) contains: ";
  cursorSinglyListVector.printList();
  std::cout << std::endl;

  CursorSinglyLinkedList cursorSinglyList;
  std::cout << "Cursor Singly Linked List (Array)" << std::endl;
  cursorSinglyList.insert(15);
  cursorSinglyList.insert(25);
  cursorSinglyList.insert(35);

  std::cout << "Contains 25: " << (cursorSinglyList.contains(25) ? "YES" : "NO") << std::endl;
  cursorSinglyList.remove(25);
  std::cout << "Contains 25 after deletion: " << (cursorSinglyList.contains(25) ? "YES" : "NO") << "\n";
  std::cout << "Cursor Singly Linked List (Array) contains: ";
  cursorSinglyList.print();
  std::cout << std::endl;

  CursorDoublyLinkedList cursorDoublyList;
  std::cout << "Cursor Doubly Linked List (Array)" << std::endl;
  cursorDoublyList.insert(40);
  cursorDoublyList.insert(50);
  cursorDoublyList.insert(60);

  std::cout << "Contains 50: " << (cursorDoublyList.contains(50) ? "YES" : "NO") << std::endl;
  cursorDoublyList.remove(50);
  std::cout << "Contains 50 after deletion: " << (cursorDoublyList.contains(50) ? "YES" : "NO") << "\n";
  std::cout << "Cursor Doubly Linked List (Array) contains: ";
  cursorDoublyList.print();
  std::cout << std::endl;

  CursorDoublyLinkedListVector cursorDoublyListVector;
  std::cout << "Cursor Doubly Linked List (Vector)" << std::endl;
  cursorDoublyListVector.insert(45);
  cursorDoublyListVector.insert(55);
  cursorDoublyListVector.insert(65);

  std::cout << "Contains 55: " << (cursorDoublyListVector.contains(55) ? "YES" : "NO") << std::endl;
  cursorDoublyListVector.remove(55);
  std::cout << "Contains 55 after deletion: " << (cursorDoublyListVector.contains(55) ? "YES" : "NO") << "\n";
  std::cout << "Cursor Doubly Linked List (Vector) contains: ";
  cursorDoublyListVector.printList();
  std::cout << std::endl;

  return 0;
}
