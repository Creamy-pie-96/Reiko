#include "emotion.h"
#include "library.h"
#include <algorithm>
#include <sstream>
#include <unordered_set>
#include <random>
#include <ctime>
using namespace std;

// Function to get a random response from an unordered_set<string>
string getRandomResponse(const unordered_set<string>& responses) {
    if (responses.empty()) return "I'm here to listen.";

    // Initialize the random seed
    static bool seedInitialized = false;
    if (!seedInitialized) {
        srand(static_cast<unsigned int>(time(0))); // Seed the random number generator
        seedInitialized = true;
    }

    // Generate a random response
    auto it = responses.begin();
    advance(it, rand() % responses.size());
    return *it;
}

// Helper function: Sort emotions by intensity
vector<pair<int, double>> sortEmotionsByIntensity(const vector<pair<int, double>>& emotionDetails) {
    vector<pair<int, double>> sortedEmotions = emotionDetails;
    sort(sortedEmotions.begin(), sortedEmotions.end(), [](const pair<int, double>& a, const pair<int, double>& b) {
        return abs(a.second) > abs(b.second); // Higher absolute intensity first
    });
    return sortedEmotions;
}

// Helper function: Generate a response for a single emotion
string generateEmotionResponse(int emotionType, double score) {
    string response;
    string colorCode;

    // Determine intensity-based response and assign color
    if (emotionType == 1 || emotionType == -2 || emotionType == -3 || emotionType == -4 || emotionType == -6) { // Happy
        colorCode = "\033[32m"; // Green
        if (score <= 3.50) {
            response = getRandomResponse(happy_replies_moderate); // Moderate happy replies
        } else {
            response = getRandomResponse(happy_replies_strong); // Strong happy replies
        }
    } else if (emotionType == 2 || emotionType == -1) { // Sad
        colorCode = "\033[36m"; // Cyan
        if (score >= -3.50) {
            response = getRandomResponse(sad_replies_moderate); // Moderate sad replies
        } else {
            response = getRandomResponse(sad_replies_strong); // Strong sad replies
        }
    } else if (emotionType == 3) { // Angry
        colorCode = "\033[31m"; // Red
        if (score >= -3.50) {
            response = getRandomResponse(angry_replies_moderate); // Moderate angry replies
        } else {
            response = getRandomResponse(angry_replies_strong); // Strong angry replies
        }
    } else if (emotionType == 4) { // Fear
        colorCode = "\033[34m"; // Blue
        if (score >= -3.50) {
            response = getRandomResponse(fear_replies_moderate); // Moderate fear replies
        } else {
            response = getRandomResponse(fear_replies_strong); // Strong fear replies
        }
    } else if (emotionType == 5) { // Surprise
        colorCode = "\033[33m"; // Yellow
        response = getRandomResponse(surprise_replies);
    } else if (emotionType == 6) { // Disgust
        colorCode = "\033[35m"; // Magenta
        if (score >= -3.50) {
            response = getRandomResponse(disgust_replies_moderate); // Moderate disgust replies
        } else {
            response = getRandomResponse(disgust_replies_strong); // Strong disgust replies
        }
    } else if (emotionType == 7) { // Neutral
        colorCode = "\033[37m"; // White
        response = getRandomResponse(neutral_replies);
    }

    // Wrap the response with the color code and reset code
    return colorCode + response + "\033[0m";
}

// Helper function: Generate a response for a single sentence
string generateSentenceResponse(const vector<pair<int, double>>& emotionDetails) {
    string sentenceResponse;

    for (size_t i = 0; i < emotionDetails.size(); ++i) {
        int emotionType = emotionDetails[i].first;
        double score = emotionDetails[i].second;

        // Generate a response for the current emotion
        string response = generateEmotionResponse(emotionType, score);

        if (!sentenceResponse.empty()) {
            // Use "and" if the emotion type is the same as the previous one
            if (emotionDetails[i - 1].first == emotionType) {
                sentenceResponse += " and ";
            } else {
                // Use "but" if the emotion type is different
                sentenceResponse += " but ";
            }
        }
        sentenceResponse += response;
    }

    return sentenceResponse;
}

// Main function: Generate a response based on the emotions detected in a sentence
string generateResponse(vector<pair<double, vector<pair<int, double>>>>& emotionPerSentence) {
    stringstream finalResponse;
    cout << "Response is being generated!" << endl;

    // Check if emotionPerSentence is empty
    if (emotionPerSentence.empty()) {
        cout << "emotionPerSentence is empty!" << endl;
        return "I'm not sure how you're feeling right now, but I'm here to listen.";
    }

    // Iterate through the emotions and their scores for each sentence
    for (auto& emotion : emotionPerSentence) {
        // Check if the current sentence has no emotions
        if (emotion.second.empty()) {
            cout << "No emotions detected in this sentence!" << endl;
            continue;
        }

        double sentenceScore = emotion.first;
        vector<pair<int, double>> emotionDetails = sortEmotionsByIntensity(emotion.second);

        // Generate responses for the sentence
        string sentenceResponse = generateSentenceResponse(emotionDetails);

        // Add the sentence response to the final response
        if (!finalResponse.str().empty()) {
            finalResponse << " ";
        }
        finalResponse << sentenceResponse;
    }

    // If no valid responses were generated, return the fallback response
    if (finalResponse.str().empty()) {
        return "I'm not sure how you're feeling right now, but I'm here to listen.";
    }

    return finalResponse.str();
}

void respondHI() {
    cout << "Hi there, I am REIKO. Your caring helpful emotion tracker!" << endl; // Later I will add a file-based hello reply
    is_hi = false;
}

void responseGenerate() {
    cout << "Response called" << endl;
    if (is_hi) {
        respondHI();
    }

    double overallMoodScore = 0.0; // Initialize the overall mood score
    for (const auto& sentence : emotionPer_sentence) {
        overallMoodScore += sentence.first; // Add the sentence score to the overall mood score
    }

    if (overallMoodScore != 0) {
        // Add the overall mood score to the mood history
        moodHistory.push_back(overallMoodScore);
        saveMoodHistory("../user_data/mood_history.txt");
    }

    // Provide an overall sentiment summary
    cout << "\nOverall Sentiment Summary:" << endl;
    if (overallMoodScore > 5.0) {
        cout << "You seem to be in a very positive mood overall!" << endl;
    } else if (overallMoodScore < -5.0) {
        cout << "You seem to be feeling down. I'm here to help!" << endl;
    } else {
        cout << "Your overall mood seems neutral." << endl;
    }

    // Generate and print the final response
    string finalResponse = generateResponse(emotionPer_sentence);
    cout << "\nGenerated Response: " << finalResponse << endl;

    // Ask the user if they want mood history and analysis
    cout << "Do you want mood history and mood analysis? (Yes/No)" << endl;
    string userResponse;
    cin >> userResponse;
    if (userResponse == "Yes" || userResponse == "yes") {
        // Display mood history and analysis
        analyzeMoodChanges();
        displayMoodSummary();
    }
}