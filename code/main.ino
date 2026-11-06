
const int trigPin = 9;
const int echoPin = 10;
const int buttonPin = 2;

// Светодиоды расстояния
const int redLED = 13;
const int yellowLED = 12;
const int greenLED = 11;

// Имитация двигателя
const int motorForward = 5; // IN1
const int motorReverse = 6; // IN2
const int enablePin = 3;    // ENA PWM

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

  // Проверка кнопки
  if (digitalRead(buttonPin) == HIGH) {
    stopMotor();
    allLEDsOff();
    delay(50);
    return;
  }

  // Измерение расстояния
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000UL);

  // Ошибка датчика
  if (duration == 0) {
    stopMotor();
    allLEDsOff();
    Serial.println("ERROR: Echo timeout");
    delay(100);
    return;
  }

  int distance = duration * 0.0343 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  allLEDsOff();

  // Меньше 20 см
  if (distance < 20) {
    stopMotor();
    digitalWrite(redLED, HIGH);
  }

  // От 20 до 50 см включительно
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
