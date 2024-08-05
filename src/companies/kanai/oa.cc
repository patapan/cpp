#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <iostream>
#include <functional>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);

/*
 * Complete the 'findTotalWeight' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts INTEGER_ARRAY products as parameter.
 */

int findTotalWeightGPT(vector<int> products) {
    int totalWeight = 0;
    while (!products.empty()) {
        // Find the minimum weight product
        auto minElement = min_element(products.begin(), products.end());
        int minIndex = distance(products.begin(), minElement);
        totalWeight += *minElement;

        // Remove the minimum weight product and its adjacent products
        if (minIndex > 0 && minIndex < products.size() - 1) {
            // If there are products to the left and right
            products.erase(products.begin() + minIndex - 1, products.begin() + minIndex + 2);
        } else if (minIndex == 0) {
            // If the minimum element is at the start
            products.erase(products.begin(), products.begin() + 2);
        } else {
            // If the minimum element is at the end
            products.erase(products.end() - 2, products.end());
        }
    }
    return totalWeight;
}

int findTotalWeight(vector<int> products) {
    int count = 0;
    vector<bool> seen(products.size(), false);

    int seen_count = 0;
    while (seen_count < products.size()) {
        int min_index = -1;
        for (int i = 0; i < products.size(); i++) {
            if (!seen[i] && (min_index == -1 || (products[i] < products[min_index]))) {
                min_index = i;
            }
        }

        // we have the  min
        count += products[min_index];
        seen_count += 1;
        seen[min_index] = true;
        if ((min_index > 0) && !seen[min_index - 1]) {
            seen[min_index - 1] = true;
            seen_count += 1;
        }
        if ((min_index < products.size() - 1) && !seen[min_index + 1]) {
            seen[min_index + 1 ] = true;
            seen_count += 1;
        }
    }

    return count;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string products_count_temp;
    getline(cin, products_count_temp);

    int products_count = stoi(ltrim(rtrim(products_count_temp)));

    vector<int> products(products_count);

    for (int i = 0; i < products_count; i++) {
        string products_item_temp;
        getline(cin, products_item_temp);

        int products_item = stoi(ltrim(rtrim(products_item_temp)));

        products[i] = products_item;
    }

    int result = findTotalWeight(products);

    std::cout << result << "\n";
    fout << result << "\n";

    fout.close();

    return 0;
}

string ltrim(const string &str) {
    string s(str);

    s.erase(
        s.begin(),
        find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)))
    );

    return s;
}

string rtrim(const string &str) {
    string s(str);

    s.erase(
        find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
        s.end()
    );

    return s;
}
