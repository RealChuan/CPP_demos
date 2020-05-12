#ifndef CRCALGORITHM_H
#define CRCALGORITHM_H

#define enumtoCharArr(val) #val
#define enumtoStr(val) Setstr(#val)

#include <string>

namespace CRC {

std::string Setstr(const char *val);

// crc算法模型（crc参数模型）
enum CRCModel{ CRC_8, CRC_8_ITU, CRC_8_ROHC, CRC_8_MAXIM,

               CRC_16_IBM, CRC_16_MAXIM, CRC_16_USB,  CRC_16_MODBUS, CRC_16_CCITT,
               CRC_16_CCITT_FALSE, CRC_16_x25, CRC_16_XMODEM, CRC_16_DNP,

               CRC_32, CRC_32_MPEG2
             };

static const char *CRC_MODEL[] = { "CRC_8", "CRC_8_ITU", "CRC_8_ROHC", "CRC_8_MAXIM",

                                   "CRC_16_IBM", "CRC_16_MAXIM", "CRC_16_USB",
                                   "CRC_16_MODBUS", "CRC_16_CCITT", "CRC_16_CCITT_FALSE",
                                   "CRC_16_x25", "CRC_16_XMODEM", "CRC_16_DNP",

                                   "CRC_32", "CRC_32_MPEG2"
                                 };

static const int CRC_MODEL_SIZE = int(CRC_32_MPEG2) + 1;

/**
 * @brief reverseInt    -- 将一个整数按位反转
 * @param input         -- 输入输出数据
 */
template<typename T>
void reverseInt(T &input){
    int bitsWidth = sizeof (input) * 8;

    T result = 0;
    for (int i=0; i<bitsWidth; i++){
        result <<= 1;
        result |= ((input >> i) & 1); //任何数和0按位或都是它本身
    }
    input = result;
}

/**
 * @brief getPolyFormula    -- 获取多项式公式
 * @param model             -- crc参数模型
 * @return                  -- 多项式公式
 */
std::string getPolyFormula(CRCModel model);

/**
 * @brief getInitValue  -- 获取crc初始值
 * @param model         -- crc参数模型
 * @return              -- 指定参数模型的crc初始值(根据实际情况进项强制转换)
 */
uint32_t getInitValue(CRCModel model);

/**
 * @brief getPoly   -- 获取多项式
 * @param model     -- crc参数模型
 * @return          -- 执行的参数模型的多项式(根据实际情况进项强制转换)
 */
uint32_t getPoly(CRCModel model);

/**
 * @brief xorValue  -- 结果异或值
 * @param model     -- crc参数模型
 * @return          -- 结果异或值
 */
uint32_t getXorValue(CRCModel model);

/**
 * @brief getInputReversal  -- 判断输入是否需要翻转
 * @param model             -- crc参数模型
 * @return                  -- 输入数据需要翻转返回true，否则返回false
 */
uint32_t getInputReversal(CRCModel model);

/**
 * @brief getOutputReversal -- 判断输出是否需要翻转
 * @param model             -- crc参数模型
 * @return                  -- 输出数据需要翻转返回true，否则返回false
 */
bool getOutputReversal(CRCModel model);

/**
 * @brief getBitsWidth  -- 获取位宽
 * @param model         -- 参数模型
 * @return              -- 位宽
 */
int getBitsWidth(CRCModel model);

int CRC_Calculate(unsigned char *data, long long int lenght, int model_);

/**
 * @brief crcCalculate  -- crc计算
 * @param input         -- 输入数据
 * @param length        -- 输入数据长度
 * @param crc           -- crc计算结果
 * @param model         -- crc参数模型
 */
template<typename T>
T crcCalculate(uint8_t *input, uint64_t length, CRCModel model){
    T crcReg = static_cast<T>(getInitValue(model));
    T poly = static_cast<T>(getPoly(model));
    uint8_t byte = 0;

    T temp = 1;
    while (length--){
        byte = *(input++);

        if (getInputReversal(model)){
            reverseInt(byte);
        }

        crcReg ^= static_cast<T>((byte << 8*(sizeof (T)-1)));
        for(int i=0; i < 8; i++){
            if(crcReg & (temp << (sizeof (T)*8-1))){
                crcReg = static_cast<T>((crcReg << 1) ^ poly);
            }else {
                crcReg = static_cast<T>(crcReg << 1);
            }
        }
    }

    if (getOutputReversal(model)){
        reverseInt(crcReg);
    }

    T crc = (crcReg ^ static_cast<T>(getXorValue(model))) ;
    return crc;
}

}


#endif // CRCALGORITHM_H
