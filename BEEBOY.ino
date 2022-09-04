#include <SoftwareSerial.h>
// pitches.h 를 사용하기 위해서는 Arduino/libraries 폴더에 pitches 폴더를 만들고 음계를 나타내는 pitches.h 파일을 넣는다.(인터넷 다운)
#include "pitches.h"

#define PIEZ0_PIN 8

int RX = 3;
int TX = 2;
SoftwareSerial mySerial(RX, TX);
int dir_l = 4;
int dir_r = 7;
int pwm_l = 6;
int pwm_r = 5;
static int q = 0;
static int sg = 0;
static int ok = 0;
int trigPin = 13; // 초음파센서 출력핀
int echoPin = 12; // 초음파센서 입력핀
long duration = 0;
long distance = 0;

void setup()
{
    pinMode(trigPin, OUTPUT); // 트리거 핀을 출력으로 설정
    pinMode(echoPin, INPUT);  // 데코 핀을 입력으로 설정
    Serial.begin(9600);
    mySerial.begin(9600);
    pinMode(dir_l, OUTPUT);
    pinMode(dir_r, OUTPUT);
    pinMode(pwm_l, OUTPUT);
    pinMode(pwm_r, OUTPUT);
    pinMode(PIEZ0_PIN, OUTPUT);
    pinMode(9, OUTPUT);  //초록
    pinMode(10, OUTPUT); //빨강
    pinMode(11, OUTPUT); //파랑
}

char btbuf;

void moving(int dirl, int dirr, int pwml, int pwmr)
{
    digitalWrite(dir_l, dirl);
    digitalWrite(dir_r, dirr);
    analogWrite(pwm_l, pwml);
    analogWrite(pwm_r, pwmr);
}

