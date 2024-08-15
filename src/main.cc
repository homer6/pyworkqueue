#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <iostream>

namespace nb = nanobind;

int main() {
    // Initialize the Python interpreter
    nb::interpreter interpreter;

    try {
        // Create a Python scope
        nb::module_ main = nb::module_::import_("__main__");
        nb::object globals = main.attr("__dict__");

        // Python code as a string
        std::string python_code = R"(
print("Hello from Python!")
x = 10
y = 20
result = x + y
print(f"The sum of {x} and {y} is {result}")
)";

        // Execute the Python code
        nb::eval<nb::eval_statements>(python_code, globals);

        // Access Python variables from C++
        int result = nb::cast<int>(globals["result"]);
        std::cout << "Result retrieved in C++: " << result << std::endl;

        // Run more Python code
        nb::eval<nb::eval_statements>("print('Python can also use the result:', result)", globals);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}