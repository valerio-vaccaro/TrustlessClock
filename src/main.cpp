#include <M5ez.h>
#include <ezTime.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "config.h"

#define MAIN_DECLARED

HTTPClient http;

void setup() {
  #include <themes/default.h>
  #include <themes/dark.h>
  ezt::setDebug(INFO);
  ez.begin();
  http.setReuse(true);
}

void bitcoin_menus();
void bitcoint_menus();
void bitcoinL_menus();
void bitcoinLt_menus();
void liquid_menus();
void powerOff();
void about();

void loop() {
  ezMenu mainmenu("Trustless Clock");
  mainmenu.txtSmall();
  mainmenu.addItem("Bitcoin", bitcoin_menus);
  mainmenu.addItem("Bitcoin testnet", bitcoint_menus);
  mainmenu.addItem("Litecoin", bitcoinL_menus);
  mainmenu.addItem("Litecoin testnet", bitcoinLt_menus);
  mainmenu.addItem("Liquid", liquid_menus);
  mainmenu.addItem("Built-in wifi & other settings", ez.settings.menu);
  mainmenu.addItem("About", about);
  mainmenu.addItem("Power off", powerOff);
  mainmenu.upOnFirst("last|up");
  mainmenu.downOnLast("first|down");
  mainmenu.run();
}

void powerOff() {
  m5.powerOFF();
}

void about() {
  ez.msgBox("About Trustless Clock", "Trustless Clock was written by | Valerio | | ");
}

void bitcoin_heigh();
void bitcoin_hash();
void bitcoin_merkle();
void bitcoin_tx();
void bitcoin_timestamp();

void bitcoin_menus() {
  ezMenu submenu("BTC last block");
  submenu.txtSmall();
  submenu.buttons("up#Back#select##down#");
  submenu.addItem("Heigh", bitcoin_heigh);
  submenu.addItem("Hash", bitcoin_hash);
  submenu.addItem("Merkle root", bitcoin_merkle);
  submenu.addItem("TX count", bitcoin_tx);
  submenu.addItem("Timestamp", bitcoin_timestamp);
  submenu.addItem("Exit | Go back to main menu");
  submenu.run();
}

void bitcoin_heigh() {
  http.begin(coreUrl, corePort);
  http.setAuthorization(coreUser, corePassword);
  char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getblockcount\",\"params\":[]}");
  int httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<300> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      ez.msgBox("BTC heigh", parsed["result"]);
    }
  } else {
    ez.msgBox("BTC heigh", "ERROR");
  }
  http.end();
}

void bitcoin_hash() {
  http.begin(coreUrl, corePort);
  http.setAuthorization(coreUser, corePassword);
  char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getbestblockhash\",\"params\":[]}");
  int httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<300> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      ez.textBox("BTC hash", parsed["result"]);
    }
  } else {
    ez.msgBox("BTC hash", "ERROR");
  }
  http.end();
}

void bitcoin_timestamp() {
  const char * hash;
  http.begin(coreUrl, corePort);
  http.setAuthorization(coreUser, corePassword);
  char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getbestblockhash\",\"params\":[]}");
  int httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<300> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      hash = (const char *) parsed["result"];
    }
  }
  http.end();

  USE_SERIAL.println(hash);
  http.begin(coreUrl, corePort);
  http.setAuthorization(coreUser, corePassword);
  //char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getblockheader\",\"params\":[\"%s\"]}", hash );
  USE_SERIAL.println(buff);
  httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<1000> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      time_t t = atol(parsed["result"]["time"]);
      if (minute(t) < 10) {
        ez.msgBox("BTC timestamp", String(day(t))+"/"+String(month(t))+"/"+String(year(t))+" "+String(hour(t))+":0"+String(minute(t)) );
      } else {
        ez.msgBox("BTC timestamp", String(day(t))+"/"+String(month(t))+"/"+String(year(t))+" "+String(hour(t))+":"+String(minute(t)) );
      }
    }
  } else {
    ez.msgBox("BTC timestamp", "ERROR");
  }
  http.end();
}


