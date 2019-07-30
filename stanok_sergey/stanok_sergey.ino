#include "Arduino.h"

int relay1_Pin = 7;
int relay2_Pin = 6;
int relay3_Pin = 5;
int relay4_Pin = 4;
int relay5_Pin = 3;

//Датчики
int sensor1_Pin = A0;
int sensor2_Pin = 8;
int sensor3_Pin = 10;
int sensor4_Pin = 9;
int countSensor1 = 0;

// LED
int led_pin = 13;

//Кнопки

int stopButtonPin = 8;
bool stopButtonState = 0;
int startButtonPin = 2;
bool startButtonState = 0;

boolean butt_flag = 0;
boolean butt;
unsigned long last_press;



class Relay
{
public:
	void setPinRelay(int pin);
	void On();
	void Off();
	void OnAfterDelay(int delayTimeSec);
	void OffAfterDelay(int delayTimeSec);
	void OnByTime(int delayTimeSec);
	void TimeOreSensor(int delayTimeSec, int sensorForCheck);
	static bool stopFlag;
private:
	int _pin;
};

bool Relay::stopFlag = 0;
void Relay::setPinRelay(int pin)
{
	_pin = pin;
	pinMode(_pin, OUTPUT);
	digitalWrite(_pin, HIGH);
}
void Relay::Off()
{
	digitalWrite(_pin, HIGH);
}
void Relay::On()
{
	digitalWrite(_pin, LOW);
}

void Relay::OffAfterDelay(int delayTimeSec)
{
	unsigned long last_time;
	unsigned long check_time;
	last_time = millis();
	check_time = millis() + delayTimeSec * 1000;
	while (check_time - last_time > 0)
	{
		last_time = millis();
	}
	Relay::Off();


}
void Relay::OnAfterDelay(int delayTimeSec)
{
	unsigned long last_time;
	unsigned long check_time;
	last_time = millis();
	check_time = millis() + delayTimeSec * 1000;
	while (check_time - last_time > 0)
	{
		last_time = millis();
	}
	Relay::On();

}
void Relay::OnByTime(int delayTimeSec)
{
	Relay::On();
	unsigned long last_time;
	unsigned long check_time;
	last_time = millis();
	check_time = millis() + delayTimeSec * 1000;
	while (check_time - last_time > 0)
	{
		last_time = millis();
	}
	Relay::Off();


}
void Relay::TimeOreSensor(int delayTimeSec, int sensorForCheck)
{
	for (int i = 0; i < delayTimeSec * 10; i++)
	{

		int stateSensor;
		int x;
		switch (sensorForCheck)
		{
		case 1:
			x = sensor1_Pin;
			break;
		case 2:
			x = sensor2_Pin;
			break;
		case 3:
			x = sensor3_Pin;
			break;

		case 4:
			x = sensor4_Pin;
			break;

		}
		stateSensor = digitalRead(x);
		if (stateSensor == 0 && stopFlag == 0)
		{

			Relay::On();
			unsigned long last_time;
			unsigned long check_time;
			last_time = millis();
			check_time = millis() + 100;
			int temp = check_time - last_time;

			while (temp > 0)
			{
				temp = check_time - last_time;
				last_time = millis();

			}


		}
		else
		{
			Serial.print("stopFlag: ");
			Serial.print(stopFlag);
			Serial.print("stateSensor: ");
			Serial.print(stateSensor);
			stopFlag = 1;
			break;
		}

		Relay::Off();
	}



}


class Sensor
{
public:

	int sensorCheckNow();
	void sensorCount(int countCheck);
	void setSensorPin(int pin);
	int _status = digitalRead(_pin);

private:

	int _pin;
	friend Relay;
};
void Sensor::setSensorPin(int pin)
{
	_pin = pin;
	pinMode(pin, INPUT);
	digitalWrite(pin, HIGH);
}
int Sensor::sensorCheckNow(void)
{

	_status = digitalRead(_pin);
	return _status;

}
void Sensor::sensorCount(int countCheck)
{
	for (int i = 0; i < countCheck; )
	{
		Serial.println(i);
		if (digitalRead(_pin) == 1)
		{
			while (digitalRead(_pin) == 1)
			{

			}
			i++;
		}

	}

}


