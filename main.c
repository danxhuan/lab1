#include "8.h"
#include "8.c"


int main() {
    String filename;
    error_msg errorMsg = create_string(&filename, "");
    if(errorMsg){
        return print_error(errorMsg);
    }

    printf("Write name of input file: ");
    read_string(stdin, &filename);
    FILE * in = fopen(filename.arr, "r");
    if(!in){
        destroy_string(&filename);
        return print_error(INPUT_FILE_ERROR);
    }
    errorMsg = read_file_with_instruction(in, stdout);
    if(errorMsg){
        fclose(in);
        destroy_string(&filename);
        return print_error(errorMsg);
    }
    destroy_string(&filename);

    fclose(in);
}