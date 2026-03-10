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
const string S = "0";
const string L = "10";
const string LL = "11";
const string R = "00";
const string RR = "01";

/**
 * @brief Converts integer to binary string without leading zeros
 */
string intToBs(int i) {
    if (i == 0) return "0";

    std::string bin;
    while (i > 0) {
        bin = char('0' + (i % 2)) + bin;
        i /= 2;
    }
    return bin;
}
struct Instruction {
    int startState;
    char read;
    int nextState;
    char write;
    string dir;

    string toString() {
        return to_string(startState) + "#" + read + "#" + to_string(nextState) + "#" + write + "#" + dir;
    }

    string format() {
        return intToBs(startState) + "#" + read + "#" + intToBs(nextState) + "#" + write + "#" + dir;
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
        if (i.nextState > max) max = i.nextState;
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
            a[4]
        });
    }

    return insts;
}

/**
 * @brief Converts S, LL and RR instructions to standard L,R by ading intermediate states
 * @param isnts list of instructions
 * @return List of Instructions (including intermediate ones)
 */
void simplifyInstructions(list<Instruction>& insts) {
    for (auto it = insts.begin(); it != insts.end(); ++it) {
        // Stationary
        if (it->dir == S) {
            Qmax++;
            for (char c : {'_', '0', '1'}) {
                insts.push_back({
                    Qmax,
                    c,
                    it->nextState,
                    c,
                    L
                });
            }

            it->dir = R;
            it->nextState = Qmax;

        } else 
        // Double Left
        if (it->dir == LL) {
            Qmax++;
            for (char c : {'_', '0', '1'}) {
                insts.push_back({
                    Qmax,
                    c,
                    it->nextState,
                    c,
                    L
                });
            }

            it->dir = L;
            it->nextState = Qmax;

        } else 
        // Double Right
        if (it->dir == RR) {
            Qmax++;
            for (char c : {'_', '0', '1'}) {
                insts.push_back({
                    Qmax,
                    c,
                    it->nextState,
                    c,
                    R
                });
            }

            it->dir = R;
            it->nextState = Qmax;
        }
    }
}

string outputFormat(list<Instruction> insts) {
    string out = "";
    for (auto it = insts.begin(); it != insts.end(); ++it) {
        out+= it->format() + ";";
    }
    return out;
}

int main(int argc, char const *argv[])
{
    list<Instruction> insts;

    string line;
    getline(cin, line);

    insts = splitInstructions(line);
    Qmax = maxQ(insts);
    sortPriority(insts);

    simplifyInstructions(insts);

    cout << outputFormat(insts) << endl;

    return 0;
}

