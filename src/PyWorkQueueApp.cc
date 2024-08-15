#include "PyWorkQueueApp.h"

#include "PythonEnvironment.h"

#include <thread>


namespace pyworkqueue {

    PyWorkQueueApp::PyWorkQueueApp( int argc, char** argv )
        :py_env( PythonEnvironment::create() )
    {
        //set argv
        //PySys_SetArgv(argc, argv);
    }

    void PyWorkQueueApp::run(){
            
        constexpr int num_interpreters = 4;
        std::vector<std::shared_ptr<PyInterpreter>> interpreters;
        std::vector<std::thread> threads;
        
        for( int i = 0; i < num_interpreters; i++ ){
            interpreters.push_back(PyInterpreter::create());
            threads.emplace_back(&PyWorkQueueApp::runInterpreter, this, interpreters.back());
        }

        while( !this->should_stop.load() ){
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        for( auto& thread : threads ){
            if( thread.joinable() ){
                thread.join();
            }
        }

    }

}