/* stdio.h library for large systems - small embedded systems use clibrary.c instead */
#ifndef BUILTIN_MINI_STDLIB

#include <errno.h>
#include <stdio.h>
#include "../interpreter.h"

#define MAX_FORMAT 80
#define MAX_SCANF_ARGS 10

static int Standard_ZeroValue = 0;
static int EOFValue = EOF;

/* our own internal output stream which can output to FILE * or strings */
typedef struct StdOutStreamStruct
{
    FILE *FilePtr;
    char *StrOutPtr;
    int StrOutLen;
    int CharCount;
    
} StdOutStream;

/* our representation of varargs within picoc */
struct StdVararg
{
    struct Value **Param;
    int NumArgs;
};

/* initialises the I/O system so error reporting works */
void BasicIOInit(Picoc *pc)
{
    pc->CStdOut = stdout;
}

void StandardPrint(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    printf ((char*)Param[0]->Val->Pointer);
}

/* handy structure definitions */
const char StandardDefs[] = "\
typedef struct __va_listStruct va_list; \
typedef struct __FILEStruct FILE;\
";

/* all stdio functions */
struct LibraryFunction StandardFunctions[] =
{
    { StandardPrint,  "void print(char *);" },
    { NULL,         NULL }
};

/* creates various system-dependent definitions */
void StandardSetupFunc(Picoc *pc)
{
    struct ValueType *StructFileType;
    struct ValueType *FilePtrType;

    /* make a "struct __FILEStruct" which is the same size as a native FILE structure */
    StructFileType = TypeCreateOpaqueStruct(pc, NULL, TableStrRegister(pc, "__FILEStruct"), sizeof(FILE));
    
    /* get a FILE * type */
    FilePtrType = TypeGetMatching(pc, NULL, StructFileType, TypePointer, 0, pc->StrEmpty, TRUE);

    /* make a "struct __va_listStruct" which is the same size as our struct StdVararg */
    TypeCreateOpaqueStruct(pc, NULL, TableStrRegister(pc, "__va_listStruct"), sizeof(FILE));
    
    /* define EOF equal to the system EOF */
    VariableDefinePlatformVar(pc, NULL, "EOF", &pc->IntType, (union AnyValue *)&EOFValue, FALSE);
    
    /* define NULL, TRUE and FALSE */
    if (!VariableDefined(pc, TableStrRegister(pc, "NULL")))
        VariableDefinePlatformVar(pc, NULL, "NULL", &pc->IntType, (union AnyValue *)&Standard_ZeroValue, FALSE);
}

/* portability-related I/O calls */
void PrintCh(char OutCh, FILE *Stream)
{
    putc(OutCh, Stream);
}

void PrintSimpleInt(long Num, FILE *Stream)
{
    fprintf(Stream, "%ld", Num);
}

void PrintStr(const char *Str, FILE *Stream)
{
    fputs(Str, Stream);
}

void PrintFP(double Num, FILE *Stream)
{
    fprintf(Stream, "%f", Num);
}

#endif /* !BUILTIN_MINI_STDLIB */
