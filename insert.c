#include "functions.h"

extern struct termios raw;
extern char c;

void insert()
{
	char seq[3];
	int ret;

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
	printf("You're in insert mode:\n");			
	while(1)
	{
		ret = read(STDIN_FILENO, seq, 3);

		switch(ret)
		{
			case 3:
				switch(seq[2])
				{
					case 65:
						write(STDOUT_FILENO, "\x1b[A", 3);
						break;
					case 66:
						write(STDOUT_FILENO, "\x1b[B", 3);
						break;
					case 67:
						write(STDOUT_FILENO, "\x1b[C", 3);
						break;
					case 68:
						write(STDOUT_FILENO, "\x1b[D", 3);
						break;
					default:
						break;
				}
				break;

			case 1:
				switch(seq[0])
				{
					case 127:
						write(STDOUT_FILENO, "\x1b[D \x1b[D", 7);
						break;						
					case 27:
						c = 27;
						goto EXIT;			//case 127:					
					default:
						write(STDOUT_FILENO, seq, 1);
				}
				break;

			default:
				write(STDOUT_FILENO, seq, 1);
				break;
		}
	}
	EXIT:
		c=c;
}