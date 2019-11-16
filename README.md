# Expert System
École 42 assignment - an expert system for propositional calculus using backward-chaining algorithm

## About the project
* This is a group project of [École 42](https://42.fr).
* My teammate is [keatchut](https://github.com/keatchut).
* The objective of this project is to create a program that can take logical statements, their initial value and use backward chaining algorithm to find the value of the queried facts.


## System Requirements
* Compiler that support C++17
* CMake 3.10+
* Boost library 1.68+


## Installation

### Requirements
* MacOS
    ```
    brew install cmake
    brew install boost
    ```
* Ubuntu 18.04
    ```
    sudo apt update
    sudo apt install cmake
    sudo apt install libboost-all-dev
    ```

### Expert System
```
mkdir build
cd build
cmake ..
cmake -build .
cd ..
```
The executable will be at `bin/expert_system`.


## Usage
To run in normal mode
```
./bin/expert_system [filename]
```

### Options
Shorthand | Full | Mode | Description
--- | --- | --- | ---
-h | --help | | show help message 
-i | --interactive | interactive mode | user can change initial facts and query without having to rerun the program
-v | --visualisation | reasoning visualisation mode | display step-by-step backward chaining reasoning process
-d | --debug | debug mode | display all values after evaluation
-c | --color | color mode | display with color


## Input
Below is an example of an input.
```
A + B => C
C => D

=AB

?D
```
The input are seperated into 3 sequential parts:

* rules

    Format: `condition => conclusion`.

    A condition contains one or more facts and operators on the list below (ordered by priority of execution).
    * NOT !
    * AND +
    * OR |
    * XOR ^

    A conclusion has one or more facts and can have only operator `AND +`

* initial facts

    Format: `=[facts]`

    All fact values are default to FALSE. The initial facts declaration will change the fact values to TRUE.

* query

    Format: `?[facts]`
    The program will do backward chaining on the facts and print the result.


