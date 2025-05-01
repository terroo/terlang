# Terlang

![Ter/Terlang](./../logo-ter-terlang.png) 

Ter/Terlang is a programming language for scripting with syntax similar to C++.
The name comes from "**te**rminal **r**oot", the [youtube channel](https://www.youtube.com/channel/UCqd9oVuR4tuKD5Y9AFwxz7Q) of [Marcos Oliveira](https://github.com/terroo).

## Comments
As C++, they can be one line `//` or multi-line `/* .... */`.

## Statements
Statements can terminate with `;`, but it is optional.

## Variables and values types
Variables have no types, they can store any value. Variables can hold, change values and types during the execution.
Values have types:
* Null values have a special `nil` keyword for its type.
* Boolean types using `true` and `false` keywords for them.
* Numbers are C++ 'double' precision (64-bit). 
* Strings are similar to C++, including escape characters.
* Arrays with elements having different types.

### Variable declaration
Use the keyword `auto` to declare a variable:
* `auto varUndefined` will create a variable without value assigned.
* `auto varDefined = 5` to assign a value to the new created variable.

Notes:
Trying to use a variable that was not initialized will cause an error and program will be stopped.

### Numbers
Internally, as double precision, means integers can lost precision above 53-bit limit (> 9,007,199,254,740,991 or < -9,007,199,254,740,991).
Be aware if making calculations or using bit operators beyond this limit.
Infinity (Inf) and not a number (nan) are also numbers allowed.
Limits for minimal value is 2.22507e-308 and max 1.79769e+308. Negative numbers also supported.
Numbers can have decimal point followed by the fractional part. Numbers cannot end with a point.
It is also supported hexadecimal strings if they start with 0x or 0X. Hexadecimal numbers can also have fractional part.

### Strings
Strings can be defined between double quotes `"`.
It is possible to use C style escape sequences: `'\0' '\a' '\b' '\f' '\n' '\r' '\t' '\v' '\\' '\'' '\"' '\?'`.
Unicode text can be escaped with `\x{XXX}` or `\u{XXXXX}`, with the hex code of the respecting string inside the braces.
Unknow escape codes will be ignored.
Generated string is UTF-8 compliant.

### Arrays
It is possible to declare an array and the items can have any type:
* `auto arr = { 1, "texto", nil, -2.2 }` can create an array, and the items can be fetch using C representation `arr[0]` to get their items.

## Functions
Use the keyword `set` to define a new function.
The function can return a value using the keyword `return`.
If no return value is specified, the function will return null value `nil`.
Recursion is supported.

**Example**
```
set sum(a, b) {
    return a + b
}
```

## Built-in functions
Some functions are provided by the language:
* **to_string(value)**
  Function to return a string that represents the value passed as argument.
* **include(file)**
  Makes the script to start the execution of a given file. All variables and functions declared there will be available to the caller script.
* **out(value)**
  Prints the value to the standard output.
* **output(value)**
  Prints the value to the standard output, ending it with a newline.
* **rand(low, hi)**
  Returns an integer value between `lo` and `hi`, both included.
* **clock()**
  Returns current timestamp in seconds, including miliseconds.
* **getenv(value)**
  If the environment variable is found, its value is returned.
* **exec(command)**
  Executes the command in a shell.
* **args()**
  Returns all values passed as argument during the script call.
Arguments are parsed and returned as an array of strings.
If no arguments are given, empty array is returned.
The program name and script name aren't returned as arguments.

## Classes
Currently classes can have methods. Declare them using the `class` keyword.

**Example:**
```
class ClassName {
  methodA(name){
    /* ... */
  }
  methodB(){
    /* ... */
  }
}
```

## Branches and logics
Keyword `if` can test conditions and run/skip a block.
The statement is similar to C:
* `if (condition) { block } else { block }`

### Comparision keywords
Compare varibles and values using tokens like C++:

|Token|Meaning|
|---|---|
|`==`| true if equal|
|`!=`| true if not equal|
|`>`| true if greater|
|`>=`| true if greater or equal|
|`<`| true if lower|
|`<=`| true if lower or equal|

Strings can be compared and they are equal only if all chars are equal.

### Logic and/or/negation
Use multiple conditions with keywords `or` and `and`. Negate a condition with the token `!`.
* `if (a == b and !(c == d)) { /* ... */ }` executes the block if a is equal b and not if c is equal d.

## Loops
Repeat a block using keywords `for` or `while`.
`for` and `while` statement are similar to C++:
* `for (initialization; condition; statement) { block }`
* `while (condition) { block }`
