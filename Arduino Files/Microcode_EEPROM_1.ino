#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_EN 13

#define ROM_NO 1

/* signal word: bit position  23    22    21    20    19    18    17    16
*               meaning       BS3   BS2   BS1   BS0   _MRW  _RAW  _IRW  OTW
*                             15    14    13    12    11    10    09    08
*                             _AW   _BW   _CW   _DW   _PCW  PCI   _TSR  HLT
*                             07    06    05    04    03    02    01    00
*                             ALS3  ALS2  ALS1  ALS0  ALM   ALC   _ALW  _FW
*/

#define BS0   ((uint32_t)1 << 0)
#define BS1   ((uint32_t)1 << 1)
#define BS2   ((uint32_t)1 << 2)
#define BS3   ((uint32_t)1 << 3)
#define _TSR  ((uint32_t)1 << 4)
#define HLT   ((uint32_t)1 << 5)
#define _MRW  ((uint32_t)1 << 6)
#define _IRW  ((uint32_t)1 << 7)

#define _AW   ((uint32_t)1 << 8)
#define _BW   ((uint32_t)1 << 9)
#define _CW   ((uint32_t)1 << 10)
#define _DW   ((uint32_t)1 << 11)
#define _PCW  ((uint32_t)1 << 12)
#define PCI   ((uint32_t)1 << 13)
#define _RAW  ((uint32_t)1 << 14)
#define _OTW  ((uint32_t)1 << 15)

#define _FW  ((uint32_t)1 << 16)
#define _ALW ((uint32_t)1 << 17)
#define _ALC  ((uint32_t)1 << 18)
#define ALM  ((uint32_t)1 << 19)
#define ALS0 ((uint32_t)1 << 20)
#define ALS1 ((uint32_t)1 << 21)
#define ALS2 ((uint32_t)1 << 22)
#define ALS3 ((uint32_t)1 << 23)

#define NA0  ((uint32_t)1 << 24)
#define NA1  ((uint32_t)1 << 25)
#define NA2  ((uint32_t)1 << 26)
#define NA3  ((uint32_t)1 << 27)
#define NA4  ((uint32_t)1 << 28)
#define NA5  ((uint32_t)1 << 29)
#define NA6  ((uint32_t)1 << 30)
#define NA7  ((uint32_t)1 << 31)




#define AO  ( BS0 )
#define BO  ( BS1 )
#define CO  ( BS1 | BS0 )
#define DO  ( BS2 )
#define PCO ( BS2 | BS0 )
#define RAO ( BS2 | BS1 )
#define ROO ( BS2 | BS1 | BS0 )
#define ALO ( BS3 )

#define ADD ( ALS3 | ALS0)

#define ADC ( ALS3 | ALS0 | _ALC )
#define SUB ( ALS2 | ALS1 | _ALC )
#define AND ( ALS3 | ALM )
#define OR  ( ALS3 | ALS2 | ALS1 | ALM )
#define XOR ( ALS2 | ALS1 | ALM )
#define NOT ( ALS2 | ALS0 | ALM )
#define INC ( ALS3 | ALS2 | ALS1 | ALS0 | _ALC)
#define DEC ( ALS3 | ALS2 | ALS1 | ALS0 )

/* EEPROM address: bit position    12     11      10  ....  8     7  ....  0
*                  meaning      flags  carry    (microtime) T         opcode
*/

#define CARRY_ADDR (1 << 11)
#define FLAGS_ADDR (1 << 12)

#define ins_eeprom_address(opcode, carry, flags, T) (opcode | (carry ? CARRY_ADDR : 0) | (flags ? FLAGS_ADDR : 0) | (T << 8))



#define MOV 0b00
#define LOD 0b01
#define STO 0b10
#define ALU ob11

#define Ra  0b000
#define Rb  0b001
#define Rc  0b010
#define Rd  0b011
#define SP  0b100
#define PC  0b101
#define SPi 0b110
#define IMM 0b111


