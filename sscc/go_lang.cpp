#include <ctype.h>
#include "go_lang.h"

GoLang::GoLang() : Language("go") {
}

void GoLang::option(const char *name, ptr<Token> value) {

}

const char *GoLang::type_decl(ptr<TypeTree> tree, bool union_item) {
    switch (tree->type()) {
    case TYPE_INT8:
        return "int8";
    case TYPE_UINT8:
        return "uint8";
    case TYPE_INT16:
        return "int16";
    case TYPE_UINT16:
        return "uint16";
    case TYPE_INT32:
        return "int32";
    case TYPE_UINT32:
        return "uint32";
    case TYPE_INT64:
        return "int64";
    case TYPE_UINT64:
        return "uint64";
    case TYPE_FLOAT:
        return "float32";
    case TYPE_DOUBLE:
        return "float64";
    case TYPE_STRING:
        return "string";
    case TYPE_STRUCT:
        return tree->decl()->name()->text();
    default:
        assert(0);
        return nullptr;
    }
}

const char *GoLang::type_decl(ptr<StructItemTree> tree, bool union_item) {
    const char *type;
    if (tree->array()) {
        type = type_decl(tree->type(), false);
        type = Pool::instance()->printf("[]%s", type);
    }
    else {
        type = type_decl(tree->type(), union_item);
    }
    return type;
}

void GoLang::print_define(GoPrinter &printer, ptr<DefineTree> tree) {
    switch (tree->value()->exprType()) {
    case EXPR_INT:
        printer.const_(tree->name()->text(), "0x%llx", tree->value()->vint());
        break;
    case EXPR_STRING:
        printer.const_(tree->name()->text(), "\"%s\"", tree->value()->vstr());
        break;
    default:
        assert(0);
        break;
    }
}

void GoLang::print_var(GoPrinter &printer, ptr<StructItemTree> tree, bool union_item) {
    char tmp[256];
    char *name = tmp;
    strcpy(name, tree->name()->text());
    char x = toupper(name[0]);
    name[0] = x;
    printer.s("%s %s", name, type_decl(tree, union_item));
}

void GoLang::print_base_var_serial(GoPrinter &printer, ptr<StructItemTree> tree, const char *name) {
    const char *method;
    switch (tree->type()->type()) {
    case TYPE_INT8:
        method = "p.WriteInt8";
        break;
    case TYPE_UINT8:
        method = "p.WriteUint8";
        break;
    case TYPE_INT16:
        method = "p.WriteInt16";
        break;
    case TYPE_UINT16:
        method = "p.WriteUint16";
        break;
    case TYPE_INT32:
        method = "p.WriteInt32";
        break;
    case TYPE_UINT32:
        method = "p.WriteUint32";
        break;
    case TYPE_INT64:
        method = "p.WriteInt64";
        break;
    case TYPE_UINT64:
        method = "p.WriteUint64";
        break;
    case TYPE_FLOAT:
        method = "p.WriteFloat32";
        break;
    case TYPE_DOUBLE:
        method = "p.WriteFloat64";
        break;
    case TYPE_STRING:
        method = "p.WriteString";
        break;
    default:
        assert(0);
        return;
    }

    printer.s("%s(%s)", method, name);
}

void GoLang::print_base_var_unserial(GoPrinter &printer, ptr<StructItemTree> tree, const char *name) {
    const char *method;
    switch (tree->type()->type()) {
    case TYPE_INT8:
        method = "p.ReadInt8";
        break;
    case TYPE_UINT8:
        method = "p.ReadUint8";
        break;
    case TYPE_INT16:
        method = "p.ReadInt16";
        break;
    case TYPE_UINT16:
        method = "p.ReadUint16";
        break;
    case TYPE_INT32:
        method = "p.ReadInt32";
        break;
    case TYPE_UINT32:
        method = "p.ReadUint32";
        break;
    case TYPE_INT64:
        method = "p.ReadInt64";
        break;
    case TYPE_UINT64:
        method = "p.ReadUint64";
        break;
    case TYPE_FLOAT:
        method = "p.ReadFloat32";
        break;
    case TYPE_DOUBLE:
        method = "p.ReadFloat64";
        break;
    case TYPE_STRING:
        method = "p.ReadString";
        break;
    default:
        assert(0);
        return;
    }

    printer.s("%s = %s()", name, method);
}

