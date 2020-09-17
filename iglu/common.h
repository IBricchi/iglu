#pragma once

struct AST;
struct Chunk;
class Compiler;
class Object;
class Parser;
class Scanner;
struct Token;
class VM;

// default objects
struct Constant;
class Error;
class Null;
class Number;
class Function;
class LinkedUnoFn;
class LinkedBinFn;
