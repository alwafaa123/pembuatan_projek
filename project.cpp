#include <iostream> 
#include <vector>   
#include <string>   
#include <limits>   
#include <algorithm> 
#include <iomanip>   

class Product {
public:
    std::string barcode;
    std::string name;
    double purchasePrice;
    double sellingPrice;
    std::string unit;
    std::string category;
    int stock;

    Product(std::string bc = "", std::string n = "", double pp = 0.0, double sp = 0.0,
            std::string u = "", std::string c = "", int s = 0)
        : barcode(bc), name(n), purchasePrice(pp), sellingPrice(sp),
          unit(u), category(c), stock(s) {}

    void display() const {
        std::cout << "  Barcode    : " << barcode << std::endl;
        std::cout << "  Nama       : " << name << std::endl;
        std::cout << "  Harga Beli : Rp " << purchasePrice << std::endl;
        std::cout << "  Harga Jual : Rp " << sellingPrice << std::endl;
        std::cout << "  Satuan     : " << unit << std::endl;
        std::cout << "  Kategori   : " << category << std::endl;
        std::cout << "  Stok       : " << stock << std::endl;
    }
};

void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void displayMenu() {
    std::cout << "\n--- Sistem Informasi Manajemen Stok Gudang ---" << std::endl;
    std::cout << "1. Manajemen Produk" << std::endl;
    std::cout << "2. Barang Masuk" << std::endl;
    std::cout << "3. Barang Keluar" << std::endl;
    std::cout << "4. Laporan Inventarisasi" << std::endl;
    std::cout << "5. Keluar" << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Pilih opsi: ";
}

void manageProducts(std::vector<Product>& products) {
    int choice;
    do {
        std::cout << "\n--- Manajemen Produk ---" << std::endl;
        std::cout << "1. Tambah Produk Baru" << std::endl;
        std::cout << "2. Lihat Daftar Produk" << std::endl;
        std::cout << "3. Edit Produk" << std::endl;
        std::cout << "4. Hapus Produk" << std::endl;
        std::cout << "5. Kembali ke Menu Utama" << std::endl;
        std::cout << "Pilih opsi: ";
        std::cin >> choice;
        clearInputBuffer();

        switch (choice) {
            case 1: { 
                Product newProduct;
                std::cout << "\n--- Tambah Produk Baru ---" << std::endl;
                std::cout << "Masukkan Barcode        : "; std::getline(std::cin, newProduct.barcode);
                std::cout << "Masukkan Nama Produk    : "; std::getline(std::cin, newProduct.name);
                std::cout << "Masukkan Harga Beli (Rp): "; std::cin >> newProduct.purchasePrice;
                std::cout << "Masukkan Harga Jual (Rp): "; std::cin >> newProduct.sellingPrice;
                clearInputBuffer(); // Membersihkan buffer setelah input double
                std::cout << "Masukkan Satuan (contoh: Pcs, Kg): "; std::getline(std::cin, newProduct.unit);
                std::cout << "Masukkan Kategori       : "; std::getline(std::cin, newProduct.category);
                newProduct.stock = 0;
                products.push_back(newProduct);
                std::cout << "Produk '" << newProduct.name << "' berhasil ditambahkan!" << std::endl;
                break;
            }
            case 2: {
                std::cout << "\n--- Daftar Produk ---" << std::endl;
                if (products.empty()) {
                    std::cout << "Belum ada produk terdaftar." << std::endl;
                } else {
                    for (size_t i = 0; i < products.size(); ++i) {
                        std::cout << "\nProduk #" << i + 1 << std::endl;
                        products[i].display();
                    }
                }
                break;
            }
            case 3: {
                std::string searchBarcode;
                std::cout << "\n--- Edit Produk ---" << std::endl;
                std::cout << "Masukkan Barcode produk yang akan diedit: ";
                std::getline(std::cin, searchBarcode);

                bool found = false;
                for (Product& p : products) {
                    if (p.barcode == searchBarcode) {
                        found = true;
                        std::cout << "Produk ditemukan. Masukkan data baru:" << std::endl;
                        std::cout << "Nama Produk Baru    (" << p.name << "): "; std::getline(std::cin, p.name);
                        std::cout << "Harga Beli Baru (Rp)(" << p.purchasePrice << "): "; std::cin >> p.purchasePrice;
                        std::cout << "Harga Jual Baru (Rp)(" << p.sellingPrice << "): "; std::cin >> p.sellingPrice;
                        clearInputBuffer(); // Membersihkan buffer setelah input double
                        std::cout << "Satuan Baru         (" << p.unit << "): "; std::getline(std::cin, p.unit);
                        std::cout << "Kategori Baru       (" << p.category << "): "; std::getline(std::cin, p.category);
                        std::cout << "Produk berhasil diperbarui!" << std::endl;
                        break;
                    }
                }
                if (!found) {
                    std::cout << "Produk dengan barcode '" << searchBarcode << "' tidak ditemukan." << std::endl;
                }
                break;
            }
            case 4: {
                std::string deleteBarcode;
                std::cout << "\n--- Hapus Produk ---" << std::endl;
                std::cout << "Masukkan Barcode produk yang akan dihapus: ";
                std::getline(std::cin, deleteBarcode);

                size_t initialSize = products.size();
                products.erase(std::remove_if(products.begin(), products.end(),
                                               [&](const Product& p) { return p.barcode == deleteBarcode; }),
                               products.end());

                if (products.size() < initialSize) {
                    std::cout << "Produk dengan barcode '" << deleteBarcode << "' berhasil dihapus." << std::endl;
                } else {
                    std::cout << "Produk dengan barcode '" << deleteBarcode << "' tidak ditemukan." << std::endl;
                }
                break;
            }
            case 5:
                std::cout << "Kembali ke Menu Utama..." << std::endl;
                break;
            default:
                std::cout << "Pilihan tidak valid. Silakan coba lagi." << std::endl;
                break;
        }
    } while (choice != 5);
}

