#ifndef syms_SYMBOL_STORE_H
#define syms_SYMBOL_STORE_H

typedef struct syms_SymbolStore syms_SymbolStore;
typedef unsigned long int syms_SymbolID;

syms_SymbolStore *syms_new(void);
void syms_delete(syms_SymbolStore *symtbl);
const syms_SymbolID *syms_put(syms_SymbolStore *symtbl, const char *symbol);
const char *syms_lookup(const syms_SymbolStore *symtbl, syms_SymbolID id);

#endif
