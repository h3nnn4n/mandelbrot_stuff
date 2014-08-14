import numpy
import math

screenx=1600/1
screeny=1200/1

bailout=2048/1
bailout=256*2

inc=10.0
inc=bailout

real_min, real_max=-2.5,1.5
imag_min, imag_max=-1.5,1.5

real_center,imag_center,radius= -0.743643135,0.131825963,0.000014628
real_center,imag_center,radius= 0.274,0.482,0.01

real_min,real_max=real_center-radius,real_center+radius
imag_min,imag_max=imag_center-radius,imag_center+radius

real_range=numpy.arange(real_min,real_max,(real_max-real_min)/screenx)
imag_range=numpy.arange(imag_min,imag_max,(imag_max-imag_min)/screeny)

fout=open('mandelbrot.ppm','w')
fout.write('P3\n'+str(screenx)+' '+str(screeny)+'\n255\n')

for im in imag_range:
	for re in real_range:
		c=complex(re,im)
		z=complex(0.0,0.0)
		n=0
		while abs(z)<4 and n<bailout:
			z=z*z+c
			n+=1
		if n==bailout:
			fout.write('0 0 0 ')
		else:
			h=((n+1-math.log(math.log(abs(z)))/math.log(2)))
			while h>inc:
				h-=inc
			h/=inc
			h*=6.0
			hi=int(h)
			hf=h-hi
			if hi==4:
				r,g,b=1.0,hf,0.0
			elif hi==5:
				r,g,b=1.0-hf,1.0,0.0
			elif hi==0:
				r,g,b=0.0,1.0,hf
			elif hi==1:
				r,g,b=0.0,1.0-hf,1.0
			elif hi==2:
				r,g,b=hf,0.0,1.0
			elif hi==3:
				r,g,b=1.0,0.0,1.0-hf
			
			r*=255
			g*=255
			b*=255
			
			fout.write(str(int(r))+' '+str(int(g))+' '+str(int(b))+' ');
	fout.write('\n')
	print('Imaginary value starts in '+str(imag_min)+' and goes to '+str(imag_max)+' and is at '+str(im))

fout.close()
