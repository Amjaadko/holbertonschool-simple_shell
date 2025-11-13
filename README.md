# 🐚 Simple Shell Project


---

## 📘 Overview
The **Simple Shell** is a custom UNIX command interpreter written in C as part of the **Holberton School curriculum**.  
It replicates the core behavior of a real shell like `/bin/sh`, allowing users to run commands, explore directories, and interact with the system in a command-line interface.

This project demonstrates key system programming concepts such as **process creation**, **environment management**, and **input/output handling** in C.

---

## 🧩 Features

✅ **Command Execution** – Run programs using full or relative paths.  
✅ **PATH Handling** – Automatically searches directories in the `PATH` environment variable.  
✅ **Built-in Commands** – Includes simple built-ins like:
- `exit` → exits the shell  
- `env` → prints environment variables  

✅ **Error Handling** – Displays clear messages when a command is not found.  
✅ **Interactive & Non-Interactive Mode** – Works both in a terminal or as a piped input.  
✅ **Memory Management** – Proper freeing of allocated memory to avoid leaks.

---

## 🧠 Technical Concepts

| Concept | Description |
|----------|--------------|
| **Fork & Execve** | Used to create and execute child processes |
| **Environment Variables** | Managed via `environ` and `getenv` |
| **PATH Resolution** | Searches executable files in directories listed in `PATH` |
| **File Descriptors** | Standard input/output/error handling |
| **String Manipulation** | Tokenization using `strtok()` and manual parsing |

---

## 🧱 File Structure

| File | Description |
|------|--------------|
| `simple_shell.c` | Main entry point; contains input loop |
| `execute_child.c` | Handles process creation and command execution |
| `path.c` | Builds the executable path using the environment variable |
| `env_helper.c` | Contains helper functions for environment access |
| `write_not_found.c` | Prints formatted error messages |
| `shell.h` | Header file with prototypes, structs, and macros |
| `man_1_simple_shell` | Manual page for the shell |
| `AUTHORS` | List of contributors |

---

## 🧰 Compilation

To compile all source files, use:
```bash
gcc -Wall -Werror -Wextra -pedantic *.c -o simple_shell

