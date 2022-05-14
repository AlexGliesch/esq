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
#include "Interpreter.h"
#include "Context.h"
#include "InterpreterExceptions.h"
#include "Parser.h"
#include "Validator.h"

#include <cassert>
#include <iostream>
#include <ciso646>

#include <boost/algorithm/string/predicate.hpp>

using namespace std;

Cell Interpreter::interpret(const Cell& c, shared_ptr<Context> ctx) {
	if (c.is_value()) {					
		return (c.type() == Cell::Symbol) ? ctx->get(c.value()) : c;		
	}

	if (c.arity() == 0) {
		return Cell::empty_cell();
	}

	// cout << c.to_string() << endl;

	if (c.arg(0).type() == Cell::Symbol) {
		const auto& first_argument = c.arg(0).value();
		auto r = c;

		if (boost::starts_with(first_argument, "lambda")) {
			return interpret_lambda(r, ctx);
		} else if (first_argument == "define") {	
			return interpret_define(r, ctx);
		} else if (first_argument == "if") {
			return interpret_if(r, ctx);
		} else if (first_argument == "local") {
			return interpret_local(r, ctx);
		} else if (first_argument == "begin") {
			return interpret_begin(r, ctx);
		} 
	} 	

	/* it's a function call */	
	auto r = interpret(c.arg(0), ctx);
	Cells args;
	for (int i = 1; i < c.arity(); ++i) {
		args.push_back(interpret(c.arg(i), ctx));
	}

	if (r.type() == Cell::Lambda) {

		auto new_ctx = make_shared<Context>((r.context() != nullptr) ? 
			r.context() : ctx);
		
		Validator::assert_arity(c.arg(0).value(), r.arg(1).arity(), args);

		for (int i = 0; i < r.arg(1).arity(); ++i) {			
			Validator::assert_type(r.arg(0).value(),
				r.arg(1).arg(i).literal_type(), args[i]);
			new_ctx->set(r.arg(1).arg(i).value(), args[i]);
		}

		return interpret(r.arg(2), new_ctx);

	} else /* If r.type() == Cell::Procedure */ { 	
		if (r.procedure() == nullptr) {
			throw InterpreterException("Undefined procedure: " 
				+ r.value() + ".");
		}
		
		return r.procedure()(args);		
	}	

	throw InterpreterException::undefined(); 
}

Cell Interpreter::interpret_if(Cell& c, shared_ptr<Context> ctx) {
	Validator::assert_arity("if", 3, c.args().size() - 1);
	auto test = interpret(c.arg(1), ctx);
	Validator::assert_type("if's test", "bool", test);
	if (test.value() == "true") {
		return interpret(c.arg(2), ctx);
	}  else {
		return interpret(c.arg(3), ctx);
	}
}

Cell Interpreter::interpret_lambda(Cell& c, shared_ptr<Context> ctx) {	
	Validator::assert_arity("lambda", 2, c.args().size() - 1);

	c.set_type(Cell::Lambda);	
	auto lambda_return_type = Parser::parse_value_and_type(c.arg(0).value());
	if (not (boost::starts_with(lambda_return_type.first, "lambda")))
		throw InterpreterException::undefined();
	auto& return_type = lambda_return_type.second;

	for (auto& a : c.arg(1).args()) {
		auto value_type = Parser::parse_value_and_type(a.value());
		a.set_value(value_type.first);
		a.set_literal_type(value_type.second);
	}
	
	string literal_type;
	if (c.arg(1).arity() == 0) {
		literal_type = "nothing";
	} else {
		literal_type = c.arg(1).arg(0).literal_type();
		for (int i = 1; i < c.arg(1).arity(); ++i) {
			literal_type += "," + c.arg(1).arg(i).literal_type();
		}
	}
	literal_type += "->" + return_type;
	c.set_literal_type(literal_type);
	c.set_value("lambda:" + c.literal_type());	
	c.arg(0).set_value("lambda:" + c.literal_type());
	c.arg(0).set_literal_type(c.literal_type());

	return c;
}

Cell Interpreter::interpret_define(Cell& c, shared_ptr<Context> ctx) {
	Validator::assert_arity("define", 2, c.args().size() - 1);
	ctx->set(c.arg(1).value(), interpret(c.arg(2), ctx));
	return ctx->get(c.arg(1).value());
}

Cell Interpreter::interpret_local(Cell& c, shared_ptr<Context> ctx) {
	Validator::assert_arity("local", 2, c.args().size() - 1);
	auto new_ctx = make_shared<Context>(ctx);
	interpret_command_list(c.arg(1), new_ctx);
	c.arg(2) = interpret(c.arg(2), new_ctx);
	c.arg(2).set_context(new_ctx);
	return c.arg(2);
}

Cell Interpreter::interpret_begin(Cell& c, std::shared_ptr<Context> ctx) {
	c.pop_first_arg();
	return interpret_command_list(c, ctx);
}

Cell Interpreter::interpret_command_list(const Cell& c,
										 shared_ptr<Context> ctx) {
	for (int i = 0; i < c.arity() - 1; ++i) {
		interpret(c.arg(i), ctx);
	}
	return interpret(c.arg(c.arity() - 1), ctx);	
}
