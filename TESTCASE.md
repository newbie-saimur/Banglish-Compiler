# Banglish Compiler Testcases

Below are 10 standalone Banglish programs covering basics, conditionals, loops, arrays, string handling, and assorted array operations. For each test, copy the code into `main.banglish`, place the input into `input.txt`, then build/run your compiler.

---

## 1) Basic I/O and arithmetic
```
shuru
purno sonkha a;
purno sonkha b;
poro(a);
poro(b);
purno sonkha sum = a + b;
purno sonkha prod = a * b;
dekhao "Sum: {sum}\n";
dekhao "Product: {prod}\n";
ferot dao 0;
shesh
```
Input:
```
7
5
```

Expected Output:
```
Sum: 12
Product: 35
```

---

## 2) If-else (even/odd)
```
shuru
purno sonkha x;
poro(x);
jodi (x % 2 == 0) { dekhao "Even\n"; }
nahoy { dekhao "Odd\n"; }
ferot dao 0;
shesh
```
Input:
```
42
```

Expected Output:
```
Even
```

---

## 3) Else-if chain (grading)
```
shuru
purno sonkha marks;
poro(marks);
jodi (marks >= 80) { dekhao "A\n"; }
nahoy jodi (marks >= 60) { dekhao "B\n"; }
nahoy { dekhao "C\n"; }
ferot dao 0;
shesh
```
Input:
```
73
```

Expected Output:
```
B
```

---

## 4) For loop: factorial
```
shuru
purno sonkha n;
poro(n);
purno sonkha fact = 1;
loop (purno sonkha i = 1; i <= n; i++) { fact = fact * i; }
dekhao "Factorial: {fact}\n";
ferot dao 0;
shesh
```
Input:
```
5
```

Expected Output:
```
Factorial: 120
```

---

## 5) Fixed-size array: sum and average
```
shuru
purno sonkha a[5];
loop (purno sonkha i = 0; i < 5; i++) { poro(a[i]); }
purno sonkha sum = 0;
loop (purno sonkha i = 0; i < 5; i++) { sum += a[i]; }
dosomik sonkha avg = (double)sum / 5;
dekhao "Sum: {sum}\n";
dekhao "Avg: {avg}\n";
ferot dao 0;
shesh
```
Input:
```
2 4 6 8 10
```

Expected Output:
```
Sum: 30
Avg: 6
```

---

## 6) Array min/max and even count
```
shuru
purno sonkha a[6];
loop (purno sonkha i = 0; i < 6; i++) { poro(a[i]); }
purno sonkha mn = a[0];
purno sonkha mx = a[0];
purno sonkha evens = 0;
loop (purno sonkha i = 0; i < 6; i++) {
    jodi (a[i] < mn) { mn = a[i]; }
    jodi (a[i] > mx) { mx = a[i]; }
    jodi (a[i] % 2 == 0) { evens++; }
}
dekhao "Min: {mn}, Max: {mx}\n";
dekhao "Even count: {evens}\n";
ferot dao 0;
shesh
```
Input:
```
7 3 12 5 8 9
```

Expected Output:
```
Min: 3, Max: 12
Even count: 2
```

---

## 7) String input and interpolation
```
shuru
lekha name;
dekhao "Enter name: ";
poro(name);
dekhao "Hello, {name}!\n";
ferot dao 0;
shesh
```
Input:
```
Ada Lovelace
```

Expected Output:
```
Enter name: Hello, Ada Lovelace!
```

---

## 8) Linear search in array
```
shuru
purno sonkha a[7];
loop (purno sonkha i = 0; i < 7; i++) { poro(a[i]); }
purno sonkha target; poro(target);
sotto-mittha found = false;
loop (purno sonkha i = 0; i < 7; i++) {
    jodi (a[i] == target) { found = true; }
}
jodi (found) { dekhao "Found\n"; }
nahoy { dekhao "Not Found\n"; }
ferot dao 0;
shesh
```
Input:
```
5 1 4 9 2 8 7
9
```

Expected Output:
```
Found
```

---

## 9) Reverse print array
```
shuru
purno sonkha a[5];
loop (purno sonkha i = 0; i < 5; i++) { poro(a[i]); }
dekhao "Reversed: ";
loop (purno sonkha i = 4; i >= 0; i--) { dekhao "{a[i]} "; }
dekhao "\n";
ferot dao 0;
shesh
```
Input:
```
3 1 4 1 5
```

Expected Output:
```
Reversed: 5 1 4 1 3 
```

---

