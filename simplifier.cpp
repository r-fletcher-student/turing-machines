// made by 2886315: Ryan Fletcher

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int Qmax = 0;

int priority(const string& c) {
    if (c == "_") return 0;
    if (c == "0") return 1;
    if (c == "1") return 2;
    return 3;
}

/**
 * @brief Splits a string by sepcified delimeter.
 * @param line string to split
 * @param delim specified delimeter
 * @return Vector of strings split from input string by delimeter
 */
vector<string> split(const string& line, char delim) {
    vector<string> splitStrings;
    stringstream ss(line);
    string part;

    while (getline(ss, part, delim)) {
        splitStrings.push_back(part);
    }

    return splitStrings;
}

/**
 * @brief Converts binary string to integer
 * @param b binary string
 * @return integer representation
 */
int bsToInt(const string& b) {
    return stoi(b, nullptr, 2);
}

/**
 * @brief Sorts vector of instructions into priority order.
 * @param insts Vector of instructions
 */
void sortPriority(vector<string>& insts) {
    sort( insts.begin(), insts.end(), 
    [](const string& a, const string& b) {

        auto A = split(a, '#');
        auto B = split(b, '#');

        int stateA = bsToInt(A[0]);
        int stateB = bsToInt(B[0]);

        if (stateA != stateB) return stateA < stateB;

        return priority(A[1]) < priority(B[1]);

    });
}

int maxQ(vector<string> insts) {
    int max = 0;
    for (const string& i : insts) {
        auto inst = split(i, '#');
        int state = bsToInt(inst[0]);
        if (state > max) max = state;
    }
    return max;
}

int main(int argc, char const *argv[])
{
    vector<string> insts;

    string line;
    getline(cin, line);

    insts = split(line, ';');
    Qmax = maxQ(insts);
    cout << Qmax << endl;
    sortPriority(insts);

    return 0;
}

