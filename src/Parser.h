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
#pragma once
#include "Cell.h"
#include <memory>
#include <string>
#include <utility>
#include <vector>

class Parser {
public:
  static Cells parse(std::string program);

  static std::pair<std::string, std::string>
  parse_value_and_type(const std::string& program);

private:
  static Cell parse_cell(std::string program);

  static void normalize(std::string& s);

  static std::vector<std::string> tokenize(std::string program);

  static void trim(std::string& s);

  static bool is_literal(const std::string& s);

  static bool is_integer(const std::string& s);

  static bool is_bool(const std::string& s);

  static bool is_string(const std::string& s);

  static bool is_keyword(const std::string& k);

  static bool is_existing_symbol(const std::string& k);

  static void remove_empty_parts(std::vector<std::string>& parts);
};