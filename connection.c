#include <strings.h>
#include "connection.h"
#define FILE_TREE "data/tree.txt"

/*
 * ==================== MODUL: IMPLEMENTASI MANAJEMEN DATA & RUTE ====================
 * Implementasi fungsi-fungsi yang dideklarasikan di connection.h dan
 * fungsi-fungsi internal (static) untuk:
 * - Load/save data koridor dari/ke file
 * - Membangun sequential chain tree dan BFS
 * - Manajemen halte (tambah/hapus)
 * ====================================================================================
 */

/* Variabel global — menyimpan seluruh data koridor di memori */
Corridor corridors[MAX_CORRIDORS];
int jumlah_koridor;

/* ===== Fungsi Internal (static) ===== */
int find_stop(char *name, int *kor, int *idx);
int cari_iris(int kor_cabang, int *main_idx, int *cabang_idx);

/*
 * pastikan_kapasitas: Grow array jika kapasitas tidak mencukupi.
 * Param cor: pointer ke Corridor, butuh: jumlah minimum yang diperlukan.
 * Strategi: jika kapasitas 0 → alokasi 4, jika tidak → double sampai cukup.
 * Dipanggil oleh: load_data(), tambah_halte().
 */
static void pastikan_kapasitas(Corridor *cor, int butuh) {
    if (butuh <= cor->kapasitas) return;
    int baru = cor->kapasitas == 0 ? 4 : cor->kapasitas;
    while (baru < butuh) baru *= 2;
    cor->halte = realloc(cor->halte, baru * sizeof(char*));
    cor->kapasitas = baru;
}

/*
 * load_data: Membaca file data halte, mengisi corridors[] dan jumlah_koridor.
 * Param filename: path ke file data/halte.txt.
 * Format file: #NamaKoridor → halte per baris → #KoridorBerikutnya → ...
 * Skip BOM UTF-8. Fallback ke init_bus_data() jika file tidak ditemukan.
 * Memanggil: pastikan_kapasitas(), validasi_data() di akhir.
 * Dipanggil oleh: main().
 */
void load_data(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Gagal membuka file '%s'. Gunakan data default.\n", filename);
        init_bus_data();
        return;
    }

    int kor = -1, idx = 0;
    char buf[200];

    while (fgets(buf, sizeof(buf), fp)) {
        size_t len = strlen(buf);
        if (len > 0 && buf[len - 1] == '\n') buf[len - 1] = '\0';
        if (strlen(buf) == 0) continue;

        /* Skip UTF-8 BOM (Byte Order Mark) jika ada */
        if ((unsigned char)buf[0] == 0xEF && (unsigned char)buf[1] == 0xBB && (unsigned char)buf[2] == 0xBF) {
            memmove(buf, buf + 3, strlen(buf) - 2);
        }

        if (buf[0] == '#') {
            kor++;
            if (kor >= MAX_CORRIDORS) break;
            strcpy(corridors[kor].name, buf + 1);
            corridors[kor].jumlah_halte = 0;
            corridors[kor].halte = NULL;
            corridors[kor].kapasitas = 0;
            idx = 0;
        } else if (kor >= 0 && kor < MAX_CORRIDORS) {
            pastikan_kapasitas(&corridors[kor], idx + 1);
            corridors[kor].halte[idx] = strdup(buf);
            idx++;
            corridors[kor].jumlah_halte = idx;
        }
    }

    jumlah_koridor = (kor >= 0) ? kor + 1 : 0;
    fclose(fp);
    printf("Memuat %d koridor dari '%s'\n", jumlah_koridor, filename);
}

/*
 * validasi_data: Memastikan tiap koridor cabang punya irisan dengan koridor utama.
 * Juga cek tidak ada irisan antar koridor cabang.
 * Membaca: corridors[], jumlah_koridor (global).
 * Memanggil: cari_iris().
 * Dipanggil oleh: load_data().
 */
