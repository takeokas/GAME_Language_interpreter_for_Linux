/*
	GAME Language interpreter ,32bit Takeoka ver.
	by Shozo TAKEOKA (http://www.takeoka.org/~take/ )
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <setjmp.h>

#define MODULO_OP

#define	MAX_STK	100

u_char text_buf[100000];


/*****/
#define iSnum(c) ('0'<=(c) && (c)<='9')
#define iShex(c) (('0'<=(c) && (c)<='9')||('A'<=(c) && (c)<='F')||('a'<=(c) && (c)<='f'))

u_char lin[256];
u_char *pc;
int  sp,stack[MAX_STK];

jmp_buf toplvl;

int lno;

/*	Var	*/
int var[256-32];

#define VARA(v) var[(v)-0x20]

#define	TOPP	VARA('=')
#define	BTMP	VARA('&')
#define	RAND	VARA('\'')
#define	MOD	VARA('%')

int
getNum( f)
int *f;
{
 int c;
 int n=0;
 *f=0;
 for(;c= *pc;){
	if(!iSnum(c)) break;
	n= n*10 + (c-'0');
	pc++;
	*f=1;
 }
 return n;
}

int
getHex( f)
int *f;
{
 int c;
 int n=0;
 *f=0;
 for(;c= *pc;){
	if(!iShex(c)) break;
	n= n*16 + ((c<'A')?(c-'0'):((c<'a')?(c-'A') :(c-'a'))+10 );
	pc++;
	*f=1;
 }
 return n;
}


newText()
{
	if( *((u_char*)BTMP) != 0xFF){
		xputs("\ntext is locked");
		longjmp(toplvl,1);
	}
	newText1();
}

newText1()
{
	BTMP = TOPP;
	*((u_char*)BTMP) = 0xFF;
}

extern u_char* searchLine();
extern u_char* makeLine();

main()
{
 int n,x;

	mach_init();

	TOPP =(int)text_buf;
	newText1();


	crlf();xputs("--- GAME Interpreter 32bit (95/Jan/01) ---\n");
	xputs("execute '\\' to quit GAME.");
	for(;;){
		setjmp(toplvl);
		sp= -1;
		lno=0;
/*		printf("BTMP=%x\n",BTMP); /**/
		*lin='\0';
 reenter:
		crlf();xputs("G>>");
		xxgets(lin);
		crlf();
		*(lin+strlen(lin)+1) = 0x80; /* EOF on endOfLinebuf*/
		pc=lin;
		skipBlank();
		n=getNum(&x);
		if(x==0)exqt();
		else{
			if(*pc=='\\'){ /* edit the line */
				int f;
				u_char *p;
				p=searchLine(n, &f);
				if(f==0) continue;
				p=makeLine(p);
				strcpy(lin,p);
				goto reenter;
			}
			edit(n);
		}
	}
}

dmp(p,n)
u_char *p;
{
 int i;
 for(i=0;i<n;i++)
	printf("%2x ",*p++);
}

u_char*
skipLine(p)
u_char *p;
{
 for(;*p;)
	p++;
 return p+1;
}

u_char*
searchLine(n, f)
int n;
int *f;
{
 u_char* p;
 int l;

 for(p=(u_char*)TOPP;!(*p & 0x80);){
	l= (*p << 8) | *(p+1);
	if(n==l){ *f=1; return p;}
	if(n< l){ *f=0; return p;}
	p=skipLine(p+2);
 }
 *f=0;
 return p;
}

/* line edit routines */
edit(n)
int n;
{
 u_char *p;
 int f;
	if(n==0){	dispList(TOPP); return 0; }

	p=searchLine(n, &f);
	if(*pc=='/'){ /* list */
		dispList(p);
	}else{ /*edit */
/*		printf("edit:(%d)%d=%s",f,n,pc); */
		if(*((u_char*)BTMP) != 0xFF){
			xputs("Text is locked\n");
			return 0;
		}
		if(f) deleteLine(p);
		if(*pc=='\0') return 0; /* delete line */
		addLine(n,p,pc);
	}
	return 0;
}

