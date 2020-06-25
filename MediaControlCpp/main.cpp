#include "Server.h"
#include "pch.h"
int main(int argc, char* args[])
{
    win::init_apartment();
    HWND handle = GetConsoleWindow();
    ShowWindow(handle, SW_HIDE);
    MediaControl* mediaController = new MediaControl(handle);
    Server      server(3000);
    server.addController(mediaController);
    server.run();
}