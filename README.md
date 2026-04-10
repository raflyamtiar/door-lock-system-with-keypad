# door-lock-system-with-keypad

Sistem **door lock berbasis Arduino** dengan input **keypad 4x4**, output **servo** sebagai pengunci, **LCD I2C 16x2** sebagai tampilan, dan **buzzer** sebagai notifikasi.

## Fitur Utama

- Input password menggunakan keypad (default: `123456`).
- Tampilan instruksi dan status pada LCD I2C.
- Password dimasking (`*`) saat diketik.
- Buka/kunci pintu menggunakan servo:
  - Sudut kunci: `50°`
  - Sudut buka: `140°`
- Notifikasi buzzer untuk setiap input, password benar, dan password salah.
- Hapus input terakhir.
- Reset input password.
- Ganti password setelah verifikasi password lama.

## File Proyek

- `door-lock-with-keypad.ino` — source code utama sistem door lock.

## Mapping Pin (sesuai kode)

- **Buzzer**: pin `12`
- **Servo**: pin `11`
- **Keypad Row**: pin `2, 3, 4, 5`
- **Keypad Column**: pin `6, 7, 8, 9`
- **LCD I2C**: alamat `0x27` (SDA/SCL mengikuti board Arduino)

## Kontrol Keypad

- Angka `0-9`: input password
- `B`: hapus karakter terakhir
- `*`: reset/clear seluruh input
- `D`: eksekusi buka/kunci pintu
- `A`: ubah password (dengan verifikasi password lama)

## Library yang Digunakan

Pastikan library berikut sudah terpasang di Arduino IDE:

- `Servo.h`
- `LiquidCrystal_I2C.h`
- `Keypad.h`

## Cara Menjalankan

1. Buka file `door-lock-with-keypad.ino` di Arduino IDE.
2. Pilih board dan port yang sesuai.
3. Upload program ke board Arduino.
4. Masukkan password melalui keypad, lalu tekan `D` untuk membuka/mengunci.

## Catatan

- Password default saat awal nyala adalah `123456`.
- Jika password salah, buzzer akan berbunyi 3 kali dan LCD menampilkan peringatan.
- Password baru diset menjadi 6 digit saat proses ganti password.
