static char *g_version = "@(!--#) @(#) fieldentry.c, sversion 0.1.0, fversion 004, 11-december-2025";

/*
 *  edit a field on screen and return the value on stdout
 *
 */

/**********************************************************************/

/*
 *  includes
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <poll.h>
#include <sys/stat.h>

/**********************************************************************/

/*
 *  defines
 */

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define MAX_ALLOWED_VALUE_LENGTH 10

#define POLL_TIMEOUT 10

/**********************************************************************/

/*
 *  globals
 */

char *g_progname;

/**********************************************************************/

void usage()
{
	fprintf(stderr, "%s: usage: %s \"value\" max_value_length\n", g_progname, g_progname);

	exit(2);
}

/**********************************************************************/

char *basename(char *s)
{
        /* locals */
        char    *bn;

        bn = s;

        while (*s != '\0') {
                if (*s == '/') {
                        if (*(s+1) != '\0') {
                                bn = s+1;
                        }
                }

                s++;
        }

        return bn;
}

/**********************************************************************/

int issafeascii(char c)
{
	if (((int)c >= 32) && ((int)c <= 126)) {
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

int allsafeascii(char *s)
{
	while (*s != '\0') {
		if (! issafeascii(*s)) {
			return FALSE;
		}

		s++;
	}

	return TRUE;
}

/**********************************************************************/

/*
 *  Main
 */

int main(int argc, char *argv[])
{
	/* locals */
	char			*value;
	int			max_length;
	struct termios		original_dev_tty_options;
	struct termios		dev_tty_options;
	int			dev_tty;
	char			buffer[MAX_ALLOWED_VALUE_LENGTH];
	int			bufpos;
	int			i;
	struct pollfd		ttyfd[1];
	char			ibuf[1];
	char			erasebuf[3];
	int			pollretcode;
	int			n;
	int			c;

	g_progname = basename(argv[0]);

	if (argc != 3) {
		usage();    /* usage() function also exit's */
	}

	value = argv[1];
	max_length = atoi(argv[2]);

	if (! allsafeascii(value)) {
		fprintf(stderr, "%s: the value argument has one or more \"non-safe\" ASCII characters\n", g_progname);
		exit(2);
	}

	if (max_length <= 0) {
		fprintf(stderr, "%s: maximum value length \"%s\" is 0, negative or otherwise invalid\n", g_progname, argv[2]);
		exit(2);
	}

	if (max_length > MAX_ALLOWED_VALUE_LENGTH) {
		fprintf(stderr, "%s: maximum value length %d exceeds allowable maximum of %d\n", g_progname, max_length, MAX_ALLOWED_VALUE_LENGTH);
		exit(2);
	}

	if (strlen(value) > max_length) {
		fprintf(stderr, "%s: length of value argument %ld exceeds %d characters\n", g_progname, strlen(value), max_length);
		exit(2);
	}

	if ((dev_tty = open("/dev/tty", O_RDWR)) == -1) {
		fprintf(stderr, "%s: unable to open /dev/tty\n", g_progname);
		exit(2);
	}

	/* save /dev/tty options and then put /dev/tty into cooked mode */
	tcgetattr(dev_tty, &original_dev_tty_options);
	tcgetattr(dev_tty, &dev_tty_options);
	cfmakeraw(&dev_tty_options);
	tcsetattr(dev_tty, TCSANOW, &dev_tty_options);

	/* copy the string into the buffer */
	bufpos = strlen(value);

	for (i = 0; i < bufpos; i++) {
		buffer[i] = value[i];
	}

	/* display the string */
	write(dev_tty, buffer, bufpos);

	/* set up the erase buffer for deleting the character to the left of the cursor */
	erasebuf[0] = 8;
	erasebuf[1] = 32;
	erasebuf[2] = 8;

	/* loop */
	while (TRUE) {
		/* set up poll for /dev/tty */
		ttyfd[0].fd      = dev_tty;
		ttyfd[0].events  = POLLIN;

		/* do the poll */
		pollretcode = poll(ttyfd, (nfds_t)1, POLL_TIMEOUT);

		/* error is poll fails */
		if (pollretcode < 0) {
			fprintf(stderr, "\n%s: poll failure\n", g_progname);
			break;
		}

		/* if /dev/tty not ready loop again */
		if (pollretcode == 0) {
			continue;
		}
		
		/* read one character from keyboard */
		n = read(dev_tty, ibuf, 1);

		/* error if read fails */
		if (n < 0) {
			fprintf(stderr, "\n%s: read failure\n", g_progname);
			break;
		}

		/* error if no characters read as poll said there was a minimum of 1 character ready to be read */
		if (n == 0) {
			fprintf(stderr, "\n%s: read should have returned 1 character\n", g_progname);
			break;
		}

		/* also error is read returns 2 or more characters as we only asked for one! */
		if (n > 1) {
			fprintf(stderr, "\n%s: read returned more than 1 character\n", g_progname);
			break;
		}

		/* extract the character */
		c = ibuf[0];

		/* if return or newline then exit loop */
		if ((c == 10) || (c == 13)) {
			break;
		}

		/* if delete or backspace then delete last character */
		if ((c == 127) || (c == 8)) {
			if (bufpos > 0) {
				write(dev_tty, erasebuf, 3);

				bufpos--;
			}

			continue;
		}

		/* if safe ASCII character then add to buffer and echo it to /dev/tty */
		if (issafeascii(c)) {
			if (bufpos < max_length) {
				buffer[bufpos] = c;

				write(dev_tty, buffer + bufpos, 1);

				bufpos++;
			}

			continue;
		}

		/* ignore any unsafe ASCII - I know that is cheating :-] */
	}

	/* restore /dev/tty options from saved copy */
	tcsetattr(dev_tty, TCSAFLUSH, &original_dev_tty_options);

	/* if any characters in the buffer output them onto stdout */
	if (bufpos > 0) {
		for (i = 0; i < bufpos; i++) {
			putchar(buffer[i]);
		}
	}

	/* flush stdout */
	fflush(stdout);

	/* return program success */
	return 0;
}

/**********************************************************************/

/* end of file: fieldentry.c */