void validasi_data() {
    int masalah = 0;
    printf("\n=== VALIDASI DATA ===\n");

    if (jumlah_koridor < 1) {
        printf("  ERROR: Tidak ada koridor utama!\n");
        return;
    }

    if (corridors[0].jumlah_halte <= 0) {
        printf("  ERROR: %s tidak memiliki halte!\n", corridors[0].name);
        masalah++;
    }

    for (int k = 1; k < jumlah_koridor; k++) {
        if (corridors[k].jumlah_halte <= 0) {
            printf("  ERROR: %s tidak memiliki halte!\n", corridors[k].name);
            masalah++;
            continue;
        }

        int mi, ci;
        if (!cari_iris(k, &mi, &ci)) {
            printf("  WARNING: %s TIDAK beririsan dengan koridor utama!\n"
                   "           - Halte pertama: '%s'\n"
                   "           - Halte terakhir: '%s'\n",
                   corridors[k].name,
                   corridors[k].halte[0],
                   corridors[k].halte[corridors[k].jumlah_halte - 1]);
            masalah++;
        } else {
            printf("  OK: %s beririsan di '%s'\n", corridors[k].name,
                   corridors[0].halte[mi]);
        }

        for (int k2 = k + 1; k2 < jumlah_koridor; k2++) {
            for (int i = 0; i < corridors[k].jumlah_halte; i++) {
                for (int j = 0; j < corridors[k2].jumlah_halte; j++) {
                    if (strcasecmp(corridors[k].halte[i], corridors[k2].halte[j]) == 0) {
                        printf("  WARNING: '%s' ada di %s dan %s (hanya koridor utama yang boleh beririsan)\n",
                               corridors[k].halte[i], corridors[k].name, corridors[k2].name);
                        masalah++;
                    }
                }
            }
        }
    }

    if (masalah == 0)
        printf("  Semua data valid.\n");
    else
        printf("  Ditemukan %d masalah.\n", masalah);
    printf("============================\n");
}

/*
 * init_bus_data: Data bawaan 3 koridor bus Trans Padang (~53 halte).
 * Fallback jika file data/halte.txt tidak ditemukan.
 * Menulis: corridors[], jumlah_koridor (global).
 * Dipanggil oleh: load_data().
 */