#define OPCODE(op, dreg, sreg) (((op) << 6) | ((dreg) << 3) | (sreg))


uint32_t inline flip_active_lows(uint32_t microcode_word) {
  // Flip all of the necessary bits for active low signals
  microcode_word ^= ( _FW | _ALW | _TSR | _PCW | _DW | _CW | _BW | _AW  | _IRW | _RAW | _MRW | _ALC | _OTW);
  return microcode_word;
}

uint32_t _E(uint32_t reg) {

  switch (reg) {
    case Ra: return AO;
    case Rb: return BO;
    case Rc: return CO;
    case Rd: return DO;
    case PC: return PCO;
    default: return 0;
  }
}

uint32_t _W(uint32_t reg) {

  switch (reg) {
    case Ra: return _AW;
    case Rb: return _BW;
    case Rc: return _CW;
    case Rd: return _DW;
    case PC: return _PCW;
    default: return 0;
  }
}


#define FETCH0 ( _MRW | PCO )
#define FETCH1 ( _IRW | ROO | PCI)

uint32_t microcode[8] = { FETCH0, FETCH1, _TSR, 0, 0, 0, 0, 0 };

uint32_t *MICROCODE0() {
  microcode[2] = _TSR;
  microcode[3] = microcode[4] = microcode[5] = microcode[6] = microcode[7] = 0;
  return microcode;
}


uint32_t *MICROCODE1(uint32_t c1) {
  microcode[2] = c1 | _TSR;
  microcode[3] = microcode[4] = microcode[5] = microcode[6] = microcode[7] = 0;
  return microcode;
}

uint32_t *MICROCODE2(uint32_t c1, uint32_t c2) {
  microcode[2] = c1;
  microcode[3] = c2 | _TSR;
  microcode[4] = microcode[5] = microcode[6] = microcode[7] = 0;
  return microcode;
}

uint32_t *MICROCODE3(uint32_t c1, uint32_t c2, uint32_t c3) {
  microcode[2] = c1;
  microcode[3] = c2;
  microcode[4] = c3 | _TSR;
  microcode[5] = microcode[6] = microcode[7] = 0;
  return microcode;
}

uint32_t *MICROCODE4(uint32_t c1, uint32_t c2, uint32_t c3, uint32_t c4) {
  microcode[2] = c1;
  microcode[3] = c2;
  microcode[4] = c3;
  microcode[5] = c4 | _TSR;
  microcode[6] = microcode[7] = 0;
  return microcode;
}

void write_conditional_instruction(uint16_t opcode, bool carry, bool flags, uint32_t microcode[], uint8_t rom_no) {
  for (uint8_t T =  0; T < 8; T++) {

    uint16_t eeprom_address = ins_eeprom_address(opcode, carry, flags, T);
    uint8_t eeprom_byte = (flip_active_lows(microcode[T]) >> (8*rom_no)) & 0xFF;
    writeEEPROM(eeprom_address, eeprom_byte);

  }
}

void write_carry_cond_instruction(uint16_t opcode, bool carry, uint32_t microcode[], uint8_t rom_no) {

  write_conditional_instruction(opcode, carry, false, microcode, rom_no);
  write_conditional_instruction(opcode, carry, true, microcode, rom_no);
}

void write_instruction(uint16_t opcode, uint32_t microcode[], uint8_t rom_no) {

  write_carry_cond_instruction(opcode, false, microcode, rom_no);
  write_carry_cond_instruction(opcode, true, microcode, rom_no);
}


