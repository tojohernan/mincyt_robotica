/*
 * EVITADOR DE COLISIONES, MECANTRONIC V1
 * 
 *  Este codigo es para que el robot evite colisiones utilizando el sensor de distancia de Ultra Sonido
 *  Para este 
 *  
 *  Autores: Sofia Villar y Naim Ayoub
    Comentarios y modificaciones: Bernardo de Elia
 */

//-------------------------------DECLARACION DE LIBRERIAS-----------------------------------
#include <SoftwareSerial.h>
#include <Servo.h> // Se incluye la librería para hacer mas facil el uso de servos, mas info en: https://www.arduino.cc/en/reference/servo

//---------------------------------------------------------------------------------------------------------


//-------------------------------DECLARACION DE VARIABLES Y CONSTANTES-----------------------------------

Servo ServoAzul; // Declaro un objeto de la clase Servo, este objeto puede usar las funciones de la libreria Servo

//PINES DEL SERVO Sg09

#define pinServo 2 // Este es el pin digital (no necesita ser pwm) que comandara la pocision del servo a travez del cable amarillo

//PINES DEL SENSOR DE ULTRA SONIDO Hc-04

#define pinUSTrig 12  //  Por este pin se comandara el Trigger (gatillo) del ultrasonido, al activarlo en HIGH genera un tono ultrasonico 
#define pinUSEcho 13  //  Y por este pin reciviremos el eco del tono ultrasonico
                      //  nota: los pines de entrada que se eligieron para este ejemplo son A#, lo que quiere decir que pueden leer señales analogicas
                      //        pero nosotros los usaremos como entradas digitales.


//PINES DE MOTOR (los colores representan el color del Cable H-H (hembra-hembra) que se conecta entre el Shield V5 y el Driver L298n, si prefiere usar otros colores no hay problema)

#define DA  8   //  amarillo, Motor derecha, sentido adelante 
#define DR  9   //  naranja, Motor derecha, sentido reversa 
#define PWM_D 10  //  velocidad del motor derecho

#define IA  7   //  verde, Motor izquierda, sentido adelante 
#define IR  6   //  azul, Motor izquierda, sentido reversa
#define PWM_I 5 //  velocidad del motor izquierdo, recordemos que tiene que tener capacidades PWM el pin para poder variar la velocidad
//---------------------------------------------------------------------------------------------------------

//-----------------------CONFIGURACIONES INICIALES---------------------------------------------------------


void setup() {
  //pinMode(pinIN1, OUTPUT);
  //pinMode(pinIN2, OUTPUT);
  //pinMode(pinIN3, OUTPUT);
  //pinMode(pinIN4, OUTPUT);
  Serial.begin(9600);
  pinMode(pinUSTrig, OUTPUT);
  pinMode(pinUSEcho, INPUT);

  ServoAzul.attach(pinServo);  //Asocio mi servo al pin
}

//--------------------------------------------------------------------------------------------------------------

//-----------------------CICLO INFINITO DONDE VA EL PROGRAMA PRINCIPAL ----------------------------------------


