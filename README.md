# Cathy

A simple programming langauge.

## Features currently implemented

- User defined functions (through let binding)
- User defined variables
- Control structures - while, if-then-else
- Lists
- Integer arithmetic
- Real arithmetic
- Operator precedence
- Associativity of operators
- Parenthesization of expressions

## Ideas for future

- Polymorphism
- Subtyping
- Universal types
- Exception handling system
- General recursion
- New datatypes - tuples, sums, variants, records
- Syntactic sugar to implement for structure
- Hexadecimal arithmetic
- Case structure
- Bit manipulation

## Installation

To install cathy, first install gcc, flex and bison. Most Linux distributions come with these installed. If yours doesn't, use the package manager to install them. 

On Arch Linux,

```
sudo pacman -S gcc flex bison
```

Next clone (or download as a .zip file using button above) the repository.

```
git clone https://github.com/iambrj/cathy
cd cathy
```

Install and run cathy

```
make
make clean # to remove temporary files generated
./cathy
```

## Syntax

Identifiers for varible begin with an alphabet (lowercase or uppercase). Values for variables can be assigned using `=` operator.

```
> a = 3
 =    3
> b = 4
 =    4
> a + b
 =    7
```

`if-then-else` block works as follows

```
> if a > b then a; else b;
 =    4
```
Note the use of `;` to end `then` and `else`

Functions can be defined using `let`. Example
```
> let square(a) = a * a;
Defined square
```
Note that function declarations must be terminated by a `;`
