// made by 2886315: Ryan Fletcher

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

vector<string> splitInstructions(string);
void simplifyInstruction(string&);

int main(int argc, char const *argv[])
{
    vector<string> instructions;

    string line;
    getline(cin, line);

    instructions = splitInstructions(line);

    return 0;
}

vector<string> splitInstructions(string line) {
    vector<string> instructions;
    stringstream ss(line);
    string instruction;

    while (getline(ss, instruction, ';')) {
        instructions.push_back(instruction);
    }

    return instructions;
}

void simplifyInstruction(string& instruction) {
    
}