void GoLang::print_base_var_dump(GoPrinter &printer, ptr<StructItemTree> tree, const char *name) {
    switch (tree->type()->type()) {
    case TYPE_INT8:
        printer.s("log.PrintF(\"%%d(0x%%x)\", (int)%s, (unsigned)%s)", name, name);
        break;
    case TYPE_UINT8:
        printer.s("log.PrintF(\"%%u(0x%%x)\", (unsigned)%s, (unsigned)%s)", name, name);
        break;
    case TYPE_INT16:
        printer.s("log.PrintF(\"%%d(0x%%x)\", (int)%s, (unsigned)%s)", name, name);
        break;
    case TYPE_UINT16:
        printer.s("log.PrintF(\"%%u(0x%%x)\", (unsigned)%s, (unsigned)%s)", name, name);
        break;
    case TYPE_INT32:
        printer.s("log.PrintF(\"%%d(0x%%x)\", (int)%s, (unsigned)%s)", name, name);
        break;
    case TYPE_UINT32:
        printer.s("log.PrintF(\"%%u(0x%%x)\", (unsigned)%s, (unsigned)%s)", name, name);
        break;
    case TYPE_INT64:
        printer.s("log.PrintF(\"%%" PRId64 "(0x%%" PRIx64 ")\", %s, %s)", name, name);
        break;
    case TYPE_UINT64:
        printer.s("log.PrintF(\"%%" PRIu64 "(0x%%" PRIx64 ")\", %s, %s)", name, name);
        break;
    case TYPE_FLOAT:
        printer.s("log.PrintF(\"%%f\", (double)%s)", name);
        break;
    case TYPE_DOUBLE:
        printer.s("log.PrintF(\"%%f\", %s)", name);
        break;
    case TYPE_STRING:
        printer.s("log.PrintF(\"\\\"%%s\\\"\", string(%s))", name);
        break;
    default:
        assert(0);
        return;
    }
}

void GoLang::print_struct_serial(GoPrinter &printer, ptr<StructItemTree> tree, const char *name, bool is_pointer) {
    if (is_pointer) {
        printer.if_("!%s.Serial(p *rpc.Protocol)", name); {
            printer.s("return false");
        }
        printer.end();
    }
    else {
        printer.if_("!%s.Serial(p *rpc.Protocol)", name); {
            printer.s("return false");
        }
        printer.end();
    }
}

void GoLang::print_struct_unserial(GoPrinter &printer, ptr<StructItemTree> tree, const char *name, bool is_pointer) {
    if (is_pointer) {
        printer.if_("!%s.Unserial(p *rpc.Protocol)", name); {
            printer.s("return false");
        }
        printer.end();
    }
    else {
        printer.if_("!%s.Unserial(p *rpc.Protocol)", name); {
            printer.s("return false");
        }
        printer.end();
    }
}

void GoLang::print_struct_dump(GoPrinter &printer, ptr<StructItemTree> tree, const char *name, bool is_pointer) {
    printer.s("log.PrintF(\"{\\n\")");
    printer.s("++sscc_indent");
    if (is_pointer) {
        printer.s("SSCC_POINTER_GET(%s)->SSCC_DUMP_FUNC(sscc_indent SSCC_DUMP_PARAM)", name);
    }
    else {
        printer.s("%s.SSCC_DUMP_FUNC(sscc_indent SSCC_DUMP_PARAM)", name);
    }
    printer.s("--sscc_indent");
    print_indent(printer);
    printer.s("log.PrintF(\"}\")");
}

void GoLang::print_array_serial(GoPrinter &printer, ptr<StructItemTree> tree, const char *name) {
    printer.if_("!p.WriteUint32(len(%s))", name); {
        printer.s("return false");
    }
    printer.end();

    printer.for_("_, v := range %s", name); {
        if (tree->type()->type() == TYPE_STRUCT) {
            print_struct_serial(printer, tree, "v", false);
        }
        else {
            print_base_var_serial(printer, tree, "v");
        }
    }
    printer.end();
}

