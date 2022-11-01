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
