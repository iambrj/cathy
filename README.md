# Cathy

An interpreter for a simple programming langauge.

## Features currently implemented

- Integer arithmetic
- Real arithmetic
- Operator precedence
- Associativity of operators
- Parenthesization of expressions

## Features to be added

- Bit manipulation
- Hexadecimal arithmetic
- User defined functions
- User defined variables

## Installation

To install cathy, first install flex and bison. Most Linux distributions come with these installed. If yours doesn't, use the package manager to install them. 

On Arch Linux,

```
    sudo pacman -S flex bison
```

Next clone (or download as a .zip file using button above) the repository.

```
    git clone https://github.com/iambrj/deskCal
    cd cathy
```

Install and run cathy

```
    make
    ./cathy
```
