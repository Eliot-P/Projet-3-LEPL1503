import argparse
import logging
import os
import sys

logger = logging.getLogger()


def is_div(number, i):
	return number % i == 0

def is_prime(number):
	for i in range(2, number):
		if is_div(number, i):
			return False
	return True

def prime_divs(number):
	prime_dividers = []
	for i in range(2, number):
		if is_prime(i) and is_div(number, i):
			prime_dividers.append(i)
	return prime_dividers


def main(input, output):
	logger.info("Debut de la boucle principale")
	nline = 0
	for line in input:
		print("Working on line " + str(nline))
		try:
			number = int(line)
		except ValueError:
			logger.warning("Entier mal formatte : %s", line[:-1])
			continue
		if number < 2:
			logger.warning("Entier plus petit que 2 : %s", line[:-1])
			continue

		prime_dividers = prime_divs(number)
		res = str(number) + " " + " ".join(str(div) for div in prime_dividers)

		output.write(res + "\n")
		logger.debug(res)
		nline +=1

In = open("Input.txt","r")
Out = open("PythonOUT.txt","w+")
main(In,Out)
In.close()
Out.close()
