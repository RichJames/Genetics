import sys
import math
import copy
from randev import RanDev
from floattools import *
from roulette import RouletteWheel
from decimal import *

# initialize variables with default values
POP_SZ			=  50
GEN_SZ			=  1001
ReptFreq		=   10
#SIG_DIG			= '.00000000'
SIG_DIG			=  8
XMin			= -10.0
XMax			=  10.0
YMin			= -10.0
YMax			=  10.0
FitLinBase		= 100.0
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
EQ_ID			= 3	# 0=F6, 1=F7, 2=F8, 3=Custom
FSA_EXPON		= 1
FSA_WINDOW		= 2
FSA_LINEAR		= 3
Normalization		= 1

# fitness algorithms
def FitnessF6(x, y):
	res = (0.7 + math.pow(x, 2) 
	           + 2.0 * math.pow(y, 2)
	           - 0.3 * math.cos(3.0 * math.pi * x)	
	           - 0.4 * math.cos(4.0 * math.pi * y))
	return res

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
	           	   - 0.3 * math.cos(3.0 * math.pi * x)	
	           	   - 0.4 * math.cos(4.0 * math.pi * y)))	

#helper function for sorting
def getKey(item):
	return item[0]

def TestPeakSearch():
	# create random deviate and mutations objects
	devgen = RanDev()
	fmute = FloatMutagen(WtSign, WtExp, WtMant)
	#fmute = FloatMutagen()

	# allocate fitness and population array (fitness, x, y)
	x	= [[0.0] * 3 for i in range(POP_SZ)]
	xnew	= [[0.0] * 3 for i in range(POP_SZ)]
		
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
		best	= sys.float_info.max * -1.0
		lowf	= sys.float_info.max
		ibest	= 0

		for i in range(POP_SZ):
			if EQ_ID == 0:
				x[i][0] = 1.0 - FitnessF6(x[i][1], x[i][2])
			elif EQ_ID == 1:
				x[i][0] = 1.0 - FitnessF7(x[i][1], x[i][2])
			elif EQ_ID == 2:
				x[i][0] = 1.0 - FitnessF8(x[i][1], x[i][2])
			else:
				x[i][0] = FitnessCust(x[i][1], x[i][2])

			x[i][0] = SigDig(x[i][0], SIG_DIG)

			# Track best fitness
			if x[i][0] > best:
				best	= x[i][0]
				ibest	= i

			# Track lowest fitness
			if x[i][0] < lowf:
				lowf = x[i][0]

			#print('g{4} ({0}): ({1}, {2}) fitness = {3}'.format(i, x[i][1], x[i][2], x[i][0], g))

		# display best solution so far
		if (g % ReptFreq) == 0:
			print('g{0}: Best ({1}, {2}) fit = {3}'.format(g, x[ibest][1], x[ibest][2], best))
			#print('')

		# fitness scaling
		if FitScale:
			for i in range(POP_SZ):
				if Normalization == FSA_EXPON:
					x[i][0] = math.pow((x[i][0] + 1.0), 2)
				elif Normalization == FSA_WINDOW:
					x[i][0] -= lowf
				elif Normalization == FSA_LINEAR:
					# sort by fitness if linear normalization (high to low i.e. best to worst fitness order)
					x = sorted(x, key=getKey, reverse=True)

					fitn = FitLinBase
					x[i][0] = fitn

					if fitn > FitLinMin:
						fitn -= FitLinDec

						if fitn < FitLinMin:
							fitn = FitLinMin


		# create roulette wheel for reproduction selection
		weights		= [fit[0] for fit in x]		# extract all fitness values into a list
		roulette	= RouletteWheel(POP_SZ, weights)

		# if elitist, include best from orig. population
		if Elitist:
			if Normalization == FSA_LINEAR:
				xnew[0] = copy.deepcopy(x[0])
			else:
				xnew[0] = copy.deepcopy(x[ibest])

			# reset xnew[0] fitness to 0.0
			xnew[0][0] = 0.0

			i = 1
		else:
			i = 0

		# create new population of x's
		while i < POP_SZ:
			# create a new x
			p1 = roulette.GetIndex()

			if CrossX and (devgen.GetUDev() <= CrossRate):
				p2 = roulette.GetIndex()
				xnew[i][1] = doubleCrossover(x[p1][1], x[p2][1])
				#if abs(xnew[i][1]) < 1.0e-300:
					#print('CrossX:  xnew[{0}][1] = {1}, from parents {2} and {3}'.format(i, xnew[i][1], x[p1][1], x[p2][1]))
			else:
				xnew[i][1] = x[p1][1]
				#xnew[i][1] = SigDig(rangex * devgen.GetUDev() + XMin, SIG_DIG)

			# create a new y
			if CrossB:
				p1 = roulette.GetIndex()

			if CrossY and (devgen.GetUDev() <= CrossRate):
				p2 = roulette.GetIndex()
				xnew[i][2] = doubleCrossover(x[p1][2], x[p2][2])
				#if abs(xnew[i][2]) < 1.0e-300:
					#print('CrossY:  xnew[{0}][2] = {1}, from parents {2} and {3}'.format(i, xnew[i][2], x[p1][2], x[p2][2]))
			else:
				xnew[i][2] = x[p1][2]
				#xnew[i][2] = SigDig(rangey * devgen.GetUDev() + YMin, SIG_DIG)

			# mutate x
			if MutateX:
				if MuteLoop:
					while devgen.GetUDev() < MuteRate:
						xnew[i][1] = fmute.doubleMutate(xnew[i][1])
				else:
					if devgen.GetUDev() < MuteRate:
						prevx = xnew[i][1]	
						xnew[i][1] = fmute.doubleMutate(xnew[i][1])
						#if abs(xnew[i][1]) < 1.0e-300:
							#print('MutateX:  xnew[{0}][1] = {1}, from {2}'.format(i, xnew[i][1], prevx))

			# mutate y
			if MutateY:
				if MuteLoop:
					while devgen.GetUDev() < MuteRate:
						xnew[i][2] = fmute.doubleMutate(xnew[i][2])
				else:
					if devgen.GetUDev() < MuteRate:
						prevy = xnew[i][1]	
						xnew[i][2] = fmute.doubleMutate(xnew[i][2])
						#if abs(xnew[i][2]) < 1.0e-300:
							#print('MutateY:  xnew[{0}][2] = {1}, from {2}'.format(i, xnew[i][2], prevy))
			
			# make sure x & y fit ranges
			if xnew[i][1] > XMax:
				xnew[i][1] = XMax
			elif xnew[i][1] < XMin:
				xnew[i][1] = XMin

			if xnew[i][2] > YMax:
				xnew[i][2] = YMax
			elif xnew[i][2] < YMin:
				xnew[i][2] = YMin

			# truncate digits
			prevxnew = xnew[i][1]
			xnew[i][1] = SigDig(xnew[i][1], SIG_DIG)
			#if abs(xnew[i][1] == 0.0):
				#print('SigDig:  xnew[{0}][1] = {1}, from {2}'.format(i, xnew[i][1], prevxnew))
			prevynew = xnew[i][2]
			xnew[i][2] = SigDig(xnew[i][2], SIG_DIG)
			#if abs(xnew[i][2] == 0.0):
				#print('SigDig:  xnew[{0}][2] = {1}, from {2}'.format(i, xnew[i][2], prevynew))

			# set xnew fitness to 0.0
			xnew[i][0] = 0.0

			# increment i
			i += 1

		# copy new population
		x = copy.deepcopy(xnew)

TestPeakSearch()
