#include <Termina/Core/Window.hpp>

int main()
{
    Termina::Window window(1280, 720, "Hi");
    while (window.IsOpen()) {
        window.Update();
    }
    return 0;
}
