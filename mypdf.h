typedef struct 
{
	// char pdf[5];//%PDF-
	// char dot[1];//.
	int versionX, versionY;//PDF-X.Y
}HEADER;


typedef enum
{
	PDF_INT,
	PDF_REAL,
	PDF_STRING,
	PDF_NAME,
	PDF_ARRAY,
	PDF_DICT,
	PDF_INDIRECT
}OBJKIND;

typedef struct
{
	// char slash[1];//'/name'	
	char * name;
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
	struct _VALUE * items[1000];
}ARRAY;

typedef struct _DICT
{
	int len;
	NAME keys[1000];
	struct _VALUE * values[1000];
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
	char entries[1000][21];
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
