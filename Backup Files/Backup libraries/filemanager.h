#pragma once
#include<unordered_map>
#include<unordered_set>
#include<string>
#include<vector>
using namespace std;

void fileManager(string filename,unordered_set<string> &input);
void FileManager(string filename,unordered_map<string, double>& input);
void FileManager2(string filename, unordered_map<string, vector<pair<int, double>>>& input);
void loadMoodHistory(const string& filename);