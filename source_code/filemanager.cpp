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
        if (line.empty() || line.find_first_not_of(" \t") == string::npos) {
            continue; 
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

    while (file >> key >> emotionType >> emotionValue) {
        if (key.empty() || file.fail()) {
            continue; 
        }

        if (input.find(key) != input.end()) {
            input[key].push_back(make_pair(emotionType, emotionValue));
        }
        else {
            input[key] = { make_pair(emotionType, emotionValue) };
        }
    }

    file.close();
}
