#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Driver {
    string nama;
    string nomorHP;
    string alamat;
    string nomorKTP;
    string statusVerifikasi;
};

// PERBAIKAN 1: Nama variabel diubah menjadi listPendaftar agar tidak bentrok dengan fungsi
vector<Driver> listPendaftar; 

void tampilkanBanner() {
    cout << "\n╔════════════════════════════════════════════════════════╗\n";
    cout << "║      SELAMAT DATANG DI OJEK ONLINE TERPERCAYA ANDA     ║\n";
    cout << "║  \"Bekerja dengan Hati, Melayani dengan Sepenuh Jiwa\"   ║\n";
    cout << "╚════════════════════════════════════════════════════════╝\n";
}

void tampilkanPromosi() {
    cout << "═══════════════════════════════════════════════════════\n";
    cout << "               ✨ PROMOSI SPESIAL UNTUK ANDA ✨\n";
    cout << "═══════════════════════════════════════════════════════\n";
    cout << "✓ Bonus pendaftaran: Rp 500.000\n";
    cout << "✓ Komisi kompetitif hingga 85%\n";
    cout << "✓ Asuransi kesehatan gratis untuk 1 tahun\n";
    cout << "═══════════════════════════════════════════════════════\n";
}

void daftarDriver() {
    cout << "\n📝 FORM PENDAFTARAN DRIVER OJEK ONLINE\n";
    cout << "─────────────────────────────────────────\n";
    
    Driver driver;
    
    // Perbaikan Input: Menggunakan cin.ignore() untuk membersihkan 'Enter' dari menu sebelumnya
    cin.ignore(); 
    cout << "Nama lengkap: ";
    getline(cin, driver.nama);
    
    cout << "Nomor HP (WhatsApp): ";
    getline(cin, driver.nomorHP);
    
    cout << "Alamat lengkap: ";
    getline(cin, driver.alamat);
    
    cout << "Nomor KTP: ";
    getline(cin, driver.nomorKTP);
    
    driver.statusVerifikasi = "Menunggu Verifikasi";
    listPendaftar.push_back(driver); // Menggunakan nama variabel yang baru
    
    cout << "\n✅ Pendaftaran berhasil!\n";
    cout << "Status: " << driver.statusVerifikasi << "\n";
}

void lihatPendaftar() {
    if (listPendaftar.empty()) {
        cout << "\n⚠️  Belum ada pendaftar.\n";
        return;
    }
    
    cout << "\n📋 DAFTAR PENDAFTAR\n";
    cout << "─────────────────────────────────────────\n";
    for (size_t i = 0; i < listPendaftar.size(); i++) {
        cout << "\nNo. " << (i + 1) << "\n";
        cout << "Nama   : " << listPendaftar[i].nama << "\n";
        cout << "HP     : " << listPendaftar[i].nomorHP << "\n";
        cout << "Alamat : " << listPendaftar[i].alamat << "\n";
        cout << "KTP    : " << listPendaftar[i].nomorKTP << "\n";
        cout << "Status : " << listPendaftar[i].statusVerifikasi << "\n";
    }
}

void tampilkanMenu() {
    cout << "\n╔════════════════════════════════════════╗\n";
    cout << "║         MENU UTAMA PENDAFTARAN         ║\n";
    cout << "╠════════════════════════════════════════╣\n";
    cout << "║ 1. Lihat Promosi                       ║\n";
    cout << "║ 2. Daftar Sebagai Driver               ║\n";
    cout << "║ 3. Lihat Daftar Pendaftar              ║\n";
    cout << "║ 4. Keluar                              ║\n";
    cout << "╚════════════════════════════════════════╝\n";
    cout << "Pilihan Anda: ";
}

int main() {
    int pilihan;
    
    tampilkanBanner();
    tampilkanPromosi();
    
    while (true) {
        tampilkanMenu();
        if (!(cin >> pilihan)) { // Proteksi jika user input huruf
            cout << "Input harus angka!\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        
        switch (pilihan) {
            case 1:
                tampilkanPromosi();
                break;
            case 2:
                daftarDriver();
                break;
            case 3:
                lihatPendaftar();
                break;
            case 4:
                cout << "\n🙏 Terima kasih! Semoga sukses bersama kami.\n";
                return 0;
            default:
                cout << "❌ Pilihan tidak valid!\n";
        }
    }
    return 0;
}