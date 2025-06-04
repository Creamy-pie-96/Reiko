#pragma once
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
using namespace std;

void keyWord_match(const string tokenizedInput, const int index);
void responseGenerate();
void displayMoodSummary();
void analyzeMoodChanges();
void saveMoodHistory(const string& filename);

extern vector<int> saveNegation;
extern double emotionScore;
extern int startIndex;
extern vector<pair<double, vector<pair<int, double>>>> emotionPer_sentence; // Declare as extern
extern vector<pair<double, int>> intensifiervalu_andItsIndex;
extern unordered_set<string> hi_words;
