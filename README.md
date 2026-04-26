# Shelfic - Kütüphane Yönetim Sistemi / Library Management System

## [TR] Proje Hakkında
Shelfic, Qt ve C++ kullanılarak geliştirilmiş, SQLite veritabanı altyapısına sahip bir kütüphane yönetim otomasyonudur. Kitap ekleme/silme, üye yönetimi ve ödünç verme işlemlerini sekmeli bir arayüz üzerinden kolayca yapmanızı sağlar.

### Özellikler
- Kitap Kaydı ve Takibi: Kitap adı, yazar ve ID ile yönetim.
- Üye Yönetimi: Üye adı, soyadı ve ID ile kayıt.
- Ödünç Sistemi: Kitap ve Üye ID'leri üzerinden ödünç verme ve iade alma.
- Akıllı Kontrol: Ödünç verilmiş bir kitabın tekrar verilmesini engelleme.

### Kurulum
1. Bilgisayarınızda Qt Framework ve bir C++ derleyicisinin (GCC, MSVC veya Clang) kurulu olduğundan emin olun.
2. Bu depoyu klonlayın:
   git clone https://github.com/myakupozluk/Shelfic.git
3. Proje dizinine gidin ve `CMakeLists.txt` dosyasını Qt Creator ile açın.
4. Projeyi derleyin (Build) ve çalıştırın (Run).
5. Uygulama, ilk açılışta `library.db` dosyasını otomatik olarak oluşturacaktır.

---

## [EN] About the Project
Shelfic is a library management automation developed using Qt and C++ with a SQLite database backend. It allows you to easily perform book addition/deletion, member management, and borrowing transactions through a tabbed interface.

### Features
- Book Registration: Management by book title, author, and ID.
- Member Management: Registration with member name, surname, and ID.
- Borrowing System: Borrowing and returning via Book and Member IDs.
- Logic Control: Preventing a book that is already borrowed from being lent again.

### Installation
1. Ensure Qt Framework and a C++ compiler (GCC, MSVC, or Clang) are installed on your computer.
2. Clone this repository:
   git clone https://github.com/myakupozluk/Shelfic.git
3. Navigate to the project directory and open the `CMakeLists.txt` file with Qt Creator.
4. Build and Run the project.
5. The application will automatically create the `library.db` file upon the first launch.
