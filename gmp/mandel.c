#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <gmp.h>
#include "color.h"

int calc_point(mpf_t cx,mpf_t cy, int iterations){
//~ double calc_point(mpf_t cx,mpf_t cy, int iterations){
	mpf_t	zx,zy,zxn,zyn,
			zx2,zy2,
			zx2_fast,t1;
	
	double er=2.0;
	int i,k;
	
	mpf_init(zx);mpf_init(zy);
	mpf_init(t1);
	mpf_init(zxn);mpf_init(zyn);
	mpf_init(zx2);mpf_init(zy2);
	
	mpf_set_d(zx,0.0);
	mpf_set_d(zy,0.0);
	
	for(i=0;i<iterations;i++){
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
	
	mpf_clear(zx);mpf_clear(zy);
	mpf_clear(t1);
	mpf_clear(zxn);mpf_clear(zyn);
	mpf_clear(zx2);mpf_clear(zy2);
	
	return i;
	//~ return mpf_get_d(t1);
} 

int main(){
	int 	i,j,k,l,
			ix,iy,
			screenx,screeny,bailout,
			*intET,*hist;
	mpf_t	zx,zy,zxn,zyn,
			zx2,zy2,
			zx_fast,zy_fast,zxn_fast,zyn_fast,
			zx2_fast,zy2_fast,
			cx,cy,
			dx,dy,
			t1,t2,t3,t4,zz,
			minx,miny,maxx,maxy;
	double	er,sens,z,hue,
			min,max,maxET,total,
			min2,max2,
			coef,n,
			*realET;
	_color	*bitmap,col,*pal;
	
	FILE *img=fopen("mandel.ppm","wt");
	
	pal=(_color*)malloc(sizeof(_color)*255);
	
	populatePal(pal);
	if(pal==NULL){
		fprintf(stderr,"NULL pointer crap!\n");
		return 1;
	}
	
	maxET=0;
	
	mpf_set_default_prec(1024);
	
	mpf_init(zx);mpf_init(zy);
	mpf_init(dx);mpf_init(dy);
	mpf_init(cx);mpf_init(cy);
	mpf_init(t1);mpf_init(t2);mpf_init(t3);mpf_init(t4);mpf_init(zz);
	mpf_init(zx_fast);mpf_init(zy_fast);
	mpf_init(zxn_fast);mpf_init(zyn_fast);
	mpf_init(zx2_fast);mpf_init(zy2_fast);
	mpf_init(zxn);mpf_init(zyn);
	mpf_init(zx2);mpf_init(zy2);
	mpf_init(minx);mpf_init(miny);
	mpf_init(maxx);mpf_init(maxy);
	
	screenx=400;
	screenx=800;
	screenx=1600;
	screenx=2400;
	//~ screenx=4800;
	screeny=screenx*3/4.0;
	er=2.0;
	sens=0.2;
	coef=0.2;
	
	bailout=128*0.125;
	bailout=128*0.25;
	bailout=128*0.5;
	bailout=128*1.0;
	bailout=128*2.0;
	bailout=128*4.0;
	bailout=128*8.0;
	bailout=128*16.0;
	bailout=128*32.0;
	
	//~ bailout=500;
	
	//mpf_set_str(minx,"0.00001",10);
	
	//~ mpf_set_d(minx,-0.743643135-0.00012068/64.0);
	//~ mpf_set_d(minx,-0.743643135-0.00012068/64.0);
	//~ mpf_set_d(maxx,-0.743643135+0.00012068/64.0);
	//~ mpf_set_d(miny,0.131825963-0.00012068/64.0);
	//~ mpf_set_d(maxy,0.131825963+0.00012068/64.0);
	//~ 
	mpf_set_d(minx,-0.743643135-0.000014628/2);
	mpf_set_d(maxx,-0.743643135+0.000014628/2);
	mpf_set_d(miny,0.131825963-0.000014628/2);
	mpf_set_d(maxy,0.131825963+0.000014628/2);
	//~ 
	//~ mpf_set_d(minx,0.16125-0.06125);
	//~ mpf_set_d(maxx,0.16125+0.06125);
	//~ mpf_set_d(miny,0.638438-0.06125);
	//~ mpf_set_d(maxy,0.638438+0.06125);
	//~ 
	//~ mpf_set_d(minx,-0.77269);
	//~ mpf_set_d(maxx,-0.76959);
	//~ mpf_set_d(miny,0.10693);
	//~ mpf_set_d(maxy,0.11003);
	//~ 
	//~ mpf_set_d(minx,-0.165976305907-0.000014628/500000);
	//~ mpf_set_d(maxx,-0.165976305907+0.000014628/500000);
	//~ mpf_set_d(miny,-1.043596114624-0.000014628/500000);
	//~ mpf_set_d(maxy,-1.043596114624+0.000014628/500000);
	//~ 
	//~ mpf_set_d(minx,-2.5);
	//~ mpf_set_d(maxx,1.0);
	//~ mpf_set_d(miny,-1.5);
	//~ mpf_set_d(maxy,1.5);
	
	//~ mpf_set_d(minx,-1.4740397376611-1.6796761995608E-10);
	//~ mpf_set_d(maxx,-1.4740397376611+1.6796761995608E-10);
	//~ mpf_set_d(miny,-0.0022378372064193-1.679676199582E-10);
	//~ mpf_set_d(maxy,-0.0022378372064193+1.679676199582E-10);
	
	//~ mpf_set_d(minx,-0.41334939-0.00023341673);
	//~ mpf_set_d(maxx,-0.41334939+0.00023341673);
	//~ mpf_set_d(miny,-0.59929132-0.00023340057);
	//~ mpf_set_d(maxy,-0.59929132+0.00023340057);
	
	//~ mpf_set_d(minx,-1.25831-0.0157901);
	//~ mpf_set_d(maxx,-1.25831+0.0157901);
	//~ mpf_set_d(miny,-0.353963-0.015789);
	//~ mpf_set_d(maxy,-0.353963+0.015789);
	
	bitmap=	(_color*)malloc(sizeof	(_color	)*screenx*screeny);
	realET=	(double*)malloc(sizeof	(double	)*screenx*screeny);
	intET=	(int*	)malloc(sizeof	(int	)*screenx*screeny);
	hist=	(int*	)malloc(sizeof	(int	)*bailout);
	
	bzero(hist,sizeof(int)*bailout);
	
	fprintf(img,"P3\n%d %d\n255\n",screenx,screeny);
	
	mpf_sub		(dx,maxx,minx);
	mpf_div_ui	(dx,dx,(unsigned long int)screenx);
	mpf_div_ui	(dx,dx,(unsigned long int)2);
	
	mpf_sub		(dy,maxy,miny);
	mpf_div_ui	(dy,dy,(unsigned long int)screeny);
	mpf_div_ui	(dy,dy,(unsigned long int)2);
	
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
			
			mpf_sub(t1,cx,dx);
			mpf_add(t2,cx,dx);
			mpf_sub(t3,cy,dy);
			mpf_add(t4,cy,dy);
			
			i =calc_point(cx,cy,bailout);
			
//			i+=calc_point(t1,cy,bailout);
//			i+=calc_point(t2,cy,bailout);
//			i+=calc_point(cx,t3,bailout);
//			i+=calc_point(cx,t4,bailout);
			
//			i+=calc_point(t1,t3,bailout);
//			i+=calc_point(t1,t4,bailout);
//			i+=calc_point(t2,t3,bailout);
//			i+=calc_point(t2,t4,bailout);
			
//			i/=9;
			
			if(i==bailout){
				intET[iy*screenx+ix]=0;
				//~ realET[iy*screenx+ix]=0;
			}else{
				intET[iy*screenx+ix]=i;
				hist[i]++;
				//~ z=mpf_get_d(zz);
				//realET[iy*screenx+ix]=i+1+(log(log(bailout)-log(log(sqrtf(z))))/log(2));
				//realET[iy*screenx+ix]=1*log2(1.75+i-log2(log2(sqrt(z))))/log2(bailout);
				//realET[iy*screenx+ix]=i-log2(log2(sqrt(z)))/bailout;
				//realET[iy*screenx+ix]=i-log(log(z)/2)/log(2);
				//~ realET[iy*screenx+ix]=i+1.5-log2(log2(sqrt(z)));
				//realET[iy*screenx+ix]=sqrt(realET[iy*screenx+ix]);
				//realET[iy*screenx+ix]=sqrt(realET[iy*screenx+ix]);
				//~ if(realET[iy*screenx+ix]>maxET){
					//~ maxET=realET[iy*screenx+ix];
				//~ }
			}
		}
		
		if(iy%(screeny/25)==0){
			fprintf(stdout," -- %.2f%%\n",((iy)/(double)screeny)*100.0);
		}
	}
	
	fprintf(stdout," -- %.2f%%\n",100.0);
	
	printf("Colouring...\n");
	
	//~ coef=0.001;
	
	//~ for(i=0;i<screeny;i++){
		//~ for(j=0;j<screenx;j++){
			//~ n=0;
			//~ if(i>0) n+=intET[(i-1)*screenx+(j)];
			//~ if(i<screeny-1) n+=intET[(i+1)*screenx+(j)];
			//~ if(j>0)n+=intET[(i)*screenx+(j-1)];
			//~ if(j<screenx-1) n+=intET[(i)*screenx+(j+1)];
			//~ 
			//~ n+=intET[(i-1)*screenx+(j)];
			//~ n+=intET[(i+1)*screenx+(j)];
			//~ n+=intET[(i)*screenx+(j-1)];
			//~ n+=intET[(i)*screenx+(j+1)];
			//~ 
			//~ if(intET[(i)*screenx+(j)]<n/4.0){
				//~ intET[(i)*screenx+(j)]+=(n/4.0)*coef;
			//~ }else{
				//~ intET[(i)*screenx+(j)]-=(n/4.0)*coef;
			//~ }
		//~ }
	//~ }
	
	
	total=0;
	for(i=0;i<bailout;i++){
		total+=hist[i];
	}
	
	//~ printf("%lf\n",total);
	
	for(i=0;i<screeny;i++){
		for(j=0;j<screenx;j++){
			hue = 0.0;
			for(k=0;k<intET[i*screenx+j];k++){
			//~ for(k=0;k<realET[iy*screenx+ix]*255;k++){
				hue+=hist[k]/total;
			}
			
			col=getPalMem(hue,pal);
			//~ 
			//~ realET[i*screenx+j]/=1.5;
			//~ realET[i*screenx+j]=((1.0-exp(-sens*realET[i*screenx+j])));
			//~ col=getGrad2(realET[i*screenx+j]);
			//~ col=getHue(realET[i*screenx+j]);
			//~ col=getPal(realET[i*screenx+j]);
			//~ col=getPalMem(realET[i*screenx+j]*255.0/maxET,pal);
			//~ fprintf(stderr,"%f\n",realET[i*screenx+j]*2500);
			
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
