#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <list>

/*
  Implementacao do trabalho Produtor/Consumidor em Pthreads.

  Solucao utilizando o minimo de variaveis estaticas possiveis,
  sendo realizada a passagem de argumentos para as threads por
  meio de structs.

  Para isso foi feito uso de Mutex para areas criticas, e, para
  controle de acesso, variaveis de condicao.

  Author: Felipe Lopes
  Data_inicio: 15/09/19
  Data_termino: 19/09/19
*/

typedef struct{
    int v; // v = n de iteracoes por produtor
    int t; // t = tamanho maximo do buffer
    std::list<int> *buffer;  // list utilizado para o buffer
}thread_arg_prod, *ptr_thread_arg_prod;

typedef struct{
    std::list<int> *buffer;  // list utilizado para o buffer
    int t; // t = tamanho maximo do buffer
}thread_arg_cons, *ptr_thread_arg_cons;

pthread_mutex_t lock; // Varivel de controle para o Mutex
pthread_cond_t pos_vazia; // Para o produtor esperar
pthread_cond_t pos_ocupada; // Para o consumidor esperar

// Declaracao de funcoes
void *produtor(void *arg);
void *consumidor(void *arg);
void msg_erro();
bool ehPrimo(int n);
// ===========================

int main(int argc, char **argv){
  // v = n de iteracoes por produtor
  // p = n de produtores
  // c = n de consumidores
  // t = tamanho maximo do buffer
  int v=0, p=0, c=0, t=0;

  // Le os argumentos passados
  if(argc != 5){
    msg_erro();
    return 1;
  }else{
    v = atoi(argv[1]);
    p = atoi(argv[2]);
    c = atoi(argv[3]);
    t = atoi(argv[4]);
  }

  pthread_t cons[c]; // Threads Consumidor
  pthread_t prod[p]; // Threads Produtor
  thread_arg_prod arguments_p[p]; // Vetor com argumentos para produtores
  thread_arg_cons arguments_c[c]; // Vetor com argumentos para consumidores
  std::list<int> buffer; // Buffer do tipo list do c++

  int i;
  pthread_mutex_init(&lock, NULL); // Inicializa o mutex

  srand(time(NULL));

  // Laco responsavel pela criacao das threads produtoras
  for(i=0; i<p; i++){
    arguments_p[i].v = v;
    arguments_p[i].buffer = &(buffer);
    arguments_p[i].t = t;
    pthread_create(&(prod[i]), NULL, produtor, &(arguments_p[i]));
  }

  // Laco responsavel pela criacao das threads consumidoras
  for(i=0; i<c; i++){
    arguments_c[i].buffer = &(buffer);
    arguments_c[i].t = t;
    pthread_create(&(cons[i]), NULL, consumidor, &(arguments_c[i]));
  }

  // Laco de lancamento das threads produtoras criadas
  for(i=0; i<p; i++){
    pthread_join(prod[i], NULL);
  }

  /*
    Laco responsavel por adicionar o valor -1 em quantidade equivalente
    a quantidade de threads consumidoras
  */
  for(i=0;i<c;i++){
    buffer.push_back(-1);
    pthread_cond_signal(&pos_ocupada);
  }

  // Laco de lancamento das threads consumidoras criadas
  for(i=0; i<c; i++){
    pthread_join(cons[i], NULL);
  }
}

/*
  Funcao responsavel pelo comportamento das threads produtoras
  Utilizada passagem de argumentos:
    @param argument->v : N de iteracoes da thread
    @param argument->buffer : referencia o buffer
    @param argument->t : Tam maximo do buffer
  Return: 0;
*/
void *produtor(void *arg){
  ptr_thread_arg_prod argument = (ptr_thread_arg_prod) arg;
  int valor;
  for(int i=0;i<argument->v;i++){
    valor = rand() % 100;
    pthread_mutex_lock(&lock); // inicio sessao critica
    if(argument->buffer->size() == argument->t) // Se o buffer atingiu tamanho maximo, espere
		  pthread_cond_wait(&pos_vazia, &lock);
    if(argument->buffer->size() < argument->t){ // Se o buffer tem espaco, avise e adicione
      pthread_cond_signal(&pos_ocupada);
      argument->buffer->push_back(valor);
    }
    pthread_mutex_unlock(&lock); // fim sessao critica
  }
  return 0;
}

/*
  Funcao responsavel pelo comportamento das threads consumidoras
  Utilizada passagem de argumentos:
    @param argument->buffer : referencia o buffer
    @param argument->t : Tam maximo do buffer
  Return: 0;
*/
void *consumidor(void *arg){
  ptr_thread_arg_cons argument = (ptr_thread_arg_cons) arg;
  int valor = 0;
  while(1){
    pthread_mutex_lock(&lock); // inicio sessao critica
    if(argument->buffer->empty()) // Se buffer vazio, aguarde
      pthread_cond_wait(&pos_ocupada,&lock);
    if(argument->buffer->size() > 0){ // Se o buffer tiver algum elemento, informe e remova da lista
      pthread_cond_signal(&pos_vazia);
      valor = argument->buffer->front();
      argument->buffer->pop_front();
    }
    pthread_mutex_unlock(&lock);  // Fim da sessao critica
    if(valor == -1) // Se valor lido for -1, abandone a thread
      break;
    else if(ehPrimo(valor)) // se valor for primo, imprima id thread e valor
      printf("Consumido primo [%ld : %d]\n",pthread_self(),valor);
  }
  return 0;
}

/*
  Mensagens de erro para quantidade errada de argumentos
*/
void msg_erro(){
  printf("Numero de parametros errados\n");
  printf("Parametros:\n");
  printf("N iteracoes por Produtor\n");
  printf("Qtd de produtores\n");
  printf("Qtd de consumidores\n");
  printf("Tam. max. buffer\n");
}

/*
  Verifica se um numero eh primo
*/
bool ehPrimo(int valor){
  int aux = 0;
  for(int i=1;i <= valor/2; i++){
    if((int)(valor % i) == 0){
      aux++;
    }
  }
  if(aux == 1)
    return true;
  else
    return false;
}