## 10) Chars and mapping
```
shuru
akkhor grade;
poro(grade);
jodi (grade == 'A') { dekhao "Excellent\n"; }
nahoy jodi (grade == 'B') { dekhao "Good\n"; }
nahoy jodi (grade == 'C') { dekhao "Average\n"; }
nahoy { dekhao "Unknown\n"; }
ferot dao 0;
shesh
```
Input:
```
B
```

Expected Output:
```
Good
```

Notes:
- Arrays use fixed sizes to ensure C++ compatibility after transpilation.
- `dekhao` doesn’t add a newline automatically; explicit "\n" is added where needed.
- `lekha` reads entire lines with `poro(name)`.# Quick testcases

1) Simple IO/loop
```
shuru
purno sonkha n = 3;
dekhao "Count:";
for (purno sonkha i = 0; i < n; i++) {
  dekhao i;
}
ferot dao 0;
shesh
```

2) If / else-if / else and expressions
```
shuru
purno sonkha a = 5;
purno sonkha b = 10;
jekhane = a + b; // not recognized as declaration; shows token stream
jodi (a < b && b != 0) {
  dekhao "sum {a+b}";
} nahoy jodi (a == b) {
  dekhao "equal";
} nahoy {
  dekhao "no";
}
ferot dao 0;
shesh
```

3) scanf/printf style
```
shuru
purno sonkha x;
dekhao "enter x:";
poro (x);
dekhao "x is {x}";
ferot dao 0;
shesh
```

4) scanf/printf with space newline
```
shuru
lekha name;
poro (name);

purno sonkha n;
poro (n);

jodi (n <= 0) {
  dekhao "No items\n";
} nahoy jodi (n == 1) {
  dekhao "One item for {name}\n";
} nahoy {
  dekhao "{name} has {n} items\n";
}

for (purno sonkha i = n; i > 0; i--) {
  dekhao "{i} ";
}
dekhao \n;

purno sonkha a = 10;
purno sonkha b = 3;
purno sonkha c;
c = a + b * 2 - 5 / 5;
c += 1;
c -= 2;

jodi (!(a % b == 1) || (a >= b && a > 0)) {
  dekhao "ok {c}\n";
} nahoy {
  dekhao "bad\n";
}

ferot dao 0;
shesh
```

---

## 11) Nested If-Else Statements
```
shuru
purno sonkha x;
purno sonkha y;
poro(x);
poro(y);

jodi (x > 0) {
    dekhao "x is positive\n";
    jodi (y > 0) {
        dekhao "Both x and y are positive\n";
        jodi (x > y) {
            dekhao "x is greater than y\n";
        }
        nahoy jodi (x == y) {
            dekhao "x equals y\n";
        }
        nahoy {
            dekhao "x is less than y\n";
        }
    }
    nahoy {
        dekhao "x is positive but y is not\n";
    }
}
nahoy {
    dekhao "x is not positive\n";
}
ferot dao 0;
shesh
```
Input:
```
10
5
```

Expected Output:
```
x is positive
Both x and y are positive
x is greater than y
```

---

## 12) Nested Loops with Pattern Printing
```
shuru
purno sonkha rows;
poro(rows);

loop (purno sonkha i = 1; i <= rows; i++) {
    loop (purno sonkha j = 1; j <= i; j++) {
        dekhao "{j} ";
    }
    dekhao "\n";
}

dekhao "Multiplication Table:\n";
loop (purno sonkha i = 1; i <= 3; i++) {
    loop (purno sonkha j = 1; j <= 3; j++) {
        purno sonkha product = i * j;
        dekhao "{i}x{j}={product} ";
    }
    dekhao "\n";
}
ferot dao 0;
shesh
```
Input:
```
4
```

Expected Output:
```
1 
1 2 
1 2 3 
1 2 3 4 
Multiplication Table:
1x1=1 1x2=2 1x3=3 
2x1=2 2x2=4 2x3=6 
3x1=3 3x2=6 3x3=9 
```

---

## 13) Mixed Data Types and Boolean Operations
```
shuru
purno sonkha age;
dosomik sonkha salary;
lekha name;
sotto-mittha isEmployed;

poro(age);
poro(salary);
poro(name);
poro(isEmployed);

jodi (age >= 18 && isEmployed) {
    jodi (salary >= 50000.0) {
        dekhao "{name} is an adult with high salary\n";
    }
    nahoy {
        dekhao "{name} is an adult with low salary\n";
    }
}
nahoy jodi (age >= 18) {
    dekhao "{name} is an adult but unemployed\n";
}
nahoy {
    dekhao "{name} is a minor\n";
}

dosomik sonkha tax = 0.0;
jodi (salary > 30000.0) {
    tax = salary * 0.15;
}
dekhao "Tax: {tax}\n";
ferot dao 0;
shesh
```
Input:
```
25
55000.5
John
1
```

