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
 * Convert input stream key code to printable character 
 */
static void print_code(int code) {
	struct parse_event *p;
	for (p = key_events; p->name != NULL; p++) {
		if (p->value == (unsigned) code) {
			printf(" %s (0x%02x)\n", p->name, code);
			return;
		}
	}
  // Only print 'printable' characters
	if (isprint(code)) {
		printf("'%c' (0x%02x)\n", code, code);
	} else {
		printf("0x%02x\n", code);
	}
}

int main() {

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
        // Essentially skips the events fired before and after a key press
        // this way only the pressed key value is shown
				if ((evt[i].value == KEY_IS_PRESSED) || (evt[i].value == KEY_KEEPING_PRESSED)) {
					// printf("%d\n", evt[i].code);
					print_code(evt[i].code);
				}
			}
		}
	}
	return 0;
}