void GoLang::print_array_unserial(GoPrinter &printer, ptr<StructItemTree> tree, const char *name) {
    const char *sizeName = Pool::instance()->printf("%sSize", tree->name()->text());
    printer.s("%s := p.ReadUint32()", sizeName);
    printer.s("%s = make([]%s, %s)", name, sizeName, type_decl(tree->type(), false));
    printer.for_("i =: 0 ; i < %s; ++i", sizeName); {
        if (tree->type()->type() == TYPE_STRUCT) {
            print_struct_unserial(printer, tree, Pool::instance()->printf("%s[i]", name), false);
        }
        else {
            print_base_var_unserial(printer, tree, Pool::instance()->printf("%s[i]", name));
        }
    }
    printer.end();
}

void GoLang::print_array_dump(GoPrinter &printer, ptr<StructItemTree> tree, const char *name) {
    printer.s("log.PrintF(\"{\\n\")");
    printer.s("++sscc_indent");
    {
        printer.s("size_t sscc_i = 0");
        printer.for_("auto &sscc_obj : %s", name); {
            print_indent(printer);
            printer.s("log.PrintF(\"[%%lu] = \", sscc_i++)");
            name = "sscc_obj";
            if (tree->type()->type() == TYPE_STRUCT) {
                print_struct_dump(printer, tree, name, false);
            }
            else {
                print_base_var_dump(printer, tree, name);
            }
            printer.s("log.PrintF(\",\\n\")");
        }
        printer.end();
    }
    printer.s("--sscc_indent");
    print_indent(printer);
    printer.s("log.PrintF(\"}\")", name);
}

void GoLang::print_serial(GoPrinter &printer, ptr<StructTree> tree, bool is_req) {
    printer.function_("(this *%s) Serial(p *rpc.Protocol) bool", tree->name()->text()); {
        if (tree->inherited()) {
            const char* inherited = tree->inherited()->name()->text();
            if (strcmp(inherited, "INotify") != 0 && strcmp(inherited, "ISerial") != 0) {
                if (is_req) {
                    printer.s("p.WriteUint32(this.Id)");
                } else {
                    printer.s("p.WriteUint32(this.Rc)");
                }
            }
        }
        for (auto sym : *tree) {
            switch (sym->type()) {
            case TREE_STRUCT_ITEM:
                print_var_serial(printer, sym.cast<StructItemTree>(), nullptr, false);
                break;
            default:
                assert(0);
                break;
            }
        }
        printer.s("return true");
    }
    printer.end();
};

void GoLang::print_unserial(GoPrinter &printer, ptr<StructTree> tree, bool is_req) {
    printer.function_("(this *%s) Unserial(p *rpc.Protocol) bool" ,tree->name()->text()); {
        if (tree->inherited()) {
            const char* inherited = tree->inherited()->name()->text();
            if (strcmp(inherited, "INotify") != 0 && strcmp(inherited, "ISerial") != 0) {
                if (is_req) {
                    printer.s("this.Id = p.ReadUint32()");
                } else {
                    printer.s("this.Rc = p.ReadUint32()");
                }
            }
        }
        for (auto sym : *tree) {
            switch (sym->type()) {
            case TREE_STRUCT_ITEM:
                print_var_unserial(printer, sym.cast<StructItemTree>(), nullptr, false);
                break;
            default:
                assert(0);
                break;
            }
        }
        printer.s("return true");
    }
    printer.end();
};

void GoLang::print_constructor(GoPrinter &printer, ptr<StructTree> tree) {
    bool first = true;
    printer.println("%s(SSCC_ALLOCATOR_PARAM_DECL)", tree->name()->text());
    if (tree->inherited()) {
        printer.print(": %s(SSCC_ALLOCATOR_PARAM)", tree->inherited()->name()->text());
        first = false;
    }

    ptr<StructItemTree> item;
    for (auto sym : *tree) {
        switch (sym->type()) {
        case TREE_STRUCT_ITEM:
            item = sym.cast<StructItemTree>();
            if (first) {
                first = false;
            }
            else {
                printer.println(",");
            }

            if (item->array()) {
                printer.print("  %s(%s::allocator_type(SSCC_ALLOCATOR_PARAM))", item->name()->text(), type_decl(item, false));
                break;
            }
            if (item->type()->type() == TYPE_STRUCT) {
                printer.print("  %s(SSCC_ALLOCATOR_PARAM)", item->name()->text());
                break;
            }
            if (item->type()->type() == TYPE_STRING) {
                printer.print("  %s(%s::allocator_type(SSCC_ALLOCATOR_PARAM))", item->name()->text(), type_decl(item, false));
                break;
            }
            printer.print("  %s()", item->name()->text());
            break;
        default:
            break;
        }
    }

    printer.println("");
    printer.println("{ }");
};

