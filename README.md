# Custom-Shell-Linux


The objective is to develop your own shell that takes input from standard input. Your shell must
support the following commands which you will code:
- ls; which will list file type, access rights, file size and file name of all files in the present working
directory
- pwd; which will print the present working directory
- cd; which will change the present working directory to the location provided as argument
- help; which will print the list of supported commands
- cat; which will print on standard output the contents of the file provided to it as argument
- wc; which will print on standard output the number of lines in the file provided to it as argument
- exit; which will exit the shell
* ls, cat and wc commands must be coded and executed in a separate process using fork+exec.
These must not be embedded in shell code. Your single makefile will compile each of these
commands code and your shell code separately.
Your shell must have the following features:
- it must enable the user to access directly the n-th previously typed command
- the up arrow key must enable us to access previously typed commands
- the vertical bar character ‘|’ must enable constructing a pipe between commands
- must support redirecting standard input and output of commands to files through the ‘<’ and ‘>’
characters.
- in case of SIGTERM, your shell must exit properly (e.g. by printing a message on screen).
- You may upgrade your homework in future so design your program keeping that in mind.
Rules
a) You must implement all the commands supported by your shell; you cannot use the “system()”
system call. You must use fork+exec for process spawning.
b) Your program must handle eventual errors gracefully according to the POSIX traditions.
c) Use POSIX and Standard C libraries. You can write your program using C11 standards.
d) Valgrind rule from previous homework still holds and will be hold on every homework.
e) Your program MUST not give “segmentation fault” or any other fault that causes your program to
end in a bad way. It is better for you to send a little part of the homework with clean and executing
code instead of full homework with faults like this.
f) Provide a makefile to compile your homework. Do not run your program inside makefile.
Just compile it.
) Test your homework using the Virtual Machine given to you.
k) No late submissions.
f) Do not send any additional files like screenshots. Just send the makefile and source files.
g) Taking any code from internet will result getting a grade of -100. Do no put links or references to
internet, you don’t need code from internet to do this homework. Man pages and your book is well
enough