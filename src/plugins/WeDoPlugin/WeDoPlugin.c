/* Automatically generated by
	VMPluginCodeGenerator VMMaker.oscog-eem.1313 uuid: f7251538-4676-49b4-bc2c-f2cfecd2a3ae
   from
	WeDoPlugin VMMaker.oscog-eem.1313 uuid: f7251538-4676-49b4-bc2c-f2cfecd2a3ae
 */
static char __buildInfo[] = "WeDoPlugin VMMaker.oscog-eem.1313 uuid: f7251538-4676-49b4-bc2c-f2cfecd2a3ae " __DATE__ ;



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

#include "WeDoPlugin.h"
#include "sqMemoryAccess.h"


/*** Constants ***/


/*** Function Prototypes ***/
EXPORT(const char*) getModuleName(void);
EXPORT(sqInt) primClosePort(void);
EXPORT(sqInt) primOpenPort(void);
EXPORT(sqInt) primRead(void);
EXPORT(sqInt) primWrite(void);
EXPORT(sqInt) setInterpreter(struct VirtualMachine*anInterpreter);


/*** Variables ***/

#if !defined(SQUEAK_BUILTIN_PLUGIN)
static sqInt (*failed)(void);
static void * (*firstIndexableField)(sqInt oop);
static sqInt (*isBytes)(sqInt oop);
static sqInt (*pop)(sqInt nItems);
static sqInt (*pushInteger)(sqInt integerValue);
static sqInt (*stSizeOf)(sqInt oop);
static sqInt (*stackValue)(sqInt offset);
static sqInt (*success)(sqInt aBoolean);
#else /* !defined(SQUEAK_BUILTIN_PLUGIN) */
extern sqInt failed(void);
extern void * firstIndexableField(sqInt oop);
extern sqInt isBytes(sqInt oop);
extern sqInt pop(sqInt nItems);
extern sqInt pushInteger(sqInt integerValue);
extern sqInt stSizeOf(sqInt oop);
extern sqInt stackValue(sqInt offset);
extern sqInt success(sqInt aBoolean);
extern
#endif
struct VirtualMachine* interpreterProxy;
static const char *moduleName =
#ifdef SQUEAK_BUILTIN_PLUGIN
	"WeDoPlugin VMMaker.oscog-eem.1313 (i)"
#else
	"WeDoPlugin VMMaker.oscog-eem.1313 (e)"
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


/*	Close the WeDo port. */

EXPORT(sqInt)
primClosePort(void)
{
	success(WeDoClosePort());
	return 0;
}


/*	Open the WeDo port. */

EXPORT(sqInt)
primOpenPort(void)
{
	success(WeDoOpenPort());
	return 0;
}


/*	Read data from the WeDo port into the given buffer (a ByteArray or
	String). Answer the number of bytes read.
 */

EXPORT(sqInt)
primRead(void)
{
    sqInt bufOop;
    char *bufPtr;
    sqInt bufSize;
    sqInt byteCount;

	bufOop = stackValue(0);
	if (!(isBytes(bufOop))) {
		success(0);
		return 0;
	}
	bufPtr = ((char *) (firstIndexableField(bufOop)));
	bufSize = stSizeOf(bufOop);
	if (failed()) {
		return 0;
	}
	byteCount = WeDoRead(bufPtr, bufSize);
	if (byteCount < 0) {
		success(0);
		return 0;
	}
	pop(2);
	pushInteger(byteCount);
	return 0;
}


/*	Write data to the WeDo port from the given buffer (a ByteArray or String).
	Answer the number of bytes written.
 */

EXPORT(sqInt)
primWrite(void)
{
    sqInt bufOop;
    char *bufPtr;
    sqInt bufSize;
    sqInt byteCount;

	bufOop = stackValue(0);
	if (!(isBytes(bufOop))) {
		success(0);
		return 0;
	}
	bufPtr = ((char *) (firstIndexableField(bufOop)));
	bufSize = stSizeOf(bufOop);
	if (failed()) {
		return 0;
	}
	byteCount = WeDoWrite(bufPtr, bufSize);
	if (byteCount < 0) {
		success(0);
		return 0;
	}
	pop(2);
	pushInteger(byteCount);
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
		failed = interpreterProxy->failed;
		firstIndexableField = interpreterProxy->firstIndexableField;
		isBytes = interpreterProxy->isBytes;
		pop = interpreterProxy->pop;
		pushInteger = interpreterProxy->pushInteger;
		stSizeOf = interpreterProxy->stSizeOf;
		stackValue = interpreterProxy->stackValue;
		success = interpreterProxy->success;
#endif /* !defined(SQUEAK_BUILTIN_PLUGIN) */
	}
	return ok;
}


#ifdef SQUEAK_BUILTIN_PLUGIN

void* WeDoPlugin_exports[][3] = {
	{"WeDoPlugin", "getModuleName", (void*)getModuleName},
	{"WeDoPlugin", "primClosePort\000\377", (void*)primClosePort},
	{"WeDoPlugin", "primOpenPort\000\377", (void*)primOpenPort},
	{"WeDoPlugin", "primRead\000\000", (void*)primRead},
	{"WeDoPlugin", "primWrite\000\000", (void*)primWrite},
	{"WeDoPlugin", "setInterpreter", (void*)setInterpreter},
	{NULL, NULL, NULL}
};

#else /* ifdef SQ_BUILTIN_PLUGIN */

signed char primReadAccessorDepth = 0;
signed char primWriteAccessorDepth = 0;

#endif /* ifdef SQ_BUILTIN_PLUGIN */
