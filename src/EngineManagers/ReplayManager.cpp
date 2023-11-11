//
// Created by jesse on 02/11/2023.
//

#include "EngineManagers/ReplayManager.hpp"

ReplayManager ReplayManager::instance;

void ReplayManager::StartReplay(std::string filePath) {

}

void ReplayManager::PauseReplay() {

}

void ReplayManager::StopReplayCapture() {

}

void ReplayManager::StartReplayPlayback(std::string filePath) {

}

void ReplayManager::PauseReplayPlayback() {

}

void ReplayManager::StopReplayPlayback() {

}

ReplayManager &ReplayManager::GetInstance() {
    return instance;
}
