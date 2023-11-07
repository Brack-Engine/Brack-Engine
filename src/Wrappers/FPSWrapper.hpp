//
// Created by jesse on 07/11/2023.
//

#ifndef BRACKOCALYPSE_FPSWRAPPER_HPP
#define BRACKOCALYPSE_FPSWRAPPER_HPP

#include <SDL.h>

class FPSWrapper {
public:
    FPSWrapper() = default;
    ~FPSWrapper() = default;
    void Start();
    void End();
    int GetFPS() const;
private:
    Uint64 start, end, fps;
};


#endif //BRACKOCALYPSE_FPSWRAPPER_HPP
