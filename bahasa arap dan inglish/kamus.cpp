
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>

// Untuk Windows: aktifkan mode Unicode
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif

using namespace std;

// ==================== INISIALISASI UTF-8 ====================
void initUTF8() {
    #ifdef _WIN32
    // Aktifkan mode Unicode console
    _setmode(_fileno(stdout), _O_U8TEXT);
    _setmode(_fileno(stdin), _O_U8TEXT);
    #endif
    
    // Set locale untuk karakter internasional
    setlocale(LC_ALL, "en_US.UTF-8");
    setlocale(LC_CTYPE, "en_US.UTF-8");
}

// ==================== STRUCTURES ===============  =====

struct Entry {
    string arabic;
    string english;
    string indonesian;
    string category;
};

struct Dictionary {
    vector<Entry> entries;
    string sourceLang;
    string targetLang;
};

// ==================== KONSTANTA BATAS PANJANG ====================
const int MIN_WORD_LENGTH = 1;     // Minimum 1 karakter
const int MAX_ARABIC_LENGTH = 50;  // Maksimum 50 karakter untuk Arab
const int MAX_ENGLISH_LENGTH = 100; // Maksimum 100 karakter untuk English
const int MAX_INDONESIAN_LENGTH = 100; // Maksimum 100 karakter untuk Indonesia

// ==================== FUNGSI VALIDASI INPUT ====================

bool validateInputLength(const string& input, int minLen, int maxLen, const string& fieldName) {
    if (input.length() < minLen) {
        cout << "\n[ERROR] " << fieldName << " terlalu pendek!"
             << "\n        Minimum: " << minLen << " karakter"
             << "\n        Yang Anda masukkan: " << input.length() << " karakter";
        return false;
    }
    if (input.length() > maxLen) {
        cout << "\n[ERROR] " << fieldName << " terlalu panjang!"
             << "\n        Maksimum: " << maxLen << " karakter"
             << "\n        Yang Anda masukkan: " << input.length() << " karakter";
        return false;
    }
    return true;
}

void showNotFoundMessage(const string& query, const string& lang) {
    cout << "\n+-------------------------------------------------------+\n";
    cout << "|             HASIL PENCARIAN TIDAK DITEMUKAN             |\n";
    cout << "+---------------------------------------------------------+\n";
    cout << "|  Kata yang Anda cari: \"" << query << "\"" << setw(38 - query.length()) << "║\n";
    cout << "|  Bahasa: " << lang << setw(40) << right << "|\n";
    cout << "+-----------------------------------------------------------+\n";
    cout << "|     MAAF, KATA INI TIDAK TERSEDIA DALAM KAMUS            |\n";
    cout << "|                                                          |\n";
    cout << "|  Saran:                                                  |\n";
    cout << "|    • Periksa kembali penulisan kata Anda                  |\n";
    cout << "|    • Pastikan tidak ada kesalahan ketik                 |\n";
    cout << "|    • Coba gunakan kata kunci yang lebih umum            |\n";
    cout << "|    • Atau tambahkan kata baru melalui menu 5            |\n";
    cout << "+-----------------------------------------------------------+\n";
}

void showInputRules() {
    cout << "\n  ATURAN INPUT:"
         << "\n   • Bahasa Arab: 1-" << MAX_ARABIC_LENGTH << " karakter"
         << "\n   • English:     1-" << MAX_ENGLISH_LENGTH << " karakter"
         << "\n   • Indonesia:   1-" << MAX_INDONESIAN_LENGTH << " karakter";
}

// ==================== KATEGORI ====================
const string categories[] = {
    "Umum", "Kata Kerja", "Kata Benda", "Kata Sifat", 
    "Angka", "Waktu", "Keluarga", "Makanan", "Perjalanan",
    "Pendidikan", "Pekerjaan", "Badan Manusia", "Warna",
    "Benda", "Tempat", "Hewan", "Tanaman"
};

