#include "FTPolyGlyph.h"
#include "FTVectoriser.h"


FTPolyGlyph::FTPolyGlyph( FT_GlyphSlot glyph)
:   FTGlyph( glyph),
    glList(0)
{
    if( ft_glyph_format_outline != glyph->format)
    {
        err = 0x14; // Invalid_Outline
        return;
    }

    FTVectoriser vectoriser( glyph);

    if(( vectoriser.ContourCount() < 1) || ( vectoriser.PointCount() < 3))
    {
        return;
    }
    
    unsigned int horizontalTextureScale = glyph->face->size->metrics.x_ppem * 64;
    unsigned int verticalTextureScale = glyph->face->size->metrics.y_ppem * 64;        
        
    vectoriser.MakeMesh( 1.0);
    
    glList = glGenLists( 1);
    glNewList( glList, GL_COMPILE);

        const FTMesh* mesh = vectoriser.GetMesh();
        for( unsigned int index = 0; index < mesh->TesselationCount(); ++index)
        {
            const FTTesselation* subMesh = mesh->Tesselation( index);
            unsigned int polyonType = subMesh->PolygonType();

            glBegin( polyonType);
                for( unsigned int pointIndex = 0; pointIndex < subMesh->PointCount(); ++pointIndex)
                {
                    FTPoint point = subMesh->Point(pointIndex);
                    
                    glTexCoord2f( point.x / horizontalTextureScale,
                                  point.y / verticalTextureScale);
                    
                    glVertex3f( point.x / 64.0f,
                                point.y / 64.0f,
                                0.0f);
                }
            glEnd();
        }
    glEndList();
}


FTPolyGlyph::~FTPolyGlyph()
{
    glDeleteLists( glList, 1);
}


float FTPolyGlyph::Render( const FTPoint& pen)
{
    glTranslatef(  pen.x,  pen.y, 0);

    if( glList)
    {
        glCallList( glList);    
    }
    
    return advance;
}
