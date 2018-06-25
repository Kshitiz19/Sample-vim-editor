#include "functions.h"

extern char* ptr, *buffer;
extern char c;
extern struct termios orig_termios, ins_termios, raw;

void normal()
{	
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
	while(1){
		read(STDIN_FILENO, &c, 1);
		if(c==58) {
			write(STDOUT_FILENO, "\x1b[s", 4);			//Saving cursor position
			break;
			}
		else if(c=='a') break;
		else printf("The key that u pressed is %c(%d)\n", c, c);
	}
}