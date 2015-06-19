typedef struct 
{
	// char pdf[5];//%PDF-
	// char dot[1];//.
	int versionX, versionY;//PDF-X.Y
}HEADER;


typedef enum
{
	PDF_INT = 'i',
	PDF_REAL = 'f',
	PDF_STRING = 's',
	PDF_NAME = 'n',
	PDF_ARRAY = 'a',
	PDF_DICT = 'd',
	PDF_INDIRECT = 'r'
}OBJKIND;

typedef struct
{
	// char slash[1];//'/name'	
	char name[100];
}NAME;

typedef struct _OBJ
{
	int objectNumber;
	int versionNumber;
	// char obj[3];//obj
	struct _VALUE * value;
}OBJ;

typedef struct _STRING
{
	char * str;
}STRING;

typedef struct
{
	int objectNumber;
	int versionNumber;
	// char R[1];
}INDIRECT;

typedef struct _ARRAY
{
	int len;
	struct _VALUE * items[100];
}ARRAY;

typedef struct _DICT
{
	int len;
	NAME keys[100];
	struct _VALUE * values[100];
}DICT;

// typedef struct _STREAM
// {

// }STREAM;

typedef struct _VALUE
{
	OBJKIND k;
	union
	{
		int i;
		float f;
		STRING s;
		NAME n;
		ARRAY a;
		DICT d;
		INDIRECT r;
	}v;
}VALUE;

// typedef struct _XREFENTRY
// {
// 	char address[11];//10 byte integer
// 	char genNumber[6];//6 byte integer
// 	char m;//n or f
// 	//2 bytes for eol
// }XREFENTRY;

typedef struct _XREF
{
	int startObjectIndex;
	int numObjects;
	char entries[100][21];
	int xrefStartAddress;
}XREF;

char * headerString();
char * commentString(char *);
char * nameToString(NAME);
char * stringToString(STRING);
char * arrayToString(ARRAY);
char * dictToString(DICT);
char * indirectToString(INDIRECT);
char * valueToString(VALUE);
char * objToString(OBJ);