void init_bus_data() {
    int kor = 0;
    strcpy(corridors[kor].name, "Koridor I");
    corridors[kor].kapasitas = 17;
    corridors[kor].halte = malloc(17 * sizeof(char*));
    corridors[kor].halte[0] = strdup("Pool Damri");
    corridors[kor].halte[1] = strdup("RTH Imam Bonjol Padang");
    corridors[kor].halte[2] = strdup("GOR H Agus Salim");
    corridors[kor].halte[3] = strdup("Masjid Raya Sumatera Barat");
    corridors[kor].halte[4] = strdup("DPRD Provinsi Sumatera Barat");
    corridors[kor].halte[5] = strdup("Basko Grand Mall");
    corridors[kor].halte[6] = strdup("Simpang Labor");
    corridors[kor].halte[7] = strdup("Universitas Negeri Padang Air Tawar");
    corridors[kor].halte[8] = strdup("Simpang Tunggul Hitam");
    corridors[kor].halte[9] = strdup("Simpang GIA");
    corridors[kor].halte[10] = strdup("Asrama Haji Padang");
    corridors[kor].halte[11] = strdup("Perumahan Monang Indah");
    corridors[kor].halte[12] = strdup("SMP Negeri 15 Padang");
    corridors[kor].halte[13] = strdup("Fakultas Kedokteran Universitas Negeri Padang");
    corridors[kor].halte[14] = strdup("Kayu Kalek");
    corridors[kor].halte[15] = strdup("Kompleks Mega Permai");
    corridors[kor].halte[16] = strdup("Terminal Anak Air");
    corridors[kor].jumlah_halte = 17;
    kor = 1;
    strcpy(corridors[kor].name, "Koridor II");
    corridors[kor].kapasitas = 15;
    corridors[kor].halte = malloc(15 * sizeof(char*));
    corridors[kor].halte[0] = strdup("RTH Imam Bonjol Padang");
    corridors[kor].halte[1] = strdup("Masjid Nurul Iman");
    corridors[kor].halte[2] = strdup("Pasar Gaung");
    corridors[kor].halte[3] = strdup("Toko Abd Gani");
    corridors[kor].halte[4] = strdup("Masjid Al Qahaar");
    corridors[kor].halte[5] = strdup("Pabrik Karet Sungai Beremas");
    corridors[kor].halte[6] = strdup("Pantai Nirwana");
    corridors[kor].halte[7] = strdup("Kantor VTS Bungus");
    corridors[kor].halte[8] = strdup("Pisang Salay Citra Minang");
    corridors[kor].halte[9] = strdup("SDN 06 Cindakir");
    corridors[kor].halte[10] = strdup("Cecen Cellular");
    corridors[kor].halte[11] = strdup("Naldi Cell/Kedai Aura");
    corridors[kor].halte[12] = strdup("Puskesmas Pembantu Bungus");
    corridors[kor].halte[13] = strdup("SDN 12 Teluk Kabung");
    corridors[kor].halte[14] = strdup("Simpang Sungai Pisang");
    corridors[kor].jumlah_halte = 15;
    kor = 2;
    strcpy(corridors[kor].name, "Koridor III");
    corridors[kor].kapasitas = 21;
    corridors[kor].halte = malloc(21 * sizeof(char*));
    corridors[kor].halte[0] = strdup("Terminal Anak Air");
    corridors[kor].halte[1] = strdup("Simpang Kalumpang");
    corridors[kor].halte[2] = strdup("SBPU Anak Air");
    corridors[kor].halte[3] = strdup("Simpang Koto Tuo");
    corridors[kor].halte[4] = strdup("Simpang Aia Pacah Permai");
    corridors[kor].halte[5] = strdup("Simpang Maransi");
    corridors[kor].halte[6] = strdup("FKG Baiturrahmah");
    corridors[kor].halte[7] = strdup("Universitas Terbuka");
    corridors[kor].halte[8] = strdup("SPBU Taruko");
    corridors[kor].halte[9] = strdup("Kantor Camat Kuranji");
    corridors[kor].halte[10] = strdup("Simpang Kalumbuk");
    corridors[kor].halte[11] = strdup("Polsek/ Pukesmas Kuranji");
    corridors[kor].halte[12] = strdup("Utama Furniture");
    corridors[kor].halte[13] = strdup("Masjid Jamiatul Huda");
    corridors[kor].halte[14] = strdup("RS Semen Padang Hospital");
    corridors[kor].halte[15] = strdup("Poliklinik Fitria");
    corridors[kor].halte[16] = strdup("Pool ALS");
    corridors[kor].halte[17] = strdup("Simpang Lubeg");
    corridors[kor].halte[18] = strdup("Simpang Beringin");
    corridors[kor].halte[19] = strdup("Pampangan");
    corridors[kor].halte[20] = strdup("Lantamal");
    corridors[kor].jumlah_halte = 21;
    jumlah_koridor = kor + 1;
}

/*
 * ===== FUNGSI BANTU (HELPERS) =====
 */

/*
 * find_stop: Mencari nama halte di seluruh koridor.
 * Param name: nama halte yang dicari.
 * Param kor, idx: output — pointer untuk menyimpan hasil (koridor, index).
 * Return: 1 jika ditemukan, 0 jika tidak.
 * Membaca: corridors[], jumlah_koridor (global).
 * Dipanggil oleh: build_tree(), cetak_rute_tree(), cetak_rute().
 */
int find_stop(char *name, int *kor, int *idx) {
    for (int k = 0; k < MAX_CORRIDORS; k++)
        for (int i = 0; i < corridors[k].jumlah_halte; i++)
            if (strcasecmp(corridors[k].halte[i], name) == 0) { *kor = k; *idx = i; return 1; }
    return 0;
}

