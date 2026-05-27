#include <LiquidCrystal.h>

// LCD
LiquidCrystal lcd(12, 11, 10, 9, 8, 4);

// LEDs e buzzer
int ledVerde = 7;
int ledVermelho = 6;
int buzzer = 5;

// Sensores
int sensorTemp = A0;
int sensorUmidade = A1;
int sensorLuz = A2;

void setup()
{
  // Serial monitor
  Serial.begin(9600);

  // LCD
  lcd.begin(16, 2);

  // Pinos
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Mensagem inicial
  lcd.setCursor(0,0);
  lcd.print("AgroVision");
  lcd.setCursor(0,1);
  lcd.print("Iniciando...");
  
  delay(2500);

  lcd.clear();
}

void loop()
{
  // =========================
  // LEITURA DOS SENSORES
  // =========================

  // TMP36
  int leituraTemp = analogRead(sensorTemp);

  float voltagem =
      leituraTemp * 5.0 / 1023.0;

  float temperatura =
      (voltagem - 0.5) * 100;

  // Potenciômetro = umidade
  int leituraUmidade =
      analogRead(sensorUmidade);

  int umidade =
      map(leituraUmidade,
          0, 1023,
          0, 100);

  // LDR
  int leituraLuz =
      analogRead(sensorLuz);

  // Suavização da luz
  leituraLuz = map(
      leituraLuz,
      0, 1023,
      0, 100
  );

  // Reset
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledVermelho, LOW);
  noTone(buzzer);

  lcd.clear();

  // =========================
  // ALERTA SATELITAL
  // =========================
  if (temperatura > 38 &&
      umidade < 40)
  {
    lcd.setCursor(0,0);
    lcd.print("SATELITE");

    lcd.setCursor(0,1);
    lcd.print("Seca prevista");

    digitalWrite(ledVermelho, HIGH);

    tone(buzzer, 1500);

    delay(2500);

    noTone(buzzer);

    lcd.clear();
  }

  // =========================
  // SOLO SECO
  // =========================
  if (umidade < 30)
  {
    digitalWrite(ledVermelho, HIGH);

    tone(buzzer, 800);

    lcd.setCursor(0,0);
    lcd.print("ALERTA SECA");

    lcd.setCursor(0,1);
    lcd.print("Irrigar agora");
  }

  // =========================
  // UMIDADE MEDIA
  // =========================
  else if (umidade >= 30 &&
           umidade <= 60)
  {
    lcd.setCursor(0,0);
    lcd.print("ATENCAO");

    lcd.setCursor(0,1);
    lcd.print("Pouca umidade");
  }

  // =========================
  // SOLO BOM
  // =========================
  else
  {
    digitalWrite(ledVerde, HIGH);

    lcd.setCursor(0,0);
    lcd.print("SOLO IDEAL");

    lcd.setCursor(0,1);
    lcd.print("Irrigacao OK");
  }

  delay(3000);

  // =========================
  // TEMPERATURA
  // =========================
  lcd.clear();

  if (temperatura > 38)
  {
    lcd.setCursor(0,0);
    lcd.print("ALTO CALOR");

    lcd.setCursor(0,1);
    lcd.print("Risco Agric.");
  }
  else
  {
    lcd.setCursor(0,0);
    lcd.print("Temp:");

    lcd.print(temperatura);
    lcd.print(" C");
  }

  delay(3000);

  // =========================
  // LUMINOSIDADE
  // =========================
  lcd.clear();

  if (leituraLuz < 35)
  {
    lcd.setCursor(0,0);
    lcd.print("Pouca Luz");

    lcd.setCursor(0,1);
    lcd.print("Verificar");
  }
  else
  {
    lcd.setCursor(0,0);
    lcd.print("Luz OK");

    lcd.setCursor(0,1);
    lcd.print("Plantacao");
  }

  delay(3000);

  // =========================
  // DADOS SERIAL
  // =========================
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" C");

  Serial.print("Umidade: ");
  Serial.print(umidade);
  Serial.println("%");

  Serial.print("Luz: ");
  Serial.println(leituraLuz);

  Serial.println("----------------");
}