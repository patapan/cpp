

void run() {
    int* var = new int[99999990000000000];
    delete[] var;
}

int main() {
    run();
}