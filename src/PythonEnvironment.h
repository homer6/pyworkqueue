#pragma once

#include <Python.h>
#include <csignal>
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <memory>
#include <stdexcept>


namespace pyworkqueue {

    /*
type PyInterpreterConfig
Structure containing most parameters to configure a sub-interpreter. Its values are used only in Py_NewInterpreterFromConfig() and never modified by the runtime.

Added in version 3.12.

Structure fields:

int use_main_obmalloc
If this is 0 then the sub-interpreter will use its own “object” allocator state. Otherwise it will use (share) the main interpreter’s.

If this is 0 then check_multi_interp_extensions must be 1 (non-zero). If this is 1 then gil must not be PyInterpreterConfig_OWN_GIL.

int allow_fork
If this is 0 then the runtime will not support forking the process in any thread where the sub-interpreter is currently active. Otherwise fork is unrestricted.

Note that the subprocess module still works when fork is disallowed.

int allow_exec
If this is 0 then the runtime will not support replacing the current process via exec (e.g. os.execv()) in any thread where the sub-interpreter is currently active. Otherwise exec is unrestricted.

Note that the subprocess module still works when exec is disallowed.

int allow_threads
If this is 0 then the sub-interpreter’s threading module won’t create threads. Otherwise threads are allowed.

int allow_daemon_threads
If this is 0 then the sub-interpreter’s threading module won’t create daemon threads. Otherwise daemon threads are allowed (as long as allow_threads is non-zero).

int check_multi_interp_extensions
If this is 0 then all extension modules may be imported, including legacy (single-phase init) modules, in any thread where the sub-interpreter is currently active. Otherwise only multi-phase init extension modules (see PEP 489) may be imported. (Also see Py_mod_multiple_interpreters.)

This must be 1 (non-zero) if use_main_obmalloc is 0.

int gil
This determines the operation of the GIL for the sub-interpreter. It may be one of the following:

PyInterpreterConfig_DEFAULT_GIL
Use the default selection (PyInterpreterConfig_SHARED_GIL).

PyInterpreterConfig_SHARED_GIL
Use (share) the main interpreter’s GIL.

PyInterpreterConfig_OWN_GIL
Use the sub-interpreter’s own GIL.

If this is PyInterpreterConfig_OWN_GIL then PyInterpreterConfig.use_main_obmalloc must be 0.


    
    */



    class PythonInterpreterConfig : public PyInterpreterConfig {

        public:
            PythonInterpreterConfig(){
                this->use_main_obmalloc = 0;
                this->allow_fork = 0;
                this->allow_exec = 0;
                this->allow_threads = 0;
                this->allow_daemon_threads = 0;
                this->check_multi_interp_extensions = 1;
                this->gil = PyInterpreterConfig_OWN_GIL;
            }

    };




    class PythonInterpreter {

        PyThreadState* thread_state;
        PyInterpreterState* interpreter_state;

        PythonInterpreter( PythonInterpreterConfig& config )
            :thread_state(nullptr), interpreter_state(nullptr)
        {
            cout << 1 << endl;
            // this->interpreter_state = PyInterpreterState_New();
            // if( !this->interpreter_state ){
            //     throw std::runtime_error("Failed to create new Python interpreter state");
            // }

            cout << 2 << endl;

            PyStatus status = Py_NewInterpreterFromConfig(&this->thread_state, &config);
            if( PyStatus_Exception(status) ){
                throw std::runtime_error("Failed to create new Python interpreter from config");
            }

            if( !this->thread_state ){
                throw std::runtime_error("Failed to create new Python thread state");
            }

            cout << 3 << endl;

        }

    public:
        // https://docs.python.org/3/c-api/init.html#c.PyInterpreterState_New

        ~PythonInterpreter(){
            cout << "Destroying PythonInterpreter" << endl;
            if( this->thread_state ){
                cout << "Destroying PythonInterpreter thread state" << endl;
                PyThreadState_Swap(this->thread_state);

                cout << "Destroying PythonInterpreter interpreter state" << endl;
                Py_EndInterpreter(this->thread_state);

                this->thread_state = nullptr;
                this->interpreter_state = nullptr;
                cout << "Destroyed PythonInterpreter (inner)" << endl;
            }
            cout << "Destroyed PythonInterpreter (outer)" << endl;
        }

        // Prevent copying
        PythonInterpreter(const PythonInterpreter&) = delete;
        PythonInterpreter& operator=(const PythonInterpreter&) = delete;

        // Allow moving
        PythonInterpreter(PythonInterpreter&& other) noexcept
            : thread_state(other.thread_state), interpreter_state(other.interpreter_state) {
            other.thread_state = nullptr;
            other.interpreter_state = nullptr;
        }

        PythonInterpreter& operator=(PythonInterpreter&& other) noexcept {
            if (this != &other) {
                thread_state = other.thread_state;
                interpreter_state = other.interpreter_state;
                other.thread_state = nullptr;
                other.interpreter_state = nullptr;
            }
            return *this;
        }

        static std::shared_ptr<PythonInterpreter> create(){
            PythonInterpreterConfig config;
            return std::shared_ptr<PythonInterpreter>(new PythonInterpreter(config));
        }

        void run_code( const std::string& code ){
            PyThreadState* saved_thread_state = PyThreadState_Swap( this->thread_state );
            PyRun_SimpleString( code.c_str() );
            PyThreadState_Swap( saved_thread_state );
        }
    };



    class PythonEnvironment{

            PythonEnvironment() {
                Py_Initialize();
                main_tstate = PyThreadState_Get();
            }

        public:
            ~PythonEnvironment() {
                PyThreadState_Swap(main_tstate);
                Py_Finalize();
            }

            // Prevent copying and moving
            PythonEnvironment(const PythonEnvironment&) = delete;
            PythonEnvironment& operator=(const PythonEnvironment&) = delete;
            PythonEnvironment(PythonEnvironment&&) = delete;
            PythonEnvironment& operator=(PythonEnvironment&&) = delete;

            static std::shared_ptr<PythonEnvironment> create() {
                return std::shared_ptr<PythonEnvironment>(new PythonEnvironment());
            }

        private:
            PyThreadState* main_tstate;

    };

}