addLine(n,p,new)
u_char *p,*new;
{
 int l;
 l= 2+ strlen(new)+1;
 bcopy(p,p+l,(((u_char*)BTMP)-p)+1);
 *p= n>>8;
 *(p+1)= n;
 strcpy(p+2,new);
 BTMP += l;
/* *((u_char*)BTMP) = 0xFF; */
}

deleteLine(p)
u_char *p;
{
 int l;
 l= 2+ strlen(p+2)+1;
 bcopy(p+l,p,(((u_char*)BTMP)-p)-l+1);
 BTMP -= l;
}

/** listing display routine **/
u_char *
makeLine(p)
u_char *p;
{
 int l;
 static u_char buf[256];

 l= (*p << 8) | *(p+1); p+=2;
 sprintf(buf, "%d", l);
 strcat(buf,p);
 return buf;
} 

u_char *
dispLine(p)
u_char *p;
{
 int l;
 u_char b[256];

 l= (*p << 8) | *(p+1); p+=2;
 sprintf(b, "%d", l);
 xputs(b);
 for(;*p;){
	xputc(*p++);
 }
 crlf();
 return p+1;
}

dispList(p)
u_char *p;
{
 for(;!(*p & 0x80);){
	breakCheck();
	p = dispLine(p);
 }
}



skipBlank()
{int x;
	for(;;){
		if((x= *pc) != ' ') return x;
		pc++;
	}
}

skipAlpha()
{int x;
	for(;;){
		x= *pc;
		if((x<'A')||('z'<x)||('Z'<x && x<'a')) return x;
		pc++;
	}
}

/** execute statement **/

exqt()
{int c;
	for(;;){
		c=skipBlank();
		do_cmd();
	}
}

topOfLine()
{
 int x,c;
more:
	x= *pc++;
	if(x & 0x80){
		xputs("\nat EOF ");
		longjmp(toplvl,1);
	}
	lno = (x <<8)| *pc++;

	if(*pc != ' '){ /* Comment */
		pc=skipLine(pc);
		goto more;
	}
}

breakCheck()
{int c;
 if(xkeychk()){
	c=xgetkey();
	if(c == 0x03) longjmp(toplvl,1);
	if(c == 0x13) xgetkey(); /*pause*/
 }
}

do_cmd()
{int c,c1,c2,e,vmode,off;
	breakCheck();
	c= *pc++;
	c1= *pc;
/* printf("%02x ",c); /**/
	switch(c){
	case '\0':
		topOfLine();
		return 1;
	case ']' :
		pc=(u_char*)pop(); return 0;
	case '"' :
		do_pr(); return 0;
	case '/' :
		crlf();return 0;
	case '@' :
		if(c1=='='){break; }
		do_do();return 0;

	case '?' :
		do_prNum(c1);return 0;
	/**/
	case '\\' :
		mach_fin();
	}

	if(c1=='='){
		switch(c){
		case '#' :
			e=operand();do_goto(e);return 0;
		case '!' :
			e=operand();do_gosub(e);return 0;
		case '$' :
			e=operand();do_prChar(e);return 0;
		case '.' :
			e=operand();do_prSpc(e);return 0;
		case ';' :
			e=operand();do_if(e);return 0;
		case '\'' : /*RAND seed */
			e=operand();srand(e);return 0;
		case '@' :
			c2= *(pc+1);e=operand();do_until(e,c2); return 0;
		case '&' :
			e=operand();
			if(e==0){
				newText();
			}
			return 0;
		default:
			/* Variable */
			break;
		}
	}
	vmode=skipAlpha();
/* printf("exp:%02x ",vmode); /**/
	if(vmode==':' || vmode=='(' || vmode=='['){
		pc++;
		off=expr(*pc++);
		if(*(pc-1) !=')'){
			errMsg("var ')' mismatch");
			longjmp(toplvl,1);
		}
		e=operand();
/*printf(" v=%c%c%d)=%d ",c,vmode,off,e); /**/
		switch(vmode){
		case ':' : *(((u_char*)VARA(c)+off))=e; return;
		case '(' : *(((u_int*)VARA(c)+off))=e; return;
		case '[' : *(((u_short*)VARA(c)+off))=e; return;
		}
		return;
	}
	e=operand();
	VARA(c)=e;
/* printf("exp:%02x ",*(pc-1)); /**/
	if(*(pc-1)== ','){ /* For */
			c= *pc++;
			e= expr(c);
/*printf("operandExpr=%d\n",e); /**/
			push(pc);
			push(e);
	}
	return 0;
}

