#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

using namespace std;

vector<int> sortKMessedArray( const vector<int>& arr, int k ) 
{
  // your code goes here

  // O(N*k)
  vector<int> ret;

  // track all indices we've used in arr
  vector<bool> seen(arr.size(), false);

  for (int i = 0; i < arr.size(); i++) {
    int min_value = INT_MAX;
    int min_index = -1;
    for (int j = std::max(i - k, 0); j <= std::min(i + k, static_cast<int>(arr.size() - 1)); j++) {
      if (!seen[j] && (arr[j] < min_value)) {
        min_value = arr[j];
        min_index = j;
      }
    }

    // at this point we know 
    seen[min_index] = true;
    ret.push_back(min_value);
  }

  return ret;
}

// comment out main() before running tests
int main() {
    // debug your code below
    vector<int> arr = { 6, 1, 4, 11, 2, 0, 3, 7, 10, 5, 8, 9}; 
    int k = 6;
    vector<int> result = sortKMessedArray(arr, k);

    cout << "Sorted KMessed Array: ";
    for (int num : result) {
        cout << num << " ";
    }
    cout << endl;

    return 0;
}
