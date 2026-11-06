
const int trigPin = 9;
const int echoPin = 10;
const int buttonPin = 2;

// Светодиоды расстояния
const int redLED = 13;
const int yellowLED = 12;
const int greenLED = 11;

// Имитация двигателя
const int motorForward = 5;
const int motorReverse = 6;
const int enablePin = 3;

// Защита кнопки от дребезга
int lastButtonReading = HIGH;
int stableButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

// Допустимый диапазон HC-SR04
const int minDistance = 2;
const int maxDistance = 400;

void stopMotor() {
  analogWrite(enablePin, 0);
  digitalWrite(motorForward, LOW);
  digitalWrite(motorReverse, LOW);
}

void allLEDsOff() {
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, LOW);
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  pinMode(motorForward, OUTPUT);
  pinMode(motorReverse, OUTPUT);
  pinMode(enablePin, OUTPUT);

  Serial.begin(9600);

  stopMotor();
  allLEDsOff();
}

void loop() {
  // Проверка кнопки с debounce
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonReading) {
    lastDebounceTime = millis();
  }

  if (millis() - lastDebounceTime >= debounceDelay) {
    stableButtonState = reading;
  }

  lastButtonReading = reading;

  // Кнопка отпущена
  if (stableButtonState == HIGH) {
    stopMotor();
    allLEDsOff();
    delay(10);
    return;
  }

  // Измерение расстояния
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000UL);

  // Проверка отсутствия Echo
  if (duration == 0) {
    stopMotor();
    allLEDsOff();
    Serial.println("ERROR: Echo timeout");
    delay(100);
    return;
  }

  int distance = duration * 0.0343 / 2;

  // Проверка допустимого диапазона
  if (distance < minDistance || distance > maxDistance) {
    stopMotor();
    allLEDsOff();
    Serial.println("ERROR: Invalid distance");
    delay(100);
    return;
  }

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  allLEDsOff();

  // Меньше 20 см
  if (distance < 20) {
    stopMotor();
    digitalWrite(redLED, HIGH);
  }

  // От 20 до 50 см
  else if (distance <= 50) {
    digitalWrite(yellowLED, HIGH);

    digitalWrite(motorForward, HIGH);
    digitalWrite(motorReverse, LOW);
    analogWrite(enablePin, 120);
  }

  // Больше 50 см
  else {
    digitalWrite(greenLED, HIGH);

    digitalWrite(motorForward, HIGH);
    digitalWrite(motorReverse, LOW);
    analogWrite(enablePin, 255);
  }

  delay(100);
}
