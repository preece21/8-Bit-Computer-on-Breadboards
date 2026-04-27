// Flashing Lights Program
// This program loads different values into the a register then loops back to the beginning
// This creates the effect of a light looping through the a register

  DATA Ra #0x80
  DATA Ra #0x40
  DATA Ra #0x20
  DATA Ra #0x10
  DATA Ra #0x08
  DATA Ra #0x04
  DATA Ra #0x02
  DATA Ra #0x01
  JMP #0x00
