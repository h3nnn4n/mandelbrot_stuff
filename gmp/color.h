#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct{
	double r,g,b;
}_color;

_color pal[256];

_color getHue(double h){
	_color col;
	h*=6.0;
	int hi=(int)h;
	double hf=h-hi;
	switch(hi){
		case 0:
			col.r=1.0;
			col.g=hf;
			col.b=0.0;
			break;
		case 1:
			col.r=1.0-hf;
			col.g=1.0;
			col.b=0.0;
			break;
		case 2:
			col.r=0.0;
			col.g=1.0;
			col.b=hf;
			break;
		case 3:
			col.r=0.0;
			col.g=1.0-hf;
			col.b=1.0;
			break;
		case 4:
			col.r=hf;
			col.g=0.0;
			col.b=1.0;
			break;
		case 5:
			col.r=1.0;
			col.g=0.0;
			col.b=1.0-hf;
			break;
	}
	
	//~ col.r=1-col.r;col.g=1-col.g;col.b=1-col.b;
	
	return col;
}

_color getGrad2(double p){
	_color col,c1,c2;
	//~ c2.r = 255	/255.0;
	//~ c2.g = 255	/255.0;
	//~ c2.b = 255	/255.0;
	
	//~ c1.r = 0	/255.0;
	//~ c1.g = 0	/255.0;
	//~ c1.b = 0	/255.0;
	
	c2.r = 255	/1.0;
	c2.g = 255	/1.0;
	c2.b = 255	/1.0;
	
	c1.r = 0	/1.0;
	c1.g = 0	/1.0;
	c1.b = 0	/1.0;
	
	//~ c1.r=1-c1.r;c1.g=1-c1.g;c1.b=1-c1.b;
	//~ c2.r=1-c2.r;c2.g=1-c2.g;c2.b=1-c2.b;
	
	col.r=c1.r+p*(c2.r-c1.r);
	col.g=c1.g+p*(c2.g-c1.g);
	col.b=c1.b+p*(c2.b-c1.b);
	
	return col;
}


int populatePal(_color *pallete){
	_color col;
	int size,i,r,g,b;
	FILE *pal=fopen("pal2.ppm","rt");
	fscanf(pal,"%d\n",&size);
	size=255;
	
	if(size>0 && pal!=NULL){
		for(i=0;i<size;i++){
			fscanf(pal,"%d\n%d\n%d\n",&r,&g,&b);
			col.r=(double)r;
			col.g=(double)g;
			col.b=(double)b;
			memcpy(&pallete[i],&col,sizeof(_color));
		}
	}
	fclose(pal);
}
_color getPalMem(double p,_color *pal){
	int i,size=255;
	_color col;
	//~ memcpy(&col,&pal[((int)p)%size],sizeof(_color));
	memcpy(&col,&pal[(int)(p*size)],sizeof(_color));
	return col;
}

_color getPal(double p){
	_color col;
	int size,i,r,g,b;
	FILE *pal=fopen("pal2.ppm","rt");
	fscanf(pal,"%d\n",&size);
	
	p=p*size;
	
	if(p>0){
		for(i=0;i<p;i++){
			fscanf(pal,"%d\n%d\n%d\n",&r,&g,&b);
		}
	}else{
		fscanf(pal,"%d\n%d\n%d\n",&r,&g,&b);
	}
	
	//~ printf("%d %d %d\n",r,g,b);
	
	//~ col.r=r/255.0;
	//~ col.g=g/255.0;
	//~ col.b=b/255.0;
	
	fclose(pal);
	return col;
}

_color invert_color(_color col){
	col.r=255-col.r;
	col.g=255-col.g;
	col.b=255-col.b;
	return col;
}
