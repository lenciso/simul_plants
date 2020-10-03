#include "plant.h"

double xx[3]={0.0};
double uu=0.0;
double u_[9]={0.0};

void plant_init(void) {
    xx[0] = 0.0;
    xx[1] = 0.0;
    xx[2] = 0.0;
    uu = 0.0;
}

double plant_dynamics1(double *x, double u){
//     unsigned long seed = mix(clock(), time(NULL), getpid());
//     srand(seed);
    /* xdot=Ax+Bu */
    double dx = 0.3679*x[0]+0.0063*u_[3];
    return dx;
}

double plant_dynamics2(double *x, double u){
    unsigned long seed = mix(clock(), time(NULL), getpid());
    srand(seed);
    /* xdot=Ax+Bu */
    double dx = 0.9418*x[1]+9.7683*(x[0])+0.0576*u_[3];//+(double)rand()/RAND_MAX*0.1-0.05;
    return dx;
}

double plant_dynamics3(double *x, double u){
//     unsigned long seed = mix(clock(), time(NULL), getpid());
//     srand(seed);
    /* xdot=Ax+Bu */
    double dx = 0.5769*x[2]+0.4095*x[1]+2.6205*x[0]+0.01*u_[3];
    
    u_[4] = u_[3];
    u_[3] = u_[2];
    u_[2] = u_[1];
    u_[1] = u_[0];
    u_[0] = u;
    
    return dx;
}

void plant_update(double *x, double u){
    xx[0] = x[0];
    xx[1] = x[1];
    xx[2] = x[2];
    uu = u;
}

double analog_out(double u){
    double scale = 1.0;
    double bias = 0.2;
    double ux = u*scale+bias;
    return ux;
}

double analog_in(void){
    unsigned long seed = mix(clock(), time(NULL), getpid());
    srand(seed);
    /* y=Cx+Du */
    double y = xx[2] + (double)rand()/RAND_MAX*5-2.5;
    return y;
}

unsigned long mix(unsigned long a, unsigned long b, unsigned long c)
{
    a=a-b;  a=a-c;  a=a^(c >> 13);
    b=b-c;  b=b-a;  b=b^(a << 8);
    c=c-a;  c=c-b;  c=c^(b >> 13);
    a=a-b;  a=a-c;  a=a^(c >> 12);
    b=b-c;  b=b-a;  b=b^(a << 16);
    c=c-a;  c=c-b;  c=c^(b >> 5);
    a=a-b;  a=a-c;  a=a^(c >> 3);
    b=b-c;  b=b-a;  b=b^(a << 10);
    c=c-a;  c=c-b;  c=c^(b >> 15);
    return c;
}