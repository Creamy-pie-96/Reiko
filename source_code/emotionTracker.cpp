#include "library.h"
#include "emotion.h"
using namespace std;


void contextAwareness();

double emotionScore = 0;
vector<pair<double, vector<pair<int, double>>>> emotionPer_sentence;  // Store emotion score, emotion type, and intensity for each sentence
vector<pair<double, int>> intensifiervalu_andItsIndex;  // Intensifier value and its index
int startIndex = 0;
vector<pair<string, int>> pendingUnknownAdjectives; // Stores unknown adjectives and their positions
vector<pair<string, int>> pendingUnknownAdverbs;   // Stores unknown adverbs and their positions

// Function to calculate the cumulative intensifier value for a given range
double intensifier(const int &startingIndex, const int &endingIndex) {
    double intensify = 1.0;  // Start with no intensity adjustment (neutral multiplier)

    for (const auto &i : intensifiervalu_andItsIndex) {
        if (i.second >= startingIndex && i.second <= endingIndex) {
            intensify *= i.first;  // Use intensifier values directly as double
        }
    }
    cout<<"             INSIDE THE FUNCTION INTENSIFY ="<<intensify<<endl;
    return intensify;
}

// Function to update the emotion score immediately after processing an emotion word
void updateEmotionScore(double emotionValue, double intensify, int &emotionType) {
    double actualEmotionValue = emotionValue * intensify;
    emotionScore += actualEmotionValue;  // Update the total emotion score
    
    if (actualEmotionValue > -1.0 && actualEmotionValue < 1.0) {
        emotionType = 7; // Neutral
        cout << "Emotion weight is between -1.0 and 1.0. Setting emotion type to Neutral." << endl;
    }

    // Ensure the current sentence has an entry in emotionPer_sentence
    if (emotionPer_sentence.empty() || emotionPer_sentence.back().second.empty()) {
        emotionPer_sentence.push_back({emotionScore, {}});
    }

    emotionPer_sentence.back().second.push_back({emotionType, intensify});  // Store emotion type and its intensity

    // // Debug: Print the updated emotion score and type
    // cout << "Updated Emotion Score: " << emotionScore << endl;
    // cout << "Emotion Type: " << emotionType << ", Intensity: " << intensify << endl;
      // Set emotion type to Neutral if the weight is between -1.0 and 1.0
     
}

// Function to process when the sentence ends (finalize and store the data)
void thisIstheEnd() {
    cout << "this is the end called!" << endl;

    // Store the emotion score and corresponding emotion types with their intensities for the sentence
    if (!emotionPer_sentence.empty()) {
        emotionPer_sentence.back().first = emotionScore;  // Update the score for the current sentence
    } else {
        emotionPer_sentence.push_back({emotionScore, {}});
    }

    // Call the context awareness function to process unknown words
    contextAwareness();

    // Reset the emotion score
    emotionScore = 0;

    // Call response generation
    responseGenerate();
}

