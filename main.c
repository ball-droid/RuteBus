#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "connection.h"

#define FILE_DATA "data/halte.txt"

/*
 * Modul: Input
 * Membaca string dari stdin, menghapus newline di akhir.
 * Param buf: buffer tujuan, size: ukuran maksimal buffer.
 * Tidak mengembalikan nilai (void).
 * Dipanggil oleh: main() untuk semua input user.
 */
static void baca_string(char *buf, int size) {
    if (fgets(buf, size, stdin) != NULL) {
        size_t len = strlen(buf);
        if (len > 0 && buf[len - 1] == '\n')
            buf[len - 1] = '\0';
    }
}

/*
 * Modul: Main / Entry Point
 * Program utama Sistem Rute Bus Trans Padang.
 * Alur: load_data() → loop menu (cari rute / tambah halte / hapus halte / keluar).
 * Menggunakan modul dari connection.h (build_tree, tambah_halte, hapus_halte, dll).
 */
int main() {
    char asal[100], tujuan[100], nama_halte[100];
    int pilihan, koridor_pilih, halte_pilih;
    char buf[10];

    load_data(FILE_DATA);

    while (1) {
        printf("\n=================== MENU ===================\n");
        printf("1. Cari Rute\n");
        printf("2. Tambah Halte Baru\n");
        printf("3. Hapus Halte\n");
        printf("4. Keluar\n");
        printf("============================================\n");
        printf("Pilihan: ");
        baca_string(buf, sizeof(buf));
        pilihan = atoi(buf);

        switch (pilihan) {
            case 1:
                printf("Masukkan nama halte asal   : ");
                baca_string(asal, sizeof(asal));
                printf("Masukkan nama halte tujuan : ");
                baca_string(tujuan, sizeof(tujuan));
                build_tree(asal, tujuan);
                break;

            case 2:
                tampilkan_koridor();
                printf("Pilih koridor (1-%d): ", jumlah_koridor);
                baca_string(buf, sizeof(buf));
                koridor_pilih = atoi(buf) - 1;

                if (koridor_pilih < 0 || koridor_pilih >= jumlah_koridor) {
                    printf("Koridor tidak valid!\n");
                    break;
                }

                printf("Masukkan nama halte baru: ");
                baca_string(nama_halte, sizeof(nama_halte));

                if (strlen(nama_halte) == 0) {
                    printf("Nama halte tidak boleh kosong!\n");
                    break;
                }

                int hasil = tambah_halte(nama_halte, koridor_pilih);
                if (hasil == 1) {
                    printf("Halte '%s' berhasil ditambahkan ke %s.\n", nama_halte, corridors[koridor_pilih].name);
                    simpan_data(FILE_DATA);
                } else if (hasil == 0) {
                    printf("ERROR: Halte '%s' sudah ada di %s!\n", nama_halte, corridors[koridor_pilih].name);
                } else {
                    printf("ERROR: Gagal menambah halte ke %s.\n", corridors[koridor_pilih].name);
                }
                break;

            case 3:
                tampilkan_koridor();
                printf("Pilih koridor (1-%d): ", jumlah_koridor);
                baca_string(buf, sizeof(buf));
                koridor_pilih = atoi(buf) - 1;

                if (koridor_pilih < 0 || koridor_pilih >= jumlah_koridor) {
                    printf("Koridor tidak valid!\n");
                    break;
                }

                if (corridors[koridor_pilih].jumlah_halte == 0) {
                    printf("Tidak ada halte di %s.\n", corridors[koridor_pilih].name);
                    break;
                }

                printf("\n=== DAFTAR HALTE - %s ===\n", corridors[koridor_pilih].name);
                for (int i = 0; i < corridors[koridor_pilih].jumlah_halte; i++) {
                    printf("%d. %s\n", i + 1, corridors[koridor_pilih].halte[i]);
                }
                printf("==========================\n");
                printf("Pilih nomor halte yang akan dihapus (1-%d): ", corridors[koridor_pilih].jumlah_halte);
                baca_string(buf, sizeof(buf));
                halte_pilih = atoi(buf) - 1;

                if (halte_pilih < 0 || halte_pilih >= corridors[koridor_pilih].jumlah_halte) {
                    printf("Nomor halte tidak valid!\n");
                    break;
                }

                printf("Hapus '%s' dari %s? (y/n): ", corridors[koridor_pilih].halte[halte_pilih], corridors[koridor_pilih].name);
                baca_string(buf, sizeof(buf));
                if (buf[0] != 'y' && buf[0] != 'Y') {
                    printf("Penghapusan dibatalkan.\n");
                    break;
                }

                int hapus = hapus_halte(koridor_pilih, halte_pilih);
                if (hapus == 1) {
                    printf("Halte berhasil dihapus.\n");
                    simpan_data(FILE_DATA);
                } else {
                    printf("Gagal menghapus halte.\n");
                }
                break;

            case 4:
                printf("Terima kasih! Sampai jumpa.\n");
                return 0;

            default:
                printf("Pilihan tidak valid! Masukkan 1-4.\n");
        }
    }

    return 0;
}