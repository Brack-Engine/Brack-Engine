//
// Created by jesse on 02/11/2023.
//

#ifndef BRACK_ENGINE_REPLAYMANAGER_HPP
#define BRACK_ENGINE_REPLAYMANAGER_HPP

#include <string>
#include <memory>
#include "../includes/EntityManager.hpp"

class ReplayManager {
public:
    static ReplayManager &GetInstance();

    ~ReplayManager() = default;

    ReplayManager(const ReplayManager &) = delete;

    ReplayManager &operator=(const ReplayManager &) = delete;

    ReplayManager &operator=(ReplayManager &&) = delete;

    ReplayManager(ReplayManager &&) = delete;

    void StartReplay(std::string filePath);

    void PauseReplay();

    void StopReplayCapture();

    void StartReplayPlayback(std::string filePath);

    void PauseReplayPlayback();

    void StopReplayPlayback();

private:
    static ReplayManager instance;

    ReplayManager() = default;
};


#endif //BRACK_ENGINE_REPLAYMANAGER_HPP
