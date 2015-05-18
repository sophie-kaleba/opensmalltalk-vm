/* Automatically generated by
	VMPluginCodeGenerator VMMaker.oscog-eem.1313 uuid: f7251538-4676-49b4-bc2c-f2cfecd2a3ae
   from
	CameraPlugin VMMaker.oscog-eem.1313 uuid: f7251538-4676-49b4-bc2c-f2cfecd2a3ae
 */
static char __buildInfo[] = "CameraPlugin VMMaker.oscog-eem.1313 uuid: f7251538-4676-49b4-bc2c-f2cfecd2a3ae " __DATE__ ;



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

#include "CameraPlugin.h"
#include "sqMemoryAccess.h"


/*** Constants ***/


/*** Function Prototypes ***/
EXPORT(const char*) getModuleName(void);
EXPORT(sqInt) primCameraName(void);
EXPORT(sqInt) primCloseCamera(void);
EXPORT(sqInt) primFrameExtent(void);
EXPORT(sqInt) primGetFrame(void);
EXPORT(sqInt) primGetParam(void);
EXPORT(sqInt) primOpenCamera(void);
EXPORT(sqInt) setInterpreter(struct VirtualMachine*anInterpreter);


/*** Variables ***/

#if !defined(SQUEAK_BUILTIN_PLUGIN)
static sqInt (*classString)(void);
static sqInt (*failed)(void);
static void * (*firstIndexableField)(sqInt oop);
static sqInt (*instantiateClassindexableSize)(sqInt classPointer, sqInt size);
static sqInt (*integerObjectOf)(sqInt value);
static sqInt (*isWords)(sqInt oop);
static sqInt (*pop)(sqInt nItems);
static sqInt (*popthenPush)(sqInt nItems, sqInt oop);
static sqInt (*stSizeOf)(sqInt oop);
static sqInt (*stackIntegerValue)(sqInt offset);
static sqInt (*stackValue)(sqInt offset);
static sqInt (*success)(sqInt aBoolean);
#else /* !defined(SQUEAK_BUILTIN_PLUGIN) */
extern sqInt classString(void);
extern sqInt failed(void);
extern void * firstIndexableField(sqInt oop);
extern sqInt instantiateClassindexableSize(sqInt classPointer, sqInt size);
extern sqInt integerObjectOf(sqInt value);
extern sqInt isWords(sqInt oop);
extern sqInt pop(sqInt nItems);
extern sqInt popthenPush(sqInt nItems, sqInt oop);
extern sqInt stSizeOf(sqInt oop);
extern sqInt stackIntegerValue(sqInt offset);
extern sqInt stackValue(sqInt offset);
extern sqInt success(sqInt aBoolean);
extern
#endif
struct VirtualMachine* interpreterProxy;
static const char *moduleName =
#ifdef SQUEAK_BUILTIN_PLUGIN
	"CameraPlugin VMMaker.oscog-eem.1313 (i)"
#else
	"CameraPlugin VMMaker.oscog-eem.1313 (e)"
#endif
;



/*	Note: This is hardcoded so it can be run from Squeak.
	The module name is used for validating a module *after*
	it is loaded to check if it does really contain the module
	we're thinking it contains. This is important! */

EXPORT(const char*)
getModuleName(void)
{
	return moduleName;
}


/*	Get the name for the camera with the given number. Fail if the camera
	number is greater than the number of available cameras.
 */

EXPORT(sqInt)
primCameraName(void)
{
    sqInt cameraNum;
    sqInt count;
    char* dst;
    sqInt i;
    char* nameStr;
    sqInt resultOop;

	cameraNum = stackIntegerValue(0);
	if (failed()) {
		return 0;
	}
	nameStr = CameraName(cameraNum);
	if (nameStr == null) {
		success(0);
		return 0;
	}
	count = (int) strlen(nameStr);
	resultOop = instantiateClassindexableSize(classString(), count);
	dst = ((char *) (firstIndexableField(resultOop)));
	for (i = 0; i < count; i += 1) {
		dst[i] = (nameStr[i]);
	}
	popthenPush(2, resultOop);
	return 0;
}


/*	Close the camera. Do nothing if it was not open. */

EXPORT(sqInt)
primCloseCamera(void)
{
    sqInt cameraNum;

	cameraNum = stackIntegerValue(0);
	if (failed()) {
		return 0;
	}
	CameraClose(cameraNum);
	pop(1);
	return 0;
}


/*	Answer the frame extent of the given camera, or zero if the camera is not
	open. The extent is 16 bits of width and height packed into a single
	integer. 
 */

