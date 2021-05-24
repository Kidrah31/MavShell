/*
Name: Hardik Shukla
UTA ID: 1001664934
*/

// The MIT License (MIT)
// 
// Copyright (c) 2016, 2017, 2020 Trevor Bakker 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define WHITESPACE " \t\n"      // We want to split our command line up into tokens
                                // so we need to define what delimits our tokens.
                                // In this case  white space
                                // will separate the tokens on our command line

#define MAX_COMMAND_SIZE 255    // The maximum command-line size

#define MAX_NUM_ARGUMENTS 10     // Mav shell only supports five arguments
 /*
 Function to to list the PIDs of
the last 15 processes or less spawned by the shell. If there have been less than 15 processes
spawned then it shall print only those process PIDs 
*/
void showpids(int num,int pid_hist[])
{
  
  int i;
  for (i = 0; i < num; i++)
  {
    printf("%d: %d\n", (i + 1), pid_hist[i]);
  }

}
/*
 Function to to print the history of
the last 15 processes, it takes the history_index and 
the history[] as parameters
*/
void showhistory(int index,char *history[])
{
  int i;
  for(i=0;i<index;i++)
  {
    
      printf("%d. %s",i+1,history[i]);
  }
    
}
int main()
{

  char * cmd_str = (char*) malloc( MAX_COMMAND_SIZE );
  
  char *history[15];
  int j;
  //to malloc every element of the array of pointers,
  //great help from the lecture video segfaulted on this one
  for(j=0;j<15;j++)
    history[j] =(char*) malloc( 255);
    // to store the history index
  int history_index=0;
  //array to store the pids
  int pid_history[15];
  //to store the index of the pids
  int pid_index=0;
  while( 1 )
  {
    // Print out the msh prompt
    printf ("msh> ");

    // Read the command from the commandline.  The
    // maximum command that will be read is MAX_COMMAND_SIZE
    // This while command will wait here until the user
    // inputs something since fgets returns NULL when there
    // is no input
    while( !fgets (cmd_str, MAX_COMMAND_SIZE, stdin) );
    
    if(cmd_str[0]=='\n')
    continue;


    char *token[MAX_NUM_ARGUMENTS];

    int   token_count = 0;                                 
                                                           
    // Pointer to point to the token
    // parsed by strsep
    char *argument_ptr;                                         
                                                           
    char *working_str  = strdup( cmd_str );    
   /*  */
    // we are going to move the working_str pointer so
    // keep track of its original value so we can deallocate
    // the correct amount at the end


    //save the history here
    if(history_index>14)
    history_index=0;

    strcpy(history[history_index],cmd_str);            
 
    //replace the ! command here
   //to check for the !n
   /* !n, where n is a number between 1 and 15 will
    result in your shell re-running the nth command.
     If the nth command does not exist then
      your shell will state “Command not in history.”. */
    if(cmd_str[0] == '!' )
    {  
      // if((((int)cmd_str[1])-48)>history_index)this failed
       // we convert the parameter at cmd_str[1] to integer and 
       //compare with the value at history_index
     if(atoi(&cmd_str[1])-1>history_index)
      {
          
        printf("Command not in history.\n");
         continue;
      }
        //strcpy(working_str,history[(((int)cmd_str[1])-49)]);
        // copy the value of the retreived command from the history to run
      strcpy(working_str,history[atoi(&cmd_str[1])-1]);
    }
    
    

    char *working_root = working_str;

    // Tokenize the input stringswith whitespace used as the delimiter
    while ( ( (argument_ptr = strsep(&working_str, WHITESPACE ) ) != NULL) && 
              (token_count<MAX_NUM_ARGUMENTS))
    {
      token[token_count] = strndup( argument_ptr, MAX_COMMAND_SIZE );
      if( strlen( token[token_count] ) == 0 )
      {
        token[token_count] = NULL;
      }
      token_count++;
    }

    
    //if input is exit or quit then exit
    if(strcmp(token[0], "exit")==0|| strcmp(token[0], "quit")==0)
    {
      exit(0);
    }
    //else if input is cd then change directories
    else if(strcmp(token[0],"cd")==0)
    {
      chdir(token[1]);
      history_index++;
    }
     //else if user wants to display the pids
    else if (strcmp(token[0],"showpids")==0)
    {
      if(pid_index>15)
     pid_index=0;
        
      showpids(pid_index,pid_history);
      history_index++;
    }
    //else if user wants the history
    else  if(strcmp(token[0],"history")==0)
    {
      
      if(history_index>15)
       history_index=0;
      showhistory(history_index,history);
      history_index++;
    }
    /*if input is hisory then history

    //else if input is !
    //else
    //  fork
    //  if pid=0  (child)
    //      excvp
    //  else (you're in the parent)
    //  wait
    */
    else
    {
   
      int ret;
      int status;
      pid_t pid = fork( );
      if( pid == 0 )//child process
      {
        ret = execvp( token[0], &token[0]);  
        if(ret==-1)//if execvp fails the command doesn't exist
          printf("%s : Command not found\n",token[0]);
        
      }
      else if( pid == -1 )//error in fork
      {
        perror("fork");
        exit(0);
      }
      else if(pid >0)//parent process
      {
        
       if(ret!=-1)
        {
          if(pid_index>15)//if pid index exceeds 15
          pid_index=0;  
        
          pid_history[pid_index]=pid;//Update the pids in the array
          pid_index++;
         history_index++;

        }
        //blocks the calling process until one of 
        //its child processes exits or a signal is received.
       wait( & status );
       //clearing the output buffer before it becomes full
       fflush(NULL);
      }
    }
   
    free( working_root );

  }
  return 0;
}
