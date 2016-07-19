import sys
import math
from randev import RanDev
from floattools import *

# initialize variables with default values
POP_SZ			=  50
GEN_SZ			= 101
ReptFreq		=   1
SIG_DIG			= '.00000000'
XMin			= -10.0
XMax			=  10.0
YMin			= -10.0
YMax			=  10.0
FitLineBase		= 100.0
FitLinDec		=  10.0
FitLinMin		=  10.0
WtSign			=   2.0
WtExp			=  13.0
WtMant			=  85.0
MuteRate		=   1.0
CrossRate		=   1.0
CrossX			= True
CrossY			= True
CrossB			= True
MutateX			= True
MutateY			= True
MuteLoop		= False
Elitist			= True
FitScale		= True
EQ_ID			= 3
FSA_EXPON		= 1
FSA_WINDOW		= 2
FSA_LINEAR		= 3
Normalization		= 3

# fitness algorithms
def FitnessF6(x, y):
	return (0.7 + math.pow(x, 2) 
	           + 2.0 * math.pow(y, 2)
	           - 0.3 * math.cos(3.0 * math.pi * x)	
	           - 0.4 * math.cos(4.0 * math.pi * y))

def FitnessF7(x, y):
	return (0.3 + math.pow(x, 2) 
	           + 2.0 * math.pow(y, 2)
	           - 0.3 * (math.cos(3.0 * math.pi * x)	
	           	    *  math.cos(4.0 * math.pi * y)))

def FitnessF8(x, y):
	return (0.3 + math.pow(x, 2) 
	           + 2.0 * math.pow(y, 2)
	           - 0.3 * (math.cos(3.0 * math.pi * x)	
	           	    +  math.cos(4.0 * math.pi * y)))

def FitnessCust(x, y):
	return (1.0 / (0.8 + math.pow((x + 0.5), 2) 
	           	   + 2.0 * math.pow((y - 0.5), 2)
	           	   - 0.3 * math.cos(3.0 * math.pi * x)))	

#helper function for sorting
def getKey(item):
	return item[0]

def TestPeakSearch():
	# create random deviate and mutations objects
	devgen = RanDev()
	fmute = FloatMutagen(WtSign, WtExp, WtMant)

	# allocate population and fitness arrays
	#x 	= [0.0] * POP_SZ
	xnew	= [0.0] * POP_SZ
	#y 	= [0.0] * POP_SZ
	ynew	= [0.0] * POP_SZ
	#fit	= [0.0] * POP_SZ

	# allocate fitness and population array (fitness, x, y)
	x	=[[0.0, 0.0, 0.0]] * POP_SZ
		
	# calculate ranges
	rangex = XMax - XMin
	rangey = YMax - YMin

	# generate initial X values
	for i in range(POP_SZ):
		x[i][1] = SigDig(rangex * devgen.GetUDev() + XMin, SIG_DIG)
		x[i][2] = SigDig(rangey * devgen.GetUDev() + YMin, SIG_DIG)

	# do the generations
	for g in range(GEN_SZ):
		# calculate fitness for x values
		best	= sys.float_info.max
		lowf	= sys.float_info.min
		ibest	= 0

		for i in range(POP_SZ):
			if EQ_ID == 0:
				x[i][0] = 1.0 - FitnessF6(x[i][1], y[i][2])
			elif EQ_ID == 1:
				x[i][0] = 1.0 - FitnessF7(x[i][1], y[i][2])
			elif EQ_ID == 2:
				x[i][0] = 1.0 - FitnessF8(x[i][1], y[i][2])
			else:
				x[i][0] = 1.0 - FitnessCust(x[i][1], y[i][2])

			x[i][0] = SigDig(x[i][0], SIG_DIG)

			# Track best fitness
			if fit[i][0] > best:
				best	= fit[i][0]
				ibest	= i

			# Track lowest fitness
			if fit[i][0] < lowf:
				lowf = fit[i][0]

		# display best solution so far
		if (g % ReptFreq) == 0:
			print('{0}: ({1}, {2}) fit = {3}'.format(g, x[ibest][1], y[ibest][2], best))

		# sort by fitness if linear normalization

		# fitness scaling
		if FitScale:
			for i in range(POP_SZ):
				if normalization == FSA_EXPON:
					x[i][0] = math.pow((x[i][0] + 1.0), 2)
				elif normalizaition == FSA_WINDOW:
					x[i][0] -= lowf
				elif normalization == FSA_LINEAR:
					# sort by fitness if linear normalization (does this need to be sorted in reverse order?)
					x = sorted(x, key=getKey)

					fitn = FitLinBase
					x[i][0] = fitn

					if fitn > FitLinMin:
						fitn -= FitLinDec

						if fitn < FitLinMin:
							fitn = FitLinMin


