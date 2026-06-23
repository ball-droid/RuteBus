#ifndef CONNECTION_H
#define CONNECTION_H

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "BinSTree.h"

#define MAX_CORRIDORS 10

/*
 * ==================== MODUL: MANAJEMEN DATA & RUTE ====================
 * Menyediakan tipe data Corridor, variabel global, dan fungsi untuk:
 * - Manajemen data halte (load/save/tambah/hapus)
 * - Pencarian rute (build_tree → BFS → backtrack → cetak)
 * - Validasi data dan utilitas pencarian
 * =======================================================================
 */

/*
 * Corridor — struktur data koridor bus.
 * name[50]: nama koridor (contoh: "Koridor I")
 * halte**: dynamic array of char*, menyimpan nama-nama halte
 * jumlah_halte: jumlah halte terisi saat ini
 * kapasitas: kapasitas yang sudah dialokasikan (grow/shrink otomatis)
 */
typedef struct{
    char name[50];
    char** halte;
    int jumlah_halte;
    int kapasitas;
} Corridor;

/* Variabel global — diisi oleh load_data() / init_bus_data() */
extern Corridor corridors[MAX_CORRIDORS];
extern int jumlah_koridor;

/* ===== Manajemen Data ===== */

/*
 * load_data: Baca file data, isi corridors[] dan jumlah_koridor.
 * Param filename: path ke file halte.txt.
 * Fallback ke init_bus_data() jika file tidak ditemukan.
 * Memanggil: validasi_data() di akhir.
 */
void load_data(const char *filename);

/*
 * init_bus_data: Data bawaan 3 koridor (~53 halte). Fallback.
 */
void init_bus_data();

/*
 * validasi_data: Cetak laporan irisan koridor ke layar.
 * Cek tiap koridor cabang punya irisan dengan koridor utama.
 */
void validasi_data();

/* ===== Pencarian Rute ===== */

/*
 * build_tree: Fungsi utama pencarian rute.
 * Param origin: nama halte asal, destination: nama halte tujuan.
 * Alur:
 *   1. Validasi halte via find_stop()
 *   2. Bangun sequential chain tree
 *   3. bfs_cari() → cari node tujuan
 *   4. backtrack() → trace ke root
 *   5. cetak_rute_tree() → output terminal
 *   6. simpan_tree() → file tree.txt
 *   7. destroyTree() → bersihkan memori
 * Memanggil: find_stop, cari_iris, insert_bentang, bfs_cari,
 *            backtrack, cetak_rute_tree, simpan_tree, destroyTree
 */
void build_tree(char origin[100], char destination[100]);

/* ===== Manajemen Halte ===== */

/*
 * tampilkan_koridor: Cetak daftar koridor + jumlah halte.
 * Membaca: corridors[], jumlah_koridor (global).
 */
void tampilkan_koridor();

/*
 * tambah_halte: Tambah halte baru ke koridor.
 * Param nama: nama halte, koridor: indeks koridor (0-based).
 * Return: 1 sukses, 0 duplikat, -1 error.
 * Memanggil: pastikan_kapasitas (internal).
 */
int tambah_halte(const char *nama, int koridor);

/*
 * hapus_halte: Hapus halte di indeks tertentu.
 * Param koridor: indeks koridor, index: indeks halte (0-based).
 * Return: 1 sukses, <0 error.
 * Menulis ulang: file data/halte.txt via simpan_data().
 */
int hapus_halte(int koridor, int index);

/*
 * simpan_data: Tulis corridors[] ke file (format sama seperti load).
 * Param filename: path file output.
 */
void simpan_data(const char *filename);

#endif