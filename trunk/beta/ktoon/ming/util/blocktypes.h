
#ifndef SWF_BLOCKTYPES_H_INCLUDED
#define SWF_BLOCKTYPES_H_INCLUDED

extern const char *blockName(int header);

typedef enum
{
  DEFINESHAPE			=  2,
  DEFINESHAPE2		        = 22,
  DEFINESHAPE3		        = 32,
  DEFINEMORPHSHAPE	        = 46,
  DEFINEBITS			=  6,
  DEFINEBITSJPEG2		= 21,
  DEFINEBITSJPEG3		= 35,
  DEFINELOSSLESS		= 20,
  DEFINELOSSLESS2		= 36,
  JPEGTABLES			=  8,
  DEFINEBUTTON		        =  7,
  DEFINEBUTTON2		        = 34,
  DEFINEBUTTONCXFORM	        = 23,
  DEFINEBUTTONSOUND	        = 17,
  DEFINEFONT			= 10,
  DEFINEFONT2			= 48,
  DEFINEFONTINFO		= 13,
  DEFINETEXT			= 11,
  DEFINETEXT2			= 33,
  DEFINESOUND			= 14,
  SOUNDSTREAMBLOCK	        = 19,
  SOUNDSTREAMHEAD		= 18,
  SOUNDSTREAMHEAD2	        = 45,
  DEFINESPRITE		        = 39,
  PLACEOBJECT			=  4,
  PLACEOBJECT2		        = 26,
  REMOVEOBJECT		        =  5,
  REMOVEOBJECT2		        = 28,
  SHOWFRAME			=  1,
  SETBACKGROUNDCOLOR	        =  9,
  FRAMELABEL			= 43,
  PROTECT			= 24,
  STARTSOUND			= 15,
  END				=  0,
  DOACTION			= 12,
  TEXTFIELD                     = 37,
  LIBRARYSYMBOL                 = 56,
  IMPORTASSETS            	= 57,
  PASSWORD                      = 58
} Blocktype;

#endif /* SWF_BLOCKTYPES_H_INCLUDED */
