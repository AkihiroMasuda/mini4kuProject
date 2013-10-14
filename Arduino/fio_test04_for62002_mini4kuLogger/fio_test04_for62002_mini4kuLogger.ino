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

// メディアンフィルタ
BYTE medianFilter(BYTE* dat, BYTE size)
{
  //ソートする
  //	bubbleSort(dat,size);

  //	RB2 = 1;
  {
    BYTE i=0;
    BYTE j=0;
    BYTE t=0;
    BYTE k=0;
    for (i=0; i<size-1; i++){
      for(j=size-1; j>i; j--){
        if(dat[j]<dat[j-1]){
          t = dat[j];
          k = dat[j-1];
          //				dat[j] = dat[j-1];
          dat[j] = k;
          dat[j-1] = t;
        }
      }
    }	
  }
  //	RB2 = 0;

  if (size%2 == 0){
    //データ数が偶数の時
    return (dat[size/2-1] + dat[size/2])>>1;
  }
  else{
    //データ数が奇数の時
    return dat[(size-1)/2];
  }
}

BYTE getAnalogData()
{
  //アナログポート読み込み
  int val = analogRead(APORT_X);
  return val>0xFF ? 0xFF : val;

  //  int ADCValue;
  //
  //  AD1CON1bits.SAMP = 0;  //AD変換開始
  //  while(!AD1CON1bits.DONE){
  //  };
  //  ADCValue = ADC1BUF0;
  //  AD1CON1bits.SAMP = 1;  //変換が終わったのでフラグ立てる
  //
  //  return ADCValue>=0xFF ? 0xFF : ADCValue&0x00FF;
}

// 閾値以下、または以上になるまで監視する。
// 引数：flg   閾値以下を見つける場合は０、閾値以上の場合は１
// 戻り値： 正常：１
//       タイムアウト：０
int detectPitch(BYTE flg, BYTE *buf, BYTE curInd)
{
  BYTE tmp[5];
  //	const BYTE shikii = 0x20;
  //	const BYTE shikii = 146;
//  const BYTE shikii = 253;
//  const BYTE shikii = 150;
  const BYTE shikii = 100;

  buf[0] = getAnalogData();
  buf[1] = getAnalogData();
  buf[2] = getAnalogData();
  buf[3] = getAnalogData();
  buf[4] = getAnalogData();

  while(1){
    //    if (flgLimit){
    //      //タイムアウトエラー
    //      return 0;
    //    }

    int newdat = getAnalogData();
    //		buf[curInd%5] = (BYTE)((newdat>>2)&0x00FF);
    buf[curInd%5] = (BYTE)(newdat);
    ++curInd;
    if (curInd == 5){ 
      curInd = 0;
    }
    my_memcpy(tmp,buf,5);
    BYTE median = medianFilter(tmp, 5);
    if (flg==0){
      if (median <= shikii){
        break;
      }
    }
    else{
      if (median > shikii){
        break;
      }
      //			rs_outbyte(0x99);
    }

  }

  return 1;
}

// タイヤ一回転の時間を測定する。
// 戻り値：時間 [μｓ]
unsigned long calRotUS()
//float calRotUS()
{
  BYTE buf[5];
  BYTE curInd = 0;

  /*
	//まずはバッファにデータを貯める
   	buf[0] = getAnalogData();
   	buf[1] = getAnalogData();
   	buf[2] = getAnalogData();
   	buf[3] = getAnalogData();
   	buf[4] = getAnalogData();
   */
  /*デバッグ用*/
  /*
	while(1){
   		//最初に印が無いところを探す。
   		detectPitch(0, buf, curInd);
   	}
   */

  //タイムリミット用のタイマーをセット
  //  initTimerForTimeLimit();

  //最初に印が無いところを探す。
  if(!detectPitch(0, buf, curInd)){
    //    closeTimerForTimeLimit();
    //    return timeLimitCnt*16;
    return -1;
  }

  //印が通過するのを待つ。
  if(!detectPitch(1, buf, curInd)){
    //    closeTimerForTimeLimit();
    //    return timeLimitCnt*16;
    return -1;
  }

  //タイマーセット
  //  initTimerForVelCnt();
  unsigned long tc0 = micros();

  //ちょっと待って見る。チャタリング対策。
  //  delay_ms(1);
  delay(1);

  //次に印が無いところを通過するのを探す。
  if(!detectPitch(0, buf, curInd)){
    //    closeTimerForVelCnt();
    //    closeTimerForTimeLimit();
    //    return timeLimitCnt*16;
    return -1;
  }

  //ちょっと待って見る。チャタリング対策。
  //  delay_ms(1);
  delay(1);

  //再び印が通過するのを待つ。
  if(!detectPitch(1, buf, curInd)){
    //    closeTimerForVelCnt();
    //    closeTimerForTimeLimit();
    //    return timeLimitCnt*16;
    return -1;
  }

  //タイマー読み込み
  //  unsigned long tmr = TMR4;
  //	int cnt = tmrCnt;
  unsigned long tc1 = micros();

  //タイマー停止
  //  closeTimerForVelCnt();
  //  closeTimerForTimeLimit();

  /*
	while(1){
   		DB_PORTB = 1;
   	}
   */
  //	float fs = 16; //16MIPS
  //	return tmr*64/fs;  //カウント数＊分周比/fs [μｓ]
  //	return tmr*4; //16MIPS, 分周比64なら、１カウント4μｓ
  //  return tmr*16; //16MIPS, 分周比256なら、１カウント16μｓ

  return tc1-tc0;
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

int flg = 0;

//unsigned long tc_old =  0;
//unsigned long dts[5];
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
//    int val[] = {analogRead(APORT_X), analogRead(APORT_Y), analogRead(APORT_Z)};    // アナログピンを読み取る
//    int val[] = {analogRead(APORT_X), 0, 0};    // アナログピンを読み取る
//    int val[] = {0,0,0};    // アナログピンを読み取る
    unsigned long curTime = millis();
    if (curTime-beforeTime >= 50){
      // 50msに一回速度測定するようにする。
      // この閾値無しで測定すると、送信間隔が約15ms程度となり、Bluetoothでの送信処理が輻輳する。
      // なのでとりあえずこの間隔で。
      beforeTime = curTime;

      unsigned long dt = calRotUS();
      float vel = (0.03f*3.14159)/((float)(dt)/1000.f/1000.f)*3.6f;//km/h, 直径3cmと仮定

      memset(sendText, 0, sizeof(sendText));
      sprintf(sendText, "%ld,%ld,%d", curTime, dt, (int)(vel));
      mySerial.println(sendText);
      cnt++;
      
      flg = flg==1 ? 0 : 1;
      digitalWrite(DPORT_LED,flg==1?LOW:HIGH);
      
    }
  }
}



