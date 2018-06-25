#include "functions.h"

extern char *buffer, *ptr;
extern int start, end, gap;
extern struct termios orig_termios, ins_termios, raw;
extern char c;

int main(int argc, char *argv[])
{		
	write(STDOUT_FILENO, "\033[?1049h\033[H", 11);
	
	tcgetattr(STDIN_FILENO, &orig_termios);
	raw = orig_termios;
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN);
	raw.c_cc[VMIN] = 1;
	raw.c_cc[VTIME] = 1;


	if (argc >= 2) {
    editorOpen(argv[1]);
	write(STDOUT_FILENO, buffer, ptr-buffer+1);
	}

	NOR:
		normal();
		if(c == 58) goto CMD;
		else goto INS;

	CMD:
		command();
		if(c == 27) goto NOR;
		else if(c == 0) goto E;
	
	INS:
		insert();
		goto NOR;

	E:
	printf("\033[?1049l");
	return 0;
}