void receiveGoods(std::vector<Product>& products) {
    std::string scannedBarcode;
    int quantity;

    std::cout << "\n--- Barang Masuk ---" << std::endl;
    std::cout << "Pindai Barcode Produk (simulasi: ketik barcode lalu enter): ";
    std::getline(std::cin, scannedBarcode);

    bool found = false;
    for (Product& p : products) {
        if (p.barcode == scannedBarcode) {
            found = true;
            std::cout << "Produk ditemukan: " << p.name << std::endl;
            std::cout << "Masukkan Jumlah Barang Masuk: ";
            std::cin >> quantity;
            clearInputBuffer();

            if (quantity > 0) {
                p.stock += quantity;
                std::cout << "Stok '" << p.name << "' berhasil ditambahkan sebanyak " << quantity << " unit." << std::endl;
                std::cout << "Stok terkini: " << p.stock << std::endl;
            } else {
                std::cout << "Jumlah harus lebih dari 0." << std::endl;
            }
            break;
        }
    }
    if (!found) {
        std::cout << "Produk dengan barcode '" << scannedBarcode << "' tidak ditemukan." << std::endl;
        std::cout << "Silakan tambahkan produk ini terlebih dahulu di Manajemen Produk." << std::endl;
    }
}

void issueGoods(std::vector<Product>& products) {
    std::string scannedBarcode;
    int quantity;

    std::cout << "\n--- Barang Keluar ---" << std::endl;
    std::cout << "Pindai Barcode Produk (simulasi: ketik barcode lalu enter): ";
    std::getline(std::cin, scannedBarcode);

    bool found = false;
    for (Product& p : products) {
        if (p.barcode == scannedBarcode) {
            found = true;
            std::cout << "Produk ditemukan: " << p.name << std::endl;
            std::cout << "Stok Tersedia: " << p.stock << std::endl;
            std::cout << "Masukkan Jumlah Barang Keluar: ";
            std::cin >> quantity;
            clearInputBuffer();

            if (quantity > 0) {
                if (p.stock >= quantity) {
                    p.stock -= quantity;
                    std::cout << "Stok '" << p.name << "' berhasil dikurangi sebanyak " << quantity << " unit." << std::endl;
                    std::cout << "Stok terkini: " << p.stock << std::endl;
                } else {
                    std::cout << "Stok tidak mencukupi! Hanya ada " << p.stock << " unit." << std::endl;
                }
            } else {
                std::cout << "Jumlah harus lebih dari 0." << std::endl;
            }
            break;
        }
    }
    if (!found) {
        std::cout << "Produk dengan barcode '" << scannedBarcode << "' tidak ditemukan." << std::endl;
    }
}

void generateReports(const std::vector<Product>& products) {
    std::cout << "\n--- Laporan Inventarisasi (Stok Terkini) ---" << std::endl;
    if (products.empty()) {
        std::cout << "Belum ada produk dalam stok." << std::endl;
        return;
    }


    std::cout << std::left << std::setw(15) << "BARCODE"
              << std::setw(30) << "NAMA PRODUK"
              << std::setw(10) << "STOK"
              << std::setw(10) << "SATUAN"
              << std::setw(20) << "KATEGORI" << std::endl;
    std::cout << std::string(85, '-') << std::endl;

    for (const Product& p : products) {
        std::cout << std::left << std::setw(15) << p.barcode
                  << std::setw(30) << p.name
                  << std::setw(10) << p.stock
                  << std::setw(10) << p.unit
                  << std::setw(20) << p.category << std::endl;
    }
    std::cout << std::string(85, '-') << std::endl;
}

int main() {
    std::vector<Product> products;

    products.push_back(Product("123456789012", "Sabun Mandi Citra", 10000.0, 12500.0, "Pcs", "Perlengkapan Mandi", 50));
    products.push_back(Product("987654321098", "Pasta Gigi Pepsodent", 8000.0, 10000.0, "Tube", "Perlengkapan Mandi", 30));
    products.push_back(Product("112233445566", "Shampoo Pantene 250ml", 15000.0, 18000.0, "Botol", "Perlengkapan Mandi", 25));

    int choice;
    do {
        displayMenu();
        std::cin >> choice;
        clearInputBuffer();

        switch (choice) {
            case 1:
                manageProducts(products);
                break;
            case 2:
                receiveGoods(products);
                break;
            case 3:
                issueGoods(products);
                break;
            case 4:
                generateReports(products);
                break;
            case 5:
                std::cout << "Terima kasih telah menggunakan aplikasi ini. Sampai jumpa!" << std::endl;
                break;
            default:
                std::cout << "Pilihan tidak valid. Silakan masukkan angka 1-5." << std::endl;
                break;
        }
    } while (choice != 5);

    return 0;
}