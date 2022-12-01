// https://geegrow.ru/wiki/products/controls/encoder/

#include <Servo.h>

#define ENC_A_PIN 2 // Канал А энкодера
#define ENC_B_PIN 3 // Канал В энкодера

#define STOP_PULSE 1500 // Значение импульса остановки
#define MIN_PULSE_VAL 300 // Минимальное значение диапазона импульса
#define MAX_PULSE_VAL 2700 // Максимальное значение диапазона импульса

#define SERVO_PIN 10 // Пин серво мотора

volatile int aPresState = -1; // Предыдущее состояние канала А
volatile int value = 120; // Текущее значение ШИМ

Servo servoMot; // Инициализация объектов моторов

int pulse = STOP_PULSE; // Переменная для значения импульса

void setup() {
  //Открытие соединения на скорости
  Serial.begin(115200);
  Serial.setTimeout(10);
  while(!Serial); // Ждём открытия Serial
  Serial.println();
  servoMot.attach(SERVO_PIN); // ПОдключение серво
  //Инициализация входов
  pinMode(ENC_A_PIN, INPUT); // Подключение канала A энкодера
  pinMode(ENC_B_PIN, INPUT); // Подключение канала B энкодра
  Serial.println(pulse);
}
void loop() {
  if (Serial.available() > 2) {
    String inputStr = Serial.readStringUntil('\n');
    pulse = inputStr.toInt();
    Serial.println(pulse);
  }
  //Запоминаем текущее состояние канала А во временную переменную
  int aCurrentState = digitalRead(ENC_A_PIN);
  //Проверяем направление вращения
  if (aPresState >-1 && aPresState != aCurrentState && aCurrentState == LOW) {
    //Проверяем состояние порта В, чтобы определить направление вращения
    if (digitalRead(ENC_B_PIN) == HIGH) {
      //Сюда попадаем если по часовой стрелке
      if (pulse < MAX_PULSE_VAL) { // Проверяем значение на максимальное
        pulse++;
      }
    } else {
      //Сюда попадаем если против часовой стрелки
      if (pulse > MIN_PULSE_VAL) { // Проверяем значение на минимальное
        pulse--;
      }
    }
    Serial.println(pulse);
  }
  servoMot.writeMicroseconds(pulse); // Установить сервомотору шим сигнал
  //Сохраняем текущее значение порта А
  aPresState = aCurrentState;
}
