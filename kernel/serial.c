#include <mpx/io.h>
#include <mpx/serial.h>
#include <sys_req.h>

#define COM_LIMIT 5
#define MAX_STR_LEN 100

enum uart_registers {
	RBR = 0,	// Receive Buffer
	THR = 0,	// Transmitter Holding
	DLL = 0,	// Divisor Latch LSB
	IER = 1,	// Interrupt Enable
	DLM = 1,	// Divisor Latch MSB
	IIR = 2,	// Interrupt Identification
	FCR = 2,	// FIFO Control
	LCR = 3,	// Line Control
	MCR = 4,	// Modem Control
	LSR = 5,	// Line Status
	MSR = 6,	// Modem Status
	SCR = 7,	// Scratch
};

typedef struct history {
	char command[COM_LIMIT][MAX_STR_LEN];
	int com_num;
	int position;
} history;

history logged_history;

static int initialized[4] = { 0 };

static int serial_devno(device dev)
{
	switch (dev) {
	case COM1: return 0;
	case COM2: return 1;
	case COM3: return 2;
	case COM4: return 3;
	}
	return -1;
}

int serial_init(device dev)
{
	int dno = serial_devno(dev);
	if (dno == -1) {
		return -1;
	}
	outb(dev + IER, 0x00);	//disable interrupts
	outb(dev + LCR, 0x80);	//set line control register
	outb(dev + DLL, 115200 / 9600);	//set bsd least sig bit
	outb(dev + DLM, 0x00);	//brd most significant bit
	outb(dev + LCR, 0x03);	//lock divisor; 8bits, no parity, one stop
	outb(dev + FCR, 0xC7);	//enable fifo, clear, 14byte threshold
	outb(dev + MCR, 0x0B);	//enable interrupts, rts/dsr set
	(void)inb(dev);		//read bit to reset port
	initialized[dno] = 1;
	return 0;
}

int serial_out(device dev, const char *buffer, size_t len)
{
	int dno = serial_devno(dev);
	if (dno == -1 || initialized[dno] == 0) {
		return -1;
	}
	for (size_t i = 0; i < len; i++) {
		outb(dev, buffer[i]);
	}
	return (int)len;
}


int serial_poll(device dev, char *buffer, size_t len)
{
	size_t cursor = 0;
	
	size_t bytesRead = 0; 

	// Uses the provided size of the buffer and ensures the length of the content
	// does not exceed the allowable size
	while (bytesRead < len)
	{
		
		// captures the entire byte and stores it in a char variable
		char lsr = inb(dev + LSR);
		// Performs bitmasking to see if data is available and ANDs
		// the data between the dev data and 0x01 if the last bit is 1 enter if
		if (lsr & 0x01)
		{
			// returns a single byte
			unsigned char c = inb(dev + RBR);
			
			
			// Handles \r and \n cases for <ENTER> key
			if (c == '\r' || c == '\n')
			{
				buffer[bytesRead] = '\0'; // terminates at the same index the <ENTER> key was read
				int index = logged_history.com_num % COM_LIMIT;
				strcpy(logged_history.command[index], buffer);

				logged_history.com_num++;

				logged_history.position = logged_history.com_num;
				outb(COM1 + THR, '\r');
				outb(COM1 + THR, '\n');
				return bytesRead;
			}
			
			// Handles Backspace
			else if (c == 0x08 || c == 0x7F)
			{
				int is_DEL = 0;
				delete_handler(buffer, &bytesRead, &cursor, &is_DEL);
			}

			// Handles if the arrow is pressed. The first part is ESC hex. Also handles delete case char sequence ESC [ ~ D
			else if (c == 0x1B)
			{
				arrow_handler(buffer, &cursor, &bytesRead, dev, c);
			}

			else if (c >= 32 && c <= 126)
			{
				// Changes buffer if inserted mid line
				for (size_t i = bytesRead; i > cursor; i--)
				{
					buffer[i] = buffer[i - 1];
				}
				buffer[cursor] = c;
				outb(COM1 + THR, c);
				cursor++;
				bytesRead++;

				// Redraws line elements for mid line elements
				for (size_t i = cursor; i < bytesRead; i++)
				{
					outb(COM1 + THR, buffer[i]);
				}

				// changes cursor postion back to the correct position
				for (size_t i = cursor; i < bytesRead; i++)
				{
					outb(COM1 + THR, '\b');
				}
			}
		}
	}

	buffer[bytesRead] = '\0';
	return bytesRead;
}

char* strcpy(char destination[], char source[])
{
	size_t i = 0;

	while (source[i] != '\0')
	{
		destination[i] = source[i];
		i++;
	}

	destination[i] = '\0';
	return destination;
}


