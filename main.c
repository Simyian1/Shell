#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "shell.c"
#include <sys/wait.h>



char **tokenize(char *line);
bool parse(char **args, int start, int *end);
char** cleanArgs(char **args);
int doPipe(char **args, int pipei);
void processCommand(char **args);

enum{READ, WRITE};
#define ASCII_ART "\
credit: https://emojicombos.com/starry-night-ascii-art \n\
⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀ \n\
⣿⣿⢻⢻⣻⣻⡟⢻⣿⣿⡟⣿⣿⡻⣿⣿⣿⡿⠛⠛⠟⣍⠟⠹⣻⡿⢟⢝⠛⠿⣟⠉⢉⣹⡿⠛⣽⠟⢻⣿⠟⠟⠻⣻⣿⡍⠻⢿⢛⡿⠛⣻⣿⡿⢻⡛⣻⣿⣿⠿⠋⢉⠛⠛⢿ \n\
⣿⣏⠸⣼⣿⠹⣿⣿⣿⢟⣵⣿⣟⠿⢶⡾⣭⣤⠤⣀⠢⠒⣺⠖⠋⣥⡶⣺⠇⠐⣉⡤⠞⠪⠜⢀⣹⡷⠿⣫⣿⡆⡃⠟⢿⠇⠀⣯⣸⠁⣰⡇⡞⢀⠞⢙⡽⣿⡷⢂⣀⣉⡛⠲⣾ \n\
⣿⢏⠷⣽⢷⣒⣿⣿⣵⠛⢋⣿⣿⣤⣭⣋⠙⠑⢠⠌⣉⣉⡙⠚⠛⣒⣻⣻⣽⣚⣁⠐⠒⢒⠚⠉⠊⠳⣶⡍⠉⢕⣧⡼⠉⠀⠀⣿⣇⡇⣿⡁⢁⡞⢠⣿⡸⢹⡖⢹⢿⠉⠙⢲⣸ \n\
⣿⡈⢛⠶⠐⠺⠋⠀⠀⠀⣾⣿⣿⠀⠑⠊⢷⠀⡄⠛⡉⣡⣴⣖⠯⠽⠉⠀⠀⠀⠈⠉⠛⠻⣭⣵⡢⣤⡈⠉⠻⡝⣳⠀⠀⢀⣤⣜⡟⡇⢹⡇⢸⠀⠘⣇⡇⡏⠀⣏⠈⣔⣀⠀⣿ \n\
⣿⠹⣌⡓⢦⡀⠀⠀⠀⠀⢃⣿⡟⠄⢁⣤⣿⡆⠀⣠⣾⡿⠋⢀⣤⠄⣀⣤⡄⢤⣤⣄⠠⢤⡀⠙⣣⡀⠉⠳⣄⠙⣎⠀⡰⠒⠂⠈⢽⣿⠸⡇⣼⣦⡄⠘⠧⢧⠀⠘⠲⠤⠤⢚⣿ \n\
⣿⠳⣜⢻⡲⠮⠟⠃⠀⠀⢸⡏⣷⣛⣿⠉⣩⡴⠚⢑⡏⢀⡴⠫⠒⠉⠀⠀⠀⠀⠈⠉⠓⠦⣿⣧⠀⠀⠀⠈⣿⣆⢸⢯⢳⣸⣱⡿⡟⠈⢧⠘⣽⡸⣿⣄⠀⣈⠑⠢⠤⠤⢔⡪⣿ \n\
⣿⠀⠈⠑⠯⣵⣲⢶⢦⣤⣤⣷⣿⣿⣻⠞⠉⠀⣴⣟⡿⢭⠴⠊⠕⣲⡤⠟⠛⠫⢭⣐⠢⣤⡀⠙⣦⡀⠀⠙⣮⣿⡎⣏⠐⢇⠑⠊⡊⠡⠋⠳⣝⠦⡈⢾⡳⣍⣓⡦⠤⠤⠤⠔⣻ \n\
⣿⣣⣄⠀⠀⠀⠉⠙⣿⡟⢾⣿⠿⣿⢁⣤⠆⠈⣿⡕⡠⠞⣶⣄⡋⠁⣠⡤⠒⠦⣄⠈⠳⡌⠻⣦⠈⣇⠀⣷⠈⢿⢹⠹⢆⠀⠀⠀⠠⠆⠀⢰⡙⢧⣉⠀⠹⡝⡋⠉⠉⠓⠚⠉⣸ \n\
⣿⢯⣿⣿⣷⣶⣤⣀⣾⡇⢹⣿⢠⣿⡇⠀⣠⠾⠉⠀⢾⡿⢋⣿⠃⡞⠁⣶⣶⣀⡘⡆⠀⢣⣤⡎⠃⢸⠀⠀⠀⠀⡼⠀⠀⠙⠒⠒⠒⡂⢤⡀⠈⠉⠚⠛⢷⣦⡍⠹⠛⠦⠈⠁⢺ \n\
⣿⠀⠈⠙⠛⠂⠙⠛⣿⣿⣸⣿⠸⢡⣷⣨⠵⡿⠛⠮⠍⠹⠿⠃⢸⠀⢸⢹⠈⣶⣶⢿⡄⠘⢻⣤⢁⡜⣄⡜⣠⡿⣡⠴⠒⠒⠒⠒⠦⣌⠀⠈⢦⡈⢦⣙⠲⣤⠄⠁⠀⠈⣥⣤⣸ \n\
⣿⣤⣄⠀⠀⠀⠀⢀⣻⡿⣿⣿⠀⢸⡿⡋⠉⢀⡠⢖⡉⠀⠀⠀⠘⣆⠈⢯⡀⠈⢿⣌⠳⣤⡀⠀⣭⡉⢁⡠⠟⠋⣁⡠⠤⠤⢤⣀⣶⠈⢳⡀⠀⢹⡔⣯⣭⣭⡤⠤⠴⠤⠴⠒⢻ \n\
⣿⣿⡿⣾⡿⠋⣻⣿⣿⣿⡉⣿⡇⠘⢿⣧⣴⣯⡾⠋⣠⣾⣷⣒⡦⠙⣆⠈⣿⣦⡀⠙⠲⠦⢍⣉⣉⠭⠟⣀⠖⢉⣥⢖⣊⡑⢦⢨⣿⠀⡆⢻⢦⠀⣇⡤⠛⠁⠀⠀⠀⠀⠀⠀⢸ \n\
⣿⣽⡿⢻⢸⣾⣿⣿⣿⣇⠃⠀⠀⠀⠈⣿⢯⢃⣠⡾⢃⣤⠶⠶⢬⡻⣿⣷⣌⠣⠉⠛⠶⠤⢄⣀⠤⠖⠋⢁⡴⡋⠀⡃⠠⢇⡔⠄⡿⠀⣵⡾⣼⣰⠟⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸ \n\
⣿⠭⣵⣁⠈⠉⢿⣿⡿⣿⡄⠀⡄⣀⢀⣿⠈⢿⣽⢠⠯⠊⠈⣙⠆⠙⠚⡿⣎⠓⣶⠤⢄⣀⣀⣠⢴⠚⢉⡡⠾⠃⢰⡳⣌⡓⠒⠚⣀⣼⠟⣿⠿⠋⠀⠀⠀⠀⠀⠀⢠⡶⠒⠁⢸ \n\
⣿⠀⠉⠉⠉⢳⣘⡿⢿⡟⣷⠀⠿⣿⡞⣿⡇⣿⡛⡸⡀⢀⠤⠎⢀⠀⡖⣷⡏⠓⠾⠿⠖⠊⢉⠉⠀⣀⣂⡥⢶⣇⡬⠾⠶⠿⠿⠿⠶⠚⠉⠁⠀⠀⠀⠀⠀⠀⣠⠖⠒⠉⠉⠁⢹ \n\
⣿⠉⠉⠓⠲⢮⡿⣿⣿⣿⠹⡇⠀⢻⢱⣻⣧⡟⣿⡷⣝⡾⠁⣒⡡⣾⣾⣿⣇⠤⠖⠉⠉⢉⣳⣶⣛⣁⠤⠶⠊⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣠⠴⢺⡇⣀⣀⡀⢤⣀⣼ \n\
⣿⠀⠀⠀⠀⠀⠙⢿⣿⣏⡇⠀⣶⠉⡿⡏⠙⣿⠛⣷⠒⠺⠿⠷⠊⠁⠀⢀⣀⣀⣠⠶⠋⠉⠐⠋⠀⠀⠀⢀⣀⡤⠶⠶⢶⣶⠤⣶⡒⠖⠚⠉⣉⣽⣋⣀⡼⠛⠉⠁⠀⠀⣀⡤⢾ \n\
⣿⠶⣄⡀⠀⠀⠀⠀⢻⣿⡆⣰⣿⡀⢾⣿⣆⢹⠀⠘⣿⣄⠀⠀⣀⠴⠊⠉⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⠞⠉⠀⠀⠀⣀⡭⠽⣯⡿⠿⠋⠉⠉⠉⠀⠀⣠⠖⠀⠀⠀⣠⠞⠁⠀⢸ \n\
⣿⠀⠀⠙⠲⠦⣄⣀⣀⣿⠀⣿⡏⡇⣾⣧⣿⢸⠀⠀⠹⣿⣿⣍⢁⣠⡴⠒⠛⠋⠙⣦⣄⡠⠴⠞⢉⡟⡆⠀⠀⠤⠊⢁⡴⢻⠟⢀⣀⣀⣠⣴⣶⣶⣞⣁⣀⣀⣠⣞⣁⣀⠤⡔⣻ \n\
⣿⠛⠲⣖⣦⣤⢴⣾⣾⣿⡄⢿⡇⢷⣿⢿⣿⡌⣧⠀⠀⠈⠙⣿⣯⡵⠴⠦⠤⣄⡀⠀⢀⣀⣠⡤⣾⡇⣷⣤⠔⠒⠛⠉⠀⠀⣴⡏⠟⠏⣾⠝⠇⠀⠟⠹⠹⢻⠀⠀⠀⠀⠀⠑⣿ \n\
⣿⡀⢀⣠⠟⠛⠋⢁⣼⣿⠇⢸⢸⠈⢿⣿⡝⢿⡜⣇⢰⣠⠀⠘⡿⡟⠓⢦⡤⠤⠭⢿⣏⡙⠀⣰⢿⣇⣹⠈⢳⣶⡛⠛⣛⠾⠿⣾⣶⡶⠀⠀⢀⣠⣠⠴⣿⣷⣤⠤⠀⠤⣴⡚⢿ \n\
⣿⣌⣁⣀⡀⠀⢀⣼⡏⣿⣾⠘⡆⢧⣸⠋⣿⡜⣷⢸⠀⣿⠀⠀⣧⣿⣾⠿⢦⣀⣀⣴⣿⠶⣾⣿⣿⠿⣿⣿⣿⣿⢿⣿⣿⣧⣾⣿⣿⣿⣿⣶⣿⣿⣧⠞⣿⢻⣿⠇⡤⠖⠋⠀⢸ \n\
⣿⣀⣀⣨⡟⢛⣛⣯⣇⢷⣿⣆⠹⣾⡏⠀⡟⡇⢸⣿⢀⡟⠀⣸⣿⣿⢹⣄⣾⣤⣿⣽⣿⣶⣿⣿⣿⠼⣿⣿⠻⣿⣿⣥⢿⣿⣿⣿⣿⣧⠿⠵⢿⣿⣿⣾⣷⣿⣠⣤⣿⡶⠶⣦⣼ \n\
⣿⣀⡉⣻⣿⣿⣭⣿⣽⡌⢯⣿⢦⢹⠀⢰⢧⠇⡾⡟⠞⡸⣿⣿⠙⣎⠀⢿⣿⣍⠁⠛⢩⡏⢫⡿⠿⢿⡟⠛⣻⡿⠻⣿⣿⠿⠿⠿⣿⠿⣿⣷⣶⣿⠥⣤⣿⣿⡿⣿⣿⡿⢯⠗⢻ \n\
⣿⣿⢻⣻⣿⣿⣿⣿⡿⣟⠈⢿⡻⣿⠀⡎⡾⡼⠁⣧⡼⡅⣿⡟⢧⠙⢦⠈⡟⣿⣦⣏⡾⠿⠿⠭⠭⣿⠉⠉⢁⡖⠛⠉⠙⠋⣹⣷⣁⣤⣼⣿⢿⣽⣦⢴⡿⠿⠿⠿⣿⢔⣶⡒⣻ \n\
⣿⣿⣮⣿⢿⣼⣿⣿⣟⣛⡷⡀⠙⡽⠀⣇⣧⡇⠀⠘⣇⣇⠘⢷⣆⢳⡈⢷⣹⢹⣿⠀⠀⠀⠠⠤⠭⠛⠚⢿⣏⣀⣀⣀⣀⣼⣋⣀⣀⣀⡟⣿⡟⠛⡟⠾⣧⣤⣤⣤⡿⠋⠈⠀⢹ \n\
⠿⠾⠾⠿⠶⠿⠿⠿⠾⠿⠷⠷⠶⠾⠶⠾⠿⠿⠶⠶⠾⠾⠶⠾⠿⠾⠷⠾⠷⠾⠿⠶⠶⠶⠶⠶⠶⠶⠶⠶⠷⠿⠶⠿⠾⠿⠿⠿⠿⠾⠷⠿⠷⠶⠶⠶⠿⠿⠿⠿⠷⠶⠶⠶⠾"

