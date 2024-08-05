#include <algorithm>
#include <cmath>
#include <vector>
#include <iostream>

using namespace std;

// Compute speed of the car it moved between two positions and times.
// Assume invariant that time1 < time2 && pos1 < pos2
inline float compute_speed(int pos1, int pos2, int time1, int time2) {
    return static_cast<float>(pos2 - pos1) / static_cast<float>(time2 - time1);
}

int solution(vector<int> &A, vector<int> &B, int Z) {
    // We assume moving between 2 positions the car has a constant speed
    int N = A.size();

    if (Z < A[0]) {
        // Use first two indices to extrapolate speed
        float speed = compute_speed(B[0], B[1], A[0], A[1]);
        return std::round(B[0] - speed * (A[0] - Z));
    } else if (Z > A[N - 1]) {
        // Use last two indices to extrapolate speed
        float speed = compute_speed(B[N - 2] , B[N - 1], A[N - 2], A[N - 1]);
        return std::round(B[N - 1] + speed * (Z - A[N - 1]));
    }

    // Binary search to find the location in the array
    int index =  std::distance(A.begin(), std::lower_bound(A.begin(), A.end(), Z));

    // If the exact time is found we can return that
    if (A[index] == Z) return B[index];

    // Index now points to the first element larger than it
    float speed = compute_speed(B[index - 1], B[index], A[index - 1], A[index]);
    return std::round(B[index - 1] + speed * (Z - A[index - 1]));
}

int main() {
    // Test Case 1
    vector<int> A1 = {0, 1, 4, 5};
    vector<int> B1 = {20, 30, 61, 90};
    int Z1 = 3;
    cout << "Result for Test Case 1: " << solution(A1, B1, Z1) << endl; // Expected: 51

    // Test Case 2
    vector<int> A2 = {0, 1, 4, 5};
    vector<int> B2 = {20, 30, 61, 90};
    int Z2 = -1;
    cout << "Result for Test Case 2: " << solution(A2, B2, Z2) << endl; // Expected: 10

    return 0;
}

