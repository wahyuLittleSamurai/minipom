#include <LiquidCrystal.h>
#include <Keypad.h>

#include <EEPROM.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#define backLcd PC_3
#define display PB_3
#define buzzer PD_6

#define strobePin PB_2
#define dataPin PB_1
#define clockPin PB_0

#define valve1  PD_7
#define pump    PD_4

LiquidCrystal lcd(PC_1, PC_2, PC_4, PC_5, PC_6, PC_7);

char lcdBuff[16];

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {PA_0, PA_1, PA_2, PA_3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {PA_4, PA_5, PA_6, PA_7}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

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
byte segChar2[] = {
  B00010000,                                            
  B10110011,                                             
  B00100100,                                             
  B00100001,                                             
  B10000011,                                             
  B00001001,                                             
  B00001000,                                           
  B00110011,                                           
  B00000000,                                           
  B00000001,
  B11111111 //clear
  };
  
byte cursorMenu[8] = {    //emoticon battery tinggal 4
  0b00000,
  0b00100,
  0b00110,
  0b11111,
  0b11111,
  0b00110,
  0b00100,
  0b00000
};
byte cursorDown[8] = {    //emoticon battery tinggal 4
  0b00000,
  0b00000,
  0b01110,
  0b01110,
  0b11111,
  0b11111,
  0b01110,
  0b00100
};
int flowMeter;

void setup() {
  // initialize digital pin 13 as an output.
  pinMode(valve1, OUTPUT);
  pinMode(pump, OUTPUT);
  digitalWrite(pump, HIGH);
  //digitalWrite(valve1, HIGH);
  
  pinMode(backLcd, OUTPUT);
  pinMode(display, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  pinMode(strobePin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  digitalWrite(strobePin, LOW);
  clear7Seg();
  digitalWrite(strobePin, HIGH);

  digitalWrite(strobePin, LOW);
  
  attachInterrupt(0, countWaterFlow, RISING);
  clear7Seg();
  
  lcd.begin(16, 2);
  digitalWrite(backLcd, HIGH);
  lcd.createChar (0, cursorMenu);
  lcd.createChar (1, cursorDown);
  lcd.clear();
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  delay(25);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  
}
String valueInputRupiah, inputUserHarga;
long int ratusanRibu, puluhanRibu, ribuan, ratusan, puluhan, satuan;
long int ratusanRibu2, puluhanRibu2, ribuan2, ratusan2, puluhan2, satuan2;
long int ratusanRibu3, puluhanRibu3, ribuan3, ratusan3, puluhan3, satuan3;
long int realValueSecond = 0;
void tampilkan(long int nomer, long int nomer2, long int nomer3)
{
  digitalWrite(strobePin, LOW);
  ratusanRibu = nomer/100000;
  puluhanRibu = (nomer-(ratusanRibu*100000))/10000;
  ribuan = (nomer-((ratusanRibu*100000)+(puluhanRibu*10000)))/1000;
  ratusan = (nomer-((ratusanRibu*100000)+(puluhanRibu*10000)+(ribuan*1000)))/100;
  puluhan = (nomer-((ratusanRibu*100000)+(puluhanRibu*10000)+(ribuan*1000)+(ratusan*100)))/10;
  satuan = nomer-((ratusanRibu*100000)+(puluhanRibu*10000)+(ribuan*1000)+(ratusan*100)+(puluhan*10));

 
  ribuan2 = nomer2/1000;
  ratusan2 = (nomer2-(ribuan2*1000))/100;
  puluhan2 = (nomer2-((ribuan2*1000)+(ratusan2*100)))/10;
  satuan2 = nomer2-((ribuan2*1000)+(ratusan2*100)+(puluhan2*10));
  
  puluhanRibu3 = nomer3/10000;
  ribuan3 = (nomer3-(puluhanRibu3*10000))/1000;
  ratusan3 = (nomer3-((puluhanRibu3*10000)+(ribuan3*1000)))/100;
  puluhan3 = (nomer3-((puluhanRibu3*10000)+(ribuan3*1000)+(ratusan3*100)))/10;
  satuan3 = nomer3-((puluhanRibu3*10000)+(ribuan3*1000)+(ratusan3*100)+(puluhan3*10));
  
  if(nomer >= 100000 && nomer<999999)
  {
    pisahInt(ratusanRibu,false);
    pisahInt(puluhanRibu,false);
    pisahInt(ribuan,true);
    pisahInt(ratusan,false);
    pisahInt(puluhan,false);
    pisahInt(satuan,false);
  }
  else if(nomer>=10000 && nomer<=99999)
  {
    pisahInt(10,false);
    pisahInt(10,false);
    pisahInt(puluhanRibu,false);
    pisahInt(ribuan,true);
    pisahInt(ratusan,false);
    pisahInt(puluhan,false);
    pisahInt(satuan,false);
  }
  else if(nomer>=1000 && nomer<=9999)
  {
    pisahInt(10,false);
    pisahInt(10,false);
    pisahInt(ribuan,true);
    pisahInt(ratusan,false);
    pisahInt(puluhan,false);
    pisahInt(satuan,false);
  }
  else if(nomer>=100 && nomer<=999)
  {
    pisahInt(10,false);
    pisahInt(10,false);
    pisahInt(10,false);
    pisahInt(ratusan,false);
    pisahInt(puluhan,false);
    pisahInt(satuan,false);
  }
  else if(nomer>=10 && nomer<=99)
  {
    pisahInt(10,false);
    pisahInt(10,false);
    pisahInt(10,false);
    pisahInt(10,false);
    pisahInt(puluhan,false);
    pisahInt(satuan,false);
  }
  else
  {
    pisahInt(10,false);
    pisahInt(10,false);
    pisahInt(10,false);
    pisahInt(10,false);
    pisahInt(satuan,false);
  }
  
  if(nomer2>=1000 && nomer2<=9999)
  {
    pisahInt(ribuan2,false);
    pisahInt(ratusan2,true);
    pisahInt(puluhan2,false);
    pisahInt(satuan2,false);
  }
  else if(nomer2>=100 && nomer2<=999)
  {
    pisahInt(10,false);
    pisahInt(ratusan2,true);
    pisahInt(puluhan2,false);
    pisahInt(satuan2,false);
  }
  else if(nomer2>=10 && nomer2<=99)
  {
    pisahInt(10,false);
    pisahInt(0,true);
    pisahInt(puluhan2,false);
    pisahInt(satuan2,false);
  }
  else
  {
    pisahInt(10,false);
    pisahInt(0,true);
    pisahInt(0,false);
    pisahInt(satuan2,false);
  }
  
  if(nomer3>=10000 && nomer3<=99999)
  {
    pisahInt(puluhanRibu3,false);
    pisahInt(ribuan3,true);
    pisahInt(ratusan3,false);
    pisahInt(puluhan3,false);
    pisahInt(satuan3,false);
  }
  else if(nomer3>=1000 && nomer3<=9999)
  {
    pisahInt(10,false);
    pisahInt(ribuan3,true);
    pisahInt(ratusan3,false);
    pisahInt(puluhan3,false);
    pisahInt(satuan3,false);
  }
  else if(nomer3>=100 && nomer3<=999)
  {
    pisahInt(10,false);
    pisahInt(10,false);
    pisahInt(ratusan3,false);
    pisahInt(puluhan3,false);
    pisahInt(satuan3,false);
  }
  else if(nomer3>=10 && nomer3<=99)
  {
    pisahInt(10,false);
    pisahInt(10,false);
    pisahInt(10,false);
    pisahInt(puluhan3,false);
    pisahInt(satuan3,false);
  }
  else
  {
    pisahInt(10,false);
    pisahInt(10,false);
    pisahInt(10,false);
    pisahInt(10,false);
    pisahInt(satuan3,false);
  }
  digitalWrite(strobePin, HIGH);
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
void pisahInt(int nomer, boolean koma)
{
  if(koma == false)
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
  else
  {
    switch(nomer)
    {
      case 0: shiftOut(dataPin, clockPin, MSBFIRST, segChar2[0]); break;
      case 1: shiftOut(dataPin, clockPin, MSBFIRST, segChar2[1]); break;
      case 2: shiftOut(dataPin, clockPin, MSBFIRST, segChar2[2]); break;
      case 3: shiftOut(dataPin, clockPin, MSBFIRST, segChar2[3]); break;
      case 4: shiftOut(dataPin, clockPin, MSBFIRST, segChar2[4]); break;
      case 5: shiftOut(dataPin, clockPin, MSBFIRST, segChar2[5]); break;
      case 6: shiftOut(dataPin, clockPin, MSBFIRST, segChar2[6]); break; 
      case 7: shiftOut(dataPin, clockPin, MSBFIRST, segChar2[7]); break;
      case 8: shiftOut(dataPin, clockPin, MSBFIRST, segChar2[8]); break;
      case 9: shiftOut(dataPin, clockPin, MSBFIRST, segChar2[9]); break;
      case 10:shiftOut(dataPin, clockPin, MSBFIRST, segChar2[10]); break;
    }
  }
    
}
void countWaterFlow()
{
  flowMeter++;
}
// the loop function runs over and over again forever
int perMili,perHarga, stopHargaPompa;
int posX, realValue, perRising, valueRupiah;
float realValueFloat;
char customKey;
char password[10];
String pass;
String hargaPerLiter;
String passwordChange;
String changeP1;
char kode[5];
String kodeToStr;
String valueP1, valueP2, solRem, valueStokBbm, valueToleransi, valueCutAwal;
int saveAllValue[10];
boolean forTrue = false;
boolean keyPress = false;
float realValueSecondFloat;
boolean pompa = false;
int tahanLiter;
void loop() {
 
awal:
  lcd.setCursor(0,0);
  lcd.print("*sfe-electronics");
  lcd.setCursor(0,1);
  lcd.print("*PertaMini v0.1*");
  customKey = customKeypad.getKey();
  if (customKey == '0')//admin
  {
    lcd.clear();
    goto password;
  }
  if(customKey == '1')//user
  {
    lcd.clear();
    goto user;  
  }
  if(customKey == '*')//start
  {
    lcd.clear();
    flowMeter = 0;
    goto start;
  }
goto awal;
password:
  lcd.setCursor(0,0);
  lcd.print("Press Password");
  customKey = customKeypad.getKey();
  if (customKey)
  {
    lcd.setCursor(posX, 1);
    lcd.print("*");
    password[posX] = customKey;
    posX++;
  }  
  if(customKey == 'A')
  {
    lcd.clear();
    for(int i=0; i<posX-1; i++)
    {
      pass += String(password[i]);
    }
    
    int getPassword = eeprom_read_word((uint16_t*)2);
    String pwToString = String(getPassword);
    //if(pass == pwToString)
    if(pass == "2222")
    {
      posX = 6;
      pass = "";
      lcd.setCursor(0,0);
      lcd.print("success login...");
      delay(1500);
      lcd.clear();
      
      goto passwordValid;  
    }  
    if(pass != pwToString)
    {
      posX = 0;
      pass = "";
      lcd.setCursor(0,0);
      lcd.print("invalid password");
      delay(1500);
      lcd.clear();
      
      goto awal;
    }
  }
goto password;
passwordValid:
  customKey = customKeypad.getKey();
  
  lcd.setCursor(posX,0);
  lcd.write(byte(1));
  switch (posX)
  {
    case 6: lcd.setCursor(0,1); lcd.print("~Set Harga/Liter"); break;
    case 7: lcd.setCursor(0,1); lcd.print("~Total Penjualan"); break;
    case 8: lcd.setCursor(0,1); lcd.print("~Ganti Password"); break;
    case 9: lcd.setCursor(0,1); lcd.print("~Reset Transaksi"); break;
  }
  
  if(customKey == 'B')
  {
    lcd.clear();
    posX++;
  }
  if(customKey == 'C')
  {
    lcd.clear();
    posX--;
  }
  if(customKey == 'A')
  {
    lcd.clear();
    goto awal;
  }
  if(posX > 9)
  {
    posX = 6;
  }
  if(posX < 6)
  {
    posX = 9;
  }

  if(customKey == 'D')
  {
    switch(posX)
    {
      case 6: lcd.clear(); posX = 0; realValue = 0; goto setHarga; break;
      case 7: lcd.clear(); posX = 0; goto totalPenjualan; break;
      case 8: lcd.clear(); posX = 0; realValue = 0; goto gantiPassword; break;
      case 9: lcd.clear(); posX = 0; goto resetTransaksi; break;
    }
  }
goto passwordValid;
setHarga:
  lcd.setCursor(0,0);
  sprintf(lcdBuff,"Harga:%i",eeprom_read_word((uint16_t*)0));
  lcd.print(lcdBuff);
  
  customKey = customKeypad.getKey();

  if(customKey)
  {
    if(customKey == '*' || customKey == 'B' || customKey == 'C' || customKey == '#')
    {}
    else if(customKey == 'A')
    {
      lcd.clear();
      posX = 0;
      goto passwordValid;
    }
    else if(customKey == 'D')
    {
      for(int i=0; i<posX; i++)
      {
        realValueFloat += saveAllValue[i] * pow(10,posX-1-i);
      }
      realValueFloat += 0.5;
      realValue = realValueFloat;
     
      if(forTrue == true)
      {
        eeprom_write_word((uint16_t*)0, realValue);
      }

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Save Harga");
      delay(1500);
      lcd.clear();
      posX = 6;
      forTrue = false;
      hargaPerLiter = "";
      realValue = realValueFloat = 0;
      goto passwordValid;  
    }
    else
    {

      forTrue = true;
      saveAllValue[posX] = int(customKey);
      saveAllValue[posX] -= 48;
      hargaPerLiter += String(customKey);
      lcd.setCursor(15-posX, 1);
      lcd.print(hargaPerLiter);
      posX++;
    }
  }
goto setHarga;

totalPenjualan:
  lcd.setCursor(0,0);
  lcd.print("Total Penjualan");
goto totalPenjualan;

gantiPassword:
  lcd.setCursor(0,0);
  sprintf(lcdBuff,"Pass:%i",eeprom_read_word((uint16_t*)2));
  lcd.print(lcdBuff);

  customKey = customKeypad.getKey();

  if(customKey)
  {
    if(customKey == '*' || customKey == 'B' || customKey == 'C' || customKey == '#')
    {}
    else if(customKey == 'A')
    {
      lcd.clear();
      posX = 0;
      goto passwordValid;
    }
    else if(customKey == 'D')
    {
      
      for(int i=0; i<posX; i++)
      {
        realValueFloat += saveAllValue[i] * pow(10,posX-1-i);
      }
      realValueFloat += 0.5;
      realValue = realValueFloat;
      
      if(forTrue == true)
      {
        eeprom_write_word((uint16_t*)2, realValue);
      }
      
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Save Password");
      lcd.clear();
      posX = 6;
      forTrue = false;
      passwordChange = "";
      realValue = realValueFloat = 0;
      goto passwordValid;  
    }
    else
    {
      forTrue = true;
      saveAllValue[posX] = int(customKey);
      saveAllValue[posX] -= 48;
      passwordChange += String(customKey);
      lcd.setCursor(15-posX, 1);
      lcd.print(passwordChange);
      posX++;
    }
  }
goto gantiPassword;

resetTransaksi:
  lcd.setCursor(0,0);
  lcd.print("Reset Transaksi");
goto resetTransaksi;


//####### user
user:
  lcd.setCursor(0,0);
  lcd.print("Inputkan Kode");

  customKey = customKeypad.getKey();
  if (customKey)
  {
    lcd.setCursor(posX, 1);
    lcd.print("*");
    kode[posX] = customKey;
    posX++;
  }  
  if(posX >=2)
  {
    kodeToStr = String(kode);
    posX = 0;
    lcd.clear();
    goto switchKode;
  }
goto user;
  
switchKode:
  if(kodeToStr == "31")
  {
    customKey = 0;
    goto inputP1;
  }
  else if(kodeToStr == "32")
  {
    customKey = 0;
    goto inputP2;
  }
  else if(kodeToStr == "33")
  {
    customKey = 0;
    goto inputSolRem;
  }
  else if(kodeToStr == "34")
  {
    customKey = 0;
    goto stokBbm;
  }
  else if(kodeToStr == "35")
  {
    customKey = 0;
    goto sisaBbm;
  }
  else if(kodeToStr == "36")
  {
    customKey = 0;
    goto jualTerakhir;
  }
  else if(kodeToStr == "37")
  {
    customKey = 0;
    goto toleransi;
  }
  else if(kodeToStr == "38")
  {
    customKey = 0;
    goto cutAwal;
  }
  else
  {
    lcd.clear();
    posX = 0;
    goto awal;
    
  }
goto switchKode;

inputP1:
  lcd.setCursor(0,0);
  sprintf(lcdBuff,"Harga P1:%i",eeprom_read_word((uint16_t*)4));
  lcd.print(lcdBuff);

  customKey = customKeypad.getKey();
  if(customKey)
  {
    if(customKey == '*' || customKey == 'B' || customKey == 'C' || customKey == '#')
    {}
    else if(customKey == 'A')
    {
      lcd.clear();
      posX = 0;
      goto awal;
    }
    else if(customKey == 'D')
    {
      
      for(int i=0; i<posX; i++)
      {
        realValueFloat += saveAllValue[i] * pow(10,posX-1-i);
      }
      realValueFloat += 0.5;
      realValue = realValueFloat;
      
      if(forTrue == true)
      {
        eeprom_write_word((uint16_t*)4, realValue);
      }
      
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Save Nilai P1");
      delay(1500);
      lcd.clear();
      posX = 0;
      forTrue = false;
      valueP1 = "";
      realValue = realValueFloat = 0;
      goto awal;  
    }
    else
    {
      forTrue = true;
      saveAllValue[posX] = int(customKey);
      saveAllValue[posX] -= 48;
      valueP1 += String(customKey);
      lcd.setCursor(15-posX, 1);
      lcd.print(valueP1);
      posX++;
    }
  }
goto inputP1;
inputP2:
  lcd.setCursor(0,0);
  sprintf(lcdBuff,"Harga P2:%i",eeprom_read_word((uint16_t*)6));
  lcd.print(lcdBuff);

  customKey = customKeypad.getKey();
  if(customKey)
  {
    if(customKey == '*' || customKey == 'B' || customKey == 'C' || customKey == '#')
    {}
    else if(customKey == 'A')
    {
      lcd.clear();
      posX = 0;
      goto awal;
    }
    else if(customKey == 'D')
    {
      
      for(int i=0; i<posX; i++)
      {
        realValueFloat += saveAllValue[i] * pow(10,posX-1-i);
      }
      realValueFloat += 0.5;
      realValue = realValueFloat;
      
      if(forTrue == true)
      {
        eeprom_write_word((uint16_t*)6, realValue);
      }
      
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Save Nilai P2");
      delay(1500);
      lcd.clear();
      posX = 0;
      forTrue = false;
      valueP2 = "";
      realValue = realValueFloat = 0;
      goto awal;  
    }
    else
    {
      forTrue = true;
      saveAllValue[posX] = int(customKey);
      saveAllValue[posX] -= 48;
      valueP2 += String(customKey);
      lcd.setCursor(15-posX, 1);
      lcd.print(valueP2);
      posX++;
    }
  }
goto inputP2;

inputSolRem:
  lcd.setCursor(0,0);
  sprintf(lcdBuff,"SolRem:%i",eeprom_read_word((uint16_t*)8));
  lcd.print(lcdBuff);

  customKey = customKeypad.getKey();
  if(customKey)
  {
    if(customKey == '*' || customKey == 'B' || customKey == 'C' || customKey == '#')
    {}
    else if(customKey == 'A')
    {
      lcd.clear();
      posX = 0;
      goto awal;
    }
    else if(customKey == 'D')
    {
      
      for(int i=0; i<posX; i++)
      {
        realValueFloat += saveAllValue[i] * pow(10,posX-1-i);
      }
      realValueFloat += 0.5;
      realValue = realValueFloat;
      if(forTrue == true)
      {
        eeprom_write_word((uint16_t*)8, realValue);
      }
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Save SolRem");
      delay(1500);
      lcd.clear();
      posX = 0;
      forTrue = false;
      solRem = "";
      realValue = realValueFloat = 0;
      goto awal;  
    }
    else
    {
      forTrue = true;
      saveAllValue[posX] = int(customKey);
      saveAllValue[posX] -= 48;
      solRem += String(customKey);
      lcd.setCursor(15-posX, 1);
      lcd.print(solRem);
      posX++;
    }
  }
goto inputSolRem;

stokBbm:
  lcd.setCursor(0,0);
  sprintf(lcdBuff,"Stok BBM :%i",eeprom_read_word((uint16_t*)14));
  lcd.print(lcdBuff);
  
  customKey = customKeypad.getKey();
  if(customKey)
  {
    if(customKey == '*' || customKey == 'B' || customKey == 'C' || customKey == '#')
    {}
    else if(customKey == 'A')
    {
      lcd.clear();
      posX = 0;
      goto awal;
    }
    else if(customKey == 'D')
    {
      for(int i=0; i<posX; i++)
      {
        realValueFloat += saveAllValue[i] * pow(10,posX-1-i);
      }
      realValueFloat += 0.5;
      realValue = realValueFloat;
      
      if(forTrue == true)
      {
        int readSisa = eeprom_read_word((uint16_t*)14);
        eeprom_write_word((uint16_t*)14, realValue+readSisa);
      }
      
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Save Stok BBM");
      delay(1500);
      lcd.clear();
      forTrue = false;
      posX = 0;
      realValue = realValueFloat = 0;
      goto awal;  
    }
    else
    {
      saveAllValue[posX] = int(customKey);
      saveAllValue[posX] -= 48;
      valueStokBbm += String(customKey);
      lcd.setCursor(15-posX, 1);
      lcd.print(valueStokBbm);
      forTrue = true;
      posX++;
    }
  }
goto stokBbm;

sisaBbm:
  lcd.setCursor(0,0);
  sprintf(lcdBuff,"Sisa BBM :%i",eeprom_read_word((uint16_t*)14));
  lcd.print(lcdBuff);

  customKey = customKeypad.getKey();
  if(customKey == 'D')
  {
    posX = 0;
    goto awal;
  }
goto sisaBbm;
  
jualTerakhir:
  lcd.setCursor(0,0);
  lcd.print("Jual Terakhir");

  customKey = customKeypad.getKey();
  if(customKey == 'D')
  {
    posX = 0;
    goto awal;
  }
goto jualTerakhir;

toleransi:
  lcd.setCursor(0,0);
  sprintf(lcdBuff,"Toleransi:%i",eeprom_read_word((uint16_t*)10));
  lcd.print(lcdBuff);
  
  customKey = customKeypad.getKey();
  if(customKey)
  {
    if(customKey == '*' || customKey == 'B' || customKey == 'C' || customKey == '#')
    {}
    else if(customKey == 'A')
    {
      lcd.clear();
      posX = 0;
      goto awal;
    }
    else if(customKey == 'D')
    {
      
      for(int i=0; i<posX; i++)
      {
        realValueFloat += saveAllValue[i] * pow(10,posX-1-i);
      }
      realValueFloat += 0.5;
      realValue = realValueFloat;
      if(forTrue == true)
      {
        eeprom_write_word((uint16_t*)10, realValue);
      }
      
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Save Toleransi");
      delay(1500);
      lcd.clear();
      posX = 0;
      forTrue = false;
      valueToleransi = "";
      realValue = realValueFloat = 0;
      goto awal;  
    }
    else
    {
      forTrue = true;
      saveAllValue[posX] = int(customKey);
      saveAllValue[posX] -= 48;
      valueToleransi += String(customKey);
      lcd.setCursor(15-posX, 1);
      lcd.print(valueToleransi);
      posX++;
    }
  }
goto toleransi;

cutAwal:
  lcd.setCursor(0,0);
  sprintf(lcdBuff,"Val /Liter: %i",eeprom_read_word((uint16_t*)12));
  lcd.print(lcdBuff);
  
  customKey = customKeypad.getKey();
  if(customKey)
  {
    if(customKey == '*' || customKey == 'B' || customKey == 'C' || customKey == '#')
    {}
    else if(customKey == 'A')
    {
      lcd.clear();
      posX = 0;
      goto awal;
    }
    else if(customKey == 'D')
    {
      
      for(int i=0; i<posX; i++)
      {
        realValueFloat += saveAllValue[i] * pow(10,posX-1-i);
      }
      realValueFloat += 0.5;
      realValue = realValueFloat;
      if(forTrue == true)
      {
        eeprom_write_word((uint16_t*)12, realValue);
      }
      
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Save Cut Awal");
      delay(1500);
      lcd.clear();
      posX = 0;
      forTrue = false;
      valueCutAwal = "";
      realValue = realValueFloat = 0;
      goto awal;  
    }
    else
    {
      forTrue = true;
      saveAllValue[posX] = int(customKey);
      saveAllValue[posX] -= 48;
      valueCutAwal += String(customKey);
      lcd.setCursor(15-posX, 1);
      lcd.print(valueCutAwal);
      posX++;
    }
  }
goto cutAwal;
  lcd.setCursor(0,0);
  lcd.print("start...");
  posX = 1;
start:
  lcd.setCursor(0,0);
  lcd.print("Input: ");

  perHarga = eeprom_read_word((uint16_t*)0);
  perMili = eeprom_read_word((uint16_t*)12);

  float perMiliSecond = perMili;
  float nilaiLiter = flowMeter/perMiliSecond;
  int nilaiMiliLiter = nilaiLiter*100;
  
  perRising = perHarga/perMili;
  valueRupiah = perRising * flowMeter;
  
  lcd.setCursor(9,0);
  sprintf(lcdBuff,"%6d",realValueSecond);
  lcd.print(lcdBuff);
  lcd.setCursor(0,1);
  sprintf(lcdBuff,"%15d",valueRupiah);
  lcd.print(lcdBuff);

  if(pompa == true)
  {
    tampilkan(valueRupiah,nilaiMiliLiter,perHarga);
  }
  else
  {
    tampilkan(realValueSecond,tahanLiter,perHarga);  
  }
  
  if(valueRupiah >= stopHargaPompa && keyPress == true)
  {
    tahanLiter = nilaiMiliLiter;
    valueRupiah = stopHargaPompa;
    tampilkan(realValueSecond,nilaiMiliLiter,perHarga); 
    //motor and solenoid close
    digitalWrite(pump, HIGH);
    digitalWrite(valve1, LOW);

    digitalWrite(buzzer, LOW);
    delay(2000);
    digitalWrite(buzzer, HIGH);

    flowMeter = 0;
    
    int readSisa = eeprom_read_word((uint16_t*)14);
    int setelahDiKurang = readSisa - int(nilaiLiter);
    eeprom_write_word((uint16_t*)14, setelahDiKurang);

    for(int i=0; i<=10; i++)
    {
      saveAllValue[i]=0;
    }
    
    keyPress = false;
    pompa = false;
  }
  
    customKey = customKeypad.getKey();
    if(customKey)
    {
      if(customKey == 'A')
      {
        lcd.setCursor(8,0);
        lcd.print("10.000");
        realValueSecond = 10000;
        stopHargaPompa = eeprom_read_word((uint16_t*)4);
        perMili = perHarga = flowMeter = 0;
      }
      else if(customKey == 'B')
      {
        lcd.setCursor(8,0);
        lcd.print("15.000");
        realValueSecond = 15000;  
        stopHargaPompa = eeprom_read_word((uint16_t*)6);
        perMili = perHarga = flowMeter = 0;
      }
      else if(customKey == '#')
      {
        keyPress = true;
        pompa = true;
        digitalWrite(valve1, HIGH);
        digitalWrite(pump, LOW);
        //motor jalan...
      }
      else if(customKey == 'D')
      {
        lcd.clear();
        digitalWrite(strobePin, LOW);
        clear7Seg();
        digitalWrite(strobePin, HIGH);
        posX = 0;
        goto awal;  
      }
      else if (customKey == 'C')
      {
        // manual
      }
      else if(customKey == '*')
      {
        //not change
      }
      else
      {
        
        realValueSecond = realValueSecondFloat = 0;
               
        saveAllValue[posX] = int(customKey);
        saveAllValue[posX] -= 48;
        inputUserHarga += String(customKey);
        //lcd.setCursor(15-posX, 1);
        //lcd.print(inputUserHarga);
  
        for(int i=posX; i>=0; i--)
        { 
          realValueSecondFloat += saveAllValue[i] * pow(10,posX-i);  
        }
        realValueSecondFloat += 0.5;
        realValueSecond = realValueSecondFloat;

        stopHargaPompa = realValueSecond;
        
        posX++;
      }
    }
goto start;

}

