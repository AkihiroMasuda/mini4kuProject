#include <SoftwareSerial.h>
#define BYTE unsigned char

unsigned long t_next;
const unsigned long T = 1; //50ms
const int DPORT_RX = 4; //シリアル通信用 ポート
const int DPORT_TX = 3; //シリアル通信用 ポート
const int APORT_X = 0;
const int APORT_Y = 1;
const int APORT_Z = 2;
const int DPORT_LED = 8;
char sendText[256]; //シリアル通信で送る文字

SoftwareSerial mySerial(DPORT_RX, DPORT_TX);

boolean isInit = true;

void *my_memcpy(void * d1, const void * s1, int n)
{
  char *d;
  const char *s;

  s = (const char*)s1;
  d = (char*)d1;
  while(n--){
    *d++ = *s++;
  }
  return d1;
}



void setup()
{
  pinMode(DPORT_LED, OUTPUT);
  t_next = millis();
  mySerial.begin(9600);
  //  mySerial.begin(19200);
}


unsigned long cnt = 0;
unsigned long tc_old =  0;
unsigned long dts[5];
unsigned long beforeTime = 0;

void loop()
{
  if (isInit){
    isInit = false;
    delay(5000);
    //LED点灯
    digitalWrite(DPORT_LED,HIGH);
  }else{
    // 一定間隔で送信
    //アナログポート読み込み
    int val[] = {analogRead(APORT_X), analogRead(APORT_Y), analogRead(APORT_Z)};    // アナログピンを読み取る
//    int val[] = {analogRead(APORT_X), 0, 0};    // アナログピンを読み取る
//    int val[] = {0,0,0};    // アナログピンを読み取る
    unsigned long curTime = millis();
    if (curTime-beforeTime >= 50){
      beforeTime = curTime;
      memset(sendText, 0, sizeof(sendText));
      sprintf(sendText, "%ld,%ld,%d", curTime, cnt, val[0]);
      mySerial.println(sendText);
      cnt++;
    }
  }
}



