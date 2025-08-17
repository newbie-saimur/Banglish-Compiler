# Error Test Cases for Banglish Compiler

## 1. Lexical Errors

### 1.1 Invalid Keywords
```banglish
dhori x = 10;          // Invalid keyword 'dhori' instead of 'dhor'
jodi x > 5 taile       // Invalid keyword 'taile' instead of 'tahole'
    dekhao("Hello");
ses

invalid_keyword = 5;   // Using non-Banglish keyword
```

### 1.2 Invalid Identifiers
```banglish
dhor 123abc = 10;      // Identifier starts with number
dhor my-var = 20;      // Hyphen in identifier
dhor @invalid = 30;    // Special character in identifier
dhor class = 40;       // Reserved C++ keyword
dhor int = 50;         // Reserved C++ keyword
```

### 1.3 Naming Convention Violations
```banglish
dhor MyVariable = 10;     // CamelCase instead of snake_case
dhor CONSTANT_VAR = 20;   // All caps (acceptable for constants)
dhor 2invalid = 30;       // Starts with number
dhor _private = 40;       // Starts with underscore
dhor variable$ = 50;      // Contains special character
```

### 1.4 Unclosed String Literals
```banglish
dhor message = "Hello World;    // Missing closing quote
dekhao("Incomplete string      // Missing closing quote and parenthesis
dhor another = "Valid string";
dhor broken = "Another unclosed string
               spanning multiple lines;  // Unclosed multi-line string
```

## 2. Syntax Errors

### 2.1 Missing Semicolons
```banglish
dhor x = 10          // Missing semicolon
dhor y = 20;
dekhao("Hello")      // Missing semicolon
```

### 2.2 Mismatched Brackets
```banglish
jodi (x > 5 {                    // Mismatched parentheses and braces
    dekhao("Greater than 5");
}

jodi x < 10) tahole {            // Extra closing parenthesis
    dekhao("Less than 10");
ses

lup (dhor i = 0; i < 5; i++ {    // Missing closing parenthesis
    dekhao(i);
ses
```

### 2.3 Invalid Control Structure Syntax
```banglish
jodi x > 5              // Missing 'tahole'
    dekhao("Greater");
ses

lup (x < 10) {          // Missing initialization in loop
    x++;
ses

jodi x == 5 tahole      // Missing opening brace
dekhao("Equal to 5");   // Statement without proper block
ses                     // Misplaced 'ses'
```

## 3. Semantic Errors

### 3.1 Undeclared Variables
```banglish
dhor x = 10;
y = x + 5;              // 'y' not declared
dekhao(z);              // 'z' not declared
```

### 3.2 Redeclaration Errors
```banglish
dhor x = 10;
dhor x = 20;            // Redeclaration of 'x'
dhor y = 30;
dhor y = 40;            // Redeclaration of 'y'
```

## 4. Type Errors

### 4.1 Type Mismatch
```banglish
dhor x = 10;
x = "string value";     // Assigning string to integer variable
dhor text = "hello";
text = 42;              // Assigning integer to string variable
```

## 5. Complex Error Scenarios

### 5.1 Multiple Errors in Single File
```banglish
dhor x = 10          // Missing semicolon
dhori y = "Hello"    // Invalid keyword 'dhori' and missing semicolon
dhor 123invalid = 5; // Invalid identifier
jodi x > y tahole {  // Type mismatch in comparison
    dekhao("Result");
    undeclared_var = 10;  // Undeclared variable
}
ses

lup (dhor i = 0; i < 10; i++ {  // Missing closing parenthesis
    dekhao(i
ses                             // Missing closing parenthesis and semicolon
```

### 5.2 Nested Structure Errors
```banglish
jodi x > 5 tahole {
    jodi y < 10 tahole {
        lup (dhor i = 0; i < 5; i++ {  // Missing closing parenthesis
            dekhao(undeclared);         // Undeclared variable
            jodi i == 2 tahole
                dekhao("Two");          // Missing braces
        ses
    ses
```

## 6. Edge Cases

### 6.1 Empty File
```banglish
// Empty file or only comments
```

### 6.2 Invalid Characters
```banglish
dhor x = 10;
dhor y = 20©;        // Invalid character '©'
dekhao("Hello™");    // Invalid character '™'
```
