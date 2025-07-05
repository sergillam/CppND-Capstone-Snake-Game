#pragma once

#include <string>
#include <vector>

struct ScoreEntry {
    std::string name;
    int score;
};

class ScoreManager {
public:
    ScoreManager(const std::string& filename);

    void AddScore(const std::string& name, int score);
    std::vector<ScoreEntry> GetHighScores(int topN = 5);

private:
    std::string filename_;
    void SaveScores(const std::vector<ScoreEntry>& scores);
    std::vector<ScoreEntry> LoadScores();
};
