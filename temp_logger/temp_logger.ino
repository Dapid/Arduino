#include <SPI.h>
#include <SD.h>

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

const int chip_select = 10;
const auto filename = "datalog2.txt";
const auto separator = ", ";
const int STATUS_LED = 2;

void setup() {
  // Status LED:
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);

  // Monitoring LEDs
  for(int i = 4; i <= 7; i++){
      pinMode(i, OUTPUT);
  }
  
  //Initialising serial connection
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  #ifdef DEBUG
  Serial.print("Initializing SD card...");
  #endif
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chip_select)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  else{
    digitalWrite(STATUS_LED, HIGH);
  }

  #ifdef DEBUG
  Serial.println("card initialized.");
  #endif

  // Print header
  File dataFile = SD.open(filename, FILE_WRITE);
  String header = "# seconds, temperature, voltage, free_memory";
  dataFile.println(header);
  dataFile.close();

}


void write_to_sd(float temperature, float voltage){
  File dataFile = SD.open(filename, FILE_WRITE);
  if (dataFile){
    digitalWrite(STATUS_LED, HIGH);
  }
  else{
    digitalWrite(STATUS_LED, LOW);
  }

  unsigned long time_code = millis() / 1000;
  /*
  String dataString = "";
  
  dataString += String(time_code);
  dataString += F(", ");
  dataString += String(temperature);
  dataString += F(", ");
  dataString += String(voltage);
  dataString += F(", ");
  dataString += String(freeMemory());

  dataFile.println(dataString);
  dataFile.close();
  */

  dataFile.print(time_code);
  dataFile.print(separator);
  dataFile.print(temperature);
  dataFile.print(separator);
  dataFile.print(voltage);
  dataFile.print(separator);
  dataFile.print(freeMemory());
  dataFile.println();
  dataFile.close();
  
}

void write_to_led(float number){
  // Blue zone
  if (number < 2){
    digitalWrite(7, HIGH);
  }
  else{
    digitalWrite(7, LOW);
  }

  // Green zone
  if (number >= 1 && number < 11){
    digitalWrite(6, HIGH);
  }
  else{
    digitalWrite(6, LOW);
  }

  // Yellow zone
  if (number >= 8 && number < 20){
    digitalWrite(5, HIGH);
  }
  else{
    digitalWrite(5, LOW);
  }

  // Red zone
  if (number >= 15){
    digitalWrite(4, HIGH);
  }
  else{
    digitalWrite(4, LOW);
  }
}


void loop() {
  float temperature, voltage;
  double read_value;
  
  // Read temperature
  read_value = analogRead(0);
  voltage = 5. * read_value / 1023.;
  temperature = voltage * 100;

  #ifdef DEBUG
  Serial.print(temperature);
  Serial.print(" ");
  Serial.println(read_value);
  #endif
  
  // Save to SD
  write_to_sd(temperature, voltage);

  // Display
  write_to_led(temperature);

  delay(2000);
  //delay(20);
}
