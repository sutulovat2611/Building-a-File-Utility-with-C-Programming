/*
 * Name: Sutulova Tatiana
 * Start date: 22/08/2021
 * Last modified: 27/08/2021
 * -------------------------
 * 
 * The following application allows to read or copy specified number of lines 
 * (or 10 lines by default) from the "sample.txt" or any other specified file. 
 * If the copy functionality is performed, the chosen file may be copied to any
 * directory specified by the user. Whereas if the view functionality is performed 
 * the lines will be printed to the terminal. Moreover, it allows to choose whether
 * the lines are supposed to be printed from the end or from the beginning of the file. 
 */

#include <sys/file.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <ctype.h>


int read_line(int read_from_fd, int index, int read_to_fd);
int read_arguments(int argc, char *argv[], int read_from_fd, int file_specified);
void read_file(int read_from_fd, int read_to_fd, int lines_max);
void read_file_end(int read_from_fd, int read_to_fd, int lines_max);
void stnderror(char* error_txt, int error_code);
int verify_int(char* input);

/*
 * Function: main 
 * --------------
 * opens the file for reading from it, checking whether the user input is valid. 
 * if the user did not specify the file to read from, opens the default "sample.txt",
 * otherwise opens the specified file. then, calls the read_arguments file.
 *
 * argc: argument counter, which is the number of arguments the user input
 * argv: array containing all the arguments the user input 
 * 
 * returns: none
 */
int main(int argc, char *argv[]){
	/*declaration*/
	int read_from_fd, read_to_fd, file_specified=0, dest_file_exist;
	
	/*checks whether the first argument is not a specified file, and if not opens
	default "sample.txt"*/
	if (argv[1]==NULL || strcmp("-d", argv[1])==0 || strcmp("-n", argv[1])==0 || strcmp("-L", argv[1])==0 ){
		if ((read_from_fd = open("sample.txt", O_RDONLY )) < 0) {
			stnderror("Could not open the file", 1);
		}
		file_specified=1;
	}
	else{
		if ((read_from_fd = open(argv[1], O_RDONLY, S_IRUSR | S_IWUSR )) < 0) {
			stnderror("Could not open the file", 1);
		}
	}
	
	dest_file_exist=read_arguments(argc, argv, read_from_fd, file_specified);
	
	/*close the files and exit cleanly*/
	if (dest_file_exist==1){
		close(read_to_fd);
	}
	close(read_from_fd);
 	exit(0);
}


/*
 * Function: read_arguments
 * ------------------------
 * responsible for validating the arguments. checks whether -n and -d has a 
 * specified value afterwards. sets up the further request according to the 
 * persistence of -d, -n, -L arguments
 *
 *
 * argc: argument counter, which is the number of arguments the user input
 * argv: array containing all the arguments the user input 
 * read_from_fd: the unique identifier of the file, that information is read from
 * file_specified: integer that contains 0/1, which has 0 if the read FROM file was not specified, 
 * 				   otherwise has 1
 *
 * returns: none
 */
int read_arguments(int argc, char *argv[], int read_from_fd, int file_specified){
	/*declaration*/
	int read_to_fd, dest_file_exist=0, lines_max=10, tail_mode=0;
	char *dest;

	for (int i=1; i<argc; i+=1){
		/*error if there is -d or -n without the specified value afterwards*/
		if ((strcmp("-d", argv[i])==0 && (argv[i+1]==NULL || argv[i+1][0]!='/'))
			|| (strcmp("-n", argv[i])==0 && (argv[i+1]==NULL || verify_int(argv[i+1])==0))){ 
				stnderror("Specify the proper directory/number of lines", 2);
		}
		
		/*validates the destination directory*/
		if (strcmp("-d", argv[i])==0){
			/*creates the proper destination directory name*/
			strcpy(dest, argv[i+1]);
			strcat(dest, "/");
			if (file_specified==0){ /* gets the name of the file to read from, if it was specified*/
				strcat(dest, basename(argv[1]));
			}
			else{
				strcat(dest, "sample.txt");
			}
			/*create file appending there the values accordingly*/
			if ((read_to_fd = open(dest, O_CREAT |O_EXCL| O_WRONLY | O_APPEND, 0664)) < 0) {
				stnderror("Could not open the file", 2);
			}
			else{
				dest_file_exist=1; /*shows that the destination file exists*/
			}
		}
		/*checks the number of lines*/
		if (strcmp("-n", argv[i])==0){ 
			lines_max= atoi(argv[i+1]);
		}
		/*checks whether the tail mode was requested*/
		if (strcmp("-L", argv[i])==0){
			tail_mode=1;
		}
	} 
	/*if the destination is not define, output fd is 1, which is terminal*/
	if (dest_file_exist==0){ 
		read_to_fd=1;
	}
	/*checks whether the tail mode is on and calls the functions accordingly*/
	if(tail_mode==0){
		read_file(read_from_fd, read_to_fd, lines_max);
	}
	else{
		read_file_end(read_from_fd, read_to_fd, lines_max);
	}
	return dest_file_exist;  
}

