#include <Wire.h>
#include <LiquidCrystal_I2C.h> // si da error descargar e instalar manualmente de https://github.com/johnrickman/LiquidCrystal_I2C?tab=readme-ov-file

// Configuración LCD I2C (dirección común 0x27 para 16x2)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pines para el motor DC (cinta transportadora) conectados al Puente L298, ver la posibilidad de usar un capacitor para estabilizar el voltaje
const int ENA_motor = 10;
const int IN1_motor = 8;
const int IN2_motor = 9;

// Pines para la bomba de agua  conectados al Puente L298, ver la posibilidad de usar un capacitor para estabilizar el voltaje
const int ENA_bomba = 6;
const int IN1_bomba = 5;
const int IN2_bomba = 4;

// Sensores HW201
const int sensor1 = 2;
const int sensor2 = 3;

// LEDs y pulsador
const int ledRojo = 11;    // Bomba activa
const int ledVerde = 12;   // Cinta activa
const int pulsador = 7;    // Pausa/inicio

// Variables de estado
bool sistemaActivo = true;
int contadorBomba = 0;
unsigned long ultimoCambio = 0;
const int debounceDelay = 200;

void setup() {
  // Configurar pines de control
  pinMode(ENA_motor, OUTPUT);
  pinMode(IN1_motor, OUTPUT);
  pinMode(IN2_motor, OUTPUT);
  pinMode(ENA_bomba, OUTPUT);
  pinMode(IN1_bomba, OUTPUT);
  pinMode(IN2_bomba, OUTPUT);
  
  // Configurar LEDs y pulsador
  pinMode(ledRojo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(pulsador, INPUT_PULLUP);
  
  // Configurar sensores
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  
  // Iniciar LCD
  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);
  lcd.print("Sistema Listo");
  
  // Iniciar sistema
  activarMotorDC();//
  actualizarLCD();
}

void loop() {
  // Manejo del pulsador con antirrebote
  if (digitalRead(pulsador) == LOW && (millis() - ultimoCambio) > debounceDelay) {
    sistemaActivo = !sistemaActivo;// el signo de admiración invertido antecediendo a una función, significa que la está negando
    ultimoCambio = millis();
    
    if (sistemaActivo) {
      activarMotorDC();
    } else {
      detenerMotorDC();
      detenerBombaAgua();
    }
    actualizarLCD();
  }

  if (sistemaActivo) {
    // Leer sensores
    bool objetoDetectado1 = !digitalRead(sensor1);
    bool objetoDetectado2 = !digitalRead(sensor2);
    
    // Lógica principal
    if (objetoDetectado1 && !objetoDetectado2) {
      detenerMotorDC();
      delay(3000);
      
      activarBombaAgua();
      contadorBomba++;
      delay(4000);
      
      detenerBombaAgua();
      delay(2000);
      
      activarMotorDC();
      actualizarLCD();
    }
    
    if (objetoDetectado2) {
      detenerMotorDC();
      delay(1000);
      activarMotorDC();
      actualizarLCD();
    }
  }
}

// Funciones de control
void activarMotorDC() {
  digitalWrite(IN1_motor, HIGH);
  digitalWrite(IN2_motor, LOW);
  analogWrite(ENA_motor, 255);
  digitalWrite(ledVerde, HIGH);
}

void detenerMotorDC() {
  digitalWrite(IN1_motor, LOW);
  digitalWrite(IN2_motor, LOW);
  analogWrite(ENA_motor, 0);
  digitalWrite(ledVerde, LOW);
}

void activarBombaAgua() {
  digitalWrite(IN1_bomba, HIGH);
  digitalWrite(IN2_bomba, LOW);
  analogWrite(ENA_bomba, 255);
  digitalWrite(ledRojo, HIGH);
}

void detenerBombaAgua() {
  digitalWrite(IN1_bomba, LOW);
  digitalWrite(IN2_bomba, LOW);
  analogWrite(ENA_bomba, 0);
  digitalWrite(ledRojo, LOW);
}

void actualizarLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Cinta: ");
  lcd.print(digitalRead(ledVerde) ? "ENCENDIDA " : "APAGADA");
  lcd.print(" Bomba: ");
  lcd.print(digitalRead(ledRojo) ? "ENCENDIDA " : "APAGADA");
  
  lcd.setCursor(0, 1);
  lcd.print("Ciclos: ");
  lcd.print(contadorBomba);
}
