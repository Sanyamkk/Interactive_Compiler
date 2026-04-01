#  Compiler PBL Project

Welcome to the **Compiler Project Based Learning (PBL)** repository! This project is a fully functional, interactive compiler built from scratch. It features a robust C++ backend that performs multiple phases of compilation and a modern React frontend that makes it easy and fun to visualize the compiler's inner workings.

---

##  What is this project?

This project is designed to help understand how a compiler works under the hood. Instead of just taking code and spitting out an executable, this compiler acts as an educational tool. It takes custom source code and breaks down the compilation process into distinct phases, allowing you to see exactly what's happening at each step:

- **Lexical Analysis:** Breaking down code into valid tokens (keywords, identifiers, numbers, operators).
- **Parsing (Syntax Analysis):** Structuring tokens and checking if they follow the grammatical rules of the language.
- **Semantic Analysis:** Checking for logical correctness (e.g., using undeclared variables).
- **Intermediate Representation (IR):** Generating a lower-level representation of the code before final machine code generation.

The coolest part? Any errors found along the way are carefully tracked, categorized by phase, and sent to the interactive frontend for easy debugging!

---

##  Tech Stack & Architecture

This project is built using a modern decoupled architecture, combining the performance of C++ with the interactivity of a web application:

1. **Backend (C++)** `[/backend]`
   - The core brain of the operation.
   - Written in C++ (`main.cpp`) for high performance and fine-grained memory control.
   - Takes a source code file, compiles it, and outputs a structured **JSON** response containing tokens, the symbol table, generated IR code, and any errors found.

2. **Middleware Server (Node.js + Express)** `[/server]`
   - A lightweight Express server (`server.js`) running on port 3001.
   - Acts as the bridge between the frontend and the C++ backend.
   - Accepts code from the user, saves it temporarily, executes the C++ compiler (`compiler.exe`), and returns the JSON output to the frontend.

3. **Frontend (React + Vite)** `[/frontend]`
   - A fast and dynamic user interface built with React and Vite.
   - Allows users to type in code, hit compile, and instantly see the results broken down into readable sections: Tokens, Symbol Table, IR Code, and Errors.

4. **Test Cases** `[/test_cases]`
   - A collection of ready-to-use text files demonstrating various compiler capabilities:
     - `test1_valid.txt`: Perfectly valid code without errors.
     - `test2_lexical_error.txt`: Code containing invalid characters.
     - `test3_syntax_error.txt`: Code with missing semicolons or braces.
     - `test4_semantic_error.txt`: Code referencing undeclared variables.
     - ...and more!

---

##  How to Run the Project Locally

To get the full experience, you'll need to start both the Node.js server and the React frontend.

### Prerequisites
- [Node.js](https://nodejs.org/) installed on your machine.
- A C++ compiler (like g++) if you plan on modifying the `main.cpp` backend.
- *(Note: A pre-compiled `compiler.exe` is already included in `backend/` for Windows users).*

### Step 1: Start the Backend Server
1. Open a terminal and navigate to the `server` directory:
   ```bash
   cd server
   ```
2. Install dependencies (only needed the first time):
   ```bash
   npm install
   ```
3. Start the Express server:
   ```bash
   npm run start
   # or
   node server.js
   ```
   *The server will start running on `http://localhost:3001`.*

### Step 2: Start the React Frontend
1. Open **a new terminal window** and navigate to the `frontend` directory:
   ```bash
   cd frontend
   ```
2. Install dependencies (only needed the first time):
   ```bash
   npm install
   ```
3. Start the Vite development server:
   ```bash
   npm run dev
   ```
   *This will provide you with a local URL (usually `http://localhost:5173`) where you can access the UI.*

### Step 3: Write some code!
Open the frontend URL in your browser, type some custom code (or copy-paste from the `test_cases/` folder), and click Compile!

---

##  The Custom Language Syntax

The compiler accepts a subset of C-like syntax. Here are some of the supported features:
- **Data Types:** `int`, `float`
- **Arithmetic:** `+`, `-`, `*`, `/`
- **Output:** `print(variable);`
- **Functions:** Basic block declarations and `return` statements.

### Example Valid Code:
```c
int x = 10;
int y = 20;
int z = x + y;
print(z);
```

---

##  Future Scope
- Adding support for conditional statements (`if`, `else`) and loops (`for`, `while`).
- Enhancing Error Recovery mechanisms (like panic mode recovery).
- Adding full target machine code generation.

---

*Enjoy exploring the magic of compilers! *


<img width="1914" height="912" alt="image" src="https://github.com/user-attachments/assets/806b4574-6a2e-4fd5-85cc-27b265097c1c" />

<img width="1914" height="813" alt="image" src="https://github.com/user-attachments/assets/85b140fc-0fc6-457d-b1d9-fe1b709b40e6" />

<img width="672" height="617" alt="image" src="https://github.com/user-attachments/assets/20f3c29e-9f10-4a4c-8d64-a52162b8d732" />
