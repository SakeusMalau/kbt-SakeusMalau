#include <stdio.h>
#include <stdlib.h> // Untuk system("cls") atau system("clear")
#include <string.h> // Untuk strcmp

#define MAX_CARS 2 // Jumlah maksimal mobil yang tersedia
#define TOTAL_SEATS_PER_CAR 10 // Sesuai diagram [1] dan [2-10]
#define MAX_NAME_LEN 50
#define MAX_EMAIL_LEN 50
#define MAX_PHONE_LEN 20
#define MAX_DEST_LEN 50

// Waktu keberangkatan yang tersedia
const char *departure_times[] = {
    "05.00", "08.00", "10.00", "12.00",
    "14.00", "16.00", "18.00", "20.00"
};
const int NUM_DEPARTURE_TIMES = sizeof(departure_times) / sizeof(departure_times[0]);

// Struktur data untuk menyimpan informasi penumpang
typedef struct {
    char name[MAX_NAME_LEN];
    char email[MAX_EMAIL_LEN];
    char phone[MAX_PHONE_LEN];
    char destination[MAX_DEST_LEN];
    int seat_number; // Disimpan sebagai indeks array (0-9), mewakili kursi 1-10
    int car_number; // Nomor mobil (1 atau 2)
    const char *departure_time; // Pointer ke string waktu keberangkatan yang dipilih
    int price;
} Passenger;

// Status kursi untuk setiap mobil (O: Open/Tersedia, X: Taken/Terisi)
// Indeks array 0-9 memetakan ke kursi 1-10 sesuai diagram user.
char car_seats[MAX_CARS][TOTAL_SEATS_PER_CAR];
int current_car_index = 0; // Indeks mobil yang sedang aktif (0 atau 1)
int seats_taken_in_current_car = 0; // Jumlah kursi yang sudah terisi di mobil saat ini

// Fungsi untuk menginisialisasi semua kursi menjadi tersedia ('O')
void initialize_seats() {
    for (int i = 0; i < MAX_CARS; i++) {
        for (int j = 0; j < TOTAL_SEATS_PER_CAR; j++) {
            car_seats[i][j] = 'O';
        }
    }
}

// Fungsi untuk menampilkan peta kursi mobil
void display_seats(int car_num) {
    printf("\n--- Peta Kursi Mobil %d ---\n", car_num + 1); // Menampilkan nomor mobil
    // Layout kursi sesuai permintaan pengguna:
    // [Supir] [1O]
    // [2O][3O][4O]
    // [5O][6O][7O]
    // [8O][9O][10O]
    printf(" [Supir] [%d%c]\n", 1, car_seats[car_num][0]); // Kursi 1 (indeks 0)
    printf(" [%d%c][%d%c][%d%c]\n", 2, car_seats[car_num][1], 3, car_seats[car_num][2], 4, car_seats[car_num][3]); // Kursi 2,3,4 (indeks 1,2,3)
    printf(" [%d%c][%d%c][%d%c]\n", 5, car_seats[car_num][4], 6, car_seats[car_num][5], 7, car_seats[car_num][6]); // Kursi 5,6,7 (indeks 4,5,6)
    printf(" [%d%c][%d%c][%d%c]\n", 8, car_seats[car_num][7], 9, car_seats[car_num][8], 10, car_seats[car_num][9]); // Kursi 8,9,10 (indeks 7,8,9)
    printf("---------------------------\n");
    printf("Keterangan: O=Tersedia, X=Terisi\n");
}

// Fungsi untuk mencetak tiket penumpang
void print_ticket(const Passenger *p) {
    printf("\n");
    printf("____________________________\n");
    printf("|                          |\n");
    printf("|      TIKET ANGKUTAN      |\n");
    printf("|__________________________|");
    printf("\n| Nama     : %-15s|\n", p->name);
    printf("| Tujuan   : %-15s|\n", p->destination);
    printf("| Mobil Ke : %-15d|\n", p->car_number);
    printf("| Jam      : %-15s|\n", p->departure_time);
    printf("| Kursi    : %-15d|\n", p->seat_number + 1); // +1 karena disimpan sebagai indeks 0-indexed
    printf("| Harga    : Rp %-12d|\n", p->price);
    printf("|__________________________|\n");
    printf("\n");
}

