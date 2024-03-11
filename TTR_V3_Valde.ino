//* Ping Pong Robot Inicio de codigo: version 3- Mayo-9-2023 - ALERTA: esta version no esta trabajando- se intento poner a trabajar los sliders- se usa la version pin_pong robot 3
// * by hernando Bolaños
// *se emplea rele par un motoreductor del alimentador de bolas - 30 rpm - variacion de velocidad con plantilla de acceso bolas
// * 01-08-2022- servo variacion update
//* Pruebas comunicaciones con APP
//*15-08-2022 -primeras pruebas de sliders- no operan correctamente con parseint() -debido a que estamos trabajando con multitasking-sin resolver a abril -25-2023
//*29-08-2022 - Settings-segun pruebas - revisar problema de entrada en hibernacion del BTh - sin reoslver a abril-25-2023
// 05-mayo-2023 - nuevo intento de leer los slider de la APP como cadenas de caracteres es muy importante poder cambiar velocidades para darle mas opciones al juego
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
////WARNING: DRAFT CODE////The robot must need adult supervision if there are kids playing with it
/////////////////////////////////////////////////////////////////////////////////////////////////////
//-- Zona de librerías --
// Comunicación serie por software
#include <SoftwareSerial.h>
//-- Zona de definiciones --
#define LED
#define RX 10
#define TX 11
const int pinENA = 9; // Motor UP PWM
const int pinIN1 = 4;// motor UP
const int pinIN2 = 5; // Motor UP
const int pinENB = 3;// Motor Down PWM
const int pinIN3 = 6;//Motor Down
const int pinIN4 = 7;//Motor Down
const int speed = 250; //velocidad de giro 80% (200 de 255)
const int speedoff = 0; //velocidad de giro 0% (0 de 255)
long unsigned valor=0;
//-- Zona de variables globales
SoftwareSerial BT1(RX,TX);

#define ENAB 12 // pIN ENAB para motoreductor de alimeantador de bolas

#include <Servo.h>
Servo myservo; // create servo object to control a servo
int servo_angle = 90;//90
int servo_angle1 = 60;// valor para quedar en posicion central fija
int variacion = 10;/// con esta variable se puede hacer mas lenta o mas rapida la osiclacion
unsigned long startTime_servo = 0;
unsigned long interval_servo = 1100;//t1100
int latchball = 0;
int latchservo = 0;
int latchup = 0;
int latchdown =0;
int latchservoizq =0;
int latchservocen =0;
int latchservoder =0;
int latchservorandom =0;
String dataIn = "";
//int speedup=180;
//int speeddown=180;
String recibido,dataIns;
int s3Act;
int s4Act;
int speedup=180;
int speeddown=180;

