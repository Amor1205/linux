command:

1. cal
show monthly calender.
2. find 
find <path> -name <filename>
instrutcton\order\  are all the executable program.
use "which" we can find the path of instruction.

3. grep <string> <filename>
e.g. grep "99" filename.txt
search for string in a file and print the line containing the string.
if we want to print the lines containing '9', use "cat file.txt | grep "9"" or grep "99"
grep -v:
print any lines which don't contain '9', grep -v '9'
grep -i/-in:
neglect case :(e.g.  I/i O/o K/k)

SUPPORT regular expressions.
 
4. ctrl + R : search for something.
5. zip/unzip
zip -r : recrusive processing
Process all files and subdirectories in a directory together.

e.g.
zip test.zip <filename>
unzip test.zip -d <directrory>  (-d : specify the path)
6. tar (bale) purpose of tar is not to reduce the space used, just to put files together.
-c create a compressed file
-z need to use gzip to compress
-v show file name in process of compression.
-f choose a compressed file name.
(so when using tar to compress file, use -czvf)
-x decompress
-C decompress file into a specific path.
(so when using tar to decompress file, use -xzvf)
if target a specific path to decompress, use tar -xzvf file.tgz -C Path

7. bc
calculate : echo"1+2+3+4+5" | bc --> print the outcome.
8. uname -a / -r

shell command and operation principle:
linux distribution = linux kernel + shell
shell : command line interpreter.
user ----> shell ----> operating system
the function of shell:
1. transfer request instruction and let operating system excuting an order.
2. for illegal command, protect kernel.
3. create child process to do risky things.
shell itself is a process.
bash is a specific type of shell.

------------------------------------------
permission:
By default, there are 2 types of users. One is root(super administrator), another is ordinary user.
use [whoami] to check which user we are using.
use [su -] to become root user.
use [exit ] to become ordinary user.
DO REMEMBER:
DON'T use SAME passwords when using root and ordinary accounts. --> will cause non-clear permission boundaries.

BUT when you change user account, the system will create a bash for the new user, and will cause memory waste.
can use [sudo] (temporary permission promotion).

permission = user + file attribute.
the file itself has natural permission attributes: r + w + x

so permission modification: modify user or modify file attributes
First we recognize the category of users in linux:
1. owner
2. group (for in-group collaboration)
3. other
These are all roles, do not conflict with user category.
---------------------------------------------------------------
drwxrwxr-x 2 amor amor 4096 Oct 31 14:46 .
drwxrwxr-x 5 amor amor 4096 Oct 30 18:18 ..
-rw-rw-r-- 1 amor amor 1298 Oct 31 11:35 notes.md
-rw-rw-r-- 1 amor amor 1024 Oct 31 14:46 .notes.md.swp
-owner-group-other
-rwx rwx rwx
------------
identify file type.
In linux,don't recognize different types of files by the suffix.
to '-' begining : ordinary files (text, dynamic and static library, exectuable program, sourse program)
to 'd' begining : directory file.
to 'c' begining : character device file.(keyboard and monitor)
to 'b' begining : block device file.(disk)
to 'p' begining : pipe file.(communication)
to 'l' begining : link file.(soft connection)


