#include "library.h"
#include "learn.h"
#include <fstream>
#include<limits>
using namespace std;

void saveTrackedData(const string& emotionFile, const string& intensifierFile) {
    
    ofstream emotionOut(emotionFile);
    if (!emotionOut) {
        cerr << "Error saving tracked emotion words to file: " << emotionFile << endl;
        return;
    }

    for (const auto& [word, data] : trackedEmotionWords) {
        emotionOut << word << " " << data.frequency;
        for (const auto& [type, value] : data.context) {
            emotionOut << " " << type << " " << value; 
        }
        emotionOut << endl;
    }
    emotionOut.close();

    ofstream intensifierOut(intensifierFile);
    if (!intensifierOut) {
        cerr << "Error saving tracked intensifier words to file: " << intensifierFile << endl;
        return;
    }

    for (const auto& [word, data] : trackedIntensifierWords) {
        intensifierOut << word << " " << data.frequency;
        for (double value : data.context) {
            intensifierOut << " " << value; 
        }
        intensifierOut << endl;
    }
    intensifierOut.close();
}

void loadTrackedData(const string& emotionFile, const string& intensifierFile) {
    
    ifstream emotionIn(emotionFile);
    if (!emotionIn) {
        cerr << "Error loading tracked emotion words from file: " << emotionFile << endl;
        return;
    }

    string word;
    while (emotionIn >> word) {
        int frequency;
        emotionIn >> frequency;
        trackedEmotionWords[word].frequency = frequency;

        int type;
        double value;
        while (emotionIn.peek() != '\n' && emotionIn >> type >> value) {
            trackedEmotionWords[word].context.push_back({type, value}); // Load emotion type and intensity
        }
    }
    emotionIn.close();

    ifstream intensifierIn(intensifierFile);
    if (!intensifierIn) {
        cerr << "Error loading tracked intensifier words from file: " << intensifierFile << endl;
        return;
    }

    while (intensifierIn >> word) {
        int frequency;
        intensifierIn >> frequency;
        trackedIntensifierWords[word].frequency = frequency;

        double value;
        while (intensifierIn.peek() != '\n' && intensifierIn >> value) {
            trackedIntensifierWords[word].context.push_back(value); 
        }
    }
    intensifierIn.close();
}

void analyzeEmotionWordWithoutContext(const string& word, TrackedEmotionWord& data, unordered_map<int, string>& emotionTypeNames) {
    cout << "The word '" << word << "' has no context. Checking emotion history..." << endl;

    int guessedType = 7; 
    if (!moodHistory.empty()) {
        guessedType = static_cast<int>(moodHistory.back()); 
    }

    if (emotionTypeNames.find(guessedType) == emotionTypeNames.end()) {
        cout << "Error: Invalid guessed emotion type. Defaulting to 'Neutral'." << endl;
        guessedType = 7;
    }

    cout << "Previously, you were feeling " << emotionTypeNames[guessedType] << "." << endl;
    cout << "Does the word '" << word << "' also indicate a " << emotionTypeNames[guessedType] << " emotion? (Yes/No)" << endl;

    string userResponse;
    cin >> userResponse;

    if (userResponse == "Yes" || userResponse == "yes") {
        emotion_words[word].push_back({guessedType, 1.0}); 
        saveEmotionWords("../mechine knowledge/emotion_words.txt");
    } else {
        cout << "Please select the correct emotion type for the word '" << word << "':" << endl;
        for (const auto& [type, name] : emotionTypeNames) {
            cout << type << ". " << name << endl;
        }
        cout << "0. Skip this word" << endl;

        int userChoice;
        cin >> userChoice;

        if (userChoice == 0) {
            cout << "Skipping the word '" << word << "'." << endl;
            adjective_words.erase(word);
            saveAdjectiveWords("../mechine knowledge/adjective.txt");
        } else if (emotionTypeNames.find(userChoice) != emotionTypeNames.end()) {
            
            cout << "Saving the word '" << word << "' as a " << emotionTypeNames[userChoice] << " emotion." << endl;
            emotion_words[word].push_back({userChoice, 1.0}); 
            saveEmotionWords("../mechine knowledge/emotion_words.txt");
        } else {
            cout << "Invalid choice. Skipping the word '" << word << "'." << endl;
        }
    }
}
void analyzeEmotionWordWithContext(const string& word, TrackedEmotionWord& data, unordered_map<int, string>& emotionTypeNames) {
    double totalValue = 0.0;
    int dominantType = 0;
    unordered_map<int, int> typeCount;

    for (const auto& [type, value] : data.context) {
        totalValue += value;
        typeCount[type]++;
    }

    for (const auto& [type, count] : typeCount) {
        if (count > typeCount[dominantType]) {
            dominantType = type;
        }
    }

    double averageValue = totalValue / data.context.size();
    cout << "The word '" << word << "' seems to be an emotion word (Type: " << emotionTypeNames[dominantType]
         << ", Value: " << averageValue << "). Do you want me to remember this? (Yes/No)" << endl;

    string userResponse;
    cin >> userResponse;

    if (userResponse == "Yes" || userResponse == "yes") {
        
        emotion_words[word].push_back({dominantType, averageValue});
        saveEmotionWords("../mechine knowledge/emotion_words.txt");
    }

    adjective_words.erase(word);
    saveAdjectiveWords("../mechine knowledge/adjective.txt");
}

