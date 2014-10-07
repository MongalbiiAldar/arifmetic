#!/usr/bin/python
import bigNumber
import argparse
import sys


def getArguments ():
	parser = argparse.ArgumentParser(prog = "TCHMK_3")
	parser.add_argument ('A_filename', help = "filename of file with 1-st big integer")
	parser.add_argument ('operation', choices=['+', '-', '*', '/', '%', '^'], help = "operations")
	parser.add_argument ('B_filename', help = "filename of file with 2-nd big integer")
	parser.add_argument ('result_filename', help = "filename of result file")
	parser.add_argument ('-b', action='store_const', const=True,  default=False, help = "perform binary files")
	parser.add_argument ('mod_filename', help = "filename of file with modulus", nargs='?')
 	return parser.parse_args()
 
def getFromFiles (fileA, fileB, fileMod, binary):
	A = bigNumber.bigNumber()
	B = bigNumber.bigNumber()
	modulus = bigNumber.bigNumber()
	if (binary):
		if not A.getNumberFromBinFile(fileA):
			print "Can't get number from:", fileA 
			return False, A, B, modulus
		if not B.getNumberFromBinFile(fileB):
			print "Can't get number from:", fileB 
			return False, A, B, modulus
		if fileMod:
			if not modulus.getNumberFromBinFile(fileMod):
				print "Can't get number from:", fileMod 
				return False, A, B, modulus
	else:
		if not A.getNumberFromTextFile(fileA):
			print "Can't get number from:", fileA 
			return False, A, B, modulus
		if not B.getNumberFromTextFile(fileB):
			print "Can't get number from:", fileB 
			return False, A, B, modulus
		if fileMod:
			if not modulus.getNumberFromTextFile(fileMod):
				print "Can't get number from:", fileMod 
				return False, A, B, modulus
	return True, A, B, modulus
	
def process(A, B, modulus, operation):
	zero = bigNumber.bigNumber("0")
	res = zero
	
	if (modulus < zero):
		print "Negative modulus!"
		return False, res
		
	if operation == '^':
		res = bigNumber.Pow(A, B, modulus)
		return True, res
			
	if (modulus > zero):
		A = A % modulus
		B = B % modulus
	
	if operation == '+':
		res = A + B

	elif operation == '-':
		res = A - B

	elif operation == '*':
		res = A * B

	elif operation == '/':
		if B == zero:
			print "Division by zero"
			return False, res
		res = A / B

	elif operation == '%':
		if (B == zero):
			print "Division by zero"
			return False, res
		res = A % B
	
	if (modulus > zero):
			res = res % modulus
			while (res < zero):
				res = res + modulus
				
	return True, res
			
def saveRes(fileRes, binary, res):
	if binary:
		if not res.saveNumberToBinFile(fileRes):
			print "Can't save result to ", fileRes 
			return False
	else:
		if not res.saveNumberToTextFile(fileRes):
			print "Can't save result to ", fileRes 
			return False
	return True

	
def main():
	arguments = getArguments()
	
	isRead, A, B, modulus = getFromFiles (arguments.A_filename, arguments.B_filename, arguments.mod_filename, arguments.b)
	if not isRead:
		sys.exit(-1)
		
	isProcess, res = process(A, B, modulus, arguments.operation)
	if not isProcess:
		sys.exit(-1)

	isWrite = saveRes(arguments.result_filename, arguments.b, res)
	if not isWrite:
		sys.exit(-1)

if __name__ == "__main__":
	main()