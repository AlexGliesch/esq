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

#include "Exceptions.h"

struct InterpreterException : public GenericException {
	InterpreterException(const std::string& s) : GenericException(s) {}

	static InterpreterException undefined() {
		return InterpreterException("Reached a run-time error. Sorry!");
	}
};

struct ContextException : public InterpreterException {
	ContextException(const std::string& s)
		: InterpreterException("Error: undefined symbol " + s + ".") {
	}
};

struct ArityException : public InterpreterException {
	ArityException(const std::string& function_name, int expected_arity,
				   int given_arity)
		: InterpreterException("Error: " + function_name + " expects "
			+ std::to_string(expected_arity) + " arguments, given "
			+ std::to_string(given_arity) + ".") {
	}
};

struct TypeException : public InterpreterException {
	TypeException(const std::string& function_name,
				  const std::string& expected_type,
				  const std::string& given_type)
		: InterpreterException("Error: " + function_name + " expects "
			"argument of type " + expected_type + ", given "
			+ given_type + ".") {
	}
};