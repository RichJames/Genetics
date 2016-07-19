import math

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
	           	   - 0.3 * math.cos(3.0 * math.pi * x)	
	           	   - 0.4 * math.cos(4.0 * math.pi * y)))


#x, y = 15.34, -29.3
#print('fitnessF6   = {0}'.format(FitnessF6(x, y)))
#print('fitnessF7   = {0}'.format(FitnessF7(x, y)))
#print('fitnessF8   = {0}'.format(FitnessF8(x, y)))
#print('fitnessCust = {0}'.format(FitnessCust(x, y)))
