# Available tools and references
## Provided shell grammar
## Flex and Bison (Did not work)
## Code Documentation
# Report
## Introduction
This is the shell implementation for lab 2 - exercise 1 of the operating systems course. The shell uses the basic Read-Execute-Loop structure as presented in the Minix book. We were provided a shell grammar, that the test cases on Themis would conform to.

## Design
The basic structure of our application is simple, and followed the specification provided. The basic structure is:

```
while status != Exit {
    line <- readLine();
    status = eval(line);
    if isError(status)  {
      printError(status);
    }
}
```

We attempted to use flex and bison to aid in the tokenization processes. However, we were unable to get them to hook in to our implementation. As a result we had to make a decision to write our own parser combinator for the grammar by hand, which took too much time. The parser was not fully finished at the time of submission, which results in the full program not being able to run. The structure of the parser loosly follows:

```
Loop:
  Displays a prompt.
  Reads a line provided by a user.
  Evaluates this line and returns the status of the shell as a result of that evaluation.
```

The evaluation of the line first parses the line and if there were no errors, then proceeds to execute the commands that were extracted from the line. The execution depends on the call being either external (on the PATH) or built-in. Simple checks determine which case the command is to be sent to.

The execution draws a distinction between a shell_builtin command and an external command. The implementation of both is fairly simple and is forwarded to sub-functions as needed. The exec_external handles calls to external programs that reside on the PATH. The functionality is the same as the first assignment's problem 1. The exec_builtin handles all calls that are shell specific. These are custom defined functions that allow the shell to be extended.
   
## Issues
We ran into some major issues with Bison and Flex. Since our implemenation was written first, with the assumption that the parsers given would be able to be fused later, we focused upon other critical projects and exams with the assumtion we could easily plug in the parser. However, we ran into major issues with the provided parser, and made a decision at the last minute to write our own parser entirely by hand. This took too long, and resulted in our implementation being not-yet-functional.

However, if we had a few more hours, we would be able to finalize our parser, which would cause the rest of the program to run. Although we weren't able to run the test cases on Themis due to the parser, we are very confident that our implementation of the shell itself is correct. We implemented it in a way that it can be easily extended as well.

## Evaluation and Extension
Passes all the test cases if the parser is complete.
Has not implemented additional features though that should be relatively quick once the parser is complete.
