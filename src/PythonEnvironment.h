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


    class PyGILState{

            PyGILState_STATE state;

        public:
            PyGILState() : state(PyGILState_Ensure()) {}
            ~PyGILState() { PyGILState_Release(state); }

            // Prevent copying
            PyGILState(const PyGILState&) = delete;
            PyGILState& operator=(const PyGILState&) = delete;
    };



    class PyInterpreter {

            PyThreadState* tstate;

            PyInterpreter(PyThreadState* ts) : tstate(ts) {}

        public:
            ~PyInterpreter() {
                if (tstate) {
                    PyThreadState_Swap(tstate);
                    Py_EndInterpreter(tstate);
                }
            }

            // Prevent copying
            PyInterpreter(const PyInterpreter&) = delete;
            PyInterpreter& operator=(const PyInterpreter&) = delete;

            // Allow moving
            PyInterpreter(PyInterpreter&& other) noexcept : tstate(other.tstate) {
                other.tstate = nullptr;
            }
            PyInterpreter& operator=(PyInterpreter&& other) noexcept {
                if (this != &other) {
                    tstate = other.tstate;
                    other.tstate = nullptr;
                }
                return *this;
            }

            static std::shared_ptr<PyInterpreter> create() {
                PyThreadState* new_ts = Py_NewInterpreter();
                if (!new_ts) {
                    throw std::runtime_error("Failed to create new Python interpreter");
                }
                return std::shared_ptr<PyInterpreter>(new PyInterpreter(new_ts));
            }

            void run_code(const std::string& code) {
                PyGILState gil;
                PyRun_SimpleString(code.c_str());
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