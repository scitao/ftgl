#include	"FTGlyphContainer.h"
#include	"FTGlyph.h"


FTGlyphContainer::FTGlyphContainer( FT_Face* f, int g, bool p)
:	preCache( p),
	numGlyphs(g),
	tempGlyph(0),
	face(f)
{
	glyphs.reserve( g);
}


// OPSignature:  FTGlyphContainer:~FTGlyphContainer() 
FTGlyphContainer::~FTGlyphContainer()
{
	vector<FTGlyph*>::iterator iter;
	for( iter = glyphs.begin(); iter != glyphs.end(); ++iter)
	{
		delete *iter;
	}
	
	glyphs.clear();
}


bool FTGlyphContainer::Add( FTGlyph* tempGlyph)
{
	glyphs.push_back( tempGlyph);
}


// OPSignature: bool FTGlyphContainer:render( int:index ) 
FT_Vector& FTGlyphContainer::render( int index, int next, FT_Vector pen)
{
	FT_Vector kernAdvance;
	float advance;
	
	kernAdvance.x = 0; kernAdvance.y = 0;
	
	if( index > glyphs.size())
		return kernAdvance;
	
	if( 0 < next <=  glyphs.size())
	{
		FT_Error err = FT_Get_Kerning( *face, glyphs[index]->glyphIndex, glyphs[next]->glyphIndex, ft_kerning_default, &kernAdvance);
	}
	
	advance = glyphs[index]->Render( pen);
	
	kernAdvance.x = advance + kernAdvance.x;
//	kernAdvance.y = advance.y;
	return kernAdvance;
}