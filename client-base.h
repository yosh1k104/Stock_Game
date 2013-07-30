#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdint.h>

#define ACCEPT 0x00000001
#define BUY 0x00000100
#define SELL 0x00000101
#define ERR_CODE 0x00000400
#define ERR_KEY 0x00000401
#define ERR_REQ 0x00000402
#define ERR_ID 0x00000403
#define ERR_PUR 0x00000404
#define ERR_SAL 0x00000405

#define STOCKS 1
#define TURN 60



struct company{
  uint32_t id;
  uint32_t price;
};


