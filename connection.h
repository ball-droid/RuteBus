#ifndef CONNECTION_H
#define CONNECTION_H

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "BinSTree.h"

#define MAX_CORRIDORS 10

typedef struct{
    char name[50];
    char** halte;      /* Dynamic array of string pointers */
    int jumlah_halte;
    int kapasitas;     /* Allocated capacity */
} Corridor;

extern Corridor corridors[MAX_CORRIDORS];
extern int jumlah_koridor;

void load_data(const char *filename);
void init_bus_data();
void validasi_data();
void build_tree(char origin[100], char destination[100]);

void tampilkan_koridor();
int tambah_halte(const char *nama, int koridor);
int hapus_halte(int koridor, int index);
void simpan_data(const char *filename);

#endif