# Banglish Compiler

This is a small Banglish-to-C++ pipeline that performs:

- Lexical analysis of `main.banglish` and writes:
  - tokens to `output_tokens.txt`
  - symbol table to `output_symbol_table.txt`
- Transpiles the Banglish program to C++ in `.generated/transpiled.cpp`
- Compiles and runs the transpiled code, reading `input.txt` for runtime input
- Writes program output to `output.txt`

Supported constructs (subset):
- Declarations: `purno sonkha`, `dosomik sonkha`, `lekha`, `akkhor`, `sotto-mittha`
- Assignment, arithmetic, comparisons, logical not `!`
- Increments/decrements: `++`, `--`, `+=`, `-=`
- Print: `dekhao <expr>` or `dekhao "string"` or `dekhao "str {var} str"`
  - Default: no newline at end; use `dekhao \n` to print a newline.
  - C-style escapes inside strings are honored: `\n`, `\t`, `\"`, `\\`.
- Input: `poro (x)`
- Control flow: `jodi` (if), `nahoy jodi` (else if), `nahoy` (else)
- Loop: `for (...)` like standard C++ syntax inside Banglish, both ++ and -- iterators
- Program boundaries: `shuru` ... `shesh`
- Return: `ferot dao <expr>`

Note: This is a minimal educational implementation, not a full parser. It relies on simple tokenization and line-based translation with regexes.

## One-command run

- Put your Banglish program in `main.banglish`
- Put inputs (if any) line-by-line in `input.txt`
- Build and run the driver:

```
# Windows PowerShell
.\u005cbuild_and_run.ps1
```

Artifacts:
- `output_tokens.txt`
- `output_symbol_table.txt`
- `output.txt` (program output)

## Example Banglish

```
shuru
  purno sonkha n = 5;
  jodi (n > 0) {
    dekhao "N is {n}";
  } nahoy {
    dekhao "No";
  }
  for (purno sonkha i = 0; i < n; i++) {
    dekhao i;
  }
  ferot dao 0;
shesh
```

