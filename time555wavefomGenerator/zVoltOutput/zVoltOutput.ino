const int outputPin = 13;
double period = 4;

void setup() 
{
  Serial.begin(9600);
  pinMode(outputPin, OUTPUT);
}

void loop() 
{
  digitalWrite(outputPin, HIGH);
  Serial.print(HIGH);
  Serial.print("\n");
  delay(period);
  analogWrite(outputPin, LOW);
  Serial.print(LOW);
  Serial.print("\n");
  delay(period);
}
