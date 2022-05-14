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
#include "CommandLine.h"
#include "Cell.h"
#include "Parser.h"
#include "Interpreter.h"
#include "Exceptions.h"
#include "ParseExceptions.h"

#include <vector>
#include <iostream>
#include <stdexcept>
#include <cassert>
#include <ciso646>
#include <fstream>

#include <boost/regex.hpp>

using namespace std;

CommandLine::CommandLine() {
	quit_ = false;
	context_ = Context::global_context();
}

void CommandLine::respond(const std::string& prompt) {	
	//load_file("standard_library.esq");

	while (not quit_) {
		cout << prompt;

		string s;
		getline(cin, s);			

		if (parse_additional_options(s)) 
			continue;

		try {
			auto v = Parser::parse(s);
			
			if (v.size() > 1) {
				throw ParseException("Parse error: ill-formed sentece.");
			}

			if (v.empty()) continue;
		
			auto result = Interpreter::interpret(v[0], context_);
			cout << result.to_string() << ": " << result.literal_type() << endl;

		} catch (GenericException& e) {
			cout << e.what() << endl;
		}
		cout << endl;
	}

	cout << "bye!" << endl;
}

bool CommandLine::parse_additional_options(const string& s) {
	boost::smatch sm;
	static boost::regex load_regex("load *([^ ]+) *.*", boost::regex::icase);
	if (boost::regex_match(s, sm, load_regex)) {
		load_file(sm[1].str());
		return true;;
	}

	if (s == "reset") {
		reset();
		return true;
	}

	if (s == "quit") {
		quit_ = true;
		return true;
	}

	return false;
}

void CommandLine::load_file(const string& filename) {
	ifstream f(filename);
	if (f.bad() or f.fail()) {
		cout << "Could not load library file " << filename 
			<< ": file doesn't exist." << endl;
		return;
	}		

	string program, buffer;
	while (getline(f, buffer)) {
		if (not parse_additional_options(buffer)) {
			program += buffer;
		}
	}
	try {
		auto cells = Parser::parse(program);
		for (auto& c : cells) {
			Interpreter::interpret(c, context_);
		}
	} catch (std::exception& e) {		
		cout << e.what() << endl;
		reset();
	}	
}

void CommandLine::reset() {
	context_.reset();
	context_ = make_shared<Context>();
}
