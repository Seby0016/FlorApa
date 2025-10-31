  #include <Arduino.h>
  #include <DHT.h>

  const int sensorPin = 34;
  const int relayPin = 2;    

  #define DHTPIN 4          
  #define DHTTYPE DHT11      

  DHT dht(DHTPIN, DHTTYPE);

  int analogMin = 0;      
  int analogMax = 4095;      

  float humidityPercentage = 0.0;  
  float temperature = 0.0;        
  float humidityAir = 0.0;        
  void setup() {
    Serial.begin(115200);
    dht.begin();
    pinMode(sensorPin, INPUT);
    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, LOW); 

    Serial.println("Incepe calibrarea senzorului de umiditate sol...");
    delay(2000);

    Serial.println("Test pentru sol uscat...");
    delay(20000);  
    analogMin = analogRead(sensorPin);  
    Serial.print("Valoare sol uscat: ");
    Serial.println(analogMin);

    Serial.println("Test pentru sol complet umed...");
    delay(20000);  
    analogMax = analogRead(sensorPin);  
    Serial.print("Valoare sol umed: ");
    Serial.println(analogMax); 

    Serial.println("Calibrarea s-a încheiat!");
    delay(5000);
  }

  void loop() {
    int sensorValue = analogRead(sensorPin);

  /// analog min=3200;analog max 1900
    humidityPercentage = map(sensorValue, analogMin, analogMax, 0, 100);

 
    temperature = dht.readTemperature();  
    humidityAir = dht.readHumidity();     

    if (isnan(temperature) || isnan(humidityAir)) {
      Serial.println("Eroare la citirea senzorului DHT11");
    } else {
      Serial.print("Umiditate sol: ");
      Serial.print(humidityPercentage);
      Serial.println("%");
  //ORIENTATIV min max 3200 1900
      Serial.print("Temperatura aerului: ");
      Serial.print(temperature);
      Serial.println(" °C");

      Serial.print("Umiditate aer: ");
      Serial.print(humidityAir);
      Serial.println(" %");

    
      if ((humidityPercentage < 40 && temperature > 3) || (temperature < 0 && humidityPercentage < 20)){

        Serial.println("Condiții îndeplinite: Pornire pompa!");
        digitalWrite(relayPin, HIGH); 
       delay(4000);    /// circa 120ml de apa vor curge.
        Serial.println("Condiții neîndeplinite: Oprire pompa!");
        digitalWrite(relayPin, LOW); 
      }
    }

    delay(5000);
  }