// ==================== DATA KAMUS ARABIC - ENGLISH (Sampel 100) ====================
vector<Entry> initArabicEnglish() {
    vector<Entry> dict;
    
    // Umum / General
    dict.push_back({"مرحبا", "Hello", "Halo", "Umum"});
    dict.push_back({"وداعا", "Goodbye", "Selamat tinggal", "Umum"});
    dict.push_back({"نعم", "Yes", "Ya", "Umum"});
    dict.push_back({"لا", "No", "Tidak", "Umum"});
    dict.push_back({"من فضلك", "Please", "Tolong", "Umum"});
    dict.push_back({"شكرا", "Thank you", "Terima kasih", "Umum"});
    dict.push_back({"عفوا", "Excuse me", "Permisi", "Umum"});
    dict.push_back({"آسف", "Sorry", "Maaf", "Umum"});
    dict.push_back({"مساء الخير", "Good evening", "Selamat malam", "Umum"});
    dict.push_back({"صباح الخير", "Good morning", "Selamat pagi", "Umum"});
    
    // Kata Kerja / Verbs
    dict.push_back({"يأكل", "To eat", "Makan", "Kata Kerja"});
    dict.push_back({"يشرب", "To drink", "Minum", "Kata Kerja"});
    dict.push_back({"ينام", "To sleep", "Tidur", "Kata Kerja"});
    dict.push_back({"يستيقظ", "To wake up", "Bangun", "Kata Kerja"});
    dict.push_back({"يقرأ", "To read", "Membaca", "Kata Kerja"});
    dict.push_back({"يكتب", "To write", "Menulis", "Kata Kerja"});
    dict.push_back({"يتكلم", "To speak", "Berbicara", "Kata Kerja"});
    dict.push_back({"يسمع", "To hear", "Mendengar", "Kata Kerja"});
    dict.push_back({"يرى", "To see", "Melihat", "Kata Kerja"});
    dict.push_back({"يذهب", "To go", "Pergi", "Kata Kerja"});
    dict.push_back({"يأتي", "To come", "Datang", "Kata Kerja"});
    dict.push_back({"يعمل", "To work", "Bekerja", "Kata Kerja"});
    dict.push_back({"يلعب", "To play", "Bermain", "Kata Kerja"});
    dict.push_back({"يتعلم", "To learn", "Belajar", "Kata Kerja"});
    dict.push_back({"يفهم", "To understand", "Memahami", "Kata Kerja"});
    
    // Kata Benda / Nouns
    dict.push_back({"كتاب", "Book", "Buku", "Kata Benda"});
    dict.push_back({"قلم", "Pen", "Pena", "Kata Benda"});
    dict.push_back({"ورقة", "Paper", "Kertas", "Kata Benda"});
    dict.push_back({"طاولة", "Table", "Meja", "Kata Benda"});
    dict.push_back({"كرسي", "Chair", "Kursi", "Kata Benda"});
    dict.push_back({"بيت", "House", "Rumah", "Kata Benda"});
    dict.push_back({"سيارة", "Car", "Mobil", "Kata Benda"});
    dict.push_back({"كمبيوتر", "Computer", "Komputer", "Kata Benda"});
    dict.push_back({"مدرسة", "School", "Sekolah", "Kata Benda"});
    dict.push_back({"جامعة", "University", "Universitas", "Kata Benda"});
    dict.push_back({"مستشفى", "Hospital", "Rumah Sakit", "Kata Benda"});
    dict.push_back({"مسجد", "Mosque", "Masjid", "Kata Benda"});
    dict.push_back({"كنيسة", "Church", "Gereja", "Kata Benda"});
    dict.push_back({"محل", "Store", "Toko", "Kata Benda"});
    dict.push_back({"مدينة", "City", "Kota", "Kata Benda"});
    
    // Kata Sifat / Adjectives
    dict.push_back({"كبير", "Big", "Besar", "Kata Sifat"});
    dict.push_back({"صغير", "Small", "Kecil", "Kata Sifat"});
    dict.push_back({"جديد", "New", "Baru", "Kata Sifat"});
    dict.push_back({"قديم", "Old", "Lama", "Kata Sifat"});
    dict.push_back({"جميل", "Beautiful", "Cantik/Indah", "Kata Sifat"});
    dict.push_back({"قبيح", "Ugly", "Jelek", "Kata Sifat"});
    dict.push_back({"سريع", "Fast", "Cepat", "Kata Sifat"});
    dict.push_back({"بطيء", "Slow", "Lambat", "Kata Sifat"});
    dict.push_back({"حار", "Hot", "Panas", "Kata Sifat"});
    dict.push_back({"بارد", "Cold", "Dingin", "Kata Sifat"});
    dict.push_back({"جيد", "Good", "Baik", "Kata Sifat"});
    dict.push_back({"سيء", "Bad", "Buruk", "Kata Sifat"});
    dict.push_back({"سهل", "Easy", "Mudah", "Kata Sifat"});
    dict.push_back({"صعب", "Difficult", "Sulit", "Kata Sifat"});
    dict.push_back({"غني", "Rich", "Kaya", "Kata Sifat"});
    
    // Angka / Numbers
    dict.push_back({"صفر", "Zero", "Nol", "Angka"});
    dict.push_back({"واحد", "One", "Satu", "Angka"});
    dict.push_back({"اثنان", "Two", "Dua", "Angka"});
    dict.push_back({"ثلاثة", "Three", "Tiga", "Angka"});
    dict.push_back({"أربعة", "Four", "Empat", "Angka"});
    dict.push_back({"خمسة", "Five", "Lima", "Angka"});
    dict.push_back({"ستة", "Six", "Enam", "Angka"});
    dict.push_back({"سبعة", "Seven", "Tujuh", "Angka"});
    dict.push_back({"ثمانية", "Eight", "Delapan", "Angka"});
    dict.push_back({"تسعة", "Nine", "Sembilan", "Angka"});
    dict.push_back({"عشرة", "Ten", "Sepuluh", "Angka"});
    dict.push_back({"عشرون", "Twenty", "Dua puluh", "Angka"});
    dict.push_back({"مئة", "Hundred", "Seratus", "Angka"});
    dict.push_back({"ألف", "Thousand", "Seribu", "Angka"});
    dict.push_back({"مليون", "Million", "Juta", "Angka"});
    
    // Waktu / Time
    dict.push_back({"اليوم", "Today", "Hari ini", "Waktu"});
    dict.push_back({"غدا", "Tomorrow", "Besok", "Waktu"});
    dict.push_back({"أمس", "Yesterday", "Kemarin", "Waktu"});
    dict.push_back({"الآن", "Now", "Sekarang", "Waktu"});
    dict.push_back({"المساء", "Evening", "Sore/Malam", "Waktu"});
    dict.push_back({"الصباح", "Morning", "Pagi", "Waktu"});
    dict.push_back({"ليل", "Night", "Malam", "Waktu"});
    dict.push_back({"ظهر", "Noon", "Siang", "Waktu"});
    dict.push_back({"ساعة", "Hour", "Jam", "Waktu"});
    dict.push_back({"دقيقة", "Minute", "Menit", "Waktu"});
    dict.push_back({"ثانية", "Second", "Detik", "Waktu"});
    dict.push_back({"أسبوع", "Week", "Minggu", "Waktu"});
    dict.push_back({"شهر", "Month", "Bulan", "Waktu"});
    dict.push_back({"سنة", "Year", "Tahun", "Waktu"});
    dict.push_back({"قرن", "Century", "Abad", "Waktu"});
    
    // Keluarga / Family
    dict.push_back({"أب", "Father", "Ayah", "Keluarga"});
    dict.push_back({"أم", "Mother", "Ibu", "Keluarga"});
    dict.push_back({"ابن", "Son", "Anak laki-laki", "Keluarga"});
    dict.push_back({"ابنة", "Daughter", "Anak perempuan", "Keluarga"});
    dict.push_back({"أخ", "Brother", "Saudara laki-laki", "Keluarga"});
    dict.push_back({"أخت", "Sister", "Saudara perempuan", "Keluarga"});
    dict.push_back({"جد", "Grandfather", "Kakek", "Keluarga"});
    dict.push_back({"جدة", "Grandmother", "Nenek", "Keluarga"});
    dict.push_back({"عم", "Uncle", "Paman", "Keluarga"});
    dict.push_back({"خالة", "Aunt", "Bibi", "Keluarga"});
    dict.push_back({"زوج", "Husband", "Suami", "Keluarga"});
    dict.push_back({"زوجة", "Wife", "Istri", "Keluarga"});
    dict.push_back({"عائلة", "Family", "Keluarga", "Keluarga"});
    dict.push_back({"طفل", "Child", "Anak", "Keluarga"});
    dict.push_back({"رجل", "Man", "Pria", "Keluarga"});
    
    // Makanan / Food
    dict.push_back({"خبز", "Bread", "Roti", "Makanan"});
    dict.push_back({"لحم", "Meat", "Daging", "Makanan"});
    dict.push_back({"سمك", "Fish", "Ikan", "Makanan"});
    dict.push_back({"دجاج", "Chicken", "Ayam", "Makanan"});
    dict.push_back({"أرز", "Rice", "Nasi", "Makanan"});
    dict.push_back({"خضار", "Vegetables", "Sayuran", "Makanan"});
    dict.push_back({"فاكهة", "Fruit", "Buah", "Makanan"});
    dict.push_back({"ماء", "Water", "Air", "Makanan"});
    dict.push_back({"قهوة", "Coffee", "Kopi", "Makanan"});
    dict.push_back({"شاي", "Tea", "Teh", "Makanan"});
    dict.push_back({"حليب", "Milk", "Susu", "Makanan"});
    dict.push_back({"زيت", "Oil", "Minyak", "Makanan"});
    dict.push_back({"ملح", "Salt", "Garam", "Makanan"});
    dict.push_back({"سكر", "Sugar", "Gula", "Makanan"});
    dict.push_back({"بيض", "Egg", "Telur", "Makanan"});
    
    // Perjalanan / Travel
    dict.push_back({"مطار", "Airport", "Bandara", "Perjalanan"});
    dict.push_back({"قطار", "Train", "Kereta", "Perjalanan"});
    dict.push_back({"باص", "Bus", "Bus", "Perjalanan"});
    dict.push_back({"سفينة", "Ship", "Kapal", "Perjalanan"});
    dict.push_back({"طائرة", "Airplane", "Pesawat", "Perjalanan"});
    dict.push_back({"محطة", "Station", "Stasiun", "Perjalanan"});
    dict.push_back({"تذكرة", "Ticket", "Tiket", "Perjalanan"});
    dict.push_back({"جواز سفر", "Passport", "Paspor", "Perjalanan"});
    dict.push_back({"فندق", "Hotel", "Hotel", "Perjalanan"});
    dict.push_back({"غرفة", "Room", "Kamar", "Perjalanan"});
    dict.push_back({"سفر", "Travel", "Perjalanan", "Perjalanan"});
    dict.push_back({"إقامة", "Stay", "Menginap", "Perjalanan"});
    dict.push_back({"خروج", "Departure", "Keberangkatan", "Perjalanan"});
    dict.push_back({"وصول", "Arrival", "Kedatangan", "Perjalanan"});
    dict.push_back({"حجز", "Reservation", "Reservasi", "Perjalanan"});
    
    // Pendidikan / Education
    dict.push_back({"طالب", "Student", "Siswa/Mahasiswa", "Pendidikan"});
    dict.push_back({"معلم", "Teacher", "Guru", "Pendidikan"});
    dict.push_back({"درس", "Lesson", "Pelajaran", "Pendidikan"});
    dict.push_back({"امتحان", "Exam", "Ujian", "Pendidikan"});
    dict.push_back({"درجة", "Grade", "Nilai", "Pendidikan"});
    dict.push_back({"مكتبة", "Library", "Perpustakaan", "Pendidikan"});
    dict.push_back({"كتاب", "Book", "Buku", "Pendidikan"});
    dict.push_back({"قاعة", "Classroom", "Kelas", "Pendidikan"});
    dict.push_back({"جامعة", "University", "Universitas", "Pendidikan"});
    dict.push_back({"كلية", "College", "Fakultas", "Pendidikan"});
    dict.push_back({"بحث", "Research", "Penelitian", "Pendidikan"});
    dict.push_back({"علم", "Science", "Ilmu", "Pendidikan"});
    dict.push_back({"تاريخ", "History", "Sejarah", "Pendidikan"});
    dict.push_back({"رياضيات", "Mathematics", "Matematika", "Pendidikan"});
    dict.push_back({"فيزياء", "Physics", "Fisika", "Pendidikan"});
    
    // Pekerjaan / Jobs
    dict.push_back({"طبيب", "Doctor", "Dokter", "Pekerjaan"});
    dict.push_back({"مهندس", "Engineer", "Insinyur", "Pekerjaan"});
    dict.push_back({"محامي", "Lawyer", "Pengacara", "Pekerjaan"});
    dict.push_back({"معلم", "Teacher", "Guru", "Pekerjaan"});
    dict.push_back({"عامل", "Worker", "Pekerja", "Pekerjaan"});
    dict.push_back({"تاجر", "Merchant", "Pedagang", "Pekerjaan"});
    dict.push_back({"فلاح", "Farmer", "Petani", "Pekerjaan"});
    dict.push_back({"صياد", "Fisherman", "Nelayan", "Pekerjaan"});
    dict.push_back({"طباخ", "Cook", "Koki", "Pekerjaan"});
    dict.push_back({"سائق", "Driver", "Sopir", "Pekerjaan"});
    dict.push_back({"شرطي", "Police", "Polisi", "Pekerjaan"});
    dict.push_back({"جندي", "Soldier", "Tentara", "Pekerjaan"});
    dict.push_back({"كاتب", "Writer", "Penulis", "Pekerjaan"});
    dict.push_back({"فنان", "Artist", "Seniman", "Pekerjaan"});
    dict.push_back({"عالم", "Scientist", "Ilmuwan", "Pekerjaan"});
    
    // Badan Manusia / Human Body
    dict.push_back({"رأس", "Head", "Kepala", "Badan Manusia"});
    dict.push_back({"عين", "Eye", "Mata", "Badan Manusia"});
    dict.push_back({"أذن", "Ear", "Telinga", "Badan Manusia"});
    dict.push_back({"أنف", "Nose", "Hidung", "Badan Manusia"});
    dict.push_back({"فم", "Mouth", "Mulut", "Badan Manusia"});
    dict.push_back({"يد", "Hand", "Tangan", "Badan Manusia"});
    dict.push_back({"رجل", "Leg", "Kaki", "Badan Manusia"});
    dict.push_back({"قلب", "Heart", "Jantung", "Badan Manusia"});
    dict.push_back({"معدة", "Stomach", "Perut", "Badan Manusia"});
    dict.push_back({"ظهر", "Back", "Punggung", "Badan Manusia"});
    dict.push_back({"جلد", "Skin", "Kulit", "Badan Manusia"});
    dict.push_back({"عظم", "Bone", "Tulang", "Badan Manusia"});
    dict.push_back({"عضلة", "Muscle", "Otot", "Badan Manusia"});
    dict.push_back({"دم", "Blood", "Darah", "Badan Manusia"});
    dict.push_back({"شعر", "Hair", "Rambut", "Badan Manusia"});
    
    // Warna / Colors
    dict.push_back({"أحمر", "Red", "Merah", "Warna"});
    dict.push_back({"أزرق", "Blue", "Biru", "Warna"});
    dict.push_back({"أخضر", "Green", "Hijau", "Warna"});
    dict.push_back({"أصفر", "Yellow", "Kuning", "Warna"});
    dict.push_back({"برتقالي", "Orange", "Oranye", "Warna"});
    dict.push_back({"بنفسجي", "Purple", "Ungu", "Warna"});
    dict.push_back({"بني", "Brown", "Coklat", "Warna"});
    dict.push_back({"أسود", "Black", "Hitam", "Warna"});
    dict.push_back({"أبيض", "White", "Putih", "Warna"});
    dict.push_back({"رمادي", "Gray", "Abu-abu", "Warna"});
    dict.push_back({"وردي", "Pink", "Merah muda", "Warna"});
    dict.push_back({"ذهبي", "Gold", "Emas", "Warna"});
    dict.push_back({"فضي", "Silver", "Perak", "Warna"});
    dict.push_back({"بيج", "Beige", "Krem", "Warna"});
    dict.push_back({"تركواز", "Turquoise", "Biru muda", "Warna"});
    
    return dict;
}

