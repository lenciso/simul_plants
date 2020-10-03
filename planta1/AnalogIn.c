/*  File    : AnalogIn.c
 *  Abstract: Analog in implementation
 *
 */

#define S_FUNCTION_NAME AnalogIn
#define S_FUNCTION_LEVEL 2

#if defined(__GNUC__) && !defined(__MINGW32__)
  #include <dlfcn.h>
  void *dllHandle;
#else
  #include <windows.h>
  HINSTANCE dllHandle = NULL;
  typedef void (CALLBACK* plant_init_type)(void);
  typedef double (CALLBACK* analog_in_type)(void);
#endif

#include "simstruc.h"

#include "plant.h"
  
#define U(element) (*uPtrs[element])  /* Pointer to Input Port0 */


 

/*====================*
 * S-function methods *
 *====================*/

/* Function: mdlInitializeSizes ===============================================
 * Abstract:
 *    The sizes information is used by Simulink to determine the S-function
 *    block's characteristics (number of inputs, outputs, states, etc.).
 */
static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 0);  /* Number of expected parameters */
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        return; /* Parameter mismatch will be reported by Simulink */
    }

    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);

    
    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, 1);

    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);
    ssSetOperatingPointCompliance(S, USE_DEFAULT_OPERATING_POINT);

    ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE);
     /*
   * Register reserved identifiers to avoid name conflict
   */
  if (ssRTWGenIsCodeGen(S) || ssGetSimMode(S)==SS_SIMMODE_EXTERNAL) {
    /*
     * Register reserved identifier for StartFcnSpec
     */
    ssRegMdlInfo(S, "plant_init", MDL_INFO_ID_RESERVED, 0, 0, ssGetPath(S));
    
    /*
     * Register reserved identifier for DerivativesFcnSpec
     */
    ssRegMdlInfo(S, "analog_in", MDL_INFO_ID_RESERVED, 0, 0, ssGetPath(S));
    
  }
    
}



/* Function: mdlInitializeSampleTimes =========================================
 * Abstract:
 *    Specify the sample time as 0.01
 */
static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, 0.01);
    ssSetOffsetTime(S, 0, 0.0);
    ssSetModelReferenceSampleTimeDefaultInheritance(S);      
}


#define MDL_START
#if defined(MDL_START)

/* Function: mdlStart =====================================================
 * Abstract:
 *    This function is called once at start of model execution. If you
 *    have states that should be initialized once, this is the place
 *    to do it.
 */
static void mdlStart(SimStruct *S)
{
  /*
   * Load the dynamic library
   */

  #if defined(__GNUC__) && !defined(__MINGW32__)
    void (*plant_init_ptr)(void);
    dllHandle = dlopen("./plant.so",RTLD_LAZY);
    plant_init_ptr = dlsym(dllHandle, "plant_init");
  #else
    plant_init_type plant_init_ptr = NULL;
    dllHandle = LoadLibrary("plant.dll");
    plant_init_ptr = (plant_init_type)GetProcAddress(dllHandle,"plant_init");
  #endif

  plant_init_ptr();
}
#endif


#define MDL_INITIALIZE_CONDITIONS
/* Function: mdlInitializeConditions ========================================
 * Abstract:
 *    Initialize both discrete states to one.
 */
static void mdlInitializeConditions(SimStruct *S)
{
    real_T *x0 = ssGetRealDiscStates(S);
    int_T  lp;

//     for (lp=0;lp<3;lp++) { 
//         *x0++=1.0; 
//     }
}



/* Function: mdlOutputs =======================================================
 * Abstract:
 *      y = Cx + Du 
 */
static void mdlOutputs(SimStruct *S, int_T tid)
{
    real_T            *y    = ssGetOutputPortRealSignal(S,0);
    real_T            *x    = ssGetRealDiscStates(S);
    InputRealPtrsType uPtrs = ssGetInputPortRealSignalPtrs(S,0);
 
    UNUSED_ARG(tid); /* not used in single tasking mode */

    #if defined(__GNUC__) && !defined(__MINGW32__)
        double (*analog_in_ptr)(void);
        analog_in_ptr = dlsym(dllHandle,"analog_in");
    #else
        analog_in_type analog_in_ptr = NULL;
        analog_in_ptr = (analog_in_type)GetProcAddress(dllHandle,"analog_in");
    #endif
    
    /* y=Cx+Du */
    
    *y = analog_in_ptr();
    
}



#define MDL_UPDATE
/* Function: mdlUpdate ======================================================
 * Abstract:
 *      xdot = Ax + Bu
 */
static void mdlUpdate(SimStruct *S, int_T tid)
{
    real_T            tempX[3] = {0.0, 0.0, 0.0};
    real_T            *x       = ssGetRealDiscStates(S);
    InputRealPtrsType uPtrs    = ssGetInputPortRealSignalPtrs(S,0);

    UNUSED_ARG(tid); /* not used in single tasking mode */

    /* xdot=Ax+Bu */
    // No process
}



/* Function: mdlTerminate =====================================================
 * Abstract:
 *    No termination needed, but we are required to have this routine.
 */
static void mdlTerminate(SimStruct *S)
{
    #if defined(__GNUC__) && !defined(__MINGW32__)
    dlclose(dllHandle);
  #else
    FreeLibrary(dllHandle);
  #endif
    UNUSED_ARG(S); /* unused input argument */
}

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
