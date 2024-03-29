Modern C++ as binary data compiler
==================================


Description
-----------

Embedded systems often need some external configuration data which should not 
reside inside a firmware image, so both the firmware and the configuration may 
be changed independently. Examples of such a configuration data are Windows 
registry or Linux device tree. While text files are good enough in most cases, 
parser implementation may be too complex compared to one for binary formats. 

Many systems reinvent the wheel by creating their own configuration data 
descriptions, compilers and parsers. This project is an yet another such 
attempt to create a simple and extensible configuration ROM-able FS-like 
storage in pure C++. The goal is to write configuration in human-readable 
subset of C++. Before using the data must be converted into efficient binary 
format. No tools needed except the C++ compiler and standard binutils.

C++ compiler is used to create binary representation of data. Data section is 
then extracted to be used as binary configuration image. Parser is used on a 
target side to extract parameters by hierarchial FS-like names. It is 
interesting that the program itself does not contain executable code, all the 
code including class constructors are interpreted at compile time to create 
static data.

All the data are organized as arrays of Nodes or key-value pairs where key is 
always a string. There are 4 generic data types: numbers, strings, folders and 
blobs. Any of these may be associated with a key. Blobs may be also used for 
any user-defined types, they are just arrays of any data. Example:

    constexpr Node foo[] = 
    {
        {“my_number”, 42},
        {“my_string”, “cool string”}
    };

foo is a folder containing two nodes, first is a number and second is a string. 
Now add some hierarchy and a blob:

    constexpr unsigned int bin_data[] = {1,2,3,4,5};

    constexpr Node bar[] = 
    {
        {“my_blob”, bin_data},
        {“child_folder”, foo}
    };

Now you may read 42 by name “/child_folder/my_number”. It is not designed to 
hold large hierarchies of billions of entries (like registry). Keys are 
compared one by one with linear search, no tree, no hash and so on. But it is 
good and simple solution for not so complex embedded systems with reasonable 
number of entries. Remember that under the hood it is just arrays, so don’t 
expect any magic.

Why use it? If you use C++ you need no extra tools to build both your code and 
its configuration. No need to learn new syntax. It is easy to add custom data 
types, much easier than make modifications to binary JSON or something like 
this. Note that while it is somewhat unusual use of the langauge it is still 
C++, so all of the language features are ready to use.


File descriptions
-----------------

- node.h: common header for C and C++ part describing data items
- node.hpp: implementation of consteval constructors for structs in node.h
- parser.c: binary data parser implemented in pure C for FW integration
- gnu.ld: Linked script for GCC to extract data sections
- build.sh: Script for building binary data image
- cppdata.h: API for firmware side code
- example_data.cpp: Sample binary data
- example_parser.c: Example how to use parser API



                                 ----------
               ------------------| node.h |-------------------
               |                 ----------                  |
               V                                             V
        ---------------                             ------------------
        |  node.hpp   |                             |   parser.c     |
        ---------------                             ------------------
               |                                             |
               V                                             V
        ---------------     FS-like data access     ------------------
        | binary data | <-------------------------> | firmware image |
        ---------------                             ------------------


Build & test
------------

To build example binary data file use:

    make

It yields cppdata.bin file.

To build example parser use:

    gcc -o test example_parser.c parser.c

It results in test executable.
Run the following command to print directory structure:

    ./test cppdata.bin
