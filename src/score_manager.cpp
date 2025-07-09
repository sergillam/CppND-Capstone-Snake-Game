#include "score_manager.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <thread>
#include <iostream>

ScoreManager::ScoreManager(const std::string& filename) : filename_(filename) {
    scores_ = LoadScores();
}

ScoreManager::~ScoreManager() {
    // Aguarda a thread de salvamento terminar antes de destruir objeto
    if (saver_thread_.joinable()) {
        saver_thread_.join();
    }
}

void ScoreManager::AddScore(const std::string& name, int score) {
    std::lock_guard<std::mutex> lock(mutex_);
    scores_.push_back({name, score});
    std::sort(scores_.begin(), scores_.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
        return b.score < a.score;  // Descrescente
    });
    if (scores_.size() > 20) scores_.resize(20);
}

std::vector<ScoreEntry> ScoreManager::GetHighScores(int topN) {
    std::lock_guard<std::mutex> lock(mutex_);
    if ((int)scores_.size() > topN) {
        return std::vector<ScoreEntry>(scores_.begin(), scores_.begin() + topN);
    }
    return scores_;
}

void ScoreManager::SaveScores(const std::vector<ScoreEntry>& scores) {
    std::ofstream file(filename_);
    if (!file.is_open()) {
        std::cerr << "Failed to open score file for writing: " << filename_ << std::endl;
        return;
    }
    for (const auto& entry : scores) {
        file << entry.name << "," << entry.score << "\n";
    }
}

std::future<void> ScoreManager::SaveScoresAsync() {
    // Cria promise e future para notificação
    save_promise_ = std::promise<void>();
    std::future<void> fut = save_promise_.get_future();

    // Copia os scores protegidos por mutex
    std::vector<ScoreEntry> scores_copy;
    {
      std::lock_guard<std::mutex> lock(mutex_);
      scores_copy = scores_;
    }

    // Se existir thread anterior rodando, aguarda
    if (saver_thread_.joinable()) {
        saver_thread_.join();
    }

    // Cria nova thread para salvar em background
    saver_thread_ = std::thread([this, scores_copy]() mutable {
        SaveScores(scores_copy);
        save_promise_.set_value();
    });

    return fut;
}

std::vector<ScoreEntry> ScoreManager::LoadScores() {
    std::vector<ScoreEntry> loaded_scores;
    std::ifstream file(filename_);
    if (!file.is_open()) {
        // Arquivo pode não existir ainda, não é erro fatal
        return loaded_scores;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name, scoreStr;
        if (std::getline(ss, name, ',') && std::getline(ss, scoreStr)) {
            try {
                loaded_scores.push_back({name, std::stoi(scoreStr)});
            } catch (...) {
                // Ignorar linhas inválidas
            }
        }
    }
    return loaded_scores;
}