EXPORT(sqInt)
primFrameExtent(void)
{
    sqInt cameraNum;
    sqInt e;

	cameraNum = stackIntegerValue(0);
	if (failed()) {
		return 0;
	}
	e = CameraExtent(cameraNum);
	popthenPush(2, integerObjectOf(e));
	return 0;
}


/*	Copy a camera frame into the given Bitmap. The Bitmap should be for a Form
	of depth 32 that is the same width and height as the current camera frame.
	Fail if the camera is not open or if the bitmap is not the right size. If
	successful, answer the number of frames received from the camera since the
	last call. If this is zero, then there has been no change.
 */

EXPORT(sqInt)
primGetFrame(void)
{
    unsigned char *bitmap;
    sqInt bitmapOop;
    sqInt cameraNum;
    sqInt pixCount;
    sqInt result;

	cameraNum = stackIntegerValue(1);
	bitmapOop = stackValue(0);
	success(isWords(bitmapOop));
	if (failed()) {
		return 0;
	}
	bitmap = ((unsigned char *) (firstIndexableField(bitmapOop)));
	pixCount = stSizeOf(bitmapOop);
	result = CameraGetFrame(cameraNum, bitmap, pixCount);
	if (result < 0) {
		success(0);
		return 0;
	}
	popthenPush(3, integerObjectOf(result));
	return 0;
}


/*	Answer the given integer parameter of the given camera. */

EXPORT(sqInt)
primGetParam(void)
{
    sqInt cameraNum;
    sqInt paramNum;
    sqInt result;

	cameraNum = stackIntegerValue(1);
	paramNum = stackIntegerValue(0);
	if (failed()) {
		return 0;
	}
	result = CameraGetParam(cameraNum, paramNum);
	popthenPush(3, integerObjectOf(result));
	return 0;
}


/*	Open a camera. Takes one argument, the index of the device to open. */

EXPORT(sqInt)
primOpenCamera(void)
{
    sqInt cameraNum;
    sqInt desiredFrameHeight;
    sqInt desiredFrameWidth;
    sqInt ok;

	cameraNum = stackIntegerValue(2);
	desiredFrameWidth = stackIntegerValue(1);
	desiredFrameHeight = stackIntegerValue(0);
	if (failed()) {
		return 0;
	}
	ok = CameraOpen(cameraNum, desiredFrameWidth, desiredFrameHeight);
	if (ok == 0) {
		success(0);
		return 0;
	}
	pop(3);
	return 0;
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
		classString = interpreterProxy->classString;
		failed = interpreterProxy->failed;
		firstIndexableField = interpreterProxy->firstIndexableField;
		instantiateClassindexableSize = interpreterProxy->instantiateClassindexableSize;
		integerObjectOf = interpreterProxy->integerObjectOf;
		isWords = interpreterProxy->isWords;
		pop = interpreterProxy->pop;
		popthenPush = interpreterProxy->popthenPush;
		stSizeOf = interpreterProxy->stSizeOf;
		stackIntegerValue = interpreterProxy->stackIntegerValue;
		stackValue = interpreterProxy->stackValue;
		success = interpreterProxy->success;
#endif /* !defined(SQUEAK_BUILTIN_PLUGIN) */
	}
	return ok;
}


#ifdef SQUEAK_BUILTIN_PLUGIN

void* CameraPlugin_exports[][3] = {
	{"CameraPlugin", "getModuleName", (void*)getModuleName},
	{"CameraPlugin", "primCameraName\000\000", (void*)primCameraName},
	{"CameraPlugin", "primCloseCamera\000\000", (void*)primCloseCamera},
	{"CameraPlugin", "primFrameExtent\000\000", (void*)primFrameExtent},
	{"CameraPlugin", "primGetFrame\000\000", (void*)primGetFrame},
	{"CameraPlugin", "primGetParam\000\000", (void*)primGetParam},
	{"CameraPlugin", "primOpenCamera\000\000", (void*)primOpenCamera},
	{"CameraPlugin", "setInterpreter", (void*)setInterpreter},
	{NULL, NULL, NULL}
};

#else /* ifdef SQ_BUILTIN_PLUGIN */

signed char primCameraNameAccessorDepth = 0;
signed char primCloseCameraAccessorDepth = 0;
signed char primFrameExtentAccessorDepth = 0;
signed char primGetFrameAccessorDepth = 0;
signed char primGetParamAccessorDepth = 0;
signed char primOpenCameraAccessorDepth = 0;

#endif /* ifdef SQ_BUILTIN_PLUGIN */