// ============================================================================

// Execute a child process.  
// Returns -1
// on failure.  On success, does not return to caller.
// ============================================================================
int child(char **args)
{
  int i = 0;
  int fd;

  while (args[i] != NULL)
  {
    if (equal(args[i], ">"))
    {
      // open the corresponding file and use dup to direct stdout to file
      fd = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
      
      if (fd < 0) {
        perror("File was not opened correctly");
        exit(1);
      }

      if (dup2(fd, 1) < 0) {
        perror("Pipe redirect failed.");
      }
      i += 2;
    }
    else if (equal(args[i], "<"))
    {
      // open the corresponding file and use dup to pull stdin from file
      fd = open(args[i+1], O_RDONLY, 0644);
      if (fd < 0) {
        perror("File was not opened correctly");
        exit(1);
      }

      if (dup2(fd, 0) < 0) {
        perror("Pipe redirect failed.");
      }
      i += 2;
    }
    else if (equal(args[i], "|"))
    { 
      // do pipe in a separate function
      doPipe(args, i);
      i++;
    }
    else
    {
      ++i;
    }
  }

  //clean args made----------------
  char **clArgs = cleanArgs(args);

  // call execvp on prepared arguments after while loop ends. You can modify arguments as you loop over the arguments above.
  execvp(clArgs[0], clArgs);
  free(clArgs);
  close(fd);
  return -1;
}

