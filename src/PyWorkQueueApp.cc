#include "PyWorkQueueApp.h"

#include "PythonEnvironment.h"

#include <thread>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;


namespace pyworkqueue {

    PyWorkQueueApp::PyWorkQueueApp( int argc, char** argv )
        :py_env( PythonEnvironment::create() )
    {
        //set argv
        //PySys_SetArgv(argc, argv);
    }

    int PyWorkQueueApp::run(){
            
        constexpr int num_interpreters = 4;

        {

            std::vector<std::shared_ptr<PyInterpreter>> interpreters;
            std::vector<std::thread> threads;
            
            for( int i = 0; i < num_interpreters; i++ ){
                interpreters.push_back(PyInterpreter::create());
                threads.emplace_back(&PyWorkQueueApp::runInterpreter, this, interpreters.back());
            }

            int x = 0;
            while( !this->should_stop.load() ){
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                cout << "Main thread running " << x++ << endl;
            }

            // Ensure all threads exit
            for( auto& thread : threads ){
                if( thread.joinable() ){
                    thread.join();
                    cout << "Thread joined successfully." << endl;
                }else{
                    cerr << "Thread was not joinable." << endl;
                }
            }

            cout << "About to destroy threads." << endl;

        }

        cout << "Exiting main thread." << endl;
        return 0;

    }

}