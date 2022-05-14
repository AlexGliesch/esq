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
#include "Context.h"
#include "InterpreterExceptions.h"
#include "BuiltIns.h"

#include <iostream>

using namespace std;

Context::Context() {
	for (const auto& c : BuiltIns::get()) {		
		set(c.value(), c);
	}
}

const Cell& Context::get(const std::string& s) const {
	if (has_symbol(s)) 
		return map_.find(s)->second;
	if (outer_ != nullptr)
		return outer_->get(s);
	throw ContextException(s);
}

void Context::set(const std::string& s, Cell c) {	
	map_[s] = move(c);
}

std::shared_ptr<Context> Context::global_context() {
	static auto context = make_shared<Context>();
	return context;
}

bool Context::has_symbol(const std::string& s) const {
	return map_.find(s) != map_.end();
}
