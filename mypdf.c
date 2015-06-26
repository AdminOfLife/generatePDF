#include "mypdf.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char * headerString()
{
	return "%PDF-1.1\n";
}

char * commentString(char * comment)
{
	char * str = (char *)malloc(1000);
	strcpy(str, "%");
	strcat(str, comment);
	return str;
}

char * stringToString(STRING s)
{
	char * str = (char *)malloc(1000);
	strcpy(str, "(");
	strcat(str, s.str);
	strcat(str, ")");
	return str;
}

char * nameToString(NAME n)
{
	char * str = (char *)malloc(1000);
	strcpy(str, "/");
	strcat(str, n.name);
	return str;
}

char * arrayToString(ARRAY arr)
{
	char * str = (char *)malloc(1000);
	strcpy(str, "[");
	int len = arr.len;
	VALUE ** items = arr.items;
	int i;
	for(i=0;i<len;++i)
	{
		strcat(str, valueToString(*(items[i])));
		strcat(str, " ");
	}
	strcat(str, "]");
	return str;
}

char * dictToString(DICT d)
{
	char * str = (char *)malloc(1000);
	strcpy(str, "<<");
	int len = d.len;
	NAME * keys = d.keys;
	VALUE ** values = d.values;
	int i;
	for(i=0;i<len;++i)
	{
		strcat(str, nameToString(keys[i]));
		strcat(str, " ");
		strcat(str, valueToString(*(values[i])));
		strcat(str, "\n");
	}
	strcat(str, ">>");
	return str;	
}

char * indirectToString(INDIRECT r)
{
	char * str = (char *)malloc(1000);
	sprintf(str, "%d %d R", r.objectNumber, r.versionNumber);
	return str;
}

char * valueToString(VALUE v)
{
	char * str = (char *)malloc(1000);
	switch(v.k)
	{
		case PDF_INT: 
			sprintf(str, "%d", v.v.i);
			break;
		case PDF_REAL:
			sprintf(str, "%f", v.v.f);
			break;
		case PDF_STRING:
			strcpy(str, stringToString(v.v.s));
			break;
		case PDF_NAME:
			strcpy(str, nameToString(v.v.n));
			break;
		case PDF_ARRAY:
			strcpy(str, arrayToString(v.v.a));
			break;
		case PDF_DICT:
			strcpy(str, dictToString(v.v.d));
			break;
		case PDF_INDIRECT:
			strcpy(str, indirectToString(v.v.r));
			break;
		default:
			strcpy(str, "");
			break;
	}
	return str;
}

char * objToString(OBJ o)
{
	char * str = (char *)malloc(1000);
	sprintf(str, "%d %d obj\n%s\nendobj\n", o.objectNumber, o.versionNumber, valueToString(*(o.value)));
	return str;
}


/* constructor, add functions */


OBJ createOBJ(int versionNumber, VALUE * value)
{
	static int objectNumber = 1;
	OBJ newOBJ;
	newOBJ.objectNumber = objectNumber;
	newOBJ.versionNumber = versionNumber;
	newOBJ.value = value;

	objectNumber++;
	return newOBJ;
}

ARRAY * initARRAY()
{
	ARRAY * newARRAY = (ARRAY *)malloc(sizeof(ARRAY));
	newARRAY->len = 0;
	return newARRAY;
}

void addToARRAY(ARRAY * arr, VALUE * item)
{
	int len = arr->len;
	arr->items[len] = item;
	arr->len = arr->len + 1;
}

DICT * initDICT()
{
	DICT * newDICT = (DICT *)malloc(sizeof(DICT));
	newDICT->len = 0;
	return newDICT;
}

void addToDICT(DICT * d, NAME key, VALUE * value)
{
	//TODO:Add capacity check
	int len = d->len;
	d->keys[len] = key;
	d->values[len] = value;
	d->len = d->len + 1;
}

NAME createNAME(char * name)
{
	NAME newNAME;
	newNAME.name = (char *)malloc(strlen(name) + 1);
	strcpy(newNAME.name, name);
	return newNAME;
}

STRING createSTRING(char * str)
{
	STRING newSTRING;
	newSTRING.str = (char *)malloc(strlen(str) + 1);
	strcpy(newSTRING.str, str);
	return newSTRING;
}

