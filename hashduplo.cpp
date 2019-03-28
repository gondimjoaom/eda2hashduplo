#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iomanip>

using namespace std;

typedef struct {

  int idade;
  char nome[20];
  int chave;

} RegD;

int TAMANHO_ARQUIVO = 11;

FILE * criaArquivoD(){
  FILE *arq;
  RegD registro;
  registro.chave = -1;
  if (fopen("arquivo.bin", "rb+") == NULL){
    arq = fopen("arquivo.bin", "wb+");
    for (int i = 0; i < TAMANHO_ARQUIVO; i++){
      fwrite(&registro,sizeof(RegD),1,arq);
    }
  }
  else {
    arq = fopen("arquivo.bin", "rb+");
  }
  return arq;        
}

int insereRegistroD(RegD registro, FILE *arq){
  int h1, h2, pos = 20;
  RegD it;
  h1 = (registro.chave)%TAMANHO_ARQUIVO;
  int aux = (registro.chave / TAMANHO_ARQUIVO);
  h2 = aux % TAMANHO_ARQUIVO;
  if (h2 == 0)
    h2 = 1;
  pos = h1;
  fseek(arq,h1*sizeof(RegD),SEEK_SET);
  fread(&it,sizeof(RegD),1,arq);
  if (it.chave == registro.chave)
    return 0;
  fseek(arq,h1*sizeof(RegD),SEEK_SET);
  while(it.chave != -1 && pos + h2 != h1){
    if (pos + h2 < TAMANHO_ARQUIVO){
      pos = pos + h2;
      fseek(arq,pos*sizeof(RegD),SEEK_SET);
      fread(&it,sizeof(RegD),1,arq);
      if (it.chave == registro.chave)
        return 0; //já existe a chave
      fseek(arq,pos*sizeof(RegD),SEEK_SET);      
    }
    else if (pos + h2 >= TAMANHO_ARQUIVO){
      pos = (pos + h2) - TAMANHO_ARQUIVO;
      fseek(arq,pos*sizeof(RegD),SEEK_SET);
      fread(&it,sizeof(RegD),1,arq);
      if (it.chave == registro.chave)
        return 0;
      fseek(arq,pos*sizeof(RegD),SEEK_SET);      
    }
  }
  if (it.chave != -1){
    return 2; //arquivo cheio
  }
  fseek(arq,pos*sizeof(RegD),SEEK_SET);
  int b = fwrite(&registro,sizeof(RegD),1,arq);
  return 1; //chave inserida
}

void imprimeRegistroD(FILE *arq){
  RegD print;
  for (int i = 0; i < 11; i++){
    fseek(arq,i*sizeof(RegD),SEEK_SET);
    fread(&print,sizeof(RegD),1,arq);
    fseek(arq,i*sizeof(RegD),SEEK_SET);
    if (print.chave != -1){
     cout << i << ": " << print.chave << " " << print.nome << " " << print.idade << endl;
    }
    else
      cout << i << ": " << "vazio" << endl;
  }
}

int consultaD(RegD registro, FILE *arq){
  RegD helper;
  fseek(arq,0*sizeof(RegD),SEEK_SET); 
  for (int i = 0; i < TAMANHO_ARQUIVO; i++){
     fseek(arq,i*sizeof(RegD),SEEK_SET);
     fread(&helper,sizeof(RegD),1,arq);
     fseek(arq,i*sizeof(RegD),SEEK_SET);     
     if (helper.chave == registro.chave){
      cout << "chave: " << helper.chave << endl;
      cout << helper.nome << endl;
      cout << helper.idade << endl;
      return 1; // achou
     }
   }
   return 0; //não achou
}

int consultaCorretaD(RegD registro, FILE *arq){
  int h1, h2, pos = 20, count = 0;
  RegD finder;
  h1 = (registro.chave)%TAMANHO_ARQUIVO;
  int aux = (registro.chave / TAMANHO_ARQUIVO);
  h2 = aux % TAMANHO_ARQUIVO;
  if (h2 == 0)
    h2 = 1;
  fseek(arq,h1*sizeof(RegD),SEEK_SET);
  fread(&finder,sizeof(RegD),1,arq);
  count++;
  if (finder.chave == registro.chave){
    cout << "chave: " << finder.chave << endl;
    cout << finder.nome << endl;
    cout << finder.idade << endl;
    return count;
  }
  while(pos != h1){
    pos = h1;
    if (pos + h2 < TAMANHO_ARQUIVO){
      pos = pos + h2;
      fseek(arq,pos*sizeof(RegD),SEEK_SET);
      fread(&finder,sizeof(RegD),1,arq);
      count++;
      if (finder.chave == registro.chave){
        cout << "chave: " << finder.chave << endl;
        cout << finder.nome << endl;
        cout << finder.idade << endl;
        return count; //achou
      }
      fseek(arq,pos*sizeof(RegD),SEEK_SET);      
    }
    else if (pos + h2 >= TAMANHO_ARQUIVO){
      pos = (pos + h2) - TAMANHO_ARQUIVO;
      fseek(arq,pos*sizeof(RegD),SEEK_SET);
      fread(&finder,sizeof(RegD),1,arq);
      count++;
      if (finder.chave == registro.chave){
        cout << "chave: " << finder.chave << endl;
        cout << finder.nome << endl;
        cout << finder.idade << endl;
        return count;
      }
      fseek(arq,pos*sizeof(RegD),SEEK_SET);      
    }
  }
  return -1;
}

