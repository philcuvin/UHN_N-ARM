// Libraries for flow and CO2 sensors 
#include <SFM3X00.h>
#include <SFM4X00.h>
#include <Wire.h>
#include <Adafruit_SCD30.h>

// linear regulator variable 
int voltagesensorvalue = analogRead(A1);
int CO2_alarm = 0;
int Flow_alarm = 0;

// delay between readings
#define SAMPLE_DELAY   1550

// address of sensor
// usually 64 or 0x40 by default
#define FLOW_SENSOR_ADDRESS 0x40

// create insance of flow sensor with address 
SFM3X00 flowSensor(FLOW_SENSOR_ADDRESS);

// create insance of CO2 sensor with address 
Adafruit_SCD30  scd30;

void setup()
{ // LED Pins 
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(A2, OUTPUT);

  // establish serial communication
  Wire.begin();
  Serial.begin(115200);

  //  CO2 sensor initialization 
  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens
  Serial.println("Adafruit SCD30 test!");
  
  if (!scd30.begin()) {
    Serial.println("Failed to find SCD30 chip");
    while (1) { delay(10); }
  }
  Serial.println("SCD30 Found!");

  Serial.print("Measurement Interval: "); 
  Serial.print(scd30.getMeasurementInterval()); 
  Serial.println(" seconds");

  // print various sensor values
  Serial.println();
  Serial.print("sensor serial number: ");
  Serial.println(flowSensor.serialNumber, HEX);
  Serial.print("sensor article number: ");
  Serial.println(flowSensor.articleNumber, HEX);
  Serial.println();
  Serial.print("read scale factor: ");
  Serial.println(flowSensor.flowScale);
  Serial.print("read flow offset: ");
  Serial.println(flowSensor.flowOffset);
  
  // display the sample rate in hertz
  Serial.print("sample rate: ");
  Serial.print(1000.0 / (float)SAMPLE_DELAY);
  Serial.println(" Hz");
  Serial.println();

 // initialize sensor values and start measuring flow
  flowSensor.begin();
  delay(200);
}


void loop()
{
  // fan speed control
  analogWrite(6, 255);

  // read input voltage from voltage sensor
  
  voltagesensorvalue = analogRead(A1); 
  Serial.println("Voltage Sensor Reading");
  Serial.println(voltagesensorvalue);
  Serial.println(voltagesensorvalue*(5/1024));

  if (voltagesensorvalue < 972){
    Serial.println("Low Voltage");
    digitalWrite(4, HIGH);
  }

  else{
    Serial.println("High Voltage");
    digitalWrite(4, LOW);
  }

  // read CO2 reading from CO2 sensor and print 
 
  if (scd30.dataReady()){
    Serial.println("Data available!");

    if (!scd30.read()){ Serial.println("Error reading sensor data"); return; }

    Serial.print("Temperature: ");
    Serial.print(scd30.temperature);
    Serial.println(" degrees C");
    
    Serial.print("Relative Humidity: ");
    Serial.print(scd30.relative_humidity);
    Serial.println(" %");
    
    Serial.print("CO2: ");
    Serial.print(scd30.CO2, 3);
    Serial.println(" ppm");
    Serial.println("");
      if (scd30.CO2 > 1400){
        Serial.println("alarm: too high");
        digitalWrite(3, HIGH);
      }
      else{
        digitalWrite(3, LOW);
      }
  } else {
    Serial.println("No data");
  }

  delay(100);

  // read flow from sensor and print
  float flow = flowSensor.readFlow();

  if(flowSensor.checkRange(flow))
  {
    Serial.print("flow exceeded sensor limits:  ");
    Serial.print(flow);
    Serial.println(" slm");
  }
  else
  {
    Serial.print("flow : ");
    Serial.print(flow);
    Serial.println(" slm");
    if (abs(flow) < 4.00){
      digitalWrite(2, HIGH);
      Serial.println("Alarm: Flow too low");
      delay(100);
    }
    else{
      digitalWrite(2, LOW);
      delay(100);
    }
  }


  delay(SAMPLE_DELAY);
}