int main() {
    initialize_seats(); // Inisialisasi status semua kursi
    Passenger current_passenger; // Variabel untuk menyimpan data penumpang yang sedang mendaftar

    printf("Selamat datang di Layanan Pemesanan Tiket Angkutan!\n");
    printf("Sebagai mahasiswa semester 2, saya akan bantu buat programnya.\n\n");

    // Loop utama untuk memungkinkan pemesanan tiket secara berkelanjutan
    while (1) {
        char less_than_3_days_str[10];
        printf("Apakah tanggal keberangkatan kurang dari 3 hari? (y/n): ");
        scanf("%s", less_than_3_days_str);

        if (strcmp(less_than_3_days_str, "y") == 0 || strcmp(less_than_3_days_str, "Y") == 0) {
            printf("Pendaftaran online tidak dapat dilakukan. Silakan datang ke loket.\n");
            printf("-------------------------------------------------------------------\n");
            
            // Tanya apakah ada pemesanan lain untuk penumpang berikutnya atau keluar
            printf("\nApakah ada pemesanan lain untuk penumpang berikutnya? (y/n): ");
            char repeat_booking[10];
            scanf("%s", repeat_booking);
            while (getchar() != '\n'); // Membersihkan buffer input
            if (strcmp(repeat_booking, "n") == 0 || strcmp(repeat_booking, "N") == 0) {
                break; // Keluar dari loop utama jika tidak ada pemesanan lain
            }
            continue; // Lanjut ke iterasi berikutnya untuk pemesanan baru
        }

        while (getchar() != '\n'); // Membersihkan buffer input setelah scanf

        printf("\n--- Data Diri Penumpang ---\n");
        printf("Nama: ");
        fgets(current_passenger.name, MAX_NAME_LEN, stdin);
        current_passenger.name[strcspn(current_passenger.name, "\n")] = 0; // Menghapus karakter newline

        printf("Email: ");
        fgets(current_passenger.email, MAX_EMAIL_LEN, stdin);
        current_passenger.email[strcspn(current_passenger.email, "\n")] = 0; // Menghapus karakter newline

        printf("Nomor HP: ");
        fgets(current_passenger.phone, MAX_PHONE_LEN, stdin);
        current_passenger.phone[strcspn(current_passenger.phone, "\n")] = 0; // Menghapus karakter newline

        printf("Tujuan (misal: Balige, Parapat): ");
        fgets(current_passenger.destination, MAX_DEST_LEN, stdin);
        current_passenger.destination[strcspn(current_passenger.destination, "\n")] = 0; // Menghapus karakter newline

        int area_choice;
        printf("Akan pergi kemana? (1) Diluar Toba (2) Diwilayah Toba sekitar Toba: ");
        scanf("%d", &area_choice);
        while (getchar() != '\n'); // Membersihkan buffer input

        int class_choice;
        printf("Ingin kelas apa? (1) Exclusive (2) Economy: ");
        scanf("%d", &class_choice);
        while (getchar() != '\n'); // Membersihkan buffer input

        // Menentukan harga tiket berdasarkan pilihan area dan kelas
        if (area_choice == 1) { // Diluar Toba
            if (class_choice == 1) current_passenger.price = 140; // Exclusive
            else current_passenger.price = 85; // Economy
        } else { // Diwilayah Toba sekitar Toba
            if (class_choice == 1) current_passenger.price = 80; // Exclusive
            else current_passenger.price = 50; // Economy
        }

        printf("\n--- Pilih Jam Keberangkatan ---\n");
        for (int j = 0; j < NUM_DEPARTURE_TIMES; j++) {
            printf("%d. %s\n", j + 1, departure_times[j]);
        }
        int time_choice;
        printf("Pilih jam keberangkatan (nomor): ");
        scanf("%d", &time_choice);
        while (getchar() != '\n'); // Membersihkan buffer input

        // Mengatur jam keberangkatan, default ke jam pertama jika input tidak valid
        if (time_choice >= 1 && time_choice <= NUM_DEPARTURE_TIMES) {
            current_passenger.departure_time = departure_times[time_choice - 1];
        } else {
            printf("Pilihan jam tidak valid, menggunakan jam pertama (05.00).\n");
            current_passenger.departure_time = departure_times[0];
        }
        
        int chosen_seat;
        int seat_found = 0;
        // Loop untuk pemilihan kursi hingga berhasil menemukan kursi yang tersedia
        while (!seat_found) {
            // Cek apakah mobil saat ini sudah penuh
            if (seats_taken_in_current_car >= TOTAL_SEATS_PER_CAR) {
                current_car_index++; // Pindah ke mobil berikutnya
                seats_taken_in_current_car = 0; // Reset hitungan kursi terisi untuk mobil baru
                if (current_car_index >= MAX_CARS) {
                    printf("Maaf, semua mobil penuh. Tidak bisa memesan lagi.\n");
                    return 1; // Keluar dari program jika semua mobil sudah penuh
                }
                printf("\nMobil sebelumnya sudah penuh. Melanjutkan ke Mobil %d.\n", current_car_index + 1);
            }

            display_seats(current_car_index); // Tampilkan peta kursi mobil saat ini
            printf("Pilih nomor kursi (1-%d) untuk Mobil %d: ", TOTAL_SEATS_PER_CAR, current_car_index + 1);
            scanf("%d", &chosen_seat);
            while (getchar() != '\n'); // Membersihkan buffer input

            // Validasi dan alokasi kursi
            if (chosen_seat >= 1 && chosen_seat <= TOTAL_SEATS_PER_CAR) {
                if (car_seats[current_car_index][chosen_seat - 1] == 'O') {
                    car_seats[current_car_index][chosen_seat - 1] = 'X'; // Tandai kursi sebagai terisi
                    current_passenger.seat_number = chosen_seat - 1; // Simpan indeks array (0-indexed)
                    current_passenger.car_number = current_car_index + 1;
                    seats_taken_in_current_car++;
                    seat_found = 1; // Kursi berhasil ditemukan dan dialokasikan
                    printf("Kursi %d di Mobil %d berhasil dipilih.\n", chosen_seat, current_car_index + 1);
                } else {
                    printf("Kursi %d sudah terisi. Mohon pilih kursi lain.\n", chosen_seat);
                }
            } else {
                printf("Nomor kursi tidak valid. Mohon masukkan angka antara 1 dan %d.\n", TOTAL_SEATS_PER_CAR);
            }
        }

        printf("\n--- Tiket Anda ---\n");
        print_ticket(&current_passenger); // Cetak tiket untuk penumpang yang baru didaftarkan
        
        printf("\nPemesanan selesai untuk penumpang ini. Apakah ada pemesanan lain untuk penumpang berikutnya? (y/n): ");
        char repeat_booking[10];
        scanf("%s", repeat_booking);
        while (getchar() != '\n'); // Membersihkan buffer input
        if (strcmp(repeat_booking, "n") == 0 || strcmp(repeat_booking, "N") == 0) {
            break; // Keluar dari loop utama jika tidak ada pemesanan lain
        }
        // Jika 'y', loop akan rlanjut ke iterasi berikutnya untuk pendaftaran penumpang baru
    }

    printf("Terima kasih telah menggunakan layanan kami!\n");
    return 0;
}