/* Preprocessor directives */
#include <stdio.h> // printf
#include <iostream> // cin, cout, endl
#include <stdlib.h> // random number generation, atoi
#include <unistd.h> // pipe
#include <time.h> // time() function

using namespace::std; // using the standard namespace

/* Functions */
/*****************************************************************************/
/* Name: main()                                                              */
/* Description: Main loop of execution, where the child threads are created,*/
/*               executed, and terminated                                    */
/*****************************************************************************/
int main(int argc, char* argv[])
{
  int chPID; // process ID for the child process
  int pipeID[2];
  time_t run_time, current_time, end_time;

  struct stock
  {
	char symbol1;  // the first character of symbol
	char symbol2;  // the second character of symbol
	char symbol3;  // the third character of symbol
	int price;     // the stock price
  }stk;

  pipe(pipeID);

/* Read user arguments */
  if( argc != 2 )  // expect 2 string elements in argv[]
  {		     //  else print correct usage
	printf("ERROR: Usage: <*.exe> <seconds to run program>\n");
exit(0);    
  }
  run_time = (time_t)atoi(argv[1]);
  current_time = time(0);
/* Calculate run time */
  end_time = current_time + run_time;

  srand(time(0)); //seeding

/* Create a process */
  chPID = fork();
  if(chPID < 0)  // failed to fork
  {
    printf("Failed to fork\n");
    exit(1);  // throw exception
  }
  else if(chPID == 0)  // Code executed by the child process 
  {
    close(pipeID[1]); // child processes closes write
                      // side of pipe

	while(current_time < end_time )
	{
		read(pipeID[0], &stk.symbol1, sizeof(stk.symbol1));
		read(pipeID[0], &stk.symbol2, sizeof(stk.symbol2));
		read(pipeID[0], &stk.symbol3, sizeof(stk.symbol3));
		read(pipeID[0], &stk.price, sizeof(stk.price));
		/* process the message in childBuf */
printf("CONSUME: %c%c%c\t$%i\n",stk.symbol1,stk.symbol2,
        stk.symbol3,stk.price);
		current_time = time(0);
	}

  }  
  else  // Code executed by the parent process
  {
    close(pipeID[0]); // parent process closes read
                      // side of pipe

	while(current_time < end_time)
	{  
		/* create a message to send to child process */
		stk.symbol1 = (char)((rand() % 26) + 65); // pick a number between 0 and 25
		stk.symbol2 = (char)((rand() % 26) + 65); // the ASCII code for "A" is 65,
		stk.symbol3 = (char)((rand() % 26) + 65); // so add 65 to the random number
		                                     	 // to get a lower case letter
	          	                                   // then convert it to a char type
		stk.price = rand() % 100 + 1; // pick a number between 1 to 100	   			/* create a message to send to child process */
		write(pipeID[1], &stk.symbol1, sizeof(stk.symbol1));
		write(pipeID[1], &stk.symbol2, sizeof(stk.symbol2));
	   	write(pipeID[1], &stk.symbol3, sizeof(stk.symbol3));
	   	write(pipeID[1], &stk.price, sizeof(stk.price));
printf("PRODUCE: %c%c%c\t$%i\n",stk.symbol1,stk.symbol2,
        stk.symbol3,stk.price);
	   	current_time = time(0);
		
	} 
 }

/* Code executed by both parent and child process */ 
  return 0;
}