char** cleanArgs(char **args)
{
  int i = 0;
  while (args[i] != NULL) i++;
  //clean args made----------------
  char ** clArgs = malloc((i + 1) * sizeof(char *));
  int cleanI = 0;
  for(int j = 0; args[j] != NULL; j++)
  {
      if(equal(args[j], ">") || equal(args[j], "<"))
      {
          j++; 
      }
      else
      {
        clArgs[cleanI++] = args[j];
      }
  }
  clArgs[cleanI] = NULL;
  return clArgs;
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
  int status;
  if(end < start) return;
  //args current made -------------------------
  char** argsCurrent = (char**)malloc((end - start + 2) * sizeof(char*));
  
  int j = 0;
  for(int i = start; i <= end; i++, j++)
  {
    argsCurrent[j] = args[i];
  }
  argsCurrent[j] = NULL;

  int pid = fork();
  if (pid == 0) {
    // Child
    child(argsCurrent);

    exit(1);
  } else {
    // parent
    if(waitfor)
    {
      wait(&status);
    }

    free(argsCurrent);
    //args current freedParent -------------------------
  }
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
  // ---------- CREATE PIPE ----------
  int pipeFD[2];

  if(pipe(pipeFD) < 0) {
    perror("failed making the pipe! :(");
    return -1;
  }

  pid_t pid = fork();
  
  if (pid < 0) {
    perror("fork failed for LEFT command");
    return -1;
  }

  // ------------ PARENT BUSINESS ------------

  if (pid > 0) {
    // parent doesn't need to read
    close(pipeFD[READ]);

    if (dup2(pipeFD[1], 1) < 0) {
      perror("dup2 failed in RIGHT child");
      exit(EXIT_FAILURE);
    }

    // allocate and copy LEFT tokens
    int leftCount = pipei; // where '|' is.
    char **leftTokens = malloc((leftCount + 1) * sizeof(char*));

    if (leftTokens == NULL) {
      perror("malloc failed for left tokens");
      return -1;
    }

    for (int i = 0; i < leftCount; i++) leftTokens[i] = args[i];   
    
    // add NULL at the end of the left token array
    leftTokens[leftCount] = NULL;
    
    //clean args made----------------
    char **clArgs = cleanArgs(leftTokens);
    free(leftTokens);
    
    // execute left command
    execvp(clArgs[0], clArgs);
    free(clArgs);
    perror("execvp failed for LEFT command"); // shouldn't run if execvp succeeds
    exit(EXIT_FAILURE);
  }
  // ------------ CHILD BUSINESS ------------
  else
  {
    // child doesn't need to read
    close(pipeFD[WRITE]);

    // redirect stout to pipe's write file
    if (dup2(pipeFD[READ], 0) < 0) {
      perror("dup2 failed in left child");
      exit(EXIT_FAILURE);
    }
    
    // allocate and copy RIGHT tokens
    int rightCount = 0;
    for (int i = pipei + 1; args[i] != NULL; i++) rightCount++;

    char **rightTokens = malloc((rightCount + 1) * sizeof(char*));

    if (rightTokens == NULL) {
      perror("malloc failed for right tokens");
      free(rightTokens);
      return -1;
    }

    for (int i = pipei + 1, j = 0; args[i] != NULL; i++, j++) rightTokens[j] = args[i];
  
    // add NULL at the end of the right token array
    rightTokens[rightCount] = NULL;
    
    // execute right command
    
    processCommand(rightTokens);
    for(int i=0; i < rightCount; i++) free(rightTokens[i]);
    free(rightTokens);
    exit(EXIT_FAILURE);

  }

  wait(NULL);  

}

