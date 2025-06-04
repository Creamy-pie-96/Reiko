#include "library.h"
#include "emotion.h"
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>
#include <string>
#include <climits> // For INT_MAX
using namespace std;

// Function to calculate the edit distance between two strings
int edit_distance(const string& token, const string& valid_word) {
    int row = token.size();
    int col = valid_word.size();

    // Initialize the DP table
    vector<vector<int>> dp(row + 1, vector<int>(col + 1));

    for (int i = 0; i <= row; i++) dp[i][0] = i; // Cost of deletions
    for (int j = 0; j <= col; j++) dp[0][j] = j; // Cost of insertions

    // Fill the DP table
    for (int i = 1; i <= row; i++) {
        for (int j = 1; j <= col; j++) {
            if (token[i - 1] == valid_word[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1]; // Characters match
            } else {
                dp[i][j] = 1 + min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]}); // Min of insert, delete, replace
            }
        }
    }

    return dp[row][col];
}

// Function to check if two strings are similar based on a threshold
string get_best_match(const string& token, const unordered_set<string>& valid_words, double threshold) {
    string best_match = "";
    double best_ratio = 1.0; // Start with worst possible ratio

    for (const auto& word : valid_words) {
        int dis = edit_distance(token, word);
        if (word.empty() || token.empty()) continue;

        double base_ratio = 1.0 - threshold;
        double forgiving = 1.0;
        double max_ratio = base_ratio + (forgiving / token.size());
        double ratio = (double)dis / (double)word.size();

        if (ratio < best_ratio && ratio <= max_ratio) {
            best_ratio = ratio;
            best_match = word;
        }
    }

    return best_match; // Will be "" if nothing passes
}


// Function to fix typos in user input
void fixTypo(vector<string>& user_input) {

    //cout << "test typo was called!" << endl;

    if (user_input.empty()) {
        cout << "Error: User input is empty!" << endl;
        return;
    }

      // Pass hi_words as the second argument to tokenizer
      map<int, string> tokenizedInput = tokenizer(user_input, hi_words);

    for (auto& [index, token] : tokenizedInput) {
        if (token == "__EnD_Of_InPuT_123!@#__") {
            continue;
        }

    
        string corrected = get_best_match(token, valid_words, 0.9);
        if (!corrected.empty()) token = corrected;
    }
    
    /*
    cout << "This is user input after fix typo: " << endl;
    for (const auto& [index, token] : tokenizedInput) {
        cout << token << endl;
    }*/

    for (const auto& [index, token] : tokenizedInput) {
        keyWord_match(token, index);
    }
}