/*
 * cari_iris: Mencari titik irisan antara koridor cabang dengan koridor utama.
 * Param kor_cabang: indeks koridor cabang.
 * Param main_idx, cabang_idx: output — index di koridor utama dan cabang.
 * Return: 1 jika ditemukan irisan, 0 jika tidak.
 * Asumsi: irisan hanya terjadi di halte PERTAMA atau TERAKHIR koridor cabang.
 * Membaca: corridors[] (global).
 * Dipanggil oleh: validasi_data(), build_tree().
 */
int cari_iris(int kor_cabang, int *main_idx, int *cabang_idx) {
    for (int i = 0; i < corridors[0].jumlah_halte; i++)
        if (strcasecmp(corridors[0].halte[i], corridors[kor_cabang].halte[0]) == 0)
            { *main_idx = i; *cabang_idx = 0; return 1; }
    int last = corridors[kor_cabang].jumlah_halte - 1;
    for (int i = 0; i < corridors[0].jumlah_halte; i++)
        if (strcasecmp(corridors[0].halte[i], corridors[kor_cabang].halte[last]) == 0)
            { *main_idx = i; *cabang_idx = last; return 1; }
    return 0;
}

/*
 * ===== STACK UNTUK BACKTRACK =====
 * stack_rute: menyimpan urutan node dari asal ke tujuan.
 * stack_transit: menyimpan node transit (tidak dipakai untuk output,
 *                tetapi tetap diisi untuk dokumentasi).
 */
#define MAX_STACK 300
static TreeNode *stack_rute[MAX_STACK];
static int top_rute = -1;
static TreeNode *stack_transit[MAX_STACK];
static int top_transit = -1;

static void push_rute(TreeNode *n)   { stack_rute[++top_rute] = n; }
static void push_transit(TreeNode *n){ stack_transit[++top_transit] = n; }

/*
 * ===== SEQUENTIAL CHAIN TREE =====
 * Tree dibangun sebagai right-skewed chain:
 * - rightChild = node berikutnya dalam rute
 * - parent = node sebelumnya dalam rute
 * - Urutan node = urutan rute (bukan urutan ID seperti BST)
 */

/*
 * cari_ekor: Mencari node terakhir dalam rantai kanan.
 * Param root: root tree.
 * Return: pointer ke node terakhir (rightChild == NULL).
 * Dipanggil oleh: insert_bentang().
 */
static TreeNode* cari_ekor(TreeNode *root) {
    if (root == NULL) return NULL;
    while (root->rightChild != NULL) root = root->rightChild;
    return root;
}

/*
 * node_ada: Memeriksa apakah nama halte sudah ada di sequential chain.
 * Param root: root tree, name: nama yang dicari.
 * Return: 1 jika sudah ada, 0 jika belum.
 * Dipanggil oleh: insert_bentang() — untuk cegah duplikat.
 */
static int node_ada(TreeNode *root, const char *name) {
    TreeNode *cur = root;
    while (cur != NULL) {
        if (strcasecmp(cur->name, name) == 0) return 1;
        cur = cur->rightChild;
    }
    return 0;
}

/*
 * insert_bentang: Menambahkan node-node ke sequential chain.
 * Param root: pointer ke root tree (bisa berubah jika root NULL).
 * Param kor: indeks koridor.
 * Param dari, ke: range index halte di koridor (bisa maju/mundur).
 * Param jm_idx: index junction (-1 jika tidak ada transit di segmen ini).
 * Proses: untuk setiap halte di range, jika belum ada di chain,
 *         buat node baru dan append ke ekor chain.
 * Memanggil: createNode(), cari_ekor(), node_ada().
 * Dipanggil oleh: build_tree().
 */
