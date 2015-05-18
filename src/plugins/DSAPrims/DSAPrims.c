/* Automatically generated by
	VMPluginCodeGenerator VMMaker.oscog-eem.1313 uuid: f7251538-4676-49b4-bc2c-f2cfecd2a3ae
   from
	DSAPlugin VMMaker.oscog-eem.1313 uuid: f7251538-4676-49b4-bc2c-f2cfecd2a3ae
 */
static char __buildInfo[] = "DSAPlugin VMMaker.oscog-eem.1313 uuid: f7251538-4676-49b4-bc2c-f2cfecd2a3ae " __DATE__ ;



#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Default EXPORT macro that does nothing (see comment in sq.h): */
#define EXPORT(returnType) returnType

/* Do not include the entire sq.h file but just those parts needed. */
/*  The virtual machine proxy definition */
#include "sqVirtualMachine.h"
/* Configuration options */
#include "sqConfig.h"
/* Platform specific definitions */
#include "sqPlatformSpecific.h"

#define true 1
#define false 0
#define null 0  /* using 'null' because nil is predefined in Think C */
#ifdef SQUEAK_BUILTIN_PLUGIN
#undef EXPORT
// was #undef EXPORT(returnType) but screws NorCroft cc
#define EXPORT(returnType) static returnType
#endif

#include "sqMemoryAccess.h"


/*** Constants ***/


/*** Function Prototypes ***/
static sqInt addBackDivisorDigitShift(sqInt digitShift);
static sqInt bigDivideLoop(void);
EXPORT(const char*) getModuleName(void);
static sqInt leftRotateby(unsigned int  anInteger, sqInt bits);
EXPORT(sqInt) primitiveBigDivide(void);
EXPORT(sqInt) primitiveBigMultiply(void);
EXPORT(sqInt) primitiveExpandBlock(void);
EXPORT(sqInt) primitiveHashBlock(void);
EXPORT(sqInt) primitiveHasSecureHashPrimitive(void);
EXPORT(sqInt) primitiveHighestNonZeroDigitIndex(void);
EXPORT(sqInt) setInterpreter(struct VirtualMachine*anInterpreter);
static sqInt subtractDivisorMultipliedByDigitdigitShift(sqInt digit, sqInt digitShift);


/*** Variables ***/
static sqInt divisorDigitCount;
static unsigned char* dsaDivisor;
static unsigned char* dsaQuotient;
static unsigned char* dsaRemainder;

#if !defined(SQUEAK_BUILTIN_PLUGIN)
static sqInt (*classLargePositiveInteger)(void);
static sqInt (*failed)(void);
static sqInt (*fetchClassOf)(sqInt oop);
static void * (*firstIndexableField)(sqInt oop);
static sqInt (*isBytes)(sqInt oop);
static sqInt (*isWords)(sqInt oop);
static sqInt (*pop)(sqInt nItems);
static sqInt (*pushBool)(sqInt trueOrFalse);
static sqInt (*pushInteger)(sqInt integerValue);
static sqInt (*stSizeOf)(sqInt oop);
static sqInt (*stackObjectValue)(sqInt offset);
static sqInt (*success)(sqInt aBoolean);
#else /* !defined(SQUEAK_BUILTIN_PLUGIN) */
extern sqInt classLargePositiveInteger(void);
extern sqInt failed(void);
extern sqInt fetchClassOf(sqInt oop);
extern void * firstIndexableField(sqInt oop);
extern sqInt isBytes(sqInt oop);
extern sqInt isWords(sqInt oop);
extern sqInt pop(sqInt nItems);
extern sqInt pushBool(sqInt trueOrFalse);
extern sqInt pushInteger(sqInt integerValue);
extern sqInt stSizeOf(sqInt oop);
extern sqInt stackObjectValue(sqInt offset);
extern sqInt success(sqInt aBoolean);
extern
#endif
struct VirtualMachine* interpreterProxy;
static const char *moduleName =
#ifdef SQUEAK_BUILTIN_PLUGIN
	"DSAPrims VMMaker.oscog-eem.1313 (i)"
#else
	"DSAPrims VMMaker.oscog-eem.1313 (e)"
#endif
;
static sqInt remainderDigitCount;



/*	Add back the divisor shifted left by the given number of digits. This is
	done only when the estimate of quotient digit was one larger than the
	correct value.
 */

