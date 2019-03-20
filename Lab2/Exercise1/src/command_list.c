#include "macros.h"

#include "checked.h"
#include "command_list.h"
#include "util.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void free_command_list(CommandList command_list) {
  for (size_t i = 0; i < command_list.amount; ++i) {
    free_command_part(command_list.command_parts[i]);
  }
  free(command_list.command_parts);
}

CommandList new_command_list(size_t amount) {
  CommandList command_list = {.amount = amount,
                              .command_parts =
                                  checked_calloc(amount, sizeof(CommandPart))};
  return command_list;
}

// Routine adapted from
// https://stackoverflow.com/questions/8082932/connecting-n-commands-with-pipes-in-a-shell
pid_t spawn_proc(int in, int out, CommandPart command_part) {
  pid_t pid;

  if ((pid = fork()) == 0) {
    if (in != 0) {
      dup2(in, 0);
      close(in);
    }

    if (out != 1) {
      dup2(out, 1);
      close(out);
    }

    char *command_path = find_command_path(command_part.command_name);
    if (command_path == NULL) {
      printf("Error: command not found!\n");
      exit(EXIT_FAILURE);
    }
    char *newargv[3];
    newargv[0] = command_part.command_name;
    newargv[1] = command_part.options;
    newargv[2] = NULL;

    execve(command_path, newargv, NULL);
    free(command_path);
  }

  return pid;
}

Exec exec_command_list(CommandList command_list, IoRedirection io_redirection) {
  size_t i;
  int set_in = 0, set_out = 1;
  int fd[2];

  switch (io_redirection.type) {
  case IO_REDIRECTION_LEFT_RIGHT:
  case IO_REDIRECTION_RIGHT_LEFT:
    if (strcmp(io_redirection.file1, io_redirection.file2) == 0) {
      printf("Error: input and output files cannot be equal!\n");
      exit(EXIT_FAILURE);
    }
    break;
  default:
    break;
  }

  switch (io_redirection.type) {
  case IO_REDIRECTION_EMPTY:
    break;
  case IO_REDIRECTION_LEFT:
    // set current_fd to file.
    set_in = open(io_redirection.file, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    break;
  case IO_REDIRECTION_RIGHT:
    // set out to file.
    set_out = open(io_redirection.file, O_RDWR | O_CREAT | O_TRUNC,
                   S_IRUSR | S_IWUSR);
    break;
  case IO_REDIRECTION_LEFT_RIGHT:
    // set current_fd to file1.
    // set out to file2.
    set_in = open(io_redirection.file1, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    set_out = open(io_redirection.file2, O_RDWR | O_CREAT | O_TRUNC,
                   S_IRUSR | S_IWUSR);
    break;
  case IO_REDIRECTION_RIGHT_LEFT:
    // set out to file1.
    // set current_fd to file2.
    set_out = open(io_redirection.file1, O_RDWR | O_CREAT | O_TRUNC,
                   S_IRUSR | S_IWUSR);
    set_in = open(io_redirection.file2, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    break;
  }

  for (i = 0; i < command_list.amount - 1; ++i) {
    pipe(fd);

    spawn_proc(set_in, fd[1], command_list.command_parts[i]);

    close(fd[1]);

    set_in = fd[0];
  }

  DBG("%d %d", set_in, set_out);

  if (set_in != 0)
    dup2(set_in, 0);

  if (set_out != 1) {
    dup2(set_out, 1);
  }

  char *command_path =
      find_command_path(command_list.command_parts[i].command_name);
  if (command_path == NULL) {
    printf("Error: command not found!\n");
    exit(EXIT_FAILURE);
  }

  char *newargv[3];
  newargv[0] = command_list.command_parts[i].command_name;
  newargv[1] = command_list.command_parts[i].options;
  newargv[2] = NULL;

  execve(command_path, newargv, NULL);
  free(command_path);
  exit(EXIT_FAILURE);
}