void bitcoin_merkle() {
  const char * hash;
  http.begin(coreUrl, corePort);
  http.setAuthorization(coreUser, corePassword);
  char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getbestblockhash\",\"params\":[]}");
  int httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<300> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      hash = (const char *) parsed["result"];
    }
  }
  http.end();

  USE_SERIAL.println(hash);
  http.begin(coreUrl, corePort);
  http.setAuthorization(coreUser, corePassword);
  //char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getblockheader\",\"params\":[\"%s\"]}", hash );
  USE_SERIAL.println(buff);
  httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<1000> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      ez.msgBox("BTC merkle root", parsed["result"]["merkleroot"]);
    }
  } else {
      ez.msgBox("BTC merkle root", "ERROR");
  }
  http.end();
}

void bitcoin_tx() {
  const char * hash;
  http.begin(coreUrl, corePort);
  http.setAuthorization(coreUser, corePassword);
  char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getbestblockhash\",\"params\":[]}");
  int httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<300> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      hash = (const char *) parsed["result"];
    }
  }
  http.end();

  USE_SERIAL.println(hash);
  http.begin(coreUrl, corePort);
  http.setAuthorization(coreUser, corePassword);
  //char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getblockheader\",\"params\":[\"%s\"]}", hash );
  USE_SERIAL.println(buff);
  httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<1000> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      ez.msgBox("BTC transactions", parsed["result"]["nTx"]);
    }
  } else {
    ez.msgBox("BTC transactions", "ERROR");
  }
  http.end();
}

void bitcoint_heigh();
void bitcoint_hash();
void bitcoint_merkle();
void bitcoint_tx();
void bitcoint_timestamp();

void bitcoint_menus() {
  ezMenu submenu("BTC testnet last block");
  submenu.txtSmall();
  submenu.buttons("up#Back#select##down#");
  submenu.addItem("Heigh", bitcoint_heigh);
  submenu.addItem("Hash", bitcoint_hash);
  submenu.addItem("Merkle root", bitcoint_merkle);
  submenu.addItem("TX count", bitcoint_tx);
  submenu.addItem("Timestamp", bitcoint_timestamp);
  submenu.addItem("Exit | Go back to main menu");
  submenu.run();
}

void bitcoint_heigh() {
  http.begin(coretUrl, coretPort);
  http.setAuthorization(coretUser, coretPassword);
  char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getblockcount\",\"params\":[]}");
  int httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<300> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      ez.msgBox("BTC testnet heigh", parsed["result"]);
    }
  } else {
    ez.msgBox("BTC testnet heigh", "ERROR");
  }
  http.end();
}

void bitcoint_hash() {
  http.begin(coretUrl, coretPort);
  http.setAuthorization(coretUser, coretPassword);
  char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getbestblockhash\",\"params\":[]}");
  int httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<300> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      ez.textBox("BTC testnet hash", parsed["result"]);
    }
  } else {
    ez.msgBox("BTC testnet hash", "ERROR");
  }
  http.end();
}

void bitcoint_timestamp() {
  const char * hash;
  http.begin(coretUrl, coretPort);
  http.setAuthorization(coretUser, coretPassword);
  char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getbestblockhash\",\"params\":[]}");
  int httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<300> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      hash = (const char *) parsed["result"];
    }
  }
  http.end();

  USE_SERIAL.println(hash);
  http.begin(coretUrl, coretPort);
  http.setAuthorization(coretUser, coretPassword);
  //char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getblockheader\",\"params\":[\"%s\"]}", hash );
  USE_SERIAL.println(buff);
  httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<1000> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      time_t t = atol(parsed["result"]["time"]);
      ez.msgBox("BTC testnet timestamp", String(day(t))+"/"+String(month(t))+"/"+String(year(t))+" "+String(hour(t))+":"+String(minute(t)) );
    }
  } else {
    ez.msgBox("BTC testnet timestamp", "ERROR");
  }
  http.end();
}

void bitcoint_merkle() {
  const char * hash;
  http.begin(coretUrl, coretPort);
  http.setAuthorization(coretUser, coretPassword);
  char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getbestblockhash\",\"params\":[]}");
  int httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<300> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      hash = (const char *) parsed["result"];
    }
  }
  http.end();

  USE_SERIAL.println(hash);
  http.begin(coretUrl, coretPort);
  http.setAuthorization(coretUser, coretPassword);
  //char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getblockheader\",\"params\":[\"%s\"]}", hash );
  USE_SERIAL.println(buff);
  httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<1000> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      ez.msgBox("BTC testnet merkle root", parsed["result"]["merkleroot"]);
    }
  } else {
      ez.msgBox("BTC testnet merkle root", "ERROR");
  }
  http.end();
}

