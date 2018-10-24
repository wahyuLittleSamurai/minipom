
int strobePin = PB_2;
int dataPin = PB_1;
int clockPin = PB_0;

byte segChar[] = {
  B01010000,                                            
  B11110011,                                             
  B01100100,                                             
  B01100001,                                             
  B11000011,                                             
  B01001001,                                             
  B01001000,                                           
  B01110011,                                           
  B01000000,                                           
  B01000001,
  B11111111 //clear
  };

void setup() {
  pinMode(strobePin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  digitalWrite(strobePin, LOW);
  clear7Seg();
  digitalWrite(strobePin, HIGH);
}

void loop() {
    digitalWrite(strobePin, LOW);
    tampilkan(32000);
    digitalWrite(strobePin, HIGH);
   
} 

int puluhanRibu, ribuan, ratusan, puluhan, satuan;
int ribuan2, ratusan2, puluhan2, satuan2;
void tampilkan(int nomer)
{
  puluhanRibu = nomer/10000;
  ribuan = (nomer-(puluhanRibu*10000))/1000;
  ratusan = (nomer-((puluhanRibu*10000)+(ribuan*1000)))/100;
  puluhan = (nomer-((puluhanRibu*10000)+(ribuan*1000)+(ratusan*100)))/10;
  satuan = nomer-((puluhanRibu*10000)+(ribuan*1000)+(ratusan*100)+(puluhan*10));

  if(nomer>=10000 && nomer<=99999)
  {
    pisahInt(puluhanRibu);
    pisahInt(ribuan);
    pisahInt(ratusan);
    pisahInt(puluhan);
    pisahInt(satuan);
  }
  else if(nomer>=1000 && nomer<=9999)
  {
    pisahInt(10);
    pisahInt(ribuan);
    pisahInt(ratusan);
    pisahInt(puluhan);
    pisahInt(satuan);
  }
  else if(nomer>=100 && nomer<=999)
  {
    pisahInt(10);
    pisahInt(10);
    pisahInt(ratusan);
    pisahInt(puluhan);
    pisahInt(satuan);
  }
  else if(nomer>=10 && nomer<=99)
  {
    pisahInt(10);
    pisahInt(10);
    pisahInt(10);
    pisahInt(puluhan);
    pisahInt(satuan);
  }
  else
  {
    pisahInt(10);
    pisahInt(10);
    pisahInt(10);
    pisahInt(10);
    pisahInt(satuan);
  }
 
}
void clear7Seg()
{
  digitalWrite(strobePin, LOW);
  for(int i=0; i<15; i++)
  {
    shiftOut(dataPin, clockPin, MSBFIRST, segChar[10]);
  }
  digitalWrite(strobePin, HIGH);
}
void pisahInt(int nomer)
{
  switch(nomer)
  {
    case 0: shiftOut(dataPin, clockPin, MSBFIRST, segChar[0]); break;
    case 1: shiftOut(dataPin, clockPin, MSBFIRST, segChar[1]); break;
    case 2: shiftOut(dataPin, clockPin, MSBFIRST, segChar[2]); break;
    case 3: shiftOut(dataPin, clockPin, MSBFIRST, segChar[3]); break;
    case 4: shiftOut(dataPin, clockPin, MSBFIRST, segChar[4]); break;
    case 5: shiftOut(dataPin, clockPin, MSBFIRST, segChar[5]); break;
    case 6: shiftOut(dataPin, clockPin, MSBFIRST, segChar[6]); break; 
    case 7: shiftOut(dataPin, clockPin, MSBFIRST, segChar[7]); break;
    case 8: shiftOut(dataPin, clockPin, MSBFIRST, segChar[8]); break;
    case 9: shiftOut(dataPin, clockPin, MSBFIRST, segChar[9]); break;
    case 10:shiftOut(dataPin, clockPin, MSBFIRST, segChar[10]); break;
  }
}
