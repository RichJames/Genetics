import struct

def binary(num):
	#Struct can provide us with the float packed into bytes.  The '!' ensures that
	#it's in network byte order (big-endian) and the 'f' says that it should be
	#packed as a float.  Alternatively, for double-precision, you could use 'd'.
	packed = struct.pack('!f', num)
	print('Packed: {0}'.format(packed))

	#For each character in the returned string, we'll turn it into its corresponding
	#integer code point
	#
	#[62, 163, 215, 10] = [ord(c) for c in '>\xa3\xd7\n']
	integers = [c for c in packed]
	print('Integers: {0}'.format(integers))

	#For each integer, we'll convert it to its binary representation.
	binaries = [bin(i) for i in integers]
	print('Binaries: {0}'.format(binaries))
	
	#Now strip off the '0b' from each of these
	stripped_binaries = [s.replace('0b', '') for s in binaries]
	print('Stripped: {0}'.format(stripped_binaries))

	#Pad each byte's binary representation's with 0's to make sure it has all 8 bits
	#
	#['00111110', '10100011', 11010111', '00001010']
	padded = [s.rjust(8, '0') for s in stripped_binaries]
	print('Padded: {0}'.format(padded))

	#At this point, we have each of the bytes for the network byte ordered float
	#in an array as binary strings.  Now we just concatenate them to get the total
	#representation of the float:
	return ''.join(padded)

def binary2(num):
	return ''.join(bin(c).replace('0b', '').rjust(8, '0') for c in struct.pack('!f', num))

x = binary(1)
print("binary: {0}".format(x))
x = binary2(1)
print("binary2: {0}".format(x))

x = binary(0.32)
print("binary: {0}".format(x))
x = binary2(0.32)
print("binary2: {0}".format(x))

