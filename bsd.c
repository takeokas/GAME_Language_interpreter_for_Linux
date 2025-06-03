/***
	GAME interpriter
	machine dependent part
	for FreeBSD
 ***/

#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <setjmp.h>

extern jmp_buf toplvl;


static struct sgttyb old_tty,new_tty;

static terminate()
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
	if(ioctl(fd, TIOCGETP, &old_tty) < 0) return -1;
	new_tty=old_tty;
/*	new_tty.sg_flags |= CBREAK; */
	new_tty.sg_flags |= RAW;
	new_tty.sg_flags &= ~(ECHO|CRMOD);
	if(ioctl(fd, TIOCSETN, &new_tty) < 0) return -1;

}

setdown_tty(fd)
{
	if(ioctl(fd, TIOCSETN, &old_tty) < 0) return -1;
}


/*
	I/O
 */
xkeychk()
{long n;
 ioctl(0,FIONREAD,&n);
 return n;
}


xgetkey()
{unsigned char c;
 fflush(stdout);
 read(0,&c,1);
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

#ifdef XXX
xgets(b)
u_char *b;
{
 u_char *p;
 int c;

 p=b;

 for(;;){
 	c=xgetkey();
/*printf("%02x ",c);*/
 	switch(c){
 	case '\n' :
		crlf();
 		*p ='\0'; return;
 	case '\b' :
		xputc('\b');
 		p--; continue;
	default:
		if(c>=32)xputc(c);
 		*p++ =c; continue;
	}
 }
}
#endif XXX

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