void GoLang::print_indent(GoPrinter &printer) {
    printer.s("SSCC_PRINT_INDENT(sscc_indent)");
}

void GoLang::print_dump(GoPrinter &printer, ptr<StructTree> tree) {
    printer.p("#ifdef SSCC_USE_DUMP");
    printer.function_("void SSCC_DUMP_FUNC(unsigned sscc_indent SSCC_DUMP_PARAM_DECL) override"); {
        if (tree->inherited()) {
            printer.s("%s::SSCC_DUMP_FUNC(sscc_indent SSCC_DUMP_PARAM)", tree->inherited()->name()->text());
        }

        for (auto sym : *tree) {
            switch (sym->type()) {
            case TREE_STRUCT_ITEM:
                print_var_dump(printer, sym.cast<StructItemTree>(), nullptr, false);
                break;
            default:
                assert(0);
                break;
            }
            printer.s("log.PrintF(\",\\n\")");
        }
    }
    printer.end();

    printer.function_("void SSCC_DUMP_FUNC(const char *sscc_name, unsigned sscc_indent SSCC_DUMP_PARAM_DECL) override"); {
        printer.if_("!sscc_name"); {
            printer.s("sscc_name = the_class_name");
        }
        printer.end();
        print_indent(printer);
        printer.s("log.PrintF(\"%%s = {\\n\", sscc_name)");
        printer.s("SSCC_DUMP_FUNC(sscc_indent + 1 SSCC_DUMP_PARAM)");
        print_indent(printer);
        printer.s("log.PrintF(\"}\\n\")");
    }
    printer.end();
    printer.p("#endif");
}

void GoLang::print_struct(GoPrinter &printer, ptr<StructTree> tree, bool is_req) {
    const char *inherited = tree->inherited() ? tree->inherited()->name()->text() : nullptr;
    printer.struct_(tree->name()->text(), inherited, is_req); {
        for (auto sym : *tree) {
            switch (sym->type()) {
            case TREE_STRUCT_ITEM:
                print_var(printer, sym.cast<StructItemTree>(), false);
                break;
            default:
                assert(0);
                break;
            }
        }

    }
    printer.end();

    //printer.println("");
    //print_constructor(printer, tree);
    printer.println("");
    print_serial(printer, tree, is_req);
    printer.println("");
    print_unserial(printer, tree, is_req);
    printer.println("");
    print_message_id(printer, tree);
    //print_dump(printer, tree);
    printer.println("");
}

void GoLang::print_message_id(GoPrinter &printer, ptr<StructTree> tree) {
    if (tree->message) {
        printer.function_("(this *%s) MessageId() uint32", tree->name()->text()); {
            printer.s("return %s", tree->message->id()->name()->text());
        }
        printer.end();
    }
}

void GoLang::print_message(GoPrinter &printer, ptr<MessageTree> tree) {
    print_struct(printer, tree->req(), true);
    if (tree->rsp()) {
        print_struct(printer, tree->rsp(), false);
    }

    printer.struct_(tree->name()->text(), nullptr); {
        printer.s("Req *%s", tree->req()->name()->text());
        if (tree->rsp()) {
            printer.s("Rsp *%s", tree->rsp()->name()->text());
        }

    }
    printer.end();

    printer.function_("(this *%s) MessageId() uint32", tree->name()->text()); {
        printer.s("return %s", tree->id()->name()->text());
    }
    printer.end();

    printer.function_("(this *%s) CreateRequest() bool", tree->name()->text()); {
        printer.s("this.Req = new(%s)", tree->req()->name()->text());
        printer.s("return true");
    }
    printer.end();

    printer.function_("(this *%s) SerialRequset(p *rpc.Protocol) bool", tree->name()->text()); {
        printer.s("this.Req.Serial(p)");
        printer.s("return true");
    }
    printer.end();

    printer.function_("(this *%s) UnserialRequset(p *rpc.Protocol) bool", tree->name()->text()); {
        printer.s("this.Req.Unserial(p)");
        printer.s("return true");
    }
    printer.end();

    printer.function_("(this *%s) CreateResponse() bool", tree->name()->text()); {
        if (tree->rsp())
            printer.s("this.Rsp = new(%s)", tree->rsp()->name()->text());
        printer.s("return true");
    }
    printer.end();

    printer.function_("(this *%s) SerialResponse(p *rpc.Protocol) bool", tree->name()->text()); {
        if (tree->rsp())
            printer.s("this.Rsp.Serial(p)");
        printer.s("return true");
    }
    printer.end();

    printer.function_("(this *%s) UnserialResponse(p *rpc.Protocol) bool", tree->name()->text()); {
        if (tree->rsp())
            printer.s("this.Rsp.Unserial(p)");
        printer.s("return true");
    }
    printer.end();
}

