//****************************************************************************************
// PWM Square Wave Generator
// VLO 2013-2014
//****************************************************************************************

int GetCas(float Tens);
void RemplirValeurs(int valeur); 
void setFrequency(float frequency);
const int outPin = 9; // output pin
const float maxFreq = 16000000; //max freq supported by Arduino (16MHz)
const int TaillTabl = 100;
int Valeurs[TaillTabl];

void setup()
{

  Serial.begin(115200); //for sending A0 pin values
  pinMode(outPin,OUTPUT);        //Signal generator pin
  pinMode(5,OUTPUT); 
  pinMode(4,OUTPUT);
  pinMode(6,OUTPUT); 

  
  for(int i=0; i<TaillTabl; i++)
    Valeurs[i]=0;

  
  setFrequency(350000); //generate a square wave (in this case at 1000Hz)
  
  //for test
  //Serial.println();
  //Serial.println(ICR1);
  //Serial.println(TCCR1B);
}

void loop()
{

  
  int valeur = analogRead(0); 
  
  RemplirValeurs(valeur);
  int miin = Min();
  int maax = Max();
  float TensCreteCrete = (((float)maax - (float)miin) /1023) * 5;
  int cas = GetCas(TensCreteCrete);
 
  Serial.println(TensCreteCrete);
  
  switch(cas)
  {
    case 1:
    digitalWrite(5,HIGH);
    digitalWrite(4,LOW);
    digitalWrite(6,LOW);
    break;

    case 2:
    digitalWrite(5,LOW);
    digitalWrite(4,HIGH);
    digitalWrite(6,LOW);
    break;


    case 3:
    digitalWrite(5,LOW);
    digitalWrite(4,LOW);
    digitalWrite(6,HIGH);
    break;

    case 4:
    digitalWrite(5,HIGH);
    digitalWrite(4,HIGH);
    digitalWrite(6,HIGH);
    break;


    default:
    digitalWrite(5,LOW);
    digitalWrite(4,LOW);
    digitalWrite(6,LOW);
    break;
  }
  

    /*/                            
  Serial.write( 0xff );                                                         
  Serial.write( (valeur >> 8) & 0xff );                                            
  Serial.write( valeur & 0xff );
 //*/
}

//set frequency (Hz)
//min frequency: 0.24 Hz
//max frequency: 8MHz
void setFrequency(float frequency)
{
 
  if(frequency >=0.24 || frequency <=8000000)
  {
    
    TCCR1A=0b10000010;        // Set up frequency generator, mode 14 (Fast PWM)
    //WGM11 = 1, COM1A1 = 1
    //Fast PWM: TOP=ICR1, update of OCR1x=BOTTOM, TOV1 Flag Set on TOP
    
  
    unsigned int v=0;
    int indscale=0;
    float prescale[] = {1.0,8.0,64.0,256.0,1024.0};
    float period=1/frequency;
    
    while(v==0)
    {
      float curfreq=maxFreq/prescale[indscale];
      float tickperiod=1/curfreq;
      float nbtick=period/tickperiod;
     
      if(nbtick>65535)
      {
        indscale=indscale+1;
      }
      else
      {
        v=nbtick;
      }

    }
    
    int c=prescale[indscale];
    
     switch (c) {
      
         case 1: TCCR1B=0b00011001; break;
         case 8: TCCR1B=0b00011010; break;
         case 64: TCCR1B=0b00011011; break;
         case 256: TCCR1B=0b00011100; break;
         case 1024: TCCR1B=0b00011101; break;
         default: TCCR1B=0b00011000;
     }
    
    //WGM12 = 1, WGM13 = 1
    
    //three last bit of TCCR1B:    CS12   CS11   CS10
    // 0: no clock (timer stopped)  0      0      0
    // clk/1: no prescaling         0      0      1 
    // clk/8                        0      1      0
    // clk/64                       0      1      1
    // clk/256                      1      0      0
    // clk/1024                     1      0      1

    
    ICR1=v; //pulse duration = ICR1 value x time per counter tick
    
    //for 16Mhz (chip frequency)
    //Prescale	Time per counter tick
    //1	        0.0625 uS
    //8	        0.5 uS
    //64	4 uS
    //256	16 uS
    //1024	64uS
    
    OCR1A=v/2; //Output Compare Register //low state

  }
  
  
}


int GetCas(float Tens)
{
int i = 0;
  if(Tens < 2.8)
    if(Tens < 1.1)
        if(Tens < 0.7)
          if(Tens < 0.4)
            i = 4;
          else 
            i = 3;
         else 
        i = 2;
    else 
    i = 1;
  else 
  i = 0;

  return i;

}

void RemplirValeurs(int valeur)
{
 int i;
 for(i=0;i<TaillTabl-1;i++)
 {
  Valeurs[i] = Valeurs[i+1];
 }
 Valeurs[TaillTabl-1]=valeur;
}

int Min()
{
 int MinActuel = Valeurs[0];
 int i;
 for(i=1;i<TaillTabl;i++)
 {
  if(MinActuel>Valeurs[i])
  {
   MinActuel=Valeurs[i]; 
  } 
 } 
 return MinActuel;
}

int Max()
{
int MaxActuel = Valeurs[0];
 int i;
 for(i=1;i<TaillTabl;i++)
 {
  if(MaxActuel<Valeurs[i])
  {
   MaxActuel=Valeurs[i]; 
  }
 } 
 return MaxActuel; 
}
  







