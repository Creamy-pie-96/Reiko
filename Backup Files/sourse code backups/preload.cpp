#include<iostream>
#include<fstream>
#include "filemanager.h"
#include "library.h"
#include "learn.h"

using namespace std;


vector<double> moodHistory; //for tracking users mood.

// Define the global variables declared as extern in library.h
unordered_set<string> valid_words;
unordered_set<string> adjective_words;
unordered_set<string> adverb_words;
unordered_set<string> hi_words;

unordered_set<string> positive_sentences;
unordered_set<string> neutral_sentences;
unordered_set<string> negative_sentences;

unordered_map<string, vector<pair<int, double>>> emotion_words; // Unified map for all emotion words
unordered_map<string, double> intensifier_words;

unordered_set<string> happy_replies_strong;
unordered_set<string> happy_replies_moderate;
unordered_set<string> sad_replies_strong;
unordered_set<string> sad_replies_moderate;
unordered_set<string> angry_replies_strong;
unordered_set<string> angry_replies_moderate;
unordered_set<string> fear_replies_moderate;
unordered_set<string> fear_replies_strong;
unordered_set<string> surprise_replies;
unordered_set<string> disgust_replies_moderate;
unordered_set<string> disgust_replies_strong;
unordered_set<string> neutral_replies;

// Global variables for tracking unknown words
unordered_map<string, TrackedEmotionWord> trackedEmotionWords;
unordered_map<string, TrackedIntensifierWord> trackedIntensifierWords;

void preload() {
    // Load sentences
   /* fileManager("../responses/Positive_sentences.txt", positive_sentences);
    fileManager("../responses/Neutral_sentences.txt", neutral_sentences);
    fileManager("../responses/Negative_sentences.txt", negative_sentences);
    */
    // Load words
    FileManager2("../mechine knowledge/emotion_words.txt", emotion_words);
    FileManager("../mechine knowledge/intensifier_words.txt", intensifier_words);

    fileManager("../mechine knowledge/valid_words.txt", valid_words);
    fileManager("../mechine knowledge/hi_words.txt", hi_words);
    fileManager("../mechine knowledge/adjective.txt", adjective_words);
    fileManager("../mechine knowledge/adverb.txt", adverb_words);

    // Load replies (ensure correct files for each emotion and intensity)
    fileManager("../responses/happy_replies_moderate.txt", happy_replies_moderate);
    fileManager("../responses/happy_replies_strong.txt", happy_replies_strong);
    fileManager("../responses/sad_replies_moderate.txt", sad_replies_moderate);
    fileManager("../responses/sad_replies_strong.txt", sad_replies_strong);
    fileManager("../responses/angry_replies_moderate.txt", angry_replies_moderate);
    fileManager("../responses/angry_replies_strong.txt", angry_replies_strong);
    fileManager("../responses/fear_replies_moderate.txt", fear_replies_moderate);
    fileManager("../responses/fear_replies_strong.txt", fear_replies_strong);
    fileManager("../responses/surprise_replies.txt", surprise_replies);
    fileManager("../responses/disgust_replies_moderate.txt", disgust_replies_moderate);
    fileManager("../responses/disgust_replies_strong.txt", disgust_replies_strong);
    fileManager("../responses/neutral_replies.txt", neutral_replies);

    loadMoodHistory("../user_data/mood_history.txt");
    // Load tracked data for self-learning
    loadTrackedData("../mechine knowledge/Potential emotion.txt", "../mechine knowledge/Potential intensifier.txt");


    return;
}
