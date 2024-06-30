FROM ubuntu:latest

# Gerekli paketleri yükle
RUN apt-get update && apt-get install -y gcc

# Çalışma dizinini ayarla
WORKDIR /usr/src/app

# Kaynak kod dosyalarını kopyala
COPY a.out a.out

COPY data data

# Başlangıç komutunu ayarla
CMD ["./a.out"]