// ==================== DATA KAMUS INDONESIA - ENGLISH (Sampel 100) ====================
vector<Entry> initIndonesianEnglish() {
    vector<Entry> dict;
    
    // Umum / General
    dict.push_back({"Halo", "Hello", "مرحبا", "Umum"});
    dict.push_back({"Selamat tinggal", "Goodbye", "وداعا", "Umum"});
    dict.push_back({"Ya", "Yes", "نعم", "Umum"});
    dict.push_back({"Tidak", "No", "لا", "Umum"});
    dict.push_back({"Tolong", "Please", "من فضلك", "Umum"});
    dict.push_back({"Terima kasih", "Thank you", "شكرا", "Umum"});
    dict.push_back({"Permisi", "Excuse me", "عفوا", "Umum"});
    dict.push_back({"Maaf", "Sorry", "آسف", "Umum"});
    dict.push_back({"Selamat malam", "Good evening", "مساء الخير", "Umum"});
    dict.push_back({"Selamat pagi", "Good morning", "صباح الخير", "Umum"});
    
    // Kata Kerja / Verbs
    dict.push_back({"Makan", "To eat", "يأكل", "Kata Kerja"});
    dict.push_back({"Minum", "To drink", "يشرب", "Kata Kerja"});
    dict.push_back({"Tidur", "To sleep", "ينام", "Kata Kerja"});
    dict.push_back({"Bangun", "To wake up", "يستيقظ", "Kata Kerja"});
    dict.push_back({"Membaca", "To read", "يقرأ", "Kata Kerja"});
    dict.push_back({"Menulis", "To write", "يكتب", "Kata Kerja"});
    dict.push_back({"Berbicara", "To speak", "يتكلم", "Kata Kerja"});
    dict.push_back({"Mendengar", "To hear", "يسمع", "Kata Kerja"});
    dict.push_back({"Melihat", "To see", "يرى", "Kata Kerja"});
    dict.push_back({"Pergi", "To go", "يذهب", "Kata Kerja"});
    dict.push_back({"Datang", "To come", "يأتي", "Kata Kerja"});
    dict.push_back({"Bekerja", "To work", "يعمل", "Kata Kerja"});
    dict.push_back({"Bermain", "To play", "يلعب", "Kata Kerja"});
    dict.push_back({"Belajar", "To learn", "يتعلم", "Kata Kerja"});
    dict.push_back({"Memahami", "To understand", "يفهم", "Kata Kerja"});
    
    // Kata Benda / Nouns
    dict.push_back({"Buku", "Book", "كتاب", "Kata Benda"});
    dict.push_back({"Pena", "Pen", "قلم", "Kata Benda"});
    dict.push_back({"Kertas", "Paper", "ورقة", "Kata Benda"});
    dict.push_back({"Meja", "Table", "طاولة", "Kata Benda"});
    dict.push_back({"Kursi", "Chair", "كرسي", "Kata Benda"});
    dict.push_back({"Rumah", "House", "بيت", "Kata Benda"});
    dict.push_back({"Mobil", "Car", "سيارة", "Kata Benda"});
    dict.push_back({"Telepon", "Phone", "تلفون", "Kata Benda"});
    dict.push_back({"Komputer", "Computer", "كمبيوتر", "Kata Benda"});
    dict.push_back({"Sekolah", "School", "مدرسة", "Kata Benda"});
    dict.push_back({"Universitas", "University", "جامعة", "Kata Benda"});
    dict.push_back({"Rumah Sakit", "Hospital", "مستشفى", "Kata Benda"});
    dict.push_back({"Masjid", "Mosque", "مسجد", "Kata Benda"});
    dict.push_back({"Gereja", "Church", "كنيسة", "Kata Benda"});
    dict.push_back({"Toko", "Store", "محل", "Kata Benda"});
    dict.push_back({"Kota", "City", "مدينة", "Kata Benda"});
    
    // Kata Sifat / Adjectives
    dict.push_back({"Besar", "Big", "كبير", "Kata Sifat"});
    dict.push_back({"Kecil", "Small", "صغير", "Kata Sifat"});
    dict.push_back({"Baru", "New", "جديد", "Kata Sifat"});
    dict.push_back({"Lama", "Old", "قديم", "Kata Sifat"});
    dict.push_back({"Cantik", "Beautiful", "جميل", "Kata Sifat"});
    dict.push_back({"Jelek", "Ugly", "قبيح", "Kata Sifat"});
    dict.push_back({"Cepat", "Fast", "سريع", "Kata Sifat"});
    dict.push_back({"Lambat", "Slow", "بطيء", "Kata Sifat"});
    dict.push_back({"Panas", "Hot", "حار", "Kata Sifat"});
    dict.push_back({"Dingin", "Cold", "بارد", "Kata Sifat"});
    dict.push_back({"Baik", "Good", "جيد", "Kata Sifat"});
    dict.push_back({"Buruk", "Bad", "سيء", "Kata Sifat"});
    dict.push_back({"Mudah", "Easy", "سهل", "Kata Sifat"});
    dict.push_back({"Sulit", "Difficult", "صعب", "Kata Sifat"});
    dict.push_back({"Kaya", "Rich", "غني", "Kata Sifat"});
    
    // Angka / Numbers
    dict.push_back({"Nol", "Zero", "صفر", "Angka"});
    dict.push_back({"Satu", "One", "واحد", "Angka"});
    dict.push_back({"Dua", "Two", "اثنان", "Angka"});
    dict.push_back({"Tiga", "Three", "ثلاثة", "Angka"});
    dict.push_back({"Empat", "Four", "أربعة", "Angka"});
    dict.push_back({"Lima", "Five", "خمسة", "Angka"});
    dict.push_back({"Enam", "Six", "ستة", "Angka"});
    dict.push_back({"Tujuh", "Seven", "سبعة", "Angka"});
    dict.push_back({"Delapan", "Eight", "ثمانية", "Angka"});
    dict.push_back({"Sembilan", "Nine", "تسعة", "Angka"});
    dict.push_back({"Sepuluh", "Ten", "عشرة", "Angka"});
    dict.push_back({"Dua puluh", "Twenty", "عشرون", "Angka"});
    dict.push_back({"Seratus", "Hundred", "مئة", "Angka"});
    dict.push_back({"Seribu", "Thousand", "ألف", "Angka"});
    dict.push_back({"Juta", "Million", "مليون", "Angka"});
    
    // Waktu / Time
    dict.push_back({"Hari ini", "Today", "اليوم", "Waktu"});
    dict.push_back({"Besok", "Tomorrow", "غدا", "Waktu"});
    dict.push_back({"Kemarin", "Yesterday", "أمس", "Waktu"});
    dict.push_back({"Sekarang", "Now", "الآن", "Waktu"});
    dict.push_back({"Sore", "Evening", "المساء", "Waktu"});
    dict.push_back({"Pagi", "Morning", "الصباح", "Waktu"});
    dict.push_back({"Malam", "Night", "ليل", "Waktu"});
    dict.push_back({"Siang", "Noon", "ظهر", "Waktu"});
    dict.push_back({"Jam", "Hour", "ساعة", "Waktu"});
    dict.push_back({"Menit", "Minute", "دقيقة", "Waktu"});
    dict.push_back({"Detik", "Second", "ثانية", "Waktu"});
    dict.push_back({"Minggu", "Week", "أسبوع", "Waktu"});
    dict.push_back({"Bulan", "Month", "شهر", "Waktu"});
    dict.push_back({"Tahun", "Year", "سنة", "Waktu"});
    dict.push_back({"Abad", "Century", "قرن", "Waktu"});
    
    // Keluarga / Family
    dict.push_back({"Ayah", "Father", "أب", "Keluarga"});
    dict.push_back({"Ibu", "Mother", "أم", "Keluarga"});
    dict.push_back({"Anak laki-laki", "Son", "ابن", "Keluarga"});
    dict.push_back({"Anak perempuan", "Daughter", "ابنة", "Keluarga"});
    dict.push_back({"Saudara laki-laki", "Brother", "أخ", "Keluarga"});
    dict.push_back({"Saudara perempuan", "Sister", "أخت", "Keluarga"});
    dict.push_back({"Kakek", "Grandfather", "جد", "Keluarga"});
    dict.push_back({"Nenek", "Grandmother", "جدة", "Keluarga"});
    dict.push_back({"Paman", "Uncle", "عم", "Keluarga"});
    dict.push_back({"Bibi", "Aunt", "خالة", "Keluarga"});
    dict.push_back({"Suami", "Husband", "زوج", "Keluarga"});
    dict.push_back({"Istri", "Wife", "زوجة", "Keluarga"});
    dict.push_back({"Keluarga", "Family", "عائلة", "Keluarga"});
    dict.push_back({"Anak", "Child", "طفل", "Keluarga"});
    dict.push_back({"Pria", "Man", "رجل", "Keluarga"});
    
    // Makanan / Food
    dict.push_back({"Roti", "Bread", "خبز", "Makanan"});
    dict.push_back({"Daging", "Meat", "لحم", "Makanan"});
    dict.push_back({"Ikan", "Fish", "سمك", "Makanan"});
    dict.push_back({"Ayam", "Chicken", "دجاج", "Makanan"});
    dict.push_back({"Nasi", "Rice", "أرز", "Makanan"});
    dict.push_back({"Sayuran", "Vegetables", "خضار", "Makanan"});
    dict.push_back({"Buah", "Fruit", "فاكهة", "Makanan"});
    dict.push_back({"Air", "Water", "ماء", "Makanan"});
    dict.push_back({"Kopi", "Coffee", "قهوة", "Makanan"});
    dict.push_back({"Teh", "Tea", "شاي", "Makanan"});
    dict.push_back({"Susu", "Milk", "حليب", "Makanan"});
    dict.push_back({"Minyak", "Oil", "زيت", "Makanan"});
    dict.push_back({"Garam", "Salt", "ملح", "Makanan"});
    dict.push_back({"Gula", "Sugar", "سكر", "Makanan"});
    dict.push_back({"Telur", "Egg", "بيض", "Makanan"});
    
    // Perjalanan / Travel
    dict.push_back({"Bandara", "Airport", "مطار", "Perjalanan"});
    dict.push_back({"Kereta", "Train", "قطار", "Perjalanan"});
    dict.push_back({"Bus", "Bus", "باص", "Perjalanan"});
    dict.push_back({"Kapal", "Ship", "سفينة", "Perjalanan"});
    dict.push_back({"Pesawat", "Airplane", "طائرة", "Perjalanan"});
    dict.push_back({"Stasiun", "Station", "محطة", "Perjalanan"});
    dict.push_back({"Tiket", "Ticket", "تذكرة", "Perjalanan"});
    dict.push_back({"Paspor", "Passport", "جواز سفر", "Perjalanan"});
    dict.push_back({"Hotel", "Hotel", "فندق", "Perjalanan"});
    dict.push_back({"Kamar", "Room", "غرفة", "Perjalanan"});
    dict.push_back({"Perjalanan", "Travel", "سفر", "Perjalanan"});
    dict.push_back({"Menginap", "Stay", "إقامة", "Perjalanan"});
    dict.push_back({"Keberangkatan", "Departure", "خروج", "Perjalanan"});
    dict.push_back({"Kedatangan", "Arrival", "وصول", "Perjalanan"});
    dict.push_back({"Reservasi", "Reservation", "حجز", "Perjalanan"});
    
    // Pendidikan / Education
    dict.push_back({"Siswa", "Student", "طالب", "Pendidikan"});
    dict.push_back({"Guru", "Teacher", "معلم", "Pendidikan"});
    dict.push_back({"Pelajaran", "Lesson", "درس", "Pendidikan"});
    dict.push_back({"Ujian", "Exam", "امتحان", "Pendidikan"});
    dict.push_back({"Nilai", "Grade", "درجة", "Pendidikan"});
    dict.push_back({"Perpustakaan", "Library", "مكتبة", "Pendidikan"});
    dict.push_back({"Buku", "Book", "كتاب", "Pendidikan"});
    dict.push_back({"Kelas", "Classroom", "قاعة", "Pendidikan"});
    dict.push_back({"Universitas", "University", "جامعة", "Pendidikan"});
    dict.push_back({"Fakultas", "College", "كلية", "Pendidikan"});
    dict.push_back({"Penelitian", "Research", "بحث", "Pendidikan"});
    dict.push_back({"Ilmu", "Science", "علم", "Pendidikan"});
    dict.push_back({"Sejarah", "History", "تاريخ", "Pendidikan"});
    dict.push_back({"Matematika", "Mathematics", "رياضيات", "Pendidikan"});
    dict.push_back({"Fisika", "Physics", "فيزياء", "Pendidikan"});
    
    // Pekerjaan / Jobs
    dict.push_back({"Dokter", "Doctor", "طبيب", "Pekerjaan"});
    dict.push_back({"Insinyur", "Engineer", "مهندس", "Pekerjaan"});
    dict.push_back({"Pengacara", "Lawyer", "محامي", "Pekerjaan"});
    dict.push_back({"Guru", "Teacher", "معلم", "Pekerjaan"});
    dict.push_back({"Pekerja", "Worker", "عامل", "Pekerjaan"});
    dict.push_back({"Pedagang", "Merchant", "تاجر", "Pekerjaan"});
    dict.push_back({"Petani", "Farmer", "فلاح", "Pekerjaan"});
    dict.push_back({"Nelayan", "Fisherman", "صياد", "Pekerjaan"});
    dict.push_back({"Koki", "Cook", "طباخ", "Pekerjaan"});
    dict.push_back({"Sopir", "Driver", "سائق", "Pekerjaan"});
    dict.push_back({"Polisi", "Police", "شرطي", "Pekerjaan"});
    dict.push_back({"Tentara", "Soldier", "جندي", "Pekerjaan"});
    dict.push_back({"Penulis", "Writer", "كاتب", "Pekerjaan"});
    dict.push_back({"Seniman", "Artist", "فنان", "Pekerjaan"});
    dict.push_back({"Ilmuwan", "Scientist", "عالم", "Pekerjaan"});
    
    // Badan Manusia / Human Body
    dict.push_back({"Kepala", "Head", "رأس", "Badan Manusia"});
    dict.push_back({"Mata", "Eye", "عين", "Badan Manusia"});
    dict.push_back({"Telinga", "Ear", "أذن", "Badan Manusia"});
    dict.push_back({"Hidung", "Nose", "أنف", "Badan Manusia"});
    dict.push_back({"Mulut", "Mouth", "فم", "Badan Manusia"});
    dict.push_back({"Tangan", "Hand", "يد", "Badan Manusia"});
    dict.push_back({"Kaki", "Leg", "رجل", "Badan Manusia"});
    dict.push_back({"Jantung", "Heart", "قلب", "Badan Manusia"});
    dict.push_back({"Perut", "Stomach", "معدة", "Badan Manusia"});
    dict.push_back({"Punggung", "Back", "ظهر", "Badan Manusia"});
    dict.push_back({"Kulit", "Skin", "جلد", "Badan Manusia"});
    dict.push_back({"Tulang", "Bone", "عظم", "Badan Manusia"});
    dict.push_back({"Otot", "Muscle", "عضلة", "Badan Manusia"});
    dict.push_back({"Darah", "Blood", "دم", "Badan Manusia"});
    dict.push_back({"Rambut", "Hair", "شعر", "Badan Manusia"});
    
    // Warna / Colors
    dict.push_back({"Merah", "Red", "أحمر", "Warna"});
    dict.push_back({"Biru", "Blue", "أزرق", "Warna"});
    dict.push_back({"Hijau", "Green", "أخضر", "Warna"});
    dict.push_back({"Kuning", "Yellow", "أصفر", "Warna"});
    dict.push_back({"Oranye", "Orange", "برتقالي", "Warna"});
    dict.push_back({"Ungu", "Purple", "بنفسجي", "Warna"});
    dict.push_back({"Coklat", "Brown", "بني", "Warna"});
    dict.push_back({"Hitam", "Black", "أسود", "Warna"});
    dict.push_back({"Putih", "White", "أبيض", "Warna"});
    dict.push_back({"Abu-abu", "Gray", "رمادي", "Warna"});
    dict.push_back({"Merah muda", "Pink", "وردي", "Warna"});
    dict.push_back({"Emas", "Gold", "ذهبي", "Warna"});
    dict.push_back({"Perak", "Silver", "فضي", "Warna"});
    dict.push_back({"Krem", "Beige", "بيج", "Warna"});
    dict.push_back({"Biru muda", "Turquoise", "تركواز", "Warna"});
    
    return dict;
}