void loop(){
Serial.println(medir());
 if (medir()<=20) { //Defino mi condición. "Si la distancia medida por el Hc-04 es menor o igual a 20 (Parametro en cm)

     parar();     // Detengo el robot para poder medir mis otras opciones
     
     int angulos[3] = {45, 90, 135}; // Creo vector de posiciones donde quiero medir, osea voy a mirar a mi derecha, al frente y a la izquierda.
     
     float distancias[3];           // Creo mi vector distancias de 3 componentes. Esta vacío, pero le "reservo" 3 lugares. En este vector voy a guarar los resultados de medir a mi derecha, al frente y a la izquierda.
 
     for(int i=0;i<3;i++){ // Defino las posiciones que voy a tener en cuenta- OJO - en Arduino la primera posicion es la posicion "0", que como definimos anteriormente son 45 grados. i sera como max 2 (2<3)
       
       ServoAzul.write(angulos[i]); // Voy indicandole a Arduino en que posiciones quiero mirar, angulo[0], angulo[1] y angulo[2]
       delay(100); //le doy tiempo a que gire
       distancias[i] = medir(); //cuando estpoy en la posicion i, llamo a medir() para que me devuelva la distancia y me la guarde en esa posicion del vector. Repite para cada i.
    
     }

     ServoAzul.write(90); // Para que quede mirando al frente mientras sigue su camino..

     if (distancias[0]>=distancias[2]) {  // Pregunto si hay mas lugar a la derecha que a la izquierda. ESTE IF REPRESENTA UNA CLARA DECICION INTELIGENTE Y AUTONOMA DEL ROBOT.
       
       derecha();   // Si es verdad que hay mas lugar giro a la derecha para tomar ese camino.
       
       delay(300);  // Le digo que gire durante 300 ms, este tiempo fue medido en nuestro robot para que gire 90 grados, Usted midalo en el suyo para encontrar el tiempo de giro necesario o deseado
                          
       parar();
    
     } 
     else {              // En este caso hay mas lugar a la izquierda.
       izquierda();
       delay(300);
       parar(); 
    
     } 
          
 } 
 else {       // Si nada de esto pasa (no encontro un obtaculo a menos de 20cm)-
  adelante(); // El robot sigue camien hacia adelante hasta encontrar un obstaculo.
  delay(50);  // El delay es poco para que mida constantemente. Si es muy grande, recien cuando el delay finalice, voy a volver a preguntar por las condiciones
 }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------DECLARACION DE FUNCIONES PERSONALIZADAS---------------------------------------------------------

void adelante(){
  Serial.println("ADELANTE");
  digitalWrite(IR,LOW); //ADELANTE
  digitalWrite(IA,HIGH);
  analogWrite(PWM_I,255);
  
  digitalWrite(DR,LOW);
  digitalWrite(DA,HIGH);
  analogWrite(PWM_D,255);
               
 }   

void atras(){
  Serial.println("ATRAS");
  digitalWrite(IR,HIGH); //ATRAS
  digitalWrite(IA,LOW);
  analogWrite(PWM_I,255);
  
  digitalWrite(DR,HIGH);
  digitalWrite(DA,LOW);
  analogWrite(PWM_D,255);

}
               
void parar(){
  Serial.println("PARAR");
  analogWrite(IR,LOW); 
  analogWrite(IA,LOW);
  
  analogWrite(DR,LOW);
  analogWrite(DA,LOW); 
}  
               

void derecha(){
  Serial.println("DERECHA");
  analogWrite(PWM_I,255);
  digitalWrite(IA,HIGH);
  digitalWrite(IR,LOW);
  
  analogWrite(PWM_D,255);
  digitalWrite(DR,HIGH);
  digitalWrite(DA,LOW);

}


void izquierda(){
  Serial.println("IZQUIERDA");  
  analogWrite(PWM_I,255);
  digitalWrite(IA,LOW);
  digitalWrite(IR,HIGH);
  
  analogWrite(PWM_D,255);
  digitalWrite(DR,LOW);
  digitalWrite(DA,HIGH);

}
              




float medir() {                     // Este es el codigo tipico para medir distancia con el Hc-04, para modificarlo se necesita cierta practica y tiempo de estudio, por ahora lo utilizaremos asi como esta.
  
  digitalWrite(pinUSTrig, HIGH);    // Prendo el tono de ultra sonido.
  delayMicroseconds(10);            // Espero 10 us.
  digitalWrite(pinUSTrig, LOW);     // Apago el tono de ultra sonido.

  long tiempo;                      // Declaro la variable tiempo, donde se guarda el tiempo entre que el tono va y vuelve.
  tiempo = pulseIn(pinUSEcho, HIGH)/2;  // La funcion pulseIn me devuelve el tiempo en us. el cual lo divido por 2 dado que el sonido recorrio la distancia ida y vuelta.

  return 0.0343*tiempo;             // la funcion medir() devuelve la distancia en cm la cual es el tiempo por la constante 0.0343 que es la velocidad del sonido en cm/us.
}