static void insert_bentang(TreeNode **root, int kor, int dari, int ke, int jm_idx) {
    int step = (dari <= ke) ? 1 : -1;
    for (int i = dari; i != ke + step; i += step) {
        if (corridors[kor].halte[i] == NULL) continue;
        if (node_ada(*root, corridors[kor].halte[i])) continue;

        int jlr = (i == jm_idx) ? 1 : 0;
        TreeNode *node = createNode(kor * 100 + i, corridors[kor].halte[i], jlr);

        if (*root == NULL) {
            *root = node;
        } else {
            TreeNode *ekor = cari_ekor(*root);
            ekor->rightChild = node;
            node->parent = ekor;
        }
    }
}

/*
 * ===== BFS (BREADTH-FIRST SEARCH) =====
 * Mencari node tujuan dalam sequential chain tree.
 * Karena tree adalah right-skewed chain, BFS mengunjungi node
 * dalam urutan yang sama dengan urutan rute.
 */

/*
 * bfs_cari: Mencari node berdasarkan nama di tree.
 * Param root: root tree, target: nama halte yang dicari.
 * Return: pointer ke node jika ditemukan, NULL jika tidak.
 * Menggunakan: static queue (TreeNode* q[MAX_Q]).
 * Dipanggil oleh: build_tree().
 */
#define MAX_Q 500
static TreeNode* bfs_cari(TreeNode *root, const char *target) {
    static TreeNode *q[MAX_Q];
    int f = 0, r = 0;
    q[r++] = root;
    while (f < r) {
        TreeNode *cur = q[f++];
        if (strcasecmp(cur->name, target) == 0) return cur;
        if (cur->leftChild)  q[r++] = cur->leftChild;
        if (cur->rightChild) q[r++] = cur->rightChild;
    }
    return NULL;
}

/*
 * ===== BACKTRACK =====
 * Menelusuri rute dari node tujuan ke node asal via parent pointer.
 * Hasil disimpan ke stack_rute[] untuk dibaca oleh cetak_rute_tree().
 */

/*
 * backtrack: Tracing dari tujuan ke asal via parent chain.
 * Param dest: node tujuan (hasil bfs_cari).
 * Proses: mulai dari dest, naik ke parent, push ke stack_rute.
 *         Node transit (jalur==1) juga di-push ke stack_transit.
 * Memanggil: push_rute(), push_transit().
 * Dipanggil oleh: build_tree().
 */
static void backtrack(TreeNode *dest) {
    top_rute = top_transit = -1;
    TreeNode *cur = dest;
    while (cur != NULL) {
        push_rute(cur);
        if (cur->jalur == 1 && cur->parent != NULL)
            push_transit(cur->parent);
        cur = cur->parent;
    }
}

/*
 * ===== CETAK RUTE =====
 * Membaca stack_rute[] hasil backtrack, mencetak rute ke terminal.
 * Mendeteksi transit dengan membandingkan koridor antar node.
 */

/*
 * cetak_rute_tree: Mencetak rute dari stack ke terminal.
 * Param asal, tujuan: nama halte (untuk header dan penanda tujuan).
 * Proses: iterasi stack_rute dari top (asal) ke bottom (tujuan).
 *         Cetak nama halte + koridor + deteksi transit.
 * Memanggil: find_stop() untuk deteksi koridor.
 * Dipanggil oleh: build_tree().
 */
static void cetak_rute_tree(const char *asal, const char *tujuan) {
    printf("\n========================================\n");
    printf("  RUTE: %s  -->  %s\n", asal, tujuan);
    printf("========================================\n");

    for (int i = top_rute; i >= 0; i--) {
        TreeNode *node = stack_rute[i];
        int kor = node->id / 100;

        if (i == top_rute)
            printf("  %s (Koridor %d)\n", node->name, kor + 1);
        else if (strcasecmp(node->name, tujuan) == 0)
            printf("  %s (tujuan)\n", node->name);
        else
            printf("  %s\n", node->name);

        if (i > 0) {
            TreeNode *next = stack_rute[i - 1];
            int next_kor = next->id / 100;
            if (next_kor != kor) {
                printf("  -- TRANSIT: %s (Koridor %d --> Koridor %d) --\n",
                       node->name, kor + 1, next_kor + 1);
            }
        }
    }
}

