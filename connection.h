/*
 * =========================================================================
 * MODUL      : connection.h - Header Modul Koneksi & Manajemen Koridor
 * =========================================================================
 * FUNGSI     : Mendeklarasikan tipe data Corridor, variabel global, dan
 *              fungsi-fungsi untuk mengelola data koridor bus.
 *
 * CARA KERJA :
 *   - Mendefinisikan struct Corridor (nama, array dinamis halte, jumlah)
 *   - Mendeklarasikan array global corridors[] sebagai wadah semua koridor
 *   - Menyediakan interface untuk: load/save file, build tree rute,
 *     CRUD halte, validasi data, dan pencarian rute
 *
 * BERISI     : Deklarasi fungsi (belum implementasi).
 *              Implementasi ada di connection.c
 *
 * DIPAKAI OLEH : main.c (memanggil fungsi-fungsi yang dideklarasikan di sini)
 * MEMAKAI      : BinSTree.h (menggunakan tipe TreeNode untuk tree rute)
 * =========================================================================
 */

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