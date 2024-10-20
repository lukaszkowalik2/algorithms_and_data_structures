#include <algorithm>
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

void swap(int* a, int* b) {
  int t = *a;
  *a = *b;
  *b = t;
}

void bubbleSort(int arr[], int n) {
  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - i - 1; j++) {
      if (arr[j] > arr[j + 1]) {
        swap(&arr[j], &arr[j + 1]);
      }
    }
  }
}

void insertionSort(int arr[], int n) {
  for (int i = 1; i < n; i++) {
    int key = arr[i];
    int j = i - 1;
    while (j >= 0 && arr[j] > key) {
      arr[j + 1] = arr[j];
      j = j - 1;
    }
    arr[j + 1] = key;
  }
}

int partition(int arr[], int low, int high) {
  int pivot = arr[high];
  int i = (low - 1);
  for (int j = low; j <= high - 1; j++) {
    if (arr[j] < pivot) {
      i++;
      swap(&arr[i], &arr[j]);
    }
  }
  swap(&arr[i + 1], &arr[high]);
  return (i + 1);
}

void quickSort(int arr[], int low, int high) {
  if (low < high) {
    int pi = partition(arr, low, high);
    quickSort(arr, low, pi - 1);
    quickSort(arr, pi + 1, high);
  }
}

void merge(int arr[], int l, int m, int r) {
  int n1 = m - l + 1;
  int n2 = r - m;

  int* L = new int[n1];
  int* R = new int[n2];

  for (int i = 0; i < n1; i++)
    L[i] = arr[l + i];
  for (int j = 0; j < n2; j++)
    R[j] = arr[m + 1 + j];

  int i = 0, j = 0, k = l;
  while (i < n1 && j < n2) {
    if (L[i] <= R[j]) {
      arr[k++] = L[i++];
    } else {
      arr[k++] = R[j++];
    }
  }
  while (i < n1)
    arr[k++] = L[i++];
  while (j < n2)
    arr[k++] = R[j++];

  delete[] L;
  delete[] R;
}

void mergeSort(int arr[], int l, int r) {
  if (l < r) {
    int m = l + (r - l) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
  }
}

void printArray(int arr[], int size) {
  for (int i = 0; i < size; i++)
    cout << arr[i] << " ";
  cout << endl;
}

int main() {
  const int size = 10000;
  int arr1[size], arr2[size], arr3[size], arr4[size];

  for (int i = 0; i < size; i++) {
    int value = rand() % size;
    arr1[i] = value;
    arr2[i] = value;
    arr3[i] = value;
    arr4[i] = value;
  }

  auto start = high_resolution_clock::now();
  bubbleSort(arr1, size);
  auto stop = high_resolution_clock::now();
  auto bubble_duration = duration_cast<milliseconds>(stop - start);
  cout << "Czas wykonania Bubble Sort: " << bubble_duration.count() << " ms" << endl;

  start = high_resolution_clock::now();
  insertionSort(arr2, size);
  stop = high_resolution_clock::now();
  auto insertion_duration = duration_cast<milliseconds>(stop - start);
  cout << "Czas wykonania Insertion Sort: " << insertion_duration.count() << " ms" << endl;

  start = high_resolution_clock::now();
  quickSort(arr3, 0, size - 1);
  stop = high_resolution_clock::now();
  auto quick_duration = duration_cast<milliseconds>(stop - start);
  cout << "Czas wykonania Quick Sort: " << quick_duration.count() << " ms" << endl;

  start = high_resolution_clock::now();
  mergeSort(arr4, 0, size - 1);
  stop = high_resolution_clock::now();
  auto merge_duration = duration_cast<milliseconds>(stop - start);
  cout << "Czas Merge Sort: " << merge_duration.count() << " ms" << endl;

  return 0;
}
