//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "CryptoMaster.h"

#include <fstream>
#include <iostream>
#include <memory>
#include "cryptopp/cryptlib.h"
#include "cryptopp/salsa.h"

template<class T>
T min(T a, T b) {
    if (a < b)return a;
    return b;
}

void CryptoMaster::Encrypt(std::string src, std::string dst)
{
    using namespace CryptoPP;
    unsigned char key1[16];
    unsigned char iv1[8];
    Salsa20::Encryption enc0;
    enc0.SetKeyWithIV(key0, 16, iv0, 8);
    enc0.ProcessData(key1, key0, 16);
    enc0.ProcessData(iv1, iv0, 8);
    std::ifstream ifs(src, std::ios::binary);
    if (!ifs) {
        std::cout << "unexist file:" << src << "\n";
        return;
    }
    std::cout << "start encrypting:" << src << "\n";
    ifs.seekg(0, std::ios_base::end);
    size_t filesize = ifs.tellg();
    ifs.seekg(0, std::ios_base::beg);
    std::ofstream ofs;
    ofs.open(dst, std::ios::binary | std::ios::trunc);
    //process_section_sizeバイトごとにちまちま読み込みと書き込みを行う
    std::shared_ptr<unsigned char[]> src_data = std::make_shared<unsigned char[]>(process_section_size);
    std::shared_ptr<unsigned char[]> dst_data = std::make_shared<unsigned char[]>(process_section_size);
    size_t processed = 0;
    while (processed < filesize) {
        size_t size = min(process_section_size, filesize - processed);
        ifs.read((char*)src_data.get(), size);
        Salsa20::Encryption enc1;
        enc1.SetKeyWithIV(key1, 16, iv1, 8);
        enc1.ProcessData(dst_data.get(), (const unsigned char*)src_data.get(), size);
        ofs.write((const char*)dst_data.get(), size);
        processed += size;
    }
    ofs.close();
    std::cout << "end encrypting\n";
}