import random

MAX32BITINT = 2147483647

n = random.randint(0,MAX32BITINT) & int(0x8fffffff)

def Blackbox(x):
	# test value -- the speed of light in meters per second
	#n = int(0x11DE784A)

	fit = 0
	mask = 1

	# count matching bits
	for i in range(32):
		if ((x & mask) == (n & mask)):
			fit += 1

		mask = (mask << 1) & 0xffffffff	# ensure mask is no more than 32 bits

	return fit

POPSIZE = 50
GENSIZE = 500
cross = True
crate = 1.0
mutate = True
mrate = 0.7
scale = True
elite = True

pop = [0] * POPSIZE
newpop = [0] * POPSIZE
fit = [0] * POPSIZE
avgf = 0

# create initial population
for i in range(POPSIZE):
	pop[i] = random.randint(0,MAX32BITINT)

# start with generation zero
g = 0

while True:
	# initialize for fitness testing
	bestf = -1
	bestl = 0
	totf = 0
	minf = MAX32BITINT + 1 # LONG_MAX

	#fitness testing
	for i in range(POPSIZE):
		# call fitness function and store result
		fit[i] = Blackbox(pop[i])

		# keep track of best fitness
		if fit[i] > bestf:
			bestf = fit[i]
			bestl = pop[i]

		# keep track of least fit
		if fit[i] < minf:
			minf = fit[i]

		# total fitness
		totf += fit[i]

	# make sure we have at least some fit values
	if totf == 0:
		print("Population has total fitness of ZERO")
		break

	# compute average fitness
	avgf = totf / POPSIZE
	
	# sum (and maybe scale) fitness values
	if scale:
		# ensure that the least fitness is one
		minf += 1

		# recalculate total fitness to reflect scaled values
		totf = 0

		for i in range(POPSIZE):
			fit[i] -= minf	# reduce by smallest fitness
			fit[i] *= fit[i]  # square result of above
			totf += fit[i]	# add into total fitness

	# display stats fo this generation
	print("{0} best: {1:8x} ({2:2d}) avg. fit = {3:2f} min. fit = {4:2d}".format(g, bestl, bestf, avgf, minf))

	# exit if this is final generation
	if g == GENSIZE:
		break

	# create new population
	for i in range(POPSIZE):
		# roulette-select parent
		sel = random.random() * float(totf)

		p1 = 0

		while sel > fit[p1]:
			sel -= fit[p1]
			p1 += 1

		# crossover reproduction
		if cross and (random.random() < crate):
			# roulette-select second parent
			sel = random.random() * float(totf)

			p2 = 0

			while sel > fit[p2]:
				sel -= fit[p2]
				p2 += 1

			# mask of bits to be copied from first parent
			mask = (int(0xffffffff) << int(random.random() * 32.0)) & 0xffffffff

			# new string from two parents
			newpop[i] = (pop[p1] & mask) | (pop[p2] & (~mask))
		else:
			# one parent, no crossover reproduction
			newpop[i] = pop[p1]

		# mutation
		if mutate and (random.random() < mrate):
			# select bit to be changed
			mask = (1 << int(random.random() * 32.0)) & 0xffffffff

			# flip the bit
			if newpop[i] & mask:
				newpop[i] &= ~mask
			else:
				newpop[i] |= mask
	
	if elite:
		newpop[0] = bestl

	# replace old population with new one
	pop = newpop	

	# stop searching if we have found the optimum answer
	if bestl == n:
		break

	# increment generation
	g += 1


print("n is {0:8x}".format(n))
 
