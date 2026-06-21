# Struktur Data

## 1. Array of Struct

### Informasi
Array of Struct adalah struktur data yang menggabungkan array (kumpulan data dengan tipe sama) dan struct (kumpulan data dengan tipe berbeda dalam satu kesatuan). Pada program ini, array of struct digunakan untuk menampung grup-grup data beserta atributnya.

`c
#define MAX_GROUPS 10

typedef struct {
    char name[50];       // Nama grup
    char** items;        // Dynamic array of string pointers
    int count;           // Jumlah item terisi
    int capacity;        // Kapasitas yang sudah dialokasi
} Group;

Group groups[MAX_GROUPS];
`

### Peran
- Menyimpan seluruh data master di memori selama program berjalan
- Menjembatani data dari file ke dalam program (input)
- Menyediakan data untuk ditampilkan ke user (output)
- Menampung perubahan data dari proses tambah dan hapus

### Analisis Proses, Input, dan Output
- **Proses:** Data dari file dibaca baris per baris, kemudian disimpan ke dalam array of struct. Setiap baris # menandai grup baru, baris biasa menjadi item dalam grup.
- **Input:** File eksternal dengan format: #NamaGrup diikuti item-itemnya.
- **Output:** Data ditampilkan ke layar dalam bentuk daftar grup dan item, serta ditulis kembali ke file saat ada perubahan.

---

## 2. Dynamic Array

### Informasi
Dynamic array adalah array yang ukurannya dapat berubah secara otomatis saat program berjalan. Berbeda dengan array statis yang ukurannya tetap sejak kompilasi, dynamic array dapat tumbuh (grow) saat item ditambah dan mengecil (shrink) saat item dihapus.

`c
char** items;    // Pointer ke array of pointers
int count;       // Jumlah terpakai
int capacity;    // Kapasitas teralokasi
`

### Peran
- Menyimpan daftar item (string) dalam setiap grup secara fleksibel
- Menghilangkan batas maksimal jumlah item per grup
- Mengoptimalkan penggunaan memori dengan menyesuaikan kapasitas sesuai kebutuhan

### Analisis Proses, Input, dan Output
- **Proses:** Saat item ditambahkan, jika kapasitas penuh maka array digandakan ukurannya. Saat item dihapus dan kapasitas terlalu longgar, array dikecilkan setengahnya. Item baru disimpan di akhir array. Item yang dihapus menyebabkan item setelahnya bergeser ke kiri.
- **Input:** Item baru dari user (keyboard) saat proses tambah.
- **Output:** Daftar item ditampilkan ke layar dalam urutan sesuai array, dan ditulis ke file dengan urutan yang sama.

---

## 3. Binary Search Tree (BST)

### Informasi
Binary Search Tree adalah struktur data pohon biner di mana setiap node memiliki paling banyak dua anak (kiri dan kanan). Node di kiri memiliki nilai lebih kecil dari parent, node di kanan memiliki nilai lebih besar. Setiap node menyimpan data dan pointer ke anak kiri, anak kanan, serta orangtua.

`c
typedef struct Node {
    int id;                  // Kunci pengurutan
    char name[100];          // Data
    int flag;                // Penanda khusus
    struct Node* left;       // Anak kiri
    struct Node* right;      // Anak kanan
    struct Node* parent;     // Orangtua
} Node;
`

### Peran
- Membangun representasi sementara dari suatu sesi/rute berdasarkan data di array of struct
- Menyusun node secara terurut berdasarkan id untuk memudahkan pencarian
- Bersifat sementara — dibuat saat diperlukan dan dihancurkan setelah selesai digunakan

### Analisis Proses, Input, dan Output
- **Proses:** BST dibangun dengan mengambil data dari array of struct. Node dimasukkan satu per satu menggunakan aturan BST (berdasarkan id). Setelah selesai digunakan, seluruh node dihapus dari memori.
- **Input:** Data dari array of struct (hasil dari proses input sebelumnya).
- **Output:** Struktur pohon di memori yang siap diproses lebih lanjut (misal untuk pencarian).

---

## 4. Queue

### Informasi
Queue adalah struktur data yang menerapkan prinsip FIFO (First In, First Out). Elemen yang pertama masuk akan menjadi yang pertama keluar. Implementasi menggunakan array statis dengan dua pointer penanda depan dan belakang.

`c
#define MAX_QUEUE 500
static Node* queue[MAX_QUEUE];
`

### Peran
- Mendukung proses pencarian pada BST dengan metode BFS (Breadth-First Search)
- Menyimpan node-node yang akan dikunjungi secara berurutan
- Bersifat sementara — hanya aktif selama proses pencarian berlangsung

### Analisis Proses, Input, dan Output
- **Proses:** Root tree dimasukkan ke queue. Selama queue tidak kosong, node depan dikeluarkan dan diperiksa. Jika cocok, proses selesai. Jika tidak, anak kiri dan kanan dari node tersebut dimasukkan ke queue. Proses diulang sampai queue kosong atau target ditemukan.
- **Input:** Node-node dari BST yang sudah dibangun sebelumnya.
- **Output:** Node tujuan jika ditemukan, atau NULL jika tidak ada dalam tree.

---

## Ringkasan

| Struktur Data | Informasi | Peran | Input | Output |
|---|---|---|---|---|
| **Array of Struct** | Kumpulan grup dengan atribut | Penyimpanan data master | File eksternal | Layar + file |
| **Dynamic Array** | Array ukuran dinamis | Daftar item per grup | Keyboard (user) | Layar + file |
| **BST** | Pohon biner terurut | Representasi sementara | Array of struct | Struktur di memori |
| **Queue** | Antrian FIFO | Pemrosesan BFS | Node BST | Node tujuan |
---

