#include <iostream> // library untuk input output
#include "/opt/homebrew/Cellar/open-mpi/4.1.4_2/include/mpi.h" // library untuk MPI (Message Passing Interface)
#include <cmath> // library untuk matematika (math)
#include <cstdlib> // library untuk fungsi umum (stdlib) 
#include <ctime> // library untuk waktu (time)
#include <iomanip> // library untuk manipulasi tampilan (iomanip)

using namespace std; // menggunakan namespace std

int prime_number(int n, int id, int p); // mendeklarasikan fungsi prime_number di awal
void timestamp(); // mendeklarasikan fungsi timestamp di awal

int main(int argc, char *argv[])
{
    int id; // ID dari proses
    int ierr; // nilai error
    int n; // batas bilangan prima
    int n_factor; // faktor untuk menentukan batas bilangan prima
    int n_hi; // batas atas bilangan prima
    int n_lo; // batas bawah bilangan prima
    int p; // banyak proses
    int primes; // banyak bilangan prima
    int primes_part; // banyak bilangan prima untuk setiap proses
    double wtime; // waktu eksekusi

    n_lo = 1; // batas bawah bilangan prima (2^0) dengan nilai awal 1
    n_hi = 262144; // batas atas bilangan prima (2^18) dengan nilai awal 262144
    n_factor = 2; // faktor untuk menentukan batas bilangan prima (n = n * n_factor) dengan nilai awal 2

    ierr = MPI_Init(&argc, &argv); // inisialisasi MPI dan mengembalikan nilai error

    if (ierr != 0){ // jika nilai error tidak sama dengan 0
        // tampilkan pesan error
        cout << "\n";
        cout << "MPI - Eror Fatal!\n";
        cout << "  MPI_Init ierr Mengembalikan Nilai nonzeros.\n";
        exit(1); 
        // akhir dari tampilan pesan error
    }

    ierr = MPI_Comm_size(MPI_COMM_WORLD, &p); // mendapatkan nilai banyak proses kemudian disimpan ke variabel ierr
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &id); // mendapatkan rank dari proses kemudian disimpan ke variabel ierr

    if (id == 0){ // jika variabel id sama dengan 0
        // tampilkan pesan
        timestamp();
        cout << "\n";
        cout << "Parallel Computing\n";
        cout << "  Contoh Program MPI Untuk Menghitung Total Bilang Prima.\n";
        cout << "  Berdasrkan Banyak Proses (p) " << p << "\n";
        cout << "\n";
        cout << "         N        Total          Waktu\n";
        cout << "\n";
        // akhir dari tampilan pesan
    }

    n = n_lo; // nilai awal n adalah n_lo

    while (n <= n_hi){ // selama n lebih kecil atau sama dengan n_hi maka lakukan perulangan
        if (id == 0){ // jika variabel id sama dengan 0
            wtime = MPI_Wtime(); // mendapatkan waktu eksekusi program
        }

        ierr = MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD); // mengirimkan nilai n ke semua proses kemudian disimpan ke variabel ierr

        primes_part = prime_number(n, id, p); // menghitung banyak bilangan prima dari fungsi prime_number kemudian disimpan ke variabel primes_part

        ierr = MPI_Reduce(&primes_part, &primes, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD); // menghitung total bilangan prima dari semua proses kemudian disimpan ke variabel ierr

        if (id == 0){ // jika variabel id sama dengan 0
            wtime = MPI_Wtime() - wtime; // mendapatkan waktu eksekusi program


            cout
                << "  " << setw(8) << n // menampilkan nilai n
                << "  " << setw(8) << primes // menampilkan nilai primes
                << "  " << setw(14) << wtime << "\n"; // menampilkan nilai wtime
        }
        n = n * n_factor; // nilai n diubah menjadi n * n_factor (n = n * n_factor)
    }
    MPI_Finalize(); // mengakhiri eksekusi program MPI
    if (id == 0){ // jika variabel id sama dengan 0
        // tampilkan pesan
        cout << "\n";
        cout << "  Eksekusi Program Berjalan Normal.\n";
        cout << "\n";
        timestamp();
        // akhir dari tampilan pesan
    }

    return 0;
}

int prime_number(int n, int id, int p){
    int i; // variabel i bertipe data integer
    int j; // variabel j bertipe data integer
    int prime; // prima digunakan untuk menampung nilai bilangan prima
    int total; // total digunakan untuk menampung nilai total

    total = 0; // nilai total diubah menjadi 0

    for (i = 2 + id; i <= n; i = i + p){ // perulangan untuk mengecek bilangan prima dari 2 + id sampai ke n dengan inrcement i = i + p
        prime = 1; // nilai prima diubah menjadi 1
        for (j = 2; j < i; j++){ // perulangan untuk mengecek bilangan prima dari 2 sampai ke i dengan increment j = j + 1
            if ((i % j) == 0){ // jika i habis dibagi j
                prime = 0; // nilai prima diubah menjadi 0
                break; // keluar dari perulangan
            }
        }
        total = total + prime; // nilai total diubah menjadi total + prima
    }
    return total; // mengembalikan nilai total
}

void timestamp(){ // fungsi timestamp
#define TIME_SIZE 40 // mendefinisikan TIME_SIZE dengan nilai 40

    static char time_buffer[TIME_SIZE]; // mendefinisikan time_buffer dengan tipe data char dan ukuran TIME_SIZE
    const struct tm *tm; // mendefinisikan tm dengan tipe data struct tm
    time_t now; // mendefinisikan now dengan tipe data time_t

    now = time(NULL); // mendapatkan waktu sekarang
    tm = localtime(&now); // mendapatkan waktu lokal

    strftime(time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm); // mengubah waktu menjadi format yang diinginkan

    cout << time_buffer << "\n"; // menampilkan waktu

    return; // mengakhiri fungsi timestamp
#undef TIME_SIZE // menghapus definisi TIME_SIZE
}