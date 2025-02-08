# Reverse Polish Notation Calculator
This is a final project for Fundamentals of Computer Programming course at the [Silesian University of Technology](https://www.polsl.pl/en/) for the 1st semester 2024/2025 of Informatics.

---
### The goal of this project is to make a **working Postfix (RPN)/Infix notation calculator** that supports following functionalities:
- Addition/Subtraction (**a+b** **a-b**)
- Multiplication/Division (**a*b** **a/b** or **a\b**)
- Parentheses **( )**
- Exponential operators
  - Square roots **sqrt(a)**
  - Cubic roots **cbrt(a)**
  - Powers **a^b**
- Trigonometric functions
  - sine **sin(a)**
  - cosine **cos(a)**
  - tangent **tan(a)**

---
### The calculator does handle basic illegal operations such as:
- Division by zero
- Roots of negative numbers (complex numbers which were not required to implement)

---
## Usage
### Flags
- `-i` Input file - relative path to the input file (one-liner).
- `-o` Output file - relative path to where the output should be generated.
- `-c` Interactive mode - Prompts user for the equation in the CLI (Priority over `-i` flag)
- `-r` Output equation - If this flag is present the output will be shown as **Postix = result**, otherwise **Infix = result** regardless of equation on the input.

Order in which flags `-i` and `-o` are passed dictate the order in which their respective paths must be passed.
Examples:
`./RPN -io input.txt output.txt` and `./RPN -oi output.txt input.txt`

Equations passed on the input can be either Infix or Postfix **with the only requirement for it to have spaces between operators and operands!!!**
The reason for it is that I couldn't find in time a reliable solution to separating operators from negative numbers (a+-b) where b is negative (-b).

### Example usage:
For Infix:
- `./RPN -c` with input `sqrt(25) * (5 - 2)` will output: `sqrt(25)*(5 - 2) = 15`
- `./RPN -cr` with input `sqrt(25) * (5 - 2)` will output: `25 sqrt 5 2 - *  = 15`

For Postfix (RPN):
- `./RPN -c` with input `0 cos 2 / 2 ^` will output: `(cos(0)/ 2)^ 2 = 0.25`
- `./RPN -cr` with input `0 cos 2 / 2 ^` will output: `0 cos 2 / 2 ^ = 0.25`

---
## Under the hood
This section will explain steps required to solve equations.
1. Check for flags
   - If no flags = output help and exit.
   - If flags present, read them and assign their control values.
2. Determine whether input goes from Interactive mode or input file.
3. Get the input.
4. Add extra spaces between paretheses, since NotationConverter for RPN doesn't understand them in functions such as tan(), sqrt(), etc.
5. Determine in which notation is the equation written.
6. Validate if the equation is valid that i.e. has matching operators to operands and not more nor less.
7. Using RPNSolver solve the equation.
   - If equation is in Infix, convert to RPN and solve RPN version.
   - If equation is in Postfix, solve directly.
8. Having the result check if the `-r` flag is present and return correct equation representation along with the result. 
9. Output the result in the CLI
10. If `-o` flag is present, also write the same result as in the CLI into the file.

---
#### &copy; 2025 Xawier Słupik - Licensed under MIT License