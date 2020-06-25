#include "Server.h"

void Server::writeStatusJson(MediaControl* const mediaController, uWS::HttpResponse<false>* res)
{
    res->writeHeader("Content-Type", "application/json");
    res->end(mediaController->getStatusJson());
}

void Server::unavailableStatus(uWS::HttpResponse<false>* res)
{
    res->writeStatus("503 Service Unavailable");
    res->end();
}

Server::Server(int _port) { port = _port; }
void Server::addController(MediaControl* const mediaController)
{
    app.get("/status",
            [mediaController](uWS::HttpResponse<false>* res, uWS::HttpRequest* req) {
                if (mediaController->updateStatus(false))
                    Server::writeStatusJson(mediaController, res);
                else
                    Server::unavailableStatus(res); 
            })
        .get("/thumbnail",
             [mediaController](uWS::HttpResponse<false>* res, uWS::HttpRequest* req) {
                 win::IRandomAccessStream stream;
                 int count = 1;
                 stream    = mediaController->getThumbnail();
                 while(!stream && count <= 5) {
                     //std::cout << "Failed to get stream for " << count << "th time." << std::endl;
                     stream = mediaController->getThumbnail();
                     count++;
                 };
                 if (stream) {
                     FileStreamer fs(stream);
                     fs.write(res);
                 }
                 else 
                     Server::unavailableStatus(res);                 
             })
        .post("/play",
              [mediaController](uWS::HttpResponse<false>* res, uWS::HttpRequest* req) {
                  if (mediaController->play()) {
                      mediaController->updateStatus(false);
                      Server::writeStatusJson(mediaController, res);
                  }
                  else
                      Server::unavailableStatus(res);
              })
        .post("/pause",
              [mediaController](uWS::HttpResponse<false>* res, uWS::HttpRequest* req) {
                  if (mediaController->pause()) {
                      mediaController->updateStatus(false);
                      Server::writeStatusJson(mediaController, res);
                  }
                  else
                      Server::unavailableStatus(res);
              })
        .post("/next",
              [mediaController](uWS::HttpResponse<false>* res, uWS::HttpRequest* req) {
                  if (mediaController->next()) {
                      mediaController->updateStatus(true);
                      Server::writeStatusJson(mediaController, res);
                  }
                  else
                      Server::unavailableStatus(res);
              })
        .post("/prev",
              [mediaController](uWS::HttpResponse<false>* res, uWS::HttpRequest* req) {
                  if (mediaController->prev()) {
                      mediaController->updateStatus(false);
                      Server::writeStatusJson(mediaController, res);
                  }
                  else
                      Server::unavailableStatus(res);
              })
        .post("/volume_up",
              [mediaController](uWS::HttpResponse<false>* res, uWS::HttpRequest* req) {
                  mediaController->volUp();
                  Server::writeStatusJson(mediaController, res);
              })
        .post("/volume_down", [mediaController](uWS::HttpResponse<false>* res, uWS::HttpRequest* req) {
            mediaController->volDown();
            Server::writeStatusJson(mediaController, res);
        });
}
void Server::run()
{
    int __port = port;
    app.listen(3000,
               [__port](us_listen_socket_t* token) {
                   if (token) std::cout << "Listening on port " << __port << std::endl;
               })
        .run();
}