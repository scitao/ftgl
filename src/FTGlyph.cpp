#include	"FTGlyph.h"


// OPSignature:  FTGlyph:FTGlyph( FT_Face:face  int:glyphIndex ) 
FTGlyph::FTGlyph( int gi)
:	advance(0),
	glyphIndex(gi),
	ftGlyph(0)
{
	pos.x = 0;
	pos.y = 0;
}


// OPSignature:  FTGlyph:~FTGlyph() 
FTGlyph::~FTGlyph()
{
	FT_Done_Glyph(ftGlyph); // Is this OK?
}