// ==================== KONSTANTA BATAS PANJANG ====================

void searchArabicEnglish(const vector<Entry>& dict) {
    string query;
    
    cout << "\n+----------------------------------------------------------+";
    cout << "\n|  🔍 CARI KATA (BAHASA ARAB -> ENGLISH)                   |";
    cout << "\n+----------------------------------------------------------+";
    showInputRules();
    cout << "\n\nMasukkan kata dalam bahasa Arab: ";
    getline(cin, query);
    getline(cin, query);
    
    // Validasi panjang input
    if (!validateInputLength(query, MIN_WORD_LENGTH, MAX_ARABIC_LENGTH, "Kata pencarian")) {
        return;
    }
    
    cout << "\n" << string(60, '=') << "\n";
    cout << "Hasil Pencarian: Arab -> English\n";
    cout << string(60, '=') << "\n\n";
    
    bool found = false;
    for (const auto& entry : dict) {
        if (entry.arabic.find(query) != string::npos || 
            entry.english.find(query) != string::npos) {
            cout << "Arab: " << entry.arabic << "\n";
            cout << "English: " << entry.english << "\n";
            cout << "Indonesia: " << entry.indonesian << "\n";
            cout << "Kategori: " << entry.category << "\n";
            cout << string(40, '-') << "\n";
            found = true;
        }
    }
    
    if (!found) {
        showNotFoundMessage(query, "Arab");
    }
}

