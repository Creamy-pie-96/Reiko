#include "library.h"
#include "learn.h"
#include <iostream>
#include <unordered_set>
#include <vector>
#include <string>
using namespace std;

int main() {
    cout << "Welcome to REIKO, your personal emotion tracker!" << endl;

    preload();

    vector<string> user_input;
    string input_line;

    cout << "Please enter your sentences (type 'exit' to quit):" << endl;

    while (true) {
        getline(cin, input_line);

        if (input_line == "exit") {
            break;
        }

        user_input.push_back(input_line);
    }

    user_input.push_back("__EnD_Of_InPuT_123!@#__");

    fixTypo(user_input);

    analyzeTrackedData();
    saveTrackedData("../mechine knowledge/Potential emotion.txt", "../mechine knowledge/Potential intensifier.txt");

    cout << "Thank you for using REIKO. Have a great day!" << endl;

    return 0;
}
