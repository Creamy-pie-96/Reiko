#pragma once
#include <iostream>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <vector>
#include <string>

using namespace std;

// Defined functions
void preload();
void search();
void fixTypo(vector<string>& user_input);
void normalizer(vector<string>& user_input);
map<int, string> tokenizer(vector<string>& user_input, const unordered_set<string>& hi_words);

// Data structures and global variables
extern unordered_set<string> valid_words;

extern unordered_set<string> positive_sentences;
extern unordered_set<string> neutral_sentences;
extern unordered_set<string> negative_sentences;

extern unordered_map<string, double> intensifier_words;
extern unordered_map<string, vector<pair<int, double>>> emotion_words; // Unified map for all emotion words


extern unordered_set<string> adjective_words;
extern unordered_set<string> adverb_words;

// Emotion replies (ensure consistency for each emotional reply set)
extern unordered_set<string> happy_replies_moderate;
extern unordered_set<string> happy_replies_strong;
extern unordered_set<string> sad_replies_moderate;
extern unordered_set<string> sad_replies_strong;
extern unordered_set<string> angry_replies_moderate;
extern unordered_set<string> angry_replies_strong;
extern unordered_set<string> fear_replies_moderate;
extern unordered_set<string> fear_replies_strong;
extern unordered_set<string> surprise_replies;
extern unordered_set<string> disgust_replies_moderate;
extern unordered_set<string> disgust_replies_strong;
extern unordered_set<string> neutral_replies;

extern vector<string> user_input;
extern bool is_hi;
extern unordered_set<string> hi_words;
extern vector<double> moodHistory;


// for self learning
// Structure for tracking potential emotion words
struct TrackedEmotionWord {
    vector<pair<int, double>> context; // Emotion type and intensity within the same sentence
    int frequency; // Frequency of the word appearing in similar contexts
    vector<int> prevSentenceEmotionTypes; // Emotion types from the previous sentence
    vector<int> nextSentenceEmotionTypes; // Emotion types from the next sentence
};
// Structure for tracking potential intensifier words
struct TrackedIntensifierWord {
    vector<double> context; // Intensifier values within the same sentence
    int frequency; // Frequency of the word appearing in similar contexts
    vector<double> prevSentenceIntensifiers; // Intensifier values from the previous sentence
    vector<double> nextSentenceIntensifiers; // Intensifier values from the next sentence
};
// Global variables for tracking unknown words
extern unordered_map<string, TrackedEmotionWord> trackedEmotionWords;
extern unordered_map<string, TrackedIntensifierWord> trackedIntensifierWords;