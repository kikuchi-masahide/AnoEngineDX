//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "CryptoMaster.h"

#include <fstream>
#include "Log.h"

std::shared_ptr<unsigned char[]> CryptoMaster::Decrypt(std::shared_ptr<unsigned char[]> src, size_t length)
{
    using namespace CryptoPP;
    unsigned char key1[16];
    unsigned char iv1[8];
    Salsa20::Encryption enc0;
    enc0.SetKeyWithIV(key0, 16, iv0, 8);
    enc0.ProcessData(key1, key0, 16);
    enc0.ProcessData(iv1, iv0, 8);
    std::shared_ptr<unsigned char[]> dst = std::make_shared<unsigned char[]>(length);
    auto dst_pos = dst.get();
    auto cur = src.get();
    size_t processed = 0;
    while (processed < length) {
        size_t size = length - processed;
        if (size > process_section_size) {
            size = process_section_size;
        }
        Salsa20::Decryption dec;
        dec.SetKeyWithIV(key1, 16, iv1, 8);
        dec.ProcessData(dst_pos, cur, size);
        cur += size;
        dst_pos += size;
        processed += size;
    }
    return dst;
}

std::pair<std::shared_ptr<unsigned char[]>, size_t> CryptoMaster::Decrypt(std::string filename)
{
    using namespace CryptoPP;
    unsigned char key1[16];
    unsigned char iv1[8];
    Salsa20::Encryption enc0;
    enc0.SetKeyWithIV(key0, 16, iv0, 8);
    enc0.ProcessData(key1, key0, 16);
    enc0.ProcessData(iv1, iv0, 8);
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs) {
        Log::OutputCritical("unexist .ftm file");
        return std::make_pair<std::shared_ptr<unsigned char[]>, size_t>(nullptr, 0);
    }
    ifs.seekg(0, std::ios_base::end);
    size_t filesize = ifs.tellg();
    ifs.seekg(0, std::ios_base::beg);
    std::shared_ptr<unsigned char[]> dst = std::make_shared<unsigned char[]>(filesize);
    //process_section_sizeÉoÉCÉgÇ≤Ç∆Ç…ÇøÇ‹ÇøÇ‹ì«Ç›çûÇﬁ
    std::shared_ptr<unsigned char[]> src = std::make_shared<unsigned char[]>(process_section_size);
    size_t processed = 0;
    unsigned char* dst_pos = dst.get();
    while (processed < filesize) {
        size_t size = min(process_section_size, filesize - processed);
        ifs.read((char*)src.get(), size);
        Salsa20::Decryption dec;
        dec.SetKeyWithIV(key1, 16, iv1, 8);
        dec.ProcessData(dst_pos, src.get(), size);
        dst_pos += size;
        processed += size;
    }
    return std::pair<std::shared_ptr<unsigned char[]>, size_t>(dst, filesize);
}

std::shared_ptr<unsigned char[]> CryptoMaster::Encrypt(std::shared_ptr<unsigned char[]> src, size_t length)
{
    using namespace CryptoPP;
    unsigned char key1[16];
    unsigned char iv1[8];
    Salsa20::Encryption enc0;
    enc0.SetKeyWithIV(key0, 16, iv0, 8);
    enc0.ProcessData(key1, key0, 16);
    enc0.ProcessData(iv1, iv0, 8);
    std::shared_ptr<unsigned char[]> dst = std::make_shared<unsigned char[]>(length);
    auto dst_pos = dst.get();
    auto cur = src.get();
    size_t processed = 0;
    while (processed < length) {
        size_t size = length - processed;
        if (size > process_section_size) {
            size = process_section_size;
        }
        Salsa20::Encryption enc1;
        enc1.SetKeyWithIV(key1, 16, iv1, 8);
        enc1.ProcessData(dst_pos, cur, size);
        cur += size;
        dst_pos += size;
        processed += size;
    }
    return dst;
}

void CryptoMaster::Encrypt(std::shared_ptr<unsigned char[]> src, size_t length, std::string filename)
{
    using namespace CryptoPP;
    unsigned char key1[16];
    unsigned char iv1[8];
    Salsa20::Encryption enc0;
    enc0.SetKeyWithIV(key0, 16, iv0, 8);
    enc0.ProcessData(key1, key0, 16);
    enc0.ProcessData(iv1, iv0, 8);
    std::ofstream ofs;
    ofs.open(filename, std::ios::binary | std::ios::trunc);
    std::shared_ptr<unsigned char[]> dst = std::make_shared<unsigned char[]>(process_section_size);
    auto src_pos = src.get();
    size_t processed = 0;
    while (processed < length) {
        size_t size = min(process_section_size, length - processed);
        Salsa20::Encryption enc1;
        enc1.SetKeyWithIV(key1, 16, iv1, 8);
        enc1.ProcessData(dst.get(), src_pos, size);
        ofs.write((char*)dst.get(), size);
        src_pos += size;
        processed += size;
    }
    ofs.close();
}
