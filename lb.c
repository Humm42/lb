#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "util.h"

static time_t	sleeptime;

void
runcmd(char **command)
{
	pid_t pid;
	if ((pid = fork()) < 0)
		die("%s: fork():", argv0);
	else if (pid == 0) {
		/* execlp("beep", "beep", NULL); */
		execvp(command[0], command);
		die("%s: execvp():", argv0);
	} else {
		if (waitpid(pid, NULL, 0) < 0) {
			fprintf(stderr, "%s: waitpid(): ", argv0);
			perror(NULL);
		}
	}
}

void
usage(void)
{
	die("usage: %s [-t time] [command [params ...]]", argv0);
}

int
main(int argc, char **argv)
{
	sleeptime = 30;
	ARGBEGIN {
	case 't':
		errno = 0;
		sleeptime = (time_t)strtol(EARGF(usage()), NULL, 10);
		if (errno != 0)
			die("%s: strtol():", argv0);
		if (sleeptime < 0)
			sleeptime = 0;
		break;
	default:
		usage();
	} ARGEND

	char *defcmd[] = {"beep", NULL};
	char **command = argv[0]?argv:defcmd;

	time_t lasttime;
	time(&lasttime);
	int c;
	while ((c = getc(stdin)) != EOF) {
		putchar(c);
		if (c == '\n') {
			if (time(NULL) - lasttime >= sleeptime)
				runcmd(command);
			time(&lasttime);
		}
	}
}
