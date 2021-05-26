# MavShell
Mav shell (msh) is similar to bash, c-shell , or korn shell (ksh). It allows the user to run system ommands (ls,cd etc.), customized comands as well as launch and control processes. The shell, like csh or bash, will run and accept commands until the user exits the shell.
***
## Build and Run
 To build the rpoject use a Vm or Linux/Unix terminal, enter:
 
    gcc msh.c
 
 To run the project enter:
 
    ./a.out
 ***
## Accepted commands
  * The program prompts `msh>` when it is ready to accept input, and reads a line of input enetered after that, if the command given is a **supported shell command** otherwise prints the command followed by “: invalid option --”
  * The shell executes any command command in /bin, /usr/bin/, /usr/local/bin/ and the current working directory.
### history
  The `history` command lists the last 15 commands entered by the user.
### !n(bang)
  Typing `!n`, where n is a number between 1 and 15 results re-running the nth command. If the nth command does not exist then user is prompted “Command not in history.”
### showpids
  The  `showpids`command lists the PIDs of the last 15 processes or less as per the history,spawned by the shell.
### quit/exit
  This command is used to exit the shell interface with status zero.
 ***
## Test Run Demo
 ![Demo-run](https://github.com/Kidrah31/MavShell/blob/main/MavShell_Walkthrough.png)
 ***
 ## Resources
 * [Course-Github-Repo](https://github.com/CSE3320/Shell-Assignment )
 *  Man pages for the following: `fork`, `exec`, `exit`, `print`, `fgets`,
`strtok`, `strsep`, `strcmp`, `wait`, and `pause`.
***