INDIRECT createINDIRECT(int objectNumber, int versionNumber)
{
	INDIRECT newINDIRECT;
	newINDIRECT.objectNumber = objectNumber;
	newINDIRECT.versionNumber = versionNumber;
	return newINDIRECT;
}

VALUE * createIntVALUE(int i)
{
	VALUE * newVALUE = (VALUE *)malloc(sizeof(VALUE));
	newVALUE->k = PDF_INT;
	newVALUE->v.i = i;
	return newVALUE;
}

VALUE * createFloatVALUE(float f)
{
	VALUE * newVALUE = (VALUE *)malloc(sizeof(VALUE));
	newVALUE->k = PDF_REAL;
	newVALUE->v.f = f;
	return newVALUE;
}

VALUE * createStringVALUE(STRING s)
{
	VALUE * newVALUE = (VALUE *)malloc(sizeof(VALUE));
	newVALUE->k = PDF_STRING;
	newVALUE->v.s = s;
	return newVALUE;
}

VALUE * createNAMEVALUE(NAME n)
{
	VALUE * newVALUE = (VALUE *)malloc(sizeof(VALUE));
	newVALUE->k = PDF_NAME;
	newVALUE->v.n = n;
	return newVALUE;
}

VALUE * createARRAYVALUE(ARRAY a)
{
	VALUE * newVALUE = (VALUE *)malloc(sizeof(VALUE));
	newVALUE->k = PDF_ARRAY;
	newVALUE->v.a = a;
	return newVALUE;
}

VALUE * createDICTVALUE(DICT d)
{
	VALUE * newVALUE = (VALUE *)malloc(sizeof(VALUE));
	newVALUE->k = PDF_DICT;
	newVALUE->v.d = d;
	return newVALUE;
}

VALUE * createINDIRECTVALUE(INDIRECT r)
{
	VALUE * newVALUE = (VALUE *)malloc(sizeof(VALUE));
	newVALUE->k = PDF_INDIRECT;
	newVALUE->v.r = r;
	return newVALUE;
}

char * rootObject()
{
	NAME type = createNAME("Type");
	NAME _catalog = createNAME("Catalog");
	VALUE * catalog = createNAMEVALUE(_catalog);

	NAME pages = createNAME("Pages");
	INDIRECT _r = createINDIRECT(2, 0);
	VALUE * r = createINDIRECTVALUE(_r);

	DICT * _d = initDICT();
	addToDICT(_d, type, catalog);
	addToDICT(_d, pages, r);
	VALUE * d = createDICTVALUE(*_d);

	OBJ rootOBJ = createOBJ(0, d);
	return objToString(rootOBJ);
}

char * pagesObject()
{
	NAME type = createNAME("Type");
	NAME _pages = createNAME("Pages");
	VALUE * pages = createNAMEVALUE(_pages);

	NAME kids = createNAME("Kids");
	INDIRECT _r1 = createINDIRECT(3, 0);
	VALUE * r1 = createINDIRECTVALUE(_r1);
	INDIRECT _r2 = createINDIRECT(5, 0);
	VALUE * r2 = createINDIRECTVALUE(_r2);
	ARRAY * arr = initARRAY();
	addToARRAY(arr, r1);
	addToARRAY(arr, r2);
	VALUE * v1 = createARRAYVALUE(*arr);

	NAME count = createNAME("Count");
	VALUE * v2 = createIntVALUE(2);

	NAME mediaBox = createNAME("MediaBox");
	arr = initARRAY();
	VALUE * i = createIntVALUE(0);
	addToARRAY(arr, i);		///MediaBox [0 0 300 144]
	i = createIntVALUE(0);
	addToARRAY(arr, i);
	i = createIntVALUE(300);
	addToARRAY(arr, i);
	i = createIntVALUE(144);
	addToARRAY(arr, i);
	VALUE * v3 = createARRAYVALUE(*arr);

	DICT * _d = initDICT();
	addToDICT(_d, type, pages);
	addToDICT(_d, kids, v1);
	addToDICT(_d, count, v2);
	addToDICT(_d, mediaBox, v3);
	VALUE * d = createDICTVALUE(*_d);

	OBJ pagesOBJ = createOBJ(0, d);
	return objToString(pagesOBJ);
}