/*
 * cetak_rute: (TIDAK DIPAKAI — digantikan cetak_rute_tree)
 * Mencetak rute via navigasi array langsung (koridor[idx]).
 * Dipertahankan sebagai referensi implementasi awal.
 */
static void cetak_rute(const char *asal, const char *tujuan) {
    int ak, ai, tk, ti;
    find_stop((char*)asal, &ak, &ai);
    find_stop((char*)tujuan, &tk, &ti);

    printf("\n========================================\n");
    printf("  RUTE: %s  -->  %s\n", asal, tujuan);
    printf("========================================\n");

    if (ak == tk) {
        printf("  %s (Koridor %d)\n", asal, ak + 1);
        int step = (ai < ti) ? 1 : -1;
        for (int i = ai + step; i != ti; i += step)
          printf("  %s\n", corridors[ak].halte[i]);
        printf("  %s (tujuan)\n", tujuan);
        return;
    }

    if (ak == 0) {
        int mi, ci;
        if (!cari_iris(tk, &mi, &ci)) {
            printf("  Tidak ada koneksi antar koridor.\n"); return;
        }
        printf("  %s (Koridor %d)\n", asal, ak + 1);
        int step = (ai < mi) ? 1 : -1;
        for (int i = ai + step; i != mi + step; i += step)
            printf("  %s\n", corridors[0].halte[i]);
        printf("  -- TRANSIT: %s (Koridor %d --> Koridor %d) --\n",
               corridors[0].halte[mi], 1, tk + 1);
        step = (ci < ti) ? 1 : -1;
        for (int i = ci + step; i != ti; i += step)
            printf("  %s\n", corridors[tk].halte[i]);
        printf("  %s (tujuan)\n", tujuan);
    }
    else if (tk == 0) {
        int mi, ci;
        if (!cari_iris(ak, &mi, &ci)) {
            printf("  Tidak ada koneksi antar koridor.\n"); return;
        }
        printf("  %s (Koridor %d)\n", asal, ak + 1);
        int step = (ai < ci) ? 1 : -1;
        for (int i = ai + step; i != ci + step; i += step)
            printf("  %s\n", corridors[ak].halte[i]);
        printf("  -- TRANSIT: %s (Koridor %d --> Koridor %d) --\n",
               corridors[0].halte[mi], ak + 1, 1);
        step = (mi < ti) ? 1 : -1;
        for (int i = mi + step; i != ti; i += step)
            printf("  %s\n", corridors[0].halte[i]);
        printf("  %s (tujuan)\n", tujuan);
    }
    else {
        int m1, c1, m2, c2;
        if (!cari_iris(ak, &m1, &c1) || !cari_iris(tk, &m2, &c2)) {
            printf("  Tidak ada koneksi antar koridor.\n"); return;
        }
        printf("  %s (Koridor %d)\n", asal, ak + 1);
        int step = (ai < c1) ? 1 : -1;
        for (int i = ai + step; i != c1 + step; i += step)
            printf("  %s\n", corridors[ak].halte[i]);
        printf("  -- TRANSIT: %s (Koridor %d --> Koridor %d) --\n",
               corridors[0].halte[m1], ak + 1, 1);
        step = (m1 < m2) ? 1 : -1;
        for (int i = m1 + step; i != m2 + step; i += step)
            printf("  %s\n", corridors[0].halte[i]);
        printf("  -- TRANSIT: %s (Koridor %d --> Koridor %d) --\n",
               corridors[0].halte[m2], 1, tk + 1);
        step = (c2 < ti) ? 1 : -1;
        for (int i = c2 + step; i != ti; i += step)
            printf("  %s\n", corridors[tk].halte[i]);
        printf("  %s (tujuan)\n", tujuan);
    }
}

