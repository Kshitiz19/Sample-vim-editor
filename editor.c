#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/types.h>


struct termios orig_termios, ins_termios;		//Instances of terminal structure


int getWindowSize(int *rows, int *cols) {		//Get window resolution from in-built methods
	struct winsize ws;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
  		return -1;
	}
	else {
  		*cols = ws.ws_col;
  		*rows = ws.ws_row;
  		return 0;
	}
}


int main(int argc, char *argv[])
{
	char seq[3];
	int ret;
	initEditor();
	if (argc >= 2) {
    editorOpen(argv[1]);
	}
	
	//printf("rows and cols are %d and %d\n", E.screenrows, E.screencols);
	
	write(STDOUT_FILENO, "\033[?1049h\033[H", 11);
	char *quit = "q";
	tcgetattr(STDIN_FILENO, &orig_termios);
	struct termios raw = orig_termios;
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN);
	raw.c_cc[VMIN] = 1;
	raw.c_cc[VTIME] = 1;

	char c;
	char command[10];
	int i;

	NOR:
		printf("You're in normal mode\n");
		
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

		while(1){
			read(STDIN_FILENO, &c, 1);
			if(c==58) {
				write(STDOUT_FILENO, "\x1b[s", 4);
				goto CMD;
				}
			if(c=='a') goto INS;
			printf("The key that u pressed is %c(%d)\n", c, c);
		}

	CMD:
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
		write(STDOUT_FILENO, "\x1b[39B", 5);
		write(STDOUT_FILENO, "\x1b[2K", 4);
		write(STDOUT_FILENO, "\r:", 1);
		while(1) {
			i=-1;
			write(STDOUT_FILENO, "\x1b[2K", 4);
			write(STDOUT_FILENO, "\x1b[3B:", 5);
			
			while (read(STDIN_FILENO, &c, 1) && c != '\n')
				{
					if(c == 27) {
						write(STDOUT_FILENO, "\x1b[2K", 4);
						write(STDOUT_FILENO, "\x1b[1A", 4);
						write(STDOUT_FILENO, "\x1b[2K", 4);			
						write(STDOUT_FILENO, "\x1b[u", 3);
						goto NOR;
					}
					command[++i] = c;
				}
					command[++i] = 0;

					if(0 == strcmp(command, quit)) goto E;
					write(STDOUT_FILENO, "Unrecognized command!!", 22);
					sleep(2);
					write(STDOUT_FILENO, "\x1b[2K\x1b[1A\x1b[2K\r", 13);
			}
	
	INS:
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
		printf("You're in insert mode:\n");			
		while(1) {
			while (ret = read(STDIN_FILENO, seq, 3)) {
				
				//printf("ret is %d and c is %d\n", ret, seq[2]);
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
						}
						break;

					case 1:
						switch(seq[0])
						{							
							case 27:
								goto NOR;			//case 127:
						
							default:
								write(STDOUT_FILENO, seq, 1);
						}
						break;

					default:
						write(STDOUT_FILENO, seq, 1);

				}
			}
		}


	E:
	printf("\033[?1049l");
	return 0;
}