void searchIndonesianEnglish(const vector<Entry>& dict) {
    string query;
    
    cout << "\n+----------------------------------------------------------+";
    cout << "\n|  🔍 CARI KATA (BAHASA INDONESIA -> ENGLISH)              |";
    cout << "\n+----------------------------------------------------------+";
    showInputRules();
    cout << "\n\nMasukkan kata dalam bahasa Indonesia: ";
    getline(cin, query);
    getline(cin, query);
    
    // Validasi panjang input
    if (!validateInputLength(query, MIN_WORD_LENGTH, MAX_INDONESIAN_LENGTH, "Kata pencarian")) {
        return;
    }
    
    cout << "\n" << string(60, '=') << "\n";
    cout << "Hasil Pencarian: Indonesia -> English\n";
    cout << string(60, '=') << "\n\n";
    
    bool found = false;
    for (const auto& entry : dict) {
        if (entry.indonesian.find(query) != string::npos || 
            entry.english.find(query) != string::npos) {
            cout << "Indonesia: " << entry.indonesian << "\n";
            cout << "English: " << entry.english << "\n";
            cout << "Arab: " << entry.arabic << "\n";
            cout << "Kategori: " << entry.category << "\n";
            cout << string(40, '-') << "\n";
            found = true;
        }
    }
    
    if (!found) {
        showNotFoundMessage(query, "Indonesia");
    }
}