/* ===== BANGUN TREE (rute-based, raw ID) ===== */
void build_tree(char origin[100], char destination[100]) {
    int asal_kor, asal_idx, tujuan_kor, tujuan_idx;

    if (!find_stop(origin, &asal_kor, &asal_idx)) {
        printf("Halte asal '%s' tidak ditemukan\n", origin); return;
    }
    if (!find_stop(destination, &tujuan_kor, &tujuan_idx)) {
        printf("Halte tujuan '%s' tidak ditemukan\n", destination); return;
    }

    /* Root = origin */
    TreeNode *root = createNode(asal_kor * 100 + asal_idx, origin, 0);

    /* Route-based tree: insert stops sepanjang rute sebagai sequential chain.
       Urutan: dari asal menuju transit, lalu dari transit menuju tujuan. */
    if (asal_kor == tujuan_kor) {
        if (asal_idx < tujuan_idx)
            insert_bentang(&root, asal_kor, asal_idx + 1, tujuan_idx, -1);
        else
            insert_bentang(&root, asal_kor, asal_idx - 1, tujuan_idx, -1);
    }
    else if (asal_kor == 0) {
        int mi, ci;
        if (cari_iris(tujuan_kor, &mi, &ci)) {
            int jm = (ci == 0) ? 0 : (ci == corridors[tujuan_kor].jumlah_halte - 1) ? ci : -1;
            if (asal_idx < mi)
                insert_bentang(&root, 0, asal_idx + 1, mi, mi);
            else if (asal_idx > mi)
                insert_bentang(&root, 0, asal_idx - 1, mi, mi);

            if (ci < tujuan_idx)
                insert_bentang(&root, tujuan_kor, ci + 1, tujuan_idx, jm);
            else
                insert_bentang(&root, tujuan_kor, ci - 1, tujuan_idx, jm);
        }
    }
    else if (tujuan_kor == 0) {
        int mi, ci;
        if (cari_iris(asal_kor, &mi, &ci)) {
            int jm = (ci == 0) ? 0 : (ci == corridors[asal_kor].jumlah_halte - 1) ? ci : -1;
            if (asal_idx < ci)
                insert_bentang(&root, asal_kor, asal_idx + 1, ci, jm);
            else
                insert_bentang(&root, asal_kor, asal_idx - 1, ci, jm);

            if (mi < tujuan_idx)
                insert_bentang(&root, 0, mi + 1, tujuan_idx, -1);
            else if (mi > tujuan_idx)
                insert_bentang(&root, 0, mi - 1, tujuan_idx, -1);
        }
    }
    else {
        int m1, c1, m2, c2;
        if (cari_iris(asal_kor, &m1, &c1) && cari_iris(tujuan_kor, &m2, &c2)) {
            int jm1 = (c1 == 0) ? 0 : (c1 == corridors[asal_kor].jumlah_halte - 1) ? c1 : -1;
            if (asal_idx < c1)
                insert_bentang(&root, asal_kor, asal_idx + 1, c1, jm1);
            else
                insert_bentang(&root, asal_kor, asal_idx - 1, c1, jm1);

            if (m1 < m2)
                insert_bentang(&root, 0, m1 + 1, m2, m2);
            else if (m1 > m2)
                insert_bentang(&root, 0, m1 - 1, m2, m2);

            int jm2 = (c2 == 0) ? 0 : (c2 == corridors[tujuan_kor].jumlah_halte - 1) ? c2 : -1;
            if (c2 < tujuan_idx)
                insert_bentang(&root, tujuan_kor, c2 + 1, tujuan_idx, jm2);
            else
                insert_bentang(&root, tujuan_kor, c2 - 1, tujuan_idx, jm2);
        }
    }

    /* Cari tujuan di tree via BFS, backtrack, lalu cetak rute */
    TreeNode *dest_node = bfs_cari(root, destination);
    if (dest_node != NULL) {
        backtrack(dest_node);
        cetak_rute_tree(origin, destination);
    }

    /* Simpan tree ke file */
    simpan_tree(FILE_TREE, root);

    destroyTree(root);
}

