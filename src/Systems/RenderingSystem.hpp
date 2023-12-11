//
// Created by jesse on 31/10/2023.
//

#ifndef BRACK_ENGINE_RENDERINGSYSTEM_HPP
#define BRACK_ENGINE_RENDERINGSYSTEM_HPP


#include <memory>
#include <set>
#include "ISystem.hpp"
#include "../Wrappers/RenderWrapper.hpp"


struct CompareByLayer {
    bool operator()(const RenderArchetype *lhs, const RenderArchetype *rhs) const {
        return std::tie(lhs->sortingLayer, lhs->orderInLayer) > std::tie(rhs->sortingLayer, rhs->orderInLayer);
    }
};

class RenderingSystem : public ISystem {
public:
    RenderingSystem();

    ~RenderingSystem() override;

    void cleanUp() override;
    void clearCache() override;

    const std::string getName() const override;

    RenderingSystem(const RenderingSystem &other);

    void update(milliseconds deltaTime) override;

private:
    void SortRenderComponents();

    std::multiset<RenderArchetype *, CompareByLayer> components;
    std::multiset<RenderArchetype *, CompareByLayer> uiComponents;
#if CURRENT_LOG_LEVEL >= LOG_LEVEL_DEBUG
    std::set<CollisionArchetype *> collisionComponents;
    std::set<CollisionArchetype *> uiCollisionComponents;
#endif
    std::unique_ptr<RenderWrapper> sdl2Wrapper;
};

#endif //BRACK_ENGINE_RENDERINGSYSTEM_HPP
