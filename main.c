#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<time.h>
#include<linux/input.h>
#include<string.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<ctype.h>

#define KEY_RELEASE 0
#define KEY_PRESS 1
#define KEY_KEEPING_PRESSED 2

#include "parse.h"

static void printcode(int codes) {
	struct parse_event *p;
	for (p = key_events; p->name != NULL; p++) {
		if (p->value == (unsigned) codes) {
			printf("scancode %s (0x%02x)\n", p->name, codes);
			return;
		}
	}
	if (isprint(codes)) {
		printf("scancode '%c' (0x%02x)\n", codes, codes);
	} else {
		printf("scancode 0x%02x\n", codes);
	}
}


int main()
{

	//static const *char ascii_lookup_table[256] = { KEY_RESERVED, KEY_ESC, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0, KEY_MINUS, KEY_EQUAL, KEY_BACKSPACE, KEY_TAB, KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P, KEY_LEFTBRACE, KEY_RIGHTBRACE, KEY_ENTER, KEY_LEFTCTRL, KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K, KEY_L, KEY_SEMICOLON, KEY_APOSTROPHE, KEY_GRAVE, KEY_LEFTSHIFT, KEY_BACKSLASH, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N, KEY_M, KEY_COMMA, KEY_DOT, KEY_SLASH, KEY_RIGHTSHIFT, KEY_KPASTERISK, KEY_LEFTALT, KEY_SPACE, KEY_CAPSLOCK, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_NUMLOCK, KEY_SCROLLLOCK, KEY_KP7, KEY_KP8, KEY_KP9, KEY_KPMINUS, KEY_KP4, KEY_KP5, KEY_KP6, KEY_KPPLUS, KEY_KP1, KEY_KP2, KEY_KP3, KEY_KP0, KEY_KPDOT, NULL, KEY_ZENKAKUHANKAKU, KEY_102ND, KEY_F11, KEY_F12, KEY_RO, KEY_KATAKANA, KEY_HIRAGANA, KEY_HENKAN, KEY_KATAKANAHIRAGANA, KEY_MUHENKAN, KEY_KPJPCOMMA, KEY_KPENTER, KEY_RIGHTCTRL, KEY_KPSLASH, KEY_SYSRQ, KEY_RIGHTALT, KEY_LINEFEED, KEY_HOME, KEY_UP, KEY_PAGEUP, KEY_LEFT, KEY_RIGHT, KEY_END, KEY_DOWN, KEY_PAGEDOWN, KEY_INSERT, KEY_DELETE, KEY_MACRO, KEY_MUTE, KEY_VOLUMEDOWN, KEY_VOLUMEUP, KEY_POWER, KEY_KPEQUAL, KEY_KPPLUSMINUS, KEY_PAUSE, KEY_SCALE, KEY_KPCOMMA, KEY_HANGEUL, KEY_HANJA, KEY_YEN, KEY_LEFTMETA, KEY_RIGHTMETA, KEY_COMPOSE, KEY_STOP, KEY_AGAIN, KEY_PROPS, KEY_UNDO, KEY_FRONT, KEY_COPY, KEY_OPEN, KEY_PASTE, KEY_FIND, KEY_CUT, KEY_HELP, KEY_MENU, KEY_CALC, KEY_SETUP, KEY_SLEEP, KEY_WAKEUP, KEY_FILE, KEY_SENDFILE, KEY_DELETEFILE, KEY_XFER, KEY_PROG1, KEY_PROG2, KEY_WWW, KEY_MSDOS, KEY_COFFEE, KEY_ROTATE_DISPLAY, KEY_CYCLEWINDOWS, KEY_MAIL, KEY_BOOKMARKS, KEY_COMPUTER, KEY_BACK, KEY_FORWARD, KEY_CLOSECD, KEY_EJECTCD, KEY_EJECTCLOSECD, KEY_NEXTSONG, KEY_PLAYPAUSE, KEY_PREVIOUSSONG, KEY_STOPCD, KEY_RECORD, KEY_REWIND, KEY_PHONE, KEY_ISO, KEY_CONFIG, KEY_HOMEPAGE, KEY_REFRESH, KEY_EXIT, KEY_MOVE, KEY_EDIT, KEY_SCROLLUP, KEY_SCROLLDOWN, KEY_KPLEFTPAREN, KEY_KPRIGHTPAREN, KEY_NEW, KEY_REDO, KEY_F13, KEY_F14, KEY_F15, KEY_F16, KEY_F17, KEY_F18, KEY_F19, KEY_F20, KEY_F21, KEY_F22, KEY_F23, KEY_F24, KEY_PLAYCD, KEY_PAUSECD, KEY_PROG3, KEY_PROG4, KEY_DASHBOARD, KEY_SUSPEND, KEY_CLOSE, KEY_PLAY, KEY_FASTFORWARD, KEY_BASSBOOST, KEY_PRINT, KEY_HP, KEY_CAMERA, KEY_SOUND, KEY_QUESTION, KEY_EMAIL, KEY_CHAT, KEY_SEARCH, KEY_CONNECT, KEY_FINANCE, KEY_SPORT, KEY_SHOP, KEY_ALTERASE, KEY_CANCEL, KEY_BRIGHTNESSDOWN, KEY_BRIGHTNESSUP, KEY_MEDIA, KEY_SWITCHVIDEOMODE, KEY_KBDILLUMTOGGLE, KEY_KBDILLUMDOWN, KEY_KBDILLUMUP, KEY_SEND, KEY_REPLY, KEY_FORWARDMAIL, KEY_SAVE, KEY_DOCUMENTS, KEY_BATTERY, KEY_BLUETOOTH, KEY_WLAN, KEY_UWB, KEY_UNKNOWN, KEY_VIDEO_NEXT, KEY_VIDEO_PREV, KEY_BRIGHTNESS_CYCLE, KEY_BRIGHTNESS_AUTO, KEY_DISPLAY_OFF, KEY_WWAN, KEY_RFKILL, KEY_MICMUTE};

//	printf("\nMy simple C program\n\n");
//
//	FILE *fp;
//	char path[1035];

	//fp = popen("cat /proc/bus/input/devices", "r");
//	fp = popen("cat /proc/bus/input/devices | grep -i '^[N: | U:]'", "r");
//	if (fp == NULL) {
//		printf("Failed to run command\n");
//		exit(1);
//	}

//	while (fgets(path, sizeof(path), fp) != NULL) {
//		printf(" %s\n", path);
//	}

//	pclose(fp);

	char* keyboard = "/dev/input/event0";
	FILE* f = fopen(keyboard, "r");
	if (f == NULL) {
		perror("Keyboard input device not found");
		exit(1);
	}

	//struct input_event evt;
	//size_t sz;

//	while ((sz = fread(&evt, sizeof(struct input_event), 1, f) != 0)) {

//		printf("Key: %d\n", evt.code);
		// Evt code relates to table from linux pages https://github.com/torvalds/linux/blob/master/include/uapi/linux/input-event-codes.h	
		//KeySym one = XKeysymToString(event.)
	
//	}

	int fd;
	struct input_event evt[64];

	if (( fd = open(keyboard, O_RDONLY)) < 0) {
		perror("Cannot read keyboard device input");
		return 1;
	}

	while (1) {

		size_t readbyte = read(fd, evt, sizeof(evt));

		if (readbyte < (int) sizeof(struct input_event)) {
			perror("Input too short to read");
			return 1;
		}

		for(int i = 0; i < (int) (readbyte / sizeof(struct input_event)); i++) {
			if (EV_KEY == evt[i].type) {
				if ((evt[i].value == KEY_PRESS) || (evt[i].value == KEY_KEEPING_PRESSED)) {
					printf("%d\n", evt[i].code);
					printcode(evt[i].code);
					//printf("%s\n", key_events[evt[i].code ]);
				}
			}
		}
	}




	return 0;
}
