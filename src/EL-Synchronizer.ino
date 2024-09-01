// ※Arduino R4 MINIMAではUSBシリアルとD0(RX),D1(TX)が独立しているため、MIDI Liberaryが使用できません。

#include <MIDI.h> //MIDIライブラリ使用のためのヘッダファイル読み込み

// MIDIメッセージ送受信の方法についての簡単なチュートリアル
// チャンネル4にメッセージが入ってきたら、1秒間ArduinoのLEDが点灯し、その間ノートが再生される。

MIDI_CREATE_DEFAULT_INSTANCE(); // MIDIクラスのインスタンスとして"MIDI"を生成する。

#define LED 13              // Arduino Uno用のLEDピンの設定
#define PRESS_THRESHOLD 3

class Button{
  public:
    int pinNumber;
    int buttonState;
    int gauge;

    void addGauge(){
      gauge++;
    }

    void resetGauge(){
      gauge = 0;
    }

    int getButtonState(){
      buttonState = digitalRead(pinNumber);
      return buttonState;
    }
};

class SeqButton : public Button{
  public:
    bool seqState;
    int seqNumber;

    SeqButton(int pin,int seqNum){
      pinNumber = pin;
      seqNumber = seqNum;
      buttonState = 1;
      gauge = 0;
      seqState = false;
    }
};

class GeneralButton : public Button{
  public:
    bool isPlaying;

    GeneralButton(int pin){
      pinNumber = pin;
      buttonState = 1;
      gauge = 0;
      isPlaying = false;
    }
};

SeqButton* seqButtons[4]; // SeqButtonオブジェクトへのポインタを格納する配列
GeneralButton* generalButtons[1]; // StartButtonオブジェクトへのポインタを格納する配列

void setup() {
  MIDI.setHandleStart(rhythmStart);
  MIDI.setHandleStop(rhythmStop);
  // MIDI.setHandleSystemExclusive(handleSysEx);

  // MIDI通信をしている時はシリアル通信は使えない
  MIDI.begin(MIDI_CHANNEL_OMNI); // MIDIインスタンスの初期化、全chをlisten
  MIDI.turnThruOff();             // MIDI Thruをオフ

  seqButtons[0] = new SeqButton(11,1); // seq1
  seqButtons[1] = new SeqButton(12,2); // seq2
  seqButtons[2] = new SeqButton(10,3); // seq3
  seqButtons[3] = new SeqButton(9,4); // seq4
  generalButtons[0] = new GeneralButton(4); // start

  
  for(auto button : seqButtons){
    pinMode(button->pinNumber, INPUT_PULLUP);//PINを受信用に設定、プルアップ抵抗付き
  }
  for(auto button : generalButtons){
    pinMode(button->pinNumber, INPUT_PULLUP);
  }

  pinMode(13, OUTPUT);//13PInを出力用に設定
  pinMode(6, OUTPUT);//3PInを出力用に設定

  digitalWrite(13,HIGH);

}

void loop() {
  MIDI.read();

  // ボタンの判定
  for(auto button : generalButtons){
    if(button->getButtonState() == LOW){
      if(button->gauge > 0){ // 十分押されたとき
        if(!button->isPlaying){
          rhythmStart();
        }else{
          rhythmStop();
        }
        button->isPlaying = !button->isPlaying;
        button->buttonState = 0;
        delay(400);
      }else{ // 押されているがまだ短いとき
        button->buttonState = 1;
        button->addGauge();
      }
    }else{ // 押されていないとき
      button->buttonState = 1;
      button->resetGauge();
    }
  }

  for(auto button : seqButtons){
    if(button->getButtonState() == LOW){
      if(button->gauge > 0){ // 十分押されたとき
        if(!button->seqState){
          seqOn(button->seqNumber);
        }else{
          seqOff(button->seqNumber);
        }
        button->seqState = !button->seqState;
        button->buttonState = 0;
        delay(200);
      }else{ // 押されているがまだ短いとき
        button->buttonState = 1;
        button->addGauge();
      }
    }else{ // 押されていないとき
      button->buttonState = 1;
      button->resetGauge();
    }
  }
}

void seqOn(int seqNumber){
  flash();
  byte sendMessage[10] = {0xF0,0x43,0x70,0x78,0x44,0x13,0x01,seqNumber-1,0x01,0xF7};
  // byte sendMessage[10] = {0xF0,0x43,0x70,0x78,0x44,0x13,0x01,i,0x01,0xF7};
  MIDI.sendSysEx(10,sendMessage,true);
}
void seqOff(int seqNumber){
  flash();
  byte sendMessage[10] = {0xF0,0x43,0x70,0x78,0x44,0x13,0x01,seqNumber-1,0x00,0xF7};
  // byte sendMessage[10] = {0xF0,0x43,0x70,0x78,0x44,0x13,0x01,i,0x00,0xF7};
  MIDI.sendSysEx(10,sendMessage,true);
}

void rhythmStart(){
  flash();
  byte sendMessage[5] = {0xF0,0x43,0x60,0x7A,0xF7};
  MIDI.sendSysEx(5,sendMessage,true);
}
void rhythmStop(){
  flash();
  byte sendMessage[5] = {0xF0,0x43,0x60,0x7D,0xF7};
  MIDI.sendSysEx(5,sendMessage,true);
}

void flash(){
  digitalWrite(6,HIGH);
  delay(10);
  digitalWrite(6,LOW);
}

void handleSysEx(byte *array, unsigned size){
  MIDI.sendSysEx(size, array, true);
}