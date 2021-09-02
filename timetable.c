#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int compare_strings(char *target, char *source);
int string_length(char* str);
void usertimeinvalidmessage();

int main(int argc, char *argv[])
{

    //check Not enough arguments

    if(argc < 4) {
        printf("Please provide <source> <destination> <time> as command line arguments\n");
        return 0;
    }

    //why not just 4096?
    //4096 + '\n' =4097
    //if a line >4097(+\n)
    //we can find it by its length

    char line[5000];

    char data[4096];
    char data2[3];

    //splite user input
    char *pUser_source = NULL;
    char *pUser_destination = NULL;
    char *pUser_time = NULL;
    char *pUser_hh = NULL;
    char *pUser_mm = NULL;
    char *pUser_ss = NULL;
    int int_user_hh = 0;
    int int_user_mm  = 0;
    int int_user_ss  = 0;

    //finally if count = 0; No count_suitable trains can be found
    int count_suitable=0;
    int count_line=0;
    //if int_user_hh>int_line_hh, record this hh as last hh
    int int_line_lasthh=0;


    //get argv[1] source

    pUser_source = data;
    pUser_source = argv[1];
    //printf("pUser_source is %s\n",pUser_source);

    //get argv[2] destination

    pUser_destination = data;
    pUser_destination = argv[2];
    //printf("pUser_destination is %s\n",pUser_destination);

    //get argv[3] time

    pUser_time = data;
    pUser_time = argv[3];
    //printf("pUser_time is %s\n",pUser_time);

    //time must be number
    if (!isdigit(pUser_time[0]) || !isdigit(pUser_time[1]) ||
    !isdigit(pUser_time[3]) || !isdigit(pUser_time[4]) ||
    !isdigit(pUser_time[6]) || !isdigit(pUser_time[7])) {

        usertimeinvalidmessage();
        return 0;
    }

    //get hh of time
    pUser_hh = data2;
    pUser_hh[0] = pUser_time[0];
    pUser_hh[1] = pUser_time[1];
    //printf("pUser_hh %s\n", pUser_hh);
    int_user_hh = atoi(pUser_hh);
    //printf("int_user_hh %d\n", int_user_hh);

    //get mm of time
    pUser_mm = data2;
    pUser_mm[0] = pUser_time[3];
    pUser_mm[1] = pUser_time[4];
    //printf("pUser_mm %s\n", pUser_mm);
    int_user_mm=atoi(pUser_mm);
    //printf("int_user_mm %d\n", int_user_mm);

    //get ss of time
    pUser_ss = data2;
    pUser_ss[0] = pUser_time[6];
    pUser_ss[1] = pUser_time[7];
    //printf("pUser_ss %s\n", pUser_ss);
    int_user_ss = atoi(pUser_ss);
    //printf("int_user_ss %d\n", int_user_ss);

    //check time is invalid
    if ((int_user_hh>23 || int_user_hh < 0) || (int_user_mm > 59 || int_user_mm < 0)
    || (int_user_ss > 59 || int_user_ss < 0) || (string_length(pUser_time) != 8)) {

        usertimeinvalidmessage();
        return 0;

    }

    //read whole list until find suitable trains or last line
    //fgets n-1 elemment
    //what is a enty bigger than 4097 with \n?
    //4097(no \n here)+Melbourpppne::Sydney::18:33:12
    //before 0 - 4096 will be sikp,
    //next element as another line
    //[...4096...]\n[...4096...]\n[...4096...]
    while(fgets(line,5000,stdin) != NULL) {

        char delim[] = "::";
        int count_delim = 0;
        int line_length = 0;
        char line_source[4096] = "";
        char line_destination[4096] = "";
        char line_hh[3] = "";
        char line_mm[3] = "";
        char line_ss[3] = "";

        //int line_hh_len = 0;
        //int line_mm_len = 0;
        //int line_ss_len = 0;

        int int_line_hh = 0;
        int int_line_mm = 0;
        int int_line_ss = 0;
        char empty[] = "";
        char enter[] = "\n";
        char space[] = " ";

        //eg.Sydney::Melbourne::07:00:00
        //printf("pre check line :%s\n", line);

        line_length=string_length(line);

        //printf("line_length:%d\n", line_length);

        //line never > 4097
        if (line_length > 4097) {

            continue;
        }

        //[...4096...]\n[...4096...]\n[...4096...]
        //so check 4097 element is '\n' or empty for all line >= 4097
        //in last line 4096 length 4096, others 4096 length 4096 + 1 = 4097
        //in last line 4097 length 4097, others 4096 length 4097 + 1 = 4098
        if (line_length == 4097) {

            //elements start at 0
            if (line[4096] != empty[0] && line[4096] != enter[0]) {
                continue;

            }

        }

        //number of ":" must == 6
        for (int i = 0; i < line_length; i++) {
            if(line[i] == delim[0])
            {
                count_delim ++;
            }

        }

        if (count_delim != 6) {

            continue;
        }

        //for missing source
        //the first element can not be ":" or " "
        if (line[0] == delim[0] || line[0] == space[0]) {
            continue;
        }

        //printf("accept line 1:%s\n",line);
        //the i is the position of first ":"
        int i = 0;
        for(i = 0;i < line_length; i++) {
            //find the first ":" then break
            if(line[i] == delim[0])
            {
                break;
            }
            else
            {
                //store all elements before ":"

                line_source[i] = line[i];

            }

        }
        //printf("line_source: %s\n", line_source);
        //printf("i: %d\n", i);

        //check the next element of line is ":"
        //if not continue;
        i = i + 1;

        if ( line[i] != delim[0] ) {
            continue;
        }

        //if i is ":"then go to next element
        i = i + 1;
        //after this "::" we will have destination
        //if this element also is ":" or space continue
        if ( line[i] == delim[0] || line[i] == space[0] ) {
            continue;
        }


        //use k instead of i
        int k = i;

        for(k = i;k < line_length; k++) {
            if(line[k] == delim[0]) {
                //break for loop
                break;
            }
            else
            {
                //store all elements after second ":"

                line_destination[k-i] = line[k];
            }

        }

        //printf("line_destination: %s\n", line_destination);
        //printf("k: %d\n",k);

        k = k + 1;
        //check the next element of line is ":"
        //if not continue;
        if ( line[k] != delim[0] ) {
            continue;
        }
        //if k is ":"then go to next element
        k = k + 1;

        //no need to check ":" occupy hh's position
        //just check time elements
        //find hh
        line_hh[0] = line[k];
        line_hh[1] = line[k+1];
        int_line_hh = atoi(line_hh);
        //line_hh_len = string_length(line_hh);
        //printf("char line_hh: %s\n", line_hh);
        //printf("line_hh_len: %d\n", line_hh_len);
        //printf("int int_line_hh: %d\n", int_line_hh);

        k = k + 2;
        //check the next element of line is ":"
        //if not continue;
        if ( line[k] != delim[0] ) {
            continue;
        }

        //find mm
        k = k + 1;
        line_mm[0] = line[k];
        line_mm[1] = line[k+1];
        int_line_mm = atoi(line_mm);
        //line_mm_len = string_length(line_mm);
        //printf("char line_mm: %s\n", line_mm);
        //printf("line_mm_len: %d\n", line_mm_len);
        //printf("int int_line_mm: %d\n",int_line_mm);

        k = k + 2;
        //check the next element of line is ":"
        //if not continue;
        if ( line[k] != delim[0] ) {
            continue;
        }

        k = k + 1;
        line_ss[0] = line[k];
        line_ss[1] = line[k+1];
        int_line_ss = atoi(line_ss);
        //line_ss_len = string_length(line_ss);
        //printf("char line_ss: %s\n", line_ss);
        //printf("line_ss_len: %d\n", line_ss_len);
        //printf("int int_line_ss: %d\n",int_line_ss);


        k=k+2;

        //the next element after ss should be \n or empty
        if (line[k] != empty[0]&&line[k] != enter[0]){
            continue;
        }


        //printf("accept line 2: %s\n", line);

        if (((line_hh[0] >= '0' && line_hh[0] <= '2') &&
        (line_hh[1] >= '0' && line_hh[1] <= '3')) ||
        ((line_hh[0] >= '0' && line_hh[0] <='1') &&
        (line_hh[1] >= '0' && line_hh[1] <= '9'))) {
            //the two element of line_hh must be number, else skip this line
            //also < 24
        }
        else {
            continue;
        }

        if ((line_mm[0] >= '0' && line_mm[0] <= '5') &&
        (line_mm[1] >= '0' && line_mm[1] <= '9')) {
            //the two element of line_mm must be number, else skip this line
            //also <60
        }
        else {
            continue;
        }

        if ((line_ss[0] >= '0' && line_ss[0] <= '5') &&
        (line_ss[1] >= '0' && line_ss[1] <= '9')) {
            //the two element of line_ss must be number, else skip this line
            //also <60
        }
        else {
            continue;
        }


        //printf("%s\n",line);
        count_line++;

        //source and destination check
        //in each loop we compare the user input and the current line of the list
        //some unsorted data will be automatically ignoe,
        //eg 1:00:00 after 0:30:00 in next day
        //for missing source and destination,

        if (compare_strings(pUser_source, line_source) == 0 &&
        compare_strings(pUser_destination, line_destination) == 0) {

            //printf("last hh: %d\n",int_line_lasthh);
            //time check
            //07:05:10 and 08:05:10
            if (int_user_hh < int_line_hh) {
                //printf("int_user_hh<int_line_hh\n");
                count_suitable = 1;

                printf("The next train to %s from %s departs at %s:%s:%s\n",
                line_destination, line_source, line_hh, line_mm, line_ss);

                //end the program
                return 0;
            }
            //08:04:10 and 08:05:10
            else if (int_user_hh == int_line_hh && int_user_mm < int_line_mm)
            {
                //printf("int_user_hh==int_line_hh&&int_user_mm<int_line_mm\n");
                count_suitable = 1;

                printf("The next train to %s from %s departs at %s:%s:%s\n",
                line_destination, line_source, line_hh, line_mm, line_ss);

                return 0;
            }
            //08:05:00 and 08:05:10
            else if (int_user_hh == int_line_hh && int_user_mm == int_line_mm
            && int_user_ss < int_line_ss) {

                //printf("int_user_hh==int_line_hh&&int_user_mm==
                //int_line_mm&&int_user_ss<int_line_ss\n");
                count_suitable = 1;

                printf("The next train to %s from %s departs at %s:%s:%s\n",
                line_destination, line_source, line_hh, line_mm, line_ss);

                return 0;

            }
            //09:30:50 and 09:30:45, but we have 11:45:30
            else if (int_user_hh == int_line_hh && int_user_mm ==
            int_line_mm && int_user_ss > int_line_ss) {

                int_line_lasthh = int_line_hh;
                //go to the next line of list
                //printf("int_line_lasthh1: %d",int_line_lasthh);

            }
            //09:30:50 and 09:33:45, but we have 11:45:30
            else if (int_user_hh == int_line_hh && int_user_mm > int_line_mm) {
                int_line_lasthh = int_line_hh;
                //go to the next line of list
                //printf("int_line_lasthh2: %d",int_line_lasthh);

            }
            //10:30:50 and 09:33:45, but we have 11:45:30;
            //also 23:00:00 and 20:00:00 but 00:30:00
            else if (int_user_hh > int_line_hh) {
            //go to the next line of list
            //last hh =0, user hh =23,current list hh =22,last hh->22
            //last hh =22, user hh =23,current list hh =01,
            //last hh>current list hh, so pass
            //what if user hh =01? the top one is closer time

                if (int_line_lasthh > int_line_hh) {
                    count_suitable = 1;

                    printf("The next train to %s from %s departs at %s:%s:%s\n",
                    line_destination, line_source, line_hh, line_mm, line_ss);

                    return 0;

                }
                else {
                        int_line_lasthh =int_line_hh;
                }
            }

            else {
                //no time is suitble, but we can check next line

            }//time check end

        }
        else {
            //in this line the source and destination is not suitable
            //go to the next line of list


        }//sourse destination check end


    }//while end

    if(count_line == 0) {
        //file is empty
        printf("No timetable to process\n");
        return 0;
    }

     //check if no suitble train
    if (count_suitable == 0) {
        //for No suitable source and destination
        //printf("count_suitable: %d\n",count_suitable);
        printf("No suitable trains can be found\n");
        return 0;
    }


return 0;
}

int compare_strings(char *target, char *source)
{
    int value = 0;
    int target_length = string_length(target);
    //printf("target_length: %d\n",target_length);
    int source_length = string_length(source);
    //printf("source_length: %d\n",source_length);

    if (target_length != source_length) {
        value=-1;
    }
    else {
        for (int i = 0; i < target_length; i++) {
            if(target[i] != source[i]) {
                value = -1;
            }
        }
    }

    return value;
}

int string_length(char *str)
{
    int length = 0;

    while(str[length] != '\0') {
        length ++;

    }
    return length;
}

void usertimeinvalidmessage()
{
    printf("<time> is invalid\n");
    printf("The time format must be supplied in 24 hour ");
    printf("format and in the following format:\n");
    printf("<hh>:<mm>:<ss>\n");
    printf("where hh represents the hour, mm represents ");
    printf("the minutes and ss , the number of seconds.\n");

}