/*
    Emulated run console command line.

    Stephen H.L.Wang 
 */

/*
   List all available command collection.

   All that is declared with MAINFUNC_ENTRY will be listed here.
   
   return number of commands found
 */
extern int listmaincmd(void);

/*
  Run command line specified in argument str.
  This function emulation command execution in console.

  str: this is the complete command line string including command name and arguments

  return 1 if command found and run, 0 if command not found, -1 if command parsing error 
 */
extern int runmaincmd(char *str);
