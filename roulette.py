from randev import RanDev

class RouletteWheel:
	def __init__(self, sz = None, weights = None):
		if sz is None:
			self._N = 1
		else:
			self._N = sz		# Number of weights

		self._T = 0.0		# Total weight of all indexes

		if weights is None:
			self._W = [1] * self._N	# Array of weights
		else:
			self._W = list(map(abs, weights))

		self._T = float(sum(self._W))
		self._G = RanDev()

	def State(self):
		print("Number of weights = {0}".format(self._N))
		print("Total weights = {0}".format(self._T))
		print("Weights = {0}".format(self._W))

	def Change(self, i, weight):
		if i >= self._N:
			return -1

		self._T -= self._W[i]
		self._T += weight

		res = self._W[i]
		self._W[i] = weight
		return res

	def GetWeight(self, i):
		res = 0
		if i < self._N:
			res = self._W[i]
		else:
			res = -1	

		return res

	def GetIndex(self):
		s = self._G.GetUDev() * self._T
		i = 0

		while (i < self._N) and (s > self._W[i]):
			s -= self._W[i]
			i += 1

		return i
		
#weights = [.1,.3,-.5,.6,.2]
#count = len(weights)

#rw = RouletteWheel(count, weights)
#rw.State()
#index = rw.GetIndex()
#print("Index = {0}".format(rw.GetIndex()))
