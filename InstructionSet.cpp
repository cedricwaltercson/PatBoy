#include "InstructionSet.h"

/* ALU INSTRUCTIONS */

void InstructionSet::increment16BitRegister(Register *reg) {
    reg->value++;
}

void InstructionSet::increment8BitRegister(byte *reg, byte *flags) {
    *reg = *reg + 1;
    
    clearFlag(ADD_SUB_FLAG, flags);
    
    if (*reg == 0x00) {
        raiseFlag(ZERO_FLAG, flags);
    } else {
        clearFlag(ZERO_FLAG, flags);
    }
    
    if (*reg & 0x0F) {
        clearFlag(HALF_CARRY_FLAG, flags);
    } else {
        raiseFlag(HALF_CARRY_FLAG, flags);
    }
}

void InstructionSet::decrement16BitRegister(Register *reg) {
    reg->value--;
}

void InstructionSet::decrement8BitRegister(byte *reg, byte *flags) {
    *reg = *reg -1;
    
    raiseFlag(ADD_SUB_FLAG, flags);
    if (*reg == 0x00) {
        raiseFlag(ZERO_FLAG, flags);
    } else {
        clearFlag(ZERO_FLAG, flags);
    }
    
    if ((*reg & 0x0F) == 0x0F) {
        raiseFlag(HALF_CARRY_FLAG, flags);
    } else {
        clearFlag(HALF_CARRY_FLAG, flags);
    }
}

void InstructionSet::add16BitRegister(Register *original, const Register adding, byte *flags) {
    
    clearFlag(ADD_SUB_FLAG, flags);
    
    if (((original->value & 0x0FFF) + (adding.value & 0x0FFF)) > 0x0FFF) {
        raiseFlag(HALF_CARRY_FLAG, flags);
    } else {
        clearFlag(HALF_CARRY_FLAG, flags);
    }
    
    if ((original->value + adding.value) > 0xFFFF) {
        raiseFlag(CARRY_FLAG, flags);
    } else {
        clearFlag(CARRY_FLAG, flags);
    }
    
    original->value = original->value + adding.value;
}

void InstructionSet::add8BitRegister(byte *accumulator, const byte adding, byte *flags) {
    if ((*accumulator + adding) & 0xFF) {
        clearFlag(ZERO_FLAG, flags);
    } else {
        raiseFlag(ZERO_FLAG, flags);
    }
    
    clearFlag(ADD_SUB_FLAG, flags);
    
    if (((*accumulator & 0x0F) + (adding & 0x0F)) > 0x0F) {
        raiseFlag(HALF_CARRY_FLAG, flags);
    } else {
        clearFlag(HALF_CARRY_FLAG, flags);
    }
    
    if((*accumulator + adding) > 0xFF) {
        raiseFlag(CARRY_FLAG, flags);
    } else {
        clearFlag(CARRY_FLAG, flags);
    }
    
    *accumulator = *accumulator + adding;
}

void InstructionSet::adc8BitRegister(byte *accumulator, const byte adding, byte *flags) {

    byte carryValue = getBitValue(*flags, CARRY_FLAG);
    word result = *accumulator + carryValue + adding;
    
    clearFlag(ADD_SUB_FLAG, flags);
    if ((result & 0xFF) == 0x00) {
        raiseFlag(ZERO_FLAG, flags);
    } else {
        clearFlag(ZERO_FLAG, flags);
    }
    
    if ((carryValue + (adding & 0x0F) + (*accumulator & 0x0F)) > 0x0F) {
        raiseFlag(HALF_CARRY_FLAG, flags);
    } else {
        clearFlag(HALF_CARRY_FLAG, flags);
    }
    
    if (result > 0xFF) {
        raiseFlag(CARRY_FLAG, flags);
    } else {
        clearFlag(CARRY_FLAG, flags);
    }
    
    *accumulator = result & 0xFF;
}