void write_MOVS(uint8_t rom_no) {

  Serial.print("Writing reg <- reg MOV instructions ");
  for (uint8_t sreg = Ra; sreg <= PC; sreg++) {
    Serial.print(".");
    for (uint8_t dreg = Ra; dreg <= PC; dreg++) {
      if (sreg != dreg) {
        write_instruction(OPCODE(MOV, dreg, sreg), MICROCODE1(_W(dreg) | _E(sreg)), rom_no);
      }
    }
  }
  Serial.println("done.");

  Serial.print("Writing reg <- imm DATA instructions ");
  for (uint8_t dreg = Ra; dreg <= PC; dreg++) {
    write_instruction(OPCODE(MOV, dreg, IMM), MICROCODE2( (PCO | _MRW | PCI), (ROO | _W(dreg))), rom_no);
  }
  Serial.println("done.");

  Serial.print("Writing NOP and HLT instructions ");
  write_instruction(OPCODE(MOV, Ra, Ra), MICROCODE0(), rom_no); // NOP
  write_instruction(OPCODE(MOV, PC, PC), MICROCODE1(HLT), rom_no); // HLT
  Serial.println("done.");

  Serial.print("Writing conditional JC, JZ, JO, and JN instructions ");
  write_carry_cond_instruction(OPCODE(MOV, IMM, 0b000), false, MICROCODE1(PCI), rom_no);
  write_carry_cond_instruction(OPCODE(MOV, IMM, 0b000), true, MICROCODE2(( _MRW | PCO | PCI ), ( ROO | _PCW )), rom_no); 

  for (uint8_t flag = 0; flag <= 2; flag++) {
    write_conditional_instruction(OPCODE(MOV, IMM, (1 <<
     flag)), false, false, MICROCODE1(PCI), rom_no);
    write_conditional_instruction(OPCODE(MOV, IMM, (1 << flag)), true, false, MICROCODE1(PCI), rom_no);

    write_conditional_instruction(OPCODE(MOV, IMM, (1 << flag)), false, true, MICROCODE2(( _MRW | PCO | PCI ), ( ROO | _PCW )), rom_no);
    write_conditional_instruction(OPCODE(MOV, IMM, (1 << flag)), true, true, MICROCODE2(( _MRW | PCO | PCI ), ( ROO | _PCW )), rom_no);
  }
  Serial.println("done.");
  Serial.println("42 Move instructions written.");
}

void write_LODS(uint8_t rom_no) {

  Serial.print("Writing reg <- [reg] LOD instructions ");
  for (uint8_t sreg = Ra; sreg <= PC; sreg++) {
    Serial.print(".");

    // [Rc] Loads from program ROM, everything else loads from RAM
    for (uint8_t dreg = Ra; dreg <= PC; dreg++) {
      write_instruction(OPCODE(LOD, dreg, sreg), MICROCODE2( _MRW | _E(sreg), ( ((sreg == Rc) ? ROO : RAO) | _W(dreg))), rom_no); 
    }
  }
  Serial.println("done.");

  Serial.print("Writing reg <- [IMM] LOD instructions ");
  for (uint8_t dreg = Ra; dreg <= PC; dreg++){
    write_instruction(OPCODE(LOD, dreg, IMM), MICROCODE3(( PCO | _MRW | PCI ), ( ROO | _MRW ), ( RAO | _W(dreg))), rom_no);
  }
  Serial.println("done.");

  Serial.print("Writing output <- reg instructions ");
  write_instruction(OPCODE(LOD, IMM, 0b000), MICROCODE1( AO | _OTW ), rom_no);
  write_instruction(OPCODE(LOD, IMM, 0b001), MICROCODE1( BO | _OTW ), rom_no);
  write_instruction(OPCODE(LOD, IMM, 0b010), MICROCODE1( CO | _OTW ), rom_no);
  write_instruction(OPCODE(LOD, IMM, 0b011), MICROCODE1( DO | _OTW ), rom_no);
  Serial.println("done.");
}

void write_STOS(uint8_t rom_no) {

  Serial.print("Writing [reg] <- reg STO instructions ");
  for (uint8_t sreg = Ra; sreg <= PC; sreg++) {
    Serial.print(".");

    for (uint8_t dreg = Ra; dreg <= PC; dreg++) {

      write_instruction(OPCODE(STO, dreg, sreg), MICROCODE2( ( _MRW | _E(dreg)), ( _RAW | _E(sreg))), rom_no);
    }
  }
  Serial.println("done.");

  Serial.print("Writing [IMM] <- reg STO instructions ");
  for (uint8_t sreg = Ra; sreg <= PC; sreg++) {
    
    write_instruction(OPCODE(STO, IMM, sreg), MICROCODE3(( _MRW | PCO | PCI), ( ROO | _MRW ), ( RAO | _E(sreg ))), rom_no);
  }
  Serial.println("done.");
}