void arrow_handler(char* buffer, size_t* cursor, size_t* bytesRead, device dev, char starting_byte)
{

	char possibilities[4]; // Array to store 3 bytes of user input

	possibilities[0] = starting_byte;
	while(!(inb(dev + LSR) & 0x01)); // Waits till second byte is available
	possibilities[1] = inb(dev + RBR);

	while(!(inb(dev + LSR) & 0x01)); // Waits till third byte is available
	possibilities[2] = inb(dev + RBR);

	if (possibilities[1] == '[') // Follows a sequence ot check whether an arrow key, or delete key was pressed
	{
		switch(possibilities[2])
		{
			case 'A':
			{
				
				if (logged_history.com_num == 0)
				{
					break;
				}
				if (logged_history.position > 0)
				{
					logged_history.position--;
				}

				int index = logged_history.position % COM_LIMIT;


				while (*cursor > 0)
				{
					outb(COM1 + THR, '\b');
					(*cursor)--;
				}

				for (size_t i = 0; i < *bytesRead; i++)
				{
					outb(COM1 + THR, ' ');
				}
				for(size_t i = 0; i < *bytesRead; i++)
				{
					outb(COM1 + THR, '\b');
				}
				strcpy(buffer, logged_history.command[index]);
				size_t len = 0;
				while (buffer[len] != '\0')
				{
					len++;
				}

				*bytesRead = len;
				*cursor = len;

				for (size_t i = 0; i < *bytesRead; i++)
				{
					outb(COM1 + THR, buffer[i]);
				}
				break;
			}

			case 'B':
				if (logged_history.com_num == 0)
				{
					break;
				}
				if (logged_history.position < logged_history.com_num)
				{
					logged_history.position++;
				}
				if (logged_history.position == logged_history.com_num)
				{
					size_t old_len = *bytesRead;
					while (*cursor > 0)
					{
						outb(COM1 + THR, '\b');
						(*cursor)--;
					}
					for (size_t i = 0; i < old_len; i++)
					{
						outb(COM1 + THR, ' ');
					}
					for (size_t i = 0; i < old_len; i++)
					{
						outb(COM1 + THR, '\b');
					}

					buffer[0] = '\0';
					*bytesRead = 0;
					*cursor = 0;
					break;
				}

				int index = logged_history.position % COM_LIMIT;


				while (*cursor > 0)
				{
					outb(COM1 + THR, '\b');
					(*cursor)--;
				}
				for (size_t i = 0; i < *bytesRead; i++)
				{
					outb(COM1 + THR, ' ');
				}
				for(size_t i = 0; i < *bytesRead; i++)
				{
					outb(COM1 + THR, '\b');
				}

				strcpy(buffer, logged_history.command[index]);
				size_t len = 0;
				while (buffer[len] != '\0')
				{
					len++;
				}

				*bytesRead = len;
				*cursor = len;

				for (size_t i = 0; i < *bytesRead; i++)
				{
					outb(COM1 + THR, buffer[i]);
				}
				break;
			case 'C':
				if (*cursor < *bytesRead)
					{
						outb(COM1 + THR, buffer[*cursor]);
						(*cursor)++;
					}
				break;
			case 'D':
				if (*cursor > 0) // Ensures user is not trying to delete past 0
				{
					outb(COM1 + THR, '\b');
					(*cursor)--;
				}
				break;
			
			case '3':
				while(!(inb(dev + LSR) & 0x01)); // Waits till fourth byte is available
				possibilities[3] = inb(COM1 + RBR);
				if (possibilities[3] == '~')
				{
					int is_DEL = 1;
					delete_handler(buffer, bytesRead, cursor, &is_DEL);
				}
				break;
			default:
				return;
				
		}
	}
	return;
}

void delete_handler(char* buffer, size_t *bytesRead, size_t* cursor, int* is_DEL)
{
	// Returns if the cursor is at the beginning of the line and there is nothing to delete
	if (*is_DEL == 0 && *cursor == 0)
	{
		return;
	}
        
    // Returns if DEL key is pressed and the cursor is ahead. Essentially, don't do anything if cursor is in front of most recent output
    if (*is_DEL == 1 && *cursor >= *bytesRead)
	{
        return;
	}
	

	if (*is_DEL == 0) // Executes if the delete key is pressed
	{
		outb(COM1 + THR, '\b'); // Sets the cursor back a single char/byte
		(*cursor)--;
	}

	// Uses the cursor position and shifts the items that are in front of the deleted item backwards in the buffer
	for (size_t i = *cursor; i < *bytesRead - 1; i++)
	{
		buffer[i] = buffer[i+1]; // Shifts item in buffer back an index based on cursor positioning
	}

	(*bytesRead)--; 
	

	// Changes terminal display
	for (size_t i = *cursor; i < *bytesRead; i++)
	{
		outb(COM1 + THR, buffer[i]); // Redisplays all the values in the buffer shifted down from deleted character to user
	}
	outb(COM1 + THR, ' '); // Blanks last character
	for (size_t i = *cursor; i <= *bytesRead; i++)
	{
		outb(COM1 + THR, '\b'); // Moves cursor back to correct postion
	}
}



