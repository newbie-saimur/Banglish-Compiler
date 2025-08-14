# Quick testcases

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