void bitcoint_tx() {
  const char * hash;
  http.begin(coretUrl, coretPort);
  http.setAuthorization(coretUser, coretPassword);
  char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getbestblockhash\",\"params\":[]}");
  int httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<300> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      hash = (const char *) parsed["result"];
    }
  }
  http.end();

  USE_SERIAL.println(hash);
  http.begin(coretUrl, coretPort);
  http.setAuthorization(coretUser, coretPassword);
  //char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getblockheader\",\"params\":[\"%s\"]}", hash );
  USE_SERIAL.println(buff);
  httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<1000> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      ez.msgBox("BTC testnet transactions", parsed["result"]["nTx"]);
    }
  } else {
    ez.msgBox("BTC testnet transactions", "ERROR");
  }
  http.end();
}

void bitcoinL_heigh();
void bitcoinL_hash();
void bitcoinL_merkle();
void bitcoinL_tx();
void bitcoinL_timestamp();

void bitcoinL_menus() {
  ezMenu submenu("LTC last block");
  submenu.txtSmall();
  submenu.buttons("up#Back#select##down#");
  submenu.addItem("Heigh", bitcoinL_heigh);
  submenu.addItem("Hash", bitcoinL_hash);
  submenu.addItem("Merkle root", bitcoinL_merkle);
  submenu.addItem("TX count", bitcoinL_tx);
  submenu.addItem("Timestamp", bitcoinL_timestamp);
  submenu.addItem("Exit | Go back to main menu");
  submenu.run();
}

void bitcoinL_heigh() {
  http.begin(coreLUrl, coreLPort);
  http.setAuthorization(coreLUser, coreLPassword);
  char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getblockcount\",\"params\":[]}");
  int httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<300> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      ez.msgBox("LTC heigh", parsed["result"]);
    }
  } else {
    ez.msgBox("LTC heigh", "ERROR");
  }
  http.end();
}

void bitcoinL_hash() {
  http.begin(coreLUrl, coreLPort);
  http.setAuthorization(coreLUser, coreLPassword);
  char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getbestblockhash\",\"params\":[]}");
  int httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<300> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      ez.textBox("LTC hash", parsed["result"]);
    }
  } else {
    ez.msgBox("LTC hash", "ERROR");
  }
  http.end();
}

void bitcoinL_timestamp() {
  const char * hash;
  http.begin(coreLUrl, coreLPort);
  http.setAuthorization(coreLUser, coreLPassword);
  char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getbestblockhash\",\"params\":[]}");
  int httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<300> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      hash = (const char *) parsed["result"];
    }
  }
  http.end();

  USE_SERIAL.println(hash);
  http.begin(coreLUrl, coreLPort);
  http.setAuthorization(coreLUser, coreLPassword);
  //char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getblockheader\",\"params\":[\"%s\"]}", hash );
  USE_SERIAL.println(buff);
  httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<1000> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      time_t t = atol(parsed["result"]["time"]);
      ez.msgBox("LTC timestamp", String(day(t))+"/"+String(month(t))+"/"+String(year(t))+" "+String(hour(t))+":"+String(minute(t)) );
    }
  } else {
    ez.msgBox("LTC timestamp", "ERROR");
  }
  http.end();
}


void bitcoinL_merkle() {
  const char * hash;
  http.begin(coreLUrl, coreLPort);
  http.setAuthorization(coreLUser, coreLPassword);
  char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getbestblockhash\",\"params\":[]}");
  int httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<300> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      hash = (const char *) parsed["result"];
    }
  }
  http.end();

  USE_SERIAL.println(hash);
  http.begin(coreLUrl, coreLPort);
  http.setAuthorization(coreLUser, coreLPassword);
  //char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getblockheader\",\"params\":[\"%s\"]}", hash );
  USE_SERIAL.println(buff);
  httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<1000> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      ez.msgBox("LTC merkle root", parsed["result"]["merkleroot"]);
    }
  } else {
      ez.msgBox("LTC merkle root", "ERROR");
  }
  http.end();
}

