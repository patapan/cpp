
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

vector<string> split(const string& str, char delimiter = ',') {
    // convert string into stream
    std::istringstream stream(str);

    vector<string> ret;
    string tmp;
    while (getline(stream, tmp, delimiter)) {
        ret.push_back(tmp);
    }

    return ret;
}

int main() {
    auto output = split("alksj.sadfjk");

    for (auto& str : output) cout << str << endl;
}