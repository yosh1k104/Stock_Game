#include "client-base.h"



void act(int sfd, struct company companies[10], int turn, uint32_t temp, uint32_t key){
  printf("\n*****ACTION*****\n");

  temp = htonl(key);
  printf("buy_key:%u\n", temp);
  write(sfd, &temp, sizeof(temp));
  if(turn % 2 == 0){
    temp = htonl(BUY);
  }else if(turn % 2 == 1){
    temp = htonl(SELL);
  }
  printf("buy_code:%u\n", temp);
  write(sfd, &temp, sizeof(temp));
  temp = htonl(companies[0].id);
  printf("buy_id:%u\n", temp);
  write(sfd, &temp, sizeof(temp));
  temp = htonl(STOCKS);
  printf("buy_value:%u\n\n", temp);
  write(sfd, &temp, sizeof(temp)); 
}


void comfirm(size_t len, int sfd, uint32_t buf, struct company companies[10], uint32_t key, uint32_t code, int num){
  printf("\n*****COMFIRM*****\n");

  int i;
  for(i = 0; i < 22; i++){
    len = read(sfd, &buf, sizeof(buf));
    if(i == 0){
      key = ntohl(buf);
      
      printf("act_key:%u\n", key);
    }else if(i == 1){ 
      code = ntohl(buf);


      if(code == ERR_CODE){
	printf("Get the error code!\n");
      }else if(code == ERR_KEY){
	printf("Get the error key!\n");
      }else if(code == ERR_REQ){
	printf("Get the error request!\n");
      }else if(code == ERR_ID){
	printf("Get the error id!\n");
      }else if(code == ERR_PUR){
	printf("Cannot purchase!\n");
      }else if(code == ERR_SAL){
	printf("Cannot sell!\n");
      }
      
      printf("act_code:%u\n", code);
    }else if(i > 1){
      if(i % 2 == 0){
	companies[num].id = ntohl(buf);
	
	printf("company#%u\t", companies[num].id);
      }else if(i % 2 == 1){
	companies[num].price = ntohl(buf);
	
	printf("%u\n\n", companies[num].price);
	num = num + 1;
	
      }
    }
  }
  
}


int showResult(size_t len, int sfd, uint32_t buf){
  printf("\n*****RESULT*****\n");

  int i;
  for(i = 0; i < 4; i++){
    len = read(sfd, &buf, sizeof(buf));
    
    if(i == 0){
      printf("last_key");
    }else if(i == 1){
      printf("last_code");
    }else if(i == 2){
      printf("rank");
    }else if(i == 3){
      printf("budget");
    }
    printf(":%u\n", ntohl(buf));

  }
  return 0;
}


int main(int argc, char *argv[]){
  struct addrinfo hints;
  struct addrinfo *result, *rp;

  struct company companies[10];

  size_t len;

  uint32_t buf = 0;
  uint32_t key = 0;
  uint32_t code = 0;
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
      for(turn = 0; turn < TURN; turn++){
	int i;
	int num = 0;

	printf("===========TURN#%d============\n", turn + 1);



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
	      printf("company#%u\t", companies[num].id);
	    }else if(i % 2 == 1){
	      companies[num].price = ntohl(buf);
	      printf("price:%u\n\n", companies[num].price);
	      num = num + 1;
	      
	    }
	  }
	}
	
	
	act(sfd, companies, turn, temp, key);

	comfirm(len, sfd, buf, companies, key, code, num);


      }//while end
      
	
      showResult(len, sfd, buf);
      
      break;

    }
  }

  if(rp == NULL){
    fprintf(stderr, "Could not connect!\n");
    exit(1);
  }

  close(sfd);

  return 0;
}
