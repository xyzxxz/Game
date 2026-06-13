#include "app.h"
#include <SDL3/SDL_main.h>

int main(int argc, char* argv[])
{
    app::App app;
    return app.run();
}
