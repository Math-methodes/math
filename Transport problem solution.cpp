#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <dos.h>
#include <time.h>
#define MAXC 500
#define MAXR 500
#define MT 250000
/* GLOBAL VARIABLE DECLARATION */
int NOSUP, NODEM, CP, CT, IT, IPR, N9,
MENUNEXT, cc;
int MOVEX[MT],MOVEY[MT], OPT, MINI, MINJ, NOWI, NOWJ, INDIC;
float C[MAXR][MAXC],FLOW[MAXR][MAXC], SUPPLY[MAXR], S1[MAXR];
float DEMAND[MAXC], D1[MAXC], U[MAXR],V[MAXC], EC[MAXR + 1][MAXC + 1];
float R[MAXR][MAXC];
float TCOST, CMIN, XMIN, CT2;
int P[500][2 * MAXC + 5];//
int ITERATE;
int ID, IDES, IF1, IOPTIMAL, ISOU, IX, IY, LT, NR;
double CC, QT, TT, XINFCC;
char SW, ch;
const char *plant[] ={ "S1","S2","S3","S4","S5","S6","S7","S8","S9","S10","S11","S12","S13","S14","S15","S16","S17","S18","S19","S20","S21","S22","S23","S24","S25","S26","S27","S28","S29","S30","S31","S32","S33","S34","S35","S36","S37","S38","S39","S40"};
const char *depot[] ={"D1","D2","D3","D4","D5","D6","D7","D8","D9","D10","D11","D12","D13","D14","D15","D16","D17","D18","D19","D20","D21","D22","D23","D24","D25","D26","D27","D28","D29","D30","D31","D32","D33","D34","D35","D36","D37","D38","D39","D40"};
FILE *fp;
int IERROR;
char ERR_MSG[100];
void INPUT_MODE()
{
	int i, j, response;

	 printf("\n\n\n");
	 printf("HOW MANY SOURCES? ");
	 scanf_s("%d",&NOSUP);
	 // printf("LABEL SOURCES AND PRESS ENTER ");
	 printf("HOW MANY DESTINATIONS? ");
	 scanf_s("%d",&NODEM);
	 MENUNEXT = 1;
	 //printf("LABEL DESTINATIONS AND PRESS ENTER");
	//for (i = 1;i<NODEM;i++) scanf("%s", depot[i]);
printf("\n\n");
printf("*******************************************\n");
printf(" HOW MUCH COST IS REQUIRED\n");
printf(" BETWEEN EACH SOURCE ANDDESTIANTION \n");
printf("********************************************\n");
for (i = 1; i <= NOSUP; i++)
{
 do
 {
 printf("\n\n");
 printf(" from %s \n", plant[i - 1]);
 for (j = 1; j <= NODEM; j++)
 {
 printf(" to %s = ", depot[j - 1]);
 scanf_s("%f", &C[i][j]);
 printf("\n");
 }
 printf("\n");
 printf(" IS THE DATA CORRECT ? <Y/N>\n");
 getchar();
 response = getchar();
 } while (response == 'N' || response == 'n');
}
printf("\n\n");
printf("*******************************************\n");
printf(" WHAT AMOUNT IS POSSIBLE\n");
printf(" FOR EACH SOURCE \n");
printf("********************************************\n");
do
{
 for (i = 1; i <= NOSUP; i++)
 {
 printf(" %s = ",plant[i - 1]);
 scanf_s("%f", &SUPPLY[i]);
 printf("\n");
}
printf(" IS THE DATA CORRECT ? <Y/N>\n");
getchar();
response = getchar();
} while (response == 'N' || response == 'n');
printf("\n\n");
printf("*******************************************\n");
printf(" WHAT AMOUNT IS REQUIRED \n");
printf(" FOR EACH DESTINATION \n");
printf("********************************************\n");
do
{
 for (j = 1; j <= NODEM; j++)
 {
 printf(" %s = ",depot[j - 1]);
 scanf_s("%f", &DEMAND[j]);
 printf("\n");
 }
 printf(" IS THE DATA CORRECT? <Y/N> \n");
 getchar();
 response = getchar();
} while (response == 'N' || response == 'n');
}
void Optimal();
 void Seek_Path(int I,int J);
 void Increase(int I,int J);
 void TCost();
 /*BALANCE8*/
 void BALANCE()
 {
  float ss,ds,diff;
  int i,j;
  ss = 0.0;
  ds = 0.0;
  for (i = 1; i <= NOSUP; i++) ss = ss + SUPPLY[i];
  for (j = 1; j <= NODEM; j++) ds += DEMAND[j];
  N9 = NODEM;
  diff = ss - ds;
  if (diff == 0.0) return;
  if (diff < 0.0)
  {
  printf("The total supply must be greater than the total demand.\n");
  printf(" So this problem is infeasible.\n");
  printf("Check the data.\n");
  printf(ERR_MSG," The total supply must be greater than the total demand.");
  IERROR = 3;
  return;
  }
  NODEM += 1;
  for (i = 1; i <= NOSUP; i++) C[i][NODEM] = 1E+20;
  DEMAND[NODEM] = diff;
  if (IPR > 1)
  {
  if (diff == 0)
  {
  printf("\n The original problem is balanced."); return;
  }
  if (diff > 0)
  {
  printf("\n The unbalanced problem is now balanced.");
  return;
  }
  }
  return;
  }
 
 void LCR()
 {
 int i,j,icom,MINI,MINJ;
 float CMIN;
 for (i = 1; i <= NOSUP; i++)
 for (j = 1; j <= NODEM; j++)
 FLOW[i][j] = 0;
 for (i = 1; i <= NOSUP; i++) S1[i] = SUPPLY[i];
 for (j = 1; j <= NODEM; j++) D1[j] = DEMAND[j];
 do
 {
 icom = 0;
 CMIN = 1E+20;
 MINI = 0;
 MINJ = 0;
 for (i = 1; i <= NOSUP; i++)
 if (S1[i] > 0.00001)
 {
 icom = 1;
 for (j = 1; j <= NODEM; j++)
 if (D1[j] > 0.00001)
 {
 icom = 1;
 if (CMIN > C[i][j])
 {
 CMIN = (C[i][j]);
 MINI = i;
 MINJ = j;
 }
 }
 }

 if (icom == 1)
 {
 if (fabs(S1[MINI] - D1[MINJ]) < 0.00001)
 {
 FLOW[MINI][MINJ] = S1[MINI]; S1[MINI] = 0;
D1[MINJ] = 0;
 }
if ((fabs(S1[MINI] - D1[MINJ]) >= 0.00001) && (S1[MINI]
< D1[MINJ]))
 {
 FLOW[MINI][MINJ] = S1[MINI]; D1[MINJ] -=
S1[MINI]; S1[MINI] = 0;
 }
if ((fabs(S1[MINI] - D1[MINJ]) >= 0.00001) && (S1[MINI]
> D1[MINJ]))
 {
 FLOW[MINI][MINJ] = D1[MINJ]; S1[MINI] -=
D1[MINJ]; D1[MINJ] = 0;
 }
 }
} while (icom == 1);
if (icom == 0) return;
}
 /*VOGEL*/
 void INISOL()
 {
 int cnt;
 int BLINK,linet,ptr,p;
 int valid,char_count;
 int i,j;
 for (i = 1; i <= NOSUP; i++)
 for (j = 1; j <= NODEM; j++) EC[i][j] = 0;
 switch (1)
 {
 case 1: LCR(); break;
 }
 TCOST = 0.0;
 for (i = 1; i <= NOSUP; i++)
 for (j = 1; j <= NODEM; j++)
 {
 if (FLOW[i][j] == 0) continue;
 EC[i][j] = 1; CT++;
 // TCOST = TCOST + CT++;
 TCOST = TCOST + (C[i][j] * FLOW[i][j]);
 }
 if (CT < CP) printf("\n The initial solution is not feasiable.");
 else printf("\nThe initial solution is feasiable.");
 }
