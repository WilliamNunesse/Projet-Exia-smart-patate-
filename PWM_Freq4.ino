//****************************************************************************************
// PWM Square Wave Generator
// VLO 2013-2014
//****************************************************************************************
  int test = 0; //Ici, on initialise la condition test que l'on va utiliser pour allumer la LED au cours du premier test.
  int test2 = 0; //Ici, c'est le second test (2 doigts).
  int test3 = 0; //Le troisième test (main complète).
  int test4 = 0;




const int outPin = 9; // output pin
const float maxFreq = 16000000; //max freq supported by Arduino (16MHz)
const int LED=2; //On définit la broche sur laquelle est branchée la première LED (ici la verte).
const int LED2=4; // Ici, c'est la broche utilisée par la seconde LED (jaune).
const int LED3=6; // Ici, c'est la broche utilisée par la dernière LED (rouge).
const int LED4=11;


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
         default:  TCCR1B=0b00011000;
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
    //Prescale  Time per counter tick
    //1         0.0625 uS
    //8         0.5 uS
    //64  4 uS
    //256 16 uS
    //1024  64uS
    
    OCR1A=v/2; //Output Compare Register //low state

  }
  
  
}


void setup()
{

  Serial.begin(115200); //for sending A0 pin values
  
  pinMode(outPin,OUTPUT);        //Signal generator pin
  
  pinMode(LED,OUTPUT); //On définie la LED comme étant une sortie.
  pinMode(LED2,OUTPUT); //Il est est de même pour la LED2.
  pinMode(LED3,OUTPUT); //Tout comme la LED3.
  pinMode(LED4,OUTPUT);

  setFrequency(200000); //generate a square wave (in this case at 1000Hz)
  
  digitalWrite(LED,LOW); //Ici, étant donné que la LED est configuré en sortie, 
  digitalWrite(LED2,LOW); //On utilise cette fonction pour dire si la LED est à un état
  digitalWrite(LED3,LOW); //Haut ou bas. Ici, c'est l'état bas, la LED est éteinte.
  digitalWrite(LED4,LOW);


  
  //for test
  //Serial.println();
  //Serial.println(ICR1);
  //Serial.println(TCCR1B);


}

void loop()
{

  int val = analogRead(0);                                              
  Serial.write( 0xff );                                                         
  Serial.write( (val >> 8) & 0xff );                                            
  Serial.write( val & 0xff );
  
  Serial.println(""); //La Fct Serial.println affiche les données sur le port série
  Serial.println(val); //Ici, cela permet d'envoyer une valeur sous forme d'octet simple
  Serial.println(test); //Affiche test en tant que nbr decimal sous la forme d'une chaîne de caractère ASCII suivi d'un retour de chariot et d'un saut de ligne.
  Serial.println(test2); //C'est la même chose pour test2
  Serial.println(test3); //Tout comme test3.
  Serial.println(test4);


 
 
if (val<1600)
 {
   test=test+1;
 }
              
 else
 { 
   digitalWrite(LED,LOW);
   test=0;
 }
              
 if (val<1600 && test>50)
 {
   digitalWrite(LED,HIGH);
 }

 else digitalWrite(LED,LOW);



 if (val<275)
 {
   test4=test4+1;
 }
              
 else
 { 
   digitalWrite(LED4,LOW);
   test4=0;
 }
              
 if (val<275 && test>50)
 {
   digitalWrite(LED4,HIGH);
 }

 else digitalWrite(LED4,LOW);
 

if (val<225)
{
  test2=test2+1;
}

else
{
  digitalWrite(LED2,LOW);
  test2=0;
}

if (val<225 && test>50)
{
  digitalWrite(LED2,HIGH);
}

else digitalWrite(LED2,LOW);



if (val<100)
{
  test3=test3+1;
}

else
{
  digitalWrite(LED3,LOW);
  test3=0;
}

if (val<100 && test>50)
{
  digitalWrite(LED3,HIGH);
}

else digitalWrite(LED3,LOW);





              
}

//set frequency (Hz)
//min frequency: 0.24 Hz
//max frequency: 8MHz

