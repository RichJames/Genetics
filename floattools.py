from ctypes import *
from decimal import *
from randev import RanDev
import math
import sys

class FloatMutagen:
	_devgen = RanDev()

	def __init__(self, sweight = 5.0, eweight = 5.0, mweight = 90.0):
		self._TotalW = sweight + eweight + mweight
		self._SignW = sweight
		self._ExpW = eweight

	def State(self):
		print("Total weight = {0}".format(self._TotalW))
		print("Sign weight = {0}".format(self._SignW))
		print("Exp weight = {0}".format(self._ExpW))

	def floatMutate(self, f):
		#mask for exponent bits
		#FExpt = 0x7F800000
		FExpt = 0b01111111100000000000000000000000
	
		#choose section to mutate
		mpick = self._devgen.GetUDev() * self._TotalW
		
		#copy double to long for manipulation
		x = cast(pointer(c_float(d)), POINTER(c_int32)).contents.value & 0xffffffff

		#if all exponent bits on (invalid number), return original
		if (x & FExpt) == FExpt:
			return f

		#mutate
		if (mpick < self._SignW):
			#flip sign
			mask = 0x80000000

			if x & mask:
				x &= ~mask
			else:
				x |= mask
		else:
			mpick -= self._SignW

			if mpick < self._ExpW:
				# mutate exponent while number is valid
				while 1==1:
					n = x
					mask = (0x00800000 << int(self._devgen.GetUDev() * 8.0)) & 0xffffffff

					if n & mask:
						n &= ~mask
					else:
						n |= mask

					if (n & FExpt) != FExpt:
						break	

				x = n
			else:
				# flip bit in mantissa
				mask = (1 << int(self._devgen.GetUDev() * 23.0)) & 0xffffffff

				if x & mask:
					x &= ~mask
				else:
					x |= mask

		res = cast(pointer(c_int32(x)), POINTER(c_float)).contents.value
		return res

	def doubleMutate(self, d):
		#mask for exponent bits
		DExpt = 0x7ff00000
		dcopy = d

		bNegd = False
		if d < 0.0:
			bNegd = True

		#choose section to mutate
		#getUDevcopy = self._devgen.GetUDev()
		#getUDevcopy = 0.05648113591638233     # test
		#mpick = getUDevcopy * self._TotalW
		mpick = self._devgen.GetUDev() * self._TotalW
		#print('mpick = {0}'.format(mpick))
		#mpickcopy = mpick
			
		#copy float to long for manipulation
		temp = bin(cast(pointer(c_double(d)), POINTER(c_int64)).contents.value & 0xffffffffffffffff)
		#mask1 = 0b1111111111111111111111111111111100000000000000000000000000000000
		#mask2 = 0b0000000000000000000000000000000011111111111111111111111111111111

		# ensure we have 64 bits; pad on the left with 0's.  Note:  66 includes '0b' part of the string.
		intd = '0'*(66 - len(temp)) + temp[2:]

		#x = [(abs(temp) & mask1) >> 32, abs(temp) & mask2]
		x = [intd[:32], intd[32:]]

		#x0copy = x[0]
		#x1copy = x[1]
		#x0flip = None
		#mpickadj = None
		#loopcount = 0
		#ncopy = None
		#maskexp = None
		#x0Expcopy = None
		#bitcopy = None
		#x031copy = None
		#x1mantcopy = None
	
		#mutate
		if (mpick < self._SignW):
			#flip sign
			#mask = 0x80000000

			#if x[0] & mask:
			#	x[0] &= ~mask
			#else:
			#	x[0] |= mask
			#x0flip = x[0]
			bNegd ^= True
		else:
			mpick -= self._SignW
			#mpickadj = mpick

			if mpick < self._ExpW:
				# mutate exponent while number is valid
				while 1==1:
					n = int(x[0], 2)
					mask = (0x00100000 << int(self._devgen.GetUDev() * 11.0)) & 0xffffffff
					#mask = 1073741824	# test
					#maskexp = mask

					if n & mask:
						n &= ~mask
					else:
						n |= mask

					#loopcount += 1
					if (n & DExpt) != DExpt:
						break	

				x[0] = '0' * (32 - len(bin(n))) + bin(n)[2:]
				#x0Expcopy = x[0]
			else:
				bit = int(self._devgen.GetUDev() * 52.0)
				#print('bit = {0}'.format(bit))
				#bitcopy = bit

				if bit > 31:
					bit -= 32
					n = int(x[0], 2)
					mask = (1 << bit) & 0xffffffff
					#print('mask = {0}, x[0] = {1}'.format(mask, x[0]))

					if n & mask:
						n &= ~mask
					else:
						n |= mask
					x[0] = '0' * (32 - len(bin(n))) + bin(n)[2:]
					#x031copy = x[0]
				else:
					# flip bit in mantissa
					n = int(x[1], 2)
					mask = (1 << bit) & 0xffffffff
					#print('mask = {0}, x[1] = {1}'.format(mask, x[1]))

					if n & mask:
						n &= ~mask
					else:
						n |= mask
					x[1] = '0' * (32 - len(bin(n))) + bin(n)[2:]
					#x1mantcopy = x[1]

		#x[0] &= 0xffffffff
		#x[1] &= 0xffffffff
		#temp	= ((x[0] << 32) & 0xffffffff00000000) ^ x[1]
		temp1 = int(x[0] + x[1], 2)
		if bNegd:
			temp1 *= -1

		res	= cast(pointer(c_int64(temp1)), POINTER(c_double)).contents.value
		if math.isnan(res):
			res = d

		#if abs(res) < 1.0e-300:
			#print('doubleMutate:  res is too small - {0}, d was {1}'.format(res, dcopy))

			#dcopy = d

			#print('dcopy = {0}'.format(dcopy))
			#print('bNegd = {0}'.format(bNegd))
			#print('getUDevcopy = {0}'.format(getUDevcopy))
			#print('mpickcopy = {0}'.format(mpickcopy))
			#print('temp = {0}'.format(temp))
			#print('x0copy = {0}'.format(x0copy))
			#print('x1copy = {0}'.format(x1copy))
			#print('x0flip = {0}'.format(x0flip))
			#print('mpickadj = {0}'.format(mpickadj))
			#print('loopcount = {0}'.format(loopcount))
			#print('ncopy = {0}'.format(ncopy))
			#print('maskexp = {0}'.format(maskexp))
			#print('x0Expcopy = {0}'.format(x0Expcopy))
			#print('bitcopy = {0}'.format(bitcopy))
			#print('x031copy = {0}'.format(x031copy))
			#print('x1mantcopy = {0}'.format(x1mantcopy))
			#print('temp1 = {0}'.format(temp1))

		return res

