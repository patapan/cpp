#include <fstream>
#include <iostream>

void read_from_file() {
    std::ifstream file("test.txt");
    if (!file) return;

    std::string input;

    while (getline(file,input)) {
        std::cout << input << std::endl;
    }

    file.close();
}

void write_to_file() {
    std::ofstream file("test.txt");

    if (!file) { 
        // error
        return;
    }

    file << "I'm piping input";

    std::string test = "im writing to a file!";

    file.write(test.c_str(), test.size());

    file.close();
}

int main() {
    read_from_file();
}