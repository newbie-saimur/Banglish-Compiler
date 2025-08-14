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
