#pragma once

#include <memory>
using std::shared_ptr;

#include <csignal>

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;


#include "PythonEnvironment.h"




namespace pyworkqueue {

    class PyWorkQueueApp {

        public:
            PyWorkQueueApp( int argc, char** argv );
            int run();


            //runs in own thread
            void runInterpreter( std::shared_ptr<PythonInterpreter> interpreter ) noexcept{
                
                try{
                    interpreter->run_code("import sys");
                    interpreter->run_code("print('Python version:', sys.version)");                    
                }catch( std::exception& e ){
                    std::cerr << "Error running Python code: " << e.what() << std::endl;
                    //return exception to main thread
                }catch(...){
                    std::cerr << "Unknown error running Python code" << std::endl;
                    //return exception to main thread                    
                }

                cout << "Exiting interpreter thread: " << std::this_thread::get_id() << endl;

            }

            void callSignalHandler( int signal ){
                cout << "Stopping app due to signal: " << signal << endl;
                should_stop.store(true);
            }

            static void signalHandler( int signal ){
                if( app_instance ){
                    app_instance->callSignalHandler(signal);
                }
            }

            static PyWorkQueueApp* app_instance;


        private:
            shared_ptr<PythonEnvironment> py_env;
            std::atomic<bool> should_stop{false};

    };

}