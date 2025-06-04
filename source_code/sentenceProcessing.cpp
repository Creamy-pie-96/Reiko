#include "library.h"
#include <algorithm>
#include <sstream>
#include <cctype>
#include <map>
#include <unordered_set>
using namespace std;

bool is_hi = false;

// Function to normalize the input (convert to lowercase)
void normalizer(vector<string>& input) {
    vector<string> normalizedSet;

    for (const auto& word : input) {
        string normalizedWord = word;
        if (word == "__EnD_Of_InPuT_123!@#__") {
            // Preserve the end-of-input marker as is
            normalizedSet.push_back(word);
            continue;
        }

        // Convert to lowercase
        transform(normalizedWord.begin(), normalizedWord.end(), normalizedWord.begin(), ::tolower);

        normalizedSet.push_back(normalizedWord);
    }
    input = normalizedSet;

    // Debug: Print the normalized input
    // cout << "After normalization:" << endl;
    // for (const auto& word : input) {
    //     cout << word << endl;
    // }
}

// Tokenizer function
map<int, string> tokenizer(vector<string>& user_input, const unordered_set<string>& hi_words) {


    normalizer(user_input);

    // Initialize token map
    map<int, string> tokenMap;
    int index = 0;
    bool greetingFound = false;

    for (const auto& sentence : user_input) {
        // Check if the sentence is the end-of-input marker
        if (sentence == "__EnD_Of_InPuT_123!@#__") {
            tokenMap[index++] = sentence; // Add the marker as is
            continue;
        }

        istringstream stream(sentence);
        string word;

        // Check if any greeting word exists in the sentence before tokenizing
        if (!greetingFound) {
            for (const auto& greeting : hi_words) {
                if (sentence.find(greeting) != string::npos) {
                    greetingFound = true;
                    is_hi = true;
                    break;
                }
            }
        }

        // If greeting is found, we skip tokenizing the greeting part and process the rest
        if (greetingFound) {
            string remaining_sentence = sentence;

            // Remove the greeting part from the sentence
            for (const auto& greeting : hi_words) {
                size_t pos = remaining_sentence.find(greeting);
                if (pos != string::npos) {
                    remaining_sentence.erase(pos, greeting.length()); // Remove greeting part
                    break;
                }
            }

            // Tokenize the remaining part of the sentence
            istringstream remainingStream(remaining_sentence);
            while (remainingStream >> word) {
                if (ispunct(word.back())) {
                    char punctuation = word.back();
                    word.pop_back();
                    if (!word.empty()) {
                        tokenMap[index++] = word;
                    }
                    if (punctuation == '!' || punctuation == '.' || punctuation == '?') {
                        string punct(1, punctuation);
                        tokenMap[index++] = punct;
                    }
                } else {
                    tokenMap[index++] = word;
                }
            }
            greetingFound = false; // Reset after processing the greeting sentence
        } else {
            // Tokenize the whole sentence normally if no greeting is found
            while (stream >> word) {
                if (ispunct(word.back())) {
                    char punctuation = word.back();
                    word.pop_back();
                    if (!word.empty()) {
                        tokenMap[index++] = word;
                    }
                    if (punctuation == '!' || punctuation == '.' || punctuation == '?') {
                        string punct(1, punctuation);
                        tokenMap[index++] = punct;
                    }
                } else {
                    tokenMap[index++] = word;
                }
            }
        }
    }

    // // Debug: Print the tokenized input
    // cout << "After tokenization:" << endl;
    // for (const auto& [idx, token] : tokenMap) {
    //     cout << "Index " << idx << ": " << token << endl;
    // }

    return tokenMap;
}