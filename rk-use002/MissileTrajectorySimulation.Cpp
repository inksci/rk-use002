//
//       ͨ�õ�������������(�����)V1.0
//
//
/*

ͨ�õ�������������V1.0
�������ΪvxLand�ṩ�ľ�����ͨ�õ������ڵ���������ȵĵ��������������������޿ص�ҩ�� 
�û�������Ҫ���ݼ������㷨�����������ȵ����������޸ġ��������Ӧ���пء��Ƶ�ģ����浯���� 
��

�������MissileTrajectorySimulationRk.h��rk()�Ǿ����4���������������΢�ַ��̵���ֵ�㷨 
�ĺ����ӳ��򣬴�ҿɲο�ʹ�á�

����ʱ���ϵ����Ӧ��matlab���԰汾��ʱû������������

�ؼ��ʣ����� ���� �ڵ� ����� ���㷨 �������� �޿� �п� �Ƶ� ���� ģ�� ���� ͨ��

MissileTrajectorySimulation_Input.dat�����������ļ�

*/

/******************************************
*   (c) Copyright vxLand 2006,2007,2008
*	y(0)	-	t	(s)
*	y(1)	-	v	(m/s)
*	y(2)	-	Theta (degree)
*	y(3)	-	x	(m)
*	y(4)	-	y	(m)
*	y(5)	-	m   (kg)
*******************************************/
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include "MissileTrajectorySimulation.h"

double acx[22][2],acy[10][2],y[NumbVars],dy[NumbVars],y00[NumbVars];
double s,alpha;
void read_array(double *array,int n1,int n2,FILE *fp);
void read_data(void);
double insert(double w,int n,double ax[][2]);
void dery(double xy[NumbVars]);
void rk(double);

#include "MissileTrajectorySimulationRk.h"

void dery(double xy[NumbVars]){
	double xf,yf,mach,q,rho;
	const double T0=289.05,p0=99991.5,R=287.053;
	double T,ps,sonic;

	T = T0 - 0.0063 * xy[4];
	ps = p0 * pow( T/T0 , 5.4246);
	sonic = sqrt( 1.4*R*T );
	rho = ps / ( R * T );
	q = 0.5*rho*xy[1]*xy[1];
	mach = xy[1] / sonic ;

	alpha = 0.0;
	xf=q*s*insert(mach,22,acx);
	yf=q*s*alpha*insert(mach,10,acy);
	dy[0] = 1.0 ;
	dy[1] = (0.0-xf-G*xy[5]*sin(xy[2]))/xy[5];
	dy[2] = yf/(xy[5]*xy[1])-G*cos(xy[2])/xy[1];
	dy[3] = xy[1]*cos(xy[2]);
	dy[4] = xy[1]*sin(xy[2]);
	dy[5] = 0.0;
}

void main() {
	int i;
	double v_time=0.0;
	FILE *fp;

	fp=fopen("MissileTrajectorySimulation_Output.dat","w"); // �������ļ�
	read_data();
	y00[2] = y00[2] * ToRadian;

	for(i=0;i<NumbVars;i++) y[i]=y00[i];

	fprintf(fp,"  T(s)   V(m/s)   Theta     X(m)     Y(m)\n");
	while( y[4] >= 0. )  { // ���߶ȴ���0

		fprintf(fp,"%6.2f%9.2f%8.2f%10.3f%10.3f\n",
			v_time,y[1],y[2]*ToDegree,y[3],y[4]); // д�뵽��������ļ�

		rk(RUN_PACE);
		v_time+=RUN_PACE; // �ۻ�ʱ��   			
	}

	fprintf(fp,"%6.2f%9.2f%8.2f%10.3f%10.3f\n",
		v_time,y[1],y[2]*ToDegree,y[3],y[4]);

	fclose(fp);
	printf("ͨ�õ�������������(�����)V1.0    BY vxLand   2008\n\n�������!\n");getch();
}

double insert(double w,int n,double ax[][2]) {
  double x;int i;
  if( w < ax[0][0] || w > ax[n-1][0])
{ printf("\tERROR: Mach is out! \n\tMach = %10.3f , V = %9.2f, t = %6.2f\n",w,y[1],y[0]);
		exit(-1); }
  for(i=0;w>ax[i][0];) i++;
  if(i==0) i=1;
  if(i==n-1) i=n-2;
  x=ax[i-1][1]*(w-ax[i][0])*(w-ax[i+1][0])
		 /((ax[i-1][0]-ax[i][0])*(ax[i-1][0]-ax[i+1][0])) +
	 ax[i][1]*(w-ax[i-1][0])*(w-ax[i+1][0])
		 /((ax[i][0]-ax[i-1][0])*(ax[i][0]-ax[i+1][0])) +
	 ax[i+1][1]*(w-ax[i-1][0])*(w-ax[i][0])
		 /((ax[i+1][0]-ax[i-1][0])*(ax[i+1][0]-ax[i][0]));
  return(x); }


// ���׸ú�����read_data�е�ʹ�ü��ɣ�����Ҫ�˽�����ԭ��
void read_array(double *array,int n1,int n2,FILE *fp){
	int i,j;float q;
	for(i=0;i<n1;i++)
	for(j=0;j<n2;j++) { 
		fscanf(fp,"%f",&q); 
		*array++ = (double)q;
	} 
}

void read_data(void){ // ���ⲿ�ļ���ȡ����

	// Ҫ���������Ϊ��acx, acy, v1->d->s, y00
	// y00: y�ĳ�ʼֵ

	double d;
	float v1;

	FILE *fp;
	fp=fopen("MissileTrajectorySimulation_Input.dat","r"); // ���������ļ�

	read_array((double *)acx,22,2,fp); // (���� ������ ������ �ļ�)
	read_array((double *)acy,10,2,fp); // ������ȡ֮����������������¿�ʼ����

	// ��ȡһ����ʹ��ԭʼ����
	fscanf(fp,"%f",&v1); // (�ļ� �������� ����) 
	
	d= (float)v1;
	s=d*d*3.1415926/4.;

	read_array(y00,1,NumbVars,fp);

	fclose(fp); 
}