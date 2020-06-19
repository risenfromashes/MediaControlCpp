#include "Server.h"
#include "pch.h"
int main()
{
    win::init_apartment();
    HWND          handle          = GetConsoleWindow();
    ShowWindow(handle, SW_HIDE);
    MediaControl* mediaController = new MediaControl(handle);
    std::atomic<bool> shouldCancel;
    std::thread th([](MediaControl* mediaController, std::atomic<bool>& shouldCancel) { 
        while (!shouldCancel) {
                mediaController->updateStatus(true, false);
                std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(200));
        }
    }, mediaController, std::ref(shouldCancel));
    Server      server(3000);
    server.addController(mediaController);
    server.run();
    if (th.joinable()) th.join();
}