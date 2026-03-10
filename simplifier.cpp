// made by 2886315: Ryan Fletcher

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <list>

using namespace std;

int Qmax = 0;

struct Instruction {
    int startState;
    char read;
    int nextState;
    char write;
    int dir;
    string toString() {
        return to_string(startState) + "#" + read + "#" + to_string(nextState) + "#" + write + "#" + to_string(dir);
    }
};

int priority(char c) {
    if (c == '_') return 0;
    if (c == '0') return 1;
    if (c == '1') return 2;
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
void sortPriority(std::list<Instruction>& insts) {
    insts.sort([](const Instruction& a, const Instruction& b) {

        if (a.startState != b.startState)
            return a.startState < b.startState;

        return priority(a.read) < priority(b.read);
    });
}

/**
 * @brief Returns the maximum state number in a vector of instructions.
 */
int maxQ(list<Instruction> insts) {
    int max = 0;
    for (const Instruction& i : insts) {
        if (i.startState > max) max = i.startState;
    }
    return max;
}

/**
 * @brief Splits a string of all instructions into individual Instruction structures.
 * @param line string of instructions
 * @return list of Instructions
 */
list<Instruction> splitInstructions(const string& line) {
    list<Instruction> insts;

    vector<string> instStrings = split(line, ';');
    for (const string& iS : instStrings) {
        auto a = split(iS, '#');
        insts.push_back({
            bsToInt(a[0]),
            a[1][0],
            bsToInt(a[2]),
            a[3][0],
            bsToInt(a[4])
        });
    }

    return insts;
}

void simplifyInstructions(list<Instruction>& insts) {
    for (int i = 0; i<insts.size(); i++) {
        if ()
    }
}

int main(int argc, char const *argv[])
{
    list<Instruction> insts;

    string line;
    getline(cin, line);

    insts = splitInstructions(line);
    sortPriority(insts);

    for (auto i : insts) {
        cout << i.toString() << endl;
    }

    return 0;
}