void write_ALUS(uint8_t rom_no) {

  Serial.print("Writing ALU ADD instructions ");
  write_instruction(0b11000000, MICROCODE2(( AO | ADD | _ALW | _FW ), ( _AW | ALO)), rom_no);
  write_instruction(0b11000001, MICROCODE2(( BO | ADD | _ALW | _FW ), ( _AW | ALO)), rom_no);
  write_instruction(0b11000010, MICROCODE2(( CO | ADD | _ALW | _FW ), ( _AW | ALO)), rom_no);
  write_instruction(0b11000011, MICROCODE2(( DO | ADD | _ALW | _FW ), ( _AW | ALO)), rom_no);
  Serial.println("done.");

  Serial.print("Writing ALU ADC instructions ");
  write_instruction(0b11000100, MICROCODE2(( AO | ADC | _ALW | _FW ), ( _AW | ALO)), rom_no);
  write_instruction(0b11000101, MICROCODE2(( BO | ADC | _ALW | _FW ), ( _AW | ALO)), rom_no);
  write_instruction(0b11000110, MICROCODE2(( CO | ADC | _ALW | _FW ), ( _AW | ALO)), rom_no);
  write_instruction(0b11000111, MICROCODE2(( DO | ADC | _ALW | _FW ), ( _AW | ALO)), rom_no);
  Serial.println("done.");

  Serial.print("Writing ALU SUB instructions ");
  write_instruction(0b11001000, MICROCODE2(( AO | SUB | _ALW | _FW ), ( _AW | ALO)), rom_no);
  write_instruction(0b11001001, MICROCODE2(( BO | SUB | _ALW | _FW ), ( _AW | ALO)), rom_no);
  write_instruction(0b11001010, MICROCODE2(( CO | SUB | _ALW | _FW ), ( _AW | ALO)), rom_no);
  write_instruction(0b11001011, MICROCODE2(( DO | SUB | _ALW | _FW ), ( _AW | ALO)), rom_no);
  Serial.println("done.");

  Serial.print("Writing ALU CMP instructions ");
  write_instruction(0b11001100, MICROCODE1( AO | SUB | _ALW | _FW ), rom_no);
  write_instruction(0b11001101, MICROCODE1( BO | SUB | _ALW | _FW ), rom_no);
  write_instruction(0b11001110, MICROCODE1( CO | SUB | _ALW | _FW ), rom_no);
  write_instruction(0b11001111, MICROCODE1( DO | SUB | _ALW | _FW ), rom_no);
  Serial.println("done.");

  Serial.print("Writing ALU AND instructions ");
  write_instruction(0b11010000, MICROCODE2(( AO | AND | _ALW | _FW ), ( _AW | ALO)), rom_no);
  write_instruction(0b11010001, MICROCODE2(( BO | AND | _ALW | _FW ), ( _AW | ALO)), rom_no);
  write_instruction(0b11010010, MICROCODE2(( CO | AND | _ALW | _FW ), ( _AW | ALO)), rom_no);
  write_instruction(0b11010011, MICROCODE2(( DO | AND | _ALW | _FW ), ( _AW | ALO)), rom_no);
  Serial.println("done.");

  Serial.print("Writing ALU OR instructions ");
  write_instruction(0b11010100, MICROCODE2(( AO | OR | _ALW | _FW ), ( _AW | ALO)), rom_no);
  write_instruction(0b11010101, MICROCODE2(( BO | OR | _ALW | _FW ), ( _AW | ALO)), rom_no);
  write_instruction(0b11010110, MICROCODE2(( CO | OR | _ALW | _FW ), ( _AW | ALO)), rom_no);
  write_instruction(0b11010111, MICROCODE2(( DO | OR | _ALW | _FW ), ( _AW | ALO)), rom_no);
  Serial.println("done.");

  Serial.print("Writing ALU XOR instructions ");
  write_instruction(0b11011000, MICROCODE2(( AO | XOR | _ALW | _FW ), ( _AW | ALO)), rom_no);
  write_instruction(0b11011001, MICROCODE2(( BO | XOR | _ALW | _FW ), ( _AW | ALO)), rom_no);
  write_instruction(0b11011010, MICROCODE2(( CO | XOR | _ALW | _FW ), ( _AW | ALO)), rom_no);
  write_instruction(0b11011011, MICROCODE2(( DO | XOR | _ALW | _FW ), ( _AW | ALO)), rom_no);
  Serial.println("done.");

  Serial.print("Writing ALU NOT instructions ");
  write_instruction(0b11011100, MICROCODE2(( AO | NOT | _ALW | _FW ), ( _AW | ALO)), rom_no);
  Serial.println("done.");

  Serial.print("Writing ALU INC instructions ");
  write_instruction(0b11011101, MICROCODE2(( AO | INC | _ALW | _FW ), ( _AW | ALO)), rom_no);
  Serial.println("done.");

  Serial.print("Writing ALU DEC instructions ");
  write_instruction(0b11011110, MICROCODE2(( AO | DEC | _ALW | _FW ), ( _AW | ALO)), rom_no);
  Serial.println("done.");

}

