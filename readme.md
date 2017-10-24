# Cubium    

[![Build Status](https://travis-ci.org/Cubium/Cubium.svg?branch=master)](https://travis-ci.org/Cubium/Cubium)

Cubium is a free and open-source flight software for Linux-based spacecraft systems. Cubium allows for a more standardized and streamlined method of handling systems with many connected components by providing the neccesarry network to allow automatic discovery and communication between components. Developed with undergraduate CubeSat teams using systems such as Beaglebone Blacks and Raspberry Pis in mind, Cubium's purpose is to lower the bar of entry for satellite development.


Cubium is designed using the Space Plug-and-play Architecture (SPA), a specification for a kind of modular satellite software architecture. It has a proven mission-success track record on Air Force and Space Dynamics Laboratory payloads.

For a fun introduction on the inner workings of Cubium, see [this illustrated writeup.](https://drive.google.com/file/d/0ByiGNyJUAlpISUo5WDFwSkh3YU0/view?usp=sharing)

For a very detailed look into the machinations of SPA in general, see [Jacob Holt Christensen's dissertation.](http://digitalcommons.usu.edu/etd/1422/)

## Project Status 
  * **Version Alpha 2.0.0**
    * Added interfacing with Python components
    * Major bugfixes
    * Architecture ready for flight testing
  * **Version Alpha 1.2.0**
    * Support for sending strings between components
  * **Version Alpha 1.1.0**
    * Finalized architecture for software demo shown at SmallSat conference
  * **Version Alpha 1.0.0**
    * All necessary framework completed for support of basic component systems.
  * **Version Alpha 0.0.6**
    * Successful transmission of SPA messages across processes via socket communication
  * **Version Alpha 0.0.5**
    * Major backend refactoring of SPA Messages
  * **Version Alpha 0.0.4**
    * Added a basic subscription service
      * Direct component-to-component subscription
      * Non-prioritized publishing
    * Improvements to message handling
    * Additional tests for components
  * **Version Alpha 0.0.3**
    * Basic implementations of:
      * Local SpaMessages
      * Components
      * Local Subnet Manager
  * **Version Alpha 0.0.2**
    * Debian dev environment complete
  * **Version Alpha 0.0.1**
    * Planning API's and project planning

## Getting Started
### Developer Tools
Cubium relies on a handful of developer tools. The following is a list of things we'll be using:
* Vagrant - Virtual development environment
* Git - Version control system
* Google Test - Unit testing framework
* Doxygen - Documentation generator
* CMake - Build system automation

<!-- ## How to Contribute  -->
<!-- TODO -->
### Set up Vagrant
Cubium uses Vagrant to create a development environment to match the devices that Cubium will run on. It also eliminiates "well, it works on my system" bugs. 

**For instructions on getting the dev environment up and running, see the [wiki page](https://github.com/Cubium/Cubium/wiki/Setting-up-the-Cubium-Dev-Environment)**
  
### Build Project
#### TL;DR
  * Run CMake in project directory `cmake .`
  * Run generated makefile  `make [optional-target]`

Cubium uses CMake for a build system. Makefiles are generally platform-dependent, so CMake generates a different Makefile for each system in order to allow for cross-plaform functionality.

### Build Docs
#### TL;DR
  * Run Doxygen with project doxyfile `doxygen ./Doxyfile`
  * View your docs. They should now live in `docs/`

Cubium uses the documentation generator Doxygen to build documentation. Annotated source code is parsed by Doxygen to generate LaTeX and HTML files.

Doxygen is configured with a file titled `Doxyfile`.

  * Build Documentation
    * Invoke commandline tool
      * `doxygen Doxyfile`

This will read all configuration options from the Doxyfile, find and parse the source code, and generate the documentation.

If the documentation is successfully built, there should be a new directory title `docs/` that should contain both HTML and LaTeX documentation.

* Read Docs
  * Open up `docs/html/index.html` in your web browser to browse docs

### Running Tests
Cubium tests use the Google Test testing framework for unit testing. Test test test.
  * To run test suite:
    * Generate a makefile with CMake `cmake .`
    * Build tests with makefile `make runTests`
    * Run test executable `./runTests`

### Testing
Cubium uses Google Test for unit testing and CMake for a build system. The short version of running tests is this:

Classes should be kept small and have functioning unit tests. When adding a new header file for a class, a header file of the same name should be added to the `test/` directory.

To add a new class to the project:
  * Create header file `my_class_name.hpp` (File names should be snake case - lowercase words seperated with underscores)
    * Define class
    ```cpp
    #ifndef MY_CLASS_NAME_HPP
    #define MY_CLASS_NAME_HPP
      class MyClassName{};
    #endif
    ```
      * Must have include guards
      * Class name should be UpperCamelCase, where each first letter of a words is capitalized. Including the first word.  
  * Add new testing file `test/my_class_name.hpp`
  * Write tests for your class
    ```cpp
    #include "../path/to/my_class_name.hpp"

    TEST(MyClassName, myMethod){
      MyClassName myClass;
      EXPECT_EQ(myClass.myMethod(),0);
    }
    ```
      * Be sure to include class header in test file
  * Include your test header in main test file
    * Open `test/gtest_main.cpp`
    * Include your new test header file
  * Hooray! Now you can run your tests! :D


### Documentation
Cubium uses Doxygen to build documentation from source code. This means that one can add comments with a special format in the code so that Doxygen may build pretty HTML docs that can be referenced by all other developers and users.

Here is an example of what this might look like to document a function.
```cpp
  //! I am the brief message, I give a short overview of what a method does. I need to be followed by a whitespace

  //! \param myParam - I describe myParameter
  //! \return - I describe a return value
  bool example(int myParam){return true;}
```
