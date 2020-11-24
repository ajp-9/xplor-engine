#include "Core.hpp"

#include <iostream>

namespace xplor
{
    void Core::initApplicatiion()
    {
        window.Init();
    }

    Core::~Core()
    {
        window.~Window();
    }
}