// https://geegrow.ru/wiki/products/controls/encoder/

#include <Servo.h>

#define ENC_A_PIN 2 // Канал А энкодера
#define ENC_B_PIN 3 // Канал В энкодера

#define MIN_PULSE_VAL 500 // Минимальное значение диапазона импульса
#define MAX_PULSE_VAL 2500 // Максимальное значение диапазона импульса
#define START_PULSE_VAL 1500 // Значение импульса при старте
#define PULSE_STEP 5 // Шаг изменения импульса при прокрутке энкодера

#define SERVO_PIN 10 // Пин сервомотора

volatile int aPresState = -1; // Предыдущее состояние канала А
volatile int value = 120; // Текущее значение ШИМ

Servo servoMot; // Инициализация объектов моторов

int pulse = START_PULSE_VAL; // Переменная для значения импульса

void setup() {
  //Открытие соединения на скорости
  Serial.begin(115200);
  Serial.setTimeout(10);
  while(!Serial); // Ждём открытия Serial
  Serial.println();
  servoMot.attach(SERVO_PIN, MIN_PULSE_VAL, MAX_PULSE_VAL); // ПОдключение серво
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
  if (aPresState > -1 && aPresState != aCurrentState && aCurrentState == LOW) {
    //Проверяем состояние порта В, чтобы определить направление вращения
    if (digitalRead(ENC_B_PIN) == HIGH) {
      //Сюда попадаем если по часовой стрелке
      if (pulse < MAX_PULSE_VAL) { // Проверяем значение на максимальное
        pulse += PULSE_STEP;
      }
    } else {
      //Сюда попадаем если против часовой стрелки
      if (pulse > MIN_PULSE_VAL) { // Проверяем значение на минимальное
        pulse -= PULSE_STEP;
      }
    }
    Serial.println(pulse);
  }
  servoMot.writeMicroseconds(pulse); // Установить сервомотору шим сигнал
  aPresState = aCurrentState; //Сохраняем текущее значение порта А
}