void searchByCategory(const vector<Entry>& dict) {
    cout << "\n=== KATEGORI KAMUS ===\n";
    for (int i = 0; i < 16; i++) {
        cout << i + 1 << ". " << categories[i] << "\n";
    }
    
    int choice;
    cout << "\nPilih kategori (1-16): ";
    cin >> choice;
    
    if (choice < 1 || choice > 16) {
        cout << "Pilihan tidak valid!\n";
        return;
    }
    
    string selectedCategory = categories[choice - 1];
    
    cout << "\n" << string(60, '=') << "\n";
    cout << "Kategori: " << selectedCategory << "\n";
    cout << string(60, '=') << "\n\n";
    
    bool found = false;
    for (const auto& entry : dict) {
        if (entry.category == selectedCategory) {
            cout << "Arab: " << entry.arabic << " | English: " << entry.english 
                 << " | Indonesia: " << entry.indonesian << "\n";
            found = true;
        }
    }
    
    if (!found) {
        cout << "Tidak ada kata dalam kategori ini.\n";
    }
}

void displayAllWords(const vector<Entry>& dict) {
    cout << "\n" << string(70, '=') << "\n";
    cout << "                    DAFTAR KAMUS LENGKAP\n";
    cout << string(70, '=') << "\n\n";
    
    for (size_t i = 0; i < dict.size(); i++) {
        const auto& entry = dict[i];
        cout << "[" << setw(3) << i + 1 << "] ";
        cout << "Arab: " << setw(15) << entry.arabic << " | ";
        cout << "English: " << setw(15) << entry.english << " | ";
        cout << "Indo: " << entry.indonesian << "\n";
    }
    
    cout << "\nTotal kosakata: " << dict.size() << "\n";
}