void InstructionSet::sub8BitRegister(byte *accumulator, const byte substract, byte *flags) {
    
    raiseFlag(ADD_SUB_FLAG, flags);
    if ((*accumulator - substract) == 0x00) {
        raiseFlag(ZERO_FLAG, flags);
    } else {
        clearFlag(ZERO_FLAG, flags);
    }
    
    if ((*accumulator & 0x0F) < (substract & 0x0F)) {
        raiseFlag(HALF_CARRY_FLAG, flags);
    } else {
        clearFlag(HALF_CARRY_FLAG, flags);
    }
    
    if (*accumulator < substract) {
        raiseFlag(CARRY_FLAG, flags);
    } else {
        clearFlag(CARRY_FLAG, flags);
    }

    *accumulator = *accumulator - substract;
}

void InstructionSet::sbc8BitRegister(byte *accumulator, const byte substract, byte * flags) {
    byte carry = getBitValue(*flags, CARRY_FLAG);
    
    raiseFlag(ADD_SUB_FLAG, flags);
    if ((*accumulator - substract - carry) == 0x00) {
        raiseFlag(ZERO_FLAG, flags);
    } else {
        clearFlag(ZERO_FLAG, flags);
    }
    
    if ((*accumulator & 0x0F) < ((substract + carry) & 0x0F)) {
        raiseFlag(HALF_CARRY_FLAG, flags);
    } else {
        clearFlag(HALF_CARRY_FLAG, flags);
    }
    
    if (*accumulator < (substract + carry)) {
        raiseFlag(CARRY_FLAG, flags);
    } else {
        clearFlag(CARRY_FLAG, flags);
    }
    
    *accumulator = *accumulator - substract - carry;
}

/* COMMON INSTRUCTIONS */
void InstructionSet::raiseFlag(Flag flag, byte *reg) {
    switch ( flag ) {
        case ZERO_FLAG:
            setBit(reg, ZERO_FLAG);
            break;
        case ADD_SUB_FLAG:
            setBit(reg, ADD_SUB_FLAG);
            break;
        case HALF_CARRY_FLAG:
            setBit(reg, HALF_CARRY_FLAG);
            break;
        case CARRY_FLAG:
            setBit(reg, CARRY_FLAG);
            break;
            break;
    }
}

void InstructionSet::toggleFlag(Flag flag, byte *reg) {
    switch ( flag ) {
        case ZERO_FLAG:
            toggleBit(reg, ZERO_FLAG);
            break;
        case ADD_SUB_FLAG:
            toggleBit(reg, ADD_SUB_FLAG);
            break;
        case HALF_CARRY_FLAG:
            toggleBit(reg, HALF_CARRY_FLAG);
            break;
        case CARRY_FLAG:
            toggleBit(reg, CARRY_FLAG);
            break;
    }
}

void InstructionSet::clearFlag(Flag flag, byte *reg) {
    switch ( flag ) {
        case ZERO_FLAG:
            clearBit(reg, ZERO_FLAG);
            break;
        case ADD_SUB_FLAG:
            clearBit(reg, ADD_SUB_FLAG);
            break;
        case HALF_CARRY_FLAG:
            clearBit(reg, HALF_CARRY_FLAG);
            break;
        case CARRY_FLAG:
            clearBit(reg, CARRY_FLAG);
            break;
    }
}

bool InstructionSet::checkFlag(Flag flag, const byte reg) {
    bool result = false;
    switch ( flag ) {
        case ZERO_FLAG:
            result = isBitSet(reg, ZERO_FLAG);
            break;
        case ADD_SUB_FLAG:
            result = isBitSet(reg, ADD_SUB_FLAG);
            break;
        case HALF_CARRY_FLAG:
            result = isBitSet(reg, HALF_CARRY_FLAG);
            break;
        case CARRY_FLAG:
            result = isBitSet(reg, CARRY_FLAG);
            break;
    }
    return result;
}

void InstructionSet::clearFlags(byte *reg) {
    reg = 0;
}