class Solution {
public:
    // O(N) time and space
    // Variation with all operations, and no brackets
    int calculate(string s) {
        stack<int> stack;
        int num = 0;
        char operation = '+';

        for (int i = 0; i <= s.size(); i++) {
            if (i < s.size() && iswspace(s[i])) continue;
            else if (i < s.size() && isdigit(s[i])) {
                num = num * 10 + (s[i] - '0');
            } else {
                // compute operation
                if (operation == '+') {
                    stack.push(num);
                } else if (operation == '-') {
                    stack.push(-num);
                } else if (operation == '*') {
                    int left = stack.top();
                    stack.pop();
                    stack.push(left * num);
                } else {
                    int left = stack.top();
                    stack.pop();
                    stack.push(left / num);
                }
                if (i < s.size()) operation = s[i];
                num = 0;
            }
        }
        // sum up the digits
        int sum = 0;
        while (stack.size() > 0) {
            sum += stack.top();
            stack.pop();
        }
        return sum;
    }
/////////////////////////////////////
// Complete with brackets + all operations
// Time: O(N) - each recursive call we are still moving index forward
// Space: O(N) - worst case we have N recursive calls which adds to the stack
class Solution {
private:
    int index = 0;
public:
    int calculate(string s) {
        int result = 0;
        int prevNum = 0;
        int num = 0;
        char operation = '+';
        while (index < s.size()) {
            if (isdigit(s[index])) {
                num = num * 10 + (s[index] - '0');
            } else if (s[index] == '(') {
                index++;
                num = calculate(s);
            } else if (s[index] == ')') {
                break;
            } else {
                computeOperation(s, result, prevNum, num, operation);
            }
            index++;
        }

        computeOperation(s, result, prevNum, num, operation);
        result += prevNum;
        return result;
    }

    void computeOperation(string& s, int& result, int& prevNum, int& num, char& operation) {
        if (operation == '+') {
            result += prevNum;
            prevNum = num;
        } else if (operation == '-') {
            result += prevNum;
            prevNum = -num;
        } else if (operation == '*') {
            prevNum *= num;
        } else if (operation == '/') {
            prevNum /= num;
        }

        if (index < s.size()) {
            num = 0;
            operation = s[index];
        } 
    }
};


};