void bitcoinL_tx() {
  const char * hash;
  http.begin(coreLUrl, coreLPort);
  http.setAuthorization(coreLUser, coreLPassword);
  char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getbestblockhash\",\"params\":[]}");
  int httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<300> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      hash = (const char *) parsed["result"];
    }
  }
  http.end();

  USE_SERIAL.println(hash);
  http.begin(coreLUrl, coreLPort);
  http.setAuthorization(coreLUser, coreLPassword);
  //char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getblockheader\",\"params\":[\"%s\"]}", hash );
  USE_SERIAL.println(buff);
  httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<1000> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      ez.msgBox("LTC transactions", parsed["result"]["nTx"]);
    }
  } else {
    ez.msgBox("LTC transactions", "ERROR");
  }
  http.end();
}

void bitcoinLt_heigh();
void bitcoinLt_hash();
void bitcoinLt_merkle();
void bitcoinLt_tx();
void bitcoinLt_timestamp();

void bitcoinLt_menus() {
  ezMenu submenu("LTC testnet last block");
  submenu.txtSmall();
  submenu.buttons("up#Back#select##down#");
  submenu.addItem("Heigh", bitcoinLt_heigh);
  submenu.addItem("Hash", bitcoinLt_hash);
  submenu.addItem("Merkle root", bitcoinLt_merkle);
  submenu.addItem("TX count", bitcoinLt_tx);
  submenu.addItem("Timestamp", bitcoinLt_timestamp);
  submenu.addItem("Exit | Go back to main menu");
  submenu.run();
}

void bitcoinLt_heigh() {
  http.begin(coreLtUrl, coreLtPort);
  http.setAuthorization(coreLtUser, coreLtPassword);
  char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getblockcount\",\"params\":[]}");
  int httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<300> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      ez.msgBox("LTC testnet heigh", parsed["result"]);
    }
  } else {
    ez.msgBox("LTC testnet heigh", "ERROR");
  }
  http.end();
}

void bitcoinLt_hash() {
  http.begin(coreLtUrl, coreLtPort);
  http.setAuthorization(coreLtUser, coreLtPassword);
  char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getbestblockhash\",\"params\":[]}");
  int httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<300> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      ez.textBox("LTC testnet hash", parsed["result"]);
    }
  } else {
    ez.msgBox("LTC testnet hash", "ERROR");
  }
  http.end();
}

void bitcoinLt_timestamp() {
  const char * hash;
  http.begin(coreLtUrl, coreLtPort);
  http.setAuthorization(coreLtUser, coreLtPassword);
  char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getbestblockhash\",\"params\":[]}");
  int httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<300> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      hash = (const char *) parsed["result"];
    }
  }
  http.end();

  USE_SERIAL.println(hash);
  http.begin(coreLtUrl, coreLtPort);
  http.setAuthorization(coreLtUser, coreLtPassword);
  //char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getblockheader\",\"params\":[\"%s\"]}", hash );
  USE_SERIAL.println(buff);
  httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<1000> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      time_t t = atol(parsed["result"]["time"]);
      ez.msgBox("LTC testnet timestamp", String(day(t))+"/"+String(month(t))+"/"+String(year(t))+" "+String(hour(t))+":"+String(minute(t)) );
    }
  } else {
    ez.msgBox("LTC testnet timestamp", "ERROR");
  }
  http.end();
}


void bitcoinLt_merkle() {
  const char * hash;
  http.begin(coreLtUrl, coreLtPort);
  http.setAuthorization(coreLtUser, coreLtPassword);
  char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getbestblockhash\",\"params\":[]}");
  int httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<300> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      hash = (const char *) parsed["result"];
    }
  }
  http.end();

  USE_SERIAL.println(hash);
  http.begin(coreLtUrl, coreLtPort);
  http.setAuthorization(coreLtUser, coreLtPassword);
  //char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getblockheader\",\"params\":[\"%s\"]}", hash );
  USE_SERIAL.println(buff);
  httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<1000> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      ez.msgBox("LTC testnet merkle root", parsed["result"]["merkleroot"]);
    }
  } else {
      ez.msgBox("LTC testnet merkle root", "ERROR");
  }
  http.end();
}

