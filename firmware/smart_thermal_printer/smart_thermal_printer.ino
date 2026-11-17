#include <SoftwareSerial.h>
#define Encoder_output_A 2  
#define Encoder_output_B 3  
#define paperMotor 4       
#define paperSensor 5      
#define paperEndSensor 6   
#define thermalSensor 7    
#define alignmentSensor 8  
#define coverSensor 9      
#define resetButton 10    
SoftwareSerial printerSerial(11, 12);

float paperPosition = 0.0;  
int pulseCount = 0;        
unsigned long previousTime = 0; 
float wheelDiameter = 2.5;  
int pulsesPerRevolution = 240; 
float distancePerPulse;       
long totalPulse = 0;         
bool printCompleted = false;  
bool motorStopped = false;  

const float paperLength = 10.0;  
int requiredPulses;  

void setup() {
  Serial.begin(9600);
  printerSerial.begin(9600);

  pinMode(Encoder_output_A, INPUT);
  pinMode(Encoder_output_B, INPUT);
  pinMode(paperMotor, OUTPUT);
  pinMode(paperSensor, INPUT_PULLUP);
  pinMode(paperEndSensor, INPUT_PULLUP);
  pinMode(alignmentSensor, INPUT_PULLUP);
  pinMode(coverSensor, INPUT_PULLUP);
  pinMode(thermalSensor, INPUT);
  pinMode(resetButton, INPUT);  

  attachInterrupt(digitalPinToInterrupt(Encoder_output_A), paperEncoderFeedback, RISING);
  
  distancePerPulse = (PI * wheelDiameter) / pulsesPerRevolution;
  requiredPulses = paperLength / distancePerPulse;  
}

void loop() {
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - previousTime;

  if (digitalRead(resetButton) == HIGH) {  
    Serial.println(" Reset button pressed! Restarting system...");
    resetSystem();
    delay(500);
    digitalWrite(paperMotor, HIGH); 

  }
 
  if (!motorStopped && elapsedTime >= 50) {  
    long pulseCountCopy = pulseCount;  
    totalPulse += pulseCount;  
    pulseCount = 0;  
    interrupts();  

    float revolutions = (float)pulseCountCopy / pulsesPerRevolution;
    float movement = revolutions * (PI * wheelDiameter); 
    paperPosition += movement;

    Serial.print("Pulses: "); Serial.print(totalPulse);
    Serial.print(" | Paper Position (cm): "); Serial.println(paperPosition);

    previousTime = currentTime;  
  }

  if (!motorStopped && totalPulse >= requiredPulses) {
    Serial.println(" Stopping motor...");
    digitalWrite(paperMotor, LOW);  
    Serial.println(" Motor stopped.");
    
    detachInterrupt(digitalPinToInterrupt(Encoder_output_A));  
    motorStopped = true;  

    delay(1000);  
    printCompleted = true;  
    printReceipt(); 
    digitalWrite(paperMotor, HIGH); 
  }

}

void printReceipt() {
  Serial.println(" Printing receipt...");

  float temperature = 50.0; 
  Serial.println("Temperature fixed at 50°C. Proceeding to print...");

  if (digitalRead(coverSensor) == HIGH) {
    Serial.println("⚠ Simulated: Printer cover closed.");
  }

  printerSerial.println("--------------------------------");
  printerSerial.println("     Smart Tech Store     ");
  printerSerial.println("--------------------------------");
  printerSerial.println("Date: 01-02-2025  Time: 12:30 PM");
  printerSerial.println("--------------------------------");
  printerSerial.println("Item                 Price");
  printerSerial.println("--------------------------------");
  printerSerial.println("Smartphone           $250.00");
  printerSerial.println("Bluetooth Earphones  $30.00");
  printerSerial.println("Fast Charger         $15.00");
  printerSerial.println("--------------------------------");
  printerSerial.println("Total:               $295.00");
  printerSerial.println("--------------------------------");
  printerSerial.println("   Thank you for shopping with us!");
  printerSerial.println("   Visit our website: www.smarttech.com");
  printerSerial.println("--------------------------------");
  printerSerial.println("\n[ QR Code Printed Here ]");
  printerSerial.println(" Scan the code to verify receipt ");
  printerSerial.println("--------------------------------");

  Serial.println("--------------------------------");
  Serial.println("     Smart Tech Store     ");
  Serial.println("--------------------------------");
  Serial.println("Date: 01-02-2025  Time: 12:30 PM");
  Serial.println("--------------------------------");
  Serial.println("Item                 Price");
  Serial.println("--------------------------------");
  Serial.println("Smartphone           $250.00");
  Serial.println("Bluetooth Earphones  $30.00");
  Serial.println("Fast Charger         $15.00");
  Serial.println("--------------------------------");
  Serial.println("Total:               $295.00");
  Serial.println("--------------------------------");
  Serial.println("   Thank you for shopping with us!");
  Serial.println("   Visit our website: www.smarttech.com");
  Serial.println("--------------------------------");
  Serial.println("\n[ QR Code Printed Here ]");
  Serial.println(" Scan the code to verify receipt ");
  Serial.println("--------------------------------");
  Serial.println(" Receipt printed successfully.");
}

void resetSystem() {
  Serial.println(" System resetting...");
  totalPulse = 0;
  pulseCount = 0;
  paperPosition = 0.0;
  printCompleted = false;
  motorStopped = false;

  digitalWrite(paperMotor, HIGH);  

  attachInterrupt(digitalPinToInterrupt(Encoder_output_A), paperEncoderFeedback, RISING);
  Serial.println("System reset complete. Ready for new print.");
}

void paperEncoderFeedback() {
  int b = digitalRead(Encoder_output_B);
  if (b > 0) {
    pulseCount++;
  } else {
    pulseCount--;
  }
  if (pulseCount < 0) {
    pulseCount = 0;
  }
}
