class 1_ command

1.
output redirection : clean the origin content and output    >
e.g. echo "text" > filename
append redirection : append string in the end of the file.  >>
e.g. echo "text" >> filename

input redirection : <
e.g. cat < filename

2.
more 
less (recommended)

if we want to view the middle content of the file, use:
	method1:
	1. head -120 file.txt > temp.txt (we want to view the 100-120 lines in the file, first create a tmp file to contain it)
	2. tail -20 temp.txt (see the tail 20 lines in the temp.txt)
	method2:
	head -120 file.txt | tail -20 (pipeline)
	assembly line data processing.
	when using |, there is an invisible redirection by default.
3.
view date

amor@amor-win:~/linux/linux/1_command$ date
Sat 29 Oct 2022 11:18:20 PM +08
amor@amor-win:~/linux/linux/1_command$ date +%Y
2022
amor@amor-win:~/linux/linux/1_command$ date +%Y%m
202210
amor@amor-win:~/linux/linux/1_command$ date +%Y%m%d
20221029
amor@amor-win:~/linux/linux/1_command$ date +%Y-%m-%d
2022-10-29
amor@amor-win:~/linux/linux/1_command$ date +%Y-%m-%d:%H:%M:%S
2022-10-29:23:19:18
amor@amor-win:~/linux/linux/1_command$ date +%Y-%m-%d:%H:%M:%S
2022-10-29:23:19:20
amor@amor-win:~/linux/linux/1_command$ date +%Y-%m-%d:%H:%M:%S
2022-10-29:23:19:20
amor@amor-win:~/linux/linux/1_command$ date +%Y-%m-%d:%H:%M:%S
2022-10-29:23:19:22

IMPORTANT:
date +%s   (s-->timestamp)
timestamp : the second counting from 1970/01/01 00:00 till now.
	we can use "date +%Y-%m-%d_%H:%M:%S -d @timestamp" to see the corresponding time of specific timestamp.
When will we use timestamps?
There is time information in logs.(program generating timestamp --> use this change timestamp into time that we can understand.)
