/*
 * MIT License
 *
 * Copyright (c) 2013 Alex Gliesch
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "BuiltIns.h"
#include "InterpreterExceptions.h"
#include "Validator.h"
#include "bigint/BigIntegerLibrary.h"
#include <algorithm>
#include <cassert>
#include <ciso646>
#include <iostream>
#include <string>

using namespace std;

Cells BuiltIns::builtin_cells_;

const Cells& BuiltIns::get() {
  static bool initialized = false;
  if (not initialized) {
    initialize();
    initialized = true;
  }
  return builtin_cells_;
}

void BuiltIns::initialize() {
  builtin_cells_.emplace_back(BuiltIns::sum, "+", "int,int->int");
  builtin_cells_.emplace_back(BuiltIns::difference, "-", "int,int->int");
  builtin_cells_.emplace_back(BuiltIns::multiplication, "*", "int,int->int");
  builtin_cells_.emplace_back(BuiltIns::less_than, "<", "int,int->bool");
  builtin_cells_.emplace_back(BuiltIns::logic_not, "not", "bool->bool");
  builtin_cells_.emplace_back(BuiltIns::logic_and, "and", "bool,bool->bool");
  builtin_cells_.emplace_back(BuiltIns::empty_test, "empty?", "x->bool");
  builtin_cells_.emplace_back(BuiltIns::first, "first", "[x]->x");
  builtin_cells_.emplace_back(BuiltIns::rest, "rest", "[x]->[x]");
  builtin_cells_.emplace_back(BuiltIns::cons, "cons", "x,[x]->[x]");
  builtin_cells_.emplace_back(BuiltIns::list, "list", "x,x,...,x->[x]");
  builtin_cells_.emplace_back(Cell::empty_cell());
  builtin_cells_.emplace_back(Cell::true_cell());
  builtin_cells_.emplace_back(Cell::false_cell());
}

Cell BuiltIns::sum(const Cells& args) {
  Validator::assert_arity("+", 2, args);
  Validator::assert_type("+", "int", args[0]);
  Validator::assert_type("+", "int", args[1]);

  auto first = stringToBigInteger(args[0].value());
  auto second = stringToBigInteger(args[1].value());

  first += second;

  return Cell(Cell::Literal, bigIntegerToString(first), "int");
}

Cell BuiltIns::difference(const Cells& args) {
  Validator::assert_arity("-", 2, args);
  Validator::assert_type("-", "int", args[0]);
  Validator::assert_type("-", "int", args[1]);

  auto first = stringToBigInteger(args[0].value());
  auto second = stringToBigInteger(args[1].value());

  first -= second;

  return Cell(Cell::Literal, bigIntegerToString(first), "int");
}

Cell BuiltIns::multiplication(const Cells& args) {
  Validator::assert_arity("*", 2, args);
  Validator::assert_type("*", "int", args[0]);
  Validator::assert_type("*", "int", args[1]);

  auto first = stringToBigInteger(args[0].value());
  auto second = stringToBigInteger(args[1].value());

  first *= second;

  return Cell(Cell::Literal, bigIntegerToString(first), "int");
}

Cell BuiltIns::less_than(const Cells& args) {

  Validator::assert_arity("<", 2, args);
  Validator::assert_type("<", "int", args[0]);
  Validator::assert_type("<", "int", args[1]);

  auto first = stringToBigInteger(args[0].value());
  auto second = stringToBigInteger(args[1].value());

  return (first < second) ? Cell::true_cell() : Cell::false_cell();
}

Cell BuiltIns::logic_not(const Cells& args) {

  Validator::assert_arity("not", 1, args);
  Validator::assert_type("not", "bool", args[0]);

  return (args[0].value() == "true") ? Cell::false_cell() : Cell::true_cell();
}

Cell BuiltIns::logic_and(const Cells& args) {

  Validator::assert_arity("and", 2, args);
  Validator::assert_type("and", "bool", args[0]);
  Validator::assert_type("and", "bool", args[1]);

  return (args[0].value() == "true" and args[1].value() == "true") ? Cell::true_cell()
                                                                   : Cell::false_cell();
}

Cell BuiltIns::empty_test(const Cells& args) {

  Validator::assert_arity("empty?", 1, args);

  return args[0].is_empty() ? Cell::true_cell() : Cell::false_cell();
}

Cell BuiltIns::first(const Cells& args) {

  Validator::assert_arity("first", 1, args);

  if (args[0].is_empty()) return Cell::empty_cell();

  if (args[0].type() != Cell::List)
    throw InterpreterException("Error: argument to first must be a list.");

  return args[0].arg(0);
}

Cell BuiltIns::rest(const Cells& args) {

  Validator::assert_arity("rest", 1, args);

  if (args[0].arity() < 2) return Cell::empty_cell();

  Cell c(args[0]);
  c.pop_first_arg();

  return c;
}

Cell BuiltIns::cons(const Cells& args) {
  Validator::assert_arity("cons", 2, args);

  Cell c(Cell::List);
  c.add_arg(args[0]);

  if (not args[1].is_empty()) {

    Validator::assert_list_type(args[0], args[1].args());

    if (args[1].type() == Cell::List) {
      for (const auto& a : args[1].args())
        c.add_arg(a);
    }
  }

  c.set_literal_type("[" + args[0].literal_type() + "]");
  return c;
}

Cell BuiltIns::list(const Cells& args) {

  if (args.size() == 0) return Cell::empty_cell();

  Validator::assert_list_type(args[0], args);

  Cell c(Cell::List);
  c.set_args(args);
  c.set_literal_type("[" + args[0].literal_type() + "]");

  return c;
}
