/**********************************************************
* UNIVERSIDADE FEDERAL DE PERNAMBUCO - UFPE
* CENTRO DE INFORMARICA - CIN
* INTRODUCAO A COMPUTACAO - IF668 EC
* CODIGO PARA LINEFOLLOWER
* TRABALHO DE CONCLUSÃO DA DISCIPLINA
* GRUPO 8
* INTEGRANTES:  MATHEUS BATISTA DE ARAÚJO (mba6); CIBELLE SOUSA RODRIGUES (csr2); EROS SANTOS VASCONCELOS (esv); PEDRO JORGE LIMA DA SILVA(pjls2); 
* LEONARDO GABRIEL MOREIRA DE OLIVEIRA (lgmo)
*
************************************************* *******/

/* ------------------- SENSOR ULTRASSÔNICO ------------------- */
#include "Ultrasonic.h"         
#define ECHO_PIN 10             
#define TRIG_PIN 8              
int distancia=0;                  
Ultrasonic ultrasonic(8, 10);   
/* ------------------------------------------------------------ */


/* --------------------- PONTE H MOTORES ---------------------- */
#define MOTOR_E1 6
#define MOTOR_E2 7
#define MOTOR_EV 9
#define MOTOR_DV 3
#define MOTOR_D2 4
#define MOTOR_D1 5
#define VELOCIDADE_MIN 40         
#define VELOCIDADE_MAX 70         
#define VELOCIDADE_MIN_OBST 40    
#define VELOCIDADE_MAX_OBST 70    
/* ------------------------------------------------------------ */


/* ----------------------- SENSOR OPTICO ---------------------- */
#define IN_PIN 2                // Pino de entrada
#define COR_LINHA LOW          // Define para cor da linha (BRANCO -> HIGH; PRETO -> LOW)
/* ------------------------------------------------------------ */


/* ---------------------------- LEDS -------------------------- */
#define LED_DIREITA 11    
#define LED_ESQUERDA 12   
#define LED_OBSTACULO 13  
/* ------------------------------------------------------------ */

//DECLARAÇÕES DAS VARIÁVEIS

int flagObstaculo = 1;    // Variavel que indica se o obstaculo ja desviado - > é 0 se ja foi

//DECLARAÇÕES DAS FUNÇÕES
int lerDistancia();       // Lê distancia entre carrinho e o obstaculo
void checkObstaculo();    // Usada para checar se existe obstaculo na frente

void para();              // Para os motores
void andaEsquerda();      // Move o carrinho para esquerda ate que encontre a linha 
void andaDireita();       // Move o carrinho para direita ate que encontre a linha
void segueLinha();        // Executa rotina de seguir linha
void MOTOE_frente(int vel); // Ativa o motor da Esquerda para a frente e zera a velocidade do motor da Esquerda
void MOTOD_frente(int vel); // Ativa o motor da Direita para a frente e zera a velocidade do motor da Direita
void andaEsquerda();        // função que controla o led e chama o motor da direita para virar pra esquerda 
void andaDireita();         // função que controla o led e chama o motor da esquerda para virar pra direita


void desvia();            // Move o carrinho para desviar o obstaculo
/* ------------------------------------------------------------ */


void setup() {
  // Define os pinos como saida do motor
  pinMode(MOTOR_E1, OUTPUT);
  pinMode(MOTOR_E2, OUTPUT);
  pinMode(MOTOR_EV, OUTPUT);
  pinMode(MOTOR_DV, OUTPUT);
  pinMode(MOTOR_D2, OUTPUT);
  pinMode(MOTOR_D1, OUTPUT);

  // Sensor óptico
  pinMode(IN_PIN, INPUT);

  // Sensor Ultrassonico
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);

  // Leds
  pinMode(LED_DIREITA, OUTPUT);
  pinMode(LED_ESQUERDA, OUTPUT);
  pinMode(LED_OBSTACULO, OUTPUT);
}

void loop() {
  segueLinha();
}


/* ------------------------------- FUNÇÕES ----------------------------- */

void segueLinha(){
  if(digitalRead(IN_PIN) == COR_LINHA){
    andaEsquerda();
    delay(10);
    checkObstaculo();
  }else{
    andaDireita();
    delay(10);
    checkObstaculo();
  }
}


// Função que ler a distancia do carrinho ao obstaculo
int lerDistancia(){
  digitalWrite(TRIG_PIN, LOW);    
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);             // Emite um pulso sonoro (indica o incio da transmissao de dados)
  delayMicroseconds(10);                
  digitalWrite(TRIG_PIN, LOW);              // Para a emissão
  distancia = (ultrasonic.Ranging(CM));     // A variavel distancia recebe o valor lido pelo sensor (distancia = (tempo echo em nivel alto*velocidade do som)/2)
  return distancia;                         // Retorna a distancia lida em centímetros
}

void checkObstaculo(){
  if (flagObstaculo) {                        // Verifica se o obstaculo ja foi desviado    
    distancia = lerDistancia();               // Chama a função 'lerDistancia' que verifica a distancia entre o carrinho e o obstaculo
  }
  if (distancia <= 10 && flagObstaculo) {     // Verifica se a distancia lida é menor ou igual a 10cm e se o obstaculo ja foi desviado
    digitalWrite(LED_OBSTACULO, HIGH);
    digitalWrite(LED_OBSTACULO, LOW);
    digitalWrite(LED_OBSTACULO, LOW);
    desvia();                                 // Chama a função para desviar o obstaculo    
  }
}