Expected Output:
```
John is an adult with high salary
Tax: 8250.075
```

---

## 14) Array Operations with Search and Sort Check
```
shuru
purno sonkha n;
poro(n);
purno sonkha arr[n];

loop (purno sonkha i = 0; i < n; i++) {
    poro(arr[i]);
}

// Find maximum and its position
purno sonkha maxVal = arr[0];
purno sonkha maxPos = 0;
loop (purno sonkha i = 1; i < n; i++) {
    jodi (arr[i] > maxVal) {
        maxVal = arr[i];
        maxPos = i;
    }
}

dekhao "Maximum: {maxVal} at position {maxPos}\n";

// Check if array is sorted
sotto-mittha isSorted = true;
loop (purno sonkha i = 0; i < n - 1; i++) {
    jodi (arr[i] > arr[i + 1]) {
        isSorted = false;
    }
}

jodi (isSorted) {
    dekhao "Array is sorted\n";
}
nahoy {
    dekhao "Array is not sorted\n";
}

// Count duplicates
purno sonkha duplicates = 0;
loop (purno sonkha i = 0; i < n; i++) {
    loop (purno sonkha j = i + 1; j < n; j++) {
        jodi (arr[i] == arr[j]) {
            duplicates++;
        }
    }
}
dekhao "Duplicate pairs: {duplicates}\n";
ferot dao 0;
shesh
```
Input:
```
5
3 7 2 9 5
```

Expected Output:
```
Maximum: 9 at position 3
Array is not sorted
Duplicate pairs: 0
```

---

## 15) String Processing and Character Operations
```
shuru
lekha text;
akkhor ch;
poro(text);
poro(ch);

// Simple string length calculation (this is conceptual as actual string length needs different approach)
dekhao "Input text: {text}\n";
dekhao "Character to count: {ch}\n";

// Note: In real implementation, string length would be handled differently
// This is a simplified example for educational purposes
dekhao "Text processed successfully\n";

jodi (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
    dekhao "Character is a vowel\n";
}
nahoy {
    dekhao "Character is a consonant\n";
}

jodi (ch >= 'A' && ch <= 'Z') {
    dekhao "Character is uppercase\n";
}
nahoy jodi (ch >= 'a' && ch <= 'z') {
    dekhao "Character is lowercase\n";
}
nahoy {
    dekhao "Character is not a letter\n";
}
ferot dao 0;
shesh
```
Input:
```
Hello World
e
```

Expected Output:
```
Input text: Hello World
Character to count: e
Text processed successfully
Character is a vowel
Character is lowercase
```

---

## 16) Mathematical Calculations and Functions Simulation
```
shuru
purno sonkha num;
poro(num);

// Prime number check
sotto-mittha isPrime = true;
jodi (num <= 1) {
    isPrime = false;
}
nahoy {
    loop (purno sonkha i = 2; i * i <= num; i++) {
        jodi (num % i == 0) {
            isPrime = false;
        }
    }
}

jodi (isPrime) {
    dekhao "{num} is prime\n";
}
nahoy {
    dekhao "{num} is not prime\n";
}

// Fibonacci sequence up to num terms
dekhao "Fibonacci sequence:\n";
purno sonkha a = 0;
purno sonkha b = 1;
jodi (num >= 1) {
    dekhao "{a} ";
}
jodi (num >= 2) {
    dekhao "{b} ";
}

loop (purno sonkha i = 3; i <= num; i++) {
    purno sonkha next = a + b;
    dekhao "{next} ";
    a = b;
    b = next;
}
dekhao "\n";

// Sum of digits
purno sonkha temp = num;
purno sonkha digitSum = 0;
loop (purno sonkha i = 0; temp > 0; i++) {
    digitSum += temp % 10;
    temp = temp / 10;
}
dekhao "Sum of digits: {digitSum}\n";
ferot dao 0;
shesh
```
Input:
```
7
```

Expected Output:
```
7 is prime
Fibonacci sequence:
0 1 1 2 3 5 8 
Sum of digits: 7
```

---

