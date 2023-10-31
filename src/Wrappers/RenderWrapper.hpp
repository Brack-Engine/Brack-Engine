//
// Created by jesse on 30/10/2023.
//

#ifndef BRACK_ENGINE_RENDERWRAPPER_HPP
#define BRACK_ENGINE_RENDERWRAPPER_HPP

#include <iostream>
#include "SDL.h"

#ifdef _WIN32

#include <windows.h>

#endif

class RenderWrapper {
public:
    RenderWrapper();

    ~RenderWrapper();

    bool Initialize();

    void Run();

    void Cleanup();


};


#endif //BRACK_ENGINE_RENDERWRAPPER_HPP
