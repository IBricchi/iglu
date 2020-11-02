# iglu
---

## Overview

iglu is an interpreted object oriented programming language written in C++ from the ground up. I started this project by reading the book (https://craftinginterpreters.com/), I would highly recomend this for anyone who is interested in starting to build a programming language, starting of from 0 previous knowledge, and finishing with 2 fully functional languages.

The idea with iglu is to create a modular language, which can esaily be expanded through the adding of new types written in C++ with an iglu interface. All objects inherit from the Object class both in c++ aswell as in iglu.

## Current features

Currently iglu is still in it's infancy, and is under development.

It includes number, string, boolean, error, null, and c++ function data types out of the box. These are all children of the Object class, which at the moment has no way of bein instanciated directly.

The c++ function data types, which act as the bridge between c++ and iglu. Because of this calling a function, vs calling a c++ function, would have identical syntax, and c++ written functions could be overwritten by iglu functions would the programer desiger it.

Operators are set up in a way which calls specific functions to deal with data, instead of simply acting on the data, this makes most operators overloadable for different types. By default c++ functions have been created to support boolean, and numerical arithmetic operations, aswell as string concatenation with all types, using the built in \__to_string__ statement built into all objects.

The program uses a mark and sweep algorithm for garbage collection.

Support for . indexing for reading the value of object properties is supported.

## Road map

### The steps looking into the near future are:

Adding the ability to declare and define object properties using . indexing.

Allowing for calling of functions with the () operator, instead of only allowing operator based function calls.

Data flow controls like "if" "for" and "while" loops.

Custom functions defenitions.

Custom class types.

iglu file importing

### The steps in the later future are:

I want to create some form of package manager for the program.

I want to create a network package and build a backend framework for web development using iglu.