static sqInt
addBackDivisorDigitShift(sqInt digitShift)
{
    sqInt carry;
    sqInt i;
    sqInt rIndex;
    sqInt sum;

	carry = 0;
	rIndex = digitShift + 1;
	for (i = 1; i <= divisorDigitCount; i += 1) {
		sum = ((dsaRemainder[rIndex]) + (dsaDivisor[i])) + carry;
		dsaRemainder[rIndex] = (sum & 0xFF);
		carry = ((usqInt) sum >> 8);
		rIndex += 1;
	}
	sum = (dsaRemainder[rIndex]) + carry;
	dsaRemainder[rIndex] = (sum & 0xFF);
}


/*	This is the core of the divide algorithm. This loop steps through the
	digit positions of the quotient, each time estimating the right quotient
	digit, subtracting from the remainder the divisor times the quotient digit
	shifted left by the appropriate number of digits. When the loop
	terminates, all digits of the quotient have been filled in and the
	remainder contains a value less than the divisor. The tricky bit is
	estimating the next quotient digit. Knuth shows that the digit estimate
	computed here will never be less than it should be and cannot be more than
	one over what it should be. Furthermore, the case where the estimate is
	one too large is extremely rare. For example, in a typical test of 100000
	random 60-bit division problems, the rare case only occured five times.
	See Knuth, volume 2 ('Semi-Numerical Algorithms') 2nd edition, pp. 257-260
 */
/*	extract the top two digits of the divisor */

static sqInt
bigDivideLoop(void)
{
    sqInt borrow;
    sqInt carry;
    unsigned char d1;
    unsigned char d2;
    sqInt digitShift;
    unsigned char firstDigit;
    sqInt firstTwoDigits;
    sqInt i;
    sqInt i1;
    sqInt j;
    sqInt prod;
    sqInt q;
    sqInt qTooBig;
    sqInt resultDigit;
    sqInt rIndex;
    sqInt rIndex1;
    sqInt sum;
    unsigned char thirdDigit;

	d1 = dsaDivisor[divisorDigitCount];
	d2 = dsaDivisor[divisorDigitCount - 1];
	for (j = remainderDigitCount; j >= (divisorDigitCount + 1); j += -1) {

		/* extract the top several digits of remainder. */

		firstDigit = dsaRemainder[j];
		firstTwoDigits = (((usqInt) firstDigit << 8)) + (dsaRemainder[j - 1]);

		/* estimate q, the next digit of the quotient */

		thirdDigit = dsaRemainder[j - 2];
		if (firstDigit == d1) {
			q = 0xFF;
		}
		else {
			q = firstTwoDigits / d1;
		}
		if ((d2 * q) > ((((usqInt) (firstTwoDigits - (q * d1)) << 8)) + thirdDigit)) {
			q -= 1;
			if ((d2 * q) > ((((usqInt) (firstTwoDigits - (q * d1)) << 8)) + thirdDigit)) {
				q -= 1;
			}
		}
		digitShift = (j - divisorDigitCount) - 1;
		if (q > 0) {
			/* begin subtractDivisorMultipliedByDigit:digitShift: */
			borrow = 0;
			rIndex1 = digitShift + 1;
			for (i1 = 1; i1 <= divisorDigitCount; i1 += 1) {
				prod = ((dsaDivisor[i1]) * q) + borrow;
				borrow = ((usqInt) prod >> 8);
				resultDigit = (dsaRemainder[rIndex1]) - (prod & 0xFF);
				if (resultDigit < 0) {

					/* borrow from the next digit */

					resultDigit += 256;
					borrow += 1;
				}
				dsaRemainder[rIndex1] = resultDigit;
				rIndex1 += 1;
			}
			if (borrow == 0) {
				qTooBig = 0;
				goto l1;
			}
			resultDigit = (dsaRemainder[rIndex1]) - borrow;
			if (resultDigit < 0) {

				/* digit was too large (this case is quite rare) */

				dsaRemainder[rIndex1] = (resultDigit + 256);
				qTooBig = 1;
				goto l1;
			}
			else {
				dsaRemainder[rIndex1] = resultDigit;
				qTooBig = 0;
				goto l1;
			}
		l1:	/* end subtractDivisorMultipliedByDigit:digitShift: */;
			if (qTooBig) {

				/* this case is extremely rare */

				/* begin addBackDivisorDigitShift: */
				carry = 0;
				rIndex = digitShift + 1;
				for (i = 1; i <= divisorDigitCount; i += 1) {
					sum = ((dsaRemainder[rIndex]) + (dsaDivisor[i])) + carry;
					dsaRemainder[rIndex] = (sum & 0xFF);
					carry = ((usqInt) sum >> 8);
					rIndex += 1;
				}
				sum = (dsaRemainder[rIndex]) + carry;
				dsaRemainder[rIndex] = (sum & 0xFF);
				q -= 1;
			}
		}
		dsaQuotient[digitShift + 1] = q;
	}
}


