----------------------------------------------------begining--------------------------------------------------------
install software in Linux:
3 ways to install:
1. source installation (may generate a cross-compilation environment)
2. rpm installation
there may be dependencies between a large number of software.(many library and very complex installation)
3. yum installation (consider dependencies by itself)

Before downloading an app, data is stored on the server.

[In ubuntu, use apt]
apt is a commandline package manager and provides commands for
searching and managing as well as querying information about packages.
It provides the same functionality as the specialized APT tools,
like apt-get and apt-cache, but enables options more suitable for
interactive use by default.

Most used commands:
  list - list packages based on package names
  search - search in package descriptions
  show - show package details
1.  install - install packages
  reinstall - reinstall packages
2.  remove - remove packages
  autoremove - Remove automatically all unused packages
3.  update - update list of available packages
  upgrade - upgrade the system by installing/upgrading packages
  full-upgrade - upgrade the system by removing/installing/upgrading packages
  edit-sources - edit the source information file
  satisfy - satisfy dependency strings

There are many offical, stable softwares, --> offical built-in

1. For apt to work, it must be connected to the Internet.
2. There can only be one apt running.

-----------------------------------VIM--------------------------------------------
IDE: set running, compiling, debugging in one.
BUT in linux, we need to do many things on our own.
Write Code.  --> vi/vim
Compile code --> 
Debug code   -->
release code -->
run code     -->
maintain code-->

vim is used for writing code.
There are multiple modes:
1. command mode (by default)   shift+:  'q' -->exit
2. bottom row mode
3. insert mode   use "esc" --> command mode -shift+':'-> bottom row mode + :wq (written + quit)

<command mode>:
[cursor]: 
1. [hjkl] h(left)l(right)j(down)k(up)
2. [sh+^] begining of the line [sh+$] end of the line.
3. [gg] start line  [sh+g] end line  [n+sh+g] specific line
4. [w][b] word

[text]:
1. [yy]:copy  [n+yy]:copy the current and following lines
2. [p]:paste [n+p]:paste n times
3. [u]:undo
3.5 -> [ctrl+r]: undo the recent undo
3. [dd/ndd]:delete current line [dd->p]:cut
4. [sh+~]:change case(A->a,a->A)
5. [x]:delete a character(left->right) [nx]
6. [sh+x]:(right->left)
7. [r]:replace a char,[nr]
8. [sh+r]:replace mode
9. [ctrl+b]:page back [ctrl+f]:page front [ctrl+u]:page back(half) [ctrl+d]:page front(half)

<bottom line mode>:
1. [set nu] [set nonu] line number
2. [vs filename] open child windows  [ctrl+ w + w]: switch the cursor of window
3. [w] save
4. [q] quit
5. [w!] forcely save

<insert mode>:
get into insert mode: [a(next char)]/[i(current char)]/[o(create a new line)]


~~~~~~~~~~~~~~~~~~~~~~~~VIM CONFIGURE~~~~~~~~~~~~~~~~~~~~~~
vim configure is only influencing on the user who are changing the vim.

--------------------------gcc/g++---------------------------------
there are many steps in gcc
1. preprocessing: gcc -E filename.c (make macro substitution,uncomment) -o filename.i  e.g. gcc -E test.c -o test.i
2. compiling: gcc -S filename.i -o filename.s (generate assembly file)  
ASK: can computer execute assembly language?/Does assembly language need translator?
YES! Computers only know binary.
3. compliation: (assembly file -> redirectable target file) gcc -c
4. link: gcc test.o 
languages also have their libraries.(head file and lib file, lib file--> libc.a,libc.so)
we need link to their own libraries in translator.

BUT generally, we do not need to sperate them,
Only use : gcc test.c -o test  OR gcc -o test test.c

LIBRARY: 
static library : .a   (copy the code from library to executable program)
dynamic library : .so 
Linking is to associate what you wrote with the library.

gcc use dynamic linking to form executable program by default.
if we want to change dynamic way to static way, use gcc -o test test.c -static


----------------------------gdb----------------------------
debugging tools: gdb
If a program can be debugging, there must be somthing about debug in the binary file of this program.
By default, the executable program is "release" type.
Use -g when using g++/gcc to change the "release" type into "debug" type.

gdb : 
r -run ->run the code
l -list -> show the code
b -breakpoint -> make a breakpoint  e.g. b 5  make a breakpoint at line 5
info b  vrew the info aboot breakpoint
d+num -delete breakpoint
enable +num
disalbe +num 
c - continuing (running till the breakpoint)
s - F11 (get in the function
n - F10 (process by process)
until - (untill), until line_num.

p + variable -> print variable
display +variable
display + &variable 
NOTE:display + variable] is always showing the info when press [s] or [n],use undisplay + line_num to close it.
finish -run till exit from a current function.
quit -quit gdb
--------------------------make/makefile----------------------------------------------
1. make is a command, makefile is a file.
2. considering the dependencies between files.(which one is compiled firstly?)
3. 
dependency relation
dependency method

use Makefile to define dependency.
when make scans makefile, it will only form a target dependency relation, by default, the first one.
.PHONY : retouch(or modify) corresponding symbol, forming pseudo target (always can be executable).

