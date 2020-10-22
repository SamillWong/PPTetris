#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include <sys/time.h>
#include <iostream>

#define TIMING 30000

struct itimerval tv;
struct termios back_attr;

void
quit(void)
{
     tcsetattr(0, TCSANOW, &back_attr);

     return;
}

void
sig_handler(int sig)
{
     switch(sig)
     {
     case SIGTERM:
     case SIGINT:
     case SIGSEGV:
		  std::cout << "SIGSEGV" << std::endl;
		  quit();
          break;
     case SIGALRM:
          tv.it_value.tv_usec -= tv.it_value.tv_usec / 3000;
          setitimer(0, &tv, NULL);
          break;
     }

     return;
}

void
init(void)
{
     struct sigaction siga;
     struct termios term;

     /* Init signal */
     sigemptyset(&siga.sa_mask);
     siga.sa_flags = 0;
     siga.sa_handler = sig_handler;
     sigaction(SIGALRM, &siga, NULL);
     sigaction(SIGTERM, &siga, NULL);
     sigaction(SIGINT,  &siga, NULL);
     sigaction(SIGSEGV, &siga, NULL);

     /* Init timer */
     tv.it_value.tv_usec = TIMING;
     sig_handler(SIGALRM);

     /* Init terminal (for non blocking & noecho getchar(); */
     tcgetattr(STDIN_FILENO, &term);
     tcgetattr(STDIN_FILENO, &back_attr);
     term.c_lflag &= ~(ICANON|ECHO);
     tcsetattr(0, TCSANOW, &term);

     return;
}

void
get_key_event(void)
{
     int c = getchar();

     if(c > 0)
          std::cout << "KEYPRESS" << std::endl;

     return;
}

int
main(int argc, char **argv)
{
     init();

	 int x = 0;

     while(x != 100)
     {
		  x++;
          get_key_event();
		  std::cout << "TEST" << std::endl;
     }

     quit();

     return 0;
}
