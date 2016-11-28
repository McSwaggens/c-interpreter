/* stdio.h library for large systems - small embedded systems use clibrary.c instead */
#include "../interpreter.h"
#include "../system_gpu.h"

#ifndef BUILTIN_MINI_STDLIB
#ifndef NO_FP


void gpu_clear(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	SystemGPU::Clear ((char*)Param[0]->Val->Pointer);
}

void gpu_push(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	SystemGPU::Display ();
}


struct LibraryFunction GPUFunctions[] =
{
	{ gpu_push,			"void gpu_push();" },
	{ gpu_clear,		"void gpu_clear(void* color);" },
	{ NULL,				NULL }
};

/* creates various system-dependent definitions */
void GPUSetupFunc(Picoc *pc)
{
}

#endif /* !NO_FP */
#endif /* !BUILTIN_MINI_STDLIB */
