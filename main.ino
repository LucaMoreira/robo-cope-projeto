/*
 * Projeto de Jogo feito com arduino
 * Contribuente: Luca    Moreira
 * Mentor:       Juliano Miranda
 * Escola:       COPE
 * Obs: 
 *    E permitido o uso ou modificacao livre deste codigo 
 *    contanto que se mantenha este header
 */

// inclui bibliotecas usadas
#include <LiquidCrystal.h>
#include <Servo.h>

// declara portas usadas
// obs: faz se o uso de variaveis para maior dinamicidade
const int lcd_rs  = 12;
const int lcd_e   = 8;
const int lcd_bd4 = 5;
const int lcd_bd5 = 4;
const int lcd_bd6 = 3;
const int lcd_bd7 = 2;

const int potX    = A0;
const int potY    = A1;

const int sX      = 6;
const int sY      = 7;

const int led_G   = 9;
const int led_B   = 10;
const int led_R   = 11;

const int bzz     = 13;
const int btt     = 1;

// declara os servos
Servo servo_X; 
Servo servo_Y;

// configura o lcd com suas respectivas portas
LiquidCrystal lcd(lcd_rs, lcd_e, lcd_bd4, lcd_bd5, lcd_bd6, lcd_bd7);

// declara variaveis para armazenar a leitura do potX e do potY
int l_potX = 0;
int l_potY = 0;

void setup()
{
  // configura as portas do led
  pinMode(led_R, OUTPUT);
  pinMode(led_G, OUTPUT);
  pinMode(led_B, OUTPUT);
  
  // inicia o lcd com o numero de linhas e colunas
  lcd.begin(16, 2);

  // configura a porta do potX e servo_X
  pinMode(potX, INPUT);
  servo_X.attach(sX, 500, 2500);

  // configura a porta do potY e servo_Y
  pinMode(potY, INPUT);
  servo_Y.attach(sY, 500, 2500);

  // configura a porta do bottao
  pinMode(btt, INPUT);

  // configura a porta do buzzer
  pinMode(bzz, OUTPUT);
}

void loop()
{
  // requerimentos para o jogo
  pre_jogo();

  // testa leituras e inicia jogo caso motores em 511
  jogo();
  
  // espera um pouco para melhorar a performace da simulacao
  delay(10);
}

void vermelho() {
  // acende vermelho
  analogWrite(led_R, 255);
  analogWrite(led_G, 0);
  analogWrite(led_B, 0);
}

void verde() {
  // acende verde
  analogWrite(led_R, 0);
  analogWrite(led_G, 255);
  analogWrite(led_B, 0);
}

void atualiza_leituras() {
  // atualiza as leituras basedo nas entradas
  l_potX = analogRead(potX);
  l_potY = analogRead(potY);
}

void move_servos() {
  // move os servos baseado nas leituras
  servo_X.write(map(l_potX, 0, 1023, 0, 180));
  servo_Y.write(map(l_potY, 0, 1023, 0, 180));
}

void mostra_leituras() {
  // printa as leituras
  lcd.setCursor(3,1);
  lcd.print(l_potX);
  lcd.print(" ");
  
  lcd.setCursor(9,1);
  lcd.print(l_potY);
  lcd.print(" ");

  // guia o usuario
  lcd.setCursor(0,0);
  lcd.print("Posicione em 511");
}

void som() {
  //sinal sonoro
  tone(bzz, 1000);
  delay(1000);
  noTone(bzz); 
}

void contagem() {
  // contagem regressiva
  for (int i = 3; i >= 0; i--) {
    lcd.setCursor(7,1);
    lcd.print(i);
    delay(500);
  }
}

void pre_jogo() {
  // acende o led na cor vermelha
  vermelho();
  
  // atualiza 
  atualiza_leituras();

  // move 
  move_servos();

  // printa
  mostra_leituras();

  // atualiza
  atualiza_leituras();
}

void jogo() {
  // testa se servos estao na posicao certa
  if (l_potX == 511 && l_potY == 511) {
    // limpa o lcd
    lcd.clear();

    // avisa a espera
    lcd.setCursor(0,0);
    lcd.print("Esperando");
    
    // espera um pouco 
    // para dar tempo para o motor ir a posicao
    delay(700);

    // limpa o lcd
    lcd.clear();

    //som
    som();

    //acende o led na cor verde
    verde();

    // avisa inicio
    contagem();

    // limpa lcd
    lcd.clear();
    
    // testa se btt nao esta apertado
    while (!(digitalRead(btt) == 1)) {
      // avisa o inicio
      lcd.setCursor(0,0);
      lcd.print("Valendo!");
      
      // atualiza
      atualiza_leituras();

      // move
      move_servos();
    }
    // limpa o lcd
    lcd.clear();
    
    // anuncia fim
    lcd.setCursor(0,0);
    lcd.print("Fim!");
   
    // acende vermelho
    vermelho();
    
    // som
    som();
  }
}