do_until(e,val)
{
/*printf("until:val=%02x,e=%d,ev=%d,pc=%x\n",val, e, stack[sp],stack[sp-1]);/**/
	VARA(val)=e;
	if(e>stack[sp]){
		sp-=2; /*pop pc,value*/
		return ;
	}
	/* repeat */
	pc=(u_char*)stack[sp-1]; /*pc*/
	return ;
}

do_do()
{
	push(pc);
	push(0);
}

do_if(e)
{
 if(e==0){
	pc=skipLine(pc);
	topOfLine();
 }
}

do_goto(n)
{
 int f;
 u_char *p;
	if(n==-1) longjmp(toplvl,1); /* Prog Stop */
	p=searchLine(n, &f);
	pc=p;
	topOfLine();
}

do_gosub(n)
{
 int f;
 u_char *p;
	p=searchLine(n, &f);
	push(pc);
	pc=p;
	topOfLine();
}

do_prSpc(e)
{int i;
 for(i=0;i<e;i++)
	xputc(' ');
}

do_prChar(e)
{
 xputc(e);
}

do_prNum(c1)
{
 u_char buf[256];
 int e,digit;

 if(c1== '('){
	u_char form[256];
	pc++;
	digit=term(c1);
/*printf("prDigi=%c\n", *pc);/**/
	e=operand();
	sprintf(form,"%%%dd",digit);
/*printf("form=%s",form);/**/
	sprintf(buf,form,e);
	xputs(buf);
	return ;
 }

 e=operand();
 switch(c1){
 case '?' :
	sprintf(buf,"%08x",e);
	break;
 case '!' :
	sprintf(buf,"%04x",e & 0xFFFF);
	break;
 case '$' :
	sprintf(buf,"%02x",e & 0xFF);
	break;
 case '=' :
	sprintf(buf,"%d",e);
	break;
 default:
	xputs("unknown cmd\n");
	longjmp(toplvl,1);
 }
 xputs(buf);
}

do_pr()
{
 int x;
	for(;;){
		if('"' == (x= *pc++)) break;
		if(x== '\0'){ pc--;break;}
		xputc(x);
	}
}

pop()
{
	if(sp<0){
		xputs("Stack UnderFlow\n");
		longjmp(toplvl,1);
	}
	return stack[sp--];
}

push(x)
{
	if(sp>=(MAX_STK-1)){
		xputs("Stack OverFlow\n");
		longjmp(toplvl,1);
	}
	return stack[++sp]=x;
}


operand()
{
 int x,e;
	for(;;){
		x= *pc++;
/*printf("operandC=%02x\n",x); /**/
		if(x == '=') break;
		if(!(x & 0xDF)){
			errMsg("No operand expression");
			longjmp(toplvl,1);
		}
	}
	x= *pc++;
	e= expr(x);
/*printf("operandExpr=%d\n",e); /**/
	return e;
}

