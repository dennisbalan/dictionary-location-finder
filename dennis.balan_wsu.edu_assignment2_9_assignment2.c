/*	int lineNum(char *dictionaryName, char *word, int dictWidth)
 	
	Where: dictionaryName is a path to the dictionary location,
	word is the string you want to search, and dictWidth is the width
	of the dictionary in characters.
	
	Return: Your function should return the line number of the
	word, if present. If the word is not present, it should return
	the negative number of the line last searched.
  */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
int lineNum(char *dictionaryName, char *word, int dictWidth){
	//create fd and open dictionaryName file
	int fd = open(dictionaryName, O_RDONLY);
	//check fd for erros. Does fd exist?
	if(fd == -1){
		//set err_one to errno
		int err_one = errno;
		//print the errors
		fprintf(stderr, "Error # =  %d\n",err_one);
		fprintf(stderr, "Error - %s\n",strerror(err_one));
		//return the error saved in err1
		return err_one;
	}
	//IMPORTANT, ERROR DETAILS ARE IN THE IF STATEMENT ABOVE, I WONT SPEND MUCH TIME DISCUSSING EVERY LINE OF ERROR STATEMENTS
	//create width for easier code writing and reading
	int width = dictWidth;
	//lseek fd to end of file from beggining to get file length, which is saved in off_t length
	off_t length = lseek(fd,0,SEEK_END);
	//check length for any errors
	if(length == -1){
		int err_two = errno;
		fprintf(stderr,"Error # = %d\n",err_two);
		fprintf(stderr,"Error - %s\n",strerror(err_two));
		return err_two;
	}
	//find number of lines
       	int lines = length/width;
	//allocate space for the buffer = width
	char *buffer = malloc(width);
	//check for malloc errors
	/*if(buffer == NULL){
		int err_three = errno;
                fprintf(stderr,"Error # = %d\n",err_three);
                fprintf(stderr,"Error - %s\n",strerror(err_three));
                return err_three;
	}*/
	//allocate space for new_word. new_word is a string that stores word
	char *new_word = malloc(width);
	//check for malloc errors
	if(new_word == NULL){
		int err_four = errno;
                fprintf(stderr,"Error # = %d\n",err_four);
                fprintf(stderr,"Error - %s\n",strerror(err_four));
                return err_four;
	}
	//initialize all elements in new_word to empty spaces ' ' by looping through all elements in new_word using the k iterator 
	int k;
	for(k = 0;k < width;k++){
		new_word[k] = ' ';
	}
	// l is an iterator and cgeck is flag that are used in the next loop
	int l = 0;
	int check = 1;
	//while l is less than the number of elements in word and check is 1,set each element of  new_word to corresponf to the specified element in word. If next value has the null terminator, stop, change flag and break from the loop
	while((l < sizeof(word) && check == 1)){
		//set a corrseponding location l in new_word to word
		new_word[l] = word[l];
		//check to see if the next element in word (l+1) is a null terminator. If it is change flag to 0, which will break the loop
		if(word[l+1] == '\0'){
			check = 0;
		}
		//iterate
		l++;
		
	}
	//add new_line to new_word
	new_word[width-1] = '\n';
	//add null terminator to new_word
	new_word[width] == '\0';
	//Start doing binary search stuff here
	//create middle. Middle is the middle value in the binary search algorithm
	int middle;
	//set left of binary search to 0
	int left = 0;
	//set right to binary search to number of lines
	int right = lines;
	//calc is the int used to calculate the sum of left + right
	int calc;
	//old_middle is the old middle value saved if value is not found. It is compared against the old value to see if there is no word in the dictionary
	int old_middle = 0;
	//infinite loop until a return value occurs. Binary Seach happens here
	while(1 ==1){
		//calc gets the sum of left and right
		calc = left + right;
		//middle is calc divided by 2
		middle = calc/2;
		//lseek middle *width times to specified location in fd to set the offset. When you read, you will start reading from the offset
		off_t lseek_error = lseek(fd,middle*width, SEEK_SET);
		//if lseek produces error, preint error number and message and return the error saved in err_five
		if(lseek_error == -1){
			int err_five = errno;
                	fprintf(stderr,"Error # = %d\n",err_five);
                	fprintf(stderr,"Error - %s\n",strerror(err_five));
                	return err_five;
		}
		//read the location that you got from the offset in lseek and read into buffer a width amount of bytes. The buffer is going to be the word at a specific location
		int num_read = read(fd,buffer,width);
		//check to see if read (saved in Num_read) did not egt an error and return a -1. If so print the error code and message and return the error # saved in err_6 
		if(num_read == -1){
			int err_six = errno;
                	fprintf(stderr,"Error # = %d\n",err_six);
                	fprintf(stderr,"Error - %s\n",strerror(err_six));
                	return err_six;
		}
		//compare buffer and new_word and return middle, you found the line where new_word (the input) is located
		if(strcmp(buffer,new_word) == 0){
			//icrement middle (search is zero indexed, return value is 1 indexed, so add a 1 to middle
			middle++;
			//close the file descriptor and check for error in fd_close
			int fd_close = close(fd);
			//free the memory allocated in new_word and buffer
			free(new_word);
			free(buffer);
			if(fd_close == -1){
				int err_sev = errno;
 		        	fprintf(stderr,"Error # = %d\n",err_sev);
        		        fprintf(stderr,"Error - %s\n",strerror(err_sev));
               			return err_sev;
			}
			//return middle
			return middle;
		}
		//if strcmp of buffer and new_word is less than 0, set left to middle +1. First check to see if the old_middle is equal to middle, a tell tale sign that you have not found the word. Return the negated previous middle + 1 if true
		else if(strcmp(buffer,new_word) < 0){
			//ceheck if old_middle is equal to middle
			if(old_middle == middle){
				//increment middle
				middle++;
				//multiply middle by -1
				middle = -1*middle;
				//close fd ( and the file) and check for errors
				int fd_close = close(fd);
				//free new_word and buffer to prevent memory leaks
				free(new_word);
				free(buffer);
				if(fd_close == -1){
					int err_eight = errno;
                			fprintf(stderr,"Error # = %d\n",err_eight);
                			fprintf(stderr,"Error - %s\n",strerror(err_eight));
                			return err_eight;
				}
				//return the negated middle value, aka the last line you seached for new_word
				return middle;
			}
			//old_middle is set to middle
			old_middle = middle;
			//set left to middle +1
			left = middle+1;
		}	
		//if strcmp of buffer and new_word is less than 0, set right to middle - 1. First check to see if the old_middle is equal to middle, a tell tale sign that you have not found the word. Return the negated previous middle + 1 if true
				else if(strcmp(buffer,new_word) > 0){
					//check if old_middle is equal to middle
					if(old_middle == middle){
						//if true increment
						middle++;
						//multiply middle by -1 and save it in itself
						middle =  -1*middle;
						//close fd and check for errors in fd_close. Print and return an error if it exists
						int fd_close = close(fd);
						//free new_word and buffer to prevent memory leaks
						free(new_word);
						free(buffer);
						if(fd_close == -1){
							int err_nin = errno;
							fprintf(stderr,"Error # = %d\n",err_nin);
                					fprintf(stderr,"Error - %s\n",strerror(err_nin));
                					return err_nin;
						}
						//return the negated middle value, aka last line you searched for new_word
						return middle;
					}
					//set old_middle to middle
					old_middle = middle;
					//set right to middle -1
					right = middle-1;
				}	
		}	
}

		
