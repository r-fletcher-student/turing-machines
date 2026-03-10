// made by 2886315: Ryan Fletcher

#include <iostream>
#include <string>
#include <sstream>
#include <map>

using namespace std;

/**
 * @brief Retireves encoding of symbols.
 * @param scheme line describing the encodign scheme used.
 * @return Map containing <symbol, encoding>
 */
map<char, string> getEncoding(string scheme) {
    map<char, string> encoding;
    stringstream ss(scheme);
    string pair;

    while (getline(ss, pair, '|')) {
        int pos = pair.find('=');

        char symbol = pair[0];
        string code = pair.substr(pos+1);

        encoding[symbol] =  code;
    }

    return encoding;
}

/**
 * @brief Encodes a string given an encoding scheme.
 * @param line line to be encoded.
 * @param encoding the encoding scheme to be used.
 * @return Encoded line.
 */
string encode(string line, map<char,string> encoding) {
    string output = "";
    for (char c : line) {
        output += encoding[c];
    }
    return output;
}

int main()
{
    map<char, string> encoding;

    string scheme;
    getline(cin, scheme);

    encoding = getEncoding(scheme);

    string line;
    getline(cin, line);

    cout << encode(line, encoding) << endl;

    return 0;
}
