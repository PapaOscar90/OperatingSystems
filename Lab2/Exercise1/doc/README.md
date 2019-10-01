# Report
## Introduction
This is the shell implementation for lab 2 - exercise 1 of the operating systems course. The shell uses the basic Read-Execute-Loop structure as presented in the Minix book. We were provided a shell grammar, that the test cases on Themis would conform to.

## Design
The basic structure of our application is simple, and followed the specification provided. The basic structure is:

```
loop {
    line <- readLine();
    status = eval(line);
    if status == EXIT {
        break;
    }
}
```

The process of `eval`ing a line is as follows:
```
parsed <- parse(line);
if parsed.is_err() {
    print(parsing error);
    return;
}

exec(parsed);
```

A crucial point to know focus in the design of our application was the determination of our data structures. To simplify and unify the parsing and execution process, we created data structures that followed the provided grammar precisely.

For example consider our `InputLine` type:
```c
typedef struct {
  size_t amount;
  Command *commands;
} InputLine;
```

This matches the provided grammar and we follow this technique recursively for `Command` and the other types in the grammar until we get to base types (`filename`, `command_name`. `options` are all `char *`).

With these structures in place our problem shifts to implementing the parser to generate the top level `InputLine` struct and the executor to run the `InputLine`.

With respect to the parsing, we attempting to use flex and bison in the parsing processes. However, this ended up being far more difficult than anticipated and delayed our work substantially. As a result, we decided to implement a parser combinator, with one parser function being dedicated to each element of the grammar. Composition of these sub-parsers provides us with the upper level `parse_input_line` function. We added some small tests to aid in the design of the parser functions and these tests may be executed via `make test`.

In a similar fashion, the executor function is also composed of sub-executors.  The upper level `exec_input_line`, iterates through each command and delegates the execution of that command to `exec_command`. This function then executes a command on a child process. If the command should be run in the background the parent does not wait for the child to terminate. The child the executes the underlying `CommandList` associated with the command. This is the base level of our execution process and handles Io-redirection, piping, and running the actual base `command_name` + `options`.

IO redirection is handled by redefining the `stdin` and `stdout` file descriptors via `dup2`. For the propagation of errors we made heavy use of tagged unions. This may not be conventional C, but it aided greatly in the development process. This could structure could be reworked, reduced, and modified to be POSIX-compliant.
   
## Issues

## Evaluation and Extension
Passes most of the test cases.

It does not however handle orphan processes correctly and allows for exiting when child processes are still running.

Introducing a distinction in the type between an `external` command and a `builtin` would aid in rapidly developing more shell builtin commands. This should be propagated through to the parser, so that a command_name is either a string or a shell builtin value from an enum.
