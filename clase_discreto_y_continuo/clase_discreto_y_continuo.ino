/*
 * Ec diferencial dy = -y + x
 * y su transformada z con T variable: 0.2, 0.5 y 1
 */


double t = 0;
double u = 0, y = 0;

double T = 1;
double dt = T/5.0;

double h, h1=0,z=0;
int k = 1;
double a1 = -exp(-T);
double b0 = 0;
double b1 = 1 - exp(-T);

double referencia(int);
double f(double);
double k1, k2, k3, k4;
void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);
  t = 0;
}

void loop() {
  //t = millis();
  double tf = t/1000.0;
  Serial.println(String(tf) + " " + u + " " + y + " " + z);
  
  u = referencia(1);
  
  if(t > k*T*1000){
    h = u - a1*h1;
    z = b0*h + b1*h1;
    
    h1 = h;
    k++;
  }

  k1 = f(y);
  k2 = f(y + 1/2.0*k1*dt);
  k3 = f(y + 1/2.0*k2*dt);
  k4 = f(y + k3*dt);
  y = y + (k1 + 2*k2 + 2*k3 + k4)*dt/6.0;

  t += 50;
  delay(50);
}


double referencia(int sel_ref){
  double periodo = 16;
  double w = 2*PI/periodo;
  double amplitud = 10;
  double r;
  
  switch(sel_ref){
    case 1: //onda cuadrada
        if(sin(w*t/1000.0)>0){  // entre mil porque t está en milisegundos
          r =  amplitud;
        }else{
          r = -amplitud;
        }
        break;
    case 2:   // senoidal
        r =   amplitud * sin(w*t/1000.0);
        break;
    case 3:
        double ta = t/1000.0;
        double * ptrdouble;
        //r = modf(x/y,*ptr) -> guarda en r la parte fraccionaria de x, y en *ptr la parte entera 
        double tr = modf(ta/periodo,ptrdouble)*periodo;
        double p_2 = periodo/2;
        if(tr < p_2){
          r =-amplitud + tr * 2*amplitud/p_2;
        }else{
          r = amplitud-(tr - p_2)*2*amplitud/p_2;
        }
        break;
  }

  return r;
}

double f(double y){
  return -y + u;
}
