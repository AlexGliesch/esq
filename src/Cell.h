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

#include <string>
#include <vector>
#include <memory>
#include <functional>

class Context;

class Cell {	
public:
	enum Type {
		Lambda,
		BuiltInProcedure,
		Literal,
		Symbol,
		List,
		Empty
	};

 	Cell();

	Cell(Type type, const std::string& value = "", 
		const std::string& literal_type = "");

	Cell(std::function<Cell(const std::vector<Cell>&)> proc, 
		const std::string& value = "", const std::string& literal_type = "");

 	Cell(const Cell& cell);

	static const Cell& true_cell();

	static const Cell& false_cell();

	static const Cell& empty_cell();

	std::string to_string() const;

	bool is_value() const { return arity() == 0; }

	int arity() const { return args_.size(); }

	bool is_empty() const { return type() == Empty; }

	bool is_literal_list() const;

	bool is_literal() const { return type() == Literal;  }

	void pop_first_arg();

	const Cell& arg(int i) const { return args_[i]; }

	Cell& arg(int i) { return args_[i]; }

	const std::vector<Cell>& args() const { return args_; }

	std::vector<Cell>& args() { return args_; }

	void set_args(const std::vector<Cell>& args) { args_ = args; }

	void set_args(std::vector<Cell>&& args) { args_ = std::move(args); }

	void add_arg(const Cell& c) { args_.push_back(c); }

	void add_arg(Cell&& c) { args_.emplace_back(std::move(c)); }

	const std::string& value() const { return value_; }

	void set_value(const std::string& value) { value_ = value; }

	Type type() const { return type_; }

	void set_type(Type type) { type_ = type; }

	const std::string& literal_type() const { return literal_type_; }

	std::string return_type() const;

	void set_literal_type(const std::string& literal_type) {
		literal_type_ = literal_type;
	}

	std::function<Cell(const std::vector<Cell>&)> 
		procedure() const { return procedure_; }

	std::shared_ptr<Context> context() const { return context_; }

	void set_context(std::shared_ptr<Context> context) { context_ = context; }


private:
	std::string value_;

	std::vector<Cell> args_;	
	
	std::function<Cell(const std::vector<Cell>&)> procedure_;

	std::string literal_type_;	

	std::shared_ptr<Context> context_;


	Type type_;
};

typedef std::vector<Cell> Cells;
typedef std::function<Cell(const Cells&)> BuiltinProcedure;