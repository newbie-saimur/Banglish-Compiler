# Error Test Cases for Banglish Compiler

## 1. Lexical Errors

### 1.1 Invalid Keywords
```banglish
dhori x = 10;          // Invalid keyword 'dhori' instead of 'purno sonkha'
jodi x > 5 taile       // Invalid keyword 'taile' - should just use 'jodi (x > 5) {'
    dekhao("Hello");
shesh

invalid_keyword = 5;   // Using non-Banglish keyword
```

### 1.2 Invalid Identifiers
```banglish
purno sonkha 123abc = 10;      // Identifier starts with number
purno sonkha my-var = 20;      // Hyphen in identifier
purno sonkha @invalid = 30;    // Special character in identifier
purno sonkha class = 40;       // Reserved C++ keyword
purno sonkha int = 50;         // Reserved C++ keyword
```

### 1.3 Naming Convention Violations
```banglish
purno sonkha MyVariable = 10;     // CamelCase instead of snake_case
purno sonkha CONSTANT_VAR = 20;   // All caps (acceptable for constants)
purno sonkha 2invalid = 30;       // Starts with number
purno sonkha _private = 40;       // Starts with underscore
purno sonkha variable$ = 50;      // Contains special character
```

### 1.4 Unclosed String Literals
```banglish
lekha message = "Hello World;    // Missing closing quote
dekhao("Incomplete string      // Missing closing quote and parenthesis
lekha another = "Valid string";
lekha broken = "Another unclosed string
               spanning multiple lines;  // Unclosed multi-line string
```

## 2. Syntax Errors

### 2.1 Missing Semicolons
```banglish
purno sonkha x = 10          // Missing semicolon
purno sonkha y = 20;
dekhao("Hello")      // Missing semicolon
```

### 2.2 Mismatched Brackets
```banglish
jodi (x > 5 {                    // Mismatched parentheses and braces
    dekhao("Greater than 5");
}

jodi x < 10) {            // Extra closing parenthesis
    dekhao("Less than 10");
}

loop (purno sonkha i = 0; i < 5; i++ {    // Missing closing parenthesis
    dekhao(i);
}
```

### 2.3 Invalid Control Structure Syntax
```banglish
jodi x > 5              // Missing parentheses around condition
    dekhao("Greater");
}

loop (x < 10) {          // Missing initialization in loop
    x++;
}

jodi (x == 5)      // Missing opening brace
dekhao("Equal to 5");   // Statement without proper block
}                     // Misplaced closing brace
```

## 3. Semantic Errors

### 3.1 Undeclared Variables
```banglish
purno sonkha x = 10;
y = x + 5;              // 'y' not declared
dekhao(z);              // 'z' not declared
```

### 3.2 Redeclaration Errors
```banglish
purno sonkha x = 10;
purno sonkha x = 20;            // Redeclaration of 'x'
purno sonkha y = 30;
purno sonkha y = 40;            // Redeclaration of 'y'
```

## 4. Type Errors

### 4.1 Type Mismatch
```banglish
purno sonkha x = 10;
x = "string value";     // Assigning string to integer variable
lekha text = "hello";
text = 42;              // Assigning integer to string variable
```

## 5. Complex Error Scenarios

### 5.1 Multiple Errors in Single File
```banglish
purno sonkha x = 10          // Missing semicolon
dhori y = "Hello"    // Invalid keyword 'dhori' and missing semicolon
purno sonkha 123invalid = 5; // Invalid identifier
jodi (x > y) {  // Type mismatch in comparison
    dekhao("Result");
    undeclared_var = 10;  // Undeclared variable
}

loop (purno sonkha i = 0; i < 10; i++ {  // Missing closing parenthesis
    dekhao(i
}                             // Missing closing parenthesis and semicolon
```

### 5.2 Nested Structure Errors
```banglish
jodi (x > 5) {
    jodi (y < 10) {
        loop (purno sonkha i = 0; i < 5; i++ {  // Missing closing parenthesis
            dekhao(undeclared);         // Undeclared variable
            jodi (i == 2)
                dekhao("Two");          // Missing braces
        }
    }
```

## 6. Edge Cases

### 6.1 Empty File
```banglish
// Empty file or only comments
```

### 6.2 Invalid Characters
```banglish
purno sonkha x = 10;
purno sonkha y = 20©;        // Invalid character '©'
dekhao("Hello™");    // Invalid character '™'
```
