# Banglish Compiler - Run Command

### Windows
```powershell
powershell -NoLogo -NoProfile -ExecutionPolicy Bypass -File "D:\Banglish-Compiler\build_and_run.ps1"
```


### Linux
```bash
chmod +x build_and_run.sh
./build_and_run.sh
```
---

**Note**: This driver script automatically:
1. Compiles the Banglish compiler
2. Runs it on `main.banglish` 
3. Generates transpiled C++ code
4. Compiles and runs the final program with `input.txt`
5. Outputs results to `output.txt`
