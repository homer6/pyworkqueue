#include <csignal>
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <memory>
#include <stdexcept>

#include "PyWorkQueueApp.h"
using pyworkqueue::PyWorkQueueApp;


PyWorkQueueApp* PyWorkQueueApp::app_instance = nullptr;


int main( int argc, char** argv ){

    PyWorkQueueApp app(argc, argv);
    PyWorkQueueApp::app_instance = &app;

    std::signal(SIGINT, PyWorkQueueApp::signalHandler);
    std::signal(SIGTERM, PyWorkQueueApp::signalHandler);
    
    app.run();
   
    return 0;

}