#include <SoftwareSerial.h>

unsigned long t_next;
const unsigned long T = 1; //50ms
const int DPORT_RX = 4; //シリアル通信用 ポート
const int DPORT_TX = 3; //シリアル通信用 ポート
const int APORT_X = 0;
const int APORT_Y = 1;
const int APORT_Z = 2;
const int DPORT_LED = 8;  //62002基板のLED
//const int DPORT_LED = 13; //ブレッドボードで組んだ互換基板のLED
char sendText[256]; //シリアル通信で送る文字

SoftwareSerial mySerial(DPORT_RX, DPORT_TX);

boolean isInit = true;

void setup()
{
  pinMode(DPORT_LED, OUTPUT);
  t_next = millis();
  mySerial.begin(9600);
//  mySerial.begin(19200);
}


int cnt = 0;
unsigned long tc_old =  0;
unsigned long dts[5];

void loop()
{
  if (isInit){
    isInit = false;
    delay(5000);
  }

  unsigned long tc = millis();
//  unsigned long tc = micros();
  unsigned long dt = tc - tc_old;
  tc_old = tc;
//  if (cnt < 5){
//    dts[cnt] = dt;  
//    ++cnt;
////    delay(0);
//  }else{
//    cnt = 0;
//    memset(sendText, 0, sizeof(sendText));
////    sprintf(sendText, "dts : %ld,%ld,%ld,%ld,%ld\n", dts[0], dts[1], dts[2], dts[3], dts[4]);
//    sprintf(sendText, "dts : %ld,%ld,%ld,%ld,%ld", dts[0], dts[1], dts[2], dts[3], dts[4]);
//    mySerial.println(sendText);
//    if (mySerial.available()){
//      mySerial.read();
//      sprintf(sendText, "OOOOOOOOOOOOOOOOOOOOOOO\n000000000000000000000\n");
//      mySerial.println(sendText);
//    }
//
//  }
  
  if (t_next < tc){
    //T秒経過した
    t_next = tc + T;
   
    //アナログポート読み込み
    int val[] = {analogRead(APORT_X), analogRead(APORT_Y), analogRead(APORT_Z)};    // アナログピンを読み取る
   
    if (mySerial.available()){
      val[0] = mySerial.read();
      memset(sendText, 0, sizeof(sendText));
//      sprintf(sendText, "%ld,%d,%d,%d\n", tc, val[0], val[1], val[2]);
      sprintf(sendText, "dataget : %ld,%d\n", tc, val[0], val[1]);
      mySerial.println(sendText);
    }else{
      //シリアル通信で送信
      memset(sendText, 0, sizeof(sendText));
      sprintf(sendText, "%ld,%04d,%04d,%04d\n", tc, val[0], val[1], val[2]);
//      sprintf(sendText, "%ld\n", tc);
      mySerial.println(sendText);
  //    mySerial.write("\n");
      
    }

  }
    //LED点灯
    digitalWrite(DPORT_LED,HIGH);
//    delay(30);
//    digitalWrite(DPORT_LED,LOW);
//    delay(1300);

//  }
}

