#ifndef __GO_LANG_H__
#define __GO_LANG_H__

#include "lang.h"
#include "go_printer.h"
#include "define_tree.h"
#include "struct_tree.h"
#include "struct_item_tree.h"
#include "type_tree.h"
#include "union_tree.h"
#include "union_ptr_tree.h"
#include "message_tree.h"
#include "include_tree.h"

class GoLang : public Language {
public:
    GoLang();
    void option(const char *name, ptr<Token> value) override;
    void print(SymbolTable &symbols, FILE *file) override;

protected:
    void print_define(GoPrinter &printer, ptr<DefineTree> tree);
    void print_struct(GoPrinter &printer, ptr<StructTree> tree, bool is_req = true);
    void print_message(GoPrinter &printer, ptr<MessageTree> tree);
    void print_include(GoPrinter &printer, ptr<IncludeTree> tree);

    void print_dump(GoPrinter &printer, ptr<StructTree> tree);
    void print_serial(GoPrinter &printer, ptr<StructTree> tree, bool is_req = true);
    void print_unserial(GoPrinter &printer, ptr<StructTree> tree, bool is_req = true);
    void print_message_id(GoPrinter &printer, ptr<StructTree> tree);

    void print_indent(GoPrinter &printer);

    void print_var(GoPrinter &printer, ptr<StructItemTree> tree, bool union_item);
    const char *type_decl(ptr<TypeTree> tree, bool union_item);
    const char *type_decl(ptr<StructItemTree> tree, bool union_item);

    void print_base_var_serial(GoPrinter &printer, ptr<StructItemTree> tree, const char *name);
    void print_base_var_unserial(GoPrinter &printer, ptr<StructItemTree> tree, const char *name);
    void print_base_var_dump(GoPrinter &printer, ptr<StructItemTree> tree, const char *name);

    void print_struct_serial(GoPrinter &printer, ptr<StructItemTree> tree, const char *name, bool is_pointer);
    void print_struct_unserial(GoPrinter &printer, ptr<StructItemTree> tree, const char *name, bool is_pointer);
    void print_struct_dump(GoPrinter &printer, ptr<StructItemTree> tree, const char *name, bool is_pointer);

    void print_array_serial(GoPrinter &printer, ptr<StructItemTree> tree, const char *name);
    void print_array_unserial(GoPrinter &printer, ptr<StructItemTree> tree, const char *name);
    void print_array_dump(GoPrinter &printer, ptr<StructItemTree> tree, const char *name);

    void print_var_serial(GoPrinter &printer, ptr<StructItemTree> tree, const char *prefix, bool is_pointer);
    void print_var_unserial(GoPrinter &printer, ptr<StructItemTree> tree, const char *prefix, bool is_pointer);
    void print_var_dump(GoPrinter &printer, ptr<StructItemTree> tree, const char *prefix, bool is_pointer);

    void print_constructor(GoPrinter &printer, ptr<StructTree> tree);
protected:
    const char *_serial_name;
    const char *_unserial_name;
};

#endif



