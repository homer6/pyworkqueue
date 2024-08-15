#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <iostream>

namespace nb = nanobind;

int main(){

    // Initialize the Python interpreter
    nb::initialize();

    try {
        // Create a Python scope
        nb::module_ main = nb::module_::import_("__main__");
        nb::object scope = main.attr("__dict__");

        // Python code as a string
        std::string python_code = R"(
print("Hello from Python!")
x = 10
y = 20
result = x + y
print(f"The sum of {x} and {y} is {result}")
)";

        // Execute the Python code
        nb::exec(python_code, scope);

        // Access Python variables from C++
        int result = nb::cast<int>(scope["result"]);
        std::cout << "Result retrieved in C++: " << result << std::endl;

        // Run more Python code
        nb::exec("print('Python can also use the result:', result)", scope);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
    
}