void setup()
{

pinMode(ENAB, OUTPUT); // pIN 12 como salida para rele motorreductor
pinMode(pinIN1, OUTPUT);
pinMode(pinIN2, OUTPUT);
pinMode(pinENA, OUTPUT);//PWM usado para velocidad de motor 1 del lanzador
pinMode(pinIN3, OUTPUT);
pinMode(pinIN4, OUTPUT);
pinMode(pinENB, OUTPUT);//PWM uasado para veloidad de motor 2 del lanzador
digitalWrite(ENAB, HIGH);
myservo.attach(6,510,1200); // ancho minimo y max exclusivo para MG996R -base aplicado a este robt attaches the servo on pin 6 to the servo object
Serial.begin(9600);

myservo.write(90); // usado al inicio para verificar si el servo esta activo
myservo.write (0); // ir a posicion de inicio en el centro
myservo.write(60); // usado al inicio para verificar si el servo esta activo

BT1.begin(9600);
BT1.setTimeout(10);
//char recibido=0;
int i=0;
}
void loop()
{
unsigned currentTime = millis();
//char recibido=0; se cambio por string
//int speedup = 0;
//int speeddown = 0;
//Si se ha recibido un dato
if(BT1.available()==true){
recibido=BT1.readString(); //Se carga en la variable recibido/se lle cadena
Serial.print(recibido);

if (recibido.startsWith ("s3")){ //compruba si la cadena empieza con s3
  dataIns= recibido.substring(2,recibido.length());//extraigo los cacteres de la posición 2 en adelante
s3Act=dataIns.toInt(); //convierto de strin a entero
speedup=s3Act;
}


if (recibido.startsWith ("s4")){ //compruba si la cadena empieza con s3
  dataIns= recibido.substring(2,recibido.length());//extraigo los cacteres de la posición 2 en adelante
s3Act=dataIns.toInt(); //convierto de strin a entero
speeddown=s4Act;
}

Serial.print(speedup);
Serial.print(speeddown);
}
//
if (recibido.startsWith("A")){
int latchrobot = 1;
Serial.print("Robot Activado");
}
if (recibido.startsWith("B")){
int latchrobot = 0;
Serial.println("Robot Desactivado");
latchball = 0; //desenganchar
latchservo =0; //desenganchar
latchup=0;//desenganchar
latchdown=0;//desenganchar
latchservoizq=0;//desenganchar
latchservoder=0;//desenganchar
latchservocen=0;//desengachar
digitalWrite(ENAB, HIGH); //aplicar Rele motoreductor off
}
if (recibido.startsWith("C")){
latchball= 1;}
if (recibido.startsWith ("D")){
latchball= 0;}
if (recibido.startsWith ("E")){
latchup= 1;}
if (recibido.startsWith("F")){
latchup= 0;}
if (recibido.startsWith("G")){
latchdown= 1;}
if (recibido.startsWith ("H")){
latchdown= 0;}
if (recibido.startsWith ("I")){
latchservo = 1;}
if (recibido.startsWith("J")){
latchservo = 0;}
if (recibido.startsWith ("Z")){
latchservoizq = 1;}
if (recibido.startsWith("W")){
latchservoizq = 0;}
if (recibido.startsWith("T")){
latchservocen = 1;}
if (recibido.startsWith("X")){
latchservocen = 0;}
if (recibido.startsWith ("R")){
latchservoder = 1;}
if (recibido.startsWith ("Y")){
latchservoder = 0;}
if (recibido.startsWith ("M")){
latchservorandom = 1;}
if (recibido.startsWith("N")){
latchservorandom = 0;}
//if (((valor >=20) and (valor <=25)) and(( latchup=1) and (latchdown =1) )){ const int speed=valor; Serial.print(speed);} // tomando trama de los slider up y down
if (latchball == 1){
Serial.print("Dispensar Bolas activado");
alimbolas();
BT1.write('C');
}
if (latchball == 0){
alimbolasoff();
Serial.println("parar Dispensar Bolas activado");
BT1.write('D');
}
if (latchservo == 1){
Serial.print("Oscilar activado");
oscilar();
//BT1.write('I');}
}
if (latchservo == 0){
Serial.println("Oscilar desactivado");
//BT1.write('J');
}
if (latchup == 1){
uprob();
}
if (latchup == 0){
upoff();
Serial. println("Up desactivado");
BT1.write(' ');
}
if (latchdown == 1){
down();
}
if (latchdown == 0){
downoff();
Serial. println("Up desactivado");
//BTI.write(' ');
}
if (latchservoizq== 1){
//
int i=0;
for (int i = 0; i <= 1; i++) {
oscizq();
}
if (i>=1){
latchservoizq == 0;}
//Serial.println("Fijo en izquierda Activado");
//BTI.write(' ');
}
if (latchservoizq == 0){
//oscizqoff();
//Serial. println(" Fijo en ziquiera desActivado")
//BTI.write(' ');
}
if (latchservocen== 1){
int i=0;
for (int i = 0; i <= 1; i++) {
osccentro();
}
if (i>=1){
latchservocen == 0;}
//Serial. println("Fijo en centro Activado");
//BTI.write(' ');
}
if (latchservocen == 0){
//osccentrooff();
//Serial. println(" Fijo en centro desaActivado)
//BTI.write(' ');
}
if (latchservoder== 1){
int i=0;
for (int i = 0; i <= 1; i++) {
oscder();
}
if (i>=1){
latchservoder == 0;}
//Serial.println("Fijo en centro Activado");
//BTI.write(' ');
}
if (latchservoder == 0){
//oscderoff();
//Serial. println(" Fijo en centro desaActivado)
//BTI.write(' ');
}
}
void alimbolas()// alimenta bolas giando en sentido NCW
{
Serial.print("Motor Bolas ON,");
digitalWrite(ENAB, LOW); // Modo Enable del Motorreductor
}