## 5. Abstraksi Data

### Informasi
Abstraksi data adalah pengelompokan data ke dalam tipe-tipe tertentu sesuai dengan fungsi dan kebutuhannya. Program ini menggunakan beberapa jenis tipe data yang masing-masing memiliki peran spesifik dalam menyimpan dan mengelola data.

### Jenis Data yang Digunakan

#### a. Tipe Data Primitif

| Jenis Data | Variabel yang Menggunakan | Kegunaan |
|---|---|---|
| int | id, lag, count, capacity, left, ight, parent, jumlah_koridor, pilihan, koridor_pilih, halte_pilih | Menyimpan bilangan bulat untuk ID, jumlah, indeks, dan penanda |
| char | uf[10], input konfirmasi (y/
) | Menyimpan karakter tunggal |
| char[] | 
ame[50], 
ame[100], sal[100], 	ujuan[100], 
ama_halte[100], uf[200] | Menyimpan string (array of characters) |

#### b. Tipe Data Bentukan (Struct)

**Struct Corridor (pada connection.h):**
`c
typedef struct {
    char name[50];       // Nama koridor
    char** halte;        // Dynamic array of string pointers
    int jumlah_halte;    // Jumlah halte terisi
    int kapasitas;       // Kapasitas teralokasi
} Corridor;
`

**Struct TreeNode (pada BinSTree.h):**
`c
typedef struct TreeNode {
    int id;                     // Indeks/urutan halte
    char name[100];             // Nama halte
    int jalur;                  // Penanda transit (0/1)
    struct TreeNode* leftChild; // Pointer ke anak kiri
    struct TreeNode* rightChild;// Pointer ke anak kanan
    struct TreeNode* parent;    // Pointer ke orangtua
} TreeNode;
`

#### c. Tipe Data Kolektif

| Jenis | Cara Deklarasi | Kegunaan |
|---|---|---|
| **Array of Struct** | Corridor corridors[MAX_CORRIDORS] | Menampung 10 koridor |
| **Dynamic Array** | char** halte (via ealloc) | Menampung nama halte per koridor |
| **Array of Pointers** | static TreeNode* stack_rute[MAX_STACK] | Stack untuk backtracking |
| **Array of Pointers** | static TreeNode* q[MAX_Q] | Queue untuk BFS |

### Data yang Tersimpan (Isi Data)

#### Data dari File (data/halte.txt)

**Grup 1 — Koridor I** (17 item)
`
Pool Damri, RTH Imam Bonjol Padang, GOR H Agus Salim, 
Masjid Raya Sumatera Barat, DPRD Provinsi Sumatera Barat,
Basko Grand Mall, Simpang Labor, Universitas Negeri Padang Air Tawar,
Simpang Tunggul Hitam, Simpang GIA, Asrama Haji Padang,
Perumahan Monang Indah, SMP Negeri 15 Padang, 
Fakultas Kedokteran Universitas Negeri Padang, Kayu Kalek,
Kompleks Mega Permai, Terminal Anak Air
`

**Grup 2 — Koridor II** (15 item)
`
RTH Imam Bonjol Padang, Masjid Nurul Iman, Pasar Gaung,
Toko Abd Gani, Masjid Al Qahaar, Pabrik Karet Sungai Beremas,
Pantai Nirwana, Kantor VTS Bungus, Pisang Salay Citra Minang,
SDN 06 Cindakir, Cecen Cellular, Naldi Cell/Kedai Aura,
Puskesmas Pembantu Bungus, SDN 12 Teluk Kabung, Simpang Sungai Pisang
`

**Grup 3 — Koridor III** (21 item)
`
Terminal Anak Air, Simpang Kalumpang, SBPU Anak Air,
Simpang Koto Tuo, Simpang Aia Pacah Permai, Simpang Maransi,
FKG Baiturrahmah, Universitas Terbuka, SPBU Taruko,
Kantor Camat Kuranji, Simpang Kalumbuk, Polsek/Pukesmas Kuranji,
Utama Furniture, Masjid Jamiatul Huda, RS Semen Padang Hospital,
Poliklinik Fitria, Pool ALS, Simpang Lubeg, Simpang Beringin,
Pampangan, Lantamal
`

#### Data Sementara di Memori

| Jenis Data | Waktu Pembuatan | Waktu Penghapusan |
|---|---|---|
| **BST (TreeNode)** | Saat user memilih Cari Rute | Setelah rute selesai dicetak |
| **Queue (Node*[])** | Saat BFS dimulai | Setelah BFS selesai |
| **Stack (Node*[])** | Tidak pernah dipakai (code mati) | — |

### Peran
- Mengelompokkan data ke dalam tipe yang sesuai dengan kebutuhannya
- Menyediakan struktur yang jelas agar data mudah diakses dan dikelola
- Memisahkan antara data permanen (dari file) dengan data sementara (di memori)

### Analisis Proses, Input, dan Output
- **Proses:** Data mentah dari file diubah menjadi tipe data bentukan Corridor dan TreeNode agar lebih mudah diproses. Tipe data primitif digunakan untuk kendali program (menu, indeks, dll).
- **Input:** Data dari file berupa teks, input user berupa string dan angka. Keduanya disimpan dalam tipe data yang sudah ditentukan.
- **Output:** Data ditampilkan dalam bentuk string (daftar item, rute) dan angka (nomor urut, jumlah), sesuai dengan tipe data yang menyimpannya.