# 🐚 Minishell

`minishell` is a small, functional Unix shell written in C.  
It reproduces the basic behavior of **bash**, including command parsing, environment handling, and process execution — giving a deeper understanding of how a real shell works behind the scenes.

---

## 🚀 Overview

`minishell` provides a simplified version of a Unix shell that can interpret and execute user commands with arguments, redirections, and pipes.  
It manages processes, signals, and environment variables while maintaining an interactive command loop.

Example:
```bash
$> ./minishell
minishell$ echo "Hello, world!"
Hello, world!
minishell$ ls -l | grep .c > output.txt