/*
 * ===== MANAJEMEN HALTE =====
 */

/*
 * tampilkan_koridor: Mencetak daftar koridor beserta jumlah halte.
 * Membaca: corridors[], jumlah_koridor (global).
 * Dipanggil oleh: main() — fitur Tambah/Hapus Halte.
 */
void tampilkan_koridor() {
    printf("\n=== DAFTAR KORIDOR ===\n");
    for (int i = 0; i < jumlah_koridor; i++) {
        printf("%d. %s (%d halte)\n", i + 1, corridors[i].name, corridors[i].jumlah_halte);
    }
    printf("======================\n");
}

/*
 * tambah_halte: Menambah halte baru ke koridor.
 * Param nama: nama halte, koridor: indeks koridor (0-based).
 * Proses: validasi indeks → cek duplikat (strcasecmp) →
 *         pastikan_kapasitas → strdup → jumlah_halte++.
 * Return: 1 sukses, 0 duplikat, -1 error indeks.
 * Memanggil: pastikan_kapasitas().
 * Dipanggil oleh: main() — fitur Tambah Halte.
 */
int tambah_halte(const char *nama, int koridor) {
    if (koridor < 0 || koridor >= jumlah_koridor) return -1;

    for (int i = 0; i < corridors[koridor].jumlah_halte; i++) {
        if (strcasecmp(corridors[koridor].halte[i], nama) == 0)
            return 0;
    }

    pastikan_kapasitas(&corridors[koridor], corridors[koridor].jumlah_halte + 1);

    int idx = corridors[koridor].jumlah_halte;
    corridors[koridor].halte[idx] = strdup(nama);
    corridors[koridor].jumlah_halte++;

    return 1;
}

/*
 * hapus_halte: Menghapus halte di indeks tertentu.
 * Param koridor: indeks koridor, index: indeks halte (0-based).
 * Proses: validasi indeks → free string → geser array ke kiri →
 *         jumlah_halte-- → shrink jika < 25% kapasitas terpakai.
 * Return: 1 sukses, -1 error koridor, -2 error index.
 * Dipanggil oleh: main() — fitur Hapus Halte.
 */
int hapus_halte(int koridor, int index) {
    if (koridor < 0 || koridor >= jumlah_koridor) return -1;
    if (index < 0 || index >= corridors[koridor].jumlah_halte) return -2;

    free(corridors[koridor].halte[index]);

    for (int i = index; i < corridors[koridor].jumlah_halte - 1; i++) {
        corridors[koridor].halte[i] = corridors[koridor].halte[i + 1];
    }

    corridors[koridor].halte[corridors[koridor].jumlah_halte - 1] = NULL;
    corridors[koridor].jumlah_halte--;

    if (corridors[koridor].kapasitas > 8 &&
        corridors[koridor].jumlah_halte < corridors[koridor].kapasitas / 4) {
        int baru = corridors[koridor].kapasitas / 2;
        corridors[koridor].halte = realloc(corridors[koridor].halte, baru * sizeof(char*));
        corridors[koridor].kapasitas = baru;
    }

    return 1;
}

/*
 * simpan_data: Menulis data koridor dan halte ke file.
 * Param filename: path file output (biasanya "data/halte.txt").
 * Format: #NamaKoridor → halte per baris → #KoridorBerikutnya → ...
 * Membaca: corridors[], jumlah_koridor (global).
 * Dipanggil oleh: main() — setelah tambah/hapus halte.
 */
void simpan_data(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Gagal membuka file '%s' untuk menulis.\n", filename);
        return;
    }

    for (int i = 0; i < jumlah_koridor; i++) {
        fprintf(fp, "#%s\n", corridors[i].name);
        for (int j = 0; j < corridors[i].jumlah_halte; j++) {
            fprintf(fp, "%s\n", corridors[i].halte[j]);
        }
    }

    fclose(fp);
    printf("Data berhasil disimpan ke '%s'\n", filename);
}