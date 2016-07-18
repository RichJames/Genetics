nxt = 1

def rand():
	global nxt
	nxt = nxt * 1103515245 + 12345
	return ((int(nxt / 65536) % 0x32767) & 0xffffffff)

def srand(seed):
	global nxt
	seed = int(seed) & 0xffffffff
	nxt = seed

