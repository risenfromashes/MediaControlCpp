#ifndef _MEDIA_CONTROL_SERVER_H
#define _MEDIA_CONTROL_SERVER_H

#include "FileStreamer.h"
#include "MediaControl.h"
#include "pch.h"
class Server {
    uWS::App app;
    int      port = 3000;

  public:
    static void writeStatusJson(MediaControl* const mediaController, uWS::HttpResponse<false>* res);

    static void unavailableStatus(uWS::HttpResponse<false>* res);

    Server(int port);
    void addController(MediaControl* const mediaController);
    void run();
};
#endif