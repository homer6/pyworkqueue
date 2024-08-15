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



    class PyInterpreter {

        PyThreadState* tstate;
        PyInterpreterState* interp_state;

        PyInterpreter(PyThreadState* ts, PyInterpreterState* is)
            : tstate(ts), interp_state(is) {}

    public:
        ~PyInterpreter() {
            if (tstate) {
                PyThreadState_Swap(tstate);
                Py_EndInterpreter(tstate);
                tstate = nullptr;
            }
        }

        // Prevent copying
        PyInterpreter(const PyInterpreter&) = delete;
        PyInterpreter& operator=(const PyInterpreter&) = delete;

        // Allow moving
        PyInterpreter(PyInterpreter&& other) noexcept
            : tstate(other.tstate), interp_state(other.interp_state) {
            other.tstate = nullptr;
            other.interp_state = nullptr;
        }

        PyInterpreter& operator=(PyInterpreter&& other) noexcept {
            if (this != &other) {
                tstate = other.tstate;
                interp_state = other.interp_state;
                other.tstate = nullptr;
                other.interp_state = nullptr;
            }
            return *this;
        }

        static std::shared_ptr<PyInterpreter> create() {
            PyInterpreterConfig config;
            config.gil = PyInterpreterConfig_OWN_GIL; // Create a new GIL for this interpreter
            config.use_main_obmalloc = 0; // Use a separate memory allocator if needed
            config.allow_threads = 1;

            PyThreadState* new_ts = nullptr;
            PyStatus status = Py_NewInterpreterFromConfig(&new_ts, &config);

            if (!new_ts) {
                throw std::runtime_error("Failed to create new Python interpreter");
            }

            return std::shared_ptr<PyInterpreter>(new PyInterpreter(new_ts, new_ts->interp));
        }

        void run_code(const std::string& code) {
            PyThreadState* save_tstate = PyThreadState_Swap(tstate);
            PyRun_SimpleString(code.c_str());
            PyThreadState_Swap(save_tstate);
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