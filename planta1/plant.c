#include "plant.h"

double xx[3]={0.0};
double uu=0.0;
double u_[9]={0.0};

void plant_init(void) {
    unsigned long seed = mix(clock(), time(NULL), getpid());
    srand(seed);
    xx[0] = 0.0;
    xx[1] = 0.0;
    xx[2] = 0.0;
    uu = 0.0;
}

double plant_dynamics1(double *x, double u){
    /* xdot=Ax+Bu */
    double dx = 0.3679*x[0]+0.0063*u_[4];
    return dx;
}

double plant_dynamics2(double *x, double u){
    /* xdot=Ax+Bu */
    double dx = 0.9608*x[1]+6.1761*(x[0])+0.0363*u_[4];//+(double)rand()/RAND_MAX*0.1-0.05;
    return dx;
}

double plant_dynamics3(double *x, double u){
    /* xdot=Ax+Bu */
    double dx = 0.6065*x[3]+0.3851*x[2]+1.5251*x[1]+0.0058*u_[4];
    
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
    double bias = 0.1;
    double ux = u*scale+bias;
    return ux;
}

double analog_in(void){
    /* y=Cx+Du */
    double y = xx[2] + (double)rand()/RAND_MAX*2-1.0;
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