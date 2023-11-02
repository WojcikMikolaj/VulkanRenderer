#include <stdexcept>
#include <iostream>

import Wrapper;

int main()
{
    Wrapper app;

    try
    {
        app.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