////////////////////////////////////////// movimentos seguir linha //////////////////////////////////////////////////

// Função que para a movimentação dos motores
void para() {
  analogWrite(MOTOR_EV, 0);
  analogWrite(MOTOR_DV, 0);
  digitalWrite(MOTOR_E1, HIGH);
  digitalWrite(MOTOR_E2, HIGH);
  digitalWrite(MOTOR_D1, HIGH);
  digitalWrite(MOTOR_D2, HIGH);
}


void MOTOE_frente(int vel){
    analogWrite(MOTOR_DV, 0);
    analogWrite( MOTOR_EV, vel); 
    digitalWrite( MOTOR_E1, HIGH);
    digitalWrite( MOTOR_E2, LOW);
    digitalWrite( MOTOR_D1, LOW);
    digitalWrite( MOTOR_D2, HIGH);
}

void MOTOD_frente(int vel){
    analogWrite(MOTOR_EV, 0);
    analogWrite( MOTOR_DV, vel); 
    digitalWrite( MOTOR_D1, LOW);
    digitalWrite( MOTOR_D2, HIGH);
    digitalWrite( MOTOR_E1, HIGH);
    digitalWrite( MOTOR_E2, LOW);
}


void andaEsquerda(){
  digitalWrite(LED_DIREITA, HIGH);
  digitalWrite(LED_ESQUERDA, LOW);
  MOTOD_frente(120);
}

void andaDireita(){
  digitalWrite(LED_DIREITA, LOW);
  digitalWrite(LED_ESQUERDA, HIGH);
  MOTOE_frente(60);
}



////////////////////////////////////////////////////////// desviar objeto /////////////////////////////////////////////////////////////


void desvia() {
  unsigned long tempo = millis();     // Variavel que recebe o tempo de execução da função em millisegundos

  // Giro para Direita enquanto o tempo for menor que 1800ms
  while (millis() - tempo < 1800){
    andaDireita();
  } ;


  // Para o carrinho durante 1000ms
  tempo = millis();
  while (millis() - tempo < 1000){
    para();
    digitalWrite(LED_OBSTACULO, LOW);
  };

  // Movimento para frente durente 1200ms
  tempo = millis();
  while (millis() - tempo < 1200){
    analogWrite(MOTOR_DV, 120);
    analogWrite( MOTOR_EV, 70); 
    digitalWrite( MOTOR_E1, HIGH);
    digitalWrite( MOTOR_E2, LOW);
    digitalWrite( MOTOR_D1, LOW);
    digitalWrite( MOTOR_D2, HIGH);
  };

  // Para o carrinho durante 150ms
  tempo = millis();
  while (millis() - tempo < 1000){
    para();
    digitalWrite(LED_OBSTACULO, LOW);
  };

  // Giro para Esquerda durante 200ms
  tempo = millis();
  while (millis() - tempo < 1000){
    digitalWrite(LED_OBSTACULO, HIGH);
    andaEsquerda();
  };

  // Para o carrinho durante 150ms
  tempo = millis();
  while (millis() - tempo < 1000){
    para();
    digitalWrite(LED_OBSTACULO, LOW);
  };

  // Movimento para frente durante 700ms
  tempo = millis();
  while (millis() - tempo < 1200){
    analogWrite(MOTOR_DV, 110);
    analogWrite( MOTOR_EV, 70); 
    digitalWrite( MOTOR_E1, HIGH);
    digitalWrite( MOTOR_E2, LOW);
    digitalWrite( MOTOR_D1, LOW);
    digitalWrite( MOTOR_D2, HIGH);
  };

  // Para o carrinho durante 150ms
  tempo = millis();
  while (millis() - tempo < 1000){
    para();
    digitalWrite(LED_OBSTACULO, LOW);
  };

  // Giro para Esquerda durante 150ms
  tempo = millis();
  while (millis() - tempo < 1000){
    andaEsquerda();
  };

  // Para o carrinho durante 150ms
  tempo = millis();
  while (millis() - tempo < 1000){
    para();
    digitalWrite(LED_OBSTACULO, LOW);
  } ;

  // Movimenta para frente até o sensor optico encontre o linha preta
  while (digitalRead(IN_PIN) == LOW){
    analogWrite(MOTOR_DV, 100);
    analogWrite( MOTOR_EV, 65); 
    digitalWrite( MOTOR_E1, HIGH);
    digitalWrite( MOTOR_E2, LOW);
    digitalWrite( MOTOR_D1, LOW);
    digitalWrite( MOTOR_D2, HIGH);
  };
  delay(200);


  // Para o carrinho durante 150ms
  tempo = millis();   //Reinicia o tempo
  while (millis() - tempo < 1000){
    para();
    digitalWrite(LED_OBSTACULO, LOW);
  };

  // Giro para Direita ate que o sensor optico encontre a linha preta
  while (digitalRead(IN_PIN) == LOW){
    andaDireita();
  } ;
   
  para();                             // Para o carrinho 
  digitalWrite(LED_OBSTACULO, LOW);   //Desliga o led(indicando o fim do desvio)
  delay(50);                          // Espera 50ms
  flagObstaculo = 0;                  // Zera a variavel 'flagObstaculo', indicando que o carrinho já realizou o desvio de obstaculo e não permitindo que o faça novamente, evitando desvios acidentais durante o resto do percuso
}
