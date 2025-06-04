#include<iostream>
#include<fstream>
#include<unordered_set>
#include<unordered_map>
#include<vector>
#include<string>
using namespace std;

// Enum for Emotion Types
enum EmotionType {
    HAPPY = 1,
    SAD = 2,
    ANGRY = 3,
    FEAR = 4,
    SURPRISE = 5,
    DISGUST = 6,
    NEUTRAL = 7
};

// FileManager for unordered_set
void fileManager(string filename, unordered_set<string> &input)
{
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file!" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        // Trim leading and trailing spaces and check if the line is empty
        if (line.empty() || line.find_first_not_of(" \t") == string::npos) {
            continue; // Skip empty or whitespace-only lines
        }
        input.insert(line);
    }

    file.close();
}

// FileManager for unordered_map with double values
void FileManager(string filename, unordered_map<string, double>& input)
{
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file!" << endl;
        return;
    }

    string key;
    double value;

    while (file >> key >> value) {
        // Check if the line is empty or contains only whitespace
        if (key.empty() || file.fail()) {
            continue; // Skip lines with invalid data
        }
        input[key] = value;
    }

    file.close();
}

// FileManager for unordered_map with vector of pairs (emotion type and value)
void FileManager2(string filename, unordered_map<string, vector<pair<int, double>>>& input)
{
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file!" << endl;
        return;
    }

    string key;
    int emotionType;
    double emotionValue;

    // Read each line and store the data in the unordered_map
    while (file >> key >> emotionType >> emotionValue) {
        // Check if the line is empty or contains only whitespace
        if (key.empty() || file.fail()) {
            continue; // Skip lines with invalid data
        }

        // Check if the word already exists in the map
        if (input.find(key) != input.end()) {
            // If the word already exists, just push the new pair (emotionType, emotionValue)
            input[key].push_back(make_pair(emotionType, emotionValue));
        }
        else {
            // If the word doesn't exist, create a new vector and push the pair
            input[key] = { make_pair(emotionType, emotionValue) };
        }
    }

    file.close();
}
