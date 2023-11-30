#include <iostream>
#include <stdio.h>
#include <string.h>

struct reg_livro {
  char titulo[50];
  char autor[50];
  float nota;
  char status;
};

int tamanho(FILE *);
void cadastrar(FILE *);
void consultar(FILE *);
void geraarqtxt(FILE *);
void excluir(FILE *);

int main() {
  int op;
  FILE *arq;

  arq = fopen("dados_livros.dat", "rb+");
  if (arq == NULL) {
    arq = fopen("dados_livros.dat", "wb+");
    if (arq == NULL) {
      printf("Falha ao abrir o arquivo!\n");
      return 1;
    }
  }

  do {
    printf("\n======= BIBLIOTECA ======= \n");
    printf("1. Cadastrar livro\n");
    printf("2. Consultar por código\n");
    printf("3. Gerar arquivo de texto\n");
    printf("4. Excluir registro\n");
    printf("5. Sair\n");
    printf("======Livros cadastrados: %d======\n", tamanho(arq));
    printf("Opção: ");
    if (scanf("%d", &op) != 1) {
      return 1;
    }

    switch (op) {
    case 1:
      cadastrar(arq);
      break;
    case 2:
      consultar(arq);
      break;
    case 3:
      geraarqtxt(arq);
      break;
    case 4:
      excluir(arq);
      break;
    case 5:
      fclose(arq);
      return 0;
    default:
      printf("Opção inválida.\n");
      break;
    }
  } while (op != 5);
}

int tamanho(FILE *arq) {
  fseek(arq, 0, SEEK_END);
  long tamanho_bytes = ftell(arq);
  rewind(arq);
  return tamanho_bytes / sizeof(struct reg_livro);
}

void cadastrar(FILE *arq) {
  struct reg_livro livro;
  char confirma;
  livro.status = ' ';

  printf("\nNúmero do registro: %d\n", tamanho(arq) + 1);

  printf("Título do livro..........: ");
  if (scanf(" %[^\n]s", livro.titulo) != 1) {
    return;
  }
  getchar();

  printf("Autor do livro...........: ");
  if (scanf(" %[^\n]s", livro.autor) != 1) {
    return;
  }
  getchar();

  printf("Nota do livro (0-10).....: ");
  if (scanf("%f", &livro.nota) != 1) {
    return;
  }
  getchar();

  printf("\nConfirmar <s/n>: ");
  if (scanf(" %c", &confirma) != 1) {
    return;
  }

  if (toupper(confirma) == 'S') {
    printf("\nCadastro realizado com sucesso.\n\n");
    fseek(arq, 0, SEEK_END);
    fwrite(&livro, sizeof(struct reg_livro), 1, arq);
  }
}

void consultar(FILE *arq) {
  struct reg_livro livro;
  int nr;
  printf("\nCódigo do registro: ");
  if (scanf("%d", &nr) != 1) {
    return;
  }

  if ((nr <= tamanho(arq)) && (nr > 0)) {
    fseek(arq, (nr - 1) * sizeof(struct reg_livro), SEEK_SET);
    if (fread(&livro, sizeof(struct reg_livro), 1, arq) == 1) {
      if (livro.status == ' ') {
        printf("\nTítulo do livro......: %s", livro.titulo);
        printf("\nAutor do livro.......: %s", livro.autor);
        printf("\nNota do livro........: %.2f\n\n", livro.nota);
      } else {
        printf("\nRegistro excluído.\n");
      }
    }
  } else {
    printf("\nNúmero de registro inválido.\n");
  }
}

void geraarqtxt(FILE *arq) {
  char nomearq[50];
  printf("Nome do arquivo de texto: ");
  if (scanf("%s", nomearq) != 1) {
    return;
  }
  strcat(nomearq, ".txt");
  FILE *arqtxt = fopen(nomearq, "w");

  if (!arqtxt) {
    printf("Não foi possível criar esse arquivo.\n");
    return;
  }

  fprintf(arqtxt, "Título                Autor            Nota    Status\n");
  fprintf(arqtxt, "====================================================\n");

  int nr;
  struct reg_livro livro;

  for (nr = 0; nr < tamanho(arq); nr++) {
    fseek(arq, nr * sizeof(struct reg_livro), SEEK_SET);
    if (fread(&livro, sizeof(struct reg_livro), 1, arq) == 1) {
      fprintf(arqtxt, "%-20s%-16s%-8.2f- %c\n", livro.titulo, livro.autor,
              livro.nota, livro.status);
    }
  }

  fprintf(arqtxt, "====================================================\n");
  fclose(arqtxt);
}

void excluir(FILE *arq) {
  struct reg_livro livro;
  char confirma;
  int nr;

  printf("\nCódigo do registro: ");
  if (scanf("%d", &nr) != 1) {
    return;
  }

  if ((nr <= tamanho(arq)) && (nr > 0)) {
    fseek(arq, (nr - 1) * sizeof(struct reg_livro), SEEK_SET);
    if (fread(&livro, sizeof(struct reg_livro), 1, arq) == 1) {
      if (livro.status == ' ') {
        printf("\nTítulo do livro......: %s", livro.titulo);
        printf("\nAutor do livro.......: %s", livro.autor);
        printf("\nNota do livro........: %.2f\n", livro.nota);
        printf("\nConfirmar? <s/n>: ");
        fflush(stdin);
        if (scanf(" %c", &confirma) != 1) {
          return;
        }

        if (toupper(confirma) == 'S') {
          printf("\nExclusão realizada com sucesso.\n\n");
          fseek(arq, (nr - 1) * sizeof(struct reg_livro), SEEK_SET);
          livro.status = '*';
          fwrite(&livro, sizeof(struct reg_livro), 1, arq);
        }
      }
    } else {
      printf("Registro inexistente.\n");
    }
  }
}
