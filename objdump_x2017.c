#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>

int Binary_to_Int(int int_Number_instructions);
void reverse_string(char data[]);
void reverse_array(int arr[], int start, int end);
int Find_Element(char a, char b[2048]);
int Find_Element_index(char a, char b[2048]);

int main(int argc, char *argv[])
{
    //use for read integer form file
    int data[2048];
    char data2[2048];
    //The Line store binary informations
    char Line[2048];

    int binaryNumber = 0;
    int remainder;
    int i = 1;
    int data_length = 0;
    char zero[] = "0";

    int file_size = 0;
    FILE *fp;

    fp = fopen(argv[1],"rb");  // r for read, b for binary

    if (fp) {
        fseek(fp,0L,SEEK_END);
        file_size = ftell(fp);
    }
    else {
        printf("%d\n", errno);
        return 1;
    }

    //if((file_size*8)>256) {
    //    return 1;
    //}

    fseek(fp, 0, SEEK_SET);

    for (int k = 0; k < file_size; k ++) {

        data[k] = fgetc(fp);
        //printf("\\x");
        //printf("%02X",data[k]);

        binaryNumber = 0;
        remainder = 0;
        i = 1;

        while (data[k] != 0)
        {
            remainder = data[k] % 2;
            data[k] /= 2;
            binaryNumber += remainder * i;
            i *= 10;
        }

        //printf("binaryNumber: %d\n",binaryNumber);
        //int to string
        sprintf(data2, "%d", binaryNumber);
        //printf("data2: %s\n",data2);
        data_length = strlen(data2);
        //printf("data_length: %d\n",data_length);

        //add 0 at front 1 byte 8 bits
        if (data_length < 8)
        {
            int diff = 0;
            diff = 8 - data_length;

            for(int j = 0; j < diff; j ++) {
                // append zero to Line
                strcat(Line, zero);
                //printf("%s\n",Line);
            }
        }
        strcat(Line, data2);
    }

    //printf("%s\n",Line);

    reverse_string(Line);
    data_length = strlen(Line);

    char char_Opcodes[2048] = "";
    char First_Type[2048] = "";
    char First_Value_A[2048] = "";
    char Second_Type[2048] = "";
    char Second_Value_A[2048] = "";
    int First_Value_int[2048];
    int Second_Value_int[2048];
    int Func_Value_int[2048];

    while (data_length > 8){

        //printf("innital Line length: %d\n",data_length);
        //printf("innital Line: %s\n",Line);
        char beforeIns[2048] = "";
        int instructions = 0;
        int check_next = 3;
        char instruction_First_Value_A[2048] = "";
        char instruction_Second_Value_A[2048] = "";

        char str_Number_instructions[10] = "";

        for(int k = 0; k < 5; k ++) {
            //printf("%c",Line[k]);
            str_Number_instructions[k] = Line[k];

        }
        reverse_string(str_Number_instructions);

        //printf("str_Number_instructions: %s\n",str_Number_instructions);
        int int_Number_instructions = atoi(str_Number_instructions);
        //printf("int_Number_instructions1: %d\n",int_Number_instructions);

         //5
        int_Number_instructions = Binary_to_Int(int_Number_instructions);
        //printf("int_Number_instructions2: %d\n",int_Number_instructions);


        int j = 0;
        for(int k = 5; k < data_length; k++) {
            beforeIns[j]=Line[k];
            j++;
        }

        strcpy(Line, beforeIns);

        //printf("beforeIns Line: %s\n",Line);

        data_length = strlen(Line);

        //printf("data_length %d\n",data_length);

        while(instructions < int_Number_instructions) {

            check_next = 3;
            char current_Opcode[10] = "";
            char current_First_Typecode[10] = "";
            char First_Valuecode[10] = "";
            char current_Second_Typecode[10] = "";
            char Second_Valuecode[10] = "";

            //get Opcode

            for(int k = 0; k < check_next; k++) {
                current_Opcode[k] = Line[k];
            }

            //update Line, that remove current_Opcode
            char new_data[2048] = "";

            int p = 0;
            for(int k = check_next;k < data_length; k++) {
                new_data[p] = Line[k];
                p++;
            }

            //printf("old Line: %s\n",Line);

            strcpy(Line, new_data);

            data_length = strlen(Line);

            //010 RET

            if(strcmp(current_Opcode,"010") == 0) {
                //printf("same");
                check_next = 3;
                strcat(char_Opcodes, "R");
                strcat(First_Type, "~");
                strcat(instruction_First_Value_A, "~");
                strcat(Second_Type, "~");
                strcat(instruction_Second_Value_A, "~");

                int b = 0;
                int position = strlen(char_Opcodes)-1;
                //"R" in char_Opcodes so length is 1
                //set element in index 0
                First_Value_int[position] = b;
                Second_Value_int[position] = b;
                Func_Value_int[position] = b;

                instructions ++;
                continue;
            }

            //MOV
            else if(strcmp(current_Opcode, "000") == 0) {
                check_next = 2;
                strcat(char_Opcodes, "M");

            }
            //ADD
            else if(strcmp(current_Opcode, "001") == 0) {
                check_next = 2;
                strcat(char_Opcodes, "A");

            }
            //CAL
            else if(strcmp(current_Opcode, "100") == 0) {
                check_next = 2;
                strcat(char_Opcodes, "C");

            }
            //REF
            else if(strcmp(current_Opcode, "110") == 0) {
                check_next = 2;
                strcat(char_Opcodes, "F");

            }
            //PRINT
            else if(strcmp(current_Opcode, "101") == 0) {
                check_next = 2;
                strcat(char_Opcodes,"P");

            }
            //NOT
            else if(strcmp(current_Opcode, "011") == 0) {
                check_next = 2;
                strcat(char_Opcodes,"N");

            }
            //EQU
            else if(strcmp(current_Opcode, "111") == 0) {
                check_next = 2;
                strcat(char_Opcodes, "E");

            }

            int b = 0;
            int position = strlen(char_Opcodes)-1;
            Func_Value_int[position] = b;

            //2
            //printf("check_next: %d bits\n",check_next);

            for(int k = 0;k < check_next; k++) {
            current_First_Typecode[k] = Line[k];
                //printf("current_First_Typecode[k]: %c\n",current_First_Typecode[k]);
            }
            //printf("[%d]current_First_Typecode for
            //First Type: %s\n",instructions,current_First_Typecode);

            //update Line, that remove current_Opcode
            char new_data2[2048] = "";

            p = 0;
            for(int k = check_next;k < data_length; k++) {
                new_data2[p] = Line[k];
                p++;
            }

            //printf("old Line: %s\n",Line);
            strcpy(Line, new_data2);
            data_length = strlen(Line);

            //printf("data_length: %d\n",data_length);
            //printf("First_Typedata3: %s\n",Line);

            //STK
            if(strcmp(current_First_Typecode,"01") == 0) {
                check_next = 5;
                strcat(First_Type,"S");

            }

            //REG

            else if(strcmp(current_First_Typecode,"10") == 0) {
                check_next = 3;
                strcat(First_Type,"R");

            }

            //VAL
            else if(strcmp(current_First_Typecode,"00") == 0) {
                check_next = 8;
                strcat(First_Type,"V");

            }
            //PTR
            else if(strcmp(current_First_Typecode,"11") == 0) {
                check_next = 5;
                strcat(First_Type,"P");

            }

            //find first value
            //printf("check_next: %d bits\n",check_next);


            for(int k = 0; k < check_next; k++) {
                First_Valuecode[k] = Line[k];

            }
            //printf("First_Valuecode for First Type: %s\n",First_Valuecode);

            //update Line, that remove current_Opcode
            char new_data3[2048] = "";

            p = 0;
            for(int k = check_next;k < data_length; k++) {
                new_data3[p] = Line[k];
                p ++;
            }

            //printf("old Line: %s\n",Line);
            strcpy(Line, new_data3);
            data_length = strlen(Line);
            //printf("data_length: %d\n",data_length);
            //printf("First_Valuedata3: %s\n",Line);

            //reverse Valuecode
            reverse_string(First_Valuecode);
            //printf("First_Valuecode: %s\n",First_Valuecode);

            //stack symbol
            if (strcmp(current_First_Typecode, "01") == 0) {
                int int_First_Value = atoi(First_Valuecode);
                check_next = 2;
                int_First_Value = Binary_to_Int(int_First_Value);
                char First_Value_1[10] = "";
                //ASCII
                sprintf(First_Value_1, "%c", 65 + int_First_Value);
                strcat(instruction_First_Value_A, First_Value_1);

                int b = 0;
                int position = strlen(char_Opcodes) -1;
                First_Value_int[position] = b;
            }
            else if (strcmp(current_First_Typecode, "11") == 0) {
                int int_First_Value = atoi(First_Valuecode);
                check_next = 2;
                int_First_Value = Binary_to_Int(int_First_Value);
                char First_Value_1[10] = "";
                sprintf(First_Value_1, "%c", 65 + int_First_Value);
                strcat(instruction_First_Value_A, First_Value_1);

                int b = 0;
                int position = strlen(char_Opcodes) -1;
                First_Value_int[position] = b;
            }
            else{
                int int_First_Value = atoi(First_Valuecode);
                check_next = 2;
                int_First_Value = Binary_to_Int(int_First_Value);
                int position = strlen(char_Opcodes)- 1;
                First_Value_int[position] = int_First_Value;
                strcat(instruction_First_Value_A, "~");
            }

            //does not have second
            //CAL
            if (strcmp(current_Opcode,"100") == 0) {
                strcat(Second_Type,"~");
                strcat(instruction_Second_Value_A, "~");
                int b = 0;
                int position = strlen(char_Opcodes) -1;
                Second_Value_int[position] = b;
                check_next = 3;
                instructions ++;
                continue;
            }
            //PRINT
            else if (strcmp(current_Opcode,"101") == 0) {
                strcat(Second_Type,"~");
                strcat(instruction_Second_Value_A, "~");
                int b = 0;
                int position = strlen(char_Opcodes) -1;
                Second_Value_int[position] = b;
                check_next = 3;
                instructions ++;
                continue;
            }
            //NOT
            else if (strcmp(current_Opcode,"011") == 0) {
                strcat(Second_Type,"~");
                strcat(instruction_Second_Value_A, "~");
                int b = 0;
                int position = strlen(char_Opcodes) -1;
                Second_Value_int[position] = b;
                check_next = 3;
                instructions ++;
                continue;
            }
            //EQU
            else if (strcmp(current_Opcode,"111") == 0) {
                strcat(Second_Type,"~");
                strcat(instruction_Second_Value_A, "~");
                int b = 0;
                int position = strlen(char_Opcodes) -1;
                Second_Value_int[position] = b;
                check_next = 3;
                instructions ++;
                continue;
            }

            //find second type
            for(int k = 0; k < check_next; k++) {
                current_Second_Typecode[k] = Line[k];

            }
            //printf("[%d]current_Second_Typecode for First Type: %s\n"
            //,instructions,current_Second_Typecode);

            //update Line, that remove current_Opcode
            char new_data4[2048] = "";

            p = 0;
            for(int k = check_next; k < data_length; k++) {
                new_data4[p] = Line[k];
                p++;
            }

            //printf("old Line: %s\n",Line);
            strcpy(Line,new_data4);
            data_length=strlen(Line);

            //STK
            if(strcmp(current_Second_Typecode, "01") == 0) {
                check_next = 5;
                strcat(Second_Type, "S");

            }

            //REG

            else if(strcmp(current_Second_Typecode, "10") == 0) {
                check_next = 3;
                strcat(Second_Type, "R");

            }
            //VAL
            else if(strcmp(current_Second_Typecode, "00") == 0) {
                check_next = 8;
                strcat(Second_Type, "V");

            }
            //PTR
            else if(strcmp(current_Second_Typecode, "11") == 0) {
                check_next = 5;
                strcat(Second_Type, "P");

            }

            //find second value

            //printf("check_next: %d bits\n",check_next);

            for(int k = 0;k < check_next; k++) {
                Second_Valuecode[k] = Line[k];

            }
            //printf("Second_Valuecode for Second Type: %s\n",Second_Valuecode);

            //update Line, that remove current_Opcode
            char new_data5[2048] = "";

            p = 0;
            for(int k = check_next; k < data_length; k++) {
                new_data5[p] = Line[k];
                p++;
            }

            //printf("old Line: %s\n",Line);
            strcpy(Line, new_data5);
            data_length=strlen(Line);

            reverse_string(Second_Valuecode);

            //stack symbol
            if(strcmp(current_Second_Typecode, "01") == 0) {
                int int_Second_Value = atoi(Second_Valuecode);
                check_next = 3;
                int_Second_Value = Binary_to_Int(int_Second_Value);
                char Second_Value_1[10] = "";
                sprintf(Second_Value_1, "%c", 65 + int_Second_Value);
                strcat(instruction_Second_Value_A, Second_Value_1);
                int b = 0;
                int position = strlen(char_Opcodes) -1;
                Second_Value_int[position] = b;
            }
            else if(strcmp(current_Second_Typecode,"11") == 0) {
                int int_Second_Value = atoi(Second_Valuecode);
                check_next = 3;
                int_Second_Value = Binary_to_Int(int_Second_Value);
                char Second_Value_1[10] = "";
                sprintf(Second_Value_1, "%c", 65 + int_Second_Value);
                strcat(instruction_Second_Value_A, Second_Value_1);
                int b = 0;
                int position = strlen(char_Opcodes) -1;
                Second_Value_int[position] = b;

            }
            else{
                int int_Second_Value = atoi(Second_Valuecode);
                check_next = 3;
                int_Second_Value = Binary_to_Int(int_Second_Value);
                int position = strlen(char_Opcodes) -1;
                Second_Value_int[position] = int_Second_Value;
                strcat(instruction_Second_Value_A, "~");

            }

            instructions ++;
        }

        //FUNC LABEL

        char labe_code[10] = "";
        for(int k = 0; k < 3; k++) {
                labe_code[k] = Line[k];
        }
        //printf("labe_code: %s\n",labe_code);

        //update Line, that remove labe_code
        char new_data[2048]="";

        int p = 0;
        for(int k = 3; k < data_length; k++) {
            new_data[p] = Line[k];
            p++;
        }

        //printf("old Line: %s\n",Line);
        strcpy(Line,new_data);

        data_length = strlen(Line);
        int b = 0;
        //here has not give char_Opcodes new value so not -1
        int position = strlen(char_Opcodes);

        reverse_string(labe_code);
        int int_labe_code_Value = atoi(labe_code);
        int_labe_code_Value = Binary_to_Int(int_labe_code_Value);
        Func_Value_int[position] = int_labe_code_Value;

        strcat(char_Opcodes, "~");
        strcat(First_Type, "~");
        strcat(instruction_First_Value_A, "~");
        strcat(Second_Type, "~");
        strcat(instruction_Second_Value_A, "~");

        First_Value_int[position] = b;
        Second_Value_int[position] = b;
        check_next = 3;
        //printf("final data3 length: %d\n",data_length);
        //printf("final data3: %s\n",data3);

        //"~~KKPKAAC~" -> ~~AABACCD~

        reverse_string(instruction_First_Value_A);
        reverse_string(instruction_Second_Value_A);

        char First_Value_A_2[2048] = "";
        char Second_Value_A_2[2048] = "";
        int index = 0;
        char empty[1] = "~";
        char check1[2048] = "";
        strcat(check1, "~");
        char check2[2048] = "";
        strcat(check2, "~");

        for(int i = 0; i < strlen(instruction_First_Value_A); i++) {
            if (instruction_First_Value_A[i]==empty[0]) {
                First_Value_A_2[i]=instruction_First_Value_A[i];
            }
            else {

                int find = Find_Element(instruction_First_Value_A[i], check1);
                if(find == 1){
                    char A[2] = "";
                    int index2 = Find_Element_index(instruction_First_Value_A[i], check1);
                    sprintf(A, "%c", 65 + index2 -1);
                    First_Value_A_2[i] = A[0];
                }
                else {
                    index++;
                    check1[index] = instruction_First_Value_A[i];
                    char A[2] = "";
                    sprintf(A, "%c", 65 + index -1);
                    First_Value_A_2[i] = A[0];
                }
            }
        }

        index = 0;

        for(int i = 0; i < strlen(instruction_Second_Value_A); i++) {
            if (instruction_Second_Value_A[i] == empty[0]) {
                Second_Value_A_2[i] = instruction_Second_Value_A[i];
            }
            else {

                int find = Find_Element(instruction_Second_Value_A[i], check2);
                if(find == 1){
                    char A[2] = "";
                    int index2 = Find_Element_index(instruction_Second_Value_A[i], check2);
                    sprintf(A, "%c", 65 + index2 -1);
                    Second_Value_A_2[i]=A[0];
                }
                else {
                    index++;
                    check2[index] = instruction_Second_Value_A[i];
                    char A[2] = "";
                    sprintf(A, "%c", 65 + index -1);
                    Second_Value_A_2[i] = A[0];
                }
            }
        }

        index = 0;

        reverse_string(First_Value_A_2);
        strcat(First_Value_A, First_Value_A_2);
        reverse_string(Second_Value_A_2);
        strcat(Second_Value_A, Second_Value_A_2);

    }

    //reverse char_Opcodes First_Type Second_Type ..
    reverse_string(char_Opcodes);
    reverse_string(First_Type);
    reverse_string(First_Value_A);
    reverse_string(Second_Type);
    reverse_string(Second_Value_A);
    reverse_array(First_Value_int, 0, strlen(char_Opcodes) -1);
    reverse_array(Second_Value_int, 0, strlen(char_Opcodes) -1);
    reverse_array(Func_Value_int, 0, strlen(char_Opcodes) -1);

    /*
    //printf("final Line: %s\n", Line);
    printf("char_Opcodes: %s\n", char_Opcodes);
    printf("First_Type: %s\n", First_Type);
    printf("First_Value_A: %s\n", First_Value_A);
    printf("Second_Type: %s\n", Second_Type);
    printf("Second_Value_A: %s\n", Second_Value_A);

    for(int k = 0; k < strlen(char_Opcodes); k++){
        printf("%d ", First_Value_int[k]);
    }
    printf("\n");
    for(int k = 0; k < strlen(char_Opcodes); k++){
        printf("%d ", Second_Value_int[k]);
    }
    printf("\n");
    printf("Func_Value_int: \n");
    for(int k = 0; k < strlen(char_Opcodes); k++){
        printf("%d ", Func_Value_int[k]);
    }
    printf("\n");
    */

    for(int k = 0;k<strlen(char_Opcodes);k++){

        char check_Opcodes_M[1] = "M";
        char check_Opcodes_A[1] = "A";
        char check_Opcodes_R[1] = "R";
        char check_Opcodes_C[1] = "C";
        char check_Opcodes_F[1] = "F";
        char check_Opcodes_P[1] = "P";
        char check_Opcodes_N[1] = "N";
        char check_Opcodes_E[1] = "E";
        char check_type_R[1] = "R";
        char check_type_S[1] = "S";
        char check_type_V[1] = "V";
        char check_type_P[1] = "P";
        char empty[1] = "~";


        //Opcodes
        if(char_Opcodes[k] == check_Opcodes_M[0]) {
            printf("    MOV ");
        }
        else if(char_Opcodes[k] == check_Opcodes_A[0]) {
            printf("    ADD ");
        }
        else if (char_Opcodes[k] == check_Opcodes_R[0]) {
            printf("    RET\n");
            continue;
        }
        else if (char_Opcodes[k] == check_Opcodes_C[0]) {
            printf("    CAL ");
        }
        else if (char_Opcodes[k] == check_Opcodes_F[0]) {
            printf("    REF ");
        }
        else if (char_Opcodes[k] == check_Opcodes_P[0]) {
            printf("    PRINT ");
        }
        else if (char_Opcodes[k] == check_Opcodes_N[0]) {
            printf("    NOT ");
        }
        else if (char_Opcodes[k] == check_Opcodes_E[0]) {
            printf("    EQU ");
        }
        else if (char_Opcodes[k] == empty[0]) {
            printf("FUNC LABEL %d\n",Func_Value_int[k]);
            continue;
        }

        //First_Type
        if (First_Type[k] == check_type_R[0]) {
            printf("REG ");
        }
        else if (First_Type[k] == check_type_S[0]){
            printf("STK ");
        }
        else if (First_Type[k] == check_type_V[0]) {
            printf("VAL ");
        }
        else if (First_Type[k] == check_type_P[0]) {
            printf("PTR ");
        }
        else if (First_Type[k] == empty[0]) {

        }

        //First_Value

        // 0MMAMR
        // OOOOAO
        // 0,0,1,0,0,0
        if(First_Value_A[k] == empty[0]) {

            if ((char_Opcodes[k] == check_Opcodes_C[0]) ||
            (char_Opcodes[k] == check_Opcodes_P[0]) ||
            (char_Opcodes[k] == check_Opcodes_N[0]) ||
            (char_Opcodes[k] == check_Opcodes_E[0])) {

                printf("%d", First_Value_int[k]);
            }
            else {

                printf("%d ", First_Value_int[k]);

            }

        }
        else {

            if ((char_Opcodes[k] == check_Opcodes_C[0]) ||
            (char_Opcodes[k] == check_Opcodes_P[0]) ||
            (char_Opcodes[k] == check_Opcodes_N[0]) ||
            (char_Opcodes[k] == check_Opcodes_E[0])) {

                printf("%c", First_Value_A[k]);
            }
            else {

                printf("%c ", First_Value_A[k]);

            }

        }

        //Second_Type
        if(Second_Type[k] == check_type_R[0]) {
            printf("REG ");
        }
        else if(Second_Type[k] == check_type_S[0]){
            printf("STK ");
        }
        else if(Second_Type[k] == check_type_V[0]) {
            printf("VAL ");
        }
        else if(Second_Type[k] == check_type_P[0]) {
            printf("PTR ");
        }
        else if(Second_Type[k] == empty[0]) {
            printf("\n");
        }

        if(Second_Type[k] != empty[0]) {
            //Second_Value
            if(Second_Value_A[k] == empty[0]) {
                printf("%d", Second_Value_int[k]);
                printf("\n");
            }
            else {
                printf("%c", Second_Value_A[k]);
                printf("\n");
            }
        }

    }

    fclose(fp);
    return 0;
}

int Binary_to_Int(int binary)
{
    int integer = 0;
    int i = 0;
    int remainder;
    while (binary != 0)
    {
        remainder = binary % 10;
        binary = binary / 10;
        remainder = remainder * pow(2, i);
        integer = integer + remainder;
        i++;
    }
    return integer;
}

void reverse_string(char data[])
{
    int data_length = strlen(data);

    for (int i = 0; i < data_length / 2; i++)
    {
        char a = data[i];
        data[i] = data[data_length - i - 1];
        data[data_length - i - 1] = a;
    }
}

void reverse_array(int arr[], int start, int end)
{
    int i;
    while (start < end)
    {
        i = arr[start];
        arr[start] = arr[end];
        arr[end] = i;
        start++;
        end--;
    }
}


int Find_Element(char a, char b[2048]){
    int find = 0;
    for (int i = 0; i < strlen(b); i++) {
        if (b[i] == a){
            find = 1;
        }
    }
    return find;

}

int Find_Element_index(char a, char b[2048]){
    int index = 0;
    for (int i = 0; i <strlen(b); i++) {
        if (b[i] == a){
            index = i;
            break;
        }
    }
    return index;
}