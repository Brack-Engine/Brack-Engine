//
// Created by Olaf van den Berg on 08/12/2023.
//

#ifndef BRACKOCALYPSE_IBEHAVIOURSCRIPT_HPP
#define BRACKOCALYPSE_IBEHAVIOURSCRIPT_HPP

#include <functional>
#include <optional>
#include "Milliseconds.hpp"


class IBehaviourScript {
public:
    explicit IBehaviourScript() = default;

    virtual ~IBehaviourScript() = default;

    virtual void onStart() {};

    virtual void onUpdate(milliseconds deltaTime) {};

    IBehaviourScript(const IBehaviourScript &other) {
        entityId = other.entityId;
        isActive = other.isActive;
    }

    IBehaviourScript(std::remove_reference<struct std::unique_ptr<struct IBehaviourScript> &>::type type) {
        entityId = type->entityId;
        isActive = type->isActive;
    }

    [[nodiscard]] virtual std::unique_ptr<IBehaviourScript> clone() const = 0;


    entity entityId = 0;
    bool isActive = true;
};

#endif //BRACKOCALYPSE_IBEHAVIOURSCRIPT_HPP
