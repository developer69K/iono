/*
  IonoEQ.cpp - Arduino library for Iono MKR's seismic sensor

    Copyright (C) 2018 Sfera Labs S.r.l. - All rights reserved.

    For information, see the iono web site:
    http://www.sferalabs.cc/iono-arduino

  This code is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  See file LICENSE.txt for further informations on licensing terms.
*/

#include "IonoEQ.h"

void IonoEQClass::begin() {
  Wire.begin();
  Wire.setClock(400000);
}

void IonoEQClass::attachShutoffISR(void (*ISR)(void)) {
  attachInterrupt(digitalPinToInterrupt(9), ISR, FALLING);
}

void IonoEQClass::detachShutoffISR() {
  detachInterrupt(digitalPinToInterrupt(9));
}

void IonoEQClass::attachProcessingISR(void (*ISR)(void)) {
  attachInterrupt(digitalPinToInterrupt(8), ISR, FALLING);
}

void IonoEQClass::detachProcessingISR() {
  detachInterrupt(digitalPinToInterrupt(8));
}

int32_t IonoEQClass::readD7S(byte addrH, byte addrL, int n) {
  Wire.beginTransmission(0x55);
  Wire.write(addrH);
  Wire.write(addrL);
  Wire.endTransmission(false);

  Wire.requestFrom(0x55, n, false);

  long val = 0;
  for (int i = 0; i < n; i++) {
    val <<= (i * 8);
    if (!Wire.available()) {
      return -1l;
    }
    val |= Wire.read();
  }

  return val;
}

byte IonoEQClass::writeD7S(byte addrH, byte addrL, byte data) {
  byte ret = 0;
  Wire.beginTransmission(0x55);
  ret += Wire.write(addrH);
  ret += Wire.write(addrL);
  ret += Wire.write(data);
  Wire.endTransmission(true);
  return ret;
}

int32_t IonoEQClass::adjust(int32_t val) {
  if (val < 0) {
    return -2147483648l;
  }

  return (int16_t) val;
}

// Settings

int16_t IonoEQClass::readState() {
  int32_t v = readD7S(0x10, 0x00, 1);
  if (v < 0) {
    return v;
  }
  return v & 0b111;
}

int16_t IonoEQClass::readAxisState() {
  int32_t v = readD7S(0x10, 0x01, 1);
  if (v < 0) {
    return v;
  }
  return v & 0b11;
}

int16_t IonoEQClass::readEvent() {
  int32_t v = readD7S(0x10, 0x02, 1);
  if (v < 0) {
    return v;
  }
  return v & 0b1111;
}

int16_t IonoEQClass::readMode() {
  int32_t v = readD7S(0x10, 0x03, 1);
  if (v < 0) {
    return v;
  }
  return v & 0b111;
}

boolean IonoEQClass::writeMode(byte data) {
  return writeD7S(0x10, 0x03, data & 0b111) == 3;
}

int16_t IonoEQClass::readCtrl() {
  int32_t v = readD7S(0x10, 0x04, 1);
  if (v < 0) {
    return v;
  }
  return (v >> 3) & 0b1111;
}

boolean IonoEQClass::writeCtrl(byte data) {
  return writeD7S(0x10, 0x04, (data & 0b1111) << 3) == 3;
}

int16_t IonoEQClass::readClearCommand() {
  int32_t v = readD7S(0x10, 0x05, 1);
  if (v < 0) {
    return v;
  }
  return v & 0b1111;
}

boolean IonoEQClass::writeClearCommand(byte data) {
  return writeD7S(0x10, 0x05, data & 0b1111) == 3;
}

// Current

int32_t IonoEQClass::readCurrentSI() {
  return readD7S(0x20, 0x00, 2);
}

int32_t IonoEQClass::readCurrentPGA() {
  return readD7S(0x20, 0x02, 2);
}

// Latest

int32_t IonoEQClass::readLatestOffsetX(int n) {
  return adjust(readD7S(0x30 + n - 1, 0x00, 2));
}

int32_t IonoEQClass::readLatestOffsetY(int n) {
  return adjust(readD7S(0x30 + n - 1, 0x02, 2));
}

int32_t IonoEQClass::readLatestOffsetZ(int n) {
  return adjust(readD7S(0x30 + n - 1, 0x04, 2));
}

int32_t IonoEQClass::readLatestTemp(int n) {
  return adjust(readD7S(0x30 + n - 1, 0x06, 2));
}

int32_t IonoEQClass::readLatestSI(int n) {
  return readD7S(0x30 + n - 1, 0x08, 2);
}

int32_t IonoEQClass::readLatestPGA(int n) {
  return readD7S(0x30 + n - 1, 0x0A, 2);
}

// SI Ranked

int32_t IonoEQClass::readRankedOffsetX(int n) {
  return adjust(readD7S(0x35 + n - 1, 0x00, 2));
}

int32_t IonoEQClass::readRankedOffsetY(int n) {
  return adjust(readD7S(0x35 + n - 1, 0x02, 2));
}

int32_t IonoEQClass::readRankedOffsetZ(int n) {
  return adjust(readD7S(0x35 + n - 1, 0x04, 2));
}

int32_t IonoEQClass::readRankedTemp(int n) {
  return adjust(readD7S(0x35 + n - 1, 0x06, 2));
}

int32_t IonoEQClass::readRankedSI(int n) {
  return readD7S(0x35 + n - 1, 0x08, 2);
}

int32_t IonoEQClass::readRankedPGA(int n) {
  return readD7S(0x35 + n - 1, 0x0A, 2);
}

IonoEQClass IonoEQ;
