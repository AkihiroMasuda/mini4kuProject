int PIN = 8; //自作ミニ四駆速度ロガー基板用(62002)。PB0を光らす。回路図上はpin13だが、ここでは8を指定しなくてはならない

void setup()
{
  pinMode(PIN, OUTPUT);
//  pinMode(8, OUTPUT);
}

void loop()
{
  digitalWrite(PIN,HIGH);
//  digitalWrite(8,HIGH);
  delay(350);
  digitalWrite(PIN,LOW);
//  digitalWrite(8,LOW);
  delay(150);
  
}
