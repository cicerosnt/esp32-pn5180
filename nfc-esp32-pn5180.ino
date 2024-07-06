#include <PN5180ISO15693.h>

// Pinos para o único leitor PN5180
const byte nssPin = 5;
const byte busyPin = 16;
const byte resetPin = 17;

// Criar o objeto para o leitor PN5180
PN5180ISO15693 nfc(nssPin, busyPin, resetPin);

void setup() {
  // Inicializa a comunicação serial
  Serial.begin(115200);
  
  // Inicializa a comunicação SPI
  SPI.begin();

  // Mensagem de inicialização
  Serial.println("Inicializando o leitor...");
  
  // Inicializa o leitor NFC
  nfc.begin();
  
  // Redefine o leitor NFC
  Serial.println("Redefinindo o leitor...");
  nfc.reset();
  
  // Mensagem de habilitação do campo NFC
  Serial.println("Habilitando o campo NFC...");

  // Verifica a versão do produto
  uint8_t productVersion[2];
  nfc.readEEprom(PRODUCT_VERSION, productVersion, sizeof(productVersion));
  if (0xff == productVersion[1]) { // verifica se houve falha
    Serial.println(F("Falha na inicialização do leitor!"));
    Serial.println(F("Pressione reset para tentar novamente..."));
    Serial.flush();
    exit(-1); // parar execução
  }

  // Mensagem de prontidão
  Serial.println("Pronto para utilização...");
  
  // Configura o campo RF do leitor NFC
  nfc.setupRF();
}

void loop() {
  // Detecção de tag
  uint8_t uid[8];

  // Tenta ler o inventário (ID da tag)
  ISO15693ErrorCode rc = nfc.getInventory(uid);
  
  // Se a leitura foi bem-sucedida
  if (rc == ISO15693_EC_OK) {
    Serial.print(F("Dispositivo NFC Detectado... ID: "));
    for (int j = 0; j < sizeof(uid); j++) {
      Serial.print(uid[j], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }

  // Atraso de 1 segundo antes da próxima leitura
  delay(1000);
}