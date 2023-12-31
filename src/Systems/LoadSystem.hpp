//
// Created by jesse on 31/10/2023.
//

#ifndef BRACK_ENGINE_LOADSYSTEM_HPP
#define BRACK_ENGINE_LOADSYSTEM_HPP


#include "ISystem.hpp"

class LoadSystem : public ISystem {
public:
    LoadSystem();

    ~LoadSystem() override;

    void update(milliseconds deltaTime) override;

    const std::string getName() const override;

    void cleanUp() override;
    void clearCache() override;
};


#endif //BRACK_ENGINE_LOADSYSTEM_HPP
