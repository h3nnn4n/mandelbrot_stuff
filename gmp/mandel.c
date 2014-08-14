#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <gmp.h>
#include "color.h"

int main(){
	int 	i,j,k,l,
			ix,iy,
			screenx,screeny,bailout;
	mpf_t	zx,zy,zxn,zyn,
			zx2,zy2,
			zx_fast,zy_fast,zxn_fast,zyn_fast,
			zx2_fast,zy2_fast,
			cx,cy,
			t1,t2,t3,
			minx,miny,maxx,maxy;
	double	er,sens,z,
			min,max,maxET,
			min2,max2,
			*realET;
	int *intET;
	_color *bitmap,col,*pal;
	
	FILE *img=fopen("mandel.ppm","wt");
	
	pal=(_color*)malloc(sizeof(_color)*255);
	
	populatePal(pal);
	if(pal==NULL){
		fprintf(stderr,"NULL pointer crap!\n");
		return 1;
	}
	
	maxET=0;
	
	mpf_set_default_prec(64);
	
	mpf_init(zx);mpf_init(zy);
	mpf_init(cx);mpf_init(cy);
	mpf_init(t1);mpf_init(t2);mpf_init(t3);
	mpf_init(zx_fast);mpf_init(zy_fast);
	mpf_init(zxn_fast);mpf_init(zyn_fast);
	mpf_init(zx2_fast);mpf_init(zy2_fast);
	mpf_init(zxn);mpf_init(zyn);
	mpf_init(zx2);mpf_init(zy2);
	mpf_init(minx);mpf_init(miny);
	mpf_init(maxx);mpf_init(maxy);
	
	//screenx=4800;
	//screenx=2400;
	screenx=1600;
	//screenx=800;
	screeny=screenx*3/4.0;
	er=2.0;
	sens=0.002;
	
	bailout=128*0.125;
	bailout=128*0.25;
	bailout=128*0.5;
	bailout=128*1.0;
	bailout=128*2.0;
	bailout=128*4.0;
	bailout=128*8.0;
	bailout=128*16.0;
	bailout=128*16.0;
	
	//mpf_set_str(minx,"0.00001",10);
	
	
	mpf_set_d(minx,-0.743643135-0.00012068/64.0);
	mpf_set_d(maxx,-0.743643135+0.00012068/64.0);
	mpf_set_d(miny,0.131825963-0.00012068/64.0);
	mpf_set_d(maxy,0.131825963+0.00012068/64.0);
	
	//~ mpf_set_d(minx,-0.743643135-0.000014628/2);
	//~ mpf_set_d(maxx,-0.743643135+0.000014628/2);
	//~ mpf_set_d(miny,0.131825963-0.000014628/2);
	//~ mpf_set_d(maxy,0.131825963+0.000014628/2);
	
	/*mpf_set_d(minx,0.16125-0.06125);
	mpf_set_d(maxx,0.16125+0.06125);
	mpf_set_d(miny,0.638438-0.06125);
	mpf_set_d(maxy,0.638438+0.06125);*/
	
	//~ mpf_set_d(minx,-2.5);
	//~ mpf_set_d(maxx,1.0);
	//~ mpf_set_d(miny,-1.5);
	//~ mpf_set_d(maxy,1.5);
	
	bitmap=	(_color*)malloc(sizeof	(_color	)*screenx*screeny);
	intET=	(int*	)malloc(sizeof	(int	)*screenx*screeny);
	realET=	(double*)malloc(sizeof	(double	)*screenx*screeny);
	
	fprintf(img,"P3\n%d %d\n255\n",screenx,screeny);
	
	for(iy=0;iy<screeny;iy++){
		mpf_sub(cy,maxy,miny);
		mpf_div_ui(cy,cy,(unsigned long int)screeny);
		mpf_mul_ui(cy,cy,(unsigned long int)iy);
		mpf_add(cy,miny,cy);
		for(ix=0;ix<screenx;ix++){
			mpf_sub(cx,maxx,minx);
			mpf_div_ui(cx,cx,(unsigned long int)screenx);
			mpf_mul_ui(cx,cx,(unsigned long int)ix);
			mpf_add(cx,minx,cx);
			
			mpf_set_d(zx,0.0);
			mpf_set_d(zy,0.0);
			
			for(i=0;i<bailout;i++){
				mpf_mul(zx2,zx,zx);
				mpf_mul(zy2,zy,zy);
				mpf_sub(zxn,zx2,zy2);
				mpf_add(zxn,zxn,cx);
				
				mpf_mul_ui(zyn,zx,2);
				mpf_mul(zyn,zyn,zy);
				mpf_add(zyn,zyn,cy);
				
				mpf_set(zx,zxn);
				mpf_set(zy,zyn);
				
				mpf_add(t1,zx2,zy2);
				k=mpf_cmp_d(t1,er*er);
				
				if(k>0){
					break;
				}
			}
			
			if(i==bailout){
				intET[iy*screenx+ix]=0;
				realET[iy*screenx+ix]=0;
			}else{
				intET[iy*screenx+ix]=i;
				z=mpf_get_d(t1);
				//realET[iy*screenx+ix]=i+1+(log(log(bailout)-log(log(sqrtf(z))))/log(2));
				//realET[iy*screenx+ix]=1*log2(1.75+i-log2(log2(sqrt(z))))/log2(bailout);
				//realET[iy*screenx+ix]=i-log2(log2(sqrt(z)))/bailout;
				//realET[iy*screenx+ix]=i-log(log(z)/2)/log(2);
				realET[iy*screenx+ix]=i+1.5-log2(log2(sqrt(z)));
				//realET[iy*screenx+ix]=sqrt(realET[iy*screenx+ix]);
				//realET[iy*screenx+ix]=sqrt(realET[iy*screenx+ix]);
				if(realET[iy*screenx+ix]>maxET){
					maxET=realET[iy*screenx+ix];
				}
			}
		}
		
		if(iy%(screeny/25)==0 || iy==screeny-1){
			fprintf(stdout," -- %.2f%%\n",((iy)/(double)screeny)*100.0);
		}
	}
	
	for(i=0;i<screeny;i++){
		for(j=0;j<screenx;j++){
			//realET[i*screenx+j]/=1.5;
			//realET[i*screenx+j]=((1.0-exp(-sens*realET[i*screenx+j])));
			//col=getGrad2(realET[i*screenx+j]);
			//col=getHue(realET[i*screenx+j]);
			//col=getPal(realET[i*screenx+j]);
			col=getPalMem(realET[i*screenx+j]*255.0/maxET,pal);
			//fprintf(stderr,"%f\n",realET[i*screenx+j]*2500);
			
			//~ fprintf(img,"%d %d %d ",(int)(col.r*255),(int)(col.g*255),(int)(col.b*255));
			fprintf(img,"%d %d %d ",(int)col.r,(int)col.g,(int)col.b);
		}
		fputc('\n',img);
	}
	
	mpf_clear(zx);mpf_clear(zy);
	mpf_clear(cx);mpf_clear(cy);
	mpf_clear(t1);mpf_clear(t2);mpf_clear(t3);
	mpf_clear(zxn);mpf_clear(zyn);
	mpf_clear(zx2);mpf_clear(zy2);
	mpf_clear(minx);mpf_clear(miny);
	mpf_clear(maxx);mpf_clear(maxy);
	
	return EXIT_SUCCESS;
}
