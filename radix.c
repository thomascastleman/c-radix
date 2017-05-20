#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define DIGITS 10	// the number of digits in base 10

// get number of digits in a given unsigned long integer
int getNumOfDigits(unsigned long n) {
	return floor(log(n)) + 1.0;
}

// get the highest amount of digits in a given array
int getMostDigits(unsigned long *a, unsigned long size) {
	int max = getNumOfDigits(*a);	// initialize max as digits of first index in a

	// for every element in a
	for (unsigned long i = 0; i < size / sizeof(unsigned long); i++) {
		if (getNumOfDigits(*(a + i)) > max) {	// if greater than max
			max = getNumOfDigits(*(a + i));		// update max
		}
	}

	return max;
}

// perform Radix sort
void radix(unsigned long *a, unsigned long size) {
	// length of array
	unsigned long len = size / sizeof(unsigned long);

	unsigned long m = DIGITS, n = 1;

	unsigned long *working[DIGITS];		// array of pointers for digits 0-9
	unsigned long sizes[DIGITS];		// the amount of memory to be allocated to each index in working
	unsigned long available[DIGITS];	// used to keep track of next available index to store a value in working[]

	int mostDigits = getMostDigits(a, size);	// get the most amount of sig figs in array

	// while not every significant digit has been covered, continue to sort
	while (getNumOfDigits(n) <= mostDigits) {

		// initialize sizes and available to 0s
		for (int i = 0; i < DIGITS; i++) {
			sizes[i] = 0;
			available[i] = 0;
		}

		// calculate memory needed for each index of working
		for (unsigned long i = 0; i < len; i++) {
			int ind = (int) (*(a + i) % m) / n;	// get index
			sizes[ind]++;						// increment amount of memory needed in that index
		}

		// allocate that memory in working[]
		for (int i = 0; i < DIGITS; i++) {
			working[i] = malloc(sizes[i] * sizeof(unsigned long));
		}

		// add values to working[]
		for (unsigned long i = 0; i < len; i++) {
			int ind = (int) (*(a + i) % m) / n;				// get index in working array
			*(working[ind] + available[ind]) = *(a + i);	// add value to next available position in working[ind]
			available[ind]++;								// increment the available index for that position, so future values will not overwrite
		}

		// transfer values in new order back to a
		unsigned long index = 0;
		// for every digit in working[]
		for (int digit = 0; digit < DIGITS; digit++) {
			// for every index under that digit
			for (unsigned long i = 0; i < sizes[digit]; i++) {
				*(a + index) = *(working[digit] + i);	// add value to a
				index++;
			}
			free(working[digit]);	// free memory
		}

		// increment m and n values
		m *= DIGITS;
		n *= DIGITS;
	}
}

void main(int argc, char* argv[]) {
	// set random seed to internal clock
	srand(time(NULL));

	unsigned long numbers = 10000;		// amount of numbers in array
	unsigned long upperBound = 1000;	// upper bound on size of number

	int display = 0;	// 1 if array is being displayed

	// get command line arguments if they exist
	if (argc >= 3) {
		numbers = strtoul(argv[1], NULL, 10);
		upperBound = strtoul(argv[2], NULL, 10);

		// if third argument given, display numbers
		if (argv[3]) {
			display = 1;
		}
	}

	printf("%lu numbers from 0 to %lu\n", numbers, upperBound);

	unsigned long size = numbers * sizeof(unsigned long);	// memory to be allocated to array
	unsigned long *array = malloc(size);					// allocate memory to array pointer

	// initialize array of random unsigned long integers
	for (unsigned long i = 0; i < numbers; i++) {
		*(array + i) = ((rand() << 15) | rand()) % upperBound;	// get random values between 0 and upper bound
	}

	clock_t start = clock();	// get start time

	// sort array
	radix(array, size);

	clock_t end = clock();	// get end time
	double timeElapsed = (double) (end - start) / CLOCKS_PER_SEC * 1000;	// time elapsed in milliseconds

	// display array contents
	if (display) {
		printf("Sorted array contents:\n");
		for (unsigned long i = 0; i < numbers; i++) {
			printf("%ld\n", *(array + i));
		}
	}

	printf("\nTime Elapsed: %f ms.\n", timeElapsed);

	free(array);	// free memory
}