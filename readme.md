# OpenSPA

OpenSPA is a free and open-source **Space Plug and Play Architecture (SPA)** services manager for Linux-based spacecraft systems. OpenSPA allows for a more standardized and streamlined method of handling systems with many connected components by providing the neccesarry network to allow automatic discovery and communication between components. Developed with undergraduate CubeSat teams using systems such as Beaglebone Blacks, Arduinos, and Raspberry Pis in mind, OpenSPA's purpose is to lower the bar of entry for satellite development.

SPA itself is a specification for a kind of modular satellite software architecture. It has a proven mission-success track record on Air Force and Space Dynamics Laboratory payloads.

For a fun introduction on the inner workings of OpenSPA, see [this illustrated writeup.](https://drive.google.com/file/d/0ByiGNyJUAlpISUo5WDFwSkh3YU0/view?usp=sharing)

For a very detailed look into the machinations of SPA in general, see [Jacob Holt Christensen's dissertation.](http://digitalcommons.usu.edu/etd/1422/)

## Project Status 
  * **Version Alpha 0.0.4a** 
    * Added interfaces for specific hardware components
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
OpenSPA relies on a handful of developer tools. The following is a list of things we'll be using:
* Vagrant - Virtual development environment
* Git - Version control system
* Google Test - Unit testing framework
* Doxygen - Documentation generator
* CMake - Build system automation

<!-- ## How to Contribute  -->
<!-- TODO -->
### Set up Vagrant
OpenSPA uses Vagrant to create a development environment to match the devices that OpenSPA will run on. It also eliminiates "well, it works on my system" bugs. 

**For instructions on getting the dev environment up and running, see the [wiki page](https://github.com/SmallSatGasTeam/OpenSPA/wiki/OpenSPA-Development-Environment) for instructions.**

#### Otherwise, TL;DR if you know what you're doing
* Be sure you've cloned in the repository and are in the /OpenSPA directory
* Install VirtualBox if you haven't already
* Install Vagrant
* Type `vagrant up` into your terminal
* Allow vagrant to finish setting everything up before attempting to login to the VM. 
  
### Build Project
#### TL;DR
  * Run CMake in project directory `cmake .`
  * Run generated makefile  `make [optional-target]`

OpenSPA uses CMake for a build system. Makefiles are generally platform-dependent, so CMake generates a different Makefile for each system in order to allow for cross-plaform functionality.

### Build Docs
#### TL;DR
  * Run Doxygen with project doxyfile `doxygen ./Doxyfile`
  * View your docs. They should now live in `docs/`

OpenSPA uses the documentation generator Doxygen to build documentation. Annotated source code is parsed by Doxygen to generate LaTeX and HTML files.

Doxygen is configured with a file titled `Doxyfile`.

  * Build Documentation
    * Invoke commandline tool
      * `doxygen Doxyfile`

This will read all configuration options from the Doxyfile, find and parse the source code, and generate the documentation.

If the documentation is successfully built, there should be a new directory title `docs/` that should contain both HTML and LaTeX documentation.

* Read Docs
  * Open up `docs/html/index.html` in your web browser to browse docs

### Running Tests
OpenSPA uses Google Test testing framework for unit testing. 
  * To run test suite:
    * Generate a makefile with CMake `cmake .`
    * Build tests with makefile `make runTests`
    * Run test executable `./runTests`

## Developer Notes
###  Abstraction Layer
OpenSPA is currently being developed for embedded Linux systems. In order to keep system-portability an option, we employ the use of what we call a "Platform Abstraction Layer." Essentially this means that whenever we need to rely on a system call or some system-specific functionality (e.g, a Linux socket), we build a wrapper around that system-specific functionality. This limits the platform dependance to a single file, which allows OpenSPA to be usable on a wide variety of platforms.

<!-- * Platform Abstractions live ... TODO document where these live -->
### Testing
OpenSPA uses Google Test for unit testing and CMake for a build system. The short version of running tests is this:

Classes should be kept small, and have functioning unit tests. When adding a new header file for a class, a header file of the same name should be added to the `test/` directory.
 It will also need to be included in th <!-- TODO finish this sentence lol -->

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
OpenSPA uses Doxygen to build documentation from source code. This means that one can add comments with a special format in the code so that Doxygen may build pretty HTML docs that can be referenced by all other developers and users.

Here is an example of what this might look like to document a function.
```cpp
  //! I am the brief message, I give a short overview of what a method does. I need to be followed by a whitespace

  //! \param myParam - I describe myParameter
  //! \return - I describe a return value
  bool example(int myParam){return true;}
```
