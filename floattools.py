#import struct
from ctypes import *
from decimal import *
from randev import RanDev

#def binary(num):
#	return ''.join(bin(c).replace('0b', '').rjust(8, '0') for c in struct.pack('!f', num))

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

	#def doubleMutate(self, d):
		#mask for exponent bits
		#DExpt = 0b0111111111110000000000000000000000000000000000000000000000000000
	
		#choose section to mutate
		#mpick = self._devgen.GetUDev() * self._TotalW
		
		#copy float to long for manipulation
		#x = cast(pointer(c_double(d)), POINTER(c_int64)).contents.value & 0xffffffffffffffff

		#if all exponent bits on (invalid number), return original
		#if (x & DExpt) == DExpt:
		#	return d

		#mutate
		#if (mpick < self._SignW):
			#flip sign
		#	mask = 0x8000000000000000

		#	if x & mask:
		#		x &= ~mask
		#	else:
		#		x |= mask
		#else:
		#	mpick -= self._SignW

		#	if mpick < self._ExpW:
				# mutate exponent while number is valid
		#		while 1==1:
		#			n = x
		#			mask = (0x0010000000000000 << int(self._devgen.GetUDev() * 11.0)) & 0xffffffffffffffff

		#			if n & mask:
		#				n &= ~mask
		#			else:
		#				n |= mask

		#			if (n & DExpt) != DExpt:
		#				break	

		#		x = n
		#	else:
				# flip bit in mantissa
		#		mask = (1 << int(self._devgen.GetUDev() * 52.0)) & 0xffffffffffffffff

		#		if x & mask:
		#			x &= ~mask
		#		else:
		#			x |= mask

		#x &= 0xffffffffffffffff
		#res = cast(pointer(c_int64(x)), POINTER(c_double)).contents.value
		#return res

	def doubleMutate(self, d):
		#mask for exponent bits
		DExpt = 0x7ff00000
	
		#choose section to mutate
		mpick = self._devgen.GetUDev() * self._TotalW
		
		#copy float to long for manipulation
		temp = cast(pointer(c_double(d)), POINTER(c_int64)).contents.value & 0xffffffffffffffff
		mask1 = 0b1111111111111111111111111111111100000000000000000000000000000000
		mask2 = 0b0000000000000000000000000000000011111111111111111111111111111111

		x = [(temp & mask1) >> 32, temp & mask2]
	
		#mutate
		if (mpick < self._SignW):
			#flip sign
			mask = 0x80000000

			if x[1] & mask:
				x[1] &= ~mask
			else:
				x[1] |= mask
		else:
			mpick -= self._SignW

			if mpick < self._ExpW:
				# mutate exponent while number is valid
				while 1==1:
					n = x[1]
					mask = (0x00100000 << int(self._devgen.GetUDev() * 11.0)) & 0xffffffff

					if n & mask:
						n &= ~mask
					else:
						n |= mask

					if (n & DExpt) != DExpt:
						break	

				x[1] = n
			else:
				bit = int(self._devgen.GetUDev() * 52.0)

				if bit > 31:
					bit -= 32
					mask = (1 << bit) & 0xffffffff

					if x[1] & mask:
						x[1] &= ~mask
					else:
						x[1] |= mask
				else:
					# flip bit in mantissa
					mask = (1 << bit) & 0xffffffff

					if x[0] & mask:
						x[0] &= ~mask
					else:
						x[0] |= mask

		temp	= ((x[0] << 32) & 0xffffffff00000000) ^ x[1]
		res	= cast(pointer(c_int64(temp)), POINTER(c_double)).contents.value
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

#def doubleCrossover(d1, d2):
	#mask for exponent bits
#	DExpt = 0b0111111111110000000000000000000000000000000000000000000000000000

	# create random deviate generator
#	devgen = RanDev()

#	l1 = cast(pointer(c_double(d1)), POINTER(c_int64)).contents.value & 0xffffffffffffffff
#	l2 = cast(pointer(c_double(d2)), POINTER(c_int64)).contents.value & 0xffffffffffffffff

#	while 1==1:
		#create mask
#		mask = (0xffffffffffffffff << int(devgen.GetUDev() * 64.0)) & 0xffffffffffffffff

		#generate offspring
#		lcross = (l1 & mask) | (l2 & (~mask))

#		if (lcross & DExpt) != DExpt:
#			break	

#	lcross &= 0xffffffffffffffff
#	dcross = cast(pointer(c_int64(lcross)), POINTER(c_double)).contents.value
#	return dcross

def doubleCrossover(d1, d2):
	#mask for exponent bits
	DExpt 	= 0x7ff00000
	lcross	= [None, None]

	# create random deviate generator
	devgen = RanDev()

	#copy float to long for manipulation
	temp1 = cast(pointer(c_double(d1)), POINTER(c_int64)).contents.value & 0xffffffffffffffff
	temp2 = cast(pointer(c_double(d2)), POINTER(c_int64)).contents.value & 0xffffffffffffffff
		
	mask1 = 0b1111111111111111111111111111111100000000000000000000000000000000
	mask2 = 0b0000000000000000000000000000000011111111111111111111111111111111

	l1 = [(temp1 & mask1) >> 32, temp1 & mask2]
	l2 = [(temp2 & mask1) >> 32, temp2 & mask2]

	while 1==1:
		# calculate bit position for flip
		bit = int(devgen.GetUDev() * 64.0)

		if bit > 31:			# if flip in high-order word
			# create mask
			mask = (0xffffffff << (bit - 32)) & 0xffffffff

			# duplicate low-order word of first parent
			lcross[0] = l1[0]

			# crossover in high-order word
			lcross[1] = (l1[1] & mask) | (l2[1] & (~mask))
		else:
			# create mask
			mask = (0xffffffff << bit) & 0xffffffff

			# crossover in low-order word
			lcross[0] = (l1[0] & mask) | (l2[0] & (~mask))

			# duplicate high-order word of first parent
			lcross[1] = l1[1]

		if (lcross[1] & DExpt) != DExpt:
			break	

	temp	= ((lcross[0] << 32) & 0xffffffff00000000) ^ lcross[1]
	res	= cast(pointer(c_int64(temp)), POINTER(c_double)).contents.value
	return res

def SigDig(x, n):
	# round a number, x, to a specific number of digits, using n as a template
	return float(Decimal(x).quantize(Decimal(n)))
