//****************************************************************************************
// Illutron take on Disney style capacitive touch sensor using only passives and Arduino
// Dzl 2012
//****************************************************************************************


//                              10n
// PIN 9 --[10k]-+-----10mH---+--||-- OBJECT
//               |            |
//              3.3k          |
//               |            V 1N4148 diode
//              GND           |
//                            |
//Analog 0 ---+------+--------+
//            |      |
//          100pf   1MOmhm
//            |      |
//           GND    GND



#define SET(x,y) (x |=(1<<y))				//-Bit set/clear macros
#define CLR(x,y) (x &= (~(1<<y)))       		// |
#define CHK(x,y) (x & (1<<y))           		// |
#define TOG(x,y) (x^=(1<<y))            		//-+



#define N 160  //How many frequencies

float results[N];            //-Filtered result buffer
float freq[N];            //-Filtered result buffer
int sizeOfArray = N;
const int LED_RED = 3;    //LED rouge sur la sortie 3
const int LED_GREEN = 7; 
const int LED_YELLOW = 5;

 
   
   

void setup()
{
  
  
  TCCR1A=0b10000010;        //-Set up frequency generator
  TCCR1B=0b00011001;        //-+
  ICR1=110;
  OCR1A=55;

  pinMode(9,OUTPUT);        //-Signal generator pin
  pinMode(8,OUTPUT);        //-Sync (test) pin

  pinMode(LED_RED, OUTPUT); //Définition du pin 3 en sortie
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);

  Serial.begin(115200);

  for(int i=0;i<N;i++)      //-Preset results
    results[i]=0;         //-+
   
  for(int i=0;i<N;i++)      //-Preset results
    freq[i]=i;         //-+
}

void loop()
{
  unsigned int d;

  int indice_max;
  
  indice_max=0;;

  int counter = 0;
  for(unsigned int d=0;d<N;d++)
  {
    int v=analogRead(1);    //-Read response signal
    CLR(TCCR1B,0);          //-Stop generator
    TCNT1=0;                //-Reload new frequency
    ICR1=d;                 // |
    OCR1A=d/2;              //-+
    SET(TCCR1B,0);          //-Restart generator

    results[d]=results[d]*0.5+(float)(v)*0.5; //Filter results
    //Serial.println(TCCR1B);
    //Serial.println(v);
    if (results[d] > results[indice_max]) //Test si la valeur que l'on vient d'ajouter est le maximum
      {
        indice_max = d;   //Sinon elle est plus grande que le maximum cette valeur devient le nouveau maximum
      }
 //   plot(v,0);              //-Display
 //   plot(results[d],1);
  // delayMicroseconds(1);

  
  }
  


    PlottArray(1,freq,results);
 

  TOG(PORTB,0);            //-Toggle pin 8 after each sweep (good for scope)


    if(indice_max >= 67 && indice_max <= 85)
    {
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_YELLOW, LOW);
    }
    if(indice_max < 53)
    {
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_YELLOW, LOW);
      digitalWrite(LED_GREEN, HIGH);
    }
    if(indice_max <= 67 && indice_max >= 53)
    {
       digitalWrite(LED_RED, LOW);
       digitalWrite(LED_GREEN, LOW);
       digitalWrite(LED_YELLOW, HIGH);
    }
    if(indice_max > 85)
    {
       digitalWrite(LED_RED, HIGH);
       digitalWrite(LED_GREEN, HIGH);
       digitalWrite(LED_YELLOW, HIGH);
    }

}
   
