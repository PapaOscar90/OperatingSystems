#ifndef SHELL_H
#define SHELL_H

/// Evaluate the provided string, thereby executing the encoded commands.
Exec eval(char const *input);

/// Display a prompt to the user.
void type_prompt(void);

#endif /* SHELL_H */
