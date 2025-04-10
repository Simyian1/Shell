



#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "shell.c"


char **tokenize(char *line);
// ============================================================================

// Execute a child process.  
// Returns -1
// on failure.  On success, does not return to caller.
// ============================================================================
int child(char **args)
{
  int i = 0;
  while (args[i] != NULL)
  {
    if (equal(args[i], ">"))
    {
     // open the corresponding file and use dup to direct stdout to file
    }
    else if (equal(args[i], "<"))
    {
     // open the corresponding file and use dup to pull stdin from file
    }
    else if (equal(args[i], "|"))
    { 
     // do pipe in a separate function
    }
    else
    {
      ++i;
    }
  }

  // call execvp on prepared arguments after while loop ends. You can modify arguments as you loop over the arguments above.
  execvp(args[0], args);
  return -1;
}

// ============================================================================
// Execute the shell command that starts at args[start] and ends at args[end].
// For example, with
//   args = {"ls" "-l", ">", "junk", "&", "cat", "hello.c"}
// start = 5 and end = 6, we would perform the command "cat hello.c"
//
// Or, with args = {"ls", "|", "wc"} and start = 0 and end = 2, we would
// execute: ls | wc
// ============================================================================
void doCommand(char **args, int start, int end, bool waitfor)
{
// you will have your classic fork() like implementation here. 
// always execute your commands in child. so pass in arguments there 
// based on waitfor flag, in parent implement wait or not wait  based on & or ;  

}

// ============================================================================
// Execute the two commands in 'args' connected by a pipe at args[pipei].
// For example, with
//   args = {"ls" "-a", "|", "wc""}
// and pipei = 2, we will perform the command "ls -a | wc", pipei is the index of the pipe, so you can split commands between parent and child.
//
// We split off the command for the parent from 'args' into 'parentArgs'.  In
// our example, parentArgs = {"ls", "-a"}
//
// The parent will write, via a pipe, to the child
// ============================================================================
int doPipe(char **args, int pipei)
{

}



// ============================================================================
// Main loop for our Unix shell interpreter
// ============================================================================
int main()
{
  bool should_run = true;          // loop until false
  char *line = calloc(1, MAXLINE); // holds user input

  int start = 0; // index into args array
  while (should_run)
  {
    printf(PROMPT);   // osh>
    fflush(stdout);   // because no "\n"
    fetchline(&line); // fetch NUL-terminated line

    if (equal(line, ""))
      continue; // blank line

    if (equal(line, "exit"))
    { // cheerio
      should_run = false;
      continue;
    }

    if (equal(line, "!!"))
    {
      // gethistory
    }
    
    // process lines
    char **args = tokenize(line); // split string into tokens
    /*
    // loop over to find chunk of independent commands and execute
    while (args[start] != NULL)
    {
      int end;
      bool waitfor = parse(...)// parse() checks if current command ends with ";" or "&"  or nothing. if it does not end with anything treat it as ; or blocking call. Parse updates "end" to the index of the last token before ; or & or simply nothing
      doCommand(args, start, end, waitfor);    // execute sub-command
      start = end + 2;                         // next command
    }
    start = 0;              // next line
    // remember current command into history
    */
  }
  return 0;
}

// ============================================================================
// Parse the shell command, starting at args[start].  For example, if
// start = 0 and args holds:
//    {"ls", "-a", ">", "junk.txt", "&", "cat", "hello.c", ";"}
// then parse will find the "&" terminator at index 4, and set end to this
// value.  This tells the caller that the current command runs from index
// 'start' thru index 'end'.
//
// We return true if the command terminates with ";" or end-of-line.  We
// return false if the command terminates with "&"
// ============================================================================
bool parse(char **args, int start, int *end)
{
}

// ============================================================================
// Tokenize 'line'.  Recall that 'strtok' writes NULLs into its string
// argument to delimit the end of each sub-string.
// Eg: line = "ls -a --color" would produce tokens: "ls", "-a" and "--color"
// ============================================================================
char **tokenize(char *line)
{
  char** tokens;
  int tknCnt = 0;
  char* linecpy = strdup(line);

  char* token;
  token = strtok(linecpy, " ");
  while(token != NULL)
  {
    tknCnt++;
    token = strtok(NULL, " ");
  }
  free(linecpy);

  linecpy = strdup(line);

  int i = 0;
  tokens = (char**)malloc((tknCnt+1) * sizeof(char*));
  token = strtok(linecpy, " ");
  while(token != NULL)
  {
    tokens[i++] = token;
    token = strtok(NULL, " ");
  }
  tokens[tknCnt] = NULL;
  printf("Tokens:\n");
  for(int i = 0; i < tknCnt; i++)
  {
    printf("%s\n", tokens[i]);
  }
  free(linecpy);
}
