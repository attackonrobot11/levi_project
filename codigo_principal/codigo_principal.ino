//Attack on Robot
//GIGACHADS: Vinicius de Paulo Almeida, Isabel Oliveira Trindade, Arthur Afonso Guimaraes, Ana Carolina Orlandi Paiva
//Data: 12/05/2022

//variaveis do radar
#define PIN_TRIG 2
#define PIN_ECHO 4

#define PIN_V1_M1 12
#define PIN_V2_M1 3
#define PIN_V1_M2 5
#define PIN_V2_M2 7

#define PIN_PWM_M1 6
#define PIN_PWM_M2 10

//variaveis dos sensores de linha
#define VEL_SOM_MS 340.29
#define BEEP_DIST_CM 15

int pesoSensores[4] = { -30, -5, 5, 30};
int pinoSensores[4] = { A0, A1, A2, A3};

int VelMC = 150; //Velocidade media do carrinho
int VelMM1 = 0; //Velocidade media do motor 1
int VelMM2 = 0; //Velocidade media do motor 2
int Kp = 1.3; //Constante escolhida
int P = 0; //
int error = 0; //
int error_now = 0; //
int sensores_no_preto = 0; //


void setup()
{
  //Configurações da entrada

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  pinMode(PIN_V1_M1, OUTPUT);
  pinMode(PIN_V2_M1, OUTPUT);
  pinMode(PIN_V1_M2, OUTPUT);
  pinMode(PIN_V2_M2, OUTPUT);

  pinMode(PIN_PWM_M1, OUTPUT);
  pinMode(PIN_PWM_M2, OUTPUT);

  for (int i = 0; i < 4; i++)
  {
    pinMode(pinoSensores[i], INPUT);
  }


  Serial.begin(9600);
  digitalWrite(PIN_TRIG, LOW);

  digitalWrite(PIN_V1_M1, LOW);
  digitalWrite(PIN_V2_M1, LOW);
  digitalWrite(PIN_V1_M2, LOW);
  digitalWrite(PIN_V2_M2, LOW);



}

void loop()
{

  // Disparar o pulso

  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  //Recebe o tempo ComandanteSmith

  unsigned int duracao_us = pulseIn(PIN_ECHO, HIGH);

  // Calculo da distancia

  float dist_m = VEL_SOM_MS * duracao_us * 1e-6 * 0.5;
  float dist_cm = dist_m * 100;
  Serial.print(" Distancia em cm ");
  Serial.println(dist_cm);

  if (dist_cm < BEEP_DIST_CM) {
    digitalWrite(PIN_V1_M1, LOW);
    digitalWrite(PIN_V2_M1, LOW);
    digitalWrite(PIN_V1_M2, LOW);
    digitalWrite(PIN_V2_M2, LOW);
  }

  else  {

  //Calculos dos sensores de linha
  error = 0;
  error_now = 0;
  sensores_no_preto = 0;
  for (int i = 0; i < 4; i++)
  {

    int read_value = digitalRead(pinoSensores[i]);
    if (read_value == 0)
    {
      error_now += pesoSensores[i];

    }
    else
    {
      sensores_no_preto++;
    }

  }

  if (sensores_no_preto > 0)
  {
    error = error_now / sensores_no_preto;
  }

  P = error * Kp;
  VelMM1 = VelMC + P;
  VelMM2 = VelMC - P;


//Limite de velocidade para nao queimar o motor
  if (VelMM1 > 255)
  {
    VelMM1 = 255;
  }
  if (VelMM1 < -225)
  {
    VelMM1 = -225;
  }

  if (VelMM2 > 255)
  {
    VelMM2 = 255;
  }
  if (VelMM2 < -255)
  {
    VelMM2 = -255;
  }



  if (VelMM1 > 0)
  {
    digitalWrite(PIN_V1_M1, HIGH);
    digitalWrite(PIN_V2_M1, LOW);
    analogWrite(PIN_PWM_M1, VelMM1);
  }
  else
  {
    digitalWrite(PIN_V1_M1, LOW);
    digitalWrite(PIN_V2_M1, HIGH);
    analogWrite(PIN_PWM_M1, abs(VelMM1));
  }

  if (VelMM2 > 0)
  {
    digitalWrite(PIN_V1_M2, HIGH);
    digitalWrite(PIN_V2_M2, LOW);
    analogWrite(PIN_PWM_M2, VelMM2);
  }
  else
  {
    digitalWrite(PIN_V1_M2, LOW);
    digitalWrite(PIN_V2_M2, HIGH);
    analogWrite(PIN_PWM_M2, abs(VelMM2));
  }  
 }

  
  delay(250);



}
