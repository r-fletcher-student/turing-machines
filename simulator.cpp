// made by 2886315: Ryan Fletcher

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>

using namespace std;

const string ENCODING = "0=000|1=001|_=010|#=100|;=101";
map<char, string> encoding;
map<string, char> decoding;

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

int main(int argc, char const *argv[])
{
    getEncoding(ENCODING);

    string line;
    getline(cin, line);

    cout << decode(line, decoding) << endl;

    return 0;
}