void Optimal() {
	//Labels: e10, e70, e140, e150
	 int I,J;
	 ITERATE = 1;
	e10: XINFCC = 0.0;
	 for (I = 1; I <= NOSUP; I++)
	 for (J = 1; J <= NODEM; J++) {
	 if (FLOW[I][J] != 0.0) goto e70;
	 Seek_Path(I,J);
	 Increase(I,J);

	e70:;
	 }
if (XINFCC >= 0.0) {
 IOPTIMAL = 1;
 goto e150;
 }
 for (I = 1; I <= LT; I++) {
 IX = P[3][I]; IY = P[4][I];
 if (I % 2 == 0) {
 FLOW[IX][IY] -= TT;
 goto e140;
 }
 FLOW[IX][IY] += TT;
e140:;
}
e150:if (IOPTIMAL == 0) {
 ITERATE = ITERATE + 1;
 goto e10;
}
}
void Seek_Path(int I, int J) {
	//Labels: e70, e160, e260
	 int I1,I2;
	 for (I1 = 1; I1 <= NOSUP; I1++)
	 for (I2 = 1; I2 <= NODEM; I2++)
	 R[I1][I2] = FLOW[I1][I2];
	 for (I1 = 1; I1 <= NOSUP; I1++) R[I1][0] = 0.0;
	 for (I2 = 1; I2 <= NODEM; I2++) R[0][I2] = 0.0;
	 R[I][J] = 1.0;
	e70: for (I2 = 1; I2 <= NODEM; I2++) {
	 if (R[0][I2] == 1.0) goto e160;
	 NR = 0;
	 for (I1 = 1; I1 <= NOSUP; I1++)
	 if (R[I1][I2] != 0.0) NR++;
	 if (NR != 1) goto e160;
	 for (I1 = 1; I1 <= NOSUP; I1++) R[I1][I2] = 0.0;
	 R[0][I2] = 1.0; IF1 = 1;
	e160:;
	}
for (I1 = 1; I1 <= NOSUP; I1++) {
if (R[I1][0] == 1.0) goto e260;
NR = 0;
for (I2 = 1; I2 <= NODEM; I2++)
if (R[I1][I2] != 0.0) NR++;
if (NR != 1) goto e260;
for (I2 = 1; I2 <= NODEM; I2++) R[I1][I2] = 0.0;
R[I1][0] = 1.0; IF1 = 1;
e260:;
}
if (IF1 == 1) {
IF1 = 0; goto e70;
}
}
void Increase(int I, int J) {
	//Labels: e20,e70,e130,e170,e180,e230
	 int I1,I2;
	 P[1][1] = I; P[2][1] = J; IX = I; IY = J; ID = 1; CC = 0.0;
	QT = 999999.0;
	e20: ID++; IF1 = 0;
	for (I1 = 1; I1 <= NOSUP; I1++) {
	 if (R[I1][IY] == 0.0 || I1 == IX) goto e70;
	 P[1][ID] = I1; P[2][ID] = IY; IX = I1; CC -= C[IX][IY];
	 IF1 = 1; I1 = NOSUP;
	 if (FLOW[IX][IY] < QT && FLOW[IX][IY] > 0.0)
	QT = FLOW[IX][IY];
	e70:;
	}
if (IF1 == 0) goto e170;
ID++; IF1 = 0;
for (I2 = 1; I2 <= NODEM; I2++) {
 if (R[IX][I2] == 0.0 || I2 == IY) goto e130;
 P[1][ID] = IX; P[2][ID] = I2; IY = I2; CC += C[IX][IY];
 IF1 = 1; I2 = NODEM;
e130:;
}
if (IF1 == 0) goto e170;
if (IX != I || IY != J) goto e20;
goto e180;
e170:printf(" DEGENERATE SOLUTION !\n");
 return;
e180:if (CC > 0.0 || CC > XINFCC) goto e230;
 TT = QT; XINFCC = CC; ID--; LT = ID;
 for (I1 = 1; I1 <= ID; I1++) {
 P[3][I1] = P[1][I1]; P[4][I1] = P[2][I1];
 }
e230:;
}
void TCost() {
int I,J;
CT2 = 0.0;
for (I = 1; I <= NOSUP; I++)
for (J = 1; J <= NODEM; J++) {
CT2 = CT2 + (FLOW[I][J] * C[I][J]);
if (FLOW[I][J] == 0.0) goto e10;
 printf(" FROM SOURCE%d TODESTINATION%d: %8.2f\n", I, J, FLOW[I][J]);
e10:;
}
printf("\n TOTAL TRANSPORT COST: %10.1f\n\n",
CT2);
}
/*main*/

int main()
{
 int i,j,menu10;
 char AB;
 int DATAOPT;
 menu10:
 TCOST = 0;
 //do
 //{
 INPUT_MODE();
 CP = NOSUP + NODEM - 1; CT = 0;
 BALANCE();
 INISOL();
 printf("\n\n");
 for (i = 1; i <= NOSUP; i++) {
 for (j = 1; j <= NODEM; j++)
 {

 printf("%f",FLOW[i][j]);
 printf(" ");
 }
 printf("\n");
 }
 printf("Total cost = %f\n\n",(TCOST));
 printf("OPTIMAL SOLUTION OF LEAST COST RULE\n\n");
 Optimal();
 TCost();
 printf("\n\n NUMBER OF ITERATION = %d\n\n", ITERATE);
 printf("PRESS 1 TO GO TO MENU? ");
 scanf_s("%d",&DATAOPT);
 if (DATAOPT == 1) goto menu10;
 // }while((DATAOPT == 'M') || (DATAOPT =='m'));
 system("PAUSE");
 return 0;
}