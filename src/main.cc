// #include <csignal>
// #include <iostream>
// #include <thread>
// #include <vector>
// #include <atomic>
// #include <memory>
// #include <stdexcept>

// #include "PyWorkQueueApp.h"
// using pyworkqueue::PyWorkQueueApp;


// PyWorkQueueApp* PyWorkQueueApp::app_instance = nullptr;


// int main( int argc, char** argv ){

//     PyWorkQueueApp app(argc, argv);
//     PyWorkQueueApp::app_instance = &app;

//     std::signal(SIGINT, PyWorkQueueApp::signalHandler);
//     std::signal(SIGTERM, PyWorkQueueApp::signalHandler);
    
//     return app.run();

// }

#include <Python.h>
#include <stdio.h>

const char* check_gil_code = 
    "import sys, threading\n"
    "print(f'Interpreter ID: {id(sys)}')\n"
    "print(f'Thread ID: {threading.get_native_id()}')\n"
    "print(f'Has GIL: {sys.is_finalizing() == False}')\n";

int main() {
    PyStatus status;
    PyConfig config;
    PyThreadState *mainThreadState, *subThreadState;

    PyConfig_InitPythonConfig(&config);
    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status)) {
        printf("Failed to initialize Python\n");
        return 1;
    }

    mainThreadState = PyThreadState_Get();

    for (int i = 0; i < 4; i++) {
        printf("Creating subinterpreter %d...\n", i+1);

        PyInterpreterConfig interp_config = {
            .use_main_obmalloc = 0,
            .allow_fork = 0,
            .allow_exec = 0,
            .allow_threads = 1,
            .allow_daemon_threads = 0,
            .check_multi_interp_extensions = 1,
            .gil = PyInterpreterConfig_OWN_GIL
        };

        status = Py_NewInterpreterFromConfig(&subThreadState, &interp_config);
        if (PyStatus_Exception(status)) {
            fprintf(stderr, "Failed to create subinterpreter %d\n", i+1);
            Py_Finalize();
            return 1;
        }

        printf("Running GIL check in subinterpreter %d...\n", i+1);
        PyRun_SimpleString(check_gil_code);

        Py_EndInterpreter(subThreadState);
        PyThreadState_Swap(mainThreadState);
    }

    Py_Finalize();
    return 0;
}