void GoLang::print_include(GoPrinter &printer, ptr<IncludeTree> tree) {
    //printer.p("import \"%s\"", (tree->path()->directory() + tree->path()->basename()).c_str());
}

void GoLang::print(SymbolTable &symbols, FILE *file) {
    GoPrinter printer;
    printer.open(file);

    printer.p("package message");
    printer.println("");
    printer.p("import \"rpc\"");

    printer.p(head().str().c_str());
    for (auto sym : symbols) {
        switch (sym->type()) {
        case TREE_DEFINE:
            print_define(printer, sym.cast<DefineTree>());
            break;
        case TREE_STRUCT:
            print_struct(printer, sym.cast<StructTree>());
            break;
        case TREE_MESSAGE:
            print_message(printer, sym.cast<MessageTree>());
            break;
        case TREE_INCLUDE:
            print_include(printer, sym.cast<IncludeTree>());
            break;
        default:
            assert(0);
            break;
        }
    }
    printer.p(tail().str().c_str());
}

void GoLang::print_var_serial(GoPrinter &printer, ptr<StructItemTree> tree, const char *prefix, bool is_pointer) {
    char tmp[256];
    char *name = tmp;
    strcpy(name, tree->name()->text());
    char x = toupper(name[0]);
    name[0] = x;
    if (prefix) {
        name = Pool::instance()->printf("this.%s.%s", prefix, name);
    }
    else {
        name = Pool::instance()->printf("this.%s", name);
    }
    if (tree->array()) {
        print_array_serial(printer, tree, name);
        return;
    }
    if (tree->type()->type() == TYPE_STRUCT) {
        print_struct_serial(printer, tree, name, is_pointer);
        return;
    }
    print_base_var_serial(printer, tree, name);
}

void GoLang::print_var_unserial(GoPrinter &printer, ptr<StructItemTree> tree, const char *prefix, bool is_pointer) {
    char tmp[256];
    char *name = tmp;
    strcpy(name, tree->name()->text());
    char x = toupper(name[0]);
    name[0] = x;
    if (prefix) {
        name = Pool::instance()->printf("%s.%s", prefix, name);
    }
    else {
        name = Pool::instance()->printf("this.%s", name);
    }
    if (tree->array()) {
        print_array_unserial(printer, tree, name);
        return;
    }
    if (tree->type()->type() == TYPE_STRUCT) {
        print_struct_unserial(printer, tree, name, is_pointer);
        return;
    }
    print_base_var_unserial(printer, tree, name);
}

void GoLang::print_var_dump(GoPrinter &printer, ptr<StructItemTree> tree, const char *prefix, bool is_pointer) {
    const char *name = tree->name()->text();
    print_indent(printer);
    printer.s("SSCC_PRINT(\"%s = \")", name);
    if (prefix) {
        name = Pool::instance()->printf("this.%s.%s", prefix, name);
    }
    else {
        name = Pool::instance()->printf("this.%s", name);
    }
    if (tree->array()) {
        print_array_dump(printer, tree, name);
    }
    else if (tree->type()->type() == TYPE_STRUCT) {
        print_struct_dump(printer, tree, name, is_pointer);
    }
    else {
        print_base_var_dump(printer, tree, name);
    }
}

