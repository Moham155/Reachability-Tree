# Reachability Tree Generator for Petri Nets

This project generates a reachability tree from a Petri net using Depth-First Search (DFS) and outputs it in [Graphviz DOT](https://graphviz.org/doc/info/lang.html) format for visualization.

## 📁 Files Included

- `png.cpp`: Main C++ source code for reachability tree construction.
- `png.exe`: Precompiled executable for Windows.
- `reachability_tree.dot`: DOT-format file representing the tree structure.
- `reachability_tree.png`: Visual graph image generated from the DOT file.
- Build files: `png.obj`, `png.pdb`, `png.ilk`, `DFSREAHCABILITy` (can be ignored unless debugging or recompiling).

## 🛠️ How to Run

### Option 1: Run the Executable (Windows)
Open a terminal in the project directory and run:
```bash
./png.exe
```

### Option 2: Compile from Source (Cross-platform)
Make sure you have a C++ compiler installed.

#### On Linux/macOS (or WSL on Windows)
```bash
g++ png.cpp -o png
./png
```

#### On Windows (MinGW or MSVC)
```bash
g++ png.cpp -o png.exe
./png.exe
```

## 📈 Output Details

- **Console Output**: Displays markings visited, transitions fired, and whether a specific marking or sequence leads to a target.
- **`reachability_tree.dot`**: Contains the full tree in DOT language.
- **To generate a PNG graph**:
```bash
dot -Tpng reachability_tree.dot -o reachability_tree.png
```

## 🔁 Logic Overview

- Initial marking: `000000001` (only p9 has a token)
- Transitions (t1 to t6) defined using input/output places
- DFS traversal to explore all reachable markings
- Tree edges labeled with transitions that cause state changes
- Optional: check if a sequence (e.g., t1 → t2 → t6 → ...) leads to a target marking

## 🧠 Customization Tips

- Modify `places`, `transitions`, or the initial marking in `png.cpp` to fit your net
- You can also change the sequence and target marking used for reachability checks

## 🧰 Dependencies

Only dependency for visualization:
- [Graphviz](https://graphviz.org/download/)

## 📝 License

This project is open-source and free to use under the MIT License.

---

Happy coding! 🚀
