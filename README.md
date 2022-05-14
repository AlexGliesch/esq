# A partial, statically-typed Scheme interpreter

This is an implementation of a subset of the [Scheme](https://www.scheme.com/tspl4/) language, but with a static type-checker. Created for the course INF05516-Formal Semantics at INF/UFRGS, in December 2013. 

The project was unanimously voted by 25 colleagues as the best project in the class.

## Implemented features

The language supports the following operations:
- <code>n</code>,
- <code>b</code>,
- <code>s</code>,
- <code>empty</code>,
- <code>(lambda:T (x<sub>1</sub>:T<sub>1</sub> x<sub>2</sub>:T<sub>2</sub> ... x<sub>n</sub>:T<sub>n</sub>) t<sub>r</sub>)</code>,
- <code>(if t<sub>1</sub> t<sub>2</sub> t<sub>2</sub>)</code>,
- <code>(define x t<sub>1</sub>)</code>,
- <code>(local (t<sub>1</sub> t<sub>2</sub> ... t<sub>n</sub>) t<sub>r</sub>)</code>,
- <code>(binop t<sub>1</sub> t<sub>2</sub>)</code>,
- <code>(unop t<sub>1</sub>)</code>,
- <code>(n-ary t<sub>1</sub> t<sub>2</sub> ... t<sub>n</sub>)</code>,

where 
- `n` is an integer. The language supports big integers using Matt McCutchen's [BigInt library](https://mattmccutchen.net/bigint/),
- `b` is either `true` or `false`,
- `s` is any string,
- `binop` is either `+,-,*,<,and,cons`,
- `unop` is either `not,empty?,first,rest`,
- `n-ary` is either `begin,list`,
- `x` is any identifier defined by the user.

The interpreter also has the following special commands:
- `reset`: cleans the global context,
- `load {fileName}`: loads the given file and executes every command in it,
- `quit`: quits the interpreter.

For details about language semantics, see `report.pdf`, and for more about the Scheme language, see https://www.scheme.com/tspl4/.

## Running the code

1. Compile under `src` with `make`. Requires [Boost](boost.org).
1. Run the interpreter using `./esq`.
1. We also provide a compiled binary for MS Windows in `bin/esq.exe`.

## Examples and standard library

For examples on how to use the language, see the files under `examples`. 

These can also be seen as a standard library for the language, and could be imported using `load`. 

- `binop.esq` implements standard relational and logical operators, which can be defined from `<`, `not` and `and`.
- `list.esq` implements high order functions like `map`, `fold` and `filter`.
- `quicksort.esq` and `mergesort.esq` are non-optimized implementations of quick- and merge-sort. 
- `math.esq` implements some math functions like factorial, power and Fibonacci.