// Function to match keywords and calculate emotion score
void keyWord_match(const string tokenizedInput, const int index) {
    cout << "Processing token: " << tokenizedInput << " at index: " << index << endl;

    // Check for the end-of-input marker
    if (tokenizedInput == "__EnD_Of_InPuT_123!@#__") {
        cout << "End-of-input marker detected!" << endl;
        thisIstheEnd(); // Call the end-of-input processing
        return;
    }

    // Check for sentence-ending punctuation
    else if (tokenizedInput == "." || tokenizedInput == "!" || tokenizedInput == "?") {
        cout << "End of sentence detected!" << endl;
        intensifiervalu_andItsIndex.clear();
        startIndex = index + 1;
        return;
    }

    // Check if the word is an intensifier
    auto intensifierItr = intensifier_words.find(tokenizedInput);
    if (intensifierItr != intensifier_words.end()) {
        cout << "Intensifier detected: " << tokenizedInput << endl;
    
        // Add the detected intensifier's value and index to the vector
        intensifiervalu_andItsIndex.emplace_back(intensifierItr->second, index);
        cout << "Added intensifier value: " << intensifierItr->second << " at index: " << index << endl;
    
        // Update the tracked intensifier words
        trackedIntensifierWords[tokenizedInput].frequency++;
        for (const auto& intensifier : intensifiervalu_andItsIndex) {
            trackedIntensifierWords[tokenizedInput].context.push_back(intensifier.first);
        }
    
        return;
    }

    // Check if the word is an emotion word
    auto emotionItr = emotion_words.find(tokenizedInput);
    if (emotionItr != emotion_words.end()) {
        cout << "Emotion word detected: " << tokenizedInput << endl;
        for (const auto& emotionPair : emotionItr->second) {
            int emotionType = emotionPair.first;
            double emotionValue = emotionPair.second;
            double intensify = intensifier(startIndex, index);
            cout<<"value of intensify="<<intensify<<endl;
            if (intensify < -1 || (intensify > -1 && intensify < 0)) {
                intensify = -1 / intensify;
            }
             // Invert the emotion type if the intensifier is negative
             if (intensify < 0) {
                emotionType = -emotionType;
            }
            cout<<"Value of intensify="<<intensify<<" value of emotion type="<<emotionType<<endl;
            updateEmotionScore(emotionValue, intensify, emotionType);
        }
        startIndex = index + 1;
        return;
    }

    // Check if the word is in adjective.txt (potential emotion word)
    else if (adjective_words.find(tokenizedInput) != adjective_words.end()) {
        cout << "Unknown adjective detected: " << tokenizedInput << endl;

        // Add the unknown adjective to the pending list
        pendingUnknownAdjectives.push_back({tokenizedInput, index});

        return;
    }

    // Check if the word is in adverb.txt (potential intensifier)
    else if (adverb_words.find(tokenizedInput) != adverb_words.end()) {
        cout << "Unknown adverb detected: " << tokenizedInput << endl;

        // Add the unknown adverb to the pending list
        pendingUnknownAdverbs.push_back({tokenizedInput, index});

        return;
    }

    cout << "No match found for token: " << tokenizedInput << endl;
}

void contextAwareness() {
    cout << "Context awareness function called!" << endl;

    // Process pending unknown adjectives
    for (const auto& [unknownAdjective, position] : pendingUnknownAdjectives) {
        cout << "Processing unknown adjective: " << unknownAdjective << " at position: " << position << endl;

        // Collect context (before and after the unknown word)
        vector<pair<int, double>> context;
        if (!emotionPer_sentence.empty()) {
            for (const auto& emotion : emotionPer_sentence.back().second) {
                context.push_back({emotion.first, emotion.second}); // Store emotion type and weight
            }
        }

        // Store the context for the unknown adjective
        trackedEmotionWords[unknownAdjective].frequency++;
        trackedEmotionWords[unknownAdjective].context.insert(
            trackedEmotionWords[unknownAdjective].context.end(),
            context.begin(),
            context.end()
        );
    }

    // Process pending unknown adverbs
    for (const auto& [unknownAdverb, position] : pendingUnknownAdverbs) {
        cout << "Processing unknown adverb: " << unknownAdverb << " at position: " << position << endl;

        // Collect context (before and after the unknown word)
        vector<double> context;
        for (const auto& intensifier : intensifiervalu_andItsIndex) {
            context.push_back(intensifier.first);
        }

        // Store the context for the unknown adverb
        trackedIntensifierWords[unknownAdverb].frequency++;
        trackedIntensifierWords[unknownAdverb].context.insert(
            trackedIntensifierWords[unknownAdverb].context.end(),
            context.begin(),
            context.end()
        );
    }

    // Clear the pending lists
    pendingUnknownAdjectives.clear();
    pendingUnknownAdverbs.clear();
}