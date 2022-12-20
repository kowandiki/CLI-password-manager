#pragma once
#include "..\aes\aes.hpp"
#include <iostream>
// generates 16 byte iv (initialization vector)
uint8_t* genIV(std::string str);
uint8_t* genIV(uint8_t* in, unsigned int size);

// generates 32 byte salt to be hashed with key
std::string genSalt(std::string str);
uint8_t* genSalt(uint8_t* in, unsigned int size);

uint8_t* genKey(std::string pass, std::string salt);
uint8_t* genKey(std::string pass, uint8_t* salt, unsigned int size);