char * pageObject()
{
	NAME type = createNAME("Type");
	// fprintf(stderr, "size is %lu\n", sizeof(type));
	NAME _page = createNAME("Page");
	// fprintf(stderr, "size is %lu\n", sizeof(_page));
	VALUE * page = createNAMEVALUE(_page);
	// fprintf(stderr, "size is %lu\n", sizeof(*page));

	NAME parent = createNAME("Parent");
	INDIRECT _r1 = createINDIRECT(2, 0);
	VALUE * v1 = createINDIRECTVALUE(_r1);
	
	NAME contents = createNAME("Contents");
	INDIRECT _r2 = createINDIRECT(4, 0);
	VALUE * v2 = createINDIRECTVALUE(_r2);

    NAME resources = createNAME("Resources");
    NAME _font = createNAME("Font");
    VALUE * font = createNAMEVALUE(_font);
    NAME f1 = createNAME("F1");
    NAME subtype = createNAME("Subtype");
    NAME _type1 = createNAME("Type1");
    VALUE * type1 = createNAMEVALUE(_type1);
    NAME baseFont = createNAME("BaseFont");
    NAME _times = createNAME("Times-Roman");
    VALUE * times = createNAMEVALUE(_times);
    DICT * _d1 = initDICT();
    addToDICT(_d1, type, font);
    addToDICT(_d1, subtype, type1);
    addToDICT(_d1, baseFont, times);
    VALUE * d1 = createDICTVALUE(*_d1);

    DICT * _d2 = initDICT();
    addToDICT(_d2, f1, d1);
    VALUE * d2 = createDICTVALUE(*_d2);

    DICT * _d3 = initDICT();
    addToDICT(_d3, _font, d2);
    VALUE * d3 = createDICTVALUE(*_d3);

	DICT * _d = initDICT();
	addToDICT(_d, type, page);
	addToDICT(_d, parent, v1);
	addToDICT(_d, contents, v2);
	addToDICT(_d, resources, d3);
	VALUE * d = createDICTVALUE(*_d);
	// fprintf(stderr, "size is %lu\n", sizeof(*d));

	OBJ pageOBJ = createOBJ(0, d);
	// fprintf(stderr, "size is %lu\n", sizeof(pageOBJ));
	return objToString(pageOBJ);
}

char * pageObject2()
{
	NAME type = createNAME("Type");
	NAME _page = createNAME("Page");
	VALUE * page = createNAMEVALUE(_page);

	NAME parent = createNAME("Parent");
	INDIRECT _r1 = createINDIRECT(2, 0);
	VALUE * v1 = createINDIRECTVALUE(_r1);
	
	NAME contents = createNAME("Contents");
	INDIRECT _r2 = createINDIRECT(6, 0);
	VALUE * v2 = createINDIRECTVALUE(_r2);

    NAME resources = createNAME("Resources");
    NAME _font = createNAME("Font");
    VALUE * font = createNAMEVALUE(_font);
    NAME f1 = createNAME("F1");
    NAME subtype = createNAME("Subtype");
    NAME _type1 = createNAME("Type1");
    VALUE * type1 = createNAMEVALUE(_type1);
    NAME baseFont = createNAME("BaseFont");
    NAME _times = createNAME("Times-Italic");
    VALUE * times = createNAMEVALUE(_times);
    DICT * _d1 = initDICT();
    addToDICT(_d1, type, font);
    addToDICT(_d1, subtype, type1);
    addToDICT(_d1, baseFont, times);
    VALUE * d1 = createDICTVALUE(*_d1);

    DICT * _d2 = initDICT();
    addToDICT(_d2, f1, d1);
    VALUE * d2 = createDICTVALUE(*_d2);

    DICT * _d3 = initDICT();
    addToDICT(_d3, _font, d2);
    VALUE * d3 = createDICTVALUE(*_d3);

	DICT * _d = initDICT();
	addToDICT(_d, type, page);
	addToDICT(_d, parent, v1);
	addToDICT(_d, contents, v2);
	addToDICT(_d, resources, d3);
	VALUE * d = createDICTVALUE(*_d);

	OBJ pageOBJ = createOBJ(0, d);
	return objToString(pageOBJ);
}

