#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "color.h"

int main(){
	int 	screenx,screeny,
			ix,iy,
			i,j,k,
			er,bailout;
	double	zx,zy,zxn,zyn,cx,cy,
			sens,
			minx,maxx,miny,maxy,
			xcenter,ycenter,size;
	
	FILE *img=fopen("mandel.ppm","wt");
	_color *bitmap,col;
	 int *escapetime;
	
	screenx=1600/2;
	screeny=1200/2;
	bailout=5000;
	sens=0.02;
	er=2;
	
	size=0.005;
	xcenter=-0.13856524454488;
	ycenter=-0.64935990748190;
	
	minx=xcenter-size;
	maxx=xcenter+size;
	miny=ycenter-size;
	maxy=ycenter+size;
	
	printf("%f \t %f\n%f\t %f\n",minx,maxx,miny,maxy);
	
	minx=-2.5;
	maxx=1.5;
	miny=-2.0;
	maxy=miny*-1;
	
	//~ escapetime=(double*)malloc(sizeof(double)*screenx*screeny);
	escapetime=(int*)malloc(sizeof(int)*screenx*screeny);
	fprintf(img,"P3\n%d %d\n255\n",screenx,screeny);
	
	for(iy=0;iy<screeny;iy++){
		cy=miny+iy*(maxy-miny)/screeny;
		for(ix=0;ix<screenx;ix++){
			cx=minx+ix*(maxx-minx)/screenx;
			zx=0;
			zy=0;
			for(i=0;i<bailout;i++){
				zxn=zx*zx-zy*zy+cx;
				zyn=2.0*zx*zy+cy;
				zx=zxn;
				zy=zyn;
				
				if(zx*zx+zy*zy>er*er){
					escapetime[iy*screenx+ix]=i;
					//~ printf("%d\n",i);
					break;
				}
			}
			if(i==bailout-1){
				escapetime[iy*screenx+ix]=0.0;
			}
		}
		if(iy%(screeny/10)==0){
			fprintf(stderr," -- %.2f%%\n",(iy/(double)screeny)*100.0);
		}
	}
	
	fprintf(stderr," -- %.2f%%\n",100.0);
	fprintf(stderr," <---- DONE ---->\n",100.0);
	fprintf(stderr," Writing to disk!\n",100.0);
	
	for(i=0;i<screeny;i++){
		for(j=0;j<screenx;j++){
			col.r=escapetime[i*screenx+j];
			fprintf(img,"%d %d %d ",(int)col.r*7,
									(int)col.r*7,
									(int)col.r*7);
		}
		fputc('\n',img);
	}
	
	fclose(img);
	free(escapetime);
	
	fprintf(stderr," -- Bye\n");
	
	return EXIT_SUCCESS;
}
