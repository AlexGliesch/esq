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
#include "Parser.h"
#include "Context.h"
#include "IteratorRange.h"
#include "ParseExceptions.h"
#include "bigint/BigIntegerLibrary.h"
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/regex.hpp>
#include <ciso646>
#include <iostream>

using namespace std;

Cells Parser::parse(string program) {
  normalize(program);
  vector<string> parts = tokenize(program);
  Cells cells;
  for (auto& s : parts) {
    cells.emplace_back(parse_cell(move(s)));
  }
  return cells;
}

Cell Parser::parse_cell(string program) {
  Cell c;

  if (not((program.front() != '(' and program.back() != ')') or
          (program.front() == '(' and program.back() == ')'))) {
    throw ParenthesesException(program);
  }

  if (program.front() != '(' or program.back() != ')') {
    c.set_value(program);

    c.set_type(is_literal(program) ? Cell::Literal : Cell::Symbol);

    if (c.is_literal()) {
      if (is_bool(program)) {
        c.set_literal_type("bool");
      } else if (is_string(program)) {
        c.set_literal_type("string");
      } else if (is_integer(program)) {
        c.set_literal_type("int");
      }
    }

  } else {
    /* Program is a list */
    boost::erase_last(program, string(")"));
    boost::erase_first(program, string("("));
    auto parts = tokenize(program);

    Cells arguments;
    for (const auto& part : parts) {
      arguments.emplace_back(parse_cell(part));
    }

    c.set_args(arguments);
    c.set_type(Cell::List);
  }

  return (c);
}

void Parser::normalize(string& s) {
  boost::replace_all_regex(s, boost::regex("[\n\t]"), string(" "));
  trim(s);
  boost::replace_all_regex(s, boost::regex(" +"), string(" "));
  boost::replace_all_regex(s, boost::regex("\\( +"), string("("));
  boost::replace_all_regex(s, boost::regex(" +\\)"), string(")"));
  boost::replace_all_regex(s, boost::regex("\\)\\("), string(") ("));
}

void Parser::trim(string& s) {
  boost::erase_all_regex(s, boost::regex("^ +"));
  boost::erase_all_regex(s, boost::regex(" +$"));
}

vector<string> Parser::tokenize(string program) {
  int start = 0;
  int paren = 0;
  vector<string> parts;

  for (auto i : range(0, program.size())) {
    if (program[i] == '(') {
      ++paren;
    } else if (program[i] == ')') {
      --paren;
    }
    if (paren == 0 and ((program[i] == ' ') or (i == (int)program.size() - 1))) {
      parts.emplace_back(program, start, i - start + 1);
      start = i + 1;
    }
  }

  if (paren != 0) {
    throw ParenthesesException(program);
  }

  for (auto& s : parts)
    trim(s);

  remove_empty_parts(parts);

  return (parts);
}

bool Parser::is_literal(const string& s) {
  return is_integer(s) or is_bool(s) or is_string(s);
}

bool Parser::is_integer(const string& s) {
  try {
    auto x = stringToBigInteger(s);
  } catch (...) {
    return false;
  }

  return s != "+" and s != "-";
}

bool Parser::is_bool(const string& s) {
  auto x = boost::to_lower_copy(s);
  return x == "true" or x == "false";
}

bool Parser::is_string(const string& s) {
  return ((int)s.size() >= 2 and s.front() == '\'' and s.back() == s.front());
}

bool Parser::is_keyword(const string& k) {
  return (k == "lambda") or (k == "define") or (k == "if") or (k == "->") or (k == ",");
}

bool Parser::is_existing_symbol(const string& k) {
  return Context::global_context()->has_symbol(k);
}

void Parser::remove_empty_parts(vector<string>& parts) {
  parts.erase(remove_if(begin(parts), end(parts),
                        [](const string& s) -> bool { return s.empty(); }),
              end(parts));
}

pair<string, string> Parser::parse_value_and_type(const string& program) {
  vector<string> parts;
  boost::split_regex(parts, program, boost::regex("[ :]"));
  remove_empty_parts(parts);
  if (parts.size() != 2) {
    throw ParseException("Error: expected expression of kind "
                         "identifier:type, given " +
                         program + ".");
  }
  return make_pair(parts[0], parts[1]);
}
