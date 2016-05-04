#include <cassert>
#include <string.h>
#include "go_printer.h"

void GoPrinter::s(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vprint(fmt, ap);
	println("");
}

void GoPrinter::p(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	unsigned old = indent;
	indent = 0;
	vprint(fmt, ap);
	println("");
	indent = old;
}

void GoPrinter::d(const char *name, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	unsigned old = indent;
	indent = 0;
	print("#define %s ", name);
	vprint(fmt, ap);
	println("");
	indent = old;
}

void GoPrinter::if_(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	print("if ");
	vprint(fmt, ap);
	println(" {");

	_blocks.push(BLOCK_IF);
	++indent;
}

void GoPrinter::else_() {
	assert(_blocks.top() == BLOCK_IF);
	end();
	println("else {");
	_blocks.push(BLOCK_ELSE);
	++indent;
}

void GoPrinter::else_if_(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	assert(_blocks.top() == BLOCK_IF);
	end();
	print("else if ");
	vprint(fmt, ap);
	println(" {");
	_blocks.push(BLOCK_IF);
	++indent;
}

void GoPrinter::for_(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	print("for ");
	vprint(fmt, ap);
	println(" {");
	_blocks.push(BLOCK_FOR);
	++indent;
}

void GoPrinter::function_(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	print("func ");
	vprint(fmt, ap);
	println(" {");
	_blocks.push(BLOCK_FUNC);
	++indent;
}

void GoPrinter::struct_(const char *name, const char *inherited, bool is_req) {
	if (inherited) {
		println("type %s struct {", name);
        if (strcmp(inherited, "INotify") != 0 && strcmp(inherited, "ISerial") != 0) {
            ++indent;
            if (is_req) {
                println("Id uint32");
            } else {
                println("Rc uint32");
            }
            --indent;
        }
	}
	else {
		println("type %s struct {", name);
	}
	_blocks.push(BLOCK_STRUCT);
	++indent;
}

void GoPrinter::enum_() {
	println("const (");
	_blocks.push(BLOCK_ENUM);
	++indent;
}

void GoPrinter::const_(const char *name, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	unsigned old = indent;
	indent = 0;
	print("const %s = ", name);
	vprint(fmt, ap);
	println("");
	indent = old;
}

void GoPrinter::end() {
	assert(_blocks.size() > 0);
	--indent;
	switch (_blocks.top()) {
	case BLOCK_IF:
	case BLOCK_ELSE:
	case BLOCK_FOR:
	case BLOCK_FUNC:
	case BLOCK_STRUCT:
	case BLOCK_CLASS:
		println("}");
		break;
	case BLOCK_ENUM:
		println(")");
		break;
	default:
		assert(0);
	}
	_blocks.pop();
}

void GoPrinter::end(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	assert(_blocks.size() > 0);
	assert(_blocks.top() == BLOCK_DO);

	--indent;
	print("} for ");
	vprint(fmt, ap);
	println(" ");
	_blocks.pop();
}


