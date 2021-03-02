#include <stdio.h>
#include <stdlib.h>

struct rankArrayList {
  int size, cap;
  struct rank* ranks;
  struct file* files;
};

struct rank {
  int location;
  int num_pieces;
  int capacity;
  struct piece* array;
};

//mirrors the rank structure
struct file {
  int location;
  int num_pieces;
  int capacity;
  struct piece* array;
};

struct piece {
  int rank, file, id;
};

void addPieceToBoard(struct rankArrayList* board,struct piece* newPiece);
void expandBoard(struct rankArrayList* board);
void cleanBoard(struct rankArrayList* board);
struct rankArrayList* createBoard();
void addPieceToRank(struct rank* rank, struct piece* newPiece);
void addPieceToFile(struct file* file, struct piece* newPiece);
void expandRank(struct rank* curRank);
void expandFile(struct file* curfile);
void cleanRank(struct rank* curRank);
struct rank* createRank();
struct file* createFile();
struct piece* createPiece(int rank,int file,int id);

int main(void) {
  struct rankArrayList* board= createBoard();
   
  //how many pieces in board & creates array (pointer) of pieces
  int numPieces=0;
  scanf("%d", &numPieces);
  struct piece* arrP = malloc(sizeof(struct piece)*numPieces);

  int rank;
  int file;
  //creates the pieces and put them to board
  for(int i=0; i< numPieces; i++){
    scanf("%d %d", &rank, &file);
    struct piece* p = createPiece(rank, file, i);
    arrP[i] = *p; 
    addPieceToBoard(board, p);
  }
  printf("\n");

  for(int k=0; k< numPieces; k++){
    //hold rank of piece we're evaluating
    struct rank temp = board->ranks[arrP[k].rank];
    //hold file of piece we're evaluating
    struct file tempf = board->files[arrP[k].file];
    int np = temp.num_pieces;
    int npf = tempf.num_pieces;
    //at most 4 pieces that can attack one piece
    int id[4];
    int attack = 0;
    
    //
    int f= -1;
    int rf= 1000000000;
    int lf = -1;
    int upr = -1;
    int dor = 1000000000; 
    int rtID = 0;
    int ltID = 0;
    int upID = 0;
    int doID = 0;

    //printf("%d\n",np);
    //goes through ranks and examines files (left right)
    for(int j = 0; j < np; j++){
      if(temp.array[j].file > arrP[k].file){
        f= temp.array[j].file;
        //files left of pieec
        if(f > arrP[k].file && f < rf){
          rf = f;
          //printf("%d", rf);
          rtID = temp.array[j].id;
        }
      }
      else if(temp.array[j].file < arrP[k].file){
        f= temp.array[j].file;
        //files right of piece
        if(f < arrP[k].file && f > lf){
          lf = f;
          ltID = temp.array[j].id;
        }
      } 
    }
    

    //goes through files and examines the ranks (up down)
    for(int j = 0; j < npf; j++){
      if(tempf.array[j].rank > arrP[k].rank){
        f= tempf.array[j].rank;
        //checks the ranks below the piece we're checking
        if(f > arrP[k].rank && f < dor){
          dor = f;
          doID = tempf.array[j].id;
          //printf("%d\n", upID);
        }
      }
      else if(tempf.array[j].rank < arrP[k].rank){
        f= tempf.array[j].rank;
        //checks the ranks above the piece we're checking
        if(f < arrP[k].rank && f > upr){
          upr = f;
          upID = tempf.array[j].id;
        }
      } 
    }

    //printf("(%d)\n", lf);
    //if any of the above variables were altered it means that there's a piece that is "threatning"
    if(lf > 0){
      id[attack] = ltID+1;
      attack++;
    } 
    if(rf != 1000000000){
      id[attack] = rtID+1;
      attack++;
    }
    if(upr > 0){
      id[attack] = upID+1;
      attack++;
    }
    if(dor != 1000000000){
      id[attack] = doID+1;
      attack++;
    }
    printf("%d ",attack);
    for(int i = 0; i < attack; i++){
      printf("%d ",id[i]);
    }
    printf("\n");
  } 


}

void addPieceToBoard(struct rankArrayList* board, struct piece* newPiece){
  if(newPiece->rank >= board->cap || newPiece->file >= board->cap)
    expandBoard(board);
  addPieceToRank(&board->ranks[newPiece->rank], newPiece);
  addPieceToFile(&board->files[newPiece->file], newPiece);
}

void expandBoard(struct rankArrayList* board){
  board->cap= board->cap*2;
  board->ranks = realloc(board->ranks, sizeof(struct rank)*board->cap*2);
  board->files = realloc(board->files, sizeof(struct file)*board->cap*2);
}

void cleanBoard(struct rankArrayList* board){
  //need to free in order from least complicated to most complex 
  free(board->ranks->array);
  free(board->ranks);
  free(board);
  board = NULL;
}

struct rankArrayList* createBoard(){
  struct rankArrayList* rArr= malloc(sizeof(struct rankArrayList));
  rArr->ranks = malloc(5*sizeof(struct rank));
  rArr->files = malloc(5*sizeof(struct file));

  for(int i = 0; i < 5; i++){
    rArr->ranks[i] = *createRank();
    rArr->ranks->location = i;
    rArr->files[i] = *createFile();
    rArr->files->location = i;
  }
  rArr->cap = 5;
  rArr->size = 0;
  return rArr;
}

void addPieceToRank(struct rank* rank, struct piece* newPiece){
  if(rank->num_pieces >= rank->capacity)
    expandRank(rank);

  rank->array[rank->num_pieces]= *newPiece;
  rank->num_pieces++;
}

void addPieceToFile(struct file* file, struct piece* newPiece){
  if(file->num_pieces >= file->capacity)
    expandFile(file);

  file->array[file->num_pieces]= *newPiece;
  file->num_pieces++;
}

void expandRank(struct rank* curRank){
  int length = curRank->capacity;
  curRank->array = realloc(curRank->array, sizeof(struct piece)*length*2);
  curRank->capacity = curRank->capacity*2;
}

void expandFile(struct file* curfile){
  int length = curfile->capacity;
  curfile->array = realloc(curfile->array, sizeof(struct piece)*length*2);
  curfile->capacity = curfile->capacity*2;
}

void cleanRank(struct rank* curRank){
  free(curRank->array);
  free(curRank);
  curRank = NULL;
}

struct rank* createRank(){
  struct rank* r= malloc(sizeof(struct rank));
  r->array = malloc(3*sizeof(struct piece));
  r->capacity = 3;
  r->num_pieces = 0;
  return r;
}

struct file* createFile(){
  struct file* f= malloc(sizeof(struct file));
  f->array = malloc(3*sizeof(struct piece));
  f->capacity = 3;
  f->num_pieces = 0;
  return f;
}

struct piece* createPiece(int rank,int file,int id){
  struct piece* temp = malloc(sizeof(struct piece));
  temp->rank = rank;
  temp->file = file;
  temp->id = id;
  return temp;
}