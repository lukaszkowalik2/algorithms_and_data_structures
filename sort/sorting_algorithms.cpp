#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

// Function to swap two elements by reference
void swap(int* a, int* b) {
  int t = *a;  // Temporarily store the value of *a
  *a = *b;     // Assign the value of *b to *a
  *b = t;      // Assign the temporary value to *b, completing the swap
}

// Function implementing Bubble Sort algorithm
void bubbleSort(std::vector<int>& arr) {
  size_t length = arr.size();  // Get the size of the array

  // Loop through each element in the array
  for (int i = 0; i < length - 1; i++) {
    // Inner loop to compare adjacent elements
    for (int j = 0; j < length - i - 1; j++) {
      // Swap if the current element is greater than the next element
      if (arr[j] > arr[j + 1]) {
        swap(&arr[j], &arr[j + 1]);
      }
    }
  }
}

// Function implementing Insertion Sort algorithm
void insertionSort(std::vector<int>& arr) {
  size_t length = arr.size();  // Get the size of the array

  // Loop through each element starting from the second
  for (int i = 1; i < length; i++) {
    int key = arr[i];  // Store the current element
    int j = i - 1;

    // Shift elements that are greater than the key to one position ahead
    while (j >= 0 && arr[j] > key) {
      arr[j + 1] = arr[j];
      j = j - 1;
    }
    arr[j + 1] = key;  // Insert the key into the correct position
  }
}

// Partition function used in Quick Sort
int partition(std::vector<int>& arr, int low, int high) {
  int pivot = arr[high];  // Select the last element as the pivot
  int i = (low - 1);      // Index of the smaller element

  // Traverse through each element in the current sub-array
  for (int j = low; j <= high - 1; j++) {
    if (arr[j] < pivot) {
      i++;                     // Increment index of smaller element
      swap(&arr[i], &arr[j]);  // Swap current element with the element at index i
    }
  }
  swap(&arr[i + 1], &arr[high]);  // Place the pivot in the correct position
  return (i + 1);                 // Return the partitioning index
}

// Function implementing Quick Sort algorithm
void quickSort(std::vector<int>& arr, int low, int high) {
  if (low < high) {
    int pi = partition(arr, low, high);  // Partition the array and get pivot index

    // Recursively sort elements before and after the pivot
    quickSort(arr, low, pi - 1);
    quickSort(arr, pi + 1, high);
  }
}

// Function implementing Selection Sort algorithm
void selectionSort(std::vector<int>& arr) {
  int length = arr.size();  // Get the size of the array

  // Loop through each element in the array
  for (int i = 0; i < length - 1; ++i) {
    int min_idx = i;  // Assume the current element is the minimum

    // Find the smallest element in the remaining unsorted array
    for (int j = i + 1; j < length; ++j) {
      if (arr[j] < arr[min_idx]) {
        min_idx = j;
      }
    }

    swap(&arr[i], &arr[min_idx]);  // Swap the found minimum element with the current element
  }
}

// Function to create a max heap for Heap Sort
void heapify(std::vector<int>& arr, int n, int i) {
  int largest = i;    // Assume the root is the largest
  int l = 2 * i + 1;  // Left child index
  int r = 2 * i + 2;  // Right child index

  // If left child exists and is greater than the root
  if (l < n && arr[l] > arr[largest])
    largest = l;

  // If right child exists and is greater than the largest so far
  if (r < n && arr[r] > arr[largest])
    largest = r;

  // If the largest is not the root, swap and continue heapifying
  if (largest != i) {
    swap(&arr[i], &arr[largest]);
    heapify(arr, n, largest);  // Recursively heapify the affected subtree
  }
}

// Function implementing Heap Sort algorithm
void heapSort(std::vector<int>& arr) {
  size_t length = arr.size();

  // Build max heap
  for (int i = length / 2 - 1; i >= 0; i--)
    heapify(arr, length, i);

  // Extract elements from heap one by one
  for (int i = length - 1; i >= 0; i--) {
    swap(&arr[0], &arr[i]);  // Move current root to end
    heapify(arr, i, 0);      // Call max heapify on the reduced heap
  }
}

int main() {
  std::vector<int> sizes = {10000, 25000, 50000, 100000};
  std::vector<std::string> algorithms = {"BubbleSort", "InsertionSort", "QuickSort", "SelectionSort", "HeapSort"};

  std::ofstream dataFile("benchmark_data.dat");
  if (!dataFile.is_open()) {
    std::cerr << "Nie udało się otworzyć pliku benchmark_data.dat do zapisu." << std::endl;
    return 1;
  }

  for (const auto& algo : algorithms) {
    dataFile << algo;

    for (int size : sizes) {
      std::vector<int> arr(size);
      for (int i = 0; i < size; i++) {
        arr[i] = rand() % size;
      }
      auto arr_copy = arr;

      auto start = std::chrono::high_resolution_clock::now();

      if (algo == "BubbleSort")
        bubbleSort(arr_copy);
      else if (algo == "InsertionSort")
        insertionSort(arr_copy);
      else if (algo == "QuickSort")
        quickSort(arr_copy, 0, size - 1);
      else if (algo == "SelectionSort")
        selectionSort(arr_copy);
      else if (algo == "HeapSort")
        heapSort(arr_copy);

      auto end = std::chrono::high_resolution_clock::now();
      auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

      dataFile << " " << time;
    }
    dataFile << "\n";
  }

  dataFile.close();

  system("gnuplot plot_script.gp");

  return 0;
}
