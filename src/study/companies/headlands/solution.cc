#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

/*
In the prompt, it states that an ambiguous input is when a filename has a conflicting opaque identifier,
yet in the first input example we see that bubblesort.cpp and bogosort.cpp have conflicting identifiers, but this is not acknowledged.

*/

class Repo {
public:
    std::map<int,int> commits; // map <commit timestamp, commit id>
    std::unordered_set<std::string> files; // <filename>
    
    void mergeRepo(Repo& other) {
        for (auto it : other.commits) {
            commits.insert(it);
        }
        for (auto it : other.files) {
            files.insert(it);
        }
    
    }
};

// query <start, end> timestamps + filepath + file_id

// commits can be combined with others that contain atleast one matching file path and opaque identifier 

using namespace std;
int main() {
    std::unordered_map<std::string, Repo> file_map; // map file_id to Repo
    std::unordered_map<std::string, std::string> file_ids; // map file_id to file_name
    
    int num_logs;
    std::cin >> num_logs;
    std::cin.ignore();

    for (int i = 0; i < num_logs; i++) {
        // std::cout << "here1\n" << num_logs << std::endl;
        int id, timestamp;
        std::string line, word;

        getline(cin, line);
        std::istringstream iss(line);
        
        iss >> word >> word;
        
        try {
            id = std::stoi(word);
            iss >> word >> word;
            timestamp = std::stoi(word);
        } catch (const std::invalid_argument& e) {
            // Malformed entry, skip
            continue;
        }
        
        // iterate through repos, see if we can add this commit to that
        std::string filename, file_id;
        Repo repo;
        repo.commits[timestamp] = id;

        while (iss >> filename >> file_id) {
            // std::cout << "Filename: " << filename << " file id: " << file_id << std::endl;
            // Record file_name -> file_id
            if (file_ids.find(filename) != file_ids.end() && file_ids[filename] != file_id) {
                std::cout << "AMBIGUOUS INPUT!\n";
                return 0;
            } else {
                file_ids[filename] = file_id;
            }
            
            if (file_map.find(file_id) != file_map.end()) {
                file_map[file_id].mergeRepo(repo);
            }  else {
                repo.files.insert(filename);
                file_map[file_id] = repo;
            }
        }
    }

    int num_queries;
    cin >> num_queries;
    std::cout << num_queries << std::endl;
    for (int i = 0; i < num_queries; i++) {
        std::string word;
        int start, end;
        std::string filename, file_id;
        std::cin >> start >> end >> filename >> file_id;
        std::cout << start << end << filename << file_id << std::endl;
        if (file_ids.find(filename) == file_ids.end() || file_ids[filename] != file_id) {
            std::cout << "\n";
            continue;
        } 
        
        Repo repo = file_map[file_id];
        auto it = repo.commits.lower_bound(start);
        cout <<"\n" << it->first;
        while (it != repo.commits.end() && it->first <= end) {
            cout << it->second << " ";
            it++;
        }
        
    }
    
    // Each response consists of the commit ids for the repo associated with the query parameters, sorted by increasing timestamp...
    // timestamp + file
    
    // for output:
    // 1.fetch repo
    // 2. binary search over commits to get first commit in timestamp
    // 3. write
    return 0;
}