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

const char* python_code = 
    "import sys\n"
    "print(f'Hello from subinterpreter {id(sys)}!')\n"
    "x = 42\n"
    "print(f'The value of x is {x}')\n";

int main() {
    PyStatus status;
    PyConfig config;
    PyThreadState *mainThreadState, *subThreadState;

    printf("Initializing Python...\n");
    PyConfig_InitPythonConfig(&config);
    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status)) {
        printf("Failed to initialize Python\n");
        return 1;
    }
    printf("Python initialized successfully\n");

    mainThreadState = PyThreadState_Get();

    for (int i = 0; i < 4; i++) {
        printf("Creating subinterpreter %d...\n", i+1);
        subThreadState = Py_NewInterpreter();
        if (subThreadState == NULL) {
            fprintf(stderr, "Failed to create subinterpreter %d\n", i+1);
            Py_Finalize();
            return 1;
        }
        printf("Subinterpreter %d created successfully\n", i+1);

        printf("Running Python code in subinterpreter %d...\n", i+1);
        PyRun_SimpleString(python_code);
        printf("Python code execution completed in subinterpreter %d\n", i+1);

        printf("Ending subinterpreter %d...\n", i+1);
        Py_EndInterpreter(subThreadState);
        printf("Subinterpreter %d ended\n", i+1);

        PyThreadState_Swap(mainThreadState);
    }

    printf("Finalizing Python...\n");
    Py_Finalize();
    printf("Python finalized successfully\n");
    return 0;
}