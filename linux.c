/***
	GAME interpriter
	machine dependent part
	for Linux 2.6
 ***/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/select.h>
#include <signal.h>
#include <setjmp.h>
#include <termio.h>

static struct  termio  old_tty;
static struct  termio  new_tty;


extern jmp_buf toplvl;

static void terminate()
{
 xputs("\nGAME killed.\n");
 mach_fin();
}

mach_init()
{
 setup_tty(1);
 signal(SIGTERM, terminate);
 signal(SIGHUP, terminate);
}


mach_fin()
{
 xputs("Bye bye.\n");
 setdown_tty(1);
 exit(0);
}





setup_tty(fd)
{

 if(ioctl(fd, TCGETA, &old_tty) < 0) return -1;
 new_tty = old_tty;
 new_tty.c_iflag = 0;
 new_tty.c_oflag = 0;
 new_tty.c_cflag = old_tty.c_cflag;
 new_tty.c_lflag = 0;
 new_tty.c_line = old_tty.c_line;
 new_tty.c_cc[VMIN] = 1;
 new_tty.c_cc[VTIME] = 0;
 if(ioctl(fd, TCSETAW, &new_tty) < 0) return -1;

/* if(fcntl(0, F_SETFL, O_NONBLOCK) < 0){ */
 if(fcntl(0, F_SETFL, FNDELAY) < 0){
   printf("fctl failed\n");
   return -1;
 }
#ifdef XXX
#endif XXX
}

setdown_tty(fd)
{
  if(ioctl(fd,TCSETAW, &old_tty) < 0) return -1;
}


/*
	I/O
 */


xkeychk()
{int n;
 fd_set rfds;
 struct timeval to;

 /* if(ioctl(0,FIONREAD,&n) < 0) return -1; */

 FD_ZERO(&rfds);
 FD_SET(0, &rfds);
 to.tv_sec = 0; to.tv_usec = 0;
 n = select(FD_SETSIZE, &rfds, NULL, NULL, &to);
 if(n == -1){
	xputs("select() error\r\n"); return -1;
 }
if(FD_ISSET(0,&rfds)){ xputs("."); fflush(stdout); }
 return FD_ISSET(0,&rfds);
}

static
waittty()
{int n;
 fd_set rfds;

 FD_ZERO(&rfds);
 FD_SET(0, &rfds);
 n = select(FD_SETSIZE, &rfds, NULL, NULL, NULL);
 if(n == -1){
	xputs("select() error\r\n"); return -1;
 }
 return FD_ISSET(0,&rfds);
}

xgetkey()
{unsigned char c;
  int x;
 fflush(stdout);
 waittty(); /**/
 x=read(0,&c,1);
 if(x == 0) return 0;
 return c;
}

xputc(c)
{
	putchar(c);
}

/* gets */
static u_char old_buf[256];

xgets(b)
u_char *b;
{
 *b='\0';
  xxgets(b);
}

xxgets(b)
u_char *b;
{
 int x;
 for(;;){
	x=linEd(b,256);
	if(x==0){ strcpy(old_buf,b); return; }
	if(x==1){ strcpy(b,old_buf); continue; }
	if(x==-1){ longjmp(toplvl,1);}

 }
}


/*
	almost common routine
*/

xputs(s)
u_char *s;
{
 int c;
 for(;(c= *s++) !='\0';){
	if(c =='\n'){
		xputc('\r');
	}
	xputc(c);
 }
}

/* */
xgetc()
{
 int c;
 c=xgetkey();
 if(c>=32) xputc(c);
}


crlf()
{
 xputs("\n");
}


/* EOF */
