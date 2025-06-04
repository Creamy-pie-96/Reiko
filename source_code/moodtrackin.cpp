#include "library.h"
#include <fstream>
#include "emotion.h"

using namespace std;


// for saving the mood history
void saveMoodHistory(const string& filename){
    ofstream file(filename);
    if(!file){
        cerr<<"Error: Could not open file for saving mood history!"<<endl;
    }
    for (const auto& mood : moodHistory) {
        file << mood << endl;
    }

    file.close();
    cout << "Mood history saved successfully to " << filename << endl;
}

// for loading the mood history
void loadMoodHistory(const string& filename) {
    ifstream file(filename, ios::in);
    if (!file) {
        cerr << "Error: Could not open file for loading mood history!" << endl;
        return;
    }

    moodHistory.clear();
    double mood;
    while (file >> mood) {
        moodHistory.push_back(mood);
    }

    file.close();
    cout << "Mood history loaded successfully from " << filename << endl;
}

//for analyzing the mood change
void analyzeMoodChanges() {
    if (moodHistory.size() < 2) {
        cout << "Not enough data to analyze mood changes." << endl;
        return;
    }

    // Define thresholds for mood change rate
    const double RAPID_CHANGE_THRESHOLD = 3.0;  // Example: Change > 3 is considered rapid
    const double MODERATE_CHANGE_THRESHOLD = 1.50;  // Example: Change > 1.5 is moderate

    cout << "Mood Change Analysis:" << endl;
    for (size_t i = 1; i < moodHistory.size(); ++i) {
        double change = moodHistory[i] - moodHistory[i - 1]; // Preserve the sign of the change

        // Categorize the change
        string changeCategory;
        if (abs(change) > RAPID_CHANGE_THRESHOLD) {
            changeCategory = "Rapid";
        } else if (abs(change) > MODERATE_CHANGE_THRESHOLD) {
            changeCategory = "Moderate";
        } else {
            changeCategory = "Slow";
        }

        // Determine if the change is improving or declining
        string direction = (change > 0) ? "Improving" : "Declining";

        cout << "Change from entry " << i << " to " << i + 1 << ": " << change
             << " (" << changeCategory << " change, " << direction << ")" << endl;
    }
}

//displaying the summary
void displayMoodSummary() {
    if (moodHistory.empty()) {
        cout << "No mood history available." << endl;
        return;
    }

    double totalMood = 0.0;
    for (const auto& mood : moodHistory) {
        totalMood += mood;
    }

    double averageMood = totalMood / moodHistory.size();
    cout << "Mood Summary:" << endl;
    cout << "Total Entries: " << moodHistory.size() << endl;
    cout << "Average Mood: " << averageMood << endl;

    if (averageMood > 2.0) {
        cout << "Overall Mood: Positive" << endl;
    } else if (averageMood < -2.0) {
        cout << "Overall Mood: Negative" << endl;
    } else {
        cout << "Overall Mood: Neutral" << endl;
    }
}