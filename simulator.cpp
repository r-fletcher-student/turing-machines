// made by 2886315: Ryan Fletcher

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <list>
#include <deque>

using namespace std;

//000100000100000100000100000000101000100001100000100001100000000101001001100010100001000100010100000000101001001100000100001000100000100000000101001001100001100001100001100000000101
const string ENCODING = "0=000|1=001|_=010|#=100|;=101";
const string L = "10";
const string R = "00";
map<char, string> encoding;
map<string, char> decoding;

int poongCount = 0;
void poong() {
    poongCount++;
    cout << "poong" << poongCount << endl;
}
void poong(string s) {
    poongCount++;
    cout << "poong" << poongCount << ": " << s << endl;
}
void poong(deque<char> d) {
    string s(d.begin(), d.end());
    poong(s);
}

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

    string toString() const {
        return to_string(startState) + "#" + read + "#" + to_string(nextState) + "#" + write + "#" + dir;
    }

    string format() {
        return intToBs(startState) + "#" + read + "#" + intToBs(nextState) + "#" + write + "#" + dir;
    }
};

string poong(map<char, Instruction> m) {
    stringstream ss;
    for (const auto& [key, value] : m) {
        ss << "[" << key << "]" << value.toString() << ", ";
    }
    return ss.str();
}

string poong(map<int, map<char, Instruction>> m) {
    stringstream ss;
    for (const auto& [key, value] : m) {
        ss << "State " << key << ": " << endl << poong(value) << endl;
    }
    return ss.str();
}

/**
 * @brief Retireves encoding of symbols.
 * @param scheme line describing the encodign scheme used.
 */
void getEncoding(const string& scheme) {
    stringstream ss(scheme);
    string pair;

    while (getline(ss, pair, '|')) {
        int pos = pair.find('=');

        char symbol = pair[0];
        string code = pair.substr(pos+1);

        encoding[symbol] =  code;
        decoding[code] = symbol;
    }
}

/**
 * @brief Encodes a string given an encoding scheme.
 * @param line line to be encoded.
 * @param encoding the encoding scheme to be used.
 * @return Encoded line.
 */
string encode(const string& line, map<char,string> e) {
    string output = "";
    for (char c : line) {
        output += e[c];
    }
    return output;
}

/**
 * @brief Decodes a line of encodings given a decoding scheme
 * @param line string of codes (codes of length 3) to be decoded
 * @param d decoding scheme.
 * @return String of the decoded line.
 */
string decode(const string& line, map<string, char> d) {
    string output = "";
    for (int i = 0; i<line.length(); i+=3){
        string code = line.substr(i, 3);
        output += d[code];
    }
    return output;
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
 * @brief Splits a string of all instructions into individual Instruction structures.
 * @param line string of instructions
 * @return list of Instructions
 */
list<Instruction> splitInstructions(const string& line) {
    list<Instruction> insts;

    vector<string> instStrings = split(decode(line,decoding), ';');
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
 * @brief Assigns instructions to their starting states in a map
 * @param insts list of instructions
 * @return Map of (state)-(instruction vector) pairs
 */
map<int, map<char, Instruction>> assignStates(const list<Instruction>& insts) {
    map<int, map<char, Instruction>> states;

    for (auto it = insts.begin(); it != insts.end(); ++it) {
        auto it2 = states.find(it->startState);
        if (it2 == states.end()) {
            states[it->startState] = {{it->read, *it}};
        } else {
            states[it->startState][it->read] = *it;
        }
        cout << it->startState << ": " << poong(states[it->startState]) << endl;
    }
    return states;
}

string simulate(const string& input, map<int, map<char, Instruction>> turingMachine) {
    deque<char> tape(input.begin(), input.end());

    //cout << poong(turingMachine);

    int i = 0;
    int currState = 0;
    int head = 0;
    while (i < 10) { //01_ s0
        i++;
        if (currState == 1 || currState == 2) break;

        Instruction inst = turingMachine[currState][tape[head]];
        cout << "head: " << head << ", tape at head: " << tape[head] << endl;
        cout << "inst: " << poong(turingMachine[currState]) << endl;
        cout << "other inst: " << inst.toString() << endl;

        tape[head] = inst.write;

        // next state
        currState = inst.nextState;


        // move direction
        if (inst.dir == R) {
            head ++;
            if (head >= tape.size()) tape.push_back('_');
        } else {
            if (head == 0) {
                tape.push_front('_');
            } else {
                head--;
            }
        }

        string s(tape.begin(), tape.end());
    }

    string w(tape.begin(), tape.begin() + head);
    string v(tape.begin()+head, tape.end());
    return w + "#" + intToBs(currState) + "#" + v;
}

int main(int argc, char const *argv[])
{
    getEncoding(ENCODING);

    string line;
    getline(cin, line);

    list<Instruction> insts = splitInstructions(line);
    map<int, map<char, Instruction>> turingMachine = assignStates(insts);

    string inputTape;
    getline(cin, inputTape);

    string output = simulate(inputTape, turingMachine);
    cout << output << endl;

    return 0;
}
