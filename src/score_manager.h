#ifndef SCORE_MANAGER_H
#define SCORE_MANAGER_H

#include <string>
#include <vector>
#include <mutex>
#include <future>

struct ScoreEntry {
    std::string name;
    int score;
};

class ScoreManager {
 public:
  explicit ScoreManager(const std::string& filename);
  ~ScoreManager();

  void AddScore(const std::string& name, int score);
  std::vector<ScoreEntry> GetHighScores(int topN = 20);

  // Método para salvar scores assincronamente (retorna future para aguardar conclusão)
  std::future<void> SaveScoresAsync();

 private:
  void SaveScores(const std::vector<ScoreEntry>& scores);
  std::vector<ScoreEntry> LoadScores();

  std::string filename_;
  std::mutex mutex_;  // Protege acesso a scores
  std::vector<ScoreEntry> scores_;  // Cache local
  std::promise<void> save_promise_;
  std::thread saver_thread_;
};

#endif  // SCORE_MANAGER_H