void addNewWord(vector<Entry>& dict) {
    Entry newEntry;
    bool valid = true;
    
    cout << "\n+----------------------------------------------------------+\n";
    cout << "|                  ✏️  TAMBAH KATA BARU                     |\n";
    cout << "+----------------------------------------------------------+";
    showInputRules();
    
    // Input kata Arab
    cout << "\n\n📝 Masukkan kata Arab: ";
    getline(cin, newEntry.arabic);
    getline(cin, newEntry.arabic);
    if (!validateInputLength(newEntry.arabic, MIN_WORD_LENGTH, MAX_ARABIC_LENGTH, "Kata Arab")) {
        return;
    }
    
    // Input terjemahan English
    cout << "📝 Masukkan terjemahan English: ";
    getline(cin, newEntry.english);
    if (!validateInputLength(newEntry.english, MIN_WORD_LENGTH, MAX_ENGLISH_LENGTH, "Terjemahan English")) {
        return;
    }
    
    // Input terjemahan Indonesia
    cout << "📝 Masukkan terjemahan Indonesia: ";
    getline(cin, newEntry.indonesian);
    if (!validateInputLength(newEntry.indonesian, MIN_WORD_LENGTH, MAX_INDONESIAN_LENGTH, "Terjemahan Indonesia")) {
        return;
    }
    
    cout << "\n📂 Pilih Kategori:\n";
    for (int i = 0; i < 16; i++) {
        cout << "   " << i + 1 << ". " << categories[i] << "\n";
    }
    cout << "\n   Pilihan (1-16): ";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    if (choice >= 1 && choice <= 16) {
        newEntry.category = categories[choice - 1];
        dict.push_back(newEntry);
        
        // Tampilkan pesan sukses
        cout << "\n+----------------------------------------------------------+\n";
        cout << "|              ✅ KATA BERHASIL DITAMBAHKAN               |\n";
        cout << "+----------------------------------------------------------+\n";
        cout << "|  Arab:       " << setw(45) << left << newEntry.arabic << "|\n";
        cout << "|  English:    " << setw(45) << left << newEntry.english << "|\n";
        cout << "|  Indonesia:  " << setw(45) << left << newEntry.indonesian << "|\n";
        cout << "|  Kategori:   " << setw(45) << left << newEntry.category << "|\n";
        cout << "+----------------------------------------------------------+\n";
    } else {
        cout << "\n⚠️  Pilihan tidak valid. Kata disimpan dengan kategori default.\n";
        newEntry.category = "Umum";
        dict.push_back(newEntry);
    }
}

