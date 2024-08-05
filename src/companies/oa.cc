#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace std;

// Function to calculate minimum moves
int solution(vector<vector<int>>& A) {
    // Vectors to store excess and deficit cell positions
    vector<pair<int, int>> excess;
    vector<pair<int, int>> deficit;

    // Calculate excess and deficit cells
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (A[i][j] > 1) {
                excess.push_back({i, j});
            } else if (A[i][j] == 0) {
                deficit.push_back({i, j});
            }
        }
    }

    int moves = 0;
    // Traverse each deficit cell
    for (auto& d : deficit) {
        pair<int, int> target = d;
        // Find the closest excess cell
        sort(excess.begin(), excess.end(), [&target](pair<int, int>& a, pair<int, int>& b) {
            return abs(a.first - target.first) + abs(a.second - target.second) <
                   abs(b.first - target.first) + abs(b.second - target.second);
        });

        // Use the closest excess cell
        auto closest = excess.front();
        excess.erase(excess.begin()); // Remove the closest excess cell

        // Calculate moves required to move one stone from the closest excess cell to the target deficit cell
        moves += abs(closest.first - target.first) + abs(closest.second - target.second);

        // Update the excess cell's stone count
        if (--A[closest.first][closest.second] > 1) {
            excess.push_back(closest); // If still excess, add back to excess list
        }
    }

    return moves;
}

int main() {
    vector<vector<int>> A1 = {{1, 0, 1}, {1, 3, 0}, {2, 0, 1}};
    cout << solution(A1) << endl; // Should return 3

    vector<vector<int>> A2 = {{2, 0, 2}, {1, 0, 0}, {2, 1, 1}};
    cout << solution(A2) << endl; // Should return 4

    vector<vector<int>> A3 = {{0, 6, 0}, {2, 0, 0}, {0, 1, 0}};
    cout << solution(A3) << endl; // Should return 9

    return 0;
}
