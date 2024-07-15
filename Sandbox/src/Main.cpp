#include "Sandbox.hpp"

int main()
{
    try
    {
        Sandbox sandbox;
        Engine engine { &sandbox };

        engine.Run();
        return EXIT_SUCCESS;
    }
    catch (const NihilException& e)
    {
        LOG_FATAL("%s", e.what());
        return EXIT_FAILURE;
    }
}