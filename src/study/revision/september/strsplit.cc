
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

vector<string> split(std::string& s) {
    vector<string> ret;
    std::istringstream stream(s);
    std::string line;
    while (getline(stream, line, ' ')) {
        ret.push_back(line);
    }

    return ret;
}

int main() {
    std::string s = "hello world";

    std::vector<std::string> ret = split(s);

    for (auto s : ret) {
        cout << "split: " << s << endl;
    }
}