void analyzeIntensifierWordWithoutContext(const string& word) {
    cout << "The word '" << word << "' has no context. It might be an intensifier." << endl;
    cout << "Please provide a value for this intensifier (e.g., 2.0 for strong, 0.5 for weak):" << endl;

    double userValue;
    while (true) {
        cin >> userValue;

        if (cin.fail() || userValue <= 0) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "Invalid input. Please enter a positive numeric value:" << endl;
        } else {
            break; 
        }
    }

    intensifier_words[word] = userValue;
    saveIntensifierWords("../mechine knowledge/intensifier_words.txt");

    cout << "The word '" << word << "' has been saved as an intensifier with a value of " << userValue << "." << endl;
}

void analyzeIntensifierWordWithContext(const string& word, TrackedIntensifierWord& data) {
    double totalValue = 0.0;
    for (double value : data.context) {
        totalValue += value;
    }
    double averageValue = totalValue / data.context.size();
    cout << "The word '" << word << "' seems to be an intensifier (Value: " << averageValue << "). Do you want me to remember this? (Yes/No)" << endl;

    string userResponse;
    cin >> userResponse;

    if (userResponse == "Yes" || userResponse == "yes") {
        
        intensifier_words[word] = averageValue;
        saveIntensifierWords("../mechine knowledge/intensifier_words.txt");
    }

    adverb_words.erase(word);
    saveAdverbWords("../mechine knowledge/adverb.txt");
}

void analyzeTrackedData() {
    
    unordered_map<int, string> emotionTypeNames = {
        {1, "Happy"},
        {2, "Sad"},
        {3, "Angry"},
        {4, "Fear"},
        {5, "Surprise"},
        {6, "Disgust"},
        {7, "Neutral"}
    };

    for (auto it = trackedEmotionWords.begin(); it != trackedEmotionWords.end();) {
        const string& word = it->first;
        TrackedEmotionWord& data = it->second;

        if (data.context.empty()) {
            analyzeEmotionWordWithoutContext(word, data, emotionTypeNames);
            it = trackedEmotionWords.erase(it);
        } else if (data.frequency >= 5) {
            analyzeEmotionWordWithContext(word, data, emotionTypeNames);
            it = trackedEmotionWords.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = trackedIntensifierWords.begin(); it != trackedIntensifierWords.end();) {
        const string& word = it->first;
        TrackedIntensifierWord& data = it->second;

        if (data.context.empty()) {
            analyzeIntensifierWordWithoutContext(word);
            it = trackedIntensifierWords.erase(it);
        } else if (data.frequency >= 5) {
            analyzeIntensifierWordWithContext(word, data);
            it = trackedIntensifierWords.erase(it);
        } else {
            ++it;
        }
    }
}
