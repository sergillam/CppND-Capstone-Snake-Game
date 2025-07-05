#include "score_manager.h"
#include <fstream>
#include <sstream>
#include <algorithm>

ScoreManager::ScoreManager(const std::string& filename) : filename_(filename) {}

void ScoreManager::AddScore(const std::string& name, int score) {
    auto scores = LoadScores();
    scores.push_back({name, score});
    std::sort(scores.begin(), scores.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
        return b.score < a.score; // Maior score primeiro
    });
    if (scores.size() > 20) scores.resize(20); // Limita para não crescer demais
    SaveScores(scores);
}

std::vector<ScoreEntry> ScoreManager::GetHighScores(int topN) {
    auto scores = LoadScores();
    std::sort(scores.begin(), scores.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
        return b.score < a.score;
    });
    if ((int)scores.size() > topN) scores.resize(topN);
    return scores;
}

void ScoreManager::SaveScores(const std::vector<ScoreEntry>& scores) {
    std::ofstream file(filename_);
    for (const auto& entry : scores) {
        file << entry.name << "," << entry.score << "\n";
    }
}

std::vector<ScoreEntry> ScoreManager::LoadScores() {
    std::vector<ScoreEntry> scores;
    std::ifstream file(filename_);
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name, scoreStr;
        if (std::getline(ss, name, ',') && std::getline(ss, scoreStr)) {
            scores.push_back({name, std::stoi(scoreStr)});
        }
    }
    return scores;
}
