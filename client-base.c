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



struct company{
  uint32_t id;
  uint32_t price;
};


int main(int argc, char *argv[]){
  struct addrinfo hints;
  struct addrinfo *result, *rp;

  struct company companies[10];

  int state = 0;
  
  size_t len;

  uint32_t buf;

  uint32_t key = 0;
  uint32_t code = 0;



  int number = 0;

  uint32_t s_key;
  uint32_t id;
  uint32_t value;

  uint32_t temp;



  int s, sfd;



  if(argc < 3){
    fprintf(stderr, "Usage: %s host port msg...\n", argv[0]);
    exit(1);
  }

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = 0;
  hints.ai_protocol = 0;

  s = getaddrinfo(argv[1], argv[2], &hints, &result);

  if(s != 0){
    exit(1);
  }

  for(rp = result; rp != NULL; rp = rp->ai_next){
    sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if(sfd == -1){
      continue;
    }

    if(connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1){
      printf("Connected!\n\n");

      int turn;
      for(turn = 0; turn < 60; turn++){
	int i;
	int num = 0;

	printf("===========TURN#%d============\n", turn);


	for(i = 0; i < 22; i++){
	  

	  len = read(sfd, &buf, sizeof(buf));
	  

	  if(i == 0){

	    key = ntohl(buf);
	    printf("key:%u\n", key);

	  }else if(i == 1){ 
	    code = ntohl(buf);
	    printf("code:%u\n", code);

	  }else if(i > 1){
	    if(i % 2 == 0){
	    companies[num].id = ntohl(buf);
	    printf("buf:%x\n", buf);

	    printf("id:%u\n", companies[num].id);

	    }else if(i % 2 == 1){
	      companies[num].price = ntohl(buf);

	      printf("buf:%x\n", buf);
	      printf("price:%u\n\n", companies[num].price);
	      num = num + 1;
	      
	    }
	  }
	}

	
	
	temp = htonl(key);
	printf("buy_key:%u\n", temp);
	write(sfd, &temp, sizeof(temp));
	if(turn % 2 == 0){
	  temp = htonl(0x00000100);
	}else if(turn % 2 == 1){
	  temp = htonl(0x00000101);
	}
	printf("buy_code:%u\n", temp);
	write(sfd, &temp, sizeof(temp));
	temp = htonl(companies[0].id);
	printf("buy_id:%u\n", temp);
	write(sfd, &temp, sizeof(temp));
	temp = htonl(10);
	printf("buy_value:%u\n\n", temp);
	write(sfd, &temp, sizeof(temp));




	for(i = 0; i < 22; i++){
	  len = read(sfd, &buf, sizeof(buf));
	  if(i == 0){
	    key = ntohl(buf);

	    printf("act_key:%u\n", key);
	  }else if(i == 1){ 
	    code = ntohl(buf);

	    printf("act_code:%u\n", code);
	  }else if(i > 1){
	    if(i % 2 == 0){
	    companies[num].id = ntohl(buf);

	    printf("act_id:%u\n", companies[num].id);
	    }else if(i % 2 == 1){
	      companies[num].price = ntohl(buf);

	      printf("act_value:%u\n\n", companies[num].price);
	      num = num + 1;
	      
	    }
	  }
	}
	  

	
      }//while end
      

      
      len = read(sfd, &buf, sizeof(buf));
      printf("end_key:%u\n", ntohl(buf));
      len = read(sfd, &buf, sizeof(buf));
      printf("end_code:%u\n", ntohl(buf));
      len = read(sfd, &buf, sizeof(buf));
      printf("end_rank:%u\n", ntohl(buf));
      len = read(sfd, &buf, sizeof(buf));
      printf("end_budget:%u\n", ntohl(buf));



      break;
    }
  }

  if(rp == NULL){
    fprintf(stderr, "Could not connect!\n");
    exit(1);
  }

  freeaddrinfo(result);

  close(sfd);

  return 0;
}
