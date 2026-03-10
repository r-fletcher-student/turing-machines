// made by 2886315: Ryan Fletcher

#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <list>
#include <deque>

using namespace std;

const string ENCODING = "0=000|1=001|_=010|#=100|;=101";
const string L = "10";
const string R = "00";
unordered_map<char, string> encoding;
unordered_map<string, char> decoding;

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

/**
 * @brief Represents a singular instruction
 */
struct Instruction {
    int startState;
    char read;
    int nextState;
    char write;
    string dir;

    string toString() const {
        return to_string(startState) + "#" + read + "#" + to_string(nextState) + "#" + write + "#" + dir;
    }
};

/**
 * @brief Retrieves encoding of symbols.
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
 * @brief Decodes a line of encodings given a decoding scheme
 * @param line string of codes (codes of length 3) to be decoded
 * @param d decoding scheme.
 * @return String of the decoded line.
 */
string decode(const string& line, unordered_map<string, char>& d) {
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
unordered_map<int, unordered_map<char, Instruction>> assignStates(const list<Instruction>& insts) {
    unordered_map<int, unordered_map<char, Instruction>> states;

    for (auto it = insts.begin(); it != insts.end(); ++it) {
        auto it2 = states.find(it->startState);
        if (it2 == states.end()) {
            states[it->startState] = {{it->read, *it}};
        } else {
            states[it->startState][it->read] = *it;
        }
    }
    return states;
}

// TESTING FUNCTION - char-instruction map to string
string poongM(unordered_map<char, Instruction> m) {
    stringstream ss;
    for (const auto& [key, value] : m) {
        ss << "[" << key << "]" << value.toString() << ", ";
    }
    return ss.str();
}

// TESTING FUNCTION - state-instructionMap to string (turing machine)
string poongTM(unordered_map<int, unordered_map<char, Instruction>> tm) {
    stringstream ss;
    for (const auto& [key, value] : tm) {
        ss << "State " << key << ": " << endl << poongM(value) << endl;
    }
    return ss.str();
}

/**
 * @brief Simulates a turing machine
 * @param input the input tape to the turning machine (string)
 * @param turingMachine a map describing the turing machine
 * @return Final state of the Turing Machine
 */
string simulate(const string& input, unordered_map<int, unordered_map<char, Instruction>>& turingMachine) {
    deque<char> tape(input.begin(), input.end());

    //cout << poongTM(turingMachine) << endl;   //testing :D

    int i = 0;
    int currState = 0;
    int head = 0;
    while (i < 1000000) {
        i++;
        if (currState == 1 || currState == 2) break;

        Instruction inst = turingMachine[currState][tape[head]];
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

        while (tape.size() > 1 && tape.back() == '_' && head < (int)tape.size() - 1)
            tape.pop_back();

        while (tape.size() > 1 && tape.front() == '_' && head > 0) {
            tape.pop_front();
            head--;
        }
    }

    string w(tape.begin(), tape.begin() + head);
    while (!w.empty() && w.front() =='_') w.erase(w.begin());

    string v(tape.begin()+head, tape.end());
    while (!v.empty() && v.back() == '_') v.pop_back();
    if (v.empty()) v = "_";


    return w + "#" + intToBs(currState) + "#" + v;
}

int main(int argc, char const *argv[])
{
    getEncoding(ENCODING);

    string line;
    getline(cin, line);

    list<Instruction> insts = splitInstructions(line);
    unordered_map<int, unordered_map<char, Instruction>> turingMachine = assignStates(insts);

    string inputTape;
    getline(cin, inputTape);

    string output = simulate(inputTape, turingMachine);
    cout << output << endl;

    return 0;
}
