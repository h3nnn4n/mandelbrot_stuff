#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "color.h"

int main(){
    int 	screenx,screeny,
                ix,iy,
                i,j,
                er,bailout;
    double	zx,zy,zxn,zyn,cx,cy,
                minx,maxx,miny,maxy,
                xcenter,ycenter,size;

    FILE *img=fopen("mandel.ppm","wt");
    _color col;
    int *escapetime;

    // Image Resolution
    screenx=1600/2;
    screeny=1200/2;

    // Max iterations
    bailout=5000;

    // Mandelbrot escape radius
    er=2;

    // Zoom and image center
    size=0.005;
    xcenter=-0.13856524454488;
    ycenter=-0.64935990748190;

    minx = xcenter - size;
    maxx = xcenter + size;
    miny = ycenter - size;
    maxy = ycenter + size;

    printf("%f \t %f\n%f\t %f\n",minx,maxx,miny,maxy);

    // This overides the above zoom and shows the whole set
    minx=-2.5;
    maxx=1.5;
    miny=-2.0;
    maxy=miny*-1;

    // The bitmap
    escapetime = (int*)malloc(sizeof(int)*screenx*screeny);
    fprintf(img,"P3\n%d %d\n255\n",screenx,screeny);

    for(iy=0;iy<screeny;iy++){
        cy=miny+iy*(maxy-miny)/screeny; // This gets the imaginary part of the complex number associated with a pixel
        for(ix=0;ix<screenx;ix++){
            cx=minx+ix*(maxx-minx)/screenx; // Same as above but with the real part
            zx=0;
            zy=0;

            for(i=0;i<bailout;i++){     // The main loop
                zxn=zx*zx-zy*zy+cx;     // Complex number black magic
                zyn=2.0*zx*zy+cy;       //
                zx=zxn;
                zy=zyn;

                if(zx*zx+zy*zy>er*er){  // Checks if the thing goes outside the escape radius
                    escapetime[iy*screenx+ix]=i;
                    //~ printf("%d\n",i);
                    break;
                }
            }

            if(i==bailout-1){           // If it doesnt escape the mark is as black(zero)
                escapetime[iy*screenx+ix]=0.0;
            }
        }

        if(iy%(screeny/10)==0){   // Shows the progress
            fprintf(stderr," -- %.2f%%\n",(iy/(double)screeny)*100.0);
        }
    }

    fprintf(stderr," -- %.2f%%\n",100.0);   // Saves everything to disk
    fprintf(stderr," <---- DONE ---->\n");
    fprintf(stderr," Writing to disk!\n");

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
