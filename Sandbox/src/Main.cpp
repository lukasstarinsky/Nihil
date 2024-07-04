#include "Sandbox.hpp"

int main()
{
    Sandbox sandbox;
    Engine engine { &sandbox };

    engine.Run();

    return 0;
}