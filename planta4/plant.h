#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#ifdef __GNUC__
#define PLANT_API
#else
#define PLANT_API __declspec(dllexport)
#endif

PLANT_API extern double xx[];
PLANT_API extern double uu;
PLANT_API extern double u_[];

PLANT_API extern void plant_init(void);

PLANT_API extern double plant_dynamics1(double *x, double u);
PLANT_API extern double plant_dynamics2(double *x, double u);
PLANT_API extern double plant_dynamics3(double *x, double u);

PLANT_API extern void plant_update(double *x, double u);

PLANT_API extern double analog_out(double u);

PLANT_API extern double analog_in(void);

unsigned long mix(unsigned long a, unsigned long b, unsigned long c);