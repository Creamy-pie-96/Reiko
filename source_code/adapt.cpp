#include "library.h"
#include<fstream>
void saveAdjectiveWords(const string& filename) {
    ofstream file(filename);
    if (!file) {
        cerr << "Error saving adjective words!" << endl;
        return;
    }
    for (const auto& word : adjective_words) {
        file << word << endl;
    }
    file.close();
}

void saveAdverbWords(const string& filename) {
    ofstream file(filename);
    if (!file) {
        cerr << "Error saving adverb words!" << endl;
        return;
    }
    for (const auto& word : adverb_words) {
        file << word << endl;
    }
    file.close();
}

void saveEmotionWords(const string& filename) {
    ofstream file(filename);
    if (!file) {
        cerr << "Error saving emotion words to file: " << filename << endl;
        return;
    }

    for (const auto& [word, emotions] : emotion_words) {
        for (const auto& [type, value] : emotions) {
            file << word << " " << type << " " << value << endl;
        }
    }

    file.close();
    cout << "Emotion words saved successfully to " << filename << endl;
}
void saveIntensifierWords(const string& filename) {
    ofstream file(filename);
    if (!file) {
        cerr << "Error saving intensifier words to file: " << filename << endl;
        return;
    }

    for (const auto& [word, value] : intensifier_words) {
        file << word << " " << value << endl;
    }

    file.close();
    cout << "Intensifier words saved successfully to " << filename << endl;
}