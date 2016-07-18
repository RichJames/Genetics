import time
import sys

IM1 = 2147483563
IM2 = 2147483399
IMM1 = IM1 - 1
IA1 = 40014
IA2 = 40692
IQ1 = 53668
IQ2 = 52774
IR1 = 12211
IR2 = 3791
NTAB = 32
NDIV = int(1 + IMM1 / NTAB)
RNMX = 1.0 - sys.float_info.epsilon
AM = 1.0 / float(IM1)

class RanDev:
	def __init__(self, initSeed = None):
		if initSeed is None:
			initSeed = time.time()

		if initSeed < 0:
			self.__seed = int(initSeed)
		else:
			self.__seed = int(-initSeed)

		self.__idum2 = 123456789
		self.__iy = 0
		self.__iv = [0] * NTAB

	def SetSeed(self, newSeed = None):
		if newSeed is None:
			newSeed = time.time()

		if newSeed < 0:
			self.__seed = int(newSeed)
		else:
			self.__seed = int(-newSeed)

	def GetUDev(self):
		j, k = 0, 0
		temp = 0.0

		if self.__seed <= 0:			# initialize
			if -self.__seed < 1:		# Be sure to prevent seed = 0
				self.__seed = 1
			else:
				self.__seed = -self.__seed

			self.__idum2 = self.__seed

			j = NTAB + 7
			while j >= 0:			# Load the shuffle table (after 8 warm-ups).
				k = int(self.__seed / IQ1)
				self.__seed = int(IA1 * (self.__seed - k * IQ1) - k * IR1)
				
				if self.__seed < 0:
					self.__seed += IM1

				if j < NTAB:
					self.__iv[j] = self.__seed

				j -= 1
	
			self.__iy = self.__iv[0]

		k = int(self.__seed / IQ1)		# Start here when not initializing.

		self.__seed = int(IA1 * (self.__seed - k * IQ1) - k * IR1)	# Compute idum=(IA1*idum) % IM1 without
										# overflows by Schrage's method.
		if self.__seed < 0:
			self.__seed += IM1

		k = int(self.__idum2 / IQ2)

		self.__idum2 = int(IA2 * (self.__idum2 - k * IQ2) - k * IR2)	# Compute idum2=(IA2*idum) % IM2 likewise.

		if self.__idum2 < 0:
			self.__idum2 += IM2

		j = int(self.__iy / NDIV)			# Will be in the range 0..NTAB-1.
		self.__iy = self.__iv[j] - self.__idum2		# Here idum is shuffled, idum and idum2 are
		self.__iv[j] = self.__seed			# combined to generate output.

		if self.__iy < 1:
			self.__iy += IMM1

		temp = AM * float(self.__iy)

		if temp > RNMX:
			return RNMX			# Because users don't expect endpoint values.
		else:
			return temp




#ranDev = RanDev()

#for i in range(20):
#	print("({0}) r = {1}".format(i, ranDev.GetUDev()))
