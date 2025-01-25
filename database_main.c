#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { // defining a new data structure called InputBuffer
  char* buffer; // this holds the pointer pointing to the first character of the buffer string
  size_t buffer_length; // stores the allocated size of the buffer in bytes
  ssize_t input_length; // length of input data currently stored in the buffer
} InputBuffer;

InputBuffer* new_input_buffer() { // dynamically allocates memory for a new InputBuffer structure and initializes it
  InputBuffer* input_buffer = malloc(sizeof(InputBuffer)); // allocates memory on the heap for a block of memory large enough to hold an InputBuffer struct
  input_buffer->buffer = NULL; // pointer to newly allocated memory block. Initialized to NULL to indicate that no memory has been allocated yet
  input_buffer->buffer_length = 0; // sets the initial buffer length to 0
  input_buffer->input_length = 0; // sets the initial input length to 0

  return input_buffer; // returns the pointer to the newly initialized struct
}

void print_prompt() { printf("db > "); } // creates the prompt for the repl environment

void read_input(InputBuffer* input_buffer) {
    ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);
    // &(input_buffer->buffer) memory address of the location containing the pointer to buffer stream
    // &(input_buffer->buffer_length) memory address of the pointer to the variable containing the size of the buffer
    // stdin: the input stream to be read from
    // this function will return the number of characters read from input stream
    // if error reading input, return -1
    // essentially this function allocates teh memory for the input buffer

    if (bytes_read <= 0) { 
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }

    // to ignore trailing newline in the input
    input_buffer->input_length = bytes_read - 1; // reduces the size of the input by 1 to ignore trailing newline character
    input_buffer->buffer[bytes_read-1] = 0; // assigns the null character to the newly calculated index of the last character to ensure that the string is properly terminated
}

void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer); // frees the pointer to the input buffer
    free(input_buffer); // empties the input buffer
}

// enum result codes are used wherever appropriate to handle different results of a function and as an alternative to exceptions

typedef enum {
    META_COMMAND_SUCCESS, // 0
    META_COMMAND_UNRECOGNIZED_COMMAND // 1
} MetaCommandResult;

typedef enum { PREPARE_SUCCESS, PREPARE_UNRECOGNIZED_STATEMENT } PrepareResult;

MetaCommandResult do_meta_command(InputBuffer* input_buffer) {
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        close_input_buffer(input_buffer);
        exit(EXIT_SUCCESS);
    }
    else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT} StatementType;

typedef struct {
    StatementType type;
} Statement;

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {
  if (strncmp(input_buffer->buffer,"insert", 6) == 0) { // we use strncmp here because "insert" statement is usually followed by data
    statement->type = STATEMENT_INSERT;
    return PREPARE_SUCCESS;
  }
  if (strcmp(input_buffer->buffer, "select") == 0) {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }

  return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement* statement) {
  switch (statement->type) {
    case (STATEMENT_INSERT):
      printf("This is where we would do an insert.\n");
      break;
    case (STATEMENT_SELECT):
      printf("This is where we would do a select.\n");
      break;
  }
}

int main(int argc, char* argv[]) {
  InputBuffer* input_buffer = new_input_buffer();
  while (true) { // infinite prompt loop
    print_prompt();
    read_input(input_buffer);

    if (input_buffer->buffer[0] == '.') {
    switch (do_meta_command(input_buffer)) {
      case (META_COMMAND_SUCCESS):
        continue;
      case (META_COMMAND_UNRECOGNIZED_COMMAND):
        printf("Unrecognized command '%s'\n", input_buffer->buffer);
        continue;
    }
    }

    Statement statement;
    switch (prepare_statement(input_buffer, &statement)) {
      case (PREPARE_SUCCESS):
        break;
      case (PREPARE_UNRECOGNIZED_STATEMENT):
        printf("Unrecognized keyword at start of '%s'.\n",
               input_buffer->buffer);
        continue;
    }

    execute_statement(&statement);
    printf("Executed.\n");
  }
}