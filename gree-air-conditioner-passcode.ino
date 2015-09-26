#include <IRremote.h>

#define BITS 32
#define BIT_MARK 650
#define ONE_SPACE 1650
#define ZERO_SPACE 550

// unsigned int raw_codes_on[] = {9000, 4500, 650, 550, 650, 550, 650, 550, 650, 1650, 650, 550, 650, 550, 650, 550, 650, 550, 650, 1650, 650, 550, 650, 550, 650, 1650, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 1650, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 1650, 650, 550, 650, 1650, 650, 550, 650, 550, 650, 1650, 650, 550, 650, 20000, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 1650, 650, 1650, 650, 550, 650, 1650, 650, 40000, 9000, 4500, 650, 550, 650, 550, 650, 550, 650, 1650, 650, 550, 650, 550, 650, 550, 650, 550, 650, 1650, 650, 550, 650, 550, 650, 1650, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 1650, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 1650, 650, 1650, 650, 1650, 650, 550, 650, 550, 650, 1650, 650, 550, 650, 20000, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 550, 650, 1650, 650, 1650, 650, 550, 650, 1650, 650};
byte reverse_table[] = {0x00, 0x08, 0x04, 0x0C, 0x02, 0x0A, 0x06, 0x0E, 0x01, 0x09, 0x05, 0x0D, 0x03, 0x0B, 0x07, 0x0F};
unsigned int i = 0;
// 1 data pair as initial data, 36 data pairs as fixed prefix, 1 data as padding, and 32 data pairs as code itself
byte offset;

IRsend irsend;

void setup()
{
  Serial.begin(9600);
}

void loop() {
  unsigned int raw_codes[2+BITS*2+4*2+1+BITS*2] = {9000, 4500, BIT_MARK, ZERO_SPACE, BIT_MARK, ZERO_SPACE, BIT_MARK, ZERO_SPACE, BIT_MARK, ZERO_SPACE, BIT_MARK, ZERO_SPACE, BIT_MARK, ZERO_SPACE, BIT_MARK, ZERO_SPACE, BIT_MARK, ZERO_SPACE, BIT_MARK, ZERO_SPACE, BIT_MARK, ZERO_SPACE, BIT_MARK, ZERO_SPACE, BIT_MARK, ZERO_SPACE, BIT_MARK, ZERO_SPACE, BIT_MARK, ZERO_SPACE, BIT_MARK, ZERO_SPACE, BIT_MARK, ZERO_SPACE, BIT_MARK, ZERO_SPACE, BIT_MARK, ZERO_SPACE, BIT_MARK, ZERO_SPACE, BIT_MARK, ZERO_SPACE, BIT_MARK, ZERO_SPACE, BIT_MARK, ZERO_SPACE, BIT_MARK, ZERO_SPACE, BIT_MARK, ZERO_SPACE, BIT_MARK, ZERO_SPACE, BIT_MARK, ZERO_SPACE, BIT_MARK, ZERO_SPACE, BIT_MARK, ZERO_SPACE, BIT_MARK, ONE_SPACE, BIT_MARK, ONE_SPACE, BIT_MARK, ONE_SPACE, BIT_MARK, ONE_SPACE, BIT_MARK, ONE_SPACE, BIT_MARK, ONE_SPACE, BIT_MARK, ZERO_SPACE, BIT_MARK, 20000, BIT_MARK};
  offset = 2+BITS*2+4*2+1;
  unsigned int n = i;
  byte number[6] = {0};
  byte pos = 0;
  do {
    number[pos] = n % 10;
    n /= 10;
    number[pos] = reverse_table[number[pos]];
    pos++;
  } while (n > 0);
  byte data[4] = {0};
  data[0] = number[0];
  data[1] = (number[1] << 4) | number[2];
  data[2] = (number[3] << 4) | number[4];
  data[3] = (number[5] << 4) | (reverse_table[0x0F & (B00001011 + reverse_table[number[0]] + reverse_table[number[2]] + reverse_table[number[4]])]);
  for (byte index = 0; index < 4; index++) {
    byte mask = 0x80;
    while (mask) {
      if (data[index] & mask) {
        raw_codes[offset++] = ONE_SPACE;
      } else {
        raw_codes[offset++] = ZERO_SPACE;
      }
      raw_codes[offset++] = BIT_MARK;
      mask >>= 1;
    }
  }
  irsend.sendRaw(raw_codes, sizeof(raw_codes)/sizeof(unsigned int), 38);
  i++;
  delay(15000);
}