void loop()
{
    if (ok == 1)
    {
        digitalWrite(trigPin, HIGH);
        delay(2);
        digitalWrite(trigPin, HIGH);
        delay(10);
        digitalWrite(trigPin, LOW);

        duration = pulseIn(echoPin, HIGH);
        distance = duration / 58.2;
        Serial.println(distance);
        if (distance <= 10)
        {
            analogWrite(9, 255);
            analogWrite(10, 0);
            analogWrite(11, 255);
        }
        else if (distance <= 20)
        {
            analogWrite(9, 210);
            analogWrite(10, 0);
            analogWrite(11, 255);
        }
        else
        {
            analogWrite(9, 0);
            analogWrite(10, 255);
            analogWrite(11, 255);
        }
    }
    if (mySerial.available())
    {
        btbuf = mySerial.read();
        Serial.println(btbuf);
        if ('U' == btbuf) //전진
        {
            Serial.println("forward");
            if (sg == 1)
            {
                analogWrite(9, 0);
                analogWrite(10, 255);
                analogWrite(11, 255);
            }

            moving(HIGH, HIGH, 200, 200);
        }
        else if ('u' == btbuf)
        {
            Serial.println("forward");
            moving(HIGH, HIGH, 0, 0);
        }
        else if ('D' == btbuf) //후진
        {
            Serial.println("backward");
            moving(LOW, LOW, 200, 200);
            if (sg == 1)
            {
                analogWrite(9, 255);
                analogWrite(10, 0);
                analogWrite(11, 255);
            }
        }
        else if ('d' == btbuf)
        {
            Serial.println("backward");
            moving(LOW, LOW, 0, 0);
        }
        else if ('F' == btbuf) //돌진
        {
            Serial.println("fast_forward");
            if (sg == 1)
            {
                analogWrite(9, 255);
                analogWrite(10, 255);
                analogWrite(11, 0);
            }
            moving(HIGH, HIGH, 240, 240);
        }
        else if ('f' == btbuf)
        {
            Serial.println("fast_forward");
            moving(HIGH, HIGH, 0, 0);
        }
        else if ('L' == btbuf) //좌회전
        {
            Serial.println("left");
            if (sg == 1)
            {
                analogWrite(9, 210);
                analogWrite(10, 0);
                analogWrite(11, 255);
            }
            moving(LOW, HIGH, 200, 200);
        }
        else if ('l' == btbuf)
        {
            Serial.println("left");
            moving(HIGH, HIGH, 0, 0);
        }
        else if ('R' == btbuf) //우회전
        {
            Serial.println("right");
            if (sg == 1)
            {
                analogWrite(9, 120);
                analogWrite(10, 20);
                analogWrite(11, 255);
            }
            moving(HIGH, LOW, 200, 150);
        }
        else if ('r' == btbuf)
        {
            Serial.println("right");
            moving(HIGH, HIGH, 0, 0);
        }
        else if ('E' == btbuf) //우측 전진
        {
            Serial.println("right_go");
            if (sg == 1)
            {
                analogWrite(9, 120);
                analogWrite(10, 20);
                analogWrite(11, 255);
            }
            moving(HIGH, LOW, 150, 150);
            delay(100);
            moving(HIGH, HIGH, 200, 200);
        }
        else if ('e' == btbuf)
        {
            Serial.println("right_go");
            if (sg == 1)
            {
                analogWrite(9, 0);
                analogWrite(10, 255);
                analogWrite(11, 255);
            }
            moving(HIGH, HIGH, 0, 0);
        }
        else if ('B' == btbuf) //좌측 전전
        {
            Serial.println("left_go");
            if (sg == 1)
            {
                analogWrite(9, 210);
                analogWrite(10, 0);
                analogWrite(11, 255);
            }
            moving(LOW, HIGH, 150, 150);
            delay(100);
            moving(HIGH, HIGH, 200, 200);
        }
        else if ('b' == btbuf)
        {
            Serial.println("left_go");
            if (sg == 1)
            {
                analogWrite(9, 0);
                analogWrite(10, 255);
                analogWrite(11, 255);
            }
            moving(HIGH, HIGH, 0, 0);
        }
        else if ('A' == btbuf) // LED
        {
            Serial.println("LED");
            sg = 0;
            ok = 0;
            for (int i = 0; i < 256; i++)
            {
                analogWrite(9, 255 - i); //초
                analogWrite(10, i);      //빨
                analogWrite(11, 255);    //파
                delay(10);
                btbuf = mySerial.read();
                if ('a' == btbuf)
                    break;
                if (i == 255)
                {
                    for (int j = 0; j < 256; j++)
                    {
                        analogWrite(9, j);        //초
                        analogWrite(10, 255);     //빨
                        analogWrite(11, 255 - j); //파
                        delay(10);
                        btbuf = mySerial.read();
                        if ('a' == btbuf)
                        {
                            break;
                            break;
                        }
                        if (j == 255)
                        {
                            for (int k = 0; k < 256; k++)
                            {
                                analogWrite(9, 255);      //초
                                analogWrite(10, 255 - k); //빨
                                analogWrite(11, k);       //파
                                delay(10);
                                btbuf = mySerial.read();
                                if ('a' == btbuf)
                                {
                                    break;
                                    break;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        else if ('S' == btbuf)
        {
            Serial.println("selemony");
            tone(PIEZ0_PIN, NOTE_A4, 150);
            delay(450);
            tone(PIEZ0_PIN, NOTE_A4, 150);
            delay(150);
            tone(PIEZ0_PIN, NOTE_B4, 150);
            delay(150);
            tone(PIEZ0_PIN, NOTE_C5, 150);
            delay(150);
            tone(PIEZ0_PIN, NOTE_D5, 150);
            delay(450);
            tone(PIEZ0_PIN, NOTE_D5, 150);
            delay(150);
            tone(PIEZ0_PIN, NOTE_C5, 150);
            delay(150);
            tone(PIEZ0_PIN, NOTE_B4, 150);
            delay(150);
            tone(PIEZ0_PIN, NOTE_E5, 1000);
            delay(1000);
            tone(PIEZ0_PIN, NOTE_E5, 150);
            delay(150);
            tone(PIEZ0_PIN, NOTE_CS5, 150);
            delay(150);
            tone(PIEZ0_PIN, NOTE_A4, 150);
            delay(150);
            tone(PIEZ0_PIN, NOTE_E5, 150);
            delay(150);
            tone(PIEZ0_PIN, NOTE_CS5, 150);
            delay(150);
            tone(PIEZ0_PIN, NOTE_A4, 150);
            delay(150);
            tone(PIEZ0_PIN, NOTE_E5, 500);
            delay(500);
            tone(PIEZ0_PIN, NOTE_A5, 100);
            delay(150);
            tone(PIEZ0_PIN, NOTE_A5, 100);
            delay(150);
            tone(PIEZ0_PIN, NOTE_A5, 100);
            delay(150);
            tone(PIEZ0_PIN, NOTE_A5, 100);
            delay(150);
        }
        else if ('s' == btbuf)
        {
            Serial.println("selemony");
            noTone(8);
            q = 0;
        }
        else if ('M' == btbuf)
        {
            ok = 0;
            Serial.println("mode1");
            if (sg == 0)
            {
                sg++;
            }
            else
            {
                sg = 0;
            }
        }
        else if ('m' == btbuf)
        {
            Serial.println("mode1");
        }
        else if ('N' == btbuf)
        {
            sg = 0;
            if (ok == 0)
            {
                ok++;
            }
            else
            {
                ok = 0;
            }
        }
        else if ('n' == btbuf)
        {
            Serial.println("mode2");
        }
    }
}