/*
Given a mouse with 2 APIs in a maze. Design an algorithm to find a cheese in the maze using only the 2 given APIs shown below.
*/

// Time complexity: O(N) - traverse over entire graph in worst case
// Space: O(N) - we store a set of coordinates
class Mouse {
private:
    string back[4] = {"down", "left", "up", "right"};
    int movement[4][2] = {{-1,0}, {0,1}, {1,0}, {0,-1}};
    string directions[4] = {"up", "right", "down", "left"};
    unordered_set<pair<int,int>> seen;

public:
	/**
	* Moves to one of the directions (left, right, up, down) and returns false if you can't move and true if you can.
	*/
	bool move(string& direction);

	/**
	* Returns true if there is a cheese in the current cell.
	*/
	bool hasCheese();

	/**
	* Should return true and leave the mouse at that location or false if we can't find cheese and return the mouse back to where it started.
	*/
	bool getCheese() {
        return traverse(0,0);
	}

    // return true when found cheese
    bool traverse(int row, int col) {
        // try and move in every direction

        seen.insert(make_pair(row, col));

        if (hasCheese()) return true;

        for (int i = 0; i < 4; i++) {
            string new_dir = directions[i];
            string go_back = back[i];
            int new_row = row + movement[i][0];
            int new_col = col + movement[i][1];
            if (!seen.contains(make_pair(new_row, new_col)) && move(new_dir)) {
                if (traverse(new_row, new_col)) return true;
                move(go_back);
            }
        }

        return false;
    }
}