void exportToFile(const vector<Entry>& dict, const string& filename) {
    ofstream file(filename);
    
    if (file.is_open()) {
        file << "No,Arabic,English,Indonesian,Category\n";
        for (size_t i = 0; i < dict.size(); i++) {
            file << i + 1 << ",";
            file << dict[i].arabic << ",";
            file << dict[i].english << ",";
            file << dict[i].indonesian << ",";
            file << dict[i].category << "\n";
        }
        file.close();
        cout << "\nData berhasil diekspor ke file: " << filename << "\n";
    } else {
        cout << "\nGagal membuat file!\n";
    }
}

// ==================== MENU UTAMA ====================

void showMainMenu() {
    cout << "\n";
    cout << "+----------------------------------------------------------+\n";
    cout << "|          KAMUS DIGITAL MULTI-BAHASA                      |\n";
    cout << "|     Arabic - English - Indonesia Dictionary              |\n";
    cout << "+----------------------------------------------------------+\n";
    cout << "|  1. Cari kata (Arab -> English)                           |\n";
    cout << "|  2. Cari kata (Indonesia -> English)                      |\n";
    cout << "|  3. Cari berdasarkan Kategori                             |\n";
    cout << "|  4. Tampilkan semua kosakata                              |\n";
    cout << "|  5. Tambah kata baru                                      |\n";
    cout << "|  6. Ekspor ke CSV                                         |\n";
    cout << "|  7. Keluar                                                |\n";
    cout << "+-----------------------------------------------------------+\n";
    cout << "|  Pilih menu (1-7): ";
}

int main() {
    // Inisialisasi UTF-8 untuk karakter Arab
    initUTF8();
    
    // Inisialisasi kamus
    vector<Entry> arabicDict = initArabicEnglish();
    vector<Entry> indonesianDict = initIndonesianEnglish();
    
    int choice;
    
    do {
        showMainMenu();
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1:
                searchArabicEnglish(arabicDict);
                break;
            case 2:
                searchIndonesianEnglish(indonesianDict);
                break;
            case 3:
                searchByCategory(arabicDict);
                break;
            case 4:
                displayAllWords(arabicDict);
                break;
            case 5:
                addNewWord(arabicDict);
                break;
            case 6:
                exportToFile(arabicDict, "kamus_export.csv");
                break;
            case 7:
                cout << "\nTerima kasih telah menggunakan Kamus Digital!\n";
                cout << "Selamat belajar bahasa!\n";
                break;
            default:
                cout << "\nPilihan tidak valid!\n";
        }
        
        if (choice != 7) {
            cout << "\nTekan Enter untuk melanjutkan...";
            cin.get();
        }
        
    } while (choice != 7);
    
    return 0;
}
