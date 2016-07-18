#import struct
from ctypes import *
from randev import RanDev

devgen = RanDev()

#def binary(num):
#	return ''.join(bin(c).replace('0b', '').rjust(8, '0') for c in struct.pack('!f', num))

class FloatMutagen:
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
		FExpt = 0x7F800000
	
		#choose section to mutate
		mpick = float(devgen.GetUDev() * self._TotalW)
		
		#copy float to long for manipulation
		#x = binary(mpick)
		x = cast(pointer(c_float(mpick)), POINTER(c_int32)).contents.value & 0xffffffff

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
					mask = (0x00800000 << int(devgen.GetUDev() * 8.0)) & 0xffffffff

					if n & mask:
						n &= ~mask
					else:
						n |= mask

					if (n & FExpt) != FExpt:
						break	

				x = n
			else:
				# flip bit in mantissa
				mask = (1 << int(devgen.GetUDev() * 23.0)) & 0xffffffff

				if x & mask:
					x &= ~mask
				else:
					x |= mask

		res = cast(pointer(c_int32(x)), POINTER(c_float)).contents.value
		return res

def floatCrossover(f1, f2):
	#mask for exponent bits
	FExpt = 0x7F800000

	l1 = cast(pointer(c_float(f1)), POINTER(c_int32)).contents.value & 0xffffffff
	l2 = cast(pointer(c_float(f2)), POINTER(c_int32)).contents.value & 0xffffffff

	while 1==1:
		#create mask
		mask = (0xFFFFFFFF << int(devgen.GetUDev() * 32.0)) & 0xffffffff

		#generate offspring
		lcross = (l1 & mask) | (l2 & (~mask))

		if (lcross & FExpt) != FExpt:
			break	

	fcross = cast(pointer(c_int32(lcross)), POINTER(c_float)).contents.value
	return fcross

fmutagen = FloatMutagen()
fmutagen.State()

for i in range(10):
	f1 = devgen.GetUDev()
	mutate1 = fmutagen.floatMutate(f1)

	print("({0}) f1 = {1}, mutate = {2}".format(i, f1, mutate1))

	f2 = devgen.GetUDev()
	mutate2 = fmutagen.floatMutate(f1)

	print("({0}) f2 = {1}, mutate = {2}".format(i, f2, mutate2))

	offspring = floatCrossover(mutate1, mutate2)

	print("({0}) offspring = {1}".format(i, offspring))
	print("")

