# Advanced Text Editor (C++ Console Application)

A feature-rich **console-based text editor** built in C++ that simulates core functionalities of real text editors such as **insert, delete, undo/redo, cursor movement, word operations, and file handling**.

---

## 📌 What is this project?

This project is a **C++ text editor simulation** that demonstrates how a real text editor works internally using **data structures and algorithms**.

It supports:
- Text insertion and deletion
- Cursor-based editing
- Undo and redo system
- Word-based deletion
- Search functionality
- File saving and loading
- Usage statistics tracking

---

## ⚙️ Features

### ✍️ Text Editing
- Insert text at cursor position
- Delete characters at cursor
- Delete custom range of text
- Delete full word at cursor

### 🔁 Undo / Redo System
- Undo last operations (insert/delete)
- Redo undone operations
- Stack-based implementation

### 📍 Cursor Control
- Move cursor to any position
- Real-time cursor display

### 🔍 Search
- Find all occurrences of a pattern
- Displays all matching positions

### 💾 File Handling
- Save text to file
- Load text from file
- Append text to existing file

### 📊 Statistics
- Number of insert operations
- Number of delete operations
- Undo/Redo counts
- Stack sizes

---

## 🧠 How it works (Concepts Used)

This project is built using core **Data Structures & Algorithms concepts**:

- `stack (vector)` → Undo/Redo system
- `string manipulation` → text editing
- `file streams` → file handling
- `cursor logic` → index-based editing
- `search algorithm` → substring search
- `encapsulation (OOP)` → class-based design

---

## 👨‍💻 Who can use this project?

This project is useful for:

- 🎓 Computer Science students (DSA / OOP practice)
- 🧑‍🏫 Beginners learning C++
- 🧪 Students preparing for programming exams
- 💼 Developers learning how text editors work internally

---

## 🚀 Why this project?

This project was created to:

- Understand how real text editors manage data internally
- Practice **object-oriented programming in C++**
- Implement **undo/redo systems using stacks**
- Improve problem-solving and logic-building skills
- Build a strong portfolio project for academic submission

---

## 🛠️ Technologies Used

- C++
- STL (vector, string, algorithm)
- File Handling (fstream)
- OOP principles

---

## 📷 Example Output
========== DOCUMENT ==========
|Hello World|

Chars: 11 | Words: 2 | Cursor: 11



---

## 📈 Future Improvements

- GUI version (Qt or SFML)
- Syntax highlighting
- Multi-line editing support
- Clipboard (copy/paste system)
- Mouse cursor support
- Auto-save feature

---

## 📁 How to run

bash
g++ main.cpp -o editor
./editor

📌 Author

Developed as a learning project for mastering:

Data Structures
Algorithms
C++ OOP design


⭐ If you like this project

Give it a star ⭐ and feel free to improve it or fork it.


---

If you want next level improvement, I can also:
- rewrite your code into a **clean “industry-style architecture”**
- or add **diagram explanation for your presentation**
- or make it look like a **real Notepad++ mini clone project** 🚀
