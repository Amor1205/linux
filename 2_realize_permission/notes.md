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

