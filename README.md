# 🐚 Minishell - My Own Little Bash

## Overview

Minishell is a custom implementation of a simplified bash shell, written entirely in C. This project challenged me to understand the intricacies of process management, file descriptors, command parsing, and system call execution - essentially recreating the core functionality of one of computing's most fundamental tools.

> "The existence of shells is linked to the very existence of IT." - Minishell subject

## 🌟 Key Features

- **Interactive Command Line**: Custom prompt with working history
- **Command Execution**: Launches executables based on PATH or absolute/relative paths
- **Quote Handling**: Proper interpretation of single and double quotes
- **I/O Redirection**: Support for `<`, `>`, `<<`, and `>>` operators
- **Piping**: Implementation of `|` to connect command outputs and inputs
- **Environment Variables**: Expansion of variables (including `$?` for exit status)
- **Signal Handling**: Proper behavior for Ctrl+C, Ctrl+D, and Ctrl+\
- **Built-in Commands**: Implementation of:
  - `echo` (with -n option)
  - `cd` (with relative/absolute paths)
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`

## 🔍 Technical Highlights

### Advanced Parsing with AST

One of the biggest challenges of this project was implementing an **Abstract Syntax Tree** for command parsing using a **recursive descent parser** - all in C! This approach allowed for:

- Proper handling of complex command structures
- Elegant management of nested operations
- Clean separation of parsing and execution logic
- Robust error handling and syntax validation

```
        Pipe
     /        \
  Cmd         Cmd
  /          /   \
Arg        Arg    Redirect
```

### Memory Management

Every allocated byte is tracked and properly freed, with special attention to:
- Command structures cleanup
- File descriptor management
- History handling
- Signal processing

## 🛠️ Implementation Details

### Signal Handling
Carefully implemented signal handling using a single global variable as specified in the requirements. All signals (Ctrl+C, Ctrl+D, Ctrl+\) behave according to spec, with proper prompt management.

### I/O Redirection & Pipes
Implemented file descriptor manipulation to handle:
- Input redirection (`<`)
- Output redirection (`>`)
- Heredoc (`<<`)
- Append output (`>>`)
- Piping multiple commands together (`|`)

### Environment Variable Expansion
Built a robust system for expanding environment variables within commands, with special handling for quotes and the `$?` variable.

## 📦 Installation & Usage

### Prerequisites

- GCC compiler
- readline library

### Building the Shell

```bash
make
```

### Running the Shell

```bash
./minishell
```

## 📋 Command Examples

```bash
# Basic command execution
$ ls -la

# Piping commands
$ ls | grep .c | wc -l

# I/O redirection
$ cat < file.txt > output.txt

# Environment variables
$ echo $HOME

# Heredoc
$ cat << EOF
```

## 🧠 Learning Outcomes

Building Minishell deepened my understanding of:
- Process creation and management in Unix systems
- Lexical analysis and parsing techniques
- File descriptor manipulation
- Signal handling in interactive programs
- Memory management in complex C applications
- Environment variable management

## 🙏 Acknowledgements

This project really pushed my C programming skills to the limit. Implementing a recursive descent parser with AST in C was challenging but incredibly rewarding. I now have a much deeper appreciation for the complexity behind tools I use every day.

---

*"As beautiful as a shell" - 42 Project Subject*