## 17) Advanced Control Flow with Multiple Conditions
```
shuru
purno sonkha score;
purno sonkha attendance;
sotto-mittha hasProject;

poro(score);
poro(attendance);
poro(hasProject);

// Complex grading system
jodi (score >= 90 && attendance >= 80) {
    jodi (hasProject) {
        dekhao "Grade: A+\n";
    }
    nahoy {
        dekhao "Grade: A\n";
    }
}
nahoy jodi (score >= 80 && attendance >= 70) {
    jodi (hasProject) {
        dekhao "Grade: B+\n";
    }
    nahoy {
        dekhao "Grade: B\n";
    }
}
nahoy jodi (score >= 70) {
    jodi (attendance >= 60 && hasProject) {
        dekhao "Grade: C+\n";
    }
    nahoy {
        dekhao "Grade: C\n";
    }
}
nahoy {
    dekhao "Grade: F\n";
}

// Status messages
jodi ((score >= 80 && attendance >= 75) || (hasProject && score >= 70)) {
    dekhao "Student is performing well\n";
}
nahoy {
    dekhao "Student needs improvement\n";
}

// Scholarship eligibility
jodi (score >= 85 && attendance >= 80 && hasProject) {
    dekhao "Eligible for scholarship\n";
}
nahoy {
    dekhao "Not eligible for scholarship\n";
}
ferot dao 0;
shesh
```
Input:
```
92
85
1
```

Expected Output:
```
Grade: A+
Student is performing well
Eligible for scholarship
```

---

## 18) Matrix-like Operations with 2D Arrays Simulation
```
shuru
purno sonkha rows;
purno sonkha cols;
poro(rows);
poro(cols);

// Simulate 2D array with 1D array
purno sonkha totalSize = rows * cols;
purno sonkha matrix[totalSize];

dekhao "Enter matrix elements:\n";
loop (purno sonkha i = 0; i < totalSize; i++) {
    poro(matrix[i]);
}

// Display matrix
dekhao "Matrix:\n";
loop (purno sonkha i = 0; i < rows; i++) {
    loop (purno sonkha j = 0; j < cols; j++) {
        purno sonkha index = i * cols + j;
        dekhao "{matrix[index]} ";
    }
    dekhao "\n";
}

// Calculate row sums
dekhao "Row sums:\n";
loop (purno sonkha i = 0; i < rows; i++) {
    purno sonkha rowSum = 0;
    loop (purno sonkha j = 0; j < cols; j++) {
        purno sonkha index = i * cols + j;
        rowSum += matrix[index];
    }
    dekhao "Row {i}: {rowSum}\n";
}

// Find overall sum and average
purno sonkha totalSum = 0;
loop (purno sonkha i = 0; i < totalSize; i++) {
    totalSum += matrix[i];
}
dosomik sonkha average = (double)totalSum / totalSize;
dekhao "Total sum: {totalSum}\n";
dekhao "Average: {average}\n";
ferot dao 0;
shesh
```
Input:
```
2
3
1 2 3 4 5 6
```

Expected Output:
```
Enter matrix elements:
Matrix:
1 2 3 
4 5 6 
Row sums:
Row 0: 6
Row 1: 15
Total sum: 21
Average: 3.5
```

---

## Additional Test Cases Summary

### Edge Cases and Special Scenarios:

**Test Case 19: Zero and Negative Numbers**
```
Input: 0, -5
Expected: Handle zero and negative number operations correctly
```

**Test Case 20: Large Numbers**
```
Input: 999999
Expected: Handle large integer calculations without overflow (within int limits)
```

**Test Case 21: Boolean Logic Complex**
```
Input: Various boolean combinations
Expected: Correct evaluation of complex boolean expressions with &&, ||, !
```

**Test Case 22: Array Boundary Conditions**
```
Input: Single element array, empty conditions
Expected: Handle edge cases in array operations
```

### Usage Instructions:

1. **Running Tests**: Copy any test case code to `main.banglish`
2. **Input Setup**: Create `input.txt` with the provided input values (each on a new line)
3. **Execution**: Run `build_and_run.ps1` 
4. **Verification**: Compare the program output with the expected output
5. **Debugging**: Check `output_validation.txt` for any syntax errors

### Test Categories:

- **Basic Operations** (Tests 1-4): I/O, arithmetic, conditionals, loops
- **Data Structures** (Tests 5-9): Arrays, searching, sorting concepts  
- **Advanced Features** (Tests 11-18): Nested structures, mixed types, complex algorithms
- **Edge Cases** (Tests 19-22): Boundary conditions and special scenarios

These test cases comprehensively verify the Banglish compiler's capabilities including nested control structures, array operations, mixed data types, and complex algorithmic implementations.
