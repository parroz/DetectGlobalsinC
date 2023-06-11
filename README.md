# DetectGlobalsinC

`detect_globals` is a command-line program that analyzes C source files and detects global variables using the libclang library. It prints the global variables found, along with their locations in the source code. The program is designed to ignore variables defined in system libraries.

## Purpose and Scope

The purpose of `detect_globals` is to assist in code analysis by identifying global variables in C programs. It can be useful for understanding code complexity, identifying potential issues, or performing static analysis.

The program specifically focuses on global variables and ignores local variables, function parameters, and variables defined in system libraries. It aims to provide a simple and efficient way to extract global variable information from C source files.

The program ignores global variables with the qualifyer `const`.

## Compilation

To compile the `detect_globals` program, ensure that you have the Clang compiler installed on your system. The program uses the libclang library, which is typically bundled with Clang.

Use the following command to compile the program:

```bash
clang++ -std=c++17 detect_global_variables.cpp -o detect_globals -lclang -L/opt/homebrew/opt/llvm@15/lib -I/opt/homebrew/opt/llvm@15/include
```
This will generate an executable file named `detect_globals`.

## Usage
The program accepts one or more C source files as command-line arguments. It analyzes each file separately and prints any global variables found. If no global variables are detected, the program exits with code 0. If at least one global variable is found, the program exits with code 1.

To use the program, run the following command:

```bash
./detect_globals [-i /path/to/ignore1:/path/to/ignore2] file1.c file2.c
```

Replace file1.c, file2.c, etc., with the paths to the C source files you want to analyze. You can also use wildcard patterns to specify multiple files, such as *.c to analyze all C files in the current directory.

The program will display the global variables found, along with their locations in the source files.

Example:

```bash
./detect_globals main.c util.c
```


Output:
```yaml
Checking file: main.c
Variable: global_var
  Location: main.c:5:5

Checking file: util.c
Variable: global_count
  Location: util.c:8:9
Variable: global_ptr
  Location: util.c:9:10
```