void setup() {
	Serial.begin(9600);
	pinMode(startButtonPin, INPUT_PULLUP);
	pinMode(led_pin, OUTPUT);
	digitalWrite(led_pin, HIGH);

}


//Функция паузы
int timeDelay(int time)
{
	unsigned long last_time;
	unsigned long check_time;
	last_time = millis();
	check_time = millis() + time;
	while (check_time - last_time > 0)
	{
		last_time = millis();
	}
	Serial.println(time);
	return int(time);
}
//Функция кнопки старта
int stopButton()
{

	butt = digitalRead(stopButtonPin); // считать текущее положение кнопки

	if (butt == 1 && butt_flag == 0 && millis() - last_press > 100) {
		butt_flag = 1;
		Serial.println("Stop Button pressed");

		last_press = millis();
		stopButtonState = 1;
	}
	if (butt == 0 && butt_flag == 1 && millis() - last_press > 100) {
		butt_flag = 0;
		Serial.println("Stop Button released");

	}

	return stopButtonPin;


}
int startButton()
{
	butt = digitalRead(2); // считать текущее положение кнопки

	if (butt == 1 && butt_flag == 0 && millis() - last_press > 100) {
		butt_flag = 1;
		Serial.println("Start Button pressed");

		last_press = millis();
	}
	if (butt == 0 && butt_flag == 1 && millis() - last_press > 100) {
		butt_flag = 0;
		Serial.println("Start Button released");
		startButtonState = 1;

	}

	return 0;

}

void programm()
{
	//Реле+порты

	Relay Rel1, Rel2, Rel3, Rel4, Rel5;
	//реле1 - Крутим
	//реле2 - Подводим сварку
	//Реле3 -Варим
	//Реле4 - убираем сварку


	Rel1.setPinRelay(relay1_Pin);
	Rel2.setPinRelay(relay2_Pin);
	Rel3.setPinRelay(relay3_Pin);
	Rel4.setPinRelay(relay4_Pin);
	Rel5.setPinRelay(relay5_Pin);

	//Сенсоры+порты
	//сенсор1 - обороты заготовки
	//сенсор2 конечник поднятия сварки
	//сенсор3 конечник на опускание сварки
	//Сенсор4 конечник шва
	Sensor Sens1, Sens2, Sens3, Sens4;
	Sens1.setSensorPin(sensor1_Pin);
	Sens2.setSensorPin(sensor2_Pin);
	Sens3.setSensorPin(sensor3_Pin);
	Sens4.setSensorPin(sensor4_Pin);

	Rel1.stopFlag = 0;


	Rel1.On();
	Sens1.sensorCount(3);
	Rel2.On();
	Sens2.sensorCount(1);
	Rel2.Off();
	Sens1.sensorCount(3);
	Rel1.Off();


	while (digitalRead(sensor1_Pin) == 0)
	{
		Rel3.TimeOreSensor(5, 1);
		Rel1.TimeOreSensor(10, 1);
	}
	Rel4.TimeOreSensor(60, 3);


}

void emergencyStop()
{

	digitalWrite(relay1_Pin, HIGH);
	digitalWrite(relay2_Pin, HIGH);
	digitalWrite(relay3_Pin, HIGH);
	digitalWrite(relay4_Pin, HIGH);
	digitalWrite(relay5_Pin, HIGH);
	Serial.println("EmergencyStopFunction");
}

void loop()
{


	startButton();

	//Пока не сработала кнопка стоп
	while (stopButtonState == 1);
	{
		//ждем нажатия кнопки старт
		while (startButtonState == 1)
		{
			digitalWrite(led_pin, HIGH);
			programm();
			Serial.print("stopFlag= ");
			Serial.println(Relay::stopFlag);
			startButtonState = 0;
			emergencyStop();

		}

		digitalWrite(led_pin, LOW);
	}



}
