	G A M E    I n t e r p r e t e r  $B$?$1$*$+(B32bit$BHG(B

  0.$B$O$8$a$K(B

  $B!V(BGAME$B!W$O!"(B 1978$BG/:"!"Bg@>;a$K$h$j9M0F$5$l!"(B $B7n4)(BASCII$B;o$G$5$+$s$K;H$o(B
$B$l$?!"%W%m%0%i%_%s%08@8l$G$9!#(B

  GAME$B$N8@8l%;%^%s%F%#%C%/%9$O(B BASIC $B$H$[$H$s$IF1$8$G$9!#(B
  $B$,!"$=$N;zLL>e$NJ8K!$O5-9f$rB?MQ$9$k$3$H$K$h$j!"%W%m%0%i%`$r%3%s%Q%/%H(B
$B$KI=8=$9$k$3$H$K@.8y$7$F$$$^$9!#(B
  $B$^$?!"G[NsJQ?t$N;2>H(B/$BA`:n$r9)IW$7$F!"5!3#8l$N4V@\%"%I%l%C%7%s%0$HF1MM(B
$B$JA`:n$r$o$+$j$d$9$/<B8=$7$F$$$^$9!#(BC$B8@8l$,9-$/CN$i$l$F$$$J$+$C$?;~Be$K!"(BC
$BIw$N%a%b%jA`:n$r2DG=$K$7$F$$$^$7$?!#(B
  8bit$B5!A4@*;~Be!"(B6800, 8080, 6502, Z80 $B$J$I!"B?$/$N(BCPU,$B%^%7%s$K0\?"$5$l!"(B
$B6&DL%W%i%C%H%U%)!<%`$H$7$F!"9-$/;H$o$l$^$7$?!#(B


  1.$BK\=hM}7O(B

  $B:#2s!"(BGAME$B%$%s%?!<%W%j%?$r(B C $B$K$h$C$F!"<BAu$7$^$7$?!#(B
  GAME $B$O(B 8bit, 16bit $BA`:n$7$+$J$+$C$?$N$G$9$,!";~Be$r4U$_!":#2s$O(B 32bit
$B$rI8=`E*$J@0?t$NBg$-$5$H$7!"(B16bit$BA`:n$r3HD%$7$^$7$?!#(B


  2.$B<g$J3HD%!"JQ99E@(B

  (1)$BJQ?tL>(B
	$BJQ?tL>$K1Q>.J8;z$r;HMQ2DG=$H$7$?(B

  (2)$B4V@\%a%b%j;2>H(B/$BBeF~(B
	8bit
		a:$B<0(B) $B$G;2>H(B/$BBeF~(B ($B=>MhDL$j(B)
	16bit
		a[$B<0(B) $B$G;2>H(B/$BBeF~(B ($B?7@_(B)
	32bit
		a($B<0(B) $B$G;2>H(B/$BBeF~(B ($B=>Mh$O(B16bit)

  (3)$B?tCM%W%j%s%H=PNO(B
	8bit (16$B?J(B2$B7e(B)
		?$=$B<0(B		($B=>MhDL$j(B)

	16bit (16$B?J(B4$B7e(B)
		?!=$B<0(B		($B?7@_(B)

	32bit (16$B?J(B8$B7e(B)
		??=$B<0(B		($B=>Mh$O(B16bit)


  (4)2$B9`1i;;;R(B
	%	$B>jM>1i;;;R(B	($B?7@_(B)
	|	OR$B1i;;;R(B	($BHsI8=`(B)
	&	AND$B1i;;;R(B	($BHsI8=`(B)
	^	XOR$B1i;;;R(B	($BHsI8=`(B)

  (5)$BMp?t4X?t(B
	GAME$B$G$O!"Mp?t4X?t$O!"$9$Y$F$N=hM}7O$G6&DL$J4X?t$,;HMQ$5$l$F$$$?(B
	$B$,!":#2s$N<BAu$G$O(B C $B%i%$%V%i%j$r;HMQ$7$?!#(B

	'	$B$X$NBeF~$O(B C $B%i%$%V%i%j$N(B srand()$B$N8F$S$@$7$H$J$k(B
	'$B9`(B	$B$O(B C $B%i%$%V%i%j$N(B rand()$B$N8F$S$@$7$H$J$k(B

  (6)$B=hM}7O$N=*N;(B
	(J\(B $B$r<B9T$9$k$H!"(BGAME$B%$%s%?!<%W%j%?$r=*N;$9$k!#(B
        $B9T$N$I$3$+$K(B'\'$B$,$"$j!"$=$l$r<B9T(B($BI>2A(B)$B$9$k$H!"%$%s%?!<%W%j%?$,=*N;!#(B
        ($BNc(B: G>> \[$B%j%?!<%s%-!<2!2<(B] $B$G=*N;(B)

  (7)$B9T%(%G%#%?$N3HD%(B
      $B0l9TF~NO$K$*$$$F$O!"(B
	^F : $BA0?J(B
	^B : $B8eB`(B
	^D : $BKu>C(B
	^H : $BKu>C(B $B8eB`(B
	^K : $B9TKv$^$G>C5n(B
	^U : $B9TKu>C(B
      $B$H$$$&JT=8$,$G$-$k!#(B

  (8)$B%(%G%#%?$N3HD%(B
	$B%3%^%s%I%i%$%s>e$G(B
		$B9THV9f(J\(B		($BNc(B  100(J\(B )
	$B$H$9$k$H!"=jK>$N9T$r9T%(%G%#%?$GJT=8$G$-$k!#(B

  (9)$B$=$NB>(B
	$B<B9TCf$K(B
		^C : $B<B9T$&$A$-$j(B
		^S : $B0l;~Dd;_(B
	$B$G$9!#(B
	$B$=$NB>$O!"(B6800 $BHG(B GAME III$B%$%s%?!<%W%j%?$H$[$\F1$8$K$J$C$F$$$^$9!#(B
	$B%a%b%j4V@\BeF~$N<0$N@)8B$J$I$b$=$N$^$^$G$9!#(B

	== $B$,$J$$!#(B=$B$X$NBeF~$,!"$*4+$a$G$J$$$?$a!#(B
            UNIX$B2<$G$O!"(B=$B$N$?$a$K!"0UL#$N$"$kHVCO$rF@$k$N$,Fq$7$$$G$"$m$&$?$a!#(B

--- EOF
