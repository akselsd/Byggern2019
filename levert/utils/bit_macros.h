#ifndef BIT_MACROS_H_INCLUDED
#define BIT_MACROS_H_INCLUDED

#define SET_BIT(reg, bit) (reg |= (1 << bit))
#define CLEAR_BIT(reg, bit) (reg &= ~(1 << bit))
#define READ_BIT(reg, bit) (reg & (1 << bit))

#endif