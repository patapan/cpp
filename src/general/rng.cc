#include <random>

/*
 given a uniform random number generator from 0 to 1, 
 write a random number generator from 0 to 1 that has a probability density function of P(x) = x. 
 Namely, 3/4 is 3 times as likely to be generated as 1/4.
*/

#include <iostream>

int rand(int N) {
    // Create a (pseudo) random uniform generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0,1);

    std::vector<int> choices;

    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= i; j++) {
            choices.push_back(i);
        }
    }

    return choices[(int)(dis(gen) * choices.size())];
}

int crand() {
    // srand(time(nullptr))

    return time(nullptr);
}

int main() {
    // std::cout << rand(10) << std::endl;
    std::cout << crand() << std::endl;
}