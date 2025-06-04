#include "library.h"
#include "learn.h"
#include <iostream>
#include <unordered_set>
#include <vector>
#include <string>
using namespace std;

int main() {
    cout << "Welcome to REIKO, your personal emotion tracker!" << endl;

    // Call preload to initialize global variables
    preload();

    vector<string> user_input;
    string input_line;

    cout << "Please enter your sentences (type 'exit' to quit):" << endl;

    // Take user input until "exit" is entered
    while (true) {
        getline(cin, input_line);

        // Check if the user wants to exit
        if (input_line == "exit") {
            break;
        }

        // Add the input line to the user_input vector
        user_input.push_back(input_line);
    }

    // Add end-of-input marker to indicate the end of user input
    user_input.push_back("__EnD_Of_InPuT_123!@#__");

    // Process the input to fix typos and generate responses
    fixTypo(user_input);

    // Trigger learning before exiting
    analyzeTrackedData();
    saveTrackedData("../mechine knowledge/Potential emotion.txt", "../mechine knowledge/Potential intensifier.txt");

    cout << "Thank you for using REIKO. Have a great day!" << endl;

    return 0;
}