char * streamObject()
{
	//TODO:Create struct for stream obj including starting dict
	char * str = (char *)malloc(1000);
	char temp[100];
	strcpy(temp, "BT\n/F1 18 Tf\n0 0 Td\n(Hello World) Tj\nET");
	int streamLen = strlen(temp);
	char lenStr[10];sprintf(lenStr, "%d", streamLen);
	strcpy(str, "4 0 obj\n<</Length ");
	strcat(str, lenStr);
	strcat(str, ">>\nstream\n");
	strcat(str, temp);
	strcat(str, "\nendstream\nendobj\n");
	return str;
}

char * streamObject2()
{
	//TODO:Create struct for stream obj including starting dict
	char * str = (char *)malloc(1000);
	char temp[100];
	strcpy(temp, "BT\n/F1 18 Tf\n0 0 Td\n(Manav Sethi IIT KGP) Tj\nET");
	int streamLen = strlen(temp);
	char lenStr[10];sprintf(lenStr, "%d", streamLen);
	strcpy(str, "6 0 obj\n<</Length ");
	strcat(str, lenStr);
	strcat(str, ">>\nstream\n");
	strcat(str, temp);
	strcat(str, "\nendstream\nendobj\n");
	return str;
}

XREF * initXREF()
{
	XREF * xref = (XREF *)malloc(sizeof(XREF));
	xref->startObjectIndex = 0;
	xref->numObjects = 0;
	return xref;
}

// XREFENTRY createXREFENTRY(int address, int genNumber, char m)
// {
// 	XREFENTRY e;
// 	sprintf(e.address, "%010d", address);
// 	sprintf(e.genNumber, "%05d", genNumber);
// 	e.m = m;
// }

void addEntryXREF(XREF * xref, int address, int genNumber, char m)
{
	int len = xref->numObjects;
	char temp[21];
	sprintf(temp, "%010d %05d %c ", address, genNumber, m);
	strcpy(xref->entries[len], temp);
	xref->numObjects++;
}

char * xrefToString(XREF * xref)
{
	char * str = (char *)malloc(1000);
	strcpy(str, "xref\n");
	char temp[20];
	sprintf(temp, "%d %d\n", xref->startObjectIndex, xref->numObjects);
	strcat(str, temp);

	int i;
	for(i=0;i<xref->numObjects;++i)
	{
		strcat(str, xref->entries[i]);
		strcat(str, "\n");
	}

	strcat(str, "trailer\n<</Root 1 0 R\n/Size ");
	sprintf(temp, "%d", xref->numObjects);
	strcat(str, temp);
	strcat(str, "\n>>\nstartxref\n");
	sprintf(temp, "%d", xref->xrefStartAddress);
	strcat(str, temp);
	strcat(str, "\n%%EOF");
	return str;
}

int main()
{
	char str[10000];
	strcat(str, headerString());
	int size1 = strlen(str);
	strcat(str, rootObject());
	int size2 = strlen(str);
	strcat(str, pagesObject());
	int size3 = strlen(str);
	strcat(str, pageObject());
	int size4 = strlen(str);
	strcat(str, streamObject());
	int size5 = strlen(str);
	//Dummy obj for stream
	VALUE * a1_01 = createIntVALUE(100);
	OBJ dummyOBJ = createOBJ(0, a1_01);
	////////////////////////
	strcat(str, pageObject2());
	int size6 = strlen(str);
	strcat(str, streamObject2());
	int xrefStartAddress = strlen(str);

	XREF * xref = initXREF();
	addEntryXREF(xref, 0, 65535, 'f');//Special entry
	addEntryXREF(xref, size1, 0, 'n');
	addEntryXREF(xref, size2, 0, 'n');
	addEntryXREF(xref, size3, 0, 'n');
	addEntryXREF(xref, size4, 0, 'n');
	addEntryXREF(xref, size5, 0, 'n');
	addEntryXREF(xref, size6, 0, 'n');
	xref->xrefStartAddress = xrefStartAddress;

	strcat(str, xrefToString(xref));

	printf("%s\n", str);

	return 0;
}