# Banglish Language Regular Expressions

This document contains all the regular expressions used in the Banglish compiler for lexical analysis, parsing, and validation.

## 1. Keywords and Reserved Words

### Single Keywords
```regex
^(shuru|shesh|poro|dekhao|loop|jodi|nahoy|lekha|akkhor)$
```

### Compound Keywords (Two Words)
```regex
^(purno sonkha|dosomik sonkha|ferot dao|nahoy jodi)$
```

### Hyphenated Keywords
```regex
^(sotto-mittha)$
```

### All Keywords Combined
```regex
^(shuru|shesh|purno sonkha|dosomik sonkha|lekha|akkhor|sotto-mittha|jodi|nahoy jodi|nahoy|poro|dekhao|loop|ferot dao)$
```

## 2. Identifiers and Variables

### Valid Identifier Pattern
```regex
^[A-Za-z_][A-Za-z0-9_]*$
```

### Banglish Naming Convention (Preferred)
- **Variables**: Should use meaningful Bengali-English words
- **Examples**: `purno_sonkha`, `naam`, `boyos`, `taka_amount`

### Invalid Identifier Patterns
```regex
^[0-9].*$        # Cannot start with digit
^[^A-Za-z_].*$   # Cannot start with special characters
.*[\s\-].*       # Cannot contain spaces or hyphens (except in keywords)
```

## 3. Data Types and Literals

### Integer Numbers
```regex
^[0-9]+$
```

### Floating Point Numbers
```regex
^[0-9]+\.[0-9]+$
```

### Combined Number Pattern
```regex
^[0-9]+(?:\.[0-9]+)?$
```

### String Literals
```regex
^"(?:[^"\\\\]|\\\\.)*"$
```

### Character Literals
```regex
^'(?:[^'\\\\]|\\\\.)+'$
```

### Boolean Values
```regex
^(true|false|0|1)$
```

## 4. Operators

### Arithmetic Operators
```regex
^(\+|-|\*|/|%|\+\+|--|\+=|-=|\*=|/=|%=)$
```

### Comparison Operators
```regex
^(==|!=|<=|>=|<|>)$
```

### Logical Operators
```regex
^(&&|\|\||!)$
```

### Assignment Operators
```regex
^(=|\+=|-=|\*=|/=|%=)$
```

### All Operators Combined
```regex
^(\+\+|--|<=|>=|==|!=|&&|\|\||\+=|-=|\*=|/=|%=|[+\-*/%<>=!&|])$
```

## 5. Punctuation and Delimiters

### Brackets and Braces
```regex
^[\(\)\{\}\[\]]$
```

### Punctuation
```regex
^[;,]$
```

### All Delimiters
```regex
^[\(\)\{\}\[\];,]$
```

## 6. Statement Patterns

### Variable Declarations
```regex
^(purno sonkha|dosomik sonkha|lekha|akkhor|sotto-mittha)\s+[A-Za-z_][A-Za-z0-9_]*(?:\s*\[\s*[^\]]+\s*\])?(?:\s*=\s*[^;]+)?\s*;?\s*$
```

### Input Statements
```regex
^poro\s*\(\s*[A-Za-z_][A-Za-z0-9_]*(?:\s*\[\s*[^\]]+\s*\])?\s*\)\s*;?\s*$
```

### Output Statements
```regex
^dekhao\s+.+;?\s*$
```

### If Statements
```regex
^jodi\s*\([^)]*\)\s*\{?\s*$
```

### Else If Statements
```regex
^nahoy jodi\s*\([^)]*\)\s*\{?\s*$
```

### Else Statements
```regex
^nahoy(?:\s*\{?\s*)?$
```

### Loop Statements
```regex
^loop\s*\([^;]*;[^;]*;[^)]*\)\s*\{?\s*$
```

### Return Statements
```regex
^ferot dao\s+[^;]+\s*;?\s*$
```

### Assignment Statements
```regex
^[A-Za-z_][A-Za-z0-9_]*(?:\s*\[\s*[^\]]+\s*\])?\s*(?:\+\+|--|\+=|-=|\*=|/=|%=|=)\s*[^;]*\s*;?\s*$
```

## 7. Comments

### Single Line Comments
```regex
^//.*$
```