/*	Note: This is hardcoded so it can be run from Squeak.
	The module name is used for validating a module *after*
	it is loaded to check if it does really contain the module
	we're thinking it contains. This is important! */

EXPORT(const char*)
getModuleName(void)
{
	return moduleName;
}


/*	Rotate the given 32-bit integer left by the given number of bits and
	answer the result.
 */

static sqInt
leftRotateby(unsigned int  anInteger, sqInt bits)
{
	return (anInteger << bits) | (((usqInt) anInteger) >> (32 - bits));
}


/*	Called with three LargePositiveInteger arguments, rem, div, quo. Divide
	div into rem and store the quotient into quo, leaving the remainder in
	rem. 
 */
/*	Assume: quo starts out filled with zeros. */

EXPORT(sqInt)
primitiveBigDivide(void)
{
    sqInt borrow;
    sqInt carry;
    unsigned char d1;
    unsigned char d2;
    sqInt digitShift;
    sqInt div;
    unsigned char firstDigit;
    sqInt firstTwoDigits;
    sqInt i;
    sqInt i1;
    sqInt j;
    sqInt prod;
    sqInt q;
    sqInt qTooBig;
    sqInt quo;
    sqInt rem;
    sqInt resultDigit;
    sqInt rIndex;
    sqInt rIndex1;
    sqInt sum;
    unsigned char thirdDigit;

	quo = stackObjectValue(0);
	div = stackObjectValue(1);
	rem = stackObjectValue(2);
	success((fetchClassOf(rem)) == (classLargePositiveInteger()));
	success((fetchClassOf(div)) == (classLargePositiveInteger()));
	success((fetchClassOf(quo)) == (classLargePositiveInteger()));
	if (failed()) {
		return null;
	}
	dsaRemainder = firstIndexableField(rem);
	dsaDivisor = firstIndexableField(div);
	dsaQuotient = firstIndexableField(quo);
	divisorDigitCount = stSizeOf(div);

	/* adjust pointers for base-1 indexing */

	remainderDigitCount = stSizeOf(rem);
	dsaRemainder -= 1;
	dsaDivisor -= 1;
	dsaQuotient -= 1;
	/* begin bigDivideLoop */
	d1 = dsaDivisor[divisorDigitCount];
	d2 = dsaDivisor[divisorDigitCount - 1];
	for (j = remainderDigitCount; j >= (divisorDigitCount + 1); j += -1) {

		/* extract the top several digits of remainder. */

		firstDigit = dsaRemainder[j];
		firstTwoDigits = (((usqInt) firstDigit << 8)) + (dsaRemainder[j - 1]);

		/* estimate q, the next digit of the quotient */

		thirdDigit = dsaRemainder[j - 2];
		if (firstDigit == d1) {
			q = 0xFF;
		}
		else {
			q = firstTwoDigits / d1;
		}
		if ((d2 * q) > ((((usqInt) (firstTwoDigits - (q * d1)) << 8)) + thirdDigit)) {
			q -= 1;
			if ((d2 * q) > ((((usqInt) (firstTwoDigits - (q * d1)) << 8)) + thirdDigit)) {
				q -= 1;
			}
		}
		digitShift = (j - divisorDigitCount) - 1;
		if (q > 0) {
			/* begin subtractDivisorMultipliedByDigit:digitShift: */
			borrow = 0;
			rIndex1 = digitShift + 1;
			for (i1 = 1; i1 <= divisorDigitCount; i1 += 1) {
				prod = ((dsaDivisor[i1]) * q) + borrow;
				borrow = ((usqInt) prod >> 8);
				resultDigit = (dsaRemainder[rIndex1]) - (prod & 0xFF);
				if (resultDigit < 0) {

					/* borrow from the next digit */

					resultDigit += 256;
					borrow += 1;
				}
				dsaRemainder[rIndex1] = resultDigit;
				rIndex1 += 1;
			}
			if (borrow == 0) {
				qTooBig = 0;
				goto l1;
			}
			resultDigit = (dsaRemainder[rIndex1]) - borrow;
			if (resultDigit < 0) {

				/* digit was too large (this case is quite rare) */

				dsaRemainder[rIndex1] = (resultDigit + 256);
				qTooBig = 1;
				goto l1;
			}
			else {
				dsaRemainder[rIndex1] = resultDigit;
				qTooBig = 0;
				goto l1;
			}
		l1:	/* end subtractDivisorMultipliedByDigit:digitShift: */;
			if (qTooBig) {

				/* this case is extremely rare */

				/* begin addBackDivisorDigitShift: */
				carry = 0;
				rIndex = digitShift + 1;
				for (i = 1; i <= divisorDigitCount; i += 1) {
					sum = ((dsaRemainder[rIndex]) + (dsaDivisor[i])) + carry;
					dsaRemainder[rIndex] = (sum & 0xFF);
					carry = ((usqInt) sum >> 8);
					rIndex += 1;
				}
				sum = (dsaRemainder[rIndex]) + carry;
				dsaRemainder[rIndex] = (sum & 0xFF);
				q -= 1;
			}
		}
		dsaQuotient[digitShift + 1] = q;
	}
	pop(3);
}


