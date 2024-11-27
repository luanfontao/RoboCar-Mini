#include <NewPing.h>
#include <Servo.h>

// ------- CONSTANTES E PINOS ------- //
#define PIN_TRIGGER_DIREITO 8
#define PIN_ECHO_DIREITO 2
#define PIN_TRIGGER_ESQUERDO 3
#define PIN_ECHO_ESQUERDO 4
#define MAX_DISTANCIA 100

const int pinMotorDireitoVelocidade = 11;
const int pinMotorDireitoFrente = 12;
const int pinMotorDireitoRe = 13;
const int pinMotorEsquerdoVelocidade = 5;
const int pinMotorEsquerdoFrente = 6;
const int pinMotorEsquerdoRe = 7;

const int pinServo = 9; // Pino do servo motor
int anguloServo = 30;   // Ângulo inicial do servo
int incrementoServo = 10;

// ------- VARIÁVEIS ------- //
int velocidadeMotorDireito = 175 ;
int velocidadeMotorEsquerdo = 220;

// Objetos para sensores e servo motor
NewPing sensorDireito(PIN_TRIGGER_DIREITO, PIN_ECHO_DIREITO, MAX_DISTANCIA);
NewPing sensorEsquerdo(PIN_TRIGGER_ESQUERDO, PIN_ECHO_ESQUERDO, MAX_DISTANCIA);
Servo servo;

// ------- FUNÇÕES ------- //
void moverParaFrente();
void pararCarrinho();
void girarParaDireita();
void girarParaEsquerda();
void virar90Graus(bool direita);
long lerDistancia(NewPing sensor);

void setup() {
    // Configuração dos pinos dos motores
    pinMode(pinMotorDireitoVelocidade, OUTPUT);
    pinMode(pinMotorDireitoFrente, OUTPUT);
    pinMode(pinMotorDireitoRe, OUTPUT);
    pinMode(pinMotorEsquerdoVelocidade, OUTPUT);
    pinMode(pinMotorEsquerdoFrente, OUTPUT);
    pinMode(pinMotorEsquerdoRe, OUTPUT);

    // Configuração inicial dos motores
    analogWrite(pinMotorDireitoVelocidade, velocidadeMotorDireito);
    analogWrite(pinMotorEsquerdoVelocidade, velocidadeMotorEsquerdo);

    // Configuração do servo motor
    servo.attach(pinServo);
    servo.write(anguloServo);

    // Comunicação serial para depuração
    Serial.begin(9600);
}

void loop() {
    // Lê distâncias dos sensores
    long distanciaDireita = lerDistancia(sensorDireito);
    long distanciaEsquerda = lerDistancia(sensorEsquerdo);

    // Depuração: Imprime as distâncias
    Serial.print("Direita: ");
    Serial.print(distanciaDireita);
    Serial.print(" cm | Esquerda: ");
    Serial.println(distanciaEsquerda);

    // Lógica de movimento
    if (distanciaDireita > 20 && distanciaEsquerda > 20) {
        moverParaFrente();
    } else if (distanciaDireita <= 20 && distanciaEsquerda > 20) {
        virar90Graus(false); // Vira para a esquerda
    } else if (distanciaEsquerda <= 20 && distanciaDireita > 20) {
        virar90Graus(true);  // Vira para a direita
    } else {
        pararCarrinho();
    }

    // Movimento do servo motor
    servo.write(anguloServo);
    delay(100);
    anguloServo += incrementoServo;

    // Inverte direção do servo ao atingir os limites
    if (anguloServo >= 90 || anguloServo <= 30) {
        incrementoServo = -incrementoServo;
    }
}

// Lê distância de um sensor ultrassônico
long lerDistancia(NewPing sensor) {
    long distancia = sensor.ping_cm();
    if (distancia == 0) {
        distancia = MAX_DISTANCIA;
    }
    return distancia;
}

// Função que move o carrinho para frente
void moverParaFrente() {
    digitalWrite(pinMotorDireitoFrente, HIGH);
    digitalWrite(pinMotorDireitoRe, LOW);
    digitalWrite(pinMotorEsquerdoFrente, LOW);
    digitalWrite(pinMotorEsquerdoRe, HIGH);
}

// Função que para o carrinho
void pararCarrinho() {
    digitalWrite(pinMotorDireitoFrente, LOW);
    digitalWrite(pinMotorDireitoRe, LOW);
    digitalWrite(pinMotorEsquerdoFrente, LOW);
    digitalWrite(pinMotorEsquerdoRe, LOW);
}

// Função para virar o carrinho para um lado (90 graus)
void virar90Graus(bool direita) {
    pararCarrinho();
    delay(500); // Pausa antes de girar

    if (direita) {
        // Gira para a direita
        digitalWrite(pinMotorDireitoFrente, LOW);
        digitalWrite(pinMotorDireitoRe, HIGH);
        digitalWrite(pinMotorEsquerdoFrente, LOW);
        digitalWrite(pinMotorEsquerdoRe, HIGH);
    } else {
        // Gira para a esquerda
        digitalWrite(pinMotorDireitoFrente, HIGH);
        digitalWrite(pinMotorDireitoRe, LOW);
        digitalWrite(pinMotorEsquerdoFrente, HIGH);
        digitalWrite(pinMotorEsquerdoRe, LOW);
    }

    delay(700); // Tempo necessário para completar o giro
    pararCarrinho();
}