void alimbolasoff()// alimenta bolas giando en sentido NCW
{
Serial.print("Motor Bolas OFF,");
digitalWrite(ENAB, HIGH); // Modo Enable del Motorreductor
//delay(1000);
}


void oscilar() //activa modo oscilar en el robot - izquierda a derecha
{
unsigned currentTime = millis();
//Serial.println("servo");
if (currentTime - startTime_servo >= interval_servo){
startTime_servo=currentTime;
myservo.write(servo_angle);
servo_angle = servo_angle + variacion;
if (servo_angle == 150) servo_angle = 30;
//Serial.println(servo_angle);
}
}
void oscizq() //activa modo oscilar en el robot - izquierda a derecha
{
unsigned currentTime = millis();
//Serial.println("servo");
if (currentTime - startTime_servo >= interval_servo){
startTime_servo=currentTime;
myservo.write(160);
//Serial.println(servo_angle);
}
}
void oscder() //activa modo oscilar en el robot - izquierda a derecha
{
unsigned currentTime = millis();
//Serial.println("servo");
if (currentTime - startTime_servo >= interval_servo){
startTime_servo=currentTime;
myservo.write(0);
}
}
void osccentro() //activa modo oscilar en el robot - izquierda a derecha
{
unsigned currentTime = millis();
//Serial.println("servo");
if (currentTime - startTime_servo >= interval_servo){
startTime_servo=currentTime;
myservo.write(90);
}
}
void oscizqoff() //activa modo oscilar en el robot - izquierda a derecha
{
unsigned currentTime = millis();
//Serial.println("servo");
if (currentTime - startTime_servo >= interval_servo){
startTime_servo=currentTime;
myservo.write(90);
//Serial.println(servo_angle);
}
}
void oscderoff() //activa modo oscilar en el robot - izquierda a derecha
{
unsigned currentTime = millis();
//Serial.println("servo");
if (currentTime - startTime_servo >= interval_servo){
startTime_servo=currentTime;
myservo.write(90);
}
}
void osccentrooff() //activa modo oscilar en el robot - izquierda a derecha
{
unsigned currentTime = millis();
//Serial.println("servo");
if (currentTime - startTime_servo >= interval_servo){
startTime_servo=currentTime;
myservo.write(90);
}
}
void uprob() //
{
//Serial.println("corriendo rutina UP,");
// Serial.println(speedup);
digitalWrite(pinIN1, HIGH);
digitalWrite(pinIN2, LOW);
analogWrite(pinENA, speedup);

}
void upoff()
{ //Serial.println("corriendo rutina upoff");
digitalWrite(pinIN1, LOW);
digitalWrite(pinIN2, LOW);
analogWrite(pinENA, speedoff);

}
void down()
{
digitalWrite(pinIN3, LOW);
digitalWrite(pinIN4, HIGH);
analogWrite(pinENB, speeddown);

}
void downoff()
{
digitalWrite(pinIN3, LOW);
digitalWrite(pinIN4, LOW);
analogWrite(pinENB, speedoff);

}
