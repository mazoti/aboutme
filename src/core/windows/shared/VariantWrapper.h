#pragma once
#include <oleauto.h>

struct VariantWrapper {
	VariantWrapper()       { VariantInit(&var); }
	~VariantWrapper()      { VariantClear(&var); }
	VARIANT* operator&()   { return &var; }
	VARIANT var;
};
