const uint8_t addressPins[15] = {
    22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36
};

const uint8_t dataPins[8] = {
    40, 41, 42, 43, 44, 45, 46, 47
};

const uint8_t chipEnable = 37;
const uint8_t outputEnable = 38;
const uint8_t writeEnable = 39;

struct programInfo{
    uint16_t address;
    uint16_t length;
};
