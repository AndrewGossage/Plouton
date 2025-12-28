# Plouton

Plouton is a toy programming language in the style of a very minimal lisp dialect. Plouton uses a tree-walking interpreter. 


## Why I built Plouton ( in C )

Plouton gave me a chance to work in C without external dependencies, quite different from my usual web dev work. C's explicitness is refreshing - you're responsible for your own memory management and data structures. I considered arena allocation for AST nodes but the complex lifetimes  meant individual frees made more sense. I used function pointers for builtin operations and node evaluation rather than nested switch statements, which kept the evaluator cleaner.

## Syntax 

The syntax is very simple. Tokens are space-delimited, where tokens are: parens, numbers, identifiers (including $N arg references), strings (backtick-delimited), and operators/function names.
```
( # simple program )
( print `Hello, World!`  )

```
Everything is a function including comments and everything returns a number. 
Comments and print statements return 0.

Arguments are unnamed and zero-indexed

```
( fn add ( + $0 $1 ) )

( !add 2 3 ) 

```

Builtin functions/operators work as a reduce on all arguments passed to them. 

The contents of parens are refered to as scopes. Each scope should start with a function call. A scope that does not start with a function call will implicitly call '+', thus `( + 1 2 3 )` and `( 1 2 3 )` are equivalent. 


## Limitations

Plouton is missing a lot of basic functionality and likely has bugs. Plouton should not be used in production. If you do try Plouton feel free to let me know if you encounter any issues. All functions are global and cannot be passed as parameters.

## How to start 

Take a look at the examples and tests folder. The tests can be run with `make test`.



  

