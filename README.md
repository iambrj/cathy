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

- Hexadecimal arithmetic
- Case structure
- Bitmanipulation
- Polymorphism
- Subtyping
- Universal types
- Exception handling system
- General recursion
- New datatypes - tuples, sums, variants, records

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
