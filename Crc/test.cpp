#include <Crc/CrcAlgorithm.h>

int main()
{
    uint8_t data[] = { 0x7E,0x10,0x00,0x08,0x00,0x7B };
    int length = 6;
    CRC::CRCModel model = CRC::CRC_8;
    int bitsWidth = getBitsWidth(model);
    uint8_t crc8;
    uint16_t crc16;
    uint32_t crc32;
    char buf[32];
    switch (bitsWidth) {
    case 8:
        crc8 = CRC::crcCalculate<uint8_t>(data, length, model);
        snprintf(buf, sizeof buf, "%02x", crc8);
        printf("CRC8: %s\n", buf);
        break;
    case 16:
        crc16 = CRC::crcCalculate<uint16_t>(data, length, model);
        snprintf(buf, sizeof buf, "%02x %02x", crc16 >> 8, crc16 & 0xff);
        printf("CRC16: %s\n", buf);
        break;
    case 32:
        crc32 = CRC::crcCalculate<uint32_t>(data, length, model);
        snprintf(buf, sizeof buf, "%02x %02x %02x %02x", crc32 >> 24,
                 (crc32 >> 16) & 0xff, (crc32 >> 8) & 0xff, crc32 & 0xff);
        printf("CRC32: %s\n", buf);
        break;
    default:
        break;
    }
    return 0;
}
