#include "functions.h"

extern struct termios orig_termios;
extern char c;

void command()
{
	char comm[10];
	char *quit = "q";
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
		write(STDOUT_FILENO, "\x1b[99B", 5);
		write(STDOUT_FILENO, "\x1b[2K", 4);
		write(STDOUT_FILENO, "\r:", 1);
		while(1) {
			int i=-1;
			write(STDOUT_FILENO, "\x1b[2K", 4);
			write(STDOUT_FILENO, "\x1b[3B:", 5);
			
			while (read(STDIN_FILENO, &c, 1) && c != '\n')
				{
					if(c == 27) {
						write(STDOUT_FILENO, "\x1b[2K", 4);
						write(STDOUT_FILENO, "\x1b[1A", 4);
						write(STDOUT_FILENO, "\x1b[2K", 4);			
						write(STDOUT_FILENO, "\x1b[u", 3);
						break;
					}
					comm[++i] = c;
				}
					comm[++i] = 0;

					if(0 == strcmp(comm, quit))
					{
						c = 0;
						break;
					}
					write(STDOUT_FILENO, "Unrecognized command!!", 22);
					sleep(2);
					write(STDOUT_FILENO, "\x1b[2K\x1b[1A\x1b[2K\r", 13);
			}
}