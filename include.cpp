/* picoc include system - can emulate system includes from built-in libraries
 * or it can include and parse files if the system has files */
 
#include "picoc.h"
#include "interpreter.h"

#ifndef NO_HASH_INCLUDE


/* initialise the built-in include libraries */
void IncludeInit(Picoc *pc)
{
    IncludeRegister(pc, "math.h", &MathSetupFunc, &MathFunctions[0], NULL);
    IncludeRegister(pc, "gpu.h", &GPUSetupFunc, &GPUFunctions[0], NULL);
    IncludeRegister(pc, "standard.h", &StandardSetupFunc, &StandardFunctions[0], StandardDefs);
}

/* clean up space used by the include system */
void IncludeCleanup(Picoc *pc)
{
    struct IncludeLibrary *ThisInclude = pc->IncludeLibList;
    struct IncludeLibrary *NextInclude;
    
    while (ThisInclude != NULL)
    {
        NextInclude = ThisInclude->NextLib;
        HeapFreeMem(pc, ThisInclude);
        ThisInclude = NextInclude;
    }

    pc->IncludeLibList = NULL;
}

/* register a new build-in include file */
void IncludeRegister(Picoc *pc, const char *IncludeName, void (*SetupFunction)(Picoc *pc), struct LibraryFunction *FuncList, const char *SetupCSource)
{
    struct IncludeLibrary *NewLib = HeapAllocMem(pc, sizeof(struct IncludeLibrary));
    NewLib->IncludeName = TableStrRegister(pc, IncludeName);
    NewLib->SetupFunction = SetupFunction;
    NewLib->FuncList = FuncList;
    NewLib->SetupCSource = SetupCSource;
    NewLib->NextLib = pc->IncludeLibList;
    pc->IncludeLibList = NewLib;
}

/* include all of the system headers */
void PicocIncludeAllSystemHeaders(Picoc *pc)
{
    struct IncludeLibrary *ThisInclude = pc->IncludeLibList;
    
    for (; ThisInclude != NULL; ThisInclude = ThisInclude->NextLib)
        IncludeFile(pc, ThisInclude->IncludeName);
}

/* include one of a number of predefined libraries */
void IncludePredefinedFile(Picoc *pc, char *FileName)
{
    struct IncludeLibrary *LInclude;
    
    /* scan for the include file name to see if it's in our list of predefined includes */
    for (LInclude = pc->IncludeLibList; LInclude != NULL; LInclude = LInclude->NextLib)
    {
        if (strcmp(LInclude->IncludeName, FileName) == 0)
        {
            /* found it - protect against multiple inclusion */
            if (!VariableDefined(pc, FileName))
            {
                VariableDefine(pc, NULL, FileName, NULL, &pc->VoidType, FALSE);
                
                /* run an extra startup function if there is one */
                if (LInclude->SetupFunction != NULL)
                    (*LInclude->SetupFunction)(pc);
                
                /* parse the setup C source code - may define types etc. */
                if (LInclude->SetupCSource != NULL)
                    PicocParse(pc, FileName, LInclude->SetupCSource, strlen(LInclude->SetupCSource), TRUE, TRUE, FALSE, FALSE);
                
                /* set up the library functions */
                if (LInclude->FuncList != NULL)
                    LibraryAdd(pc, &pc->GlobalTable, FileName, LInclude->FuncList);
            }
            return;
        }
    }
}

void IncludeFile(Picoc *pc, char *FileName)
{
    PicocPlatformScanFile(pc, FileName);
}

#endif /* NO_HASH_INCLUDE */