int consultaSemPrintD(RegD registro, FILE *arq){
  int h1, h2, pos = 20, count = 0;
  RegD finder;
  h1 = (registro.chave)%TAMANHO_ARQUIVO;
  int aux = (registro.chave / TAMANHO_ARQUIVO);
  h2 = aux % TAMANHO_ARQUIVO;
  if (h2 == 0)
    h2 = 1;
  pos = h1;
  fseek(arq,h1*sizeof(RegD),SEEK_SET);
  fread(&finder,sizeof(RegD),1,arq);
  count++;
  if (finder.chave == registro.chave)
    return count;
  else if (finder.chave == -1)
    return 0;
  while(pos + h2 != h1){
    count++;
    if (pos + h2 < TAMANHO_ARQUIVO){
      pos = pos + h2;
      fseek(arq,pos*sizeof(RegD),SEEK_SET);
      fread(&finder,sizeof(RegD),1,arq);
      
      if (finder.chave == registro.chave){
        return count; //achou
      }
      else if (finder.chave == -1)
        return 0;
      fseek(arq,pos*sizeof(RegD),SEEK_SET);      
    }
    else if (pos + h2 >= TAMANHO_ARQUIVO){
      pos = (pos + h2) - TAMANHO_ARQUIVO;
      fseek(arq,pos*sizeof(RegD),SEEK_SET);
      fread(&finder,sizeof(RegD),1,arq);
      count++;
      if (finder.chave == registro.chave){
        return count;
      }
      else if (finder.chave == -1)
        return 0;
      fseek(arq,pos*sizeof(RegD),SEEK_SET);      
    }
  }
  return 0;
}

int removeD(RegD registro, FILE *arq){
  RegD helper, replacer;
  replacer.chave = -1;
  fseek(arq,0*sizeof(RegD),SEEK_SET); 
  for (int i = 0; i < TAMANHO_ARQUIVO; i++){
     fseek(arq,i*sizeof(RegD),SEEK_SET);
     fread(&helper,sizeof(RegD),1,arq);
     fseek(arq,i*sizeof(RegD),SEEK_SET);     
     if (helper.chave == registro.chave){
      fwrite(&replacer,sizeof(RegD),1, arq);
      return 1; //removeu
     }
   }
  return 0; //não tem o elemento
}

float mediaD(FILE *arq){
  RegD helpmedia;
  int contadoracesso = 0, contadorregistros = 0;
  float saida;
  for (int i = 0; i < TAMANHO_ARQUIVO; i++){
    fseek(arq,i*sizeof(RegD),SEEK_SET);
    fread(&helpmedia,sizeof(RegD),1,arq);
    fseek(arq,i*sizeof(RegD),SEEK_SET);
    if (helpmedia.chave != -1){
      contadorregistros++;
      contadoracesso = contadoracesso + consultaSemPrintD(helpmedia, arq);
    }
  }
  saida = (float)contadoracesso/(float)contadorregistros;
  return saida;
}

int main (){
  int key, age, resultInsertion;
  char operation, operationIn, name[20];
  string bla;
  FILE *arq;
  arq = criaArquivoD();

  cin >> operation;

  switch (operation){
    case 'd':
      cin >> operationIn;
      switch(operationIn){
        case 'i':
          cin >> key;
          cin.ignore();
          getline(cin, bla);
          //cin.ignore();
          cin >> age;
          RegD entrada;          
          for (int u = 0; u < bla.size(); u++){
            entrada.nome[u] = bla[u];
          }
          entrada.chave = key;
          entrada.idade = age;
          resultInsertion = insereRegistroD(entrada, arq);
          if (resultInsertion == 0)
            cout << "chave ja existente: " << key << endl;
          break;
        
        case 'c':
          cin >> key;
          RegD consulta;
          consulta.chave = key;
          if (consultaCorretaD(consulta, arq) == -1)
            cout << "chave nao encontrada: " << key << endl;
          break;

        case 'r':
          cin >> key;
          RegD remove;
          remove.chave = key;
          if (removeD(remove, arq) == 1){
            removeD(remove, arq);
          }
          else
            cout << "chave nao encontrada: " << key << endl;          
          break;

        case 'p':
          imprimeRegistroD(arq);
          break;

        case 'm':
          cout << fixed << setprecision(1) <<mediaD(arq) << endl;
          break;

        case 'e':
          break;          
      }
  }
  fclose(arq);
}