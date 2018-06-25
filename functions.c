#include "functions.h"

const int CAPACITY=1024*1024;

char* buffer, *ptr;
int start, end;
int gap;
char c;
struct termios orig_termios, ins_termios, raw;		//Instances of terminal structure

void editorOpen(char* filename) {				//Fun() to read file from cmd argument
	FILE *fp = fopen(filename, "r");
	if(!fp) exit(1);

	end = CAPACITY - 1;
	start = 0;
	gap = CAPACITY;

	buffer = (char*)malloc(1024*1024);
	if( buffer == NULL)
    {
        perror("Unable to allocate buffer");
        exit(1);
    }
	ptr = buffer;
	size_t size = gap;
	ssize_t linelen = 1;
	
	while(1)
	{
		linelen = getline(&ptr, &size, fp);
		if (linelen != -1)
		{
			ptr += linelen;
			start += linelen;
			size -= linelen;
		}
		else break;    	
    }
    gap = size;
    
	fclose(fp);
}