void write_full_EEPROM(uint8_t rom_no) {
  Serial.print("Writing HLT to every opcode ");
  uint8_t OPCODE = 0;
  do {
    write_instruction(OPCODE, MICROCODE1(HLT), rom_no);
    Serial.println(OPCODE);
  }   while (++OPCODE != 0);
  Serial.println("done");
}

/*
 * Output the address bits and outputEnable signal using shift registers.
 */
void setAddress(int address, bool outputEnable) {
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 8) | (outputEnable ? 0x00 : 0x80));
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);

  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}

/*
 * Read a byte from the EEPROM at the specified address.
 */
byte readEEPROM(int address) {
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, INPUT);
  }
  setAddress(address, /*outputEnable*/ true);

  byte data = 0;
  for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin -= 1) {
    data = (data << 1) + digitalRead(pin);
  }
  return data;
}

/*
 * Write a byte to the EEPROM at the specified address.
 */
void writeEEPROM(int address, uint8_t data) {
  setAddress(address, /*outputEnable*/ false);
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, OUTPUT);
  }

  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    digitalWrite(pin, data & 1);
    data = data >> 1;
  }
  digitalWrite(WRITE_EN, LOW);
  delayMicroseconds(1);
  digitalWrite(WRITE_EN, HIGH);
  delay(10);
}


/*
 * Read the contents of the EEPROM and print them to the serial monitor.
 */
void printContents() {
  for (int base = 0; base <= 255; base += 16) {
    byte data[16];
    for (int offset = 0; offset <= 15; offset += 1) {
      data[offset] = readEEPROM(base + offset);
    }

    char buf[80];
    sprintf(buf, "%03x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    Serial.println(buf);
  }
}


void setup() {
  // put your setup code here, to run once:
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  digitalWrite(WRITE_EN, HIGH);
  pinMode(WRITE_EN, OUTPUT);
  
  Serial.begin(9600);
  Serial.print("About to program microcode EEPROM no. ");
  Serial.print(ROM_NO);
  Serial.print(". Press 'y' to continue ... ");

  while (Serial.available() <= 0);
  char answer = Serial.read();
  Serial.println(answer);

  if (answer == 'y') {
    //write_full_EEPROM(ROM_NO);
    //write_MOVS(ROM_NO);
    //write_LODS(ROM_NO);
    //write_STOS(ROM_NO);
    write_ALUS(ROM_NO);

    Serial.println("Programming Finished");
  }
  


}


void loop() {
  // put your main code here, to run repeatedly:

}
