/*
	lined.c
	line edit routine
		   by
		S.Takeoka (http://www.takeoka.org/~take/ )
*/


#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>

/* #define WORDSTAR /**/
/* #define EMACS   /**/


static del_(char *b, int x);
static ins_(char *b, int x);
static eraEol_(char *b,int cx);
static cback_(char *s);
static linTop(char *s,int x);




#define Ctrl(x) ((x)& 0x1F)

#define CR      '\r'
#define DEL     0x7F
#define BS      Ctrl('H')
#define CTRLC   Ctrl('C')
#define CTRLZ   Ctrl('Z')
#define CAN	Ctrl('X')
#define CAN2	Ctrl('U')


#ifdef WORDSTAR
#define UP      Ctrl('E')
#define DOWN	Ctrl('X')
#define RIGHT   Ctrl('D')
#define LEFT    Ctrl('S')
#define KILL    Ctrl('K')
#define DELC    Ctrl('G')
#define SUSP	Ctrl('Q')
#endif

#ifdef CPM3
#define UP      Ctrl('R')
#define DOWN	Ctrl('X')
#define RIGHT   Ctrl('F')
#define LEFT    Ctrl('A')
#define KILL    Ctrl('K')
#define DELC    Ctrl('G')
#define SUSP	Ctrl('D')
#endif

#ifdef EMACS
#define UP	Ctrl('P')
#define DOWN	Ctrl('N')
#define RIGHT	Ctrl('F')
#define LEFT	Ctrl('B')
#define KILL	Ctrl('K')
#define DELC	Ctrl('D')
#define SUSP	Ctrl('Q')
#endif




#define Kill() \
{\
	eraEol_(b,cx); \
	b[cx]=0; \
	len=strlen(b); \
}

static
zbs(c)
{
	if(c==0){ xputc('\b'); return;}
	if(c<' ') xputc('\b');
	xputc('\b');
}


static
del_(char *b, int x)
{
 char *p;
 p=b+x;
 for(;*p;){
        *p= *(p+1);
        p++;
 }
}


static
ins_(char *b, int x)
{
 char *p,*q;
 int save,a;

 p=b+x;
 if(*p == 0){ *(p+1)=0; return;}

 for(;*p;p++)
        ;
 save=b[x];
 b[x]=0;

 q=p+1;
 a= *p;
 for(;;){
        *q= a;
        p--;q--;
        if((a= *p) ==0) break;
 }
 *q= save;
}


static
eraEol_(char *b,int cx)
{char *s;
 int c;
 s=b+cx;
        for(c=0;*s;s++){
                if(*s<' '){xputc(' '); c++;}
                xputc(' '); c++;
        }
        for(;c;c--) xputc('\b');
}



static
cback_(char *s)
{
        for(;*s;s++){
		zbs(*s);
	}
}

static
linTop(char *s,int x)
{int save;
 save=s[x];
 s[x]=0;
 cback_(s);
 s[x]=save;
}


static
zputs(x)
char *x;
{
 char ss[256],*s;

 for(s=ss;;){
	if(*x == 0){ *s =0; break;}
	if(*x <' '){ *s++= '^'; *s++= *x | 0x40; }
	else	   { *s++= *x; }
	x++;
 }

 xputs(ss);
}


static
zputc(c)
{
	if(c< ' '){ xputc('^'); xputc(c | 0x40);}
	else		{xputc(c); }
}




linEd(b,blen)
char *b;
int blen;
{
 int cx,len;
 int c;

 blen--;

 zputs(b);      /* display the line */
 len=cx=strlen(b);

 for(;;){
        c= xgetkey();
        switch(c){
        case SUSP :
		if(len!=0) continue;
		setdown_tty(1);
		kill(getpid(), SIGTSTP);
		setup_tty(1);
		continue;
        case CTRLC :
		if(len!=0) break; /* charcter insert */
                zputs("^C");
/*		longjmp(ccpWbootEntry,-1); */
		return -1;
        case CR :
                return 0;
        case UP :
		linTop(b,cx);
		cx=0;
		Kill();
                return 1;
/*
        case DOWN :
                return 2;
*/
        case CAN :
        case CAN2 :
		linTop(b,cx);
		cx=0;
		Kill();
		continue;
        case LEFT:
                if(cx <=0) continue;
                zbs(b[cx-1]);
                cx--;
                continue;
        case RIGHT:
                if(b[cx]== '\0') continue;
                zputc(b[cx]);
                cx++;
                continue;

        case KILL :
		Kill();
                continue;

        case DEL:
        case BS:
                if(cx <=0) continue;
                zbs(b[cx-1]);
                cx--;
                /* fall into DELC */
        case DELC :
		c=b[cx];
		del_(b,cx);
		zputs(&b[cx]); /* move cursor to line end */
		if(c<' ')xputc(' ');
		xputc(' ');
		zbs(c);
		cback_(&b[cx]); /* move cursor to insertion point */
		len--;
                continue;
	default:;
        }
/*        if(c<' ') continue; */

	if(len>=blen) continue;
        ins_(b,cx);
        b[cx]=c;
        zputs(&b[cx]);
        cx++;len++;

        cback_(&b[cx]);

 }
}