def floatCrossover(f1, f2):
	#mask for exponent bits
	#FExpt = 0x7F800000
	FExpt = 0b01111111100000000000000000000000

	# create random deviate generator
	devgen = RanDev()

	l1 = cast(pointer(c_float(f1)), POINTER(c_int32)).contents.value & 0xffffffff
	l2 = cast(pointer(c_float(f2)), POINTER(c_int32)).contents.value & 0xffffffff

	while 1==1:
		#create mask
		mask = (0xffffffff << int(devgen.GetUDev() * 32.0)) & 0xffffffff

		#generate offspring
		lcross = (l1 & mask) | (l2 & (~mask))

		if (lcross & FExpt) != FExpt:
			break	

	fcross = cast(pointer(c_int32(lcross)), POINTER(c_float)).contents.value
	return fcross

def doubleCrossover(d1, d2):
	#mask for exponent bits
	DExpt 	= 0x7ff00000
	lcross	= [None, None]

	bNegd1 = False
	if d1 < 0.0:
		bNegd1 = True

	# create random deviate generator
	devgen = RanDev()

	#copy float to long for manipulation
	temp1 = cast(pointer(c_double(d1)), POINTER(c_int64)).contents.value & 0xffffffffffffffff
	temp2 = cast(pointer(c_double(d2)), POINTER(c_int64)).contents.value & 0xffffffffffffffff
		
	mask1 = 0b1111111111111111111111111111111100000000000000000000000000000000
	mask2 = 0b0000000000000000000000000000000011111111111111111111111111111111

	l1 = [(abs(temp1) & mask1) >> 32, abs(temp1) & mask2]
	l2 = [(abs(temp2) & mask1) >> 32, abs(temp2) & mask2]

	#print('l1[0] = {0}, l1[1] = {1}'.format(l1[0], l1[1]))
	#print('l2[0] = {0}, l2[1] = {1}'.format(l2[0], l2[1]))

	while 1==1:
		# calculate bit position for flip
		bit = int(devgen.GetUDev() * 64.0)
		#print('bit = {0}'.format(bit))

		if bit > 31:			# if flip in high-order word
			# create mask
			mask = (0xffffffff << (bit - 32)) & 0xffffffff
			#print('bit > 31: mask = {0}'.format(mask))

			# duplicate low-order word of first parent
			lcross[1] = l1[1]

			# crossover in high-order word
			lcross[0] = (l1[0] & mask) | (l2[0] & (~mask))
		else:
			# create mask
			mask = (0xffffffff << bit) & 0xffffffff
			#print('bit <= 31: mask = {0}'.format(mask))

			# crossover in low-order word
			lcross[1] = (l1[1] & mask) | (l2[1] & (~mask))

			# duplicate high-order word of first parent
			lcross[0] = l1[0]

		if (lcross[1] & DExpt) != DExpt:
			break	

	#print('lcross[0] = {0}, lcross[1] = {1}'.format(lcross[0], lcross[1]))
	temp	= ((lcross[0] << 32) & 0xffffffff00000000) ^ lcross[1]
	if bNegd1:
		temp *= 1

	res	= cast(pointer(c_int64(temp)), POINTER(c_double)).contents.value
	if math.isnan(res):
		res = d1

	#if abs(res) < 1.0e-300:
		#print('doubleCrossover:  res is too small - {0}'.format(res))

	return res

def ToNearest(x):
	f, i = math.modf(x)
	f = abs(f)
	
	if f == 0.0:
		return i

	if f == 0.5:
		f1, dummy = math.modf(i/2.0)
		if f1 != 0.0:
			if x < 0.0:
				i -= 1.0
			else:
				i += 1.0
	else:
		if f > 0.5:
			if x < 0.0:
				i -= 1.0
			else:
				i += 1.0

	return i

def SigDig(x, n):
	if abs(x) < 1.0e-300:
		#print('x < very small number')
		result = 0.0
	else:
		if (n == 0) or (n > sys.float_info.dig):
			#print('n = 0 or n > DBL_DIG')
			result = x
		else:
			s = math.pow(10, n - 1 - math.floor(math.log10(abs(x))))
			#print('s = {0}'.format(s))
			result = ToNearest(x * s) / s
			#print('result = {0}'.format(result))

	return result


#def SigDig(x, n):
#	# round a number, x, to a specific number of digits, using n as a template
#	return float(Decimal(x).quantize(Decimal(n)))

d = 5.748206396142578
#fmute = FloatMutagen(2.0, 13.0, 85.0)
#for i in range(100):
#dMutant = fmute.doubleMutate(d)
#	#print('dMutant = {0}'.format(dMutant))
