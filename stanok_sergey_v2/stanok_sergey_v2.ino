#include "Arduino.h"

int relay1_Pin = 7;
int relay2_Pin = 6;
int relay3_Pin = 5;
int relay4_Pin = 4;
int relay5_Pin = 3;
int relay6_Pin = 8;

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

class Element
{
public:
	Element();
	Element(int Pin, bool modePin, bool modeLevel)
	{
		_pin = Pin;
		_pinMode = modePin;
		_levelMode = modeLevel;
		if (_pinMode)
		{
			pinMode(_pin, OUTPUT);
		}
		else pinMode(_pin, LOW);
		if (modeLevel)
		{
			digitalWrite(_pin, HIGH);
		}
		else digitalWrite(_pin, LOW);
	}

	int timeDelay(int time);
private:

	int _pin;
	bool _pinMode;
	bool _levelMode;

};

Element::Element()
{
}
int Element::timeDelay(int time)
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

class Relay :public Element
{
public:
	Relay(int Pin, bool modeLevel)
	{
		_pin = Pin;
		_levelMode = modeLevel;
		pinMode(_pin, OUTPUT);
		if (modeLevel)
		{
			digitalWrite(_pin, HIGH);
		}
		else digitalWrite(_pin, LOW);
	}
	void On();
	void Off();
	void OnByTime(int delayTimeSec);
	static bool stopFlag;
private:
	int _pin;
	bool _pinMode;
	bool _levelMode;
};

void Relay::Off()
{
	if (_levelMode)
	{
		digitalWrite(_pin, LOW);
	} else digitalWrite(_pin, HIGH);
	
}
void Relay::On()
{
	if (_levelMode)
	{
		digitalWrite(_pin, HIGH);
	} else digitalWrite(_pin, LOW);
	
	
}

void Relay::OnByTime(int delayTimeSec)
{
	Relay::On();
	timeDelay(delayTimeSec);
	Relay::Off();
}

class Sensor : public Element
{
public:
	Sensor(int pin, int modeLevel)
	{
		_pin = pin;
		pinMode(pin, INPUT);
		if (modeLevel)
		{
			digitalWrite(pin, HIGH);
		}else digitalWrite(pin, LOW);
		
	}
	int sensorCheckNow();
	void sensorCount(int countCheck);
	int _status = digitalRead(_pin);

private:

	int _pin;
	friend Relay;
};

int Sensor::sensorCheckNow(void)
{
	int _status = digitalRead(_pin);
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
//Функция кнопки стопа
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
//Функция кнопки старта
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
	
	// Relay Name(pin, LOW=0(HIGH=1))
	Relay	Rel1(relay1_Pin, 0), //реле1 - Крутим
			Rel2(relay2_Pin, 0), //реле2 - Подводим сварку
			Rel3(relay3_Pin, 0), //Реле3 -Варим
			Rel4(relay4_Pin, 0); //Реле4 - убираем сварку
	//Сенсоры+порты
	Sensor	Sens1(sensor1_Pin, 0), //сенсор1 - обороты заготовки
			Sens2(sensor2_Pin, 0), //сенсор2 конечник поднятия сварки
			Sens3(sensor3_Pin, 0), //сенсор3 конечник на опускание сварки
			Sens4(sensor4_Pin, 0); //Сенсор4 конечник шва
	//Логика: 
	Rel1.On();
	Sens1.sensorCount(3);
	Rel2.On();
	Sens2.sensorCount(1);
	Rel2.Off();
	Sens1.sensorCount(3);
	Rel1.Off();
	while (digitalRead(sensor1_Pin) == 0)
	{
		//Rel3.TimeOreSensor(5, 1);		заменить новой функцией с прерыванием 
		//Rel1.TimeOreSensor(10, 1);	заменить новой функцией с прерыванием 
	}
		//Rel4.TimeOreSensor(60, 3); заменить новой функцией с прерыванием
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
	programm(); // для тестов
	/*
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
	*/
}