expr(c)
{
 int o,o1, op1,op2;
 int e;

 e=term(c);

 for(;;){
	o= *pc++;
/*printf("exprC=%02x\n",o); /**/
	switch(o){
	case '\0' :
		pc--;
	case ' ' :
	case ')' :
	case ',' :
		return e;
	case '<' :
		o1= *pc++;
		switch(o1){
		case '>' :
			op2=term(*pc++);
			e= (e!=op2);
			continue;
		case '=' :
			op2=term(*pc++);
			e=(e<=op2);
			continue;
		default:
			op2=term(o1);
			e=(e<op2);
			continue;
		}
	case '>' :
		o1= *pc++;
		switch(o1){
		case '=' :
			op2=term(*pc++);
			e=(e>=op2);
			continue;
		default:
			op2=term(o1);
			e=(e>op2);
			continue;
		}
	case '+' : op2=term(*pc++);e= e+op2;break;
	case '-' : op2=term(*pc++);e= e-op2;break;
	case '*' : op2=term(*pc++);e= e*op2;break;
	case '/' : op2=term(*pc++); MOD=e%op2; e= e/op2;break;
#ifdef MODULO_OP
	case '%' : op2=term(*pc++); MOD=e%op2; e= e%op2;break;
#endif /*MODULO_OP*/

	case '=' : op2=term(*pc++);e= (e==op2);break;

	case '&' : op2=term(*pc++);e= e&op2;break;
	case '|' : op2=term(*pc++);e= e|op2;break;
	case '^' : op2=term(*pc++);e= e^op2;break;
	default:
		xputc(o); errMsg(" unknown operator");
		longjmp(toplvl,1);
	}
 }
}

term(c)
{
 int e,f=0, vmode;
/*printf("termC=%02x\n",c); /**/
 switch(c){
 case '$' :
	e= getHex(&f);
	if(f==0){ /* get Char */
		return xgetc();
	}
/*	printf("hexTerm=%x",e); /**/
	return e;
 case '(' : /*EXPR */
	e=expr(*pc++);
	if(*(pc-1) !=')'){
		errMsg("')' mismatch");
		longjmp(toplvl,1);
	}
	return e;
 case '+' : /*ABS */
	e= term(*pc++);
	return e<0? -e : e;
 case '-' : /* MINUS */
	return -(term(*pc++));
 case '#' : /* NOT */
	return !(term(*pc++));
 case '\'' : /*RAND */
	return rand()%term(*pc++);
#ifdef MODULO_FUNC
 case '%' : /* MOD not yet*/
	return 0;
#endif /*MODULO_FUNC*/
 case '?' : /*input */
	{u_char *ppp, b[256];
		xgets(b);
		ppp=pc;
		pc=b;
		e=expr(*pc++);
		pc=ppp;
		return e;
	}
 case '"' : /*Char const */
	e = *pc++;
	if(*pc++ != '"'){
		errMsg("\" mismatch");
		longjmp(toplvl,1);
	}
	return e;
 }
 if(iSnum(c)){
	pc--; e= getNum(&f);
/*	printf("term=%d",e); /**/
	return e;
 }
/*printf("valiable=%c\n",c); /**/
/* vmode= *pc;*/
 vmode= skipAlpha();
 if(vmode==':' || vmode=='(' || vmode=='['){
	pc++;
	e=expr(*pc++);
	if(*(pc-1) !=')'){
		errMsg("var ')' mismatch");
		longjmp(toplvl,1);
	}
/* printf(" v=%c ",c); /**/
	switch(vmode){
	case ':' : return *(((u_char*)VARA(c)+e));
	case '(' : return *(((u_int*)VARA(c)+e));
	case '[' : return *(((u_short*)VARA(c)+e));
	}
 }
 return VARA(c);
}

errMsg(s)
u_char *s;
{
 char b[10];
 xputs(s);
 if(lno !=0){
	xputs(" in ");
	sprintf(b,"%d", lno);
	xputs(b);
 }
 crlf();
}

/* EOF */
