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
  byte data[4] = {0};
  data[0] = reverse_table[byte(i % 10)];
  data[1] = (reverse_table[byte((i / 10) % 10)] << 4) | (reverse_table[byte((i / 100) % 10)]);
  data[2] = (reverse_table[byte((i / 1000) % 10)] << 4) | (reverse_table[byte((i / 10000) % 10)]);
  data[3] = (reverse_table[byte((i / 100000) % 10)] << 4) | (reverse_table[0x0F & (B00001011 + byte(i % 16))]);
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
