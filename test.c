




typedef struct
{
    char* buffer;
    size_t buffer_length ; // defining the buffer length
    ssize_t input_length; 
} InputBuffer;

int main(int argc, char* argv[]) {
    InputBuffer* input_buffer = new_input_buffer(); // defining an input buffer to get a line of input
    while(true) // infinite prompt loop
    {
        print_prompt(); // user defined print function that prints the prompt
        read_input(input_buffer); // reads the user input from the input buffer

        if (strcmp(input_buffer->buffer, ".exit") == 0)
        {
            close_input_buffer(input_buffer); // function to exit the loop
            exit(EXIT_SUCCESS);
        }
        else
        {
            printf("Unrecognized command '%s'.\n", input_buffer->buffer);
        }
    }
}

