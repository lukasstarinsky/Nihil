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
        LOG_FATAL("{}", e.what());
        return EXIT_FAILURE;
    }
}