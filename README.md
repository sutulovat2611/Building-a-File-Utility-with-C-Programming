> This documents explains the main functionality of the application. 
> Moreover, it may be treated as the manual with the examples to know how to run the app in order to get the desired result.

# Building-a-File-Utility-with-C-Programming
This application is a multipurpose file utility, which combines the simplified
features of multiple Linux utilities such as head, tail, cat, and cp.

1. The first step is that the following application has to be compiled: ```gcc -o fileutil 30806151_fileutil.c```
2. The following app provides: 
### VIEWING FUNCTIONALITY:
- In order to print to the terminal the first 10 lines of the "sample.txt" file, run the program with no arguments: 
  ```./fileutil ```
- In order to print to the terminal the first 10 lines of any other file, provide the name of the file with the absolut path:
	```./fileutil /home/student/dir/example.txt ```
- If the file to be viewed does not exist at the current/specified directory it will give an error

### COPYING FUNCTIONALITY:
- In order to copy first 10 lines of the file to another file in the certain directory, run the program with -d, specifying the directory RIGHT AFTER:
```./fileutil -d /home/student/dir ``` __(important that there is no '/'at the end)__

- The directory should be provided in a form of an absolute path, otherwise it is invalid. 
- The name of the copied file will be same as the name of the original file.
- If there is a file with the same name in the directory, it will give an error.
- The copying functionality may be used with specifying the file to copy FROM as well: <br />
```./fileutil /home/student/dir/example.txt -d /home/student/dir2``` (if the file is not specified, the "sample.txt" will be copied)

### MORE (the following functionalities are available for both copying and viewing):
- To specify the number of lines to be copied/viewed, run the program with -n, specifying the number of lines RIGHT AFTER: <br />
		  ```./fileutil -n 3 ( prints to the terminal first 3 lines of "sample.txt")```<br />
		  ```./fileutil /home/student/dir/example.txt -n 6 -d /home/student/dir2``` (copies the first 6 lines from example.txt to the 
		  copy with the same name at the diretctory /home/student/dir2)
- If -n is not followed by any specified positive number it will give an error.
		
- In order to switch to the tail mode (showing/copying lines in the end), put -L as one of the arguments: <br />
```./fileutil -L (prints 10 last lines from the "sample.txt")``` <br />
```./fileutil -L -n 3 (prints 3 last lines from the "sample.txt") ``` <br />

## IMPORTANT NOTE: 
All the functionality mentioned above may be mixed up and used together, however is it important to remember that:
1. the file to be copied/viewed __FROM__ should always be specified as the __FIRST__ argument
2. -d and -n __MUST__ have the values right after them with the space (otherwise it will be invalid)

## MORE EXAMPLES 
- Copy a.txt to dir2 where the newly created a.txt only contains the first 10 lines <br/> ``` ./fileutil /home/student/dir1/a.txt -d /home/student/dir2 ```<br/>
- Copy a.txt to dir2 where the newly created a.txt only contains the first 15 lines <br/> ```  ./fileutil /home/student/dir1/a.txt -d /home/student/dir2 -n 15 ```<br />
- Copy a.txt to dir2 where the newly created a.txt only contains the last 15 lines <br/> ```   ./fileutil /home/student/dir1/a.txt -d /home/student/dir2 -n 15 -L ```<br />
- Invalid argument, no line numbers specified after the -n argument! <br/> ```   ./fileutil /home/student/dir1/a.txt -n ```<br />
- Invalid argument: immediately after -d, a directory path was expected <br/> ```./fileutil /home/student/dir1/a.txt -d -L /home/student/dir2  ```<br />
- Copy a.txt to dir2 where the newly created a.txt only contains the last 10 lines: 
<br/> ```./fileutil /home/student/dir1/a.txt -d /home/student/dir2 -L ```<br />
- Copy a.txt to dir2 where the newly created a.txt only contains the last 15 lines
<br/> ```./fileutil /home/student/dir1/a.txt -L -n 15 -d /home/student/dir2 ```<br />
- Copy a.txt to dir2 where the newly created a.txt only contains the last 15 lines)
<br/> ```./fileutil /home/student/dir1/a.txt -d /home/student/dir2/ -n 15 -L ```