/*
 * Function: verify_int
 * -------------------
 * verifies, whether the input is a positive ingeter
 * 
 * input: character that was input by the user
 *
 * returns: 1 if the verification is successful, otherwise 0.
 */
int verify_int(char* input){
	for (int i=0; i<strlen(input); i++){
		if (!isdigit(input[i])){
			return 0;
		}
	}
	return 1;
}


/*
 * Function: read_file
 * -------------------
 * 
 * loops through the first lines_max lines of the file, calling the reading_line 
 * function to print it out
 *
 * 
 * read_from_fd: the unique identifier of the file, that information is read FROM
 * read_to_fd: the unique identifier of the file, that information is read TO
 * lines_max: the number of lines requested by the user ( if not requested, then 10)
 *
 * returns: none
 */
void read_file(int read_from_fd, int read_to_fd, int lines_max){
	/*declaration*/
	int lines=0, index=0, offset, file_size;
	/*offset of the last element shows the file size*/
    file_size=lseek(read_from_fd, 0, SEEK_END); 
	lseek(read_from_fd, 0, SEEK_SET); /*returns the offset pointer back to the beginning */
	
	
	while(lines<lines_max){
		index=read_line(read_from_fd , index, read_to_fd); 
		lines++; 
		offset=lseek(read_from_fd, index, SEEK_SET); /*sets up the pointer to the end of the read line*/
		/*exits once the offset pointer reaches the end of the file (if the file is smaller than 10 lines)*/
		if (offset>=file_size){ 
			break;	
		}
	}
	
	if (read_to_fd!=1){
		write(1, "Copy successful!", 18);
	}
}

/*
 * Function: read_line
 * -------------------
 * 
 * loops through each character of the certain line one by one until it reaches \n, 
 * increasing the offset pointer and prints it out to the copied file/terminal
 *
 * 
 * read_from_fd: the unique identifier of the file, that information is read FROM
 * read_to_fd: the unique identifier of the file, that information is read TO
 * index: current position of the offset, which is increased with every character
 *
 * returns: index, which is a pointer at the place up till which the file was read
 */
int read_line(int read_from_fd, int index, int read_to_fd){
	/* declaration */
	char current_char; 
	/*reads each character at the line one by one*/
	while (read(read_from_fd, &current_char, 1)==1){
		write(read_to_fd, &current_char, 1); 
		index+=1;
		/*returns the current pointer once the end of the line was reached*/
		if (current_char=='\n'){
			return index; 
		}
	}
}


/*
 * Function: read_file_end
 * -----------------------
 * 
 * loops through each character from the end backwards, counting the \n.
 * once line_max of \n or the beginning of the file is reached, breaks out of the 
 * loop. keeps the pointer at each character being read and counts the number of
 * bytes. once it is out of the loop prints the counted number of bytes starting 
 * from index to the terminal/specified file. 
 *
 * read_from_fd: the unique identifier of the file, that information is read FROM
 * read_to_fd: the unique identifier of the file, that information is read TO
 * lines_max: the number of lines requested by the user ( if not requested, then 10)
 *
 * returns: none
 */
void read_file_end(int read_from_fd, int read_to_fd, int lines_max ){
	/*declaration*/ 
	int n_counter=0, offset, word_counter=0, shorter=0;
	char current_char;
	/*pointing at the second last character of the file (before the end identifier)*/
	offset = lseek(read_from_fd, -2, SEEK_END); 
	
	while(n_counter<lines_max){
		/*reading the character that the pointer is pointing at*/
		if (read(read_from_fd, &current_char, 1)){
			/*counting all the bytes that were passed*/
			word_counter+=1; 
			/*once the beginning of the file is reached break out of the loop*/
			if (offset<=0){
				shorter=1; /*identifying that the file was shorter than 10 lines*/
				break;
			}
			/*counting all the \n in order to stop when lines_max is reached*/
			if (current_char=='\n'){
				n_counter+=1; 
			}
			/*stepping one letter back (2 bytes since the current position was increased when read() happened*/
			offset=lseek(read_from_fd, -2, SEEK_CUR); 
		}
	}
	/*if the file is not at it's beginning going back to the first char of the first line to be printed*/
	if (0==shorter){
		lseek(read_from_fd, 2, SEEK_CUR); 
	}
	else{ /*if we reached the beginning of the file, go to the first character*/
		lseek(read_from_fd, 0, SEEK_SET); 
	}
	/*reading all the counter characters into the output*/
	char output[word_counter]; 
	read(read_from_fd, output,word_counter);
	write(read_to_fd, output, word_counter);
	
	if (read_to_fd!=1){
		write(1, "Copy successful!", 18);
	}
}


/*
 * Function: stnderror
 * -------------------
 * 
 * this function is called whenever there is an error in the application. it 
 * prints out the error_txt to the standard error output stream and exits the 
 * app with an appropriate error_code
 *
 * error_txt: the text related to the error
 * error_code: the code of the error
 *
 * returns: none
 */
void stnderror(char* error_txt, int error_code)
{
	write(2, error_txt, strlen( error_txt));
	exit(error_code);
}