void processCommand(char **args)
{
  int start = 0;
  int tknCnt = 0;
  while(args[tknCnt] != NULL) tknCnt++;

  while(start < tknCnt)
  {
    int end;
    bool waitfor = parse(args, start, &end);
    if(end >= start) doCommand(args, start, end, waitfor);
    start = end + 1;
    while(args[start] && (equal(args[start], "&") || equal(args[start], ";"))) start++;
  }
}

// ============================================================================
// Main loop for our Unix shell interpreter
// ============================================================================
int main()
{
  bool should_run = true;          // loop until false
  //line and history made--------------------
  char *line = NULL; // holds user input
  char *history = calloc(1, MAXLINE);

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
    
    if (equal(line, "ascii"))
    {
      printf("\n%s\n",ASCII_ART);
      strcpy(history, line);
      continue;
    }
    
    
    if (equal(line, "!!"))
    {
      // gethistory
      if(history[0] == '\0')
      {
        printf("No commands in history.\n");
        continue;
      }
      else
      {
        strcpy(line, history);
      }
    }
    
    // process lines
    char **args = tokenize(line); // split string into tokens
    
    int tokenCount = 0;
    while (args[tokenCount] != NULL) tokenCount++;


    // loop over to find chunk of independent commands and execute
    while (start < tokenCount)
    {
      int end;
      bool waitfor = parse(args, start, &end);// parse() checks if current command ends with ";" or "&"  or nothing. if it does not end with anything treat it as ; or blocking call. Parse updates "end" to the index of the last token before ; or & or simply nothing
      if (end >= start) doCommand(args, start, end, waitfor);   // execute sub-command
      start = end + 1;                       // next command
      while (args[start] != NULL && (equal(args[start], "&") || equal(args[start], ";"))) start++;
    }
    start = 0;              // next line
    // remember current command into history
    strcpy(history, line);
    
    for(int i = 0; args[i] != NULL; ++i)
    {
      free(args[i]);
    }
    free(args);
    //args freed--------------------
  }
  if(line != NULL) free(line);
  free(history);
  //line and history freed--------------------
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
  char **curr = args + start;

  // set end index
  *end = start;
  
  // go until you've hit ';', '&', or end of line.
  while (*curr != NULL && !equal(*curr, "&") && !equal(*curr, ";")) {
    (*end)++; // update end index
    curr++; // go to the next string
  }
  *end = *end -1;

  // to avoid errors with putting NULL into equal
  if (*curr == NULL) {
    return true;
  }

  return equal(*curr, "&"); 
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
  const char* delims = " ";
  token = strtok(linecpy, delims);
  while(token != NULL)
  {
    tknCnt++;
    token = strtok(NULL, delims);
  }
  free(linecpy);

  linecpy = strdup(line);

  int i = 0;
  tokens = (char**)malloc((tknCnt+1) * sizeof(char*));
  token = strtok(linecpy, delims);
  while(token != NULL)
  {
    tokens[i++] = strdup(token);
    token = strtok(NULL, delims);
  }
  tokens[tknCnt] = NULL;

  free(linecpy);

  return tokens;
}