/*	Multiple f1 by f2, placing the result into prod. f1, f2, and prod must be
	LargePositiveIntegers, and the length of prod must be the sum of the
	lengths of f1 and f2.
 */
/*	Assume: prod starts out filled with zeros */

EXPORT(sqInt)
primitiveBigMultiply(void)
{
    sqInt carry;
    unsigned char digit;
    sqInt f1;
    sqInt f1Len;
    unsigned char *f1Ptr;
    sqInt f2;
    sqInt f2Len;
    unsigned char *f2Ptr;
    sqInt i;
    sqInt j;
    sqInt k;
    sqInt prod;
    sqInt prodLen;
    unsigned char *prodPtr;
    sqInt sum;

	prod = stackObjectValue(0);
	f2 = stackObjectValue(1);
	f1 = stackObjectValue(2);
	success(isBytes(prod));
	success(isBytes(f2));
	success(isBytes(f1));
	success((fetchClassOf(prod)) == (classLargePositiveInteger()));
	success((fetchClassOf(f2)) == (classLargePositiveInteger()));
	success((fetchClassOf(f1)) == (classLargePositiveInteger()));
	if (failed()) {
		return null;
	}
	prodLen = stSizeOf(prod);
	f1Len = stSizeOf(f1);
	f2Len = stSizeOf(f2);
	success(prodLen == (f1Len + f2Len));
	if (failed()) {
		return null;
	}
	prodPtr = firstIndexableField(prod);
	f2Ptr = firstIndexableField(f2);
	f1Ptr = firstIndexableField(f1);
	for (i = 0; i < f1Len; i += 1) {
		if (((digit = f1Ptr[i])) != 0) {
			carry = 0;

			/* Loop invariants: 0 <= carry <= 16rFF, k = i + j - 1 */

			k = i;
			for (j = 0; j < f2Len; j += 1) {
				sum = (((f2Ptr[j]) * digit) + (prodPtr[k])) + carry;
				carry = ((usqInt) sum >> 8);
				prodPtr[k] = (sum & 0xFF);
				k += 1;
			}
			prodPtr[k] = carry;
		}
	}
	pop(3);
}


/*	Expand a 64 byte ByteArray (the first argument) into and an Bitmap of 80
	32-bit words (the second argument). When reading a 32-bit integer from the
	ByteArray, consider the first byte to contain the most significant bits of
	the word (i.e., use big-endian byte ordering).
 */

