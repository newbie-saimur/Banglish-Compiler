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
