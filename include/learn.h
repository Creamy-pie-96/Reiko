#pragma once
#include<string>
using namespace std;
void analyzeTrackedData();
void saveTrackedData(const string& emotionFile, const string& intensifierFile);
void loadTrackedData(const string& emotionFile, const string& intensifierFile);


void saveAdjectiveWords(const string& filename);
void saveAdverbWords(const string& filename);
void saveEmotionWords(const string& filename);
void saveIntensifierWords(const string& filename);