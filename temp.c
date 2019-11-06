#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

void swap(int *p1, int *p2){
    int z = *p1;
    *p1 = *p2;
    *p2 = z;
    
}

static void populateTable(int address, int frequency, int numDevice, int row, int col, int matrix[row][col]){
    int i = 0;
    bool populated = false;
    while (populated != true) {
        if (matrix[i][0] != 0) {
            i++;
        }
        else {
            matrix[i][0] = address;
            // printf("matrix [0][1] = %d\n", matrix[0][0]);
            if (i==0){
                matrix[i][1] = 0; // frame start
            }
            else {
                matrix[i][1] = matrix[i-1][2]+1;
            }

            matrix[i][2] = (3600/frequency)*(numDevice) + matrix[i][1]; // frame ends
            // printf("matrix [0][1] = %d\n", matrix[0][1]);

            matrix[i][3] = frequency;
            matrix[i][4] = numDevice;
            // printf("%d\n", matrix[0][0]); // not run
            // printf("%d\n", matrix[0][1]);
            populated = true;     
        }
    }
}

// return the local time minute (0-59)
int minTime(void){
    time_t current;
    time(&current); // necessary to get the correct time 
    struct tm* localTime = localtime(&current);
    return localTime->tm_min;
}

// return the local time minute (0-59)
int secTime(void){
    time_t current;
    time(&current); // necessary to get the correct time 
    struct tm* localTime = localtime(&current);
    return localTime->tm_sec;
}

// return the local time hour (0-23)
int hourTime(void){
    time_t current;
    time(&current); // necessary to get the correct time 
    struct tm* localTime = localtime(&current);
    return localTime->tm_hour;
}

int sumTime(void){
    time_t current;
    time(&current); // necessary to get the correct time 
    struct tm* localTime = localtime(&current);
    int secTime = localTime->tm_sec;
    int minTime = (localTime->tm_min * 60);
    int sumTime = (secTime + minTime);
    return sumTime;
}

//Design question: Should this function be running continuously? 

//For 1 hour interval refresh 
void write(int address, int counter, int row, int numDevice, int frameEnd){ //where address is matrix[i][0], and counter = matrix, row = i from frequency check
    int i = row;
    int N = 1 +  numDevice;      //matrix[i][4]; // N is timestamp + numDevices. How many rows of memory address it takes up per write. 
    int registerAddress;
    if (row == 0){
        registerAddress = (counter)*(N); // first device 
    }
    else {
        registerAddress = ((counter)*(N) + 1 + frameEnd);  //matrix[i-1][2]+1); // counter + frame ends of last device 
    }
    //writeData(registerAddress, timestamp); // We're just printing the statement for now. 
    printf("Register Number: %d, TimeStamp: %d:%d:%d\n", registerAddress, hourTime(), minTime(), secTime());
    registerAddress++;

    for (int i = 1; i < N; i++){
        // writeData(registerAddress+1, Value[i]);
        printf("Register Number: %d, Data: 1010\n", registerAddress);
        registerAddress++;
    }
   
}

void frequencyCheck(int row, int col, int matrix[row][col]){
    int timeSec = secTime();
    int timeMin = minTime();
    int combinedSum = sumTime();
    int numRow = sizeof(matrix[0])/sizeof(matrix[0][0]);
    //printf("%i", timeSec);
     for (int i = 0; i < 4; i++){ // numRow **Have to be careful since the entire table might not be fully intialize, and you get weird number values that crashes the run**
    //      if (combinedSum % matrix[i][3] == 0){
        if((combinedSum % matrix[i][3] == 0) || (timeSec == 0 && timeMin == 0)) { // make >=0 to test the print function. row 0 should always work. 
            printf("Run Write Function -> Writing to file for row[%i] \n", i);
            int counter = combinedSum / matrix[i][3];
            int numDevice = matrix[i][4];
            int frameEnd = 0;
            if (i != 0){
                frameEnd = matrix[i-1][2];
            }
            printf("Counter = %d\n", counter);
            write(matrix[i][0], counter, i, numDevice, frameEnd);
            //int counter = combinedSum/ matrix[i][3] (frequency) *the counter is VERY important. It keeps track of the position of where the data will be written to
            // say time is 5 min 24 sec. then combinedSum = (5x60)+24 = 324 seconds. 324/1sec = 324 seconds has passed. c
          }
     }
}


int main(void)
{   
    int x = 1;
    int y = 2;

    swap(&x,&y);  //result x = 2, y =1
    printf("%d\t%d\n",x,y);

    // git git git 
    // warmup exercise
    // int array[10];
    // for (int i = 0; i<10; i++)
    // {
    //     array[i] = 0;
    // }

    // array[1] = 5;
    // int value;
    // value = array[1];
    // int *pointer;
    // pointer = &array[0];
    // int value1 = *pointer;
    // int value2 = *(pointer +1);

    // for (int j = 0; j <10; j++) 
    // {
    //     printf("%d\n", array[j]);
    // }
        
    // printf("the value of array[1] is %d\n", value);
    // printf("the value of array[0] is %d\n", value1);
    // printf("the value of array[1] is %d\n", value2);


    // Initialize the table with 0s.
    int table [5][5];
    for (int i = 0; i <5; i++){
        for (int j = 0; j < 5; j++){
            table[i][j] = 0;
        }
    }
    // printf("%d", table[3][3]);

    // Populate the table 
    populateTable(1111, 1, 1, 5, 5, table);
    populateTable(2222, 3, 3, 5, 5, table);  //what happens if I exceed the table size?
    populateTable(2222, 5, 5, 5, 5, table); 
    populateTable(2222, 300, 10, 5, 5, table); 

    printf("[0][0] %d = address \n", table[0][0]);
    printf("[0][1] %d = start \n", table[0][1]);
    printf("[0][2] %d = end \n", table[0][2]);
    printf("[0][3] %d = frequency\n", table[0][3]);
    printf("[0][4] %d = numDevices \n", table[0][4]);
    printf("[1][0] %d = address \n", table[1][0]);
    printf("[1][1] %d = start \n", table[1][1]);
    printf("[1][2] %d = end \n", table[1][2]);
    printf("[1][3] %d = frequency\n", table[1][3]);
    printf("[1][4] %d = numDevices\n", table[1][4]);
    printf("[2][0] %d\n", table[2][0]);

    // time_t seconds;
    // seconds = time(NULL);
    // printf("%ld\n", seconds);
    // time_t t;
    // time(&t);
    // printf("%s", ctime(&t));

    time_t currentTime;
    time(&currentTime);
    struct tm *myTime = localtime(&currentTime);
    printf("seconds -> %i\n", myTime->tm_sec);
    printf("minutes -> %i\n", myTime->tm_min);
    // int minute = myTime->tm_min;
    // printf("%i", minute);
    int temp0;
    temp0 = minTime();
    int temp00 = secTime();
    int combinedTime = sumTime();
    printf("testing this minute fucntion %i\n", temp0);
    printf("testing this second fucntion %i\n", temp00);
    printf("testing this sum fucntion %i\n", combinedTime);

 
    frequencyCheck(5,5,table);

   // printf("full time is: %d:%d:%d\n", myTime->tm_hour, myTime->tm_min, myTime->tm_sec);  
   // printf("Git Comment Test");

    return 0;
    
}

//get step 1 - 4 fully working on VS Code 
//See how hard it will be to get it on Visual Studio
//Work on connect 
// watch video on struct/ pointers