### Multi-line Comments (Simple)
```regex
/\*.*?\*/
```

## 8. Whitespace and Line Endings

### Whitespace
```regex
[ \t\r\n]+
```

### Line Endings
```regex
\r?\n
```

## 9. Array Declarations and Access

### Array Declaration
```regex
^(purno sonkha|dosomik sonkha|lekha|akkhor|sotto-mittha)\s+[A-Za-z_][A-Za-z0-9_]*\s*\[\s*[^\]]+\s*\](?:\s*=\s*[^;]+)?\s*;?\s*$
```

### Array Access
```regex
^[A-Za-z_][A-Za-z0-9_]*\s*\[\s*[^\]]+\s*\]$
```

## 10. Expression Patterns

### Simple Expression
```regex
^[A-Za-z_][A-Za-z0-9_]*(?:\s*[\+\-\*/\%]\s*[A-Za-z0-9_]+)*$
```

### Parenthesized Expression
```regex
^\([^)]+\)$
```

### Function Call Pattern (for input/output)
```regex
^[A-Za-z_][A-Za-z0-9_]*\s*\([^)]*\)$
```

## 11. String Interpolation

### String with Variables
```regex
^"(?:[^"{\\\\]|\\\\.|\{[A-Za-z_][A-Za-z0-9_]*\})*"$
```

### Variable in String
```regex
\{([A-Za-z_][A-Za-z0-9_]*)\}
```

## 12. Error Detection Patterns

### Invalid Keywords
```regex
^(?!shuru|shesh|purno sonkha|dosomik sonkha|lekha|akkhor|sotto-mittha|jodi|nahoy jodi|nahoy|poro|dekhao|loop|ferot dao)[A-Za-z]+(?:\s+[A-Za-z]+)*$
```

### Invalid Variable Names
```regex
^(?:[0-9]|[^A-Za-z_])[A-Za-z0-9_]*$|^[A-Za-z_][A-Za-z0-9_]*[\s\-][A-Za-z0-9_]*$
```

### Unmatched Brackets
```regex
^[^(){}[\]]*[\(\{\[](?:[^(){}[\]]*[\)\}\]])*[^(){}[\]]*$
```

## 13. Language-Specific Validation

### Banglish Naming Convention Rules
1. **Variables should be meaningful**: Use descriptive names
2. **Mixed language allowed**: Bengali words in English characters
3. **Underscore separation**: Use `_` for multi-word variables
4. **No reserved words**: Cannot use keywords as identifiers

### Examples of Valid Names
```
naam, boyos, taka_amount, purno_number, student_list, is_valid
```

### Examples of Invalid Names
```
123abc, -invalid, purno sonkha, jodi, class-name
```

## 14. Complete Lexical Pattern

### Token Recognition Pattern
```regex
(?:
  (?://.*$)|                                    # Comments
  (?:"(?:[^"\\\\]|\\\\.)*")|                    # Strings
  (?:'(?:[^'\\\\]|\\\\.)+')|                    # Characters
  (?:[0-9]+(?:\.[0-9]+)?)|                     # Numbers
  (?:purno sonkha|dosomik sonkha|ferot dao|nahoy jodi|sotto-mittha)|  # Compound keywords
  (?:shuru|shesh|poro|dekhao|loop|jodi|nahoy|lekha|akkhor)|           # Simple keywords
  (?:[A-Za-z_][A-Za-z0-9_]*)|                 # Identifiers
  (?:\+\+|--|<=|>=|==|!=|&&|\|\||\+=|-=|\*=|/=|%=)|  # Multi-char operators
  (?:[+\-*/%<>=!&|(){}[\];,])|                # Single-char operators
  (?:\s+)                                      # Whitespace
)
```

## Usage Notes

1. **Case Sensitivity**: Banglish is case-sensitive
2. **Unicode Support**: Currently ASCII only, future versions may support Bengali script
3. **Error Recovery**: Parser should continue after errors for better diagnostics
4. **Precedence**: Operators follow C-style precedence rules
5. **Associativity**: Left-to-right for most operators

## Implementation Guidelines

- Use these patterns in the lexer for token recognition
- Implement error recovery for invalid patterns
- Maintain error logs with line and column information
- Provide helpful error messages for common mistakes
- Support IDE features like syntax highlighting using these patterns
