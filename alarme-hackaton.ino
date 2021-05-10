//Autor: Marcos Roberto Fernandes Filho***
//Curso: Técnico Integrado em Eletrônica**
//Instituição: IFSC - Florianópolis*******


#define trigger 7// Saída Trigger do sonar
#define echo 8 // Entrada Echo do sonar
#define buzzer 13 // Saída do Buzzer
#define circuito 4 // Entrada do Botão para ligar ou desligar o circuito
#define alarme 2 // Entrada do Botão para desligar o alarme
#define R0 10 // Vermelho, Liga Desliga
#define G0 9 // Verde, Liga Desliga
#define B0 11 // Azul, Liga Desliga (Não Utilizado)
#define R1 5 // Vermelho, Alarme
#define G1 6 // Verde, Alarme (Não Utilizado)
#define B1 3 // Azul, Alarme
#define ANODO COMUM // Se o Led RGB for catodo comum esta linha deve ser comentada

//////// Digite um Valor em Centimetros ////////
#define distancia_limite 30 // Distância Limite
/////// Aceita Valores de 2cm até 400cm ///////

int tempo = 10; // A duração da nota que o buzzer irá tocar(ms)
int frequencia = 0; // Inicializa a frequência como 0
int duracao; // Varíavel que armazena a duração da onda
int distancia; // Variáveis para a distância que será calculada
int circuito_on = LOW; // Variável de controle para saber se o circuito foi ligado ou não
int arm_circuito = LOW; // Armazena o valor de circuito_on
int alarme_on = LOW; // Variável de controle pra saber se o alarme foi desativado ou não

void setup()
{
  Serial.begin(9600);
  pinMode(trigger, OUTPUT); // Define o pino Trigger como saída
  pinMode(echo, INPUT); // Define o pino Echo como entrada
  pinMode(buzzer, OUTPUT); // Define o pino Buzzer como saída
  pinMode(circuito, INPUT); // Define o pino para o botão de ligar o circuito
  pinMode(alarme, INPUT); // Define  o pino para o botão de desativar o alarme
  // LED's para saber se o CIRCUITO está On ou Off
  pinMode(R0, OUTPUT); // Cor vermelha do Led, Desligado
  pinMode(B0, OUTPUT); // Azul não utilizado
  pinMode(G0, OUTPUT); // Cor verde do Led, Ligado
  // LED's para saber se o ALARME está Ativado ou Desativado
  pinMode(R1, OUTPUT); // Cor vermelha do Led, Pessoa Detectada 
  pinMode(G0, OUTPUT); // Verde não utilizado
  pinMode(B1, OUTPUT); // Cor azul do Led, Nada Detectado 
}

void loop() {
  CorRgb(255,0,0,1); // Cor Vermelho, Circuito Desligado
  CorRgb(0,0,0,0); // Sem Cor, Alarme Desativado
  circuito_on = digitalRead(circuito); // Lê o pino "circuito" e armazena em "circuito_on"
  if (circuito_on == HIGH) // Se o o circuito for ligado
  {
    arm_circuito = !arm_circuito; // Inverte o valor de arm_circuito e entra no laço abaixo
    delay(200);
  }
  // Laço principal
  while(arm_circuito == HIGH) 
  {
    circuito_on = digitalRead(circuito); // Lê novamente o botão de controle do circuito
    if (circuito_on == HIGH) // Se o botão for apertado novamente
    {
      arm_circuito = !arm_circuito; // Inverte o valor de arm_circuito e sai do laço, desligando o circuito
      delay(200);
      break;
    }
    CorRgb(0,255,0,1); // Cor Verde, Circuito Ligado
    digitalWrite(trigger, HIGH); // Ativa a porta Trigger para enviar a onda
    delayMicroseconds(10); // Espera a onda
    digitalWrite(trigger, LOW); // Desativa a porta Trigger

    duracao = pulseIn(echo, HIGH);   // Calcula quanto tempo demora para a onda chegar na porta Echo
    distancia = (duracao / 2)/29.1; // Calcula a conversão para Cm

    // Se a distância for maior que a limite e menor que zero não faz nada.
    if (distancia >= distancia_limite || distancia <= 0) 
    {
      CorRgb(0,0,255,0); // Cor Azul, Nada Detectado
      alarme_on = LOW; // Mantém o alarme desativado
      Serial.println("Casa Protegida");
      digitalWrite(buzzer, LOW); // Mantém o buzzer desativado
    }

    // Se a distância estiver entre a limite e 0 ele liga a sirene
    while (distancia < distancia_limite && distancia > 0 && alarme_on == LOW)
    {
      CorRgb(255,0,0,0); // Cor Vermelha, Pessoa Detectada
      Serial.println("Pessoa Detectada");
      Serial.print("Distância = ");
      Serial.println(distancia); // Mostra no terminal a distância em que a pessoa está (cm)

      // Sirene de Polícia
      for (frequencia = 150; frequencia < 1800; frequencia += 1)
      {
        alarme_on = digitalRead(alarme); // Lê o botão que desativa o alarme
        if(alarme_on == HIGH) // Se o botão for pressionado ele encerra o laço de detecção
        {
          break;
        }
        // Toca o buzzer em determinada frequência por determinado tempo
        tone(buzzer, frequencia, tempo);
        delay(1);
      }
      for (frequencia = 1800; frequencia > 150; frequencia -= 1) 
      {
        alarme_on = digitalRead(alarme); // Lê o botão que desativa o alarme
        if(alarme_on == HIGH) // Se o botão for pressionado ele encerra o laço de detecção
        {
          break;
        }
        // Toca o buzzer em determinada frequência por determinado tempo
        tone(buzzer, frequencia, tempo); 
        delay(1);
      }
    }
   } // Fim do laço principal
  }
  
  // Função para controlar a cor do led RGB anodo comum
  void CorRgb(int vermelho, int verde, int azul, int on_off)
  {
    #ifdef ANODO_COMUM
    vermelho = 255 - vermelho; // Operação para resultar no vermelho no led ANODO COMUM
    verde = 255 - verde; // Operação para resultar no verde no led ANODO COMUM
    azul = 255 - azul; // Operação para resultar no verde no led ANODO COMUM
    #endif
    if (on_off == 1) // Se for para ligar e desligar o CIRCUITO
    {
      analogWrite(R0, vermelho); 
      analogWrite(G0, verde); 
      analogWrite(B0, azul); 
    }
    else // Se for para ativar ou desativar o ALARME
    {
      analogWrite(R1, vermelho); 
      analogWrite(G1, verde); 
      analogWrite(B1, azul); 
    }
  }
 