EXPORT(sqInt)
primitiveExpandBlock(void)
{
    sqInt buf;
    unsigned char *bytePtr;
    sqInt expanded;
    sqInt i;
    sqInt src;
    sqInt v;
    unsigned int *wordPtr;

	expanded = stackObjectValue(0);
	buf = stackObjectValue(1);
	success(isWords(expanded));
	success(isBytes(buf));
	if (failed()) {
		return null;
	}
	success((stSizeOf(expanded)) == 80);
	success((stSizeOf(buf)) == 64);
	if (failed()) {
		return null;
	}
	wordPtr = firstIndexableField(expanded);
	bytePtr = firstIndexableField(buf);
	src = 0;
	for (i = 0; i <= 15; i += 1) {
		v = ((((bytePtr[src]) << 24) + ((bytePtr[src + 1]) << 16)) + ((bytePtr[src + 2]) << 8)) + (bytePtr[src + 3]);
		wordPtr[i] = v;
		src += 4;
	}
	for (i = 16; i <= 79; i += 1) {
		v = (((wordPtr[i - 3]) ^ (wordPtr[i - 8])) ^ (wordPtr[i - 14])) ^ (wordPtr[i - 16]);
		v = (v << 1) | (((usqInt) v) >> (32 - 1));
		wordPtr[i] = v;
	}
	pop(2);
}


/*	Hash a Bitmap of 80 32-bit words (the first argument), using the given
	state (the second argument).
 */

EXPORT(sqInt)
primitiveHashBlock(void)
{
    unsigned int a;
    unsigned int b;
    sqInt buf;
    unsigned int *bufPtr;
    unsigned int c;
    unsigned int d;
    unsigned int e;
    sqInt i;
    sqInt state;
    unsigned int *statePtr;
    sqInt tmp;

	state = stackObjectValue(0);
	buf = stackObjectValue(1);
	success(isWords(state));
	success(isWords(buf));
	if (failed()) {
		return null;
	}
	success((stSizeOf(state)) == 5);
	success((stSizeOf(buf)) == 80);
	if (failed()) {
		return null;
	}
	statePtr = firstIndexableField(state);
	bufPtr = firstIndexableField(buf);
	a = statePtr[0];
	b = statePtr[1];
	c = statePtr[2];
	d = statePtr[3];
	e = statePtr[4];
	for (i = 0; i <= 19; i += 1) {
		tmp = (((1518500249 + ((b & c) | (((unsigned int)~b) & d))) + ((a << 5) | (((usqInt) a) >> (32 - 5)))) + e) + (bufPtr[i]);
		e = d;
		d = c;
		c = (b << 30) | (((usqInt) b) >> (32 - 30));
		b = a;
		a = tmp;
	}
	for (i = 20; i <= 39; i += 1) {
		tmp = (((1859775393 + ((b ^ c) ^ d)) + ((a << 5) | (((usqInt) a) >> (32 - 5)))) + e) + (bufPtr[i]);
		e = d;
		d = c;
		c = (b << 30) | (((usqInt) b) >> (32 - 30));
		b = a;
		a = tmp;
	}
	for (i = 40; i <= 59; i += 1) {
		tmp = (((2400959708UL + (((b & c) | (b & d)) | (c & d))) + ((a << 5) | (((usqInt) a) >> (32 - 5)))) + e) + (bufPtr[i]);
		e = d;
		d = c;
		c = (b << 30) | (((usqInt) b) >> (32 - 30));
		b = a;
		a = tmp;
	}
	for (i = 60; i <= 79; i += 1) {
		tmp = (((3395469782UL + ((b ^ c) ^ d)) + ((a << 5) | (((usqInt) a) >> (32 - 5)))) + e) + (bufPtr[i]);
		e = d;
		d = c;
		c = (b << 30) | (((usqInt) b) >> (32 - 30));
		b = a;
		a = tmp;
	}
	statePtr[0] = ((statePtr[0]) + a);
	statePtr[1] = ((statePtr[1]) + b);
	statePtr[2] = ((statePtr[2]) + c);
	statePtr[3] = ((statePtr[3]) + d);
	statePtr[4] = ((statePtr[4]) + e);
	pop(2);
}


/*	Answer true if the secure hash primitive is implemented. */

EXPORT(sqInt)
primitiveHasSecureHashPrimitive(void)
{
	pop(1);
	pushBool(1);
}


/*	Called with one LargePositiveInteger argument. Answer the index of the
	top-most non-zero digit.
 */

EXPORT(sqInt)
primitiveHighestNonZeroDigitIndex(void)
{
    sqInt arg;
    unsigned char *bigIntPtr;
    sqInt i;

	arg = stackObjectValue(0);
	success((fetchClassOf(arg)) == (classLargePositiveInteger()));
	if (failed()) {
		return null;
	}
	bigIntPtr = firstIndexableField(arg);
	i = stSizeOf(arg);
	while ((i > 0)
	 && ((bigIntPtr[(i -= 1)]) == 0)) {
	}
	pop(1);
	pushInteger(i + 1);
}


/*	Note: This is coded so that it can be run in Squeak. */

EXPORT(sqInt)
setInterpreter(struct VirtualMachine*anInterpreter)
{
    sqInt ok;

	interpreterProxy = anInterpreter;
	ok = ((interpreterProxy->majorVersion()) == (VM_PROXY_MAJOR))
	 && ((interpreterProxy->minorVersion()) >= (VM_PROXY_MINOR));
	if (ok) {
		
#if !defined(SQUEAK_BUILTIN_PLUGIN)
		classLargePositiveInteger = interpreterProxy->classLargePositiveInteger;
		failed = interpreterProxy->failed;
		fetchClassOf = interpreterProxy->fetchClassOf;
		firstIndexableField = interpreterProxy->firstIndexableField;
		isBytes = interpreterProxy->isBytes;
		isWords = interpreterProxy->isWords;
		pop = interpreterProxy->pop;
		pushBool = interpreterProxy->pushBool;
		pushInteger = interpreterProxy->pushInteger;
		stSizeOf = interpreterProxy->stSizeOf;
		stackObjectValue = interpreterProxy->stackObjectValue;
		success = interpreterProxy->success;
#endif /* !defined(SQUEAK_BUILTIN_PLUGIN) */
	}
	return ok;
}


/*	Multiply the divisor by the given digit (an integer in the range 0..255),
	shift it left by the given number of digits, and subtract the result from
	the current remainder. Answer true if there is an excess borrow,
	indicating that digit was one too large. (This case is quite rare.)
 */

static sqInt
subtractDivisorMultipliedByDigitdigitShift(sqInt digit, sqInt digitShift)
{
    sqInt borrow;
    sqInt i;
    sqInt prod;
    sqInt resultDigit;
    sqInt rIndex;

	borrow = 0;
	rIndex = digitShift + 1;
	for (i = 1; i <= divisorDigitCount; i += 1) {
		prod = ((dsaDivisor[i]) * digit) + borrow;
		borrow = ((usqInt) prod >> 8);
		resultDigit = (dsaRemainder[rIndex]) - (prod & 0xFF);
		if (resultDigit < 0) {

			/* borrow from the next digit */

			resultDigit += 256;
			borrow += 1;
		}
		dsaRemainder[rIndex] = resultDigit;
		rIndex += 1;
	}
	if (borrow == 0) {
		return 0;
	}
	resultDigit = (dsaRemainder[rIndex]) - borrow;
	if (resultDigit < 0) {

		/* digit was too large (this case is quite rare) */

		dsaRemainder[rIndex] = (resultDigit + 256);
		return 1;
	}
	else {
		dsaRemainder[rIndex] = resultDigit;
		return 0;
	}
}


#ifdef SQUEAK_BUILTIN_PLUGIN

void* DSAPrims_exports[][3] = {
	{"DSAPrims", "getModuleName", (void*)getModuleName},
	{"DSAPrims", "primitiveBigDivide\000\001", (void*)primitiveBigDivide},
	{"DSAPrims", "primitiveBigMultiply\000\001", (void*)primitiveBigMultiply},
	{"DSAPrims", "primitiveExpandBlock\000\001", (void*)primitiveExpandBlock},
	{"DSAPrims", "primitiveHashBlock\000\001", (void*)primitiveHashBlock},
	{"DSAPrims", "primitiveHasSecureHashPrimitive\000\377", (void*)primitiveHasSecureHashPrimitive},
	{"DSAPrims", "primitiveHighestNonZeroDigitIndex\000\001", (void*)primitiveHighestNonZeroDigitIndex},
	{"DSAPrims", "setInterpreter", (void*)setInterpreter},
	{NULL, NULL, NULL}
};

#else /* ifdef SQ_BUILTIN_PLUGIN */

signed char primitiveBigDivideAccessorDepth = 1;
signed char primitiveBigMultiplyAccessorDepth = 1;
signed char primitiveExpandBlockAccessorDepth = 1;
signed char primitiveHashBlockAccessorDepth = 1;
signed char primitiveHighestNonZeroDigitIndexAccessorDepth = 1;

#endif /* ifdef SQ_BUILTIN_PLUGIN */
