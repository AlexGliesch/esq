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
#include "Cell.h"
#include "Parser.h"
#include <boost/regex.hpp>
#include <cassert>
#include <ciso646>
#include <iostream>

using namespace std;

Cell::Cell() { type_ = Type::Empty; }

Cell::Cell(const Cell& cell)
    : value_(cell.value_), args_(cell.args_), procedure_(cell.procedure_),
      literal_type_(cell.literal_type_), context_(cell.context_), type_(cell.type_) {}

Cell::Cell(BuiltinProcedure proc, const string& value, const string& literal_type)
    : value_(value), type_(BuiltInProcedure), literal_type_(literal_type),
      procedure_(proc) {}

Cell::Cell(Type type, const string& value, const std::string& literal_type)
    : type_(type), value_(value), literal_type_(literal_type) {}

const Cell& Cell::true_cell() {
  static Cell c(Literal, "true");
  c.set_literal_type("bool");
  return c;
}

const Cell& Cell::false_cell() {
  static Cell c(Literal, "false");
  c.set_literal_type("bool");
  return c;
}

const Cell& Cell::empty_cell() {
  static Cell c(Empty, "empty");
  c.set_literal_type("[]");
  return c;
}

string Cell::to_string() const {
  if (is_value()) {
    return value_;
  } else {
    string str("(");
    for (const auto& arg : args_) {
      str += arg.to_string() + " ";
    }
    str.back() = ')';
    return str;
  }
}

void Cell::pop_first_arg() {
  if (arity() >= 1) args_.erase(args_.begin());
}

std::string Cell::return_type() const {
  boost::smatch sm;
  static const boost::regex parse_type(".*->(.*)");
  if (boost::regex_match(literal_type_, sm, parse_type)) {
    return sm[1].str();
  } else {
    return literal_type_;
  }
}

bool Cell::is_literal_list() const {
  return literal_type_.size() >= 2 and literal_type_.front() == '[' and
         literal_type_.back() == ']';
}