void bitcoinLt_tx() {
  const char * hash;
  http.begin(coreLtUrl, coreLtPort);
  http.setAuthorization(coreLtUser, coreLtPassword);
  char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getbestblockhash\",\"params\":[]}");
  int httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<300> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      hash = (const char *) parsed["result"];
    }
  }
  http.end();

  USE_SERIAL.println(hash);
  http.begin(coreLtUrl, coreLtPort);
  http.setAuthorization(coreLtUser, coreLtPassword);
  //char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getblockheader\",\"params\":[\"%s\"]}", hash );
  USE_SERIAL.println(buff);
  httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<1000> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      ez.msgBox("LTC testnet transactions", parsed["result"]["nTx"]);
    }
  } else {
    ez.msgBox("LTC testnet transactions", "ERROR");
  }
  http.end();
}

void liquid_heigh();
void liquid_hash();
void liquid_merkle();
void liquid_tx();
void liquid_timestamp();

void liquid_menus() {
  ezMenu submenu("Liquid last block");
  submenu.txtSmall();
  submenu.buttons("up#Back#select##down#");
  submenu.addItem("Heigh", liquid_heigh);
  submenu.addItem("Hash", liquid_hash);
  submenu.addItem("Merkle root", liquid_merkle);
  submenu.addItem("Timestamp", liquid_timestamp);
  submenu.addItem("Exit | Go back to main menu");
  submenu.run();
}

void liquid_heigh() {
  http.begin(liquidUrl, liquidPort);
  http.setAuthorization(liquidUser, liquidPassword);
  char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getblockcount\",\"params\":[]}");
  int httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<300> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      ez.msgBox("Liquid heigh", parsed["result"]);
    }
  } else {
    ez.msgBox("Liquid heigh", "ERROR");
  }
  http.end();
}

void liquid_hash() {
  http.begin(liquidUrl, liquidPort);
  http.setAuthorization(liquidUser, liquidPassword);
  char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getbestblockhash\",\"params\":[]}");
  int httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<300> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      ez.textBox("Liquid hash", parsed["result"]);
    }
  } else {
    ez.msgBox("Liquid hash", "ERROR");
  }
  http.end();
}

void liquid_timestamp() {
  const char * hash;
  http.begin(liquidUrl, liquidPort);
  http.setAuthorization(liquidUser, liquidPassword);
  char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getbestblockhash\",\"params\":[]}");
  int httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<300> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      hash = (const char *) parsed["result"];
    }
  }
  http.end();

  http.begin(liquidUrl, liquidPort);
  http.setAuthorization(liquidUser, liquidPassword);
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getblockheader\",\"params\":[\"%s\"]}", hash );
  httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    int pos = 0;
    String s = http.getString();
    pos = s.indexOf("time")+6;
    time_t t = s.substring(pos, pos+10).toInt();
    if (minute(t) < 10) {
      ez.msgBox("Liquid timestamp", String(day(t))+"/"+String(month(t))+"/"+String(year(t))+" "+String(hour(t))+":0"+String(minute(t)) );
    } else {
      ez.msgBox("Liquid timestamp", String(day(t))+"/"+String(month(t))+"/"+String(year(t))+" "+String(hour(t))+":"+String(minute(t)) );
    }
  } else {
    ez.msgBox("Liquid timestamp", "ERROR");
  }
  http.end();
}


void liquid_merkle() {
  const char * hash;
  http.begin(liquidUrl, liquidPort);
  http.setAuthorization(liquidUser, liquidPassword);
  char buff[255];
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getbestblockhash\",\"params\":[]}");
  int httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    StaticJsonBuffer<300> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(http.getString());
    if (!parsed.success()) {
      USE_SERIAL.println("Parsing failed");
    } else {
      hash = (const char *) parsed["result"];
    }
  }
  http.end();

  http.begin(liquidUrl, liquidPort);
  http.setAuthorization(liquidUser, liquidPassword);
  sprintf(buff,"{\"jsonrpc\":\"1.0\",\"id\":\"curltext\",\"method\":\"getblockheader\",\"params\":[\"%s\"]}", hash );
  USE_SERIAL.println(buff);
  httpCode = http.POST((uint8_t *) buff, strlen(buff));
  if(httpCode == HTTP_CODE_OK) {
    int pos = 0;
    String s = http.getString();
    pos = s.indexOf("merkleroot")+13;
    ez.msgBox("Liquid merkle root", s.substring(pos, pos+64));
  } else {
      ez.msgBox("Liquid merkle root", "ERROR");
  }
  http.end();
}
