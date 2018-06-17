#include "collections/symbol_store.h"
#include <collections.h>
#include <stdlib.h>
#include <string.h>

struct syms_SymbolStore {
	hmap_HashMap *str2id_map;
	hmap_HashMap *id2str_map;
	syms_SymbolID id;

	syms_SymbolID ret_id;
};

/*
 * syms_SymbolStoreオブジェクトを生成する。
 */
syms_SymbolStore *syms_new(void)
{
	syms_SymbolStore *syms = NULL;
	hmap_HashMap *str2id_map = NULL;
	hmap_HashMap *id2str_map = NULL;

	syms = (syms_SymbolStore *) malloc(sizeof (syms_SymbolStore));
	if (syms == NULL) {
		goto FAILURE;
	}

	str2id_map = hmap_new(c_TYPE_STRING, c_TYPE_ULONG);
	if (str2id_map == NULL) {
		goto FAILURE;
	}
	id2str_map = hmap_new(c_TYPE_ULONG, c_TYPE_STRING);
	if (id2str_map == NULL) {
		goto FAILURE;
	}
	syms->str2id_map = str2id_map;
	syms->id2str_map = id2str_map;
	syms->id = 0;
	syms->ret_id = 0;

	return syms;

FAILURE:
	free(syms);
	syms = NULL;
	hmap_delete(str2id_map);
	str2id_map = NULL;
	hmap_delete(id2str_map);
	id2str_map = NULL;

	return NULL;
}

/*
 * 指定のsyms_SymbolStoreオブジェクトを破棄する。
 */
void syms_delete(syms_SymbolStore *syms)
{
	if (syms == NULL) {
		return;
	}

	// TODO syms_put_in_syms()内で生成したシンボルの複製の解放処理を実装する。

	hmap_delete(syms->str2id_map);
	syms->str2id_map = NULL;
	hmap_delete(syms->id2str_map);
	syms->id2str_map = NULL;
	free(syms);
}

/*
 * symbolをsymsへ登録する。
 * 
 * 戻り値は内部的に保持するsyms_SymbolIDへのポインタとなるため、次回呼び出し時には内容が書き換わる可能性がある。
 * よって、戻り値を受け取った呼び出し元はすぐにsyms_SymbolID型の変数へと値を退避させること。
 */
const syms_SymbolID *syms_put(syms_SymbolStore *syms, const char *symbol)
{
	const char *dup_sym;
	const syms_SymbolID *id;
    syms_SymbolID sym_id;

	if (syms == NULL || symbol == NULL) {
		return NULL;
	}

	id = (const syms_SymbolID *) hmap_lookup(syms->str2id_map, &symbol);
	if (id != NULL) {
		syms->ret_id = *id;

		return &syms->ret_id;
	}

	dup_sym = strdup(symbol);
	if (dup_sym == NULL) {
		return NULL;
	}

	if (hmap_put(syms->str2id_map, &dup_sym, &sym_id) == NULL) {
		return NULL;
	}
	if (hmap_put(syms->id2str_map, &sym_id, &dup_sym) == NULL) {
		return NULL;
	}
	
	syms->ret_id = sym_id;
    syms->id++;

	return &syms->ret_id;
}

/*
 * idに対応する記号の文字列表現を返す。
 */
const char *syms_lookup(const syms_SymbolStore *syms, syms_SymbolID id)
{
	void *sym;

	if (syms == NULL) {
		return NULL;
	}

	sym = hmap_lookup(syms->id2str_map, &id);
	if (sym == NULL) {
		return NULL;
	}

	return